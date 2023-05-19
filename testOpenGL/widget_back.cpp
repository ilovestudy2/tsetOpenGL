#include "widget_back.h"
#include "ui_widget_back.h"
/*
 * ***************************************************************************************

 * **************************************
 * @attention
 *
 * 运行平台：正点原子家的i.mx6ull开发板、树莓派4B、windows10、11系统的电脑,实测可行
 * *************************************************************************************
**/
widget_back::widget_back(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::widget_back)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint);//无边框
    maxSize = 4000;
    maxX = graph_x_axis_Count;
    maxY = 6000;
    QChart_hall->addSeries(QSplineSeries_hall);
    QChart_hall->addSeries(QSplineSeries_hall2);
    QChart_hall->legend()->hide();
    QChart_hall->createDefaultAxes();
    QChart_hall->axisX()->setRange(0, maxX);
    QChart_hall->axisX()->hide();
    QChart_hall->axisY()->setRange(0, 4000);
    QChart_hall->setTheme(QtCharts::QChart::ChartThemeDark);

    ui->widget_hall->setChart(QChart_hall);
    ui->widget_hall->setRenderHint(QPainter::Antialiasing,true);
    QChart_coil->addSeries(QSplineSeries_coil);
    QChart_coil->addSeries(QSplineSeries_coil2);
    QChart_coil->legend()->hide();
    QChart_coil->createDefaultAxes();
    QChart_coil->axisX()->setRange(0, maxX);
    QChart_coil->axisX()->hide();
    QChart_coil->axisY()->setRange(-2000, maxY);
    QChart_coil->setTheme(QtCharts::QChart::ChartThemeDark);

    ui->widget_coil->setChart(QChart_coil);
    ui->widget_coil->setRenderHint(QPainter::Antialiasing,true);
    if (QVector_QPointF_coil_graph_data1.isEmpty())//开辟空间
    {
        QVector_QPointF_hall_graph_data1.reserve(graph_x_axis_Count);
        QVector_QPointF_hall_graph_data2.reserve(graph_x_axis_Count);
        QVector_QPointF_coil_graph_data1.reserve(graph_x_axis_Count);
        QVector_QPointF_coil_graph_data2.reserve(graph_x_axis_Count);
        for (int i = 0; i < graph_x_axis_Count; ++i)
        {

            QVector_QPointF_hall_graph_data1.append(QPointF(i, 0));
            QVector_QPointF_hall_graph_data2.append(QPointF(i, 0));
            QVector_QPointF_coil_graph_data1.append(QPointF(i, 0));
            QVector_QPointF_coil_graph_data2.append(QPointF(i, 0));
        }
        /*
        QSplineSeries_coil->replace(QVector_QPointF_coil_graph_data1);
        QSplineSeries_coil2->replace(QVector_QPointF_coil_graph_data2);
        QSplineSeries_hall->replace(QVector_QPointF_hall_graph_data1);
        QSplineSeries_hall2->replace(QVector_QPointF_hall_graph_data2);
        */
    }

}

widget_back::~widget_back()
{
    delete QSplineSeries_hall;
    delete QSplineSeries_hall2;
    delete QSplineSeries_coil;
    delete QSplineSeries_coil2;
    delete QChart_coil;
    delete QChart_hall;
    delete ui;
}

void widget_back::on_pushButton_quit_clicked()
{
    this->hide();
}

