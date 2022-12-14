/*
 * ark -- archiver for the KDE project
 *
 * Copyright (C) 2009 Harald Hvaal <haraldhv@stud.ntnu.no>
 * Copyright (C) 2009-2011 Raphael Kubo da Costa <rakuco@FreeBSD.org>
 * Copyright (c) 2016 Vladyslav Batyrenko <mvlabat@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */
#include "libpigzplugin.h"
#include "datamanager.h"

#include <QDebug>
#include <QRegularExpression>
#include <QDir>
#include <QDateTime>
#include <QStandardPaths>
#include <linux/limits.h>

LibPigzPluginFactory::LibPigzPluginFactory()
{
    registerPlugin<LibPigzPlugin>();
}

LibPigzPluginFactory::~LibPigzPluginFactory()
{

}

LibPigzPlugin::LibPigzPlugin(QObject *parent, const QVariantList &args)
    : ReadWriteArchiveInterface(parent, args)
{
    setWaitForFinishedSignal(true);
    if (0 == QMetaType::type("QProcess::ExitStatus")) {
        qRegisterMetaType<QProcess::ExitStatus>("QProcess::ExitStatus");
    }
    m_ePlugintype = PT_Libpigz;
}

LibPigzPlugin::~LibPigzPlugin()
{
    deleteProcess();
}

PluginFinishType LibPigzPlugin::list()
{
    return PFT_Nomral;
}

PluginFinishType LibPigzPlugin::testArchive()
{
    return PFT_Nomral;
}

PluginFinishType LibPigzPlugin::extractFiles(const QList<FileEntry> &, const ExtractionOptions &)
{
    return PFT_Nomral;
}

PluginFinishType LibPigzPlugin::addFiles(const QList<FileEntry> &files, const CompressOptions &options)
{
    m_qTotalSize = options.qTotalSize;
    m_stdOutData.clear();
    m_isProcessKilled = false;

    // ????????????tar -cvf - filename | pigz -p 2 -5> filename.tar.gz
    m_process = new KPtyProcess;
    m_process->setPtyChannels(KPtyProcess::StdinChannel);
    m_process->setOutputChannelMode(KProcess::MergedChannels);
    m_process->setNextOpenMode(QIODevice::ReadWrite | QIODevice::Unbuffered | QIODevice::Text);

    QString strFileName;
    const QString oneSpace = " "; //????????????
    QVector<QString> strold = {" ", "!", "$", "&", "*", "(", ")", "<", ">", "+", "-", ";"};
    QVector<QString> strnew = {"\\ ", "\\!", "\\$", "\\&", "\\*", "\\(", "\\)", "\\<", "\\>", "\\+", "\\-", "\\;"};
    for (FileEntry file : files) {
        QString strFile = file.strFullPath;
        for (int n = 0; n < strold.length(); ++n) {
            strFile.replace(strold[n], strnew[n]);
        }

        if (strFile.endsWith('/')) {
            strFile.chop(1);
        }

        int pos = strFile.lastIndexOf('/');
        if (pos > 0) {
            //??????????????????files???????????????
            strFileName += "-C " + strFile.mid(0, pos + 1) + oneSpace + strFile.mid(pos + 1) + oneSpace;
        }
    }

    // ??????????????????????????????????????????
    QString strTmparchive = m_strArchiveName;
    for (int n = 0; n < strold.length(); ++n) {
        strTmparchive.replace(strold[n], strnew[n]);
    }

    QString strTemp = QString("tar -cvf - %1 | pigz -p %2 -%3 > %4").arg(strFileName).arg(options.iCPUTheadNum).arg(options.iCompressionLevel).arg(strTmparchive);

    QStringList slist = QStringList() << "-c" << strTemp;
    m_process->setProgram(QStandardPaths::findExecutable("bash"), slist);

    connect(m_process, &QProcess::readyReadStandardOutput, this, [ = ] {
        readStdout();
    });

    connect(m_process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)));

    m_process->start();

    if (m_process->waitForStarted()) {
        m_childProcessId.clear();
        m_processId = m_process->processId();

        getChildProcessId(m_processId, QStringList() << "tar" << "pigz", m_childProcessId);
    }

    return PFT_Nomral;
}

PluginFinishType LibPigzPlugin::moveFiles(const QList<FileEntry> &, const CompressOptions &)
{
    return PFT_Nomral;
}

PluginFinishType LibPigzPlugin::copyFiles(const QList<FileEntry> &, const CompressOptions &)
{
    return PFT_Nomral;
}

PluginFinishType LibPigzPlugin::deleteFiles(const QList<FileEntry> &)
{
    return PFT_Nomral;
}

PluginFinishType LibPigzPlugin::renameFiles(const QList<FileEntry> &files)
{
    return PFT_Nomral;
}

PluginFinishType LibPigzPlugin::addComment(const QString &)
{
    return PFT_Nomral;
}

PluginFinishType LibPigzPlugin::updateArchiveData(const UpdateOptions &)
{
    return PFT_Nomral;
}

