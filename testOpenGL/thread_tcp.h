#ifndef THREAD_TCP_H
#define THREAD_TCP_H

#include <QThread>
#include<QTcpServer>
#include<QTcpSocket>
/*
 * ***************************************************************************************
 * @brief 钢丝绳检测软件，需配合探头与采集卡、放大板使用
 * @file thread_tcp.h
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
class thread_tcp : public QThread
{
    Q_OBJECT
public:
    explicit thread_tcp(QObject *parent = nullptr);
    ~thread_tcp()override;
signals:

public slots:
    void QTcpSocket_DAQ_connected_slots();
    void QTcpSocket_DAQ_connected_error_slots();
    void QTcpSocket_DAQ_readyRead_slots();
protected:
    //QThread的虚函数
    //线程处理函数
    //不能直接调用，通过start()间接调用
    void run()override;
signals:
    void isDone();
    void sucessed_connected_socket();
    void failed_connected_socket();
    void sucessed_data_trans_togetner_socket(int data_return[]);
public:
    bool flag_thread_run = true;
    QVector<int> data_return;
    char DAQ_socket_cmd = 'L';
    QTcpSocket *QTcpSocket_DAQ; //采集卡通信套接字
public:////function
    void tcp_data_trans_together_func(QByteArray buff_array);
};
#endif // THREAD_TCP_H
