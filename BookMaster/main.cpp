#include "mainwindow.h"
#include "Data.h"

#include <QApplication>
#include <QFile>
#include <QMessageBox>
#include <QVector>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile file("D:\\data\\data.bm");
    if (!file.open(QIODevice::ReadWrite | QIODevice::Append)) {
        QMessageBox::critical(0, "Ошибка", "Ошибка открытия базы данных!");
        exit(-1);
    }
    read_data(file);
    file.close();
    MainWindow w;
    w.showMaximized();
    w.show();
    return a.exec();
}
