// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "gtest/gtest.h"
#include <stubext.h>
#include "demo.h"

DCOMPRESSOR_USE_NAMESPACE

class TestDemo : public testing::Test
{
public:
    void SetUp() override
    {
        m_demo = new Demo;
    }
    void TearDown() override
    {
        delete m_demo;
        m_demo = nullptr;
    }

public:
    Demo *m_demo = nullptr;
};

TEST_F(TestDemo, add)
{
    EXPECT_EQ(3, m_demo->add(1, 2));
}

TEST_F(TestDemo, svg2png)
{
    EXPECT_FALSE(m_demo->svg2png("/no/exist/svg", "/tmp/test.png"));
}
