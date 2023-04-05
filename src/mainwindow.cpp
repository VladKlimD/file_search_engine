#include "mainwindow.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //Установка названия из config.json
    setWindowTitle(converter.get_Name());

    //Вывод максимального значения ответов
    QLabel* max_Responses_Label = new QLabel;
    max_Responses_Label->setText("Max responses");
    QLineEdit* max_Responses_Line_Edit = new QLineEdit;
    max_Responses_Line_Edit->setText(QString::number(converter.get_Responses_Limit()));
    QHBoxLayout* max_Responses_Layout = new QHBoxLayout;
    max_Responses_Layout->addWidget(max_Responses_Label);
    max_Responses_Layout->addWidget(max_Responses_Line_Edit);

    //Вывод версии
    QLabel* version_Label = new QLabel;
    version_Label->setText("Version");
    QLineEdit* version_Line_Edit = new QLineEdit;
    double version = converter.get_Version();
    version_Line_Edit->setText(version > 0 ? QString::number(version) : "unknown");
    version_Line_Edit->setReadOnly(true);
    QHBoxLayout* version_Layout = new QHBoxLayout;
    version_Layout->addWidget(version_Label);
    version_Layout->addWidget(version_Line_Edit);

    //Вывод названий txt файлов
    QLabel* files_Names = new QLabel;
    QVector<QString> files_Paths = converter.get_Paths_Of_Documents();
    for (auto it = files_Paths.cbegin(); it != files_Paths.cend(); ++it) {
        files_Names->setText(files_Names->text() + *it);
        if (it != files_Paths.cend() - 1)
            files_Names->setText(files_Names->text() + "\n");
    }
    files_Names->setAlignment(Qt::AlignTop);

    //Layout с конфигурацией config.json
    QVBoxLayout* config_Layout = new QVBoxLayout;
    config_Layout->addLayout(max_Responses_Layout);
    config_Layout->addLayout(version_Layout);
    config_Layout->addWidget(files_Names);

    QVBoxLayout* main_Layout = new QVBoxLayout;
    main_Layout->addLayout(config_Layout);

    QWidget* central_Widget = new QWidget(this);
    central_Widget->setLayout(main_Layout);
    setCentralWidget(central_Widget);
}

MainWindow::~MainWindow()
{
}

