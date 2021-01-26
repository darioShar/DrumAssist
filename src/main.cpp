#include <QApplication>
#include <QtPlugin>

#include "projectdjembe.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ProjectDjembe project;
    project.show();

    return a.exec();
}
