// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "darchivemanager_p.h"
#include "kpluginfactory.h"
#include "kpluginloader.h"
#include "mimetypes.h"
#include "pluginmanager.h"
#include "singlejob.h"
#include "batchjob.h"
#include "uitools.h"
#include "archivejob.h"
#include "datamanager.h"
#include <QMimeDatabase>
#include <QFileInfo>
#include <QDebug>

DCOMPRESSOR_USE_NAMESPACE

DArchiveManager::MgrFileEntry::MgrFileEntry()
{
    reset();
}

void DArchiveManager::MgrFileEntry::reset()
{
    strFullPath = "";
    strFileName = "";
    strAlias = "";
    isDirectory = false;
    qSize = 0;
    uLastModifiedTime = 0;
    iIndex = -1;
}

bool DArchiveManager::MgrFileEntry::operator==(const MgrFileEntry &t) const
{
    if (this->strFullPath == t.strFullPath && this->strFileName == t.strFileName
        && this->isDirectory == t.isDirectory && this->qSize == t.qSize
        && this->uLastModifiedTime == t.uLastModifiedTime && this->iIndex == t.iIndex) {
        return true;
    }
    return false;
}

DArchiveManager::MgrArchiveData::MgrArchiveData()
{
    reset();
}

void DArchiveManager::MgrArchiveData::reset()
{
    qSize = 0;
    qComressSize = false;
    strComment = "";
    mapFileEntry.clear();
    listRootEntry.clear();

    isListEncrypted = false;
    strPassword.clear();
}

DArchiveManager::MgrCompressOptions::MgrCompressOptions()
    : qTotalSize(0)
{
    bEncryption = false;
    bHeaderEncryption = false;
    bSplit = false;
    iVolumeSize = 0;
    iCompressionLevel = -1;
    bTar_7z = false;
    iCPUTheadNum = 1;
}

DArchiveManager::MgrExtractionOptions::MgrExtractionOptions()
    : qSize(0), qComressSize(0)
{
    bExistList = true;
    bAllExtract = false;
    bBatchExtract = false;
    bTar_7z = false;
    bOpen = false;
}

DArchiveManager::MgrUpdateOptions::MgrUpdateOptions()
{
    reset();
}

void DArchiveManager::MgrUpdateOptions::reset()
{
    eType = Delete;
    strParentPath.clear();
    listEntry.clear();
    qSize = 0;
}

QMutex DArchiveManagerPrivate::m_mutex;
QAtomicPointer<DArchiveManager> DArchiveManagerPrivate::m_instance = nullptr;

DArchiveManager::DArchiveManager(QObject *parent)
    : QObject(parent), d_ptr(new DArchiveManagerPrivate(this))
{
}

DArchiveManager::~DArchiveManager()
{
    Q_D(DArchiveManager);
    SAFE_DELETE_ELE(d->m_pArchiveJob);
    SAFE_DELETE_ELE(d->m_pInterface);
    SAFE_DELETE_ELE(d->m_pTempInterface);
}

DArchiveManager *DArchiveManager::get_instance()
{
#ifndef Q_ATOMIC_POINTER_TEST_AND_SET_IS_SOMETIMES_NATIVE
    if (!QAtomicPointer<DArchiveManager>::isTestAndSetNative())
        qInfo() << "Error: TestAndSetNative not supported!";
#endif

    //使用双重检测。

    /*! testAndSetOrders操作保证在原子操作前和后的的内存访问
     * 不会被重新排序。
     */
    if (DArchiveManagerPrivate::m_instance.testAndSetOrdered(nullptr, nullptr)) {
        QMutexLocker locker(&DArchiveManagerPrivate::m_mutex);

        DArchiveManagerPrivate::m_instance.testAndSetOrdered(nullptr, new DArchiveManager);
    }

    return DArchiveManagerPrivate::m_instance;
}

void DArchiveManager::destory_instance()
{
    Q_D(DArchiveManager);
    SAFE_DELETE_ELE(d->m_instance)
}

