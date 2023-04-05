#include <QtTest>

// add necessary includes here
#include <search_server.h>

class Test_Search_Server : public QObject
{
    Q_OBJECT

public:
    Test_Search_Server() = default;
    ~Test_Search_Server() = default;

private slots:
    void test_Simple();
    void test_Top5();
};

void Test_Search_Server::test_Simple()
{
    const QVector<QString> docs = {
        "milk milk milk milk water water water",
        "milk water water",
        "milk milk milk milk milk water water water water water",
        "americano cappuccino"
    };

    const QVector<QString> request = {"milk water", "sugar"};

    const QVector<QList<Relative_Index>> expected = {
        {
            {0, 0.6}, {1, 0.4}, {2, 1}
        },
        {

        }
    };

    Inverted_Index idx;
    idx.update_Document_Base(docs);

    Search_Server srv(&idx);

    QVector<QList<Relative_Index>> result = srv.search(request);

    QVERIFY2(result == expected, "test_simple");
}

void Test_Search_Server::test_Top5()
{
    const QVector<QString> docs = {
        "london is the capital of great britain",
        "paris is the capital of france",
        "berlin is the capital of germany",
        "rome is the capital of italy",
        "madrid is the capital of spain",
        "lisboa is the capital of portugal",
        "bern is the capital of switzerland",
        "moscow is the capital of russia",
        "kiev is the capital of ukraine",
        "minsk is the capital of belarus",
        "astana is the capital of kazakhstan",
        "beijing is the capital of china",
        "tokyo is the capital of japan",
        "bangkok is the capital of thailand",
        "welcome to moscow the capital of russia the third rome",
        "amsterdam is the capital of netherlands",
        "helsinki is the capital of finland",
        "oslo is the capital of norway",
        "stockholm is the capital of sweden",
        "riga is the capital of latvia",
        "tallinn is the capital of estonia",
        "warsaw is the capital of poland",
    };

    const QVector<QString> request = {"moscow is the capital of russia"};

    const QVector<QList<Relative_Index>> expected = {
        {
            {7, 1}
        }
    };

    Inverted_Index idx;
    idx.update_Document_Base(docs);

    Search_Server srv(&idx);

    QVector<QList<Relative_Index>> result = srv.search(request);

    QVERIFY2(result == expected, "test_Top5");
}

QTEST_APPLESS_MAIN(Test_Search_Server)

#include "tst_test_search_server.moc"
