#include "widget_set.h"
#include "ui_widget_set.h"
/*
 * ***************************************************************************************
 * @brief 钢丝绳检测软件，需配合探头与采集卡、放大板使用
 * @file widget_set.cpp
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
Widget_set::Widget_set(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget_set)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);//无边框
    ui->comboBox_alarm_downlimit->setCurrentIndex(7);
    ui->comboBox_alarm_uplimit->setCurrentIndex(7);
    ui->comboBox_up_limit->setCurrentIndex(2);
    ui->comboBox_down_limit->setCurrentIndex(2);
    if(QDir_data_set_dir.exists("dataset_dir") == false)
    {
        QDir_data_set_dir.mkdir("dataset_dir");
    }
    QFileInfo fileInfo("./dataset_dir/setfile.txt");
    if(fileInfo.isFile())
    {
        qDebug()<<"fileisexis";
        QFile_data_set_file.setFileName("./dataset_dir/setfile.txt");//文件创建与打开
        bool file_open_sucess = QFile_data_set_file.open(QIODevice::ReadWrite);
        if(file_open_sucess == true)
        {
            QDataStream_set_data.setDevice(&QFile_data_set_file);
        }
        QDataStream_set_data>>int_show_up_limit>>int_show_down_limit>>int_alarm_up_limit>>int_alarm_down_limit>>float_distance_coefficient>>base_name;
        ui->comboBox_alarm_downlimit->setCurrentIndex((1500-int_alarm_down_limit)/500);
        ui->comboBox_alarm_uplimit->setCurrentIndex((7500-int_alarm_up_limit)/500);
        ui->lineEdit_coefficient->setText(QString("%1").arg(float_distance_coefficient));
        ui->comboBox_up_limit->setCurrentIndex((8000-int_show_up_limit)/1000);
        ui->comboBox_down_limit->setCurrentIndex(int_show_down_limit/-1000);
        ui->lineEdit_name->setText(base_name);
        QFile_data_set_file.close();
    }
    else
    {
        qDebug()<<"fileisnotexis";
        QFile_data_set_file.setFileName("./dataset_dir/setfile.txt");//文件创建与打开
        bool file_open_sucess = QFile_data_set_file.open(QIODevice::ReadWrite);
        if(file_open_sucess == true)
        {
            QDataStream_set_data.setDevice(&QFile_data_set_file);
        }
        QDataStream_set_data<<int_show_up_limit<<int_show_down_limit<<int_alarm_up_limit<<int_alarm_down_limit<<float_distance_coefficient<<base_name;
        QFile_data_set_file.close();
    }
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    emit signals_bn_set_save(int_show_up_limit,int_show_down_limit,int_alarm_up_limit,int_alarm_down_limit,float_distance_coefficient,base_name);
    connect(&QTimer_setting,&QTimer::timeout,this,&Widget_set::QTimer_setting_slots);
    QTimer_setting.start(1000);
}

Widget_set::~Widget_set()
{
    delete ui;
}
void Widget_set::on_pushButton_set_save_clicked()
{
    QString strPath = "./dataset_dir";
    if (strPath.isEmpty() ==false&&QDir().exists(strPath))//是否传入了空的路径||路径是否存在
    {
        QFileInfo FileInfo(strPath);

        if (FileInfo.isFile())//如果是文件
            QFile::remove(strPath);
        else if (FileInfo.isDir())//如果是文件夹
        {
            QDir qDir(strPath);
            qDir.removeRecursively();
        }
        QDir QDir_data_save_dir;
        if(QDir_data_save_dir.exists("dataset_dir") == false)
        {
            QDir_data_save_dir.mkdir("dataset_dir");
        }
    }
    int_show_up_limit = ui->comboBox_up_limit->currentText().toInt();
    int_show_down_limit = ui->comboBox_down_limit->currentText().toInt();
    int_alarm_up_limit = ui->comboBox_alarm_uplimit->currentText().toInt();
    int_alarm_down_limit = ui->comboBox_alarm_downlimit->currentText().toInt();
    float_distance_coefficient = ui->lineEdit_coefficient->text().toFloat();
    base_name = ui->lineEdit_name->text();
    qDebug()<<float_distance_coefficient;
    emit signals_bn_set_save(int_show_up_limit,int_show_down_limit,int_alarm_up_limit,int_alarm_down_limit,float_distance_coefficient,base_name);
    QFile_data_set_file.setFileName("./dataset_dir/setfile.txt");//文件创建与打开
    bool file_open_sucess = QFile_data_set_file.open(QIODevice::ReadWrite);
    if(file_open_sucess == true)
    {
        QDataStream_set_data.setDevice(&QFile_data_set_file);
    }
    QDataStream_set_data<<int_show_up_limit<<int_show_down_limit<<int_alarm_up_limit<<int_alarm_down_limit<<float_distance_coefficient<<base_name;
    QFile_data_set_file.close();
    this->hide();
}
void Widget_set::on_pushButton_set_quit_clicked()
{
}
void Widget_set::on_pushButton_set_filedelete_clicked()
{
    QString strPath = "./data_dir";
    if (strPath.isEmpty() ==false&&QDir().exists(strPath))//是否传入了空的路径||路径是否存在
    {
        QFileInfo FileInfo(strPath);

        if (FileInfo.isFile())//如果是文件
            QFile::remove(strPath);
        else if (FileInfo.isDir())//如果是文件夹
        {
            QDir qDir(strPath);
            qDir.removeRecursively();
        }
        QDir QDir_data_save_dir;
        if(QDir_data_save_dir.exists("data_dir") == false)
        {
            QDir_data_save_dir.mkdir("data_dir");
        }
    }
    QMessageBox::about(this,"温馨提示","数据已删除^o^");
}

void Widget_set::on_pushButton_demar_clicked()
{
    ui->lineEdit_coefficient->setText(QString("%1").arg(this->float_distance_coefficient));
    QMessageBox::about(this,"温馨提示","更新成功^o^");
}

void Widget_set::on_pushButton_copyfile_clicked()
{
     QDir dir("/mnt/data");
    if(dir.exists())
      {
        copyDirectoryFiles("./data_dir","/mnt/data",1);
        QMessageBox::about(this,"温馨提示","数据拷贝成功^o^");
      }
    else QMessageBox::about(this,"温馨提示","请插入内存卡并重启设备^o^");
}

bool Widget_set::copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist)
{
    QDir sourceDir(fromDir);
    QDir targetDir(toDir);
    if(!targetDir.exists()){    /**< 如果目标目录不存在，则进行创建 */
        if(!targetDir.mkdir(targetDir.absolutePath()))
            return false;
    }

    QFileInfoList fileInfoList = sourceDir.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList){
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;
        if(fileInfo.isDir()){    /**< 当为目录时，递归的进行copy */
            if(!copyDirectoryFiles(fileInfo.filePath(),
                targetDir.filePath(fileInfo.fileName()),
                coverFileIfExist))
                return false;
        }
        else{            /**< 当允许覆盖操作时，将旧文件进行删除操作 */
            if(coverFileIfExist && targetDir.exists(fileInfo.fileName())){
                targetDir.remove(fileInfo.fileName());
            }
            /// 进行文件copy
            if(!QFile::copy(fileInfo.filePath(),
                targetDir.filePath(fileInfo.fileName()))){
                    return false;
            }
        }
    }
    return true;
}

void Widget_set::QTimer_setting_slots(){
    if(QTimer_setting.isActive() == true)
    {
        QTimer_setting.stop();
    }
    emit signals_bn_set_save(int_show_up_limit,int_show_down_limit,int_alarm_up_limit,int_alarm_down_limit,float_distance_coefficient,base_name);
}

