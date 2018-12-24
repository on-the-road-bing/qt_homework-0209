#include "scorewindow.h"
#include "ui_scorewindow.h"
#include "adddialog.h"

#include <QMessageBox>
#include <QString>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <qDebug>
#include <QFileDialog>
#include <QTextStream>

ScoreWindow::ScoreWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ScoreWindow)
{
    ui->setupUi(this);

    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("");
    db.setDatabaseName("test");

    ui->rtn_name->setChecked(true);

    connect(ui->btnQuery,&QPushButton::clicked,this,&ScoreWindow::on_btnQuery_clicked);
    connect(ui->actinput,&QAction::triggered,this,&ScoreWindow::on_actinput_clicked);
    connect(ui->actfileinput,&QAction::triggered,this,&ScoreWindow::on_actfileinput_clicked);
    connect(ui->actoutput,&QAction::triggered,this,&ScoreWindow::on_actoutput_clicked);
}

ScoreWindow::~ScoreWindow()
{
    db.close();
    delete ui;
}

void ScoreWindow::queryStudentInfo(QString querStri){  //查询学生信息
    QSqlQuery query;
    if(!query.exec(querStri))
    {
        qDebug()<<query.lastError();
    }
    else
    {
        while (query.next()) {
        snumber = query.value(0).toString();
        sname = query.value(1).toString();
        sclass = query.value(3).toString();
        sgrade = query.value(2).toString();
    }
        qDebug() << sname << sclass <<sgrade <<snumber;
}
}
void ScoreWindow::queryStudentScore(QString queryStrscore){ //查询分数
    QSqlQuery query;
    queryStrscore = QString("select t_stud_info.sName as '姓名',t_courses.courseName as '课程名称',"
                            "t_stud_course_info.scScores as '成绩',t_courses.courseCredit as '学分' "
                            "from "
                            "(t_stud_course_info inner join t_courses on t_stud_course_info.scCourseID = t_courses.courseID) "
                            "inner join t_stud_info on t_stud_info.sNumber = t_stud_course_info.scNumber "
                            "%1 "
                            "order by t_stud_course_info.scNumber "
                            ";").arg(str);
    if(!query.exec(queryStrscore))
    {
        qDebug()<<query.lastError();
    }
    else
    {
    while (query.next()) {
    scname << query.value(0).toString();//"赵静", "赵静", "赵静", "赵静", "赵静", "赵静", "赵静"
    scclassname << query.value(1).toString();//("模拟电子线路课程设计", "模拟电子线路", "信号与系统", "毛泽东思想和中国特色社会主义理论体系概论", "大学物理B-2", "复变函数", "大学体育-3")
    scscore << query.value(2).toString();//("良好", "70", "60", "81", "47", "62", "40")
    sccredit << query.value(3).toString();//("2", "5", "4", "3", "3", "3", "1")

    }
    qDebug() << scname << scclassname <<scscore <<sccredit;
}
}

