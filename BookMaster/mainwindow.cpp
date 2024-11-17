#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Data.h"

#include <QTextStream>
#include <QFile>
#include <QVector>
#include <QMessageBox>
#include <QApplication>
#include <QStandardItemModel>
#include <QTableView>
#include <QSortFilterProxyModel>
#include <QLineEdit>
#include <QKeyEvent>


QVector<Data>* list;
QFile* file;
QStandardItemModel *model;

void make_table(Ui::MainWindow* ui);
void remake_file(Ui::MainWindow* ui);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    list = get_ptrdata();
    file = get_ptrfile();
    make_table(ui);
}

MainWindow::~MainWindow()
{
    remake_file();
    delete ui;
}

void make_table(Ui::MainWindow* ui)
{
    model = new QStandardItemModel((*list).size(), 8);
    (*model).setHorizontalHeaderLabels(QStringList() << "Название" << "Фамилия автора" <<
                                       "Имя автора" << "Год выхода" << "Издательство" <<
                                       "Себестоимость" << "Цена" << "Прибыль");
    for (int row = 0; row < (*list).size(); ++row) {
        for (int col = 0; col < 8; ++col) {
            (*model).setItem(row, col, new QStandardItem((*list)[row].get(col)));
            if (col == 7) {
                (*model).setItem(row, col, new QStandardItem(QString::number((*list)[row].get(6).toInt() -
                                                                             (*list)[row].get(5).toInt())));
            }
        }
    }
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel;
    (*proxyModel).setSourceModel(model);
    QTableView* tableView = ui->stackedWidget->widget(1)->findChild<QTableView*>("tableView");
    tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->verticalHeader()->setVisible(false);
    tableView->setModel(proxyModel);
    tableView->setSortingEnabled(true);
    QLineEdit* lineEdit = ui->stackedWidget->widget(1)->findChild<QLineEdit*>("lineEdit");
    QObject::connect(lineEdit, &QLineEdit::textChanged, [proxyModel] (const QString& text){
        proxyModel->setFilterRegExp(QRegExp(text, Qt::CaseInsensitive, QRegExp::PatternSyntax()));
    });
    tableView->resizeColumnsToContents();
    tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    tableView->show();
}

void MainWindow::remake_file()
{
    QTextStream out(file);
    if (!file->open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QMessageBox::critical(this, "Ошибка", "Ошибка открытия базы данных!");
        exit(-1);
    }
    for (int row = 0; row < model->rowCount(); ++row) {
        for (int col = 0; col < model->columnCount() - 1; ++col) {
            out << (*model).data(model->index(row, col)).toString() + '#';
        }
        out << '\n';
    }
}

void MainWindow::on_pushButton_insert_clicked()
{
    QTextStream out(file);
    if (!file->open(QIODevice::ReadWrite | QIODevice::Append)) {
        QMessageBox::critical(this, "Критическая ошибка", "Ошибка открытия базы данных!");
        exit(-1);
    }
    if (ui->lineEdit_title->text().isEmpty() || ui->lineEdit_surname->text().isEmpty() ||
            ui->lineEdit_name->text().isEmpty() || ui->lineEdit_year->text().isEmpty() ||
            ui->lineEdit_publisher->text().isEmpty() || ui->lineEdit_cost->text().isEmpty() ||
            ui->lineEdit_price->text().isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Все поля должны быть заполнены!");
        file->close();
        return;
    }
    file->seek(file->size());
    out << ui->lineEdit_title->text() + '#' + ui->lineEdit_surname->text() + '#'
           + ui->lineEdit_name->text() + '#' + ui->lineEdit_year->text() + '#'
           + ui->lineEdit_publisher->text() + '#' + ui->lineEdit_cost->text() + '#'
           + ui->lineEdit_price->text() + "#\n";
    file->flush();
    file->close();
    int row = model->rowCount(), col = 0;
    (*model).setItem(row, col++, new QStandardItem(ui->lineEdit_title->text()));
    (*model).setItem(row, col++, new QStandardItem(ui->lineEdit_surname->text()));
    (*model).setItem(row, col++, new QStandardItem(ui->lineEdit_name->text()));
    (*model).setItem(row, col++, new QStandardItem(ui->lineEdit_year->text()));
    (*model).setItem(row, col++, new QStandardItem(ui->lineEdit_publisher->text()));
    (*model).setItem(row, col++, new QStandardItem(ui->lineEdit_cost->text()));
    (*model).setItem(row, col++, new QStandardItem(ui->lineEdit_price->text()));
    (*model).setItem(row, col++, new QStandardItem(QString::number(ui->lineEdit_price->text().toInt() -
                                                                   ui->lineEdit_cost->text().toInt())));
    QMessageBox::information(this, "Выполнено", "Данные успешно внесены!");
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Delete)
    {
        QItemSelectionModel *select = ui->tableView->selectionModel();
        if(select->hasSelection())
        {
            QModelIndexList selectedRows = select->selectedRows();
            QMessageBox msgBox(this);
            msgBox.setWindowTitle("Удаление");
            msgBox.setText("Количество удаляемых книг: " + QString::number(selectedRows.count()) +
                           "\nВосстановление будет невозможно");
            msgBox.addButton("Удалить", QMessageBox::YesRole);
            QAbstractButton* pButtonNo = msgBox.addButton("Нет", QMessageBox::NoRole);
            msgBox.setDefaultButton(qobject_cast<QPushButton*>(pButtonNo));
            msgBox.exec();
            if (msgBox.clickedButton() == pButtonNo) {
                return;
            }
            for(int i = selectedRows.count(); i-- > 0;)
                ui->tableView->model()->removeRow(selectedRows.at(i).row());
        }
    }
}


void MainWindow::on_pushButton_new_book_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButton_all_book_clicked()
{
    ui->tableView->resizeColumnsToContents();
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->stackedWidget->setCurrentIndex(1);
}
