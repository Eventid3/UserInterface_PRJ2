/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDial>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *pushButton;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_5;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *ReadCurrentTemp;
    QLabel *label1;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *SetTemp;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QLabel *CurrentThreshold;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *pushButton_3;
    QLineEdit *lineEdit;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QDial *dial;
    QLCDNumber *lcdNumber;
    QPushButton *pushButton_2;
    QMenuBar *menubar;
    QMenu *menuTemp_Control;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(458, 270);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(180, 200, 80, 18));
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(10, 70, 441, 121));
        horizontalLayout_5 = new QHBoxLayout(layoutWidget);
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        ReadCurrentTemp = new QPushButton(layoutWidget);
        ReadCurrentTemp->setObjectName("ReadCurrentTemp");

        horizontalLayout_2->addWidget(ReadCurrentTemp);

        label1 = new QLabel(layoutWidget);
        label1->setObjectName("label1");

        horizontalLayout_2->addWidget(label1);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        SetTemp = new QPushButton(layoutWidget);
        SetTemp->setObjectName("SetTemp");

        horizontalLayout_3->addWidget(SetTemp);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName("label_4");

        horizontalLayout_4->addWidget(label_4);

        CurrentThreshold = new QLabel(layoutWidget);
        CurrentThreshold->setObjectName("CurrentThreshold");

        horizontalLayout_4->addWidget(CurrentThreshold);


        verticalLayout_2->addLayout(horizontalLayout_4);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        pushButton_3 = new QPushButton(layoutWidget);
        pushButton_3->setObjectName("pushButton_3");

        horizontalLayout_6->addWidget(pushButton_3);

        lineEdit = new QLineEdit(layoutWidget);
        lineEdit->setObjectName("lineEdit");

        horizontalLayout_6->addWidget(lineEdit);


        verticalLayout_2->addLayout(horizontalLayout_6);


        horizontalLayout_5->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        dial = new QDial(layoutWidget);
        dial->setObjectName("dial");
        dial->setMaximum(35);

        horizontalLayout->addWidget(dial);

        lcdNumber = new QLCDNumber(layoutWidget);
        lcdNumber->setObjectName("lcdNumber");

        horizontalLayout->addWidget(lcdNumber);


        verticalLayout->addLayout(horizontalLayout);

        pushButton_2 = new QPushButton(layoutWidget);
        pushButton_2->setObjectName("pushButton_2");

        verticalLayout->addWidget(pushButton_2);


        horizontalLayout_5->addLayout(verticalLayout);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 458, 17));
        menuTemp_Control = new QMenu(menubar);
        menuTemp_Control->setObjectName("menuTemp_Control");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuTemp_Control->menuAction());
        menuTemp_Control->addSeparator();

        retranslateUi(MainWindow);
        QObject::connect(dial, SIGNAL(valueChanged(int)), lcdNumber, SLOT(display(int)));
        QObject::connect(pushButton_2, &QPushButton::pressed, lcdNumber, qOverload<>(&QLCDNumber::hide));
        QObject::connect(pushButton_2, &QPushButton::pressed, dial, qOverload<>(&QDial::hide));
        QObject::connect(pushButton_2, &QPushButton::pressed, pushButton_2, qOverload<>(&QPushButton::hide));
        QObject::connect(SetTemp, &QPushButton::pressed, dial, qOverload<>(&QDial::show));
        QObject::connect(SetTemp, &QPushButton::pressed, lcdNumber, qOverload<>(&QLCDNumber::show));
        QObject::connect(SetTemp, &QPushButton::pressed, pushButton_2, qOverload<>(&QPushButton::show));
        QObject::connect(pushButton, &QPushButton::pressed, MainWindow, qOverload<>(&QMainWindow::close));
        QObject::connect(ReadCurrentTemp, &QPushButton::clicked, label1, qOverload<>(&QLabel::update));
        QObject::connect(pushButton_2, &QPushButton::pressed, CurrentThreshold, qOverload<>(&QLabel::update));
        QObject::connect(pushButton_3, &QPushButton::clicked, lineEdit, qOverload<>(&QLineEdit::show));
        QObject::connect(lineEdit, &QLineEdit::textChanged, lineEdit, qOverload<>(&QLineEdit::hide));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "Close", nullptr));
        ReadCurrentTemp->setText(QCoreApplication::translate("MainWindow", "Read Current Temp", nullptr));
        label1->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        SetTemp->setText(QCoreApplication::translate("MainWindow", "SetTemp", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Current Threshold:", nullptr));
        CurrentThreshold->setText(QCoreApplication::translate("MainWindow", "no current thresholdl", nullptr));
        pushButton_3->setText(QCoreApplication::translate("MainWindow", "Reset PORT", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "Set Value", nullptr));
        menuTemp_Control->setTitle(QCoreApplication::translate("MainWindow", "Temp Control", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
