#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>
#include <QMainWindow>
#include <QtSql>

namespace Ui {
class adddialog;
}

class adddialog : public QDialog
{
    Q_OBJECT

public:
    explicit adddialog(QWidget *parent = 0);
    ~adddialog();
    void setRequestdb(QSqlDatabase dbse);

private:
    Ui::adddialog *ui;
    QSqlDatabase newdb;
    QString  strr;
    QString  addNumber;
    QString  addName;
    QString  addProfession;
    QString  addClass;
    QString  addYear;
    QString  addTelphone;
    QString  addtongyaun;
    QString  addyingyu;
    QString  addweibo;
    QString  addqianru;
    QStringList level;
    QStringList scinsert;
    void getnewInfo();
    void getoldInfo();
    void updatadialog();
    void insert();
    void inputFinish();

signals:
    void addfinish();
};

#endif // ADDDIALOG_H