void widget_back::on_pushButton_choose_clicked()
{
    ui->label_axis_X1->setText(QString("%1ms").arg(0));
    ui->label_axis_X2->setText(QString("%1ms").arg(0+1600));
    ui->pushButton_choose->show();
    ui->pushButton_nextpage->show();
    ui->pushButton_lastpage->show();
    ui->pushButton_quit->show();
    QVector_int_read_data1.clear();
    QVector_int_read_data2.clear();
    QVector_int_read_data3.clear();
    QVector_int_read_data4.clear();
    QVector_int_read_data5.clear();
    QVector_int_read_data6.clear();
    QVector_int_read_data7.clear();
    QVector_int_read_data8.clear();
    QVector_int_read_data9.clear();
    QVector_int_read_data10.clear();
    QVector_int_read_data11.clear();
    QVector_int_read_data12.clear();
    QVector_int_read_data13.clear();
    QVector_int_read_data14.clear();
    QVector_int_read_data15.clear();
    QVector_int_read_data16.clear();
    QVector_double_read_data17.clear();
    path = QFileDialog::getOpenFileName(this,"open","./data_dir");
    qDebug()<<path;
    if(path.isEmpty() == false)
    {
        QFile_file.setFileName(path);
        bool file_open_sucess = QFile_file.open(QIODevice::ReadOnly);
        if(file_open_sucess == true)
        {
            //创建数据流，和file文件关联
            //往数据流中读数据，相当于往文件里读数据
            stream.setDevice(&QFile_file);
            //读的时候，按写的顺序取数据
            qint32 data1,data2,data3,data4,data5,data6,data7,data8,data9,data10,data11,data12,data13,data14,data15,data16;
            double data17;
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
            while(stream.atEnd() == false)
            {
                for(int i = 0 ; i < 640 ; i++){
                    //stream>>data16;
                    stream>>data1;
                    stream>>data2;
                    stream>>data3;
                    stream>>data4;
                    /*stream>>data5;
                    stream>>data6;
                    stream>>data7;
                    stream>>data8;
                    stream>>data9;
                    stream>>data10;
                    stream>>data11;
                    stream>>data12;
                    stream>>data13;
                    stream>>data14;
                    stream>>data15;*/
                    stream>>data17;
                    //qDebug()<<data1+data2+data3+data4+data5+data6-10000;
                    QVector_int_read_data1.append(data1);
                    QVector_int_read_data2.append(data2);
                    QVector_int_read_data3.append(data3);
                    QVector_int_read_data4.append(data4);
                    QVector_double_read_data17.append(data17);
                    /*QVector_int_read_data5.append(data5);
                    QVector_int_read_data6.append(data6);
                    QVector_int_read_data7.append(data7);
                    QVector_int_read_data8.append(data8);
                    QVector_int_read_data13.append(data13);
                    QVector_int_read_data14.append(data14);
                    QVector_int_read_data15.append(data15);
                    QVector_int_read_data16.append(data16);*/
                }
                //stream>>data17;
                //qDebug()<<data17;
                //QVector_double_read_data17.append(data17);
            }
        }
        data_length = QVector_int_read_data2.size();
        qDebug()<<"datalength:"<<data_length;
        if(data_length<=1600)//数据量较少
        {
            for(int i=0;i<data_length;i++)
            {
                 /*QVector_QPointF_coil_graph_data1[i].setY(QVector_int_read_data1[i]+QVector_int_read_data2[i]+QVector_int_read_data3[i]
                                                          +QVector_int_read_data4[i]+QVector_int_read_data5[i]+QVector_int_read_data6[i]
                                                          -10000);
                 QVector_QPointF_coil_graph_data2[i].setY(QVector_int_read_data7[i]+QVector_int_read_data8[i]+QVector_int_read_data13[i]
                                                          +QVector_int_read_data14[i]+QVector_int_read_data15[i]+QVector_int_read_data16[i]
                                                          -10000);*/
                QVector_QPointF_coil_graph_data1[i].setY(QVector_int_read_data1[i]);
                QVector_QPointF_coil_graph_data2[i].setY(QVector_int_read_data2[i]);
                QVector_QPointF_hall_graph_data1[i].setY(QVector_int_read_data3[i]);
                QVector_QPointF_hall_graph_data2[i].setY(QVector_int_read_data4[i]);
            }
            QSplineSeries_coil->replace(QVector_QPointF_coil_graph_data1);
            QSplineSeries_coil2->replace(QVector_QPointF_coil_graph_data2);
            QSplineSeries_hall->replace(QVector_QPointF_hall_graph_data1);
            QSplineSeries_hall2->replace(QVector_QPointF_hall_graph_data2);
            data_plot_pos = data_length;
            ui->pushButton_lastpage->hide();
            ui->pushButton_nextpage->hide();
            qDebug()<<"数据量少！";
        }
        else//数据量足够
        {
            for(int i=0;i<1600;i++)
            {
                /*QVector_QPointF_coil_graph_data1[i].setY(QVector_int_read_data1[i]+QVector_int_read_data2[i]+QVector_int_read_data3[i]
                                                         +QVector_int_read_data4[i]+QVector_int_read_data5[i]+QVector_int_read_data6[i]
                                                         -10000);
                QVector_QPointF_coil_graph_data2[i].setY(QVector_int_read_data7[i]+QVector_int_read_data8[i]+QVector_int_read_data13[i]
                                                         +QVector_int_read_data14[i]+QVector_int_read_data15[i]+QVector_int_read_data16[i]
                                                         -10000);*/
                QVector_QPointF_coil_graph_data1[i].setY(QVector_int_read_data1[i]);
                QVector_QPointF_coil_graph_data2[i].setY(QVector_int_read_data2[i]);
                QVector_QPointF_hall_graph_data1[i].setY(QVector_int_read_data3[i]);
                QVector_QPointF_hall_graph_data2[i].setY(QVector_int_read_data4[i]);
            }
            QSplineSeries_coil->replace(QVector_QPointF_coil_graph_data1);
            QSplineSeries_coil2->replace(QVector_QPointF_coil_graph_data2);
            QSplineSeries_hall->replace(QVector_QPointF_hall_graph_data1);
            QSplineSeries_hall2->replace(QVector_QPointF_hall_graph_data2);
            data_plot_pos = 1600;
            qDebug()<<"显示第一屏！";
            ui->pushButton_nextpage->show();
            ui->pushButton_lastpage->hide();
        }
        QFile_file.close();
    }
}

