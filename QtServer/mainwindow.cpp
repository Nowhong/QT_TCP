#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    setWindowTitle("服务器");
    m_s = new QTcpServer(this);
    ui->port->setText("8899");
    m_status = new QLabel;
    ui->statusbar->addWidget(new QLabel("连接状态"));
    ui->statusbar->addWidget(m_status);
    m_status->setPixmap(QPixmap(":/false.jpg").scaled(20,20));

    connect(m_s,&QTcpServer::newConnection,this,[=]()
    {
       m_tcp = m_s->nextPendingConnection();
       ui->record->append("成功和客户端建立了连接!");
       m_status->setPixmap(QPixmap(":/right.jpg").scaled(20,20));
       connect(m_tcp,&QTcpSocket::readyRead,this,[=]()
       {
           QByteArray data = m_tcp->readAll();
           ui->record->append("客户端说:"+data);
       });
       connect(m_tcp,&QTcpSocket::disconnected,this,[=]()
       {
           ui->record->append("客户端断开了连接!");
           m_tcp->close();
           m_tcp->deleteLater();
           m_status->setPixmap(QPixmap(":/false.jpg").scaled(20,20));
       });

    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_setListen_clicked()
{
    unsigned short port = ui->port->text().toUShort();
    m_s->listen(QHostAddress::Any,port);
    ui->setListen->setDisabled(true);
}

void MainWindow::on_sendMsg_clicked()
{
    QString msg = ui->msg->toPlainText();
    m_tcp->write(msg.toUtf8());
    ui->record->append("服务器说:"+msg);
    ui->msg->clear();
}
