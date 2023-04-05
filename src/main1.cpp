#include "search_server.h"
#include <QCoreApplication>
#include <iostream>

int main()
{
    Inverted_Index index;

    //Заполнение частотного словаря
    index.update_Document_Base(index.converter.get_Text_Documents());

    //Проверка на успешное заполнение частотного словаря
    if (index.freq_Dictionary.isEmpty()) {
        std::cout << "Failed to form a frequency dictionary\n";
        std::cout << "Searching is unavalible\n";
        return -1;
    }

    Search_Server server(&index);
    index.converter.put_Answers(server.convert_To_QPair(server.search(index.converter.get_Requests())));
}
