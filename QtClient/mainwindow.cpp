#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHostAddress>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    m_tcp = new QTcpSocket;
    ui->setupUi(this);
    ui->port->setText("8899");
    ui->ip->setText("127.0.0.1");
    ui->disconnect->setDisabled(true);
    ui->connect->setDisabled(false);
    m_status = new QLabel;
    ui->statusbar->addWidget(new QLabel("连接状态"));
    ui->statusbar->addWidget(m_status);
    m_status->setPixmap(QPixmap(":/false.jpg").scaled(20,20));
    setWindowTitle("客户端");

    connect(m_tcp,&QTcpSocket::connected,this,[=]()
    {
       m_status->setPixmap(QPixmap(":/right.jpg").scaled(20,20));
       ui->record->append("成功和服务器建立连接!");
       ui->connect->setDisabled(true);
       ui->disconnect->setDisabled(false);
    });
    connect(m_tcp,&QTcpSocket::readyRead,this,[=]()
    {
        QByteArray data = m_tcp->readAll();
        ui->record->append("服务器说:"+data);
    });

    connect(m_tcp,&QTcpSocket::disconnected,this,[=]()
    {
        //m_tcp->close();
        //m_tcp->deleteLater();
        //去掉上两行可以解决闪退问题
        m_status->setPixmap(QPixmap(":/false.jpg").scaled(20,20));
        ui->record->append("已和服务器断开连接!");
        ui->connect->setDisabled(false);
        ui->disconnect->setDisabled(true);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_connect_clicked()
{
    unsigned short port = ui->port->text().toUShort();
    //unsigned short ip = ui->ip->text().toUShort();
    QString ip = ui->ip->text();
    m_tcp->connectToHost(QHostAddress(ip),port);
}

void MainWindow::on_sendMsg_clicked()
{
    QString msg = ui->msg->toPlainText();
    ui->record->append("客户端说:"+msg);
    m_tcp->write(msg.toUtf8());
    ui->msg->clear();
}

void MainWindow::on_disconnect_clicked()
{
    m_tcp->close();
    ui->connect->setDisabled(false);
    ui->disconnect->setDisabled(true);
}
