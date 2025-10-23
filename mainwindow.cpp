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
    double result = 0;

    qDebug() << "计算前 - 操作数:" << operands << "操作符:" << opcodes;

    if(operands.size() >= 2 && opcodes.size() >= 1){
        // 取操作数
        double operand1 = operands.first().toDouble();
        operands.pop_front();
        double operand2 = operands.first().toDouble();
        operands.pop_front();

        // 取操作符
        QString op = opcodes.first();
        opcodes.pop_front();

        qDebug() << "执行计算:" << operand1 << op << operand2;

        if(op == "+"){
            result = operand1 + operand2;
        }
        else if(op == "-"){
            result = operand1 - operand2;
        }
        else if(op == "×"){
            result = operand1 * operand2;
        }
        else if(op == "/"){
            if(operand2 != 0) {
                result = operand1 / operand2;
            } else {
                ui->statusbar->showMessage("错误：除数不能为零");
                return "错误";
            }
        }

        // 关键修改：将结果作为唯一操作数放回队列
        operands.clear(); // 清空所有操作数
        operands.push_back(QString::number(result)); // 只保留计算结果

        qDebug() << "计算结果:" << result << "计算后队列 - 操作数:" << operands << "操作符:" << opcodes;

        ui->statusbar->showMessage(QString("计算完成: %1 %2 %3 = %4")
                                       .arg(operand1).arg(op).arg(operand2).arg(result));

        return QString::number(result);
    }
    else
    {
        qDebug() << "计算条件不足，无法计算";
        ui->statusbar->showMessage(QString("操作数不足: %1, 操作符: %2")
                                       .arg(operands.size()).arg(opcodes.size()));
        return "0";
    }
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
    operands.clear();
    opcodes.clear();
    ui->Display->setText("0");
    ui->statusbar->showMessage("已重置计算器");
}

void MainWindow::btnBinaryOperatorClicked()
{
    QString opcode = qobject_cast<QPushButton*>(sender())->text();
    qDebug() << "操作符:" << opcode << "当前操作数:" << operand;

    if(operand != "")
    {
        // 将当前操作数加入队列
        operands.push_back(operand);
        operand = "";

        // 只有在有2个操作数和至少1个操作符时才进行计算
        if(operands.size() >= 2 && opcodes.size() >= 1) {
            QString result = calculation();
            ui->Display->setText(result);
            // 计算后队列中应该只剩下结果
            qDebug() << "连续运算后队列状态 - 操作数:" << operands << "操作符:" << opcodes;
        }

        // 添加新的操作符
        opcodes.push_back(opcode);

        ui->statusbar->showMessage("等待第二个操作数...");
    }
    else
    {
        // 如果没有新的操作数，只是改变操作符
        if(opcodes.size() > 0) {
            opcodes.back() = opcode; // 替换最后一个操作符
            ui->statusbar->showMessage("操作符已更新: " + opcode);
        } else if(operands.size() > 0) {
            // 如果只有一个操作数但想改变操作符
            opcodes.push_back(opcode);
            ui->statusbar->showMessage("操作符已设置: " + opcode);
        }
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
    qDebug() << "等号按下 - 当前操作数:" << operand << "队列状态 - 操作数:" << operands << "操作符:" << opcodes;

    if(operand != "")
    {
        operands.push_back(operand);
        operand = "";
    }

    QString result = calculation();

    // 等号计算后，清空所有队列，将结果设为当前操作数
    operands.clear();
    opcodes.clear();
    if(result != "0" && result != "错误") {
        operand = result; // 将结果作为下一个操作数
    }

    ui->Display->setText(result);
    qDebug() << "等号计算完成 - 结果:" << result << "当前操作数:" << operand;
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