QString ScoreWindow::calculatePoints(QStringList scorepoint,QStringList creditpoint){//计算绩点
    QStringList leaveList;
    leaveList<<"优秀"<<"良好"<<"中等"<<"及格"<<"不及格"<<"缓考"<<"旷考";
    QStringList leavelist;
    leavelist<<"A"<<"B"<<"C"<<"D"<<"E";
    double point=0;
    double sum=0;
    for(int i=0;i<scorepoint.size();i++)
    {
        QString myscore=scorepoint.at(i);
        QString mycredit=creditpoint.at(i);
        qDebug()<<myscore<<mycredit;
        if (leaveList.contains(scorepoint.at(i)))//是否含有字符串
        {
            switch (leaveList.indexOf(scorepoint.at(i)))
            {
            case 0:                     // 优秀
                myscore= "95";
                break;
            case 1:                     // 良好
                myscore="85";
                break;
            case 2:                     // 中等
                myscore="75";
                break;
            case 3:                     // 及格
                myscore="65";
                break;
            case 4:                     // 不及格
                myscore="0";
                break;
            case 5:                     // 缓考
                myscore="0";
                break;
            case 6:                     // 旷考
                myscore="0";
                break;
            default:
                break;
            }

        if (leavelist.contains(scorepoint.at(i)))//是否含有字符串
        {
            switch (leavelist.indexOf(scorepoint.at(i)))
            {
            case 0:                     // A
                myscore="95";
                break;
            case 1:                     // B
                myscore="85";
                break;
            case 2:                     // C
                myscore="75";
                break;
            case 3:                     // D
                myscore="65";
                break;
            case 4:                     // E
                myscore="0";
                break;
            default:
                break;
            }
        }
        }
        if(!myscore.toDouble())
            point+=0;
        else
            point+=((myscore.toDouble()/10-5)*mycredit.toDouble());
        sum+=mycredit.toDouble();
    }
    double avg=point/sum;
    QString avgpoint = QString::number(avg);
    qDebug ()<< avgpoint;
    return avgpoint;
}
void ScoreWindow::updateInfoWindow(){
    QString temp;

    //cellpadding规定单元边沿与其内容之间的空白。
    //align规定表格相对周围元素的对齐方式。
    //border规定表格边框的宽度。
    temp.append("<table border='1' cellpadding='10' align='center' >  <tr>    <td colspan='4'align='center' bgcolor='#9DCEFF' >");
    if(!scname.size()) return;
    temp.append(scname.at(0));

    temp.append("</td>      </tr>");
    temp.append(" <tr>   <td width='80' >序号</td>   <td>课程名称</td>   <td>成绩</td>    <td>学分</td>      </tr> ");

    for(int i=0;i<scname.size();i++)
    {
        temp.append(" <tr>   <td> ");
        temp.append(QString::number(i+1));
        temp.append("</td>   <td>");
        temp.append(scclassname.at(i));
        temp.append("</td>   <td>");
        temp.append(scscore.at(i));
        temp.append("</td>   <td>");
        temp.append(sccredit.at(i));
        temp.append("</td>    </tr> ");

    }
    temp.append("<tr bgcolor='#9DCEFF' >     <td colspan='4'align='center' >");
    temp.append("平均绩点为");
    temp.append(calculatePoints(scscore,sccredit));
    temp.append("</td>      </tr></table>");

    ui->textEdit->clear();
    ui->textEdit->append(temp);
    ui->edtStNumber->setText(snumber);
    ui->edtStName->setText(sname);
    ui->edtStClass->setText(sclass);
    ui->edtStGrade->setText(sgrade);
    scname.clear();
    scclassname.clear();
    sccredit.clear();
    scscore.clear();
}


void ScoreWindow::on_btnQuery_clicked()
{
    if(db.isOpen())
        db.close();

    if(!db.open()){
        QMessageBox::critical(this,tr("错误"),tr("无法连接数据库！ \n请检查数据库连接配置"));
        return;
    }else{
        if(ui->rtn_num->isChecked())
            str=QString("where sNumber='%1'").arg(ui->edtinput->text());
        else if(ui->rtn_name->isChecked())
            str=QString("where sName='%1'").arg(ui->edtinput->text());
        else if(ui->rtn_all->isChecked())
            str=" ";
        QString queryStr = QString ("select * FROM t_stud_info %1;").arg(str);
        queryStudentInfo(queryStr);
        queryStudentScore(queryStr);
        calculatePoints(scscore,scname);
        updateInfoWindow();
        db.close();
    }
}