bool DArchiveManager::createArchive(const QList<MgrFileEntry> &files, const QString &strDestination, const MgrCompressOptions &stOptions, ArchivePluginType eType)
{
    Q_D(DArchiveManager);
    if (d->m_pInterface != nullptr) {
        delete d->m_pInterface;
        d->m_pInterface = nullptr;
    }

    d->m_pTempInterface = UiTools::createInterface(strDestination, true, (UiTools::AssignPluginType)eType);

    if (d->m_pTempInterface) {
        CreateJob *pCreateJob = new CreateJob(*((QList<FileEntry> *)&files), d->m_pTempInterface, *((CompressOptions *)&stOptions), this);

        connect(pCreateJob, &CreateJob::signalJobFinshed, d, &DArchiveManagerPrivate::slotJobFinished);
        connect(pCreateJob, &CreateJob::signalprogress, this, &DArchiveManager::signalprogress);
        connect(pCreateJob, &CreateJob::signalCurFileName, this, &DArchiveManager::signalCurFileName);

        d->m_pArchiveJob = pCreateJob;
        pCreateJob->start();

        return true;
    }

    return false;
}

bool DArchiveManager::loadArchive(const QString &strArchiveFullPath, ArchivePluginType eType)
{
    Q_D(DArchiveManager);
    if (d->m_pInterface != nullptr) {
        delete d->m_pInterface;
        d->m_pInterface = nullptr;
    }

    d->m_pInterface = UiTools::createInterface(strArchiveFullPath, false, (UiTools::AssignPluginType)eType);

    if (d->m_pInterface) {
        LoadJob *pLoadJob = new LoadJob(d->m_pInterface);

        connect(pLoadJob, &LoadJob::signalJobFinshed, d, &DArchiveManagerPrivate::slotJobFinished);

        d->m_pArchiveJob = pLoadJob;
        pLoadJob->start();

        return true;
    }
    return false;
}

DArchiveManager::MgrArchiveData *DArchiveManager::archiveData()
{
    return (MgrArchiveData *)&(DataManager::get_instance().archiveData());
}

bool DArchiveManager::addFiles(const QString &strArchiveFullPath, const QList<MgrFileEntry> &listAddEntry, const MgrCompressOptions &stOptions)
{
    Q_D(DArchiveManager);
    d->m_pTempInterface = UiTools::createInterface(strArchiveFullPath, true);

    if (d->m_pTempInterface) {
        AddJob *pAddJob = new AddJob(*(QList<FileEntry> *)&listAddEntry, d->m_pTempInterface, *(CompressOptions *)&stOptions);

        connect(pAddJob, &AddJob::signalJobFinshed, d, &DArchiveManagerPrivate::slotJobFinished);
        connect(pAddJob, &AddJob::signalprogress, this, &DArchiveManager::signalprogress);
        connect(pAddJob, &AddJob::signalCurFileName, this, &DArchiveManager::signalCurFileName);

        d->m_pArchiveJob = pAddJob;
        pAddJob->start();

        return true;
    }

    return false;
}

bool DArchiveManager::extractFiles(const QString &strArchiveFullPath, const QList<MgrFileEntry> &files, const MgrExtractionOptions &stOptions, ArchivePluginType eType)
{
    Q_D(DArchiveManager);
    if (nullptr == d->m_pInterface) {
        d->m_pInterface = UiTools::createInterface(strArchiveFullPath, false, (UiTools::AssignPluginType)eType);
    }

    if (d->m_pInterface) {
        if (!stOptions.bTar_7z) {
            ExtractJob *pExtractJob = new ExtractJob(*(QList<FileEntry> *)&files, d->m_pInterface, *(ExtractionOptions *)&stOptions);

            connect(pExtractJob, &ExtractJob::signalJobFinshed, d, &DArchiveManagerPrivate::slotJobFinished);
            connect(pExtractJob, &ExtractJob::signalprogress, this, &DArchiveManager::signalprogress);
            connect(pExtractJob, &ExtractJob::signalCurFileName, this, &DArchiveManager::signalCurFileName);
            connect(pExtractJob, &ExtractJob::signalFileWriteErrorName, this, &DArchiveManager::signalFileWriteErrorName);

            d->m_pArchiveJob = pExtractJob;
            pExtractJob->start();

            return pExtractJob->errorcode;
        } else {
            StepExtractJob *pStepExtractJob = new StepExtractJob(strArchiveFullPath, *(ExtractionOptions *)&stOptions);
            d->m_pArchiveJob = pStepExtractJob;

            connect(pStepExtractJob, &StepExtractJob::signalJobFinshed, d, &DArchiveManagerPrivate::slotJobFinished);
            connect(pStepExtractJob, &StepExtractJob::signalprogress, this, &DArchiveManager::signalprogress);
            connect(pStepExtractJob, &StepExtractJob::signalCurFileName, this, &DArchiveManager::signalCurFileName);

            pStepExtractJob->start();
            return true;
        }
    }

    // 发送结束信号
    emit signalJobFinished(DArchiveManager::JT_Extract, PFT_Error, ET_PluginError);
    return false;
}

