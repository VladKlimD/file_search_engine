#ifndef JSON_CONVERTER_H
#define JSON_CONVERTER_H

#include <QString>
#include <QVector>
#include <QJsonDocument>

class Json_Converter {
protected:
    //Пути к json файлам
    QString config_Path = "config.json";
    QString requests_Path = "requests.json";
    QString answers_Path = "answers.json";

private:
    //Метод получения содержимого документа json
    QJsonDocument load_Json_File(const QString& path);

public:
    Json_Converter() = default;

    //Метод получения путей файлов txt из config.json
    QVector<QString> get_Paths_Of_Documents();

    //Метод получения содержимого txt файлов из config.json
    QVector<QString> get_Text_Documents();

    //Метод считывает поле max_responses для определения предельного
    //количества ответов на один запрос
    size_t get_Responses_Limit();

    //Считать версию из config.json
    double get_Version();

    //Получить название из config.json
    QString get_Name();

    //Метод получения запросов из файла requests.json
    //возвращает список запросов из файла requests.json
    QVector<QString> get_Requests();


    //Положить в файл answers.json результаты поисковых запросов
    void put_Answers(QVector<QList<QPair<size_t, qreal>>> answers);
};

#endif // JSON_CONVERTER_H