void ScoreWindow::on_actinput_clicked(){
    dialog = new adddialog(this);
    connect(dialog,&adddialog::addfinish,this,&ScoreWindow::closedialog);
    dialog->show();
    dialog->setRequestdb(db);
}
void ScoreWindow::on_actfileinput_clicked(){
    /*系统调用文件选择框*/
    path = QFileDialog::getOpenFileName(this, tr("Open csv"), ".", tr("csv Files(*.csv)"));
    if(path.length() == 0) {
        return;
        QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));
    } else {
        QMessageBox::information(NULL, tr("Path"), tr("You selected ") + path);
    }
    QStringList  tempText;
    QFile file(path);
    qDebug()<<path;
    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::critical(NULL,tr("错误"),tr("打开文件失败！"));
        return;
    }
    QTextStream out(&file);
    while(!out.atEnd()){
        tempText.append(out.readLine().trimmed());
    }

    file.close();
    if(tempText.isEmpty())
        return;
    if(db.isOpen())
    {
        db.close(); //重启db
    }
    if(!db.open())
    {
        qDebug() << "Error: Failed to connect database." << db.lastError();
        return;
    }
    else{
        QString tempdata;
        QStringList singledata;
        QString  select_sql;
       QString  select_sql2;
        QStringList signalcourseID;
        QSqlQuery sql_query;       //数据库操作类

        tempdata=tempText.at(1);
        singledata=tempdata.split(',');
        qDebug()<<singledata;

        qDebug()<<singledata.size();

        for(int i=0;i<4;i++){
            select_sql = QString("select courseID from t_courses where courseName='%1';").arg(singledata.at(5+i));

            if(!sql_query.exec(select_sql))
            {
                qDebug()<<sql_query.lastError();
            }
            else
            {
                while(sql_query.next())
                {
                    signalcourseID<<sql_query.value(0).toString();

                }

            }
        }
        qDebug()<<signalcourseID;

        for(int i=1;i<tempText.size();i++)
        {
            tempdata=tempText.at(i);
            singledata=tempdata.split(',');
            qDebug()<<singledata;
            select_sql2 = QString("insert into t_stud_info value ('%1','%2','%3','%4','','');").arg(singledata.at(1),singledata.at(2),singledata.at(3),singledata.at(4));
            sql_query.exec(select_sql2);
            qDebug()<<singledata.at(1)<<singledata.at(2)<<singledata.at(3)<<singledata.at(4);

            for(int j=0;j<4;j++){

                select_sql = QString("insert into t_stud_course_info value ('','%1','%2','%3','','');").arg(singledata.at(1),signalcourseID.at(j),singledata.at(5+j));
                qDebug()<<singledata.at(1)<<signalcourseID.at(j)<<singledata.at(5+j);
                if(!sql_query.exec(select_sql))
                {
                    qDebug()<<sql_query.lastError();
                }
                else
                {
                    qDebug()<<"插入成功";
                }
            }
        }
        QMessageBox::information(NULL, tr("Info"),tr("导入成功"));
    }
    OutputTheInfo("t_courses","t_courses.csv");
    OutputTheInfo("t_stud_course_info","t_stud_course_info.csv");
    OutputTheInfo("t_stud_info","t_stud_info.csv");
}

