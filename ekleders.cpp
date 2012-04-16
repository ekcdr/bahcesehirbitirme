#include "ekleders.h"
#include "ui_ekleders.h"
#include <QMessageBox>
#include <QDebug>

ekleDers::ekleDers(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ekleDers)
{
    ui->setupUi(this);
    connect(ui->btnYilArttir,SIGNAL(clicked()),this,SLOT(yilArttir()));
    connect(ui->btnYilEksilt,SIGNAL(clicked()),this,SLOT(yilEksilt()));
    connect(ui->btnIptal,SIGNAL(clicked()),this,SLOT(kapat()));
    this->setWindowTitle("Ders Ekle");
}

void ekleDers::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Escape)
    {
        kapat();
    }
}

void ekleDers::kapat()
{
    close();
}

void ekleDers::uyari(int i)
{
    if(i==0)
    {
        QMessageBox::warning(this,"proje","boş alan var!","tamam");
    }
    else if(i==1)
    {
        QMessageBox::warning(this,"proje","bu koda sahip ders var","tamam");
    }
}

void ekleDers::yilArttir()
{
    QString yil;
    yil.append(ui->lblYil->text().at(0));
    yil.append(ui->lblYil->text().at(1));
    yil.append(ui->lblYil->text().at(2));
    yil.append(ui->lblYil->text().at(3));
    ui->lblYil->setText(QString::number(yil.toInt()+1)+" - "+QString::number(yil.toInt()+2));
}

void ekleDers::yilEksilt()
{
    QString yil;
    yil.append(ui->lblYil->text().at(0));
    yil.append(ui->lblYil->text().at(1));
    yil.append(ui->lblYil->text().at(2));
    yil.append(ui->lblYil->text().at(3));
    ui->lblYil->setText(QString::number(yil.toInt()-1)+" - "+QString::number(yil.toInt()));
}

ekleDers::~ekleDers()
{
    delete ui;
}
