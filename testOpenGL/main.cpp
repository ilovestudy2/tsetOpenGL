#include "mywidget.h"
#include <QApplication>
/*
 * ***************************************************************************************
 * @brief 钢丝绳检测软件，需配合探头与采集卡、放大板使用
 * @file main.cpp
 * @author 张宏伟
 * @E-mail:bazhangya@hotmail.com
 * @QQ:515615456
 * @version V1.0
 * @data 2021-11-15
 * **************************************
 * @attention
 *
 * 运行平台：正点原子家的i.mx6ull开发板、树莓派4B、windows10、11系统的电脑,实测可行
 * *************************************************************************************
**/
int main(int argc, char *argv[])
{
    //qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));//这句话调用Qt虚拟键盘
    QApplication app(argc, argv);
    MyWidget MyWidget_main;
    MyWidget_main.show();
    return app.exec();
}
