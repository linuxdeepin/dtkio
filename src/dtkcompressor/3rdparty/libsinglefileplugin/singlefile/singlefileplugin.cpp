/*
 * Copyright (c) 2009  Raphael Kubo da Costa <rakuco@FreeBSD.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES ( INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION ) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * ( INCLUDING NEGLIGENCE OR OTHERWISE ) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "singlefileplugin.h"
#include "queries.h"
#include "datamanager.h"

#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QDir>
#include <QThread>

#include <KFilterDev>
//#include <KLocalizedString>
#include <linux/limits.h>

#include <unistd.h>

LibSingleFileInterface::LibSingleFileInterface(QObject *parent, const QVariantList &args)
    : ReadOnlyArchiveInterface(parent, args)
{
}

LibSingleFileInterface::~LibSingleFileInterface()
{
}

PluginFinishType LibSingleFileInterface::list()
{
    DataManager::get_instance().resetArchiveData();
    ArchiveData &stArchiveData =  DataManager::get_instance().archiveData();

    FileEntry entry;
    entry.strFullPath = uncompressedFileName();
    entry.strFileName = entry.strFullPath;
    entry.qSize = QFileInfo(m_strArchiveName).size(); // ??????????????????????????????

    stArchiveData.qSize = entry.qSize;
    stArchiveData.qComressSize = entry.qSize;
    stArchiveData.listRootEntry.push_back(entry);
    stArchiveData.mapFileEntry[entry.strFullPath] = entry;

    return PFT_Nomral;
}

PluginFinishType LibSingleFileInterface::testArchive()
{
    m_workStatus = WT_Test;
    return PFT_Nomral;
}

PluginFinishType LibSingleFileInterface::extractFiles(const QList<FileEntry> &files, const ExtractionOptions &options)
{
    Q_UNUSED(files)

    QString strFileName = uncompressedFileName();       // ???????????????

    // ???????????????????????????????????????????????????????????????????????????????????????????????????xx.xx????????????????????????
    QString outputFileName = options.strTargetPath;
    if (outputFileName.endsWith(strFileName))
        outputFileName.chop(strFileName.length());

    // ????????????
    if (!outputFileName.endsWith(QLatin1Char('/'))) {
        outputFileName += QLatin1Char('/');
    }

    // ????????????????????????????????????????????????????????????
    if (QDir().exists(outputFileName) == false)
        QDir().mkpath(outputFileName);

    outputFileName += strFileName;   // ??????????????????

    // ??????????????????????????????????????????????????????
    if (!options.bExistList) {
        FileEntry entry;
        entry.strFullPath = strFileName;
        DataManager::get_instance().archiveData().listRootEntry << entry;
    }

    // ?????????????????????????????????
    QFile file(outputFileName);
    if (file.exists()) {
        m_bOverwriteAll = true;
    }

//    qInfo() << "Extracting to" << outputFileName;


    // ?????????
    QFile outputFile(outputFileName);
    if (!outputFile.open(QIODevice::WriteOnly)) {

        QList<QString> entryNameList = outputFileName.split('/');
        foreach (auto &tmp, entryNameList) {
            // ???????????????????????????
            if (NAME_MAX < tmp.toLocal8Bit().length()) {
                m_eErrorType = ET_LongNameError;
                return PFT_Error;
            }
        }

        emit signalFileWriteErrorName(QFileInfo(outputFile.fileName()).fileName());
        m_eErrorType = ET_FileWriteError;
        return PFT_Error;
    }

    // ?????????????????????????????????
    KCompressionDevice *device = new KCompressionDevice(m_strArchiveName, KFilterDev::compressionTypeForMimeType(m_mimeType));
    if (!device) {
        emit signalFileWriteErrorName(QFileInfo(outputFile.fileName()).fileName());
        m_eErrorType = ET_FileWriteError;
        return PFT_Error;
    }

    device->open(QIODevice::ReadOnly);  // ?????????????????????

    qint64 bytesRead;
    QByteArray dataChunk(1024 * 16, '\0'); // 16Kb
    m_currentExtractedFilesSize = 0; // ??????

    emit signalCurFileName(strFileName);

    // ?????????
    while (true) {
        if (QThread::currentThread()->isInterruptionRequested()) { // ????????????
            break;
        }

        if (m_bPause) { //????????????
            sleep(1);
            continue;
        }

        bytesRead = device->read(dataChunk.data(), dataChunk.size());

        // ?????????????????????
        m_currentExtractedFilesSize += bytesRead;
        emit signalprogress((double(m_currentExtractedFilesSize)) / options.qSize * 100); // ??????????????????????????????????????????????????????????????????

        if (-1 == bytesRead) {
            emit signalFileWriteErrorName(QFileInfo(outputFile.fileName()).fileName());
            m_eErrorType = ET_FileWriteError;
            break;
        } else if (0 == bytesRead) {
            break;
        }

        if (outputFile.write(dataChunk.data(), bytesRead) != bytesRead) {
            if (isInsufficientDiskSpace(options.strTargetPath, bytesRead)) { // ??????bytesRead???????????????????????????????????????
                m_eErrorType = ET_InsufficientDiskSpace;
            }
            break;
        }
    }

    outputFile.close();
    device->close();
    delete device;

    return ET_NoError == m_eErrorType ? PFT_Nomral : PFT_Error;
}

void LibSingleFileInterface::pauseOperation()
{
    m_bPause = true;
}

void LibSingleFileInterface::continueOperation()
{
    m_bPause = false;
}

bool LibSingleFileInterface::doKill()
{
    m_bPause = false;
    return false;
}

const QString LibSingleFileInterface::uncompressedFileName() const
{
    QString uncompressedName(QFileInfo(m_strArchiveName).fileName());

    // Bug 252701: For .svgz just remove the terminal "z".
    if (uncompressedName.endsWith(QLatin1String(".svgz"), Qt::CaseInsensitive)) {
        uncompressedName.chop(1);
        return uncompressedName;
    }

    for (const QString &extension : qAsConst(m_possibleExtensions)) {

        if (uncompressedName.endsWith(extension, Qt::CaseInsensitive)) {
            uncompressedName.chop(extension.size());
            return uncompressedName;
        }
    }

    return uncompressedName + QStringLiteral(".uncompressed");
}
