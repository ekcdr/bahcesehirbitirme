#include "ekleogrenci.h"
#include "ui_ekleogrenci.h"
#include <QMessageBox>
#include <QTextCodec>

ekleOgrenci::ekleOgrenci(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ekleOgrenci)
{
    ui->setupUi(this);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));
    connect(ui->btnIptal,SIGNAL(clicked()),this,SLOT(kapat()));
}

void ekleOgrenci::kapat()
{
    close();
}

void ekleOgrenci::uyari(int i)
{
    if(i==0)
    {
        QMessageBox::warning(this,"proje","boş alan var!","tamam");
    }
    else if(i==1)
    {
        QMessageBox::warning(this,"proje","bu numaraya sahip öğrenci var!","tamam");
    }
}

ekleOgrenci::~ekleOgrenci()
{
    delete ui;
}
