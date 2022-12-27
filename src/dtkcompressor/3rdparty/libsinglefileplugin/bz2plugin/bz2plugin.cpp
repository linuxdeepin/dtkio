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

#include "bz2plugin.h"

#include <QString>

//#include "kpluginfactory.h"

//K_PLUGIN_CLASS_WITH_JSON(LibBzip2Interface, "kerfuffle_libbz2.json")

LibBzip2InterfaceFactory::LibBzip2InterfaceFactory()
{
    registerPlugin<LibBzip2Interface>();
}
LibBzip2InterfaceFactory::~LibBzip2InterfaceFactory()
{

}

LibBzip2Interface::LibBzip2Interface(QObject *parent, const QVariantList &args)
    : LibSingleFileInterface(parent, args)
{
    m_mimeType = QStringLiteral("application/x-bzip");
    m_possibleExtensions.append(QStringLiteral(".bz2"));
}

LibBzip2Interface::~LibBzip2Interface()
{
}

//#include "bz2plugin.moc"
