#include "ekleders.h"
#include "ui_ekleders.h"

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
    else if(e->key()==Qt::Key_Return)
    {
        ui->btnTamam->click();
    }
}

bool ekleDers::tamam()
{
    QSqlQuery query;
    bool kayitVar=false;
    QString dersID=ui->txtDersID->text();
    QString dersIsim=ui->txtDersIsim->text();
    QString dersDonem=ui->cbDonem->currentText();
    QString dersYil=ui->lblYil->text();

    if(dersID=="" || dersIsim=="")
    {
        uyari(0);
        return false;
    }
    else
    {
        query.exec("select dersid from ders");
        while(query.next())
        {
            if(query.value(0).toString()==dersID)
            {
                uyari(1);
                kayitVar=true;
                break;
                return false;
            }
        }
        if(kayitVar==false)
        {
            query.exec(QString("insert into ders values('%1','%2','%3','%4')").arg(dersID).arg(dersIsim).arg(dersDonem).arg(dersYil));
            ui->txtDersID->setText("");
            ui->txtDersIsim->setText("");
            ui->txtDersID->setFocus();
            return true;
        }
    }
}

void ekleDers::kapat()
{
    close();
}

void ekleDers::dersEklemeOncesi()
{
    QDate tarih;
    int yil=tarih.currentDate().year();
    ui->lblYil->setText(QString::number(yil)+" - "+QString::number(yil+1));
    ui->txtDersID->clear();
    ui->txtDersIsim->clear();
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
