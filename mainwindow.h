#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include"qextserialport/win_qextserialport.h"
#include "qextserialport/qextserialenumerator.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
       HKEY hKey;
      LPCWSTR subkey;
      wchar_t keyname[256]; //键名数组
      char keyvalue[256];  //键值数组
      DWORD keysize,type,valuesize;
      int indexnum;

private:
    Ui::MainWindow *ui;
    Win_QextSerialPort *myCom;
    void init_comm();
    QString getcomm(int index, QString keyorvalue);
private slots:
    void  readMyCom();
    void on_BTN_ComOpen_clicked();
    void on_BTN_ComClose_clicked();
    void on_BTN_Send_clicked();
};

#endif // MAINWINDOW_H
