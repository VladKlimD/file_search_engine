#include "inverted_index.h"

#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>
#include <QThreadPool>

//Обновление или заполнение базы данных, по которой совершается поиск
void Inverted_Index::update_Document_Base(QVector<QString> input_Docs)
{
    docs = input_Docs;
    fill_Freq_Dictionary();
}

//Определение количества вхождений слова в загруженной базе документов
QVector<Entry> Inverted_Index::get_Word_Count(QString& word)
{
    QVector<Entry> word_Count;
    //Регулярное выражение, позволяющее искать слово целиком
    //После слова не должен идти апостроф, буква или цифра
    QRegularExpression word_Regex("\\b" + word + "\\b", QRegularExpression::CaseInsensitiveOption);
    for (size_t id = 0; id < docs.size(); ++id) {
        size_t count = docs[id].count(word_Regex);
        if (count != 0) {
            word_Count.append({ id, count });
        }
    }
    return word_Count;
}

//Индексация одного документа
void Inverted_Index::indexing_Doc(QString& doc)
{
    //Последовательности символов, которые содержат одну или более буквы,
    //цифры или знак подчеркивания, возможны апострофы в середине слов
    QRegularExpression expression("\\b\\w+(?:['’]\\w+)*\\b");

    //Итератор, который позволяет перебирать все совпадения регулярного выражения в документе
    QRegularExpressionMatchIterator it = expression.globalMatch(doc);

    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();

        QString word = match.captured(0);
        word = word.toCaseFolded();

        freq_Dictionary_mutex.lock();
        //Условие уточнает что нет этого слова в частотном словаре
        if (!freq_Dictionary.contains(word)) {
            freq_Dictionary_mutex.unlock();

            QVector<Entry> word_Count = get_Word_Count(word);

            freq_Dictionary_mutex.lock();
            //Добавление слова в частотный словарь
            freq_Dictionary.insert(word, word_Count);
        }
        freq_Dictionary_mutex.unlock();
    }
}


//Заполнение частотного словаря
void Inverted_Index::fill_Freq_Dictionary()
{
    //update_Document_Base(converter.get_Text_Documents());

    if (docs.isEmpty())
        return;

    //Создание пула потоков для индексации документов
    QThreadPool thread_Pool;

    //Максимальное количество потоков выбирается автоматически исходя из системы
    //В случае если документов больше максимального количества потоков открытых одновременно,
    //то их индексация встает в очередь
    thread_Pool.setMaxThreadCount(QThread::idealThreadCount());

    for (auto& doc : docs) {
        thread_Pool.start([&]() {
            indexing_Doc(doc);
        });
    }

    //Ожидание выполнения задач в пуле потоков
    thread_Pool.waitForDone();
}

