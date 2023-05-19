#ifndef WIDGET_SET_H
#define WIDGET_SET_H

#include <QWidget>
#include<QDebug>
#include<QDir>
#include<QFileDialog>
#include<QDataStream>
#include<QFileDialog>
#include<QDataStream>
#include<QFile>
#include<QMessageBox>
#include <QTimer> //定时器对象
//QFile::copy
/*
 * ***************************************************************************************
 * @brief 钢丝绳检测软件，需配合探头与采集卡、放大板使用
 * @file widget_set.h
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
class Widget_set;
}

class Widget_set : public QWidget
{
    Q_OBJECT

public:
    explicit Widget_set(QWidget *parent = nullptr);
    ~Widget_set();

signals:
    void signals_bn_set_save(int int_show_up_limit,int int_show_down_limit,int int_alarm_up_limit,int int_alarm_down_limit,float float_distance_coefficient,QString base_name); //启动子线程的信号
private slots:
    void on_pushButton_set_save_clicked();
    void on_pushButton_set_quit_clicked();
    void on_pushButton_set_filedelete_clicked();
    void on_pushButton_demar_clicked();

    void on_pushButton_copyfile_clicked();

public slots:
    void QTimer_setting_slots();

private:
    Ui::Widget_set *ui;
public:
    int int_show_up_limit = 6000;
    int int_show_down_limit = -2000;
    int int_alarm_up_limit = 4000;
    int int_alarm_down_limit = 0;
    QString base_name;
    float float_distance_coefficient = 1.01;
    QDir QDir_data_set_dir;
    QFile QFile_data_set_file;
    QDataStream QDataStream_set_data;
    QTimer QTimer_setting;//定时器设置文件
public:
    bool copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist);
};

#endif // WIDGET_SET_H
