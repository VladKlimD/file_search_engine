#include <QtTest>

// add necessary includes here
#include <inverted_index.h>
#include <json_converter.h>

class Test_Inverted_Index : public QObject {

    Q_OBJECT

public:
    Test_Inverted_Index() = default;
    ~Test_Inverted_Index() = default;

    bool test_Functionality(const QVector<QString>& docs,
                            const QVector<QString>& requests,
                            const QList<QVector<Entry>>& expected);

private slots:
    void basic_Test1();
    void basic_Test2();
    void missing_Word();
};

bool Test_Inverted_Index::test_Functionality(const QVector<QString>& docs,
                                             const QVector<QString>& requests,
                                             const QList<QVector<Entry>>& expected)
{
    QList<QVector<Entry>> result;
    Inverted_Index idx;

    idx.update_Document_Base(docs);

    for (auto request : requests) {
        QVector<Entry> word_Count = idx.get_Word_Count(request);
        result.append(word_Count);
    }

    return result == expected;
}

void Test_Inverted_Index::basic_Test1()
{
    const QVector<QString> docs = {
        "london is the capital of great britain",
        "big ben is the nickname for the Great bell of the striking clock"
    };

    const QVector<QString> requests = {"london", "the"};

    const QList<QVector<Entry>> expected = {
        {
            {0, 1}
        },
        {
            {0, 1}, {1, 3}
        }
    };

    QVERIFY2(test_Functionality(docs, requests, expected), "basic_Test1");
}

void Test_Inverted_Index::basic_Test2()
{
    const QVector<QString> docs = {
        "milk milk milk milk water water water",
        "milk water water",
        "milk milk milk milk milk water water water water water",
        "americano cappuchino"
    };
    const QVector<QString> requests = {"milk", "water", "cappuchino"};
    const QList<QVector<Entry>> expected = {
        {
            {0, 4}, {1, 1}, {2, 5}
        },
        {
            {0, 3}, {1, 2}, {2, 5}
        },
        {
            {3, 1}
        }
    };



    QVERIFY2(test_Functionality(docs, requests, expected), "basic_Test2");
}

void Test_Inverted_Index::missing_Word()
{
    const QVector<QString> docs = {
        "a b c d e f g h i j k l",
        "statement"
    };

    const QVector<QString> requests = {"m", "statement"};

    const QList<QVector<Entry>> expected = {
        { },
        {
            {1, 1}
        }
    };

    QVERIFY2(test_Functionality(docs, requests, expected), "missing_Word");
}

QTEST_APPLESS_MAIN(Test_Inverted_Index)

#include "tst_test_inverted_index.moc"
