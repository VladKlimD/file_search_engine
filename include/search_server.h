#ifndef SEARCH_SERVER_H
#define SEARCH_SERVER_H

#include "inverted_index.h"

#include <QVector>
#include <QString>
#include <QList>

struct Relative_Index {
    size_t doc_Id;
    double rank;

    bool operator ==(const Relative_Index& other) const {
        return (doc_Id == other.doc_Id && rank == other.rank);
    }
};

class Search_Server {
    Inverted_Index* _index;

public:
    Search_Server(Inverted_Index* idx) : _index(idx) { };

    //Метод обработки поисковых запросов
    QVector<QList<Relative_Index>> search(QVector<QString> queries_Input);

    //Метод обработки одного запроса
    QList<Relative_Index> request_To_Answer(QString request);

    //Получение списка уникальных слов из поискового запроса
    QMap<size_t, QString> get_Request_Words(QString request);

    QVector<QList<QPair<size_t, double>>> convert_To_QPair(QVector<QList<Relative_Index>> vector);
};

#endif // SEARCH_SERVER_H
