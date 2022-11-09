// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "demo.h"
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QDebug>

DCOMPRESSOR_BEGIN_NAMESPACE
Demo::Demo(QObject *parent)
    : QObject(parent) {}

Demo::~Demo() {}

int Demo::add(const int a, const int b)
{
    return a + b;
}

bool Demo::svg2png(const QString &svgfile, const QString &pngfile, QSize size)
{
    // TODO
    return false;
}

DCOMPRESSOR_END_NAMESPACE
