/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Copyright (C) 2016 Intel Corporation.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "main.h"

#include <QFile>
#include <QHash>
#include <QString>
#include <QStringList>
#include <QUuid>
#include <QTest>


class tst_QHash : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void qhash_current_data() { data(); }
    void qhash_current() { qhash_template<QString>(); }
    void qhash_qt50_data() { data(); }
    void qhash_qt50() { qhash_template<Qt50String>(); }
    void qhash_qt4_data() { data(); }
    void qhash_qt4() { qhash_template<Qt4String>(); }
    void qhash_javaString_data() { data(); }
    void qhash_javaString() { qhash_template<JavaString>(); }

    void hashing_current_data() { data(); }
    void hashing_current() { hashing_template<QString>(); }
    void hashing_qt50_data() { data(); }
    void hashing_qt50()  { hashing_template<Qt50String>(); }
    void hashing_qt4_data() { data(); }
    void hashing_qt4() { hashing_template<Qt4String>(); }
    void hashing_javaString_data() { data(); }
    void hashing_javaString() { hashing_template<JavaString>(); }

private:
    void data();
    template <typename String> void qhash_template();
    template <typename String> void hashing_template();

    QStringList smallFilePaths;
    QStringList uuids;
    QStringList dict;
    QStringList numbers;
};

///////////////////// QHash /////////////////////

#include <QDebug>

void tst_QHash::initTestCase()
{
    // small list of file paths
    QFile smallPathsData(QFINDTESTDATA("paths_small_data.txt"));
    QVERIFY(smallPathsData.open(QIODevice::ReadOnly));
    smallFilePaths = QString::fromLatin1(smallPathsData.readAll()).split(QLatin1Char('\n'));
    QVERIFY(!smallFilePaths.isEmpty());

    // list of UUIDs
    // guaranteed to be completely random, generated by http://xkcd.com/221/
    QUuid ns = QUuid("{f43d2ef3-2fe9-4563-a6f5-5a0100c2d699}");
    uuids.reserve(smallFilePaths.size());

    foreach (const QString &path, smallFilePaths)
        uuids.append(QUuid::createUuidV5(ns, path).toString());


    // lots of strings with alphabetical characters, vaguely reminiscent of
    // a dictionary.
    //
    // this programatically generates a series like:
    //  AAAAAA
    //  AAAAAB
    //  AAAAAC
    //  ...
    //  AAAAAZ
    //  AAAABZ
    //  ...
    //  AAAAZZ
    //  AAABZZ
    QByteArray id("AAAAAAA");

    if (dict.isEmpty()) {
        for (int i = id.length() - 1; i > 0;) {
            dict.append(id);
            char c = id.at(i);
            id[i] = ++c;

            if (c == 'Z') {
                // wrap to next digit
                i--;
                id[i] = 'A';
            }
        }
    }

    // string versions of numbers.
    for (int i = 5000000; i < 5005001; ++i)
        numbers.append(QString::number(i));
}

void tst_QHash::data()
{
    QTest::addColumn<QStringList>("items");
    QTest::newRow("paths-small") << smallFilePaths;
    QTest::newRow("uuids-list") << uuids;
    QTest::newRow("dictionary") << dict;
    QTest::newRow("numbers") << numbers;
}

template <typename String> void tst_QHash::qhash_template()
{
    QFETCH(QStringList, items);
    QHash<String, int> hash;

    QList<String> realitems;
    foreach (const QString &s, items)
        realitems.append(s);

    QBENCHMARK {
        for (int i = 0, n = realitems.size(); i != n; ++i) {
            hash[realitems.at(i)] = i;
        }
    }
}

template <typename String> void tst_QHash::hashing_template()
{
    // just the hashing function
    QFETCH(QStringList, items);

    QList<String> realitems;
    realitems.reserve(items.size());
    foreach (const QString &s, items)
        realitems.append(s);

    QBENCHMARK {
        for (int i = 0, n = realitems.size(); i != n; ++i)
            (void)qHash(realitems.at(i));
    }
}

QTEST_MAIN(tst_QHash)

#include "main.moc"
