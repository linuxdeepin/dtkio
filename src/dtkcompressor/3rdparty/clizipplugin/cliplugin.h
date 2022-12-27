/*
 * ark -- archiver for the KDE project
 *
 * Copyright (C) 2011 Raphael Kubo da Costa <rakuco@FreeBSD.org>
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
 */

#ifndef CLIPLUGIN_H
#define CLIPLUGIN_H

#include "../interface/cliinterface.h"
#include "../interface/kpluginfactory.h"

class CliPluginFactory : public KPluginFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.kde.KPluginFactory" FILE "kerfuffle_clizip.json")
    Q_INTERFACES(KPluginFactory)
public:
    explicit CliPluginFactory();
    ~CliPluginFactory();
};



class  CliPlugin : public CliInterface
{
    Q_OBJECT

public:
    explicit CliPlugin(QObject *parent, const QVariantList &args);
    ~CliPlugin() override;

    void resetParsing() override;
    QString escapeFileName(const QString &fileName) const override;
    bool readListLine(const QString &line) override;
    bool readExtractLine(const QString &line) override;
    bool isPasswordPrompt(const QString &line) override;
    bool isWrongPasswordMsg(const QString &line) override;
    bool isCorruptArchiveMsg(const QString &line) override;
    bool isDiskFullMsg(const QString &line) override;
    bool isFileExistsMsg(const QString &line) override;
    bool isFileExistsFileName(const QString &line) override;

    bool moveFiles(const QList<Archive::Entry *> &files, Archive::Entry *destination, const CompressionOptions &options) override;
    int moveRequiredSignals() const override;

private Q_SLOTS:
    void continueMoving(bool result);

private:
    void setupCliProperties();
    bool setMovingAddedFiles();
    void finishMoving(bool result);
    QString convertCompressionMethod(const QString &method);

    enum ParseState {
        ParseStateHeader = 0,
        ParseStateComment,
        ParseStateEntry
    } m_parseState;

    int m_linesComment;
    QString m_tempComment;
};

#endif // CLIPLUGIN_H
