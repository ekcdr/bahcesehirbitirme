#include "ekleogrenci.h"
#include "ui_ekleogrenci.h"

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

void ekleOgrenci::keyPressEvent(QKeyEvent *e)
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

bool ekleOgrenci::tamam(QString ogrenciDers)
{
    QString ogrenciID=ui->txtOgrenciID->text();
    QString ogrenciIsim=ui->txtOgrenciIsim->text();
    QSqlQuery query,query2,query3;

    if(ogrenciID=="")
    {
        uyari(0);
        return false;
    }
    else
    {
        query.exec(QString("select count(ogrenci.ogrenciid) from ogrenci,dersogrenci where ogrenci.ogrenciid=dersogrenci.ogrenciid and dersid=(select dersid from ders where dersisim='%1') and ogrenci.ogrenciid='%2'").arg(ogrenciDers).arg(ogrenciID));
        query.next();
        if(query.value(0).toInt()!=0)
        {
            uyari(1);
            return false;
        }
        else
        {
            query.exec(QString("insert into ogrenci values('%1','%2')").arg(ogrenciID).arg(ogrenciIsim));

            query.exec(QString("select count (*) from derssinav where dersid=(SELECT dersid FROM ders WHERE dersisim='%1')").arg(ogrenciDers));
            query.next();
            if(query.value(0).toInt()!=0)
            {
                //query2.exec(QString("select sinavid from sinav,derssinav where sinav.sinavid=derssinav.sinavid and dersid=(SELECT dersid FROM ders WHERE dersisim='%1')").arg(ogrenciDers));
                query2.exec(QString("select sinavid from derssinav where dersid=(SELECT dersid FROM ders WHERE dersisim='%1')").arg(ogrenciDers));
                while(query2.next())
                {
                    query3.exec(QString("select sorusayisi from sinav where sinavid='%1'").arg(query2.value(0).toString()));
                    query3.next();

                    for(int i=0;i<query3.value(0).toInt();i++)
                    {
                        query.exec(QString("insert into sonuc values('%1','%2','%3','%4','%5')").arg(ogrenciID).arg(query2.value(0).toString()).arg(i+1).arg("--").arg("--"));
                    }
                    query.exec(QString("insert into sinavogrenci values('%1','%2','%3')").arg(ogrenciID).arg(query2.value(0).toString()).arg("--"));
                }
            }
            query.exec(QString("insert into dersogrenci values('%1',(SELECT dersid FROM ders WHERE dersisim='%2'))").arg(ogrenciID).arg(ogrenciDers));
            ui->txtOgrenciID->setText("");
            ui->txtOgrenciIsim->setText("");
            ui->txtOgrenciID->setFocus();
        }
    }
}

void ekleOgrenci::kapat()
{
    close();
}

void ekleOgrenci::ogrenciEklemeOncesi(QString dersIsim)
{
    this->setWindowTitle(dersIsim+" dersi için öğrenci ekle");
    ui->txtOgrenciID->clear();
    ui->txtOgrenciIsim->clear();
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
