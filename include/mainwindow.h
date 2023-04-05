#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "json_converter.h"

#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Json_Converter converter;
};
#endif // MAINWINDOW_H
