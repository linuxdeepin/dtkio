/*
 * ark -- archiver for the KDE project
 *
 * Copyright (C) 2011 Luke Shumaker <lukeshu@sbcglobal.net>
 * Copyright (C) 2016 Elvis Angelaccio <elvis.angelaccio@kde.org>
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

#include "cliplugin.h"
#include "kprocess.h"
#include "../interface/queries.h"

#include <QJsonArray>
#include <QJsonParseError>

CliPluginFactory::CliPluginFactory()
{
    registerPlugin<CliPlugin>();
}
CliPluginFactory::~CliPluginFactory()
{

}


//K_PLUGIN_CLASS_WITH_JSON(CliPlugin, "kerfuffle_cliunarchiver.json")

CliPlugin::CliPlugin(QObject *parent, const QVariantList &args)
    : CliInterface(parent, args)
{
    setupCliProperties();
}

CliPlugin::~CliPlugin()
{
    //确保kprocess先释放，避免阻塞时调用虚函数错误
    if (m_process != nullptr) {
        m_process->kill();
        m_process->waitForFinished(1);
    }
}

bool CliPlugin::list(bool /*isbatch*/)
{
    resetParsing();
    m_operationMode = List;

    return runProcess(m_cliProps->property("listProgram").toString(), m_cliProps->listArgs(filename(), password()));
}

bool CliPlugin::extractFiles(const QVector<Archive::Entry *> &files, const QString &destinationDirectory, const ExtractionOptions &options)
{
    ExtractionOptions newOptions = options;

    newOptions.setAlwaysUseTempDir(true);

    return CliInterface::extractFiles(files, destinationDirectory, newOptions);
}

void CliPlugin::resetParsing()
{
    m_jsonOutput.clear();
    m_numberOfVolumes = 0;
}

void CliPlugin::setupCliProperties()
{
    m_cliProps->setProperty("captureProgress", false);

    m_cliProps->setProperty("extractProgram", QStringLiteral("unar"));
    m_cliProps->setProperty("extractSwitch", QStringList{QStringLiteral("-D")});
    m_cliProps->setProperty("extractSwitchNoPreserve", QStringList{QStringLiteral("-D")});

    m_cliProps->setProperty("listProgram", QStringLiteral("lsar"));
    m_cliProps->setProperty("listSwitch", QStringList{QStringLiteral("-json")});

    m_cliProps->setProperty("passwordSwitch", QStringList{QStringLiteral("-password"),
                                                          QStringLiteral("$Password")});
}

bool CliPlugin::readListLine(const QString &line)
{
    const QRegularExpression rx(QStringLiteral("Failed! \\((.+)\\)$"));

    if (rx.match(line).hasMatch()) {
        emit error(("Listing the archive failed."));
        return false;
    }

    return true;
}

bool CliPlugin::readExtractLine(const QString &line)
{
    const QRegularExpression rx(QStringLiteral("Failed! \\((.+)\\)$"));

    if (rx.match(line).hasMatch()) {
        emit error(("Extraction failed."));
        return false;
    }

    return true;
}

void CliPlugin::setJsonOutput(const QString &jsonOutput)
{
    m_jsonOutput = jsonOutput;
    readJsonOutput();
}

void CliPlugin::readStdout(bool handleAll)
{
    if (!handleAll) {
        CliInterface::readStdout(false);
        return;
    }

    // We are ready to read the json output.
    readJsonOutput();
}

bool CliPlugin::handleLine(const QString &line)
{
    // Collect the json output line by line.
    if (m_operationMode == List) {
        // #372210: lsar can generate huge JSONs for big archives.
        // We can at least catch a bad_alloc here in order to not crash.
        try {
            m_jsonOutput += line + QLatin1Char('\n');
        } catch (const std::bad_alloc &) {
            m_jsonOutput.clear();
            emit error(("Not enough memory for loading the archive."));
            return false;
        }
    }

    if (m_operationMode == List) {
        // This can only be an header-encrypted archive.
        if (isPasswordPrompt(line)) {

            PasswordNeededQuery query(filename());
            emit userQuery(&query);
            query.waitForResponse();

            if (query.responseCancelled()) {
                emit cancelled();
                // Process is gone, so we emit finished() manually and we return true.
                emit finished(false);
                return true;
            }

            setPassword(query.password());
            CliPlugin::list();
        }
    }

    return true;
}

