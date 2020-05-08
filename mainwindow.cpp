#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGridLayout>
#include "qfile"
#include <QFileDialog>
#include <QDateTime>
#define printf qDebug
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{   QByteArray temp;
    ui->setupUi(this);
    ui->BTN_Send->setEnabled(FALSE);
    ui->BTN_ComClose->setEnabled(FALSE);
    init_comm();


}

MainWindow::~MainWindow()
{
    delete ui;
}

//串口数据接收
void MainWindow::readMyCom()
{
    //串口收到数据
    QByteArray DisTemp;//用于显示数据
    QByteArray SavTemp;//用于保存数据
    QByteArray temp = myCom->readAll();
    //时间显示和获取
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy-MM-dd:hh:mm:ss");
    ui->text_time->setText(current_date);
    if(!temp.isEmpty())
    {

        if(temp[0]=='A')    //显示当前速度和报警速度
        {
            DisTemp[0]= temp[1];
            DisTemp[1]= temp[2];
            DisTemp[2]= temp[3];
            ui->textBrowser->setText(DisTemp+"m/s");      //显示当前速度

            DisTemp[0]= temp[4];
            DisTemp[1]= temp[5];
            DisTemp[2]= temp[6];
            ui->textBrowser_2->setText(DisTemp+"m/s");     //显示报警速度


        }
        else if(temp[0]=='B')
        {
            DisTemp[0]= temp[1];
            DisTemp[1]= temp[2];
            DisTemp[2]= temp[3];
            ui->textBrowser_3->setText(DisTemp+"m/s");      //显示当前速度

            DisTemp[0]= temp[4];
            DisTemp[1]= temp[5];
            DisTemp[2]= temp[6];
            ui->textBrowser_4->setText(DisTemp+"m/s");     //显示报警速度
            SavTemp[0 ]  = ' ';
            SavTemp[1 ]  = 'S';
            SavTemp[2 ]  = ':';
            SavTemp[3 ]  = temp[1];
            SavTemp[4 ]  = temp[2];
            SavTemp[5 ]  = temp[3];

            SavTemp[6 ]  = ' ';
            SavTemp[7 ]  = 'A';
            SavTemp[8 ]  = ':';
            SavTemp[9 ]  = temp[4];
            SavTemp[10]  = temp[5];
            SavTemp[11]  = temp[6];


            //保存报警记录
            QFile file(QDir::currentPath()+"/text/t.txt");
            file.open(QIODevice::ReadWrite);
            QByteArray tt = file.readAll();
            file.write(current_date.toUtf8());
            file.write(SavTemp);
            file.write("\r\n");

        }
    }
   // ui->textBrowser->insertPlainText(temp);


}
//按键按下连接
void MainWindow::on_BTN_ComOpen_clicked()
{
    struct PortSettings myComSetting={BAUD115200,DATA_8,PAR_NONE,STOP_1,FLOW_OFF,500};
    myCom = new Win_QextSerialPort(ui->CB_COM_num->currentText(),myComSetting,QextSerialBase::EventDriven);
    myCom->open(QIODevice::ReadWrite);
    connect(myCom,SIGNAL(readyRead()),this,SLOT(readMyCom()));
    ui->BTN_ComOpen->setEnabled(FALSE);
    ui->BTN_ComClose->setEnabled(TRUE);
    ui->BTN_Send->setEnabled(true);
}
//按键按下断开
void MainWindow::on_BTN_ComClose_clicked()
{
    myCom->close();
    ui->BTN_ComOpen->setEnabled(TRUE);
    ui->BTN_ComClose->setEnabled(FALSE);
    ui->BTN_Send->setEnabled(FALSE);

}

//初始化串口
void MainWindow::init_comm()
{
    QString path = "HKEY_LOCAL_MACHINE\\HARDWARE\\DEVICEMAP\\SERIALCOMM";
    QSettings *settings = new QSettings(path, QSettings::NativeFormat);
    QStringList key = settings->allKeys();
    QStringList comlist ;
    QStringList Baudlist ;
//    QStringList Paritylist ;
//    QStringList DataBitslist;
//    QStringList StopBitslist;
//    QStringList ControlFlowlist;

    comlist.clear();

    /* 取串口名 */
    for (int i=0; i<key.size(); i++) {
        comlist << getcomm(i, "value");
    }
    ui->CB_COM_num->addItems(comlist);
}
/*
    读取键值
*/
QString MainWindow::getcomm(int index, QString keyorvalue)
{
    QString commresult;

    if (::RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"), 0, KEY_READ, &hKey) != 0) {
            QString error="Cannot open regedit!";//无法打开注册表时返回error
            return error;
        }//if(::RegOpenKeyEx(HKEY_LOCAL_MACHINE,subkey,0,KEY_READ,&hKey)!=0)

        QString keymessage;//键名
        QString message;
        QString valuemessage;//键值

        indexnum = index;//要读取键值的索引号
        keysize = sizeof(keyname);
        valuesize = sizeof(keyvalue);

        if (RegEnumValue(hKey, indexnum, keyname, &keysize, 0, &type, (BYTE*)keyvalue, &valuesize) == 0) { //列举键名和值
            for (int i=0; i<(int)keysize; i++) {
                message = keyname[i];
                keymessage.append(message);
            }

            for (int j=0; j<(int)valuesize; j++) {
                 if (keyvalue[j] != 0x00) {
                     valuemessage.append(keyvalue[j]);
                 }
            }

            if (keyorvalue == "key") {
                 commresult = keymessage;
            }

            if (keyorvalue == "value") {
                 commresult=valuemessage;
            }
        } else {
             commresult = "nokey";
        }
        RegCloseKey(hKey);//关闭注册表
        return commresult;
}

void MainWindow::on_BTN_Send_clicked()
{
    QFile file(QDir::currentPath()+"/text/t.txt");//获取文本路径及名称
    file.open(QIODevice::ReadWrite);
    QString t = file.readAll();
    ui->text_r->setText(t);
   // file.close();
}

void MainWindow::on_pushButton_clicked()
{
    QString e = "ui->text_e->toPlainText()";
    QFile file(QDir::currentPath()+"/text/t.txt");
    file.open(QIODevice::ReadWrite);
    QByteArray tt = file.readAll();
  //  file.write(current_data.toUtf8());
    file.write(e.toUtf8());
    file.write("\r\n");
   // file.close();
}

void MainWindow::on_pushclear_clicked()
{
    ui->text_r->clear();
}
