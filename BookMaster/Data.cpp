#include "Data.h"

QVector<Data>* Arrdata = new QVector<Data>;

QString Data::get(const int &col) const
{
    switch (col) {
    case 0: return title;
    case 1: return surname;
    case 2: return name;
    case 3: return year;
    case 4: return publisher;
    case 5: return cost;
    case 6: return price;
    }
}

void Data::set_title(const QString &title) {this->title = title;}
void Data::set_publisher(const QString &publisher) {this->publisher = publisher;}
void Data::set_name(const QString &name) {this->name = name;}
void Data::set_surname(const QString &surname) {this->surname = surname;}
void Data::set_year(const QString &year) {this->year = year;}
void Data::set_cost(const QString &cost) {this->cost = cost;}
void Data::set_price(const QString &price) {this->price = price;}


class ptrDataAndFile {
public:
    static QVector<Data>* _ptrdata;
    static QFile* _ptrfile;
    ~ptrDataAndFile() {
        delete _ptrdata;
        delete _ptrfile;
    }
};
QVector<Data>* ptrDataAndFile::_ptrdata = nullptr;
QFile* ptrDataAndFile::_ptrfile = nullptr;

QVector<Data>* get_ptrdata() {return ptrDataAndFile::_ptrdata;}
QFile* get_ptrfile() {return ptrDataAndFile::_ptrfile;}


QVector<Data>* read_data(QFile& file) {
    file.seek(0);
    Data node;
    QString object;
    int count = 0;
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        for (const auto& cell : line) {
            if (cell != '#') {
                object += cell;
            } else {
                switch (++count) {
                case 1: node.set_title(object); break;
                case 2: node.set_surname(object); break;
                case 3: node.set_name(object); break;
                case 4: node.set_year(object); break;
                case 5: node.set_publisher(object); break;
                case 6: node.set_cost(object); break;
                case 7: node.set_price(object); break;
                }
                object = "";
            }
        }
        count = 0;
        Arrdata->append(node);
    }
    ptrDataAndFile::_ptrfile = &file;
    ptrDataAndFile::_ptrdata = Arrdata;
    return Arrdata;
};