void CliPlugin::processFinished(int exitCode, QProcess::ExitStatus /*exitStatus*/)
{

//    if (m_process) {
//        //handle all the remaining data in the process
//        readStdout(true);

//        delete m_process;
//        m_process = nullptr;
//    }

    deleteProcess();

    // #193908 - #222392
    // Don't emit finished() if the job was killed quietly.
    if (m_abortingOperation) {
        return;
    }

    if (!password().isEmpty()) {

        // lsar -json exits with error code 1 if the archive is header-encrypted and the password is wrong.
        if (exitCode == 1) {
            emit error(("Wrong password."));
            emit finished(false);
            setPassword(QString());
            return;
        }
    }

    // lsar -json exits with error code 2 if the archive is header-encrypted and no password is given as argument.
    // At this point we are asking a password to the user and we are going to list() again after we get one.
    // This means that we cannot emit finished here.
    if (exitCode == 2) {
        return;
    }

    emit finished(true);
}

void CliPlugin::readJsonOutput()
{
    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(m_jsonOutput.toUtf8(), &error);

    if (error.error != QJsonParseError::NoError) {
        return;
    }

    const QJsonObject json = jsonDoc.object();

    const QJsonObject properties = json.value(QStringLiteral("lsarProperties")).toObject();
    const QJsonArray volumes = properties.value(QStringLiteral("XADVolumes")).toArray();
    if (volumes.count() > 1) {
        m_numberOfVolumes = volumes.count();
        setMultiVolume(true);
    }

    QString formatName = json.value(QStringLiteral("lsarFormatName")).toString();
    if (formatName == QLatin1String("RAR")) {
        emit compressionMethodFound(QStringLiteral("RAR4"));
    } else if (formatName == QLatin1String("RAR 5")) {
        emit compressionMethodFound(QStringLiteral("RAR5"));
    }
    const QJsonArray entries = json.value(QStringLiteral("lsarContents")).toArray();

    for (const QJsonValue &value : entries) {
        const QJsonObject currentEntryJson = value.toObject();

        Archive::Entry *currentEntry = new Archive::Entry(this);

        QString filename = currentEntryJson.value(QStringLiteral("XADFileName")).toString();

        currentEntry->setProperty("isDirectory", !currentEntryJson.value(QStringLiteral("XADIsDirectory")).isUndefined());
        if (currentEntry->isDir()) {
            filename += QLatin1Char('/');
        }

        currentEntry->setProperty("fullPath", filename);

        // FIXME: archives created from OSX (i.e. with the __MACOSX folder) list each entry twice, the 2nd time with size 0
        currentEntry->setProperty("size", currentEntryJson.value(QStringLiteral("XADFileSize")));
        currentEntry->setProperty("compressedSize", currentEntryJson.value(QStringLiteral("XADCompressedSize")));
        currentEntry->setProperty("timestamp", currentEntryJson.value(QStringLiteral("XADLastModificationDate")).toVariant());
        currentEntry->setProperty("size", currentEntryJson.value(QStringLiteral("XADFileSize")));
        const bool isPasswordProtected = (currentEntryJson.value(QStringLiteral("XADIsEncrypted")).toInt() == 1);
        currentEntry->setProperty("isPasswordProtected", isPasswordProtected);
        if (isPasswordProtected) {
            formatName == QLatin1String("RAR 5") ? emit encryptionMethodFound(QStringLiteral("AES256")) :
            emit encryptionMethodFound(QStringLiteral("AES128"));
        }
        // TODO: missing fields

        emit entry(currentEntry);
    }
}

bool CliPlugin::isPasswordPrompt(const QString &line)
{
    return (line == QLatin1String("This archive requires a password to unpack. Use the -p option to provide one."));
}

#include "moc_cliplugin.cpp"
