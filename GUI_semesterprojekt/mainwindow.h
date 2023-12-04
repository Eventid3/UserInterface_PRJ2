#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"serial.h"
#include<QDebug>
#include <array>
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <mutex>
#include <condition_variable>




QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow,Serial
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void ChangeThreshold();
    void LoadRecievedDataToBuffer();
    int SetPort(){return PORT;};
    int SetBaud(){return BAUD;};
    void printTemp();//Bare Test
    void PrintBuffer();
   // void PrintDebugBuffer();
    void ClearBuffer();
public slots:
    void updateThresholdLabel(int value);


private slots:
    void on_pushButton_clicked();

    void on_dial_valueChanged(int value);

    void on_pushButton_2_pressed();

    void on_ReadCurrentTemp_pressed();

    void on_ReadCurrentTemp_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_4_clicked(bool checked);

    void on_pushButton_4_pressed();

    void on_pushButton_3_clicked();

    void on_lineEdit_editingFinished();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_lineEdit_textEdited(const QString &arg1);

    void on_lineEdit_returnPressed();

private:
    Ui::MainWindow *ui;
    Serial* m_Serial;
    bool m_Running;
    int PORT= 9;
    int BAUD= 9600;
    std::array<char, 10> m_Buffer; //buffer for recieved data
    //int lastDialValue;
    float floatDialValue;
    std::mutex m_Mutex; // mutex for handeling multithreading
    std:: string testPort="9";



};
#endif // MAINWINDOW_H
