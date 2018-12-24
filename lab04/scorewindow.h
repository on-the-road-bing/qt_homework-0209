#ifndef SCOREWINDOW_H
#define SCOREWINDOW_H

#include <adddialog.h>
#include <QMainWindow>
#include <QtSql>

namespace Ui {
class ScoreWindow;
}

class ScoreWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ScoreWindow(QWidget *parent = 0);
    ~ScoreWindow();

private:
    Ui::ScoreWindow *ui;
    QSqlDatabase db;
    //查询
    QString str;
    QString sname;
    QString snumber;
    QString sclass;
    QString sgrade;
    QStringList scname;
    QStringList scclassname;
    QStringList scscore ;
    QStringList sccredit;
    void on_btnQuery_clicked();
    void queryStudentInfo(QString queryi);

    QString calculatePoints(const QStringList scorepoint,const QStringList creditpoint);
    void queryStudentScore(QString queryr);

    //信息录入
    adddialog *dialog;
    QString path ;
    void on_actinput_clicked();
    void on_actfileinput_clicked();
    void on_actoutput_clicked();
    void closedialog();
    void updateInfoWindow();
    void OutputTheInfo(const QString &tableName,const QString &csvFileName);

};


#endif // SCOREWINDOW_H
