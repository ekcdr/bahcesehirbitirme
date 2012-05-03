#include "eklesinav.h"
#include "ui_eklesinav.h"

ekleSinav::ekleSinav(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ekleSinav)
{
    ui->setupUi(this);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));
    connect(ui->btnDoldur,SIGNAL(clicked()),this,SLOT(doldur()));
    connect(ui->tableKonular,SIGNAL(cellChanged(int,int)),this,SLOT(puanKontrol(int,int)));
    connect(ui->btnIptal,SIGNAL(clicked()),this,SLOT(kapat()));
    yukleme();
}

void ekleSinav::keyPressEvent(QKeyEvent *e)
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

bool ekleSinav::tamam(QString sinavDers)
{
    bool sifirNotVar=false;
    QString sinavIsim=ui->txtSinavIsim->text();
    QString sinavSoruSayisi=ui->txtSinavSoruSayisi->text();
    int sinavToplamPuan=0;

    if(sinavIsim=="" || sinavSoruSayisi=="")
    {
        uyari(0);
        return false;
    }
    else
    {
        QSqlQuery query,query2,query3;
        query.exec(QString("select count(sinavisim) from sinav,derssinav where sinav.sinavid=derssinav.sinavid and dersid=(select dersid from ders where dersisim='%1') and sinavisim='%2'").arg(sinavDers).arg(sinavIsim));
        query.next();
        if(query.value(0).toInt()!=0)
        {
            uyari(1);
            return false;
        }
        else
        {
            for(int i=0;i<ui->tableKonular->rowCount();i++)
            {
                if(ui->tableKonular->item(i,1)->text().toInt()<=0)
                {
                    sifirNotVar=true;
                    i=ui->tableKonular->rowCount();
                    uyari(3);
                    return false;
                }
            }
            if(sifirNotVar==false)//soru puanının sıfır olmaması için
            {
                for(int i=1;i<=sinavSoruSayisi.toInt();i++)
                {
                    sinavToplamPuan=sinavToplamPuan+ui->tableKonular->item(i-1,1)->text().toInt();
                }
                query.exec(QString("insert into sinav (sinavisim,sorusayisi,sinavpuan) values('%1','%2','%3')").arg(sinavIsim).arg(sinavSoruSayisi).arg(sinavToplamPuan));
                query.exec("select sinavid from sinav");
                query.last();

                query3.exec(QString("select ogrenci.ogrenciid from ogrenci,dersogrenci where ogrenci.ogrenciid=dersogrenci.ogrenciid and dersid=(select dersid from ders where dersisim='%1')").arg(sinavDers));
                while(query3.next())
                {
                    for(int i=0;i<sinavSoruSayisi.toInt();i++)
                    {
                        query2.exec(QString("insert into sonuc values('%1','%2','%3','%4','%5')").arg(query3.value(0).toString()).arg(query.value(0).toString()).arg(i+1).arg("--").arg("--"));
                    }
                    query2.exec(QString("insert into sinavogrenci values('%1','%2','%3')").arg(query3.value(0).toString()).arg(query.value(0).toString()).arg("--"));
                }
                query2.exec(QString("insert into derssinav values ('%1',(select dersid from ders where dersisim='%2'))").arg(query.value(0).toString()).arg(sinavDers));

                ui->txtSinavIsim->setText("");
                ui->txtSinavSoruSayisi->setText("");

                for(int i=1;i<=sinavSoruSayisi.toInt();i++)
                {
                    query2.exec(QString("insert into soru values ((select sinav.sinavid from sinav,derssinav where sinav.sinavid=derssinav.sinavid and sinav.sinavisim='%1' and derssinav.dersid=(select dersid from ders where dersisim='%2')),'%3','%4','%5')").arg(sinavIsim).arg(sinavDers).arg(i).arg(ui->tableKonular->item(i-1,0)->text()).arg(ui->tableKonular->item(i-1,1)->text()));
                }

                for(int i=1;i<=sinavSoruSayisi.toInt();i++)
                {
                    query.exec(QString("select count(konuisim) from konu where dersid=(select dersid from ders where dersisim='%1') and konuisim='%2'").arg(sinavDers).arg(ui->tableKonular->item(i-1,0)->text()));
                    query.next();
                    if(query.value(0).toInt()==0)
                    {
                        query2.exec(QString("insert into konu (konuisim,dersid) values('%1',(select dersid from ders where dersisim='%2'))").arg(ui->tableKonular->item(i-1,0)->text()).arg(sinavDers));
                    }
                }
                ui->tableKonular->setRowCount(0);
                ui->txtSinavIsim->setFocus();
                return true;
            }
        }
    }
}

void ekleSinav::kapat()
{
    close();
}

void ekleSinav::sinavEklemeOncesi(QString dersIsim)
{
    this->setWindowTitle(dersIsim+" dersi için sınav ekle");
    ui->txtSinavIsim->clear();
    ui->txtSinavSoruSayisi->clear();
    ui->tableKonular->setRowCount(0);
}

void ekleSinav::puanKontrol(int i, int j)
{
    if(j==1 && ui->tableKonular->item(i,j)->text()!=QString::number(ui->tableKonular->item(i,j)->text().toInt()))
    {
        uyari(2);
        ui->tableKonular->item(i,j)->setText("0");
    }
    if(j==0)
    {
        bosVar=false;
        for(int k=0;k<ui->tableKonular->rowCount();k++)
        {
            if(ui->tableKonular->item(k,0)->text()=="")
            {
                bosVar=true;
            }
        }
    }
    if(bosVar==false)
    {
        ui->btnTamam->setEnabled(true);
    }
}

void ekleSinav::doldur()
{
    bosVar=true;
    ui->btnTamam->setEnabled(false);
    if(ui->txtSinavSoruSayisi->text()==QString::number(ui->txtSinavSoruSayisi->text().toInt()))
    {
        QString sinavSoruSayisi=ui->txtSinavSoruSayisi->text();
        ui->tableKonular->setRowCount(0);

        for(int i=1;i<=sinavSoruSayisi.toInt();i++)
        {
            const int currentRow = ui->tableKonular->rowCount();
            ui->tableKonular->setRowCount(currentRow + 1);
            QTableWidgetItem *itm1=new QTableWidgetItem("");
            ui->tableKonular->setItem(currentRow,0,itm1);
            QTableWidgetItem *itm2=new QTableWidgetItem("0");
            ui->tableKonular->setItem(currentRow,1,itm2);
        }
    }
    else
    {
        uyari(2);
    }
}

void ekleSinav::uyari(int i)
{
    if(i==0)
    {
        QMessageBox::warning(this,"proje","boş alan var!","tamam");
    }
    else if(i==1)
    {
        QMessageBox::warning(this,"proje","bu sınav önceden girilmiş!","tamam");
    }
    else if(i==2)
    {
        QMessageBox::warning(this,"proje","sayı girin!","tamam");
    }
    else if(i==3)
    {
        QMessageBox::warning(this,"proje","not 0 veya 0'dan küçük olamaz","tamam");
    }
}

void ekleSinav::yukleme()
{
    ui->btnTamam->setEnabled(false);
    ui->tableKonular->setRowCount(0);
    ui->tableKonular->setColumnCount(2);
    QStringList baslik;
    baslik<<tr("Konu")<<tr("Puan");
    ui->tableKonular->setHorizontalHeaderLabels(baslik);
    QHeaderView *header1 = ui->tableKonular->horizontalHeader();
    header1->setStretchLastSection(true);

    this->setWindowTitle("Sınav Ekle");
    ui->tableKonular->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
}

ekleSinav::~ekleSinav()
{
    delete ui;
}
