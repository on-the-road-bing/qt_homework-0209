#include <QCoreApplication>
#include <iostream>
#include <QDebug>
#include <algorithm>
#include <QVector>
using namespace std;

//https://blog.csdn.net/lu597203933/article/details/41776701
//https://blog.csdn.net/zlhhsh/article/details/27389895
//https://zh.cppreference.com/w/cpp/container/vector
//https://zh.cppreference.com/w/cpp/algorithm/sort

class student
{
public:
   int num;
   QString name;
   int class1;
   int class2;

};

void s(QVector<student> &vec,int num,QString name,int class1,int class2)
{
    student stu;
    stu.num=num;
    stu.name=name;
    stu.class1=class1;
    stu.class2=class2;
    vec.push_back(stu);
}


bool sortname(const student &s1, const student &s2) //按姓名排序
    {return s1.name < s2.name;}
bool sortclass1(const student &s1, const student &s2) //按课程一排序
    {return s1.class1 > s2.class1;}
bool sortclass2(const student &s1, const student &s2) //按课程二排序
    {return s1.class2 > s2.class2;}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QVector<student> vec;
    s(vec,1403140103,"武松",88,80);
    s(vec,1403140101,"林冲",82,76);
    s(vec,1403130209,"鲁智深",80,72);
    s(vec,1403140102,"宋江",76,85);

    QVector<student>::iterator it;    //迭代器 iterato,输出

    qDebug()<< "排序前" << endl;
    for (it = vec.begin(); it != vec.end(); it++)
        {
           qDebug()<< it ->num << it->name << it->class1 << it->class2 <<endl;
        }

     sort(vec.begin(),vec.end(),sortname);
     qDebug()<< "按姓名排序" << endl;
     for (it = vec.begin(); it != vec.end(); it++)
         {
            qDebug()<< it ->num << it->name << it->class1 << it->class2 <<endl;
         }

     sort(vec.begin(),vec.end(),sortclass1);
     qDebug()<< "按课程一成绩排序" << endl;
     for (it = vec.begin(); it != vec.end(); it++)
         {
            qDebug()<< it ->num << it->name << it->class1 << it->class2 <<endl;
         }

     sort(vec.begin(),vec.end(),sortclass2);
     qDebug()<< "按课程二成绩排序" << endl;
     for (it = vec.begin(); it != vec.end(); it++)
         {
            qDebug()<< it ->num << it->name << it->class1 << it->class2 <<endl;
         }

    return a.exec();
}
