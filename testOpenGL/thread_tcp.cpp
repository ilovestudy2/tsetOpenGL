#include "thread_tcp.h"
/*
 * ***************************************************************************************
 * @brief 钢丝绳检测软件，需配合探头与采集卡、放大板使用
 * @file thread_tcp.cpp
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

thread_tcp::thread_tcp(QObject *parent) : QThread(parent)
{
}

void thread_tcp::run()
{
    QTcpSocket_DAQ = new QTcpSocket();
    connect(QTcpSocket_DAQ, &QTcpSocket::connected,this,&thread_tcp::QTcpSocket_DAQ_connected_slots,Qt::DirectConnection);
    connect(QTcpSocket_DAQ, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),this,&thread_tcp::QTcpSocket_DAQ_connected_error_slots,Qt::DirectConnection);
    qDebug()<<"sucess";
    QTcpSocket_DAQ->connectToHost("192.168.1.10",1000);
    this->exec();
}
void thread_tcp::QTcpSocket_DAQ_connected_slots()
{
    qDebug()<<"sucess connecting";
    connect(QTcpSocket_DAQ, &QTcpSocket::readyRead,this,&thread_tcp::QTcpSocket_DAQ_readyRead_slots,Qt::DirectConnection);
    QTcpSocket_DAQ->write(&DAQ_socket_cmd,1);
}
void thread_tcp::QTcpSocket_DAQ_readyRead_slots()
{
    static int flag_number = 0;
    switch (flag_number)
    {
    case 0:
        DAQ_socket_cmd = 'L';
        flag_number++;
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
        DAQ_socket_cmd = 'A';
        flag_number++;
        break;
    case 10:
        DAQ_socket_cmd = 'H';
        flag_number++;
        break;
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
        DAQ_socket_cmd = 'A';
        flag_number++;
        break;
    case 19:
        DAQ_socket_cmd = 'A';
        flag_number = 0;
        break;
    }
    QByteArray array = QTcpSocket_DAQ->readAll();
    QTcpSocket_DAQ->write(&DAQ_socket_cmd,1);
    msleep(15);
    tcp_data_trans_together_func(array);
}
void thread_tcp::tcp_data_trans_together_func(QByteArray data_in_array)
{
    int data_return[160];
    int data_buff[640];
    int i = 0;
    for (i=0;i<640;i++)
    {
        data_buff[i] = data_in_array[2*i] + data_in_array[2*i+1]*256;
    }
    for (i=0;i<40;i++)
    {
        data_return[4*i] = data_buff[i*16+1]+data_buff[i*16+2]+data_buff[i*16+3]+data_buff[i*16+4]+data_buff[i*16+5]+data_buff[i*16+6]-10000;
        data_return[4*i+1] = data_buff[i*16+7]+data_buff[i*16+8]+data_buff[i*16+13]+data_buff[i*16+14]+data_buff[i*16+15]+data_buff[i*16+0]-10000;
        data_return[4*i+2] = data_buff[i*16+10];
        data_return[4*i+3] = data_buff[i*16+11];
    }
    emit sucessed_data_trans_togetner_socket(data_return);
}
void thread_tcp::QTcpSocket_DAQ_connected_error_slots()
{
    qDebug()<<"connect error!";
}
thread_tcp::~thread_tcp()
{
    QTcpSocket_DAQ->deleteLater();
}
