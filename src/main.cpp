#include "mainwindow.h"
#include "inverted_index.h"
#include "search_server.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow w;
    w.show();

    Inverted_Index index;
    index.update_Document_Base(index.converter.get_Text_Documents());

    Search_Server server(&index);
    index.converter.put_Answers(server.convert_To_QPair(server.search(index.converter.get_Requests())));

    return app.exec();
}
