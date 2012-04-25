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
    yukleme();
}

void ekleSonuc::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Escape)
    {
        kapat();
    }
}

void ekleSonuc::kapat()
{
    close();
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