bool DArchiveManager::extractFiles2Path(const QString &strArchiveFullPath, const QList<MgrFileEntry> &listSelEntry, const MgrExtractionOptions &stOptions)
{
    Q_D(DArchiveManager);
    if (nullptr == d->m_pInterface) {
        d->m_pInterface = UiTools::createInterface(strArchiveFullPath);
    }

    if (d->m_pInterface) {
        ExtractJob *pExtractJob = new ExtractJob(*(QList<FileEntry> *)&listSelEntry, d->m_pInterface, *(ExtractionOptions *)&stOptions);

        connect(pExtractJob, &ExtractJob::signalJobFinshed, d, &DArchiveManagerPrivate::slotJobFinished);
        connect(pExtractJob, &ExtractJob::signalprogress, this, &DArchiveManager::signalprogress);
        connect(pExtractJob, &ExtractJob::signalCurFileName, this, &DArchiveManager::signalCurFileName);

        d->m_pArchiveJob = pExtractJob;
        pExtractJob->start();

        return true;
    }

    return false;
}

bool DArchiveManager::deleteFiles(const QString &strArchiveFullPath, const QList<MgrFileEntry> &listSelEntry)
{
    Q_D(DArchiveManager);
    if (nullptr == d->m_pInterface) {
        d->m_pInterface = UiTools::createInterface(strArchiveFullPath);
    }

    if (d->m_pInterface) {
        DeleteJob *pDeleteJob = new DeleteJob(*(QList<FileEntry> *)&listSelEntry, d->m_pInterface);

        connect(pDeleteJob, &DeleteJob::signalJobFinshed, d, &DArchiveManagerPrivate::slotJobFinished);
        connect(pDeleteJob, &DeleteJob::signalprogress, this, &DArchiveManager::signalprogress);
        connect(pDeleteJob, &DeleteJob::signalCurFileName, this, &DArchiveManager::signalCurFileName);

        d->m_pArchiveJob = pDeleteJob;
        pDeleteJob->start();

        return true;
    }

    return false;
}

bool DArchiveManager::renameFiles(const QString &strArchiveFullPath, const QList<MgrFileEntry> &listSelEntry)
{
    Q_D(DArchiveManager);
    if (nullptr == d->m_pInterface) {
        d->m_pInterface = UiTools::createInterface(strArchiveFullPath);
    }

    if (d->m_pInterface) {
        RenameJob *pRenameJob = new RenameJob(*(QList<FileEntry> *)&listSelEntry, d->m_pInterface);

        connect(pRenameJob, &RenameJob::signalJobFinshed, d, &DArchiveManagerPrivate::slotJobFinished);
        connect(pRenameJob, &RenameJob::signalprogress, this, &DArchiveManager::signalprogress);
        connect(pRenameJob, &RenameJob::signalCurFileName, this, &DArchiveManager::signalCurFileName);

        d->m_pArchiveJob = pRenameJob;
        pRenameJob->start();

        return true;
    }

    return false;
}

bool DArchiveManager::batchExtractFiles(const QStringList &listFiles, const QString &strTargetPath /*, bool bAutoCreatDir*/)
{
    Q_D(DArchiveManager);
    BatchExtractJob *pBatchExtractJob = new BatchExtractJob();
    pBatchExtractJob->setExtractPath(strTargetPath /*, bAutoCreatDir*/);

    if (pBatchExtractJob->setArchiveFiles(listFiles)) {
        connect(pBatchExtractJob, &BatchExtractJob::signalJobFinshed, d, &DArchiveManagerPrivate::slotJobFinished);
        connect(pBatchExtractJob, &BatchExtractJob::signalprogress, this, &DArchiveManager::signalprogress);
        connect(pBatchExtractJob, &BatchExtractJob::signalCurFileName, this, &DArchiveManager::signalCurFileName);
        connect(pBatchExtractJob, &BatchExtractJob::signalCurArchiveName, this, &DArchiveManager::signalCurArchiveName);

        d->m_pArchiveJob = pBatchExtractJob;
        pBatchExtractJob->start();

        return true;
    }

    SAFE_DELETE_ELE(pBatchExtractJob);
    return false;
}

