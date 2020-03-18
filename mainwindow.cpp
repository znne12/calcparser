#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "calcparser.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_plainTextEdit_textChanged()
{
    QString str = ui->plainTextEdit->toPlainText();
    qint32 pos = 0;
    QString error = "";
    while(1){
        qint32 fnd = str.indexOf("#@", pos);
        if (fnd == -1) return;
        pos = fnd + 1;
        int r = str.indexOf("=", fnd);
        if (r != -1){
            QString tmpStr = str.mid(fnd+2, r-fnd-2);
            str.remove("#@");
            calcparser parser(&tmpStr, &error);
            parser.init();
            if (error == "") str.insert(str.size(), tmpStr);
            else {
                str.chop(1);
                str.push_back("      " + error);
            }
            ui->plainTextEdit->setPlainText(str);
        }
    }
}
