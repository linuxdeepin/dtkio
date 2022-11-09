// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#pragma once

#include "namespace.h"
#include <QSize>
#include <qobject.h>

DCOMPRESSOR_BEGIN_NAMESPACE
class Demo : public QObject {
    Q_OBJECT
  public:
    Demo(QObject *parent = nullptr);
    ~Demo();
    int add(const int a, const int b);

    // librsvg
    bool svg2png(const QString &svgfile, const QString &pngfile, QSize size = QSize(128, 128));
};
DCOMPRESSOR_END_NAMESPACE
