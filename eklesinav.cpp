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
}

void ekleSinav::kapat()
{
    close();
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