void LibPigzPlugin::pauseOperation()
{
    if (!m_childProcessId.empty()) {
        for (int i = m_childProcessId.size() - 1; i >= 0; i--) {
            if (m_childProcessId[i] > 0) {
                kill(static_cast<__pid_t>(m_childProcessId[i]), SIGSTOP);
            }
        }
    }

    if (m_processId > 0) {
        kill(static_cast<__pid_t>(m_processId), SIGSTOP);
    }
}

void LibPigzPlugin::continueOperation()
{
    if (!m_childProcessId.empty()) {
        for (int i = m_childProcessId.size() - 1; i >= 0; i--) {
            if (m_childProcessId[i] > 0) {
                kill(static_cast<__pid_t>(m_childProcessId[i]), SIGCONT);
            }
        }
    }

    if (m_processId > 0) {
        kill(static_cast<__pid_t>(m_processId), SIGCONT);
    }
}

bool LibPigzPlugin::doKill()
{
    if (m_process) {
        killProcess(false);
        return true;
    }

    return false;
}

bool LibPigzPlugin::handleLine(const QString &line)
{
    if (line.contains(QLatin1String("No space left on device"))) {
        m_eErrorType = ET_InsufficientDiskSpace;
        return false;
    } else {
        // ???????????????????????????????????????????????????/????????????????????????????????????
        QFileInfo info(m_strArchiveName);
        emit signalprogress(static_cast<double>(info.size()) / m_qTotalSize * 100);
        emit signalCurFileName(line);
    }

    return true;
}

void LibPigzPlugin::killProcess(bool)
{
    if (!m_process) {
        return;
    }

    if (!m_childProcessId.empty()) {
        for (int i = m_childProcessId.size() - 1; i >= 0; i--) {
            if (m_childProcessId[i] > 0) {
                kill(static_cast<__pid_t>(m_childProcessId[i]), SIGKILL);
            }
        }
    }

    qint64 processID = m_process->processId();
    // ??????????????????continue???kill??????????????????????????????
    // ??????SIGTERM???7z?????????????????????????????????
    if (processID > 0) {
        kill(static_cast<__pid_t>(processID), SIGCONT);
        kill(static_cast<__pid_t>(processID), SIGTERM);
    }

}

void LibPigzPlugin::deleteProcess()
{
    if (m_process) {
        readStdout(true);
        m_process->blockSignals(true); // delete m_process????????????????????????m_process???????????????????????????
        delete m_process;
        m_process = nullptr;
    }
}

void LibPigzPlugin::getChildProcessId(qint64 processId, const QStringList &listKey, QVector<qint64> &childprocessid)
{
    /* bash(5494)-+-tar(5495)
     *            `-pigz(5496)-+-{pigz}(5498)
     *                         |-{pigz}(5499)
     *                         `-{pigz}(5500)
     */

    childprocessid.clear();

    // ????????????system process??????????????????????????????
    if (0 >= processId || 0 == listKey.count()) {
        return;
    }

    //??????pstree??????????????????????????????pstree -np 23347??????????????????23348
    QString strProcessId = QString::number(processId);
    QProcess p;
    p.setProgram("pstree");
    p.setArguments(QStringList() << "-np" << strProcessId);
    p.start();

    if (p.waitForReadyRead()) {
        QByteArray dd = p.readAllStandardOutput();
        QList<QByteArray> lines = dd.split('\n');

        if (lines.count() > 0 && lines[0].contains(strProcessId.toUtf8())) {     // ????????????processId?????????????????????
            for (const QByteArray &line : qAsConst(lines)) {
                for (const QString &strKey : qAsConst(listKey)) {
                    QString str = QString("-%1(").arg(strKey);
                    int iCount = line.count(str.toStdString().c_str());        // ?????????????????????????????????
                    int iIndex = 0;
                    for (int i = 0; i < iCount; ++i) {
                        int iStartIndex = line.indexOf(str, iIndex);
                        int iEndIndex = line.indexOf(")", iStartIndex);
                        if (0 < iStartIndex && 0 < iEndIndex) {
                            childprocessid.append(line.mid(iStartIndex + str.length(), iEndIndex - iStartIndex - str.length()).toInt());    // ???-7z(3971)??????????????????
                        }
                        iIndex = iStartIndex + 1;
                    }
                }
            }
        }
    }
}

void LibPigzPlugin::readStdout(bool)
{
    //?????????????????????????????????????????????????????????
    if (m_isProcessKilled) {
        return;
    }

    Q_ASSERT(m_process);

    if (!m_process->bytesAvailable()) { // ?????????
        return;
    }

    // ?????????????????????
    QByteArray dd = m_process->readAllStandardOutput();
    m_stdOutData += dd;

    QList<QByteArray> lines = m_stdOutData.split('\n');
    m_stdOutData = lines.takeLast();
    // ?????????????????????
    for (const QByteArray &line : qAsConst(lines)) {

        if (!handleLine(QString::fromLocal8Bit(line))) {
            killProcess();
            return;
        }

    }
}

void LibPigzPlugin::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qInfo() << "Process finished, exitcode:" << exitCode << "exitstatus:" << exitStatus;

    deleteProcess();

    PluginFinishType eFinishType;

    if (0 == exitCode) { // job????????????
        eFinishType = PFT_Nomral;
    } else {
        eFinishType = PFT_Error;
    }

    emit signalprogress(100);
    emit signalFinished(eFinishType);
}
