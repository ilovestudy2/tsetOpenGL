#include "mywidget.h"
#include "ui_mywidget.h"
/*
 * ***************************************************************************************

 * @attention
 *
 * 运行平台：正点原子家的i.mx6ull开发板、树莓派4B、windows10、11系统的电脑,实测可行
 * 电感通道1：1、2、3、 4、 5、 6
 * 电感通道2：7、8、13、14、15、0
 * 霍尔通道1：10
 * 霍尔通道2：11
 * 距离通道 ：9、12
 *
 * @log
 * 2021-11-24 添加损伤发送功能
 * *************************************************************************************
**/
/*
 * @brief 主窗口类构造函数，启动时先执行
 * @param 父类QWidget
 * @retval 无
*/
MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyWidget)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint);//无边框
    //this->resize(QSize(1920,1080));大小
    maxSize = 4000;
    maxX = graph_x_axis_Count;
    maxY = 6000;
    QSplineSeries_hall = new QLineSeries();
    QSplineSeries_hall2 = new QLineSeries();
    QChart_hall->addSeries(QSplineSeries_hall);
    QChart_hall->addSeries(QSplineSeries_hall2);
    QSplineSeries_hall->setUseOpenGL(true);
    QSplineSeries_hall2->setUseOpenGL(true);
    QChart_hall->legend()->hide();
    QChart_hall->createDefaultAxes();
    QChart_hall->axisX()->setRange(0, maxX);
    QChart_hall->axisX()->hide();
    QChart_hall->axisY()->setRange(0, 4000);
    QChart_hall->setTheme(QtCharts::QChart::ChartThemeDark);
    ui->widget_hall->setChart(QChart_hall);
    ui->widget_hall->setRenderHint(QPainter::Antialiasing,true);
    QSplineSeries_coil = new QLineSeries();//防止锯齿//QChart_coil->scroll(mx,my) mx my是x y轴的滚动量 QSplineSeries_coil->removePoints()
    QSplineSeries_coil2 = new QLineSeries();
    QSplineSeries_coil->setUseOpenGL(true);
    QSplineSeries_coil2->setUseOpenGL(true);
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
    connect(&QTimer_socket_connect_wait, &QTimer::timeout,this,&MyWidget::QTimer_socket_connect_wait_slots);
    connect(&QTimer_alarm_label,&QTimer::timeout,this,&MyWidget::QTimer_alarm_label_slots);
    connect(&QTcpSocket_DAQ, &QTcpSocket::connected,this,&MyWidget::QTcpSocket_DAQ_connected_slots);
    connect(&QTcpSocket_DAQ, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),this,&MyWidget::QTcpSocket_DAQ_connected_error_slots);
    connect(&QTcpSocket_cloud, &QTcpSocket::connected,this,&MyWidget::QTcpSocket_cloud_connected_slots);
    connect(&QTcpSocket_cloud, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),this,&MyWidget::QTcpSocket_cloud_connected_error_slots);
    connect(&Widget_set_win,&Widget_set::signals_bn_set_save,this,&MyWidget::slots_widget_set_handle);
    //connect(&thread_tcp_obj,&thread_tcp::sucessed_data_trans_togetner_socket,this,&MyWidget::thread_getdata_graph_plot,Qt::DirectConnection);
    QTimer_alarm_label.start(500);
    if (QVector_QPointF_coil_graph_data1.isEmpty())//开辟空间
    {
        QVector_QPointF_coil_graph_data1.reserve(graph_x_axis_Count);
        QVector_QPointF_coil_graph_data2.reserve(graph_x_axis_Count);
        QVector_QPointF_hall_graph_data1.reserve(graph_x_axis_Count);
        QVector_QPointF_hall_graph_data2.reserve(graph_x_axis_Count);
        for (int i = 0; i < graph_x_axis_Count; ++i)
        {
            QVector_QPointF_coil_graph_data1.append(QPointF(i, 0));
            QVector_QPointF_coil_graph_data2.append(QPointF(i, 0));
            QVector_QPointF_hall_graph_data1.append(QPointF(i, 0));
            QVector_QPointF_hall_graph_data2.append(QPointF(i, 0));
        }
    }
    qDebug()<<QDir_data_save_dir.currentPath();
    if(QDir_data_save_dir.exists("data_dir") == false)
    {
        QDir_data_save_dir.mkdir("data_dir");
    }
    //ui->pushButton_quit->hide();//回放功能未完成，隐藏按钮
}
/*
 * @brief 主窗口类析构函数，程序结束时最后执行
 * @param 无
 * @retval 无
*/
MyWidget::~MyWidget()
{
    delete QSplineSeries_coil;
    delete QSplineSeries_coil2;
    delete QSplineSeries_hall;
    delete QSplineSeries_hall2;
    delete QChart_coil;
    delete QChart_hall;
    delete ui;
}
/*
 * @brief 开始按钮的槽函数，点击开始按钮执行
 * @param 无
 * @retval 无
*/
void MyWidget::on_pushButton_start_clicked()
{
    qDebug()<<"start";
    QDateTime current_date_time =QDateTime::currentDateTime();//获取当前系统时间
    QString current_date =current_date_time.toString("yyyyMMddhhmmsszzzddd");
    QString QString_fliename = "./data_dir/"+current_date+".txt";
    qDebug()<<QString_fliename;
    QFile_data_save_file.setFileName(QString_fliename);//文件创建与打开
    bool file_open_sucess = QFile_data_save_file.open(QIODevice::WriteOnly);
    if(file_open_sucess == true)
    {
        QDataStream_detec_data.setDevice(&QFile_data_save_file);
    }
    if(QTimer_socket_connect_wait.isActive() == false)//防止连接时间太长
    {
        QTimer_socket_connect_wait.start(1000);
    }
    QTcpSocket_DAQ.abort();
    QTcpSocket_DAQ.connectToHost("192.168.1.10",1000,QAbstractSocket::ReadWrite, QAbstractSocket::IPv4Protocol);
    if(this->flag_DAQ_error == 1){
        this->flag_DAQ_error = 0;
        QFile_data_save_file.close();
        return;
    }
    detec_count = 0;
    ui->label_axisX1->setText(QString("%1ms").arg(detec_count));
    ui->label_axisX2->setText(QString("%1ms").arg(detec_count+1600));
    ui->pushButton_start->hide();
    ui->pushButton_back->hide();
    ui->pushButton_report->hide();
    ui->pushButton_set->hide();
    ui->pushButton_quit->hide();
}
/*
 * @brief 上传按钮的槽函数，点击开始执行
 * @param 无
 * @retval 无
*/
void MyWidget::on_pushButton_upload_clicked()
{
    static int cnt = 0;
    qDebug()<<"upload";
    if(cnt%2 == 0){
        QTcpSocket_cloud.connectToHost("118.178.187.119",8889,QAbstractSocket::ReadWrite, QAbstractSocket::IPv4Protocol);
        if(this->flag_cloud_error == 1){
            this->flag_cloud_error = 0;
            QTcpSocket_cloud.disconnectFromHost();
            QTcpSocket_cloud.close();
            flag_cloud_uploading = 0;
            cnt++;
            ui->pushButton_upload->setText("上传");
            return;
        }
        cnt++;
        ui->pushButton_upload->setText("停止");
    }
    else if(cnt%2 == 1){
        QTcpSocket_cloud.disconnectFromHost();
        QTcpSocket_cloud.close();
        flag_cloud_uploading = 0;
        cnt++;
        ui->pushButton_upload->setText("上传");
    }
}
/*
 * @brief 停止按钮的槽函数，点击开始执行
 * @param 无
 * @retval 无
*/
void MyWidget::on_pushButton_stop_clicked()
{
    qDebug()<<"stop";
    ui->pushButton_start->setText("检测");

    if(QTimer_socket_connect_wait.isActive() == true)
    {
        QTimer_socket_connect_wait.stop();
    }
    //主动和对方断开连接
    QTcpSocket_DAQ.disconnectFromHost();
    QTcpSocket_DAQ.close();
    int_defect_nubmer = 0;
    detec_count=0;
    ui->label_axisX1->setText(QString("%1ms").arg(detec_count));
    ui->label_axisX2->setText(QString("%1ms").arg(detec_count+1600));
    QFile_data_save_file.close();
    ui->pushButton_start->show();
    ui->pushButton_back->show();
    ui->pushButton_report->show();
    ui->pushButton_set->show();
    ui->pushButton_quit->show();
}
/*
 * @brief 报告按钮的槽函数，点击开始执行
 * @param 无
 * @retval 无
*/
void MyWidget::on_pushButton_report_clicked()
{
    qDebug()<<"report";
    widget_report_win.show();
}
/*
 * @brief 回放按钮的槽函数，点击开始执行
 * @param 无
 * @retval 无
*/
void MyWidget::on_pushButton_back_clicked()
{
    qDebug()<<"back";
    widget_back_win.show();
}
/*
 * @brief 设置按钮的槽函数，点击开始执行
 * @param 无
 * @retval 无
*/
void MyWidget::on_pushButton_set_clicked()
{
    qDebug()<<"set";
    Widget_set_win.show();
}
/*
 * @brief 退出按钮的槽函数，点击开始执行
 * @param 无
 * @retval 无
*/
void MyWidget::on_pushButton_quit_clicked()
{
    qDebug()<<"quit";
    QTimer_alarm_label.stop();
    this->close();
}
/*
 * @brief 参数设置的槽函数，设置页面点击保存按钮会自动执行，即emit信号之后自动执行
 * @param show_up_limit, show_down_limit,alarm_up_limit,alarm_down_limit,float_distance_coefficient, base_name
 * @retval 无
*/
void MyWidget::slots_widget_set_handle(int show_up_limit,int show_down_limit,int alarm_up_limit,int alarm_down_limit,float float_distance_coefficient,QString base_name)
{
    QChart_hall->axisY()->setRange(0, 4000);
    QChart_coil->axisY()->setRange(show_down_limit, show_up_limit);
    this->int_alarm_up_limit = alarm_up_limit;
    this->int_alarm_down_limit = alarm_down_limit;
    this->float_distance_coefficient = float_distance_coefficient;
    this->base_name = base_name;
    widget_report_win.int_alarm_up_limit = alarm_up_limit;
    widget_report_win.int_alarm_down_limit = alarm_down_limit;
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    qDebug()<<"handle setting data";
}
/*
 * @brief 采集卡网络对象的槽函数，套接字连接成功后自动运行
 * @param 无
 * @retval 无
*/
void MyWidget::QTcpSocket_DAQ_connected_slots()//成功连接槽函数
{
    static bool flag_connect = false;
    qDebug()<<"sucess connecting";
    if(QTimer_socket_connect_wait.isActive() == true)
    {
        QTimer_socket_connect_wait.stop();
    }
    if(flag_connect == false)//只连接一次槽函数，否则报错
    {
        connect(&QTcpSocket_DAQ, &QTcpSocket::readyRead,this,&MyWidget::QTcpSocket_DAQ_readyRead_slots);
        flag_connect = true;
    }
    char char_buff = 'C';
    QTcpSocket_DAQ.write(&char_buff,1);
    QTime dieTime = QTime::currentTime().addMSecs(1000);
    while( QTime::currentTime() < dieTime )
      QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    char_buff = 'L';
    QTcpSocket_DAQ.write(&char_buff,1);
}
/*
 * @brief 采集卡网络对象的槽函数，套接字连接失败后自动运行
 * @param 无
 * @retval 无
*/
void MyWidget::QTcpSocket_DAQ_connected_error_slots()
{
    qDebug()<<"connecting error";
    QMessageBox::about(this,"温馨提示","采集卡连接失败，请检查采集卡后重试^o^");
    this->flag_DAQ_error = 1;
}
/*
 * @brief 上传到云端网络对象的槽函数，套接字连接成功后自动运行
 * @param 无
 * @retval 无
*/
void MyWidget::QTcpSocket_cloud_connected_slots()//成功连接槽函数
{
    static bool flag_connect = false;
    qDebug()<<"sucess connecting";
    if(flag_connect == false)//只连接一次槽函数，否则报错
    {
        connect(&QTcpSocket_cloud, &QTcpSocket::readyRead,this,&MyWidget::QTcpSocket_cloud_readyRead_slots);
        flag_connect = true;
    }
    QString str ="send," + base_name;
    QByteArray sendMessage = str.toUtf8();
    //QThread::sleep(1);
    QTcpSocket_cloud.write(sendMessage);
}
/*
 * @brief 上传到云端网络对象的槽函数，套接字连接失败后自动运行
 * @param 无
 * @retval 无
*/
void MyWidget::QTcpSocket_cloud_connected_error_slots()
{
    qDebug()<<"connecting error";
    QMessageBox::about(this,"温馨提示","服务器连接失败，请检查网络后重试^o^");
    this->flag_cloud_error = 1;
}
/*
 * @brief 上传到云端网络对象的槽函数，套接字有新数据时自动运行
 * @param 无
 * @retval 无
*/
void MyWidget::QTcpSocket_cloud_readyRead_slots()//槽函数，自动调用，读取tcp数据
{
    QByteArray array = QTcpSocket_cloud.readAll();
    QString str = array;
    qDebug()<<str;
    if(str == "GetDamageData"){
        flag_cloud_uploading = 1;
    }else if(str == "GetRealTimeData"){
        flag_cloud_uploading = 2;
    }

    else{
        QMessageBox::about(this,"温馨提示","地点命名重复，停止并修改后重试!");
    }
}

