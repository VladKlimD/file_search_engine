#include "search_server.h"

QVector<QList<Relative_Index>> Search_Server::search(QVector<QString> queries_Input)
{
    //Проверка на пустые запросы
    if (queries_Input.isEmpty())
        return QVector<QList<Relative_Index>>();

    QVector<QList<Relative_Index>> answers;

    for (auto& request : queries_Input) {
        answers.append(request_To_Answer(request));
    }

    return answers;
}

QList<Relative_Index> Search_Server::request_To_Answer(QString request)
{
    //Список слов из запроса
    QMap<size_t, QString> request_Words = get_Request_Words(request);

    //Нахождение списка документов содержащих самое редкое слово
    QList<Relative_Index> answer;
    for (auto& entry : _index->freq_Dictionary[request_Words.begin().value()]) {
        answer.append({entry.doc_Id, 0});
    }

    double max_Rank = 0;
    //Количество ответов
    size_t answers_Count = 1;

    //Рассчет абсолютной релевантности слов по каждому документу
    //Поиск соответствий следующих слов на наличие их в документах из полученного списка
    //Если хотя бы одно слово не присутствует в документе из списка, то такой документ удаляется
    for (auto& doc : answer) {
        //Если количество ответов больше максимально допустимого, то ответы не формируются,
		//все последующие документы из списка удаляются
        if (answers_Count > _index->converter.get_Responses_Limit()) {
			answer.removeOne(doc);
			continue;
		}
        bool doc_Is_Present = false;
        //Абсолютная релевантность для одного документа
        double rank = 0;
        //Проход по каждому слову из запроса
        for (auto& word : request_Words) {
            doc_Is_Present = false;
            //Поиск индекса документа в частотном словаре
            for (auto& entry : _index->freq_Dictionary[word]) {
                if (doc.doc_Id == entry.doc_Id) {
                    //Слово присутствует в документе
                    doc_Is_Present = true;
                    rank += entry.count;
                    break;
                }
            }
            //Если одно из слов не присутствует в документе
            if (!doc_Is_Present)
                break;
        }
        //Если одно из слов не присутствует в документе
        if (!doc_Is_Present) {
            //Удаление данного документа из списка
            answer.removeOne(doc);
            continue;
        }
        doc.rank = rank;
        answers_Count++;
        //Поиск максимального значения абсолютной релевантности
        if (rank > max_Rank)
            max_Rank = rank;
    }

    //Документы не были найдены
    if (answer.isEmpty())
        return answer;

    //Получение относительной релевантности
    for (auto& doc : answer) {
        doc.rank /= max_Rank;
    }

    return answer;
}

QMap<size_t, QString> Search_Server::get_Request_Words(QString request)
{
    QMap<size_t, QString> request_Words;

    //Последовательности символов, которые содержат одну или более буквы,
    //цифры или знак подчеркивания, возможны апострофы в середине слов
    QRegularExpression expression("\\b\\w+(?:['’]\\w+)*\\b");

    //Итератор, который позволяет перебирать все совпадения регулярного выражения в документе
    QRegularExpressionMatchIterator it = expression.globalMatch(request);

    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();

        QString word = match.captured(0);
        word = word.toCaseFolded();

        //Формирование списка уникальных слов из запроса в порядке возрастания частоты встречаемости
        size_t count = 0;
        if (_index->freq_Dictionary.contains(word)) {
            for (auto& entry : _index->freq_Dictionary[word]) {
                count += entry.count;
            }
        }

        request_Words.insert(count, word);
    }

    return request_Words;
}

QVector<QList<QPair<size_t, double>>> Search_Server::convert_To_QPair(QVector<QList<Relative_Index>> vector) {
    QVector<QList<QPair<size_t, double>>> result;
    result.reserve(vector.size());
    for (auto const& list : vector) {
        QList<QPair<size_t, double>> convertedList;
        convertedList.reserve(list.size());
        for (auto const& relativeIndex : list) {
            convertedList.append(QPair<size_t, double>(relativeIndex.doc_Id, relativeIndex.rank));
        }
        result.append(convertedList);
    }
    return result;
}







