#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <math.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    digitBTNs = {{Qt::Key_0,ui->btnNum0},
               {Qt::Key_1,ui->btnNum1},
               {Qt::Key_2,ui->btnNum2},
               {Qt::Key_3,ui->btnNum3},
        {Qt::Key_4,ui->btnNum4},
        {Qt::Key_5,ui->btnNum5},
        {Qt::Key_6,ui->btnNum6},
        {Qt::Key_7,ui->btnNum7},
        {Qt::Key_8,ui->btnNum8},
        {Qt::Key_9,ui->btnNum9},


    };

    foreach(auto btn, digitBTNs )
        connect(btn,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum0,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum1,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum2,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum3,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum4,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum5,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum6,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum7,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum8,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum9,SIGNAL(clicked()),this,SLOT(btnNumClicked()));

    connect(ui->btnPlus,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));
    connect(ui->btnMinus,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));
    connect(ui->btnMultiple,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));
    connect(ui->btnDivide,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));

    connect(ui->btnPercentage,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnInverse,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnSquare,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnSqrt,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::calculation(bool *ok)
{
    double result=0;
    if(operands.size()==2 && opcodes.size()>0){
        //取操作数
        double operand1 = operands.front().toDouble();
        operands.pop_front();
        double operand2 = operands.front().toDouble();
        operands.pop_front();

        //取操作符
        QString op = opcodes.front();
        opcodes.pop_front();

        if(op == "+"){
            result = operand1 + operand2;

        }
        else if(op =="-"){
            result = operand1 - operand2;
        }
        else if(op =="*"){
            result = operand1 * operand2;
        }
        else if(op == "/"){
            result = operand1 / operand2;
        }

        operands.push_back(QString::number(result));


        ui->statusbar->showMessage(QString("calculation is in progress:operands is %1,opcode is %2").arg(operands.size()).arg(opcodes.size()));
    }
    else
        ui->statusbar->showMessage(QString("operands is %1,opcode is %2").arg(operands.size()).arg(opcodes.size()));
    return QString::number(result);
}

void MainWindow::btnNumClicked()
{
    QString digit = qobject_cast<QPushButton*>(sender())->text();
    if(digit == "0" && operand == "0" )
        digit = "";
    if(operand == "0" && digit != "0" )
        operand = "";

    operand += digit;
    ui->Display->setText(operand);

}


void MainWindow::on_btnPeriod_clicked()
{
    if(!operand.contains("."))
    operand += qobject_cast<QPushButton*>(sender())->text();
    ui->Display->setText(operand);
}


void MainWindow::on_btnDelete_clicked()
{
    operand = operand.left(operand.length()-1);
    ui->Display->setText(operand);
}





void MainWindow::on_btnClear_clicked()
{
    operand.clear();
    ui->Display->setText(operand);
}

void MainWindow::btnBinaryOperatorClicked()
{
    ui->statusbar->showMessage("last operand" + operand);

    QString opcode = qobject_cast<QPushButton*>(sender())->text();
    qDebug() << opcode;

    if(operand != "")
    {
        operands.push_back(operand);
        operand ="";

        opcodes.push_back(opcode);

    QString result=calculation();
    ui->Display->setText(result);
    }
}

void MainWindow::btnUnaryOperatorClicked()
{
    if(operand != "")
    {
        double result = operand.toDouble();
        operand ="";

        QString op = qobject_cast<QPushButton*>(sender())->text();

        if(op == "%")
            result /= 100.0;
        else if(op == "1/x")
            result = 1/result;
        else if(op == "x^2")
            result *=result;
        else if(op =="√")
            result = sqrt(result);

        ui->Display->setText(QString::number(result));
    }
}


void MainWindow::on_btnEqual_clicked()
{
    if(operand != "")
    {
        operands.push_back(operand);
        operand ="";
    }
    QString result=calculation();
     ui->Display->setText(result);
}


void MainWindow::simulateButtonClick(const QString &objectName)
{
    QPushButton *btn = findChild<QPushButton*>(objectName);
    if (btn) {
        btn->animateClick();
    }
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // 处理数字键
    foreach(auto btnKey , digitBTNs.keys()){
        if(event->key() == btnKey)
            digitBTNs[btnKey]->animateClick();
    }

    // 处理运算键和功能键
    switch (event->key()) {
    case Qt::Key_Plus:
        simulateButtonClick("btnPlus");
        break;
    case Qt::Key_Minus:
        simulateButtonClick("btnMinus");
        break;
    case Qt::Key_Asterisk:
        simulateButtonClick("btnMultiple");
        break;
    case Qt::Key_Slash:
        simulateButtonClick("btnDivide");
        break;
    case Qt::Key_Percent:
        simulateButtonClick("btnPercentage");
        break;
    case Qt::Key_Enter:
    case Qt::Key_Return:
    case Qt::Key_Equal:
        simulateButtonClick("btnEqual");
        break;
    case Qt::Key_Delete:
    case Qt::Key_Backspace:
        simulateButtonClick("btnDelete");
        break;
    case Qt::Key_Escape:
        simulateButtonClick("btnClear");
        break;
    case Qt::Key_Period:
    case Qt::Key_Comma:
        simulateButtonClick("btnPeriod");
        break;
    // 一元运算符
    case Qt::Key_R:
        simulateButtonClick("btnSqrt");
        break;
    case Qt::Key_I:
        simulateButtonClick("btnInverse");
        break;
    case Qt::Key_S:
        simulateButtonClick("btnSquare");
        break;
    default:
        QMainWindow::keyPressEvent(event);
        break;
    }
}
