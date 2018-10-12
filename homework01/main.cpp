#include <QCoreApplication>
#include <QDebug>
#include <QList>

//博客：https://njuferret.github.io/2018/09/03/cpp_macro_usage/#more

#ifndef COMDEF_H  //防止一个头文件被重复包含（博客）
#define COMDEF_H

#define  v 0x12345678 //定义字

#define _STR(s)     #s  // 转换宏 (博客)
#define STR(s)      _STR(s)

#define  hhi(v)  ( (v) >> 24 ) //右移，得到4段字节（博客）
#define  lhi(v)  ( (v) >> 16 )
#define  hli(v)  ( (v) >> 8 )
#define  lli(v)    (v)

#define  MAX(x, y)  ( ((x) > (y)) ? (x) : (y) )  //取最大最小值（博客）
#define  MIN(x, y)  ( ((x) < (y)) ? (x) : (y) )

#endif

int main(int argc, char *argv[])
{
    int vnew;
    QList <qint8>  values;

    qDebug()<<"原始值："<< STR(v) << "==" << v <<endl;

  //1.取出对象并保存到数组
    values.append ( hhi(v) ) ; //将参数放入values(assistant)
    values.append ( lhi(v) ) ;
    values.append ( hli(v) ) ;
    values.append ( lli(v) ) ;

    qDebug("0x%x == %d",values.at(0),values.at(0)); //输出16进制和十进制（https://blog.csdn.net/lyh__521/article/details/46986287）
    qDebug("0x%x == %d",values.at(1),values.at(1));
    qDebug("0x%x == %d",values.at(2),values.at(2));
    qDebug("0x%x == %d",values.at(3),values.at(3));

  //2.宏定义比较大小
    qDebug("最高8位和次高8位最大值：0x%x == %d", MAX(values.at(0),values.at(1)),MAX(values.at(0),values.at(1)));
    qDebug("次低8位和最低8位最小值：0x%x == %d", MIN(values.at(2),values.at(3)),MIN(values.at(2),values.at(3)));

  //3.组成新数组 56127834(2031)
    vnew=(values.at(2)<<24)+(values.at(0)<<16)+(values.at(3)<<8)+values.at(1);
    qDebug("重新组合后数值：0x%x (%d)",vnew,vnew);

  //4.从大到小，降序排序
    qDebug()<<"排序前："<<values<<endl; //输出排序前

    std::sort(values.begin(), values.end(), [](int a, int b) {return b < a;});  //https://zh.cppreference.com/w/cpp/algorithm/sort
   //另一表示： std::sort(values.begin(), values.end(), std::greater<int>());

    qDebug()<<"排序后："<<values<<endl; //输出排序后

}
