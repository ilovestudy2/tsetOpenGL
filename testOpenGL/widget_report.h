#ifndef WIDGET_REPORT_H
#define WIDGET_REPORT_H

#include <QWidget>
#include<QPushButton>
#include<QDebug>
#include<QMessageBox>
#include<QDir>
#include<QFileDialog>
#include<QDataStream>
/*
 * ***************************************************************************************
 * @brief 钢丝绳检测软件，需配合探头与采集卡、放大板使用
 * @file widget_report.h
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
namespace Ui {
class widget_report;
}

class widget_report : public QWidget
{
    Q_OBJECT

public:
    explicit widget_report(QWidget *parent = nullptr);
    ~widget_report();
private:
    Ui::widget_report *ui;
private slots:
    void on_pushButton_report_view_clicked();
    void on_pushButton_report_quit_clicked();
public:
    qint32 channel0 = 0;
    qint32 channel1 = 0;
    qint32 channel2 = 0;
    qint32 channel3 = 0;
    qint32 channel4 = 0;
    qint32 channel5 = 0;
    qint32 channel6 = 0;
    qint32 channel7 = 0;
    qint32 channel8 = 0;
    qint32 channel9 = 0;
    qint32 channel10 = 0;
    qint32 channel11 = 0;
    qint32 channel12 = 0;
    qint32 channel13 = 0;
    qint32 channel14 = 0;
    qint32 channel15 = 0;
    double channel16 = 0;
    int int_show_up_limit = 6000;
    int int_show_down_limit = -2000;
    int int_alarm_up_limit = 4000;
    int int_alarm_down_limit = -2000;
    int report_interval = 1000;
};
#endif // WIDGET_REPORT_H
