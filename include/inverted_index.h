#ifndef INVERTED_INDEX_H
#define INVERTED_INDEX_H

#include "json_converter.h"

#include <QVector>
#include <QString>
#include <QMap>
#include <QMutex>

struct Entry {
    size_t doc_Id, count;

    bool operator ==(const Entry& other) const {
        return (doc_Id == other.doc_Id && count == other.count);
    }
};

class Inverted_Index
{ 
    //Загруженная база документов
    QVector<QString> docs;

    //Мьютекс для контроля заполнения частотного словаря
    QMutex freq_Dictionary_mutex;

public:
    //Частотный словарь
    QMap<QString, QVector<Entry>> freq_Dictionary;

public:
    Inverted_Index() = default;

    //Конвертер
    Json_Converter converter;

    //Обновление или заполнение базы данных, по которой совершается поиск
    void update_Document_Base(QVector<QString> input_Docs);

    //Определение количества вхождений слова в загруженной базе документов
    QVector<Entry> get_Word_Count(QString& word);

    //Индексация одного документа
    void indexing_Doc(QString& Doc);

    //Заполнение частотного словаря
    void fill_Freq_Dictionary();
};

#endif // INVERTED_INDEX_H