void MyWidget::QTimer_socket_connect_wait_slots()
{
    this->flag_DAQ_error = 1;
    QMessageBox::about(this,"温馨提示","连接超时，采集卡可能出现问题，请检查采集卡后重试^o^");
    if(QTimer_socket_connect_wait.isActive() == true)
    {
        QTimer_socket_connect_wait.stop();
    }
}
/*
 * @brief 采集卡网络对象的槽函数，套接字有新数据时自动运行
 * @param 无
 * @retval 无
*/
void MyWidget::QTcpSocket_DAQ_readyRead_slots()//槽函数，自动调用，读取tcp数据
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
    QByteArray array = QTcpSocket_DAQ.readAll();
    //QThread::msleep(12);
    QTcpSocket_DAQ.write(&DAQ_socket_cmd,1);
    tcp_data_trans_together_func(array);
    if(flag_cloud_uploading == 2){
        QTcpSocket_cloud.write(array);//将数据上传到服务器
        qDebug()<<array.size();
    }
}
/*
 * @brief 采集卡数据解包函数
 * @param QByteArray data_in_array
 * @retval 无
*/
void MyWidget::tcp_data_trans_together_func(QByteArray data_in_array)//高八位低八位数据整合
{
    int data_return[160];
    int data_buff[640];
    int i = 0;
    for (i=0;i<640;i++)
    {
        data_buff[i] = data_in_array[2*i] + data_in_array[2*i+1]*256;
        //QDataStream_detec_data<<(qint32)data_buff[i];
    }
    //QDataStream_detec_data<<distance_buff;
    for (i=0;i<40;i++)
    {
        data_return[4*i] = data_buff[i*16+1]+data_buff[i*16+2]+data_buff[i*16+3]+data_buff[i*16+4]+data_buff[i*16+5]+data_buff[i*16+6]-10000;
        data_return[4*i+1] = data_buff[i*16+0]+data_buff[i*16+7]+data_buff[i*16+8]+data_buff[i*16+13]+data_buff[i*16+14]+data_buff[i*16+15]-10000;
        data_return[4*i+2] = data_buff[i*16+10];
        data_return[4*i+3] = data_buff[i*16+11];
        QDataStream_detec_data<<(qint32)data_return[4*i];
        QDataStream_detec_data<<(qint32)data_return[4*i+1];
        QDataStream_detec_data<<(qint32)data_return[4*i+2];
        QDataStream_detec_data<<(qint32)data_return[4*i+3];
        QDataStream_detec_data<<distance_buff;
    }
    int_distance_calc = (uint8_t)data_in_array[18] + (uint8_t)data_in_array[19]*256+(uint8_t)data_in_array[24]*65536 ;//将位移信息写入变量
    thread_getdata_graph_plot(data_return);
}
/*
 * @brief 多线程更新数据，实际上没有采用多线程方案
 * @param int data_in[]
 * @retval 无
*/
void MyWidget::thread_getdata_graph_plot(int data_in[])//长度160
{
    static int i = 0;
    static int x_axis = 0;
    int flag_defect_detec = 1;//损伤判断标志位
    if(x_axis>=1600)
    {
        x_axis = 0;
        QSplineSeries_coil->replace(QVector_QPointF_coil_graph_data1);
        QSplineSeries_coil2->replace(QVector_QPointF_coil_graph_data2);
        QSplineSeries_hall->replace(QVector_QPointF_hall_graph_data1);
        QSplineSeries_hall2->replace(QVector_QPointF_hall_graph_data2);
        detec_count+=800;
        ui->label_axisX1->setText(QString("%1ms").arg(detec_count));
        ui->label_axisX2->setText(QString("%1ms").arg(detec_count+1600));
    }
    else if(x_axis%1600==800)
    {
        QSplineSeries_coil->replace(QVector_QPointF_coil_graph_data1);
        QSplineSeries_coil2->replace(QVector_QPointF_coil_graph_data2);
        QSplineSeries_hall->replace(QVector_QPointF_hall_graph_data1);
        QSplineSeries_hall2->replace(QVector_QPointF_hall_graph_data2);
        detec_count+=800;
        ui->label_axisX1->setText(QString("%1ms").arg(detec_count));
        ui->label_axisX2->setText(QString("%1ms").arg(detec_count+1600));
    }
    /*
    for(i=0;i<40;i++)
    {
        int_filter_data_buff[i] = int_filter_data_buff[i+40];
        int_filter_data_buff[i+40] = data_in[4*i];
    }
    */
    //data_filter_func(int_filter_data_buff);
    for(i=0;i<40;i++)
    {
        //qDebug()<<int_after_filter_data[40+i];
        //QVector_QPointF_coil_graph_data1[x_axis].setY(int_after_filter_data[40+i]);
        QVector_QPointF_coil_graph_data1[x_axis].setY(data_in[4*i+0]);
        QVector_QPointF_coil_graph_data2[x_axis].setY(data_in[4*i+1]);
        QVector_QPointF_hall_graph_data1[x_axis].setY(data_in[4*i+2]);
        QVector_QPointF_hall_graph_data2[x_axis].setY(data_in[4*i+3]);
        x_axis++;
        if(flag_defect_detec ==1)//损伤判断
        {
            if(data_in[4*i+0]>int_alarm_up_limit)
            {
                flag_defect_detec = 0;
                int_defect_nubmer++;
            }
            else if(data_in[4*i+0]<int_alarm_down_limit)
            {
                flag_defect_detec = 0;
                int_defect_nubmer++;
            }
            else if(data_in[4*i+1]>int_alarm_up_limit)
            {
                flag_defect_detec = 0;
                int_defect_nubmer++;
            }
            else if(data_in[4*i+1]<int_alarm_down_limit)
            {
                flag_defect_detec = 0;
                int_defect_nubmer++;
            }
        }
    }
}
/*
 * @brief 低通滤波函数，实际上也没用
 * @param int input[80]
 * @retval 无
*/
void MyWidget::data_filter_func(int input[80])
{
    static const double dv[21] = { -1.8519480747177166E-18, -0.0064364156273118674,
        -0.020344062613925358, -0.042715906243204434, -0.0642941075802573,
        -0.0680503663752848, -0.03903518661005019, 0.024175976525245443,
        0.10439338618338236, 0.17169615839553634, 0.19795049173952656,
        0.17169615839553634, 0.10439338618338236, 0.024175976525245443,
        -0.03903518661005019, -0.0680503663752848, -0.0642941075802573,
        -0.042715906243204434, -0.020344062613925358, -0.0064364156273118674,
        -1.8519480747177166E-18 };
      memset(&int_after_filter_data[0], 0, 80U * sizeof(int));
      for (int k = 0; k < 21; k++) {
        int b_k;
        b_k = k + 1;
        for (int j = b_k; j < 81; j++) {
          int_after_filter_data[j - 1] += dv[k] * input[(j - k) - 1];
        }
      }
}
/*
 * @brief 定时器槽函数，定时更新距离数据，自动执行
 * @param 无
 * @retval 无
*/
void MyWidget::QTimer_alarm_label_slots()
{
    if(int_defect_nubmer>0)
    {
        ui->label_defect->setText("有损伤");
        /*
         * 如果工作模式为上传模式，则需要上传损伤数据到服务器
        */
        if(flag_cloud_uploading == 1){
            QString str ="sendDamage," + base_name+"."+QString::number(int_defect_nubmer);
            QByteArray sendMessage = str.toUtf8();
            QTcpSocket_cloud.write(sendMessage);
        }
        int_defect_nubmer = 0;
    }
    else
    {
        ui->label_defect->setText("正常");
    }
    distance_buff = distance_dir*float_distance_coefficient*(int_distance_calc - 8388608)/1000.0;
    ui->label_distance->setText(QString("%1m").arg(distance_buff));

  //  static int frameCount = 0;
  //  static QString labelText = QStringLiteral("FPS: %1");

  //  for (int i = 0; i < m_seriesList.size(); i++)
  //      update(m_seriesList[i], i);

  //  frameCount++;
  //  int elapsed = m_fpsTimer.elapsed();
  //  if (elapsed >= 1000) {
  //      elapsed = m_fpsTimer.restart();
  //      qreal fps = qreal(0.1 * int(10000.0 * (qreal(frameCount) / qreal(elapsed))));
  //      m_fpsLabel->setText(labelText.arg(QString::number(fps, 'f', 1)));
  //      m_fpsLabel->adjustSize();
  //      frameCount = 0;
  //  }


}