bool DArchiveManager::openFile(const QString &strArchiveFullPath, const MgrFileEntry &stEntry, const QString &strTempExtractPath, const QString &strProgram)
{
    Q_D(DArchiveManager);
    if (nullptr == d->m_pInterface) {
        d->m_pInterface = UiTools::createInterface(strArchiveFullPath);
    }

    if (d->m_pInterface) {
        OpenJob *pOpenJob = new OpenJob(*(FileEntry *)&stEntry, strTempExtractPath, strProgram, d->m_pInterface);

        connect(pOpenJob, &OpenJob::signalJobFinshed, d, &DArchiveManagerPrivate::slotJobFinished);

        d->m_pArchiveJob = pOpenJob;
        pOpenJob->start();

        return true;
    }

    return false;
}

bool DArchiveManager::updateArchiveCacheData(const MgrUpdateOptions &stOptions)
{
    Q_D(DArchiveManager);
    if (d->m_pInterface) {
        UpdateJob *pUpdateJob = new UpdateJob(*(UpdateOptions *)&stOptions, d->m_pInterface);

        connect(pUpdateJob, &UpdateJob::signalJobFinshed, d, &DArchiveManagerPrivate::slotJobFinished);

        d->m_pArchiveJob = pUpdateJob;
        pUpdateJob->start();

        return true;
    }

    return false;
}

bool DArchiveManager::updateArchiveComment(const QString &strArchiveFullPath, const QString &strComment)
{
    Q_D(DArchiveManager);
    ReadOnlyArchiveInterface *pInterface = UiTools::createInterface(strArchiveFullPath, true, UiTools::APT_Libzip);
    if (pInterface) {
        CommentJob *pCommentJob = new CommentJob(strComment, pInterface);

        connect(pCommentJob, &CommentJob::signalprogress, this, &DArchiveManager::signalprogress);
        connect(pCommentJob, &CommentJob::signalJobFinshed, d, &DArchiveManagerPrivate::slotJobFinished);

        d->m_pArchiveJob = pCommentJob;
        pCommentJob->start();

        return true;
    }

    return false;
}

bool DArchiveManager::convertArchive(const QString &strOriginalArchiveFullPath, const QString &strTargetFullPath, const QString &strNewArchiveFullPath)
{
    Q_D(DArchiveManager);
    ConvertJob *pConvertJob = new ConvertJob(strOriginalArchiveFullPath, strTargetFullPath, strNewArchiveFullPath);
    d->m_pArchiveJob = pConvertJob;

    connect(pConvertJob, &ConvertJob::signalJobFinshed, d, &DArchiveManagerPrivate::slotJobFinished);
    connect(pConvertJob, &ConvertJob::signalprogress, this, &DArchiveManager::signalprogress);
    connect(pConvertJob, &ConvertJob::signalCurFileName, this, &DArchiveManager::signalCurFileName);

    pConvertJob->start();
    return true;
}

bool DArchiveManager::pauseOperation()
{
    Q_D(DArchiveManager);
    if (d->m_pArchiveJob) {
        d->m_pArchiveJob->doPause();

        return true;
    }

    return false;
}

bool DArchiveManager::continueOperation()
{
    Q_D(DArchiveManager);
    if (d->m_pArchiveJob) {
        d->m_pArchiveJob->doContinue();

        return true;
    }

    return false;
}

bool DArchiveManager::cancelOperation()
{
    Q_D(DArchiveManager);
    if (d->m_pArchiveJob) {
        d->m_pArchiveJob->kill();
        d->m_pArchiveJob->deleteLater();
        d->m_pArchiveJob = nullptr;

        return true;
    }

    return false;
}

QString DArchiveManager::getCurFilePassword()
{
    Q_D(DArchiveManager);
    if (d->m_pInterface) {
        return d->m_pInterface->getPassword();
    }

    return "";
}

bool DArchiveManager::currentStatus()
{
    Q_D(DArchiveManager);
    if (d->m_pArchiveJob) {
        return d->m_pArchiveJob->status();
    }

    return false;
}

void DArchiveManagerPrivate::slotJobFinished()
{
    Q_Q(DArchiveManager);
    if (m_pArchiveJob) {
        ArchiveJob::JobType eJobType = m_pArchiveJob->m_eJobType;
        PluginFinishType eFinishType = m_pArchiveJob->m_eFinishedType;
        ErrorType eErrorType = m_pArchiveJob->m_eErrorType;

        m_pArchiveJob->deleteLater();
        m_pArchiveJob = nullptr;

        emit q->signalJobFinished((DArchiveManager::ArchiveJobType)eJobType, (DArchiveManager::MgrPluginFinishType)eFinishType, (DArchiveManager::MgrErrorType)eErrorType);
    }

    SAFE_DELETE_ELE(m_pTempInterface);
}
