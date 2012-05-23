#include "eklesonuc.h"
#include "ui_eklesonuc.h"

ekleSonuc::ekleSonuc(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ekleSonuc)
{
    ui->setupUi(this);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));
    connect(ui->tableSonuclar,SIGNAL(cellChanged(int,int)),this,SLOT(puanKontrol(int,int)));
    connect(ui->tableSonuclar,SIGNAL(cellChanged(int,int)),this,SLOT(toplamiGuncellestir(int,int)));
    connect(ui->btnDisariAktar,SIGNAL(clicked()),this,SLOT(disariAktar()));
    yukleme();
}

void ekleSonuc::keyPressEvent(QKeyEvent *e)
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

void ekleSonuc::disariAktar()
{
    QString dosya = QFileDialog::getSaveFileName(this,tr("Dosyayı Kaydet"),QCoreApplication::applicationDirPath()+"/untitled.csv",tr("(*.csv);;Tüm Dosyalar(*.*)"));
    QFile f(dosya);
    if (f.open(QFile::WriteOnly | QFile::Truncate))
    {
        QTextStream data( &f );
        QStringList strList;
        for( int r = 0; r < ui->tableSonuclar->rowCount(); ++r )
        {
            strList.clear();
            for( int c = 0; c < ui->tableSonuclar->columnCount(); ++c )
            {
                strList <<"\""+ui->tableSonuclar->item(r,c)->text()+"\"";
            }
            data << strList.join( ";" )+"\n";
        }
        f.close();
    }
}

bool ekleSonuc::tamam()
{
    if(ekleSonucDegisiklikVar==true)
    {
        QSqlQuery query,query2;
        for(int h=0;h<degisenIDOgrenci.count();h++)
        {
            int ayrac=degisenIDOgrenci.at(h).indexOf("|");
            QString s=degisenIDOgrenci.at(h);
            QString ogrenciid=s.remove(ayrac,degisenIDOgrenci.at(h).length());
            s=degisenIDOgrenci.at(h);
            QString sinavid=s.remove(0,ogrenciid.length()+1);

            query.exec(QString("select sorusayisi from sinav where sinavid='%1'").arg(sinavid));
            query.next();
            int sorusayisi=query.value(0).toInt();

            int toplamPuan=0;
            for(int i=0;i<sorusayisi;i++)
            {
                QList<QTableWidgetItem *> liste=ui->tableSonuclar->findItems(ogrenciid,Qt::MatchExactly);
                for(int k=0;k<liste.count();k++)
                {
                    if(liste.at(k)->column()==0)
                    {
                        query2.exec(QString("select puan from soru where sinavid='%1' and sorunumarasi='%2'").arg(sinavid).arg(i+1));
                        query2.next();
                        double yuzde=ui->tableSonuclar->item(liste.at(k)->row(),i+1)->text().toDouble()/query2.value(0).toDouble()*100;
                        if(ui->tableSonuclar->item(liste.at(k)->row(),i+1)->text()=="--")
                        {
                            query.exec(QString("update sonuc set alinanpuan='%1', yuzde='%2' where sinavid='%3' and ogrenciid='%4' and sorunumarasi='%5'").arg(0).arg(yuzde).arg(sinavid).arg(ogrenciid).arg(i+1));
                        }
                        else
                        {
                            query.exec(QString("update sonuc set alinanpuan='%1', yuzde='%2' where sinavid='%3' and ogrenciid='%4' and sorunumarasi='%5'").arg(ui->tableSonuclar->item(liste.at(k)->row(),i+1)->text()).arg(yuzde).arg(sinavid).arg(ogrenciid).arg(i+1));
                        }
                        toplamPuan=toplamPuan+ui->tableSonuclar->item(liste.at(k)->row(),i+1)->text().toInt();
                    }
                }
            }
            query.exec(QString("update sinavogrenci set toplampuan='%1' where sinavid='%2' and ogrenciid='%3'").arg(toplamPuan).arg(sinavid).arg(ogrenciid));
        }
        uyari(1);
        ekleSonucDegisiklikVar=false;
        degisenIDOgrenci.clear();
        return true;
    }
    else
    {
        uyari(0);
        return false;
    }
}