/*
 * @brief 方向控件，切换距离正负值以用来符合实际
 * @param bool checked
 * @retval 无
*/
void MyWidget::on_radioButton_distance_dir_toggled(bool checked)
{
    static double distance_buff = 0;
    if(checked)
    {
        distance_dir = 1;
        distance_buff = distance_dir*float_distance_coefficient*(int_distance_calc - 8388608)/1000.0;
        ui->label_distance->setText(QString("%1m").arg(distance_buff));
    }
    else
    {
        distance_dir = -1;
        distance_buff = distance_dir*float_distance_coefficient*(int_distance_calc - 8388608)/1000.0;
        ui->label_distance->setText(QString("%1m").arg(distance_buff));
    }
    qDebug()<<distance_dir;
}
/*
 * @brief 标定按钮的槽函数，点击按钮自动运行
 * @param 无
 * @retval 无
*/
void MyWidget::on_pushButton_dema_clicked()
{
    bool ok;
    double d = QInputDialog::getDouble(this, tr("距离系数标定"),tr("实际距离(m):"), 1.00, -10000, 10000, 2, &ok,Qt::MSWindowsFixedSizeDialogHint);
    if (ok){
        qDebug()<<(float)d;
        if(distance_buff<0.001&&distance_buff>-0.001)distance_buff = 1;
        qDebug()<<distance_buff;
        this->float_distance_coefficient = (float)(d/distance_buff)*float_distance_coefficient;
        Widget_set_win.float_distance_coefficient = this->float_distance_coefficient;
        qDebug()<<this->float_distance_coefficient;
    }
}


