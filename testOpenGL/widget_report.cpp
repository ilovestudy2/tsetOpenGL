#include "widget_report.h"
#include "ui_widget_report.h"
/*
 * ***************************************************************************************
 * @brief 钢丝绳检测软件，需配合探头与采集卡、放大板使用
 * @file widget_report.cpp
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
widget_report::widget_report(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::widget_report)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);//无边框
    ui->textEdit_report->setText("\n请先选择想要统计的检测数据\n");
}

widget_report::~widget_report()
{
    delete ui;
}
void widget_report::on_pushButton_report_view_clicked()
{
    ui->pushButton_report_quit->hide();
    ui->pushButton_report_view->hide();
    ui->textEdit_report->setText("\n请先选择想要统计的检测数据\n");
    int defect_number = 0;
    QString path = QFileDialog::getOpenFileName(this,"open","./data_dir");
    qDebug()<<path;
    if(path.isEmpty() == false)
    {
        ui->textEdit_report->clear();
        ui->textEdit_report->append("正在查找损伤,请等待结果...");
        QFile QFile_file(path);
        bool file_open_sucess = QFile_file.open(QIODevice::ReadOnly);
        if(file_open_sucess == true)
        {
            //创建数据流，和file文件关联
            //往数据流中读数据，相当于往文件里读数据
            QDataStream stream(&QFile_file);
            //读的时候，按写的顺序取数据
            int coil1,coil2;
            double distance;
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
            while(stream.atEnd() == false)
            {
                //stream >> channel0>>channel1>>channel2>>channel3>>channel4>>channel5>>channel6>>channel7>>channel8>>channel9>>channel10>>channel11>>channel12>>channel13>>channel14>>channel15>>channel16;
                stream >> channel0>>channel1>>channel2>>channel3>>channel16;
                coil1 = channel0;
                coil2 = channel1;
                distance = channel16;
                //qDebug() <<coil1;
                if(coil1>int_alarm_up_limit)
                {
                    defect_number++;
                    qDebug() <<distance;
                    ui->textEdit_report->append(QString("检测到%1m处有损伤").arg(distance));
                    for (int i =0;i<report_interval;i++)
                    {
                        if(stream.atEnd() == false)
                        {
                            stream >> channel0>>channel1>>channel2>>channel3>>channel16;
                            coil1 = channel0;
                            coil2 = channel1;
                            distance = channel16;
                            /*stream >> channel0>>channel1>>channel2>>channel3>>channel4>>channel5>>channel6>>channel7>>channel8>>channel9>>channel10>>channel11>>channel12>>channel13>>channel14>>channel15>>channel16;
                            coil1 = channel1+channel2+channel3+channel4+channel5+channel6-10000;
                            coil2 = channel13+channel14+channel15+channel0+channel7+channel8-10000;
                            distance = channel16;*/
                        }
                        else break;
                    }
                }
                else if(coil2>int_alarm_up_limit)
                {
                    defect_number++;
                    qDebug() <<distance;
                    ui->textEdit_report->append(QString("检测到%1m处有损伤").arg(distance));
                    for (int i =0;i<report_interval;i++)
                    {
                        if(stream.atEnd() == false)
                        {
                            stream >> channel0>>channel1>>channel2>>channel3>>channel16;
                            coil1 = channel0;
                            coil2 = channel1;
                            distance = channel16;
                            /*stream >> channel0>>channel1>>channel2>>channel3>>channel4>>channel5>>channel6>>channel7>>channel8>>channel9>>channel10>>channel11>>channel12>>channel13>>channel14>>channel15>>channel16;
                            coil1 = channel1+channel2+channel3+channel4+channel5+channel6-10000;
                            coil2 = channel13+channel14+channel15+channel0+channel7+channel8-10000;
                            distance = channel16;*/
                        }
                        else break;
                    }
                }
                else if(coil1<int_alarm_down_limit)
                {
                    defect_number++;
                    qDebug() <<distance;
                    ui->textEdit_report->append(QString("检测到%1m处有损伤").arg(distance));
                    for (int i =0;i<report_interval;i++)
                    {
                        if(stream.atEnd() == false)
                        {
                            stream >> channel0>>channel1>>channel2>>channel3>>channel16;
                            coil1 = channel0;
                            coil2 = channel1;
                            distance = channel16;
                            /*stream >> channel0>>channel1>>channel2>>channel3>>channel4>>channel5>>channel6>>channel7>>channel8>>channel9>>channel10>>channel11>>channel12>>channel13>>channel14>>channel15>>channel16;
                            coil1 = channel1+channel2+channel3+channel4+channel5+channel6-10000;
                            coil2 = channel13+channel14+channel15+channel0+channel7+channel8-10000;
                            distance = channel16;*/
                        }
                        else break;
                    }
                }
                else if(coil2<int_alarm_down_limit)
                {
                    defect_number++;
                    qDebug() <<distance;
                    ui->textEdit_report->append(QString("检测到%1m处有损伤").arg(distance));
                    for (int i =0;i<report_interval;i++)
                    {
                        if(stream.atEnd() == false)
                        {
                            stream >> channel0>>channel1>>channel2>>channel3>>channel16;
                            coil1 = channel0;
                            coil2 = channel1;
                            distance = channel16;
                            /*stream >> channel0>>channel1>>channel2>>channel3>>channel4>>channel5>>channel6>>channel7>>channel8>>channel9>>channel10>>channel11>>channel12>>channel13>>channel14>>channel15>>channel16;
                            coil1 = channel1+channel2+channel3+channel4+channel5+channel6-10000;
                            coil2 = channel13+channel14+channel15+channel0+channel7+channel8-10000;
                            distance = channel16;*/
                        }
                        else break;
                    }
                }
            }
            ui->pushButton_report_quit->show();
            ui->pushButton_report_view->show();
            //ui->textEdit_report->clear();
            if(defect_number ==0)
            {
                ui->textEdit_report->append("统计结束，钢丝绳完好无损伤！");
            }
            else
            {
                ui->textEdit_report->append(QString("共检测到%1处损伤！").arg(defect_number));
            }
            QFile_file.close();
        }
    }
    else
    {
        ui->pushButton_report_quit->show();
        ui->pushButton_report_view->show();
    }
}
void widget_report::on_pushButton_report_quit_clicked()
{
    ui->textEdit_report->setText("\n请先选择想要统计的检测数据\n");
}
