#include "adddialog.h"
#include "ui_adddialog.h"

#include <QMessageBox>

adddialog::adddialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::adddialog)
{
    ui->setupUi(this);

    connect(ui->actoldstud,&QPushButton::clicked,this,&adddialog::getoldInfo);
    connect(ui->actnewstud,&QPushButton::clicked,this,&adddialog::getnewInfo);
    connect(ui->actstart,&QPushButton::clicked,this,&adddialog::inputFinish);
}

adddialog::~adddialog()
{
    delete ui;
}

void adddialog::setRequestdb(QSqlDatabase dbse){
    newdb =dbse ;
}
void adddialog::getoldInfo(){//得到已有学生信息
    if (newdb.open())
        newdb.close();
    QSqlQuery sql_query;       //数据库操作类
    if(!newdb.open())
    {
        qDebug() << "Error: Failed to connect database." << newdb.lastError();
        return;
    }
    else{
        if( !ui->lineEdit_Name->text().isEmpty())
            strr=QString("where sName='%1'").arg(ui->lineEdit_Name->text());
        else if(!ui->lineEdit_Number->text().isEmpty())
            strr=QString("where sNumber='%1'").arg(ui->lineEdit_Number->text());
        QString  select_sql = QString("select * FROM t_stud_info %1;").arg(strr);
        if(!sql_query.exec(select_sql))
                {
                    qDebug()<<sql_query.lastError();
                }
                else
                {
                  while(sql_query.next())
                  {
                    addNumber=(sql_query.value(0).toString());
                    addName=(sql_query.value(1).toString());
                    addProfession=(sql_query.value(2).toString());
                    addClass=(sql_query.value(3).toString());
                  }
                }
    }
     updatadialog();
        }

void adddialog::getnewInfo(){
    QSqlQuery sql_query;
    QString  select_sql;
    addName=ui->lineEdit_Name->text();
    addNumber=ui->lineEdit_Number->text();
    addProfession=ui->lineEdit_profess->text();
    addClass=ui->lineEdit_class->text();
    if (newdb.open())
        newdb.close();
    if(!newdb.open())
    {
        qDebug() << "Error: Failed to connect database." << newdb.lastError();
        return;
    }
    else{
    QString add=QString("select * FROM t_stud_info");
    sql_query.exec(add);
    select_sql = QString("insert into  t_stud_info  value ('%1','%2','%3','%4',' ',' ');").arg(addNumber,addName,addProfession,addClass);
    //sql_query.exec(select_sql);
    }
    if(!sql_query.exec(select_sql))
    {
        qDebug()<<sql_query.lastError();
    }
    else
    {
        qDebug()<<"插入成功";
    }
    updatadialog();

 }
void adddialog::updatadialog(){
    level <<" "<<"A"<<"B"<<"C"<<"D"<<"E";
    ui->levelchoseqianru->addItems(level);
    ui->levelchoseyiingyu->addItems(level);
    ui->lineEdit_Name->setText(addName);
    ui->lineEdit_Number->setText(addNumber);
    ui->lineEdit_profess->setText(addProfession);
    ui->lineEdit_class->setText(addClass);
    ui->lineEdit_weibo->setEnabled(true);
    ui->lineEdit_tongyuan->setEnabled(true);
}
void adddialog::inputFinish(){//添加成绩
    QStringList courseneed;
    courseneed<<"嵌入式系统设计"<<"微波技术与天线"<<"通信原理"<<"科技英语";
    addTelphone=ui->lineEdit_telphone->text();
    addYear=ui->lineEdit_year->text();
    addtongyaun=ui->lineEdit_tongyuan->text();
    addweibo=ui->lineEdit_weibo->text();
    addqianru=ui->levelchoseqianru->currentText();
    addyingyu=ui->levelchoseyiingyu->currentText();

    QStringList coursescore;
    coursescore<<addqianru<<addweibo<<addtongyaun<<addyingyu;
    if(ui->lineEdit_Name->text().isEmpty())
    {
        QMessageBox::critical(this,tr("错误"),tr("请添加信息！"));
        return;
    }

    QSqlQuery sql_query;       //数据库操作类
    QString  select_sql;
    QString courseID;

    if (newdb.open())
        newdb.close();
    if(!newdb.open())
    {
        qDebug() << "Error: Failed to connect database." << newdb.lastError();
        return;
    }
    else
    {
        //存入年份，电话
        QString add=QString("select * FROM t_stud_info where sName ='%1';").arg(addName);
        sql_query.exec(add);
//        QString addd=QString("update t_stud_info set sEnrolledYear = '%1' where sName = '%2'").arg(addYear,addName);
//        sql_query.exec(addd);
          select_sql = QString("update t_stud_info set sEnrolledYear = '%1' where sName = '%2'").arg(addYear,addName);
        select_sql = QString("update t_stud_info set sPhone= '%1' where sName = '%2'").arg(addTelphone,addName);

        if(!sql_query.exec(select_sql))
        {
            qDebug()<<sql_query.lastError();
        }
        else
        {
            qDebug()<<"年份，电话插入成功";
        }
        //存入成绩
        for(int i=0;i<courseneed.size();i++)
        {
         select_sql = QString("select courseID from t_courses where courseName='%1';").arg(courseneed.at(i));
            if(!sql_query.exec(select_sql))
        {
            qDebug()<<sql_query.lastError();
        }
            else
            {
                while(sql_query.next())//存课程ID
            {
                courseID= sql_query.value(0).toString();
            }
        sql_query.exec("select * FROM t_stud_course_info");
        select_sql = QString("insert into t_stud_course_info value ('','%1','%2','%3','','');").arg(addNumber,courseID,coursescore.at(i));

                if(!sql_query.exec(select_sql))
        {
            qDebug()<<sql_query.lastError();
        }
                else
                {
            qDebug()<<"成绩插入成功";
             }
            }
        }
        QMessageBox::information(NULL, tr("Info"),tr("插入成功"));
        emit addfinish();    //发送提交完成信号，关闭dialog框

    }
}

