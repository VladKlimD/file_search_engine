#include "json_converter.h"

#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <iostream>

//Метод получения содержимого документа
QJsonDocument Json_Converter::load_Json_File(const QString& path)
{
    QFile file(path);

    //Проверка на успешное открытие файла
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cout << "Can't open the json file: " << qPrintable(config_Path);
        return QJsonDocument();
    }

    QTextStream input(&file);
    QString json_Content = input.readAll();

    //Преобразование JSON объекта
    QJsonParseError error;
    QJsonDocument json_Doc = QJsonDocument::fromJson(json_Content.toUtf8(), &error);
    //Вывод ошибок при парсинге
    if (error.error != QJsonParseError::NoError) {
        std::cout << "Parsing" << qPrintable(path) << ": "
                  << qPrintable(error.errorString()) << error.offset << error.error;
    }

    return json_Doc;
}

//Метод получения путей файлов txt из config.json
QVector<QString> Json_Converter::get_Paths_Of_Documents()
{
    //Загрузка содержимого файла config.json
    QJsonDocument json_Doc = load_Json_File(config_Path);

    //Проверка на то, что json_Doc не пустой
    if (json_Doc.isEmpty())
        return QVector<QString>();

    //Преобразование JSON объекта
    QJsonObject json_Obj = json_Doc.object();

    //Получение путей файлов в виде Json массива
    QJsonArray files_Arr = json_Obj.value("files").toArray();
    QVector<QString> files_Paths;
    for (const auto& path : files_Arr)
        files_Paths.append(path.toString());

    return files_Paths;
}

//Метод получения содержимого файлов txt из config.json
QVector<QString> Json_Converter::get_Text_Documents()
{
    QVector<QString> files_Paths = get_Paths_Of_Documents();

    //Проверка на наличие путей к файлам
    if (files_Paths.isEmpty())
        return QVector<QString>();

    //Запись содержимого файлов
    QVector<QString> files_Content;
    for (const auto& file_Path : files_Paths) {
        QFile file(file_Path);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream input(&file);
            input.setCodec("UTF-8");
            QString file_Content = input.readAll();
            files_Content.append(file_Content);
        }
        else {
            std::cout << "Can't open the file: " << qPrintable(file_Path) << std::endl;
            files_Content.append(QString());
        }
    }

    return files_Content;
}

size_t Json_Converter::get_Responses_Limit()
{
    //Загрузка содержимого файла config.json
    QJsonDocument json_Doc = load_Json_File(config_Path);

    //Проверка на то, что json_Doc не пустой
    if (json_Doc.isEmpty())
        return 0;

    //Преобразование JSON объекта
    QJsonObject json_Obj = json_Doc.object();

    //Получение значения поля config
    QJsonObject config_Obj = json_Obj.value("config").toObject();
    return config_Obj.value("max_responses").toInt();
}

double Json_Converter::get_Version()
{
    //Загрузка содержимого файла config.json
    QJsonDocument json_Doc = load_Json_File(config_Path);

    //Проверка на то, что json_Doc не пустой
    if (json_Doc.isEmpty())
        return 0;

    //Преобразование JSON объекта
    QJsonObject json_Obj = json_Doc.object();

    //Получение значения поля config
    QJsonObject config_Obj = json_Obj.value("config").toObject();
    return config_Obj.value("version").toString().toDouble();
}

QString Json_Converter::get_Name()
{
    //Загрузка содержимого файла config.json
    QJsonDocument json_Doc = load_Json_File(config_Path);

    //Проверка на то, что json_Doc не пустой
    if (json_Doc.isEmpty())
        return QString();

    //Преобразование JSON объекта
    QJsonObject json_Obj = json_Doc.object();

    //Получение значения поля config
    QJsonObject config_Obj = json_Obj.value("config").toObject();
    return config_Obj.value("name").toString();
}

QVector<QString> Json_Converter::get_Requests()
{
    //Загрузка содержимого файла requests.json
    QJsonDocument json_Doc = load_Json_File(requests_Path);

    //Проверка на то, что json_Doc не пустой
    if (json_Doc.isEmpty())
        return QVector<QString>();

    //Преобразование JSON объекта
    QJsonObject json_Obj = json_Doc.object();

    //Получение списка запросов в виде Json массива
    QJsonArray requests_Arr = json_Obj.value("requests").toArray();
    QVector<QString> requests;
    for (const auto& request : requests_Arr)
        requests.append(request.toString());

    return requests;
}

void Json_Converter::put_Answers(QVector<QList<QPair<size_t, qreal>>> answers)
{
    //Создание объекта JSON
    QJsonObject root;
    QJsonObject json_Answers;

    for (auto i = 0; i < answers.size(); ++i) {
        QJsonObject request_Object;
        if (!answers[i].isEmpty()) {
            request_Object.insert("result", "true");
            QJsonArray request_Array;
            QJsonObject relevance_Object;
            if (answers[i].size() == 1) {
                request_Object.insert("docid", static_cast<int>(answers[i].at(0).first));
                request_Object.insert("rank", answers[i].at(0).second);
            }
            else {
                for (auto j = 0; j < answers[i].size(); ++j) {
                    const auto& doc = answers[i][j];
                    relevance_Object.insert("docid", static_cast<int>(doc.first));
                    relevance_Object.insert("rank", doc.second);
                    request_Array.append(relevance_Object);
                }
                request_Object.insert("relevance", request_Array);
            }
        }
        else {
            request_Object.insert("result", "false");
        }
        json_Answers.insert("request" + QString::number(i + 1), request_Object);
    }
    root.insert("answers", json_Answers);

    // Запись ответов в файл answers.json
    QFile output_File(answers_Path);
    if (output_File.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&output_File);
        stream << QJsonDocument(root).toJson();
        output_File.close();
    }
    else {
        std::cout << "Can't write the json file: " << qPrintable(answers_Path);
    }
}