void widget_back::on_pushButton_lastpage_clicked()
{
    data_plot_pos = data_plot_pos - 1600;
    ui->label_axis_X1->setText(QString("%1ms").arg(data_plot_pos));
    ui->label_axis_X2->setText(QString("%1ms").arg(data_plot_pos+1600));
    if(data_plot_pos<=data_length)
    {
        for(int i=0;i<1600;i++)
        {
            QVector_QPointF_coil_graph_data1[i].setY(QVector_int_read_data1[data_plot_pos-1600+i]);
            QVector_QPointF_coil_graph_data2[i].setY(QVector_int_read_data2[data_plot_pos-1600+i]);
            QVector_QPointF_hall_graph_data1[i].setY(QVector_int_read_data3[data_plot_pos-1600+i]);
            QVector_QPointF_hall_graph_data2[i].setY(QVector_int_read_data4[data_plot_pos-1600+i]);
            /*QVector_QPointF_coil_graph_data1[i].setY(QVector_int_read_data1[data_plot_pos-1600+i]+QVector_int_read_data2[data_plot_pos-1600+i]+QVector_int_read_data3[data_plot_pos-1600+i]
                                                     +QVector_int_read_data4[data_plot_pos-1600+i]+QVector_int_read_data5[data_plot_pos-1600+i]+QVector_int_read_data6[data_plot_pos-1600+i]
                                                     -10000);
            QVector_QPointF_coil_graph_data2[i].setY(QVector_int_read_data7[data_plot_pos-1600+i]+QVector_int_read_data8[data_plot_pos-1600+i]+QVector_int_read_data13[data_plot_pos-1600+i]
                                                     +QVector_int_read_data14[data_plot_pos-1600+i]+QVector_int_read_data15[data_plot_pos-1600+i]+QVector_int_read_data16[data_plot_pos-1600+i]
                                                     -10000);*/
        }
        QSplineSeries_coil->replace(QVector_QPointF_coil_graph_data1);
        QSplineSeries_coil2->replace(QVector_QPointF_coil_graph_data2);
        QSplineSeries_hall->replace(QVector_QPointF_hall_graph_data1);
        QSplineSeries_hall2->replace(QVector_QPointF_hall_graph_data2);
    }

    if(data_plot_pos>=3200)
    {
        ui->pushButton_lastpage->show();
    }
    else ui->pushButton_lastpage->hide();
    if(data_plot_pos<data_length)
    {
        ui->pushButton_nextpage->show();
    }
    else ui->pushButton_nextpage->hide();
}
void widget_back::on_pushButton_nextpage_clicked()
{
    data_plot_pos = data_plot_pos + 1600;
    ui->label_axis_X1->setText(QString("%1ms").arg(data_plot_pos));
    ui->label_axis_X2->setText(QString("%1ms").arg(data_plot_pos+1600));
    if(data_plot_pos<=data_length)
    {
        for(int i=0;i<1600;i++)
        {
            QVector_QPointF_coil_graph_data1[i].setY(QVector_int_read_data1[data_plot_pos-1600+i]);
            QVector_QPointF_coil_graph_data2[i].setY(QVector_int_read_data2[data_plot_pos-1600+i]);
            QVector_QPointF_hall_graph_data1[i].setY(QVector_int_read_data3[data_plot_pos-1600+i]);
            QVector_QPointF_hall_graph_data2[i].setY(QVector_int_read_data4[data_plot_pos-1600+i]);
            /*QVector_QPointF_coil_graph_data1[i].setY(QVector_int_read_data1[data_plot_pos-1600+i]+QVector_int_read_data2[data_plot_pos-1600+i]+QVector_int_read_data3[data_plot_pos-1600+i]
                                                     +QVector_int_read_data4[data_plot_pos-1600+i]+QVector_int_read_data5[data_plot_pos-1600+i]+QVector_int_read_data6[data_plot_pos-1600+i]
                                                     -10000);
            QVector_QPointF_coil_graph_data2[i].setY(QVector_int_read_data7[data_plot_pos-1600+i]+QVector_int_read_data8[data_plot_pos-1600+i]+QVector_int_read_data13[data_plot_pos-1600+i]
                                                     +QVector_int_read_data14[data_plot_pos-1600+i]+QVector_int_read_data15[data_plot_pos-1600+i]+QVector_int_read_data16[data_plot_pos-1600+i]
                                                     -10000);
            QVector_QPointF_coil_graph_data1[i].setY(QVector_int_read_data1[data_plot_pos-1600+i]);
            QVector_QPointF_coil_graph_data2[i].setY(QVector_int_read_data2[data_plot_pos-1600+i]);*/
        }
        QSplineSeries_coil->replace(QVector_QPointF_coil_graph_data1);
        QSplineSeries_coil2->replace(QVector_QPointF_coil_graph_data2);
        QSplineSeries_hall->replace(QVector_QPointF_hall_graph_data1);
        QSplineSeries_hall2->replace(QVector_QPointF_hall_graph_data2);
    }


    if(data_plot_pos>=3200)
    {
        ui->pushButton_lastpage->show();
    }
    else ui->pushButton_lastpage->hide();
    if(data_plot_pos<data_length)
    {
        ui->pushButton_nextpage->show();
    }
    else ui->pushButton_nextpage->hide();
}
