#ifndef DATA_H
#define DATA_H

#include <QString>
#include <QVector>
#include <QFile>
#include <QTextStream>

class Data {
private:
    QString title, publisher, name, surname, year, cost, price;
public:
    void set_title(const QString &title);
    void set_publisher(const QString &publisher);
    void set_name(const QString &name);
    void set_surname(const QString &surname);
    void set_year(const QString &year);
    void set_cost(const QString &cost);
    void set_price(const QString &price);
    QString get(const int &col) const;
};

class ptrDataAndFile;

QVector<Data>* read_data(QFile& file);

QVector<Data>* get_ptrdata();
QFile* get_ptrfile();

#endif // DATA_H