void ekleSonuc::kapat()
{
    close();
}

void ekleSonuc::sonucEklemeOncesi(QString dersIsim)
{
    setWindowTitle(dersIsim+" dersi için sonuçları gir");
    ui->cbSinav->clear();
    QSqlQuery query;
    query.exec(QString("select sinavisim from sinav,derssinav where sinav.sinavid=derssinav.sinavid and dersid=(select dersid from ders where dersisim='%1')").arg(dersIsim));
    while(query.next())
    {
        ui->cbSinav->addItem(query.value(0).toString());
    }
    degisenIDOgrenci.clear();
    ekleSonucDegisiklikVar=false;
}

void ekleSonuc::toplamiGuncellestir(int i, int j)
{  
    if(ilkAcilis==false && j!=ui->tableSonuclar->columnCount()-1) //toplami değiştirdiğinde bir daha fonk a girmesin diye
    {
        int toplamPuan=0;
        for(int k=1;k<ui->tableSonuclar->columnCount()-1;k++)
        {
            toplamPuan=toplamPuan+ui->tableSonuclar->item(i,k)->text().toInt();
        }
        ui->tableSonuclar->item(i,ui->tableSonuclar->columnCount()-1)->setText(QString::number(toplamPuan));
        ekleSonucDegisiklikVar=true;
    }
}

void ekleSonuc::puanKontrol(int i, int j)
{
    if(ilkAcilis==false && j!=ui->tableSonuclar->columnCount()-1) //toplami değiştirdiğinde bir daha fonk a girmesin diye
    {
        QString degisen=ui->tableSonuclar->item(i,0)->text()+"|"+sinavID;
        if(!degisenIDOgrenci.contains(degisen))
        {
            degisenIDOgrenci.append(degisen);
        }

        if(j!=ui->tableSonuclar->columnCount()-1)//toplam sutununda kontrol yapmasın
        {
            if(j!=0 && i!=0)
            {
                if(ui->tableSonuclar->item(i,j)->text()!=QString::number(ui->tableSonuclar->item(i,j)->text().toInt()))
                {
                    uyari(2);
                    ui->tableSonuclar->item(i,j)->setText("0");
                }
                else if(ui->tableSonuclar->item(i,j)->text().toInt()<0)
                {
                    uyari(4);
                    ui->tableSonuclar->item(i,j)->setText("0");
                }
                else
                {
                    QSqlQuery query;
                    query.exec(QString("select puan from soru where sinavid='%1' and sorunumarasi='%2'").arg(sinavID).arg(j));
                    query.next();
                    if(query.value(0).toInt()<ui->tableSonuclar->item(i,j)->text().toInt())
                    {
                        uyari(3);
                        ui->tableSonuclar->item(i,j)->setText("0");
                    }
                }
            }
        }
    }
}

void ekleSonuc::uyari(int i)
{
    if(i==0)
    {
        QMessageBox::information(this,"proje","değişiklik yapılmadı","tamam");
    }
    else if(i==1)
    {
        QMessageBox::information(this,"proje","kayıtlar değiştirildi","tamam");
    }
    else if(i==2)
    {
        QMessageBox::warning(this,"proje","sayı girin","tamam");
    }
    else if(i==3)
    {
        QMessageBox::warning(this,"proje","soru puanından daha yüksek puan girilemez","tamam");
    }
    else if(i==4)
    {
        QMessageBox::warning(this,"proje","eksi değer girilemez","tamam");
    }
}

void ekleSonuc::yukleme()
{
    ilkAcilis=true;
    ekleSonucDegisiklikVar=false;
    ui->tableSonuclar->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
}

ekleSonuc::~ekleSonuc()
{
    delete ui;
}
