// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
#include <DFile>

#include <gtest/gtest.h>
#include <stubext.h>

DIO_USE_NAMESPACE
DCORE_USE_NAMESPACE

class TestDFile : public testing::Test
{
public:
    void SetUp() override
    {
    }
    void TearDown() override
    {
        m_stub.clear();
    }

    stub_ext::StubExt m_stub;
};

TEST_F(TestDFile, url)
{
    QUrl testUrl { QUrl::fromLocalFile("/tmp") };
    DFile file { testUrl };
    EXPECT_EQ(testUrl, file.url());
}
