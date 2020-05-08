#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
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


void MainWindow::readMyCom()
{
    QByteArray temp=myCom->readAll();
    ui->textBrowser->insertPlainText(temp);


}

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

void MainWindow::on_BTN_ComClose_clicked()
{
    myCom->close();
    ui->BTN_ComOpen->setEnabled(TRUE);
    ui->BTN_ComClose->setEnabled(FALSE);
    ui->BTN_Send->setEnabled(FALSE);

}


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