void ScoreWindow::on_actoutput_clicked(){
    QSqlDatabase ndb = QSqlDatabase::addDatabase("QSQLITE","db2");
    ndb.setDatabaseName("zqqdb.sqlite3");
    if(!ndb.open())
    {
        QMessageBox::critical(NULL,tr("错误"),tr("数据库连接失败或超时！"),QMessageBox::Cancel);

        return;
    }
      QFile file1("t_courses.csv"); //打开csv文件
          if(!file1.open(QIODevice::ReadOnly | QIODevice::Text))
           {
           QMessageBox::critical(NULL,tr("错误"),tr("文件打开失败！"),QMessageBox::Cancel);
                return;
           }

          QSqlQuery query1(ndb);  //默认打开
          query1.exec("create table t_courses (courseID int primary key, courseName varchar(25),"
                     "courseCredit int,courseType int)");

          QStringList list1;
          list1.clear();

          QTextStream in1(&file1);  //QTextStream读取数据

          QTextCodec *codec1=QTextCodec::codecForName("utf-8");
          in1.setCodec(codec1);
          while(!in1.atEnd())
          {
             QString fileLine = in1.readLine();  //从第一行读取至下一行
             list1 = fileLine.split(",", QString::SkipEmptyParts);
             query1.prepare("INSERT INTO t_courses (courseID, courseName,courseCredit,courseType) VALUES (:1,:2,:3,:4)"); //准备执行SQL查询
             query1.bindValue(":1", list1.at(0));   //绑定要插入的值
             query1.bindValue(":2", list1.at(1));
             query1.bindValue(":3", list1.at(2));
             query1.bindValue(":4", list1.at(3));

             query1.exec();
          }
          qDebug()<<"成功嵌入";
          query1.clear();

          QFile file2("t_stud_course_info.csv"); //打开csv文件
              if(!file2.open(QIODevice::ReadOnly | QIODevice::Text))
               {
               QMessageBox::critical(NULL,tr("错误"),tr("文件打开失败！"),QMessageBox::Cancel);
                    return;
               }

              QSqlQuery query2(ndb);  //默认打开
              query2.exec("create  table t_stud_course_info (ID int primary key, scNumber int,"
                         "scCourseID int,scScores int,scYear varchar(25),scTerm int)");

              QStringList list2;
              list2.clear();

              QTextStream in2(&file2);  //QTextStream读取数据

              QTextCodec *codec2=QTextCodec::codecForName("utf-8");
              in2.setCodec(codec2);
              while(!in2.atEnd())
              {
                 QString fileLine = in2.readLine();  //从第一行读取至下一行
                 list2 = fileLine.split(",", QString::SkipEmptyParts);


                 query2.prepare("INSERT INTO t_stud_course_info (ID, scNumber,scCourseID,scScores,scYear,scTerm) VALUES (:1,:2,:3,:4,:5,:6)"); //准备执行SQL查询
                 query2.bindValue(":1", list2.at(0));   //绑定要插入的值
                 query2.bindValue(":2", list2.at(1));
                 query2.bindValue(":3", list2.at(2));
                 query2.bindValue(":4", list2.at(3));
                 query2.bindValue(":5", list2.at(4));
                 query2.bindValue(":6", list2.at(5));

                 query2.exec();
              }
              qDebug()<<"成功嵌入";
              query2.clear();

              QFile file3("t_stud_info.csv"); //打开csv文件
                  if(!file3.open(QIODevice::ReadOnly | QIODevice::Text))
                   {
                   QMessageBox::critical(NULL,tr("错误"),tr("文件打开失败！"),QMessageBox::Cancel);
                        return;
                   }



                  QSqlQuery query3(ndb);  //默认打开
                  query3.exec("create table t_stud_info (sNumber int primary key, sName varchar(25),"
                             "sMajor varchar(25),sClass varchar(25),sEnrolledYear int,sPhone int)");

                  QStringList list3;
                  list3.clear();

                  QTextStream in3(&file3);  //QTextStream读取数据

                  QTextCodec *codec3=QTextCodec::codecForName("utf-8");
                  in3.setCodec(codec3);
                  while(!in3.atEnd())
                  {
                     QString fileLine = in3.readLine();  //从第一行读取至下一行
                     list3 = fileLine.split(",", QString::SkipEmptyParts);

                     query3.prepare("INSERT INTO t_stud_info (sNumber, sName,sMajor,sClass,sEnrolledYear,sPhone) VALUES (:1,:2,:3,:4,:5,:6)"); //准备执行SQL查询
                     query3.bindValue(":1", list3.at(0));   //绑定要插入的值
                     query3.bindValue(":2", list3.at(1));
                     query3.bindValue(":3", list3.at(2));
                     query3.bindValue(":4", list3.at(3));
                     query3.bindValue(":5", list3.at(4));
                     query3.bindValue(":6", list3.at(5));

                     query3.exec();
                  }
                  qDebug()<<"成功嵌入";
                  query3.clear();

          ndb.close();
           QMessageBox::information(NULL, tr("Info"),tr("导出成功"));
          return;
    }
//将表导出为csv格式表格
void ScoreWindow::OutputTheInfo(const QString &tableName,const QString &csvFileName)
    {
        QSqlTableModel *exportModel = new QSqlTableModel();
    //    exportModel->setTable("student");
        exportModel->setTable(tableName);
        exportModel->select();
        QStringList strList;//记录数据库中的一行报警数据
        QString strString;
        const QString FILE_PATH(csvFileName);
    //    const QString FILE_PATH("student.csv");
        QFile csvFile(FILE_PATH);
        if (csvFile.open(QIODevice::ReadWrite))
        {
            for (int i=0;i<exportModel->rowCount();i++)
            {
                for(int j=0;j<exportModel->columnCount();j++)
                {
                    strList.insert(j,exportModel->data(exportModel->index(i,j)).toString());//把每一行的每一列数据读取到strList中
                }
                strString = strList.join(", ")+"\n";//给两个列数据之前加“,”号，一行数据末尾加回车
                strList.clear();//记录一行数据后清空，再记下一行数据
                csvFile.write(strString.toUtf8());//使用方法：转换为Utf8格式后在windows下的excel打开是乱码,可先用notepad++打开并转码为unicode，再次用excel打开即可。
                qDebug()<<strString.toUtf8();
            }
            csvFile.close();
        }
    }

void ScoreWindow::closedialog(){
     dialog->close();
}


