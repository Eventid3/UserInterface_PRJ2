#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <thread>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    ,ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_Serial = new Serial();
    SetPort();
    SetBaud();

    if (!m_Serial->Open(PORT, BAUD))
    {
       qDebug()<<"Could not open Port";
        delete m_Serial;
        exit(-1);
    }
    else
    {

    }

   //   m_Buffer = { 0 };
    m_Running = true;

   floatDialValue = 0;
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_dial_valueChanged(int value)
{
    ui->lcdNumber->display(value);
   // lastDialValue = value;

    // Convert the integer value to a float
    floatDialValue = static_cast<float>(value);

}


void MainWindow::on_pushButton_2_pressed()
{
    ui->CurrentThreshold->setText(QString::number(floatDialValue));
    char input='2';
    //std::lock_guard<std::mutex> lock(m_Mutex);
    m_Serial->SendData(&input, 1);
    ChangeThreshold();
    qDebug() <<  floatDialValue;
    ui->lcdNumber->hide();
    ui->dial->hide();
}


void MainWindow::on_ReadCurrentTemp_clicked()
{
   // ui->label1->setText("Current Temp: ");
   //LoadRecievedDataToBuffer(); //Test Approved
    char input='1';
    m_Serial->SendData(&input, 1);
    LoadRecievedDataToBuffer();
    PrintBuffer();


    // printTemp();
   // PrintBuffer();
}
void MainWindow::printTemp()
{
  // ui->label1->setText(QString::fromStdString(Serial::ReadDataWaiting()));
    // LoadRecievedDataToBuffer();
    // PrintBuffer();
}

void MainWindow::LoadRecievedDataToBuffer()
{

    int waitingData=m_Serial->ReadDataWaiting();
    ClearBuffer();

    if (waitingData > 0)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        m_Serial->ReadData(&m_Buffer, 10);
    }
}



void MainWindow::PrintBuffer()
{
    QString bufferContent;
    for (int i = 0, l = m_Buffer.size(); i < l; i++)
    {
        //bufferContent.append(m_Buffer[i]);
        qDebug() << QChar(m_Buffer[i]);
         bufferContent.append(QChar(m_Buffer[i])); //Konverter alle hver bite til en char
        //std::cout << m_Buffer[i];
    }


    std::string bufferAsString = bufferContent.toStdString();

    //std::cout << std::endl
     ui->label1->setText(QString::fromStdString(bufferAsString));
     //ui->label1->setText(bufferContent);
}

// void MainWindow::PrintDebugBuffer()
// {
//     QString bufferContent;
//     for (int i = 0, l = m_Buffer.size(); i < l; i++)
//     {
//         //bufferContent.append(m_Buffer[i]);
//         bufferContent.append(QChar(m_Buffer[i])); //Konverter alle hver bite til en char
//         //std::cout << m_Buffer[i];
//     }
//     std::string bufferAsString = bufferContent.toStdString();

//     ui->label_2->setText(QString::fromStdString(bufferAsString));
//     ui->label_2->setText(bufferContent);
//     qDebug()<<bufferContent;
//     //std::cout << std::endl;
// }

void MainWindow::ClearBuffer()
{
    m_Buffer.fill(0);
}

void MainWindow::ChangeThreshold()
{

    //std::cout << "Enter new temperature threshold: ";
    //std::cin >> input;

    //TODO check for valid input

    char buffer[10];

    snprintf(buffer, sizeof(buffer), "%f", floatDialValue);
    //std::cout << buffer << std::endl;
    m_Serial->SendData(buffer, 10);
}


// void MainWindow::on_pushButton_4_pressed()
// {
//     char input='3';
//     m_Serial->SendData(&input, 1);

//     LoadRecievedDataToBuffer();
//     PrintDebugBuffer();

// }





void MainWindow::on_lineEdit_textEdited(const QString &arg1)
{
    testPort = arg1;

}

void MainWindow::on_lineEdit_returnPressed()
{
    qDebug()<<testPort;
}

