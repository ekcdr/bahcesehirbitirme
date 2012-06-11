#include "listele.h"
#include "ui_listele.h"

listele::listele(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::listele)
{
    ui->setupUi(this);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));
    connect(ui->btnKapat,SIGNAL(clicked()),this,SLOT(kapat()));
    connect(ui->btnDisariAktar,SIGNAL(clicked()),this,SLOT(disariAktar()));
    ui->tableListe->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
}

void listele::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Escape)
    {
        kapat();
    }
}

void listele::kapat()
{
    close();
}

void listele::disariAktar()
{
    QString dosya = QFileDialog::getSaveFileName(this,tr("Dosyayı Kaydet"),QCoreApplication::applicationDirPath()+"/untitled.csv",tr("(*.csv);;Tüm Dosyalar(*.*)"));
    QFile f(dosya);
    if (f.open(QFile::WriteOnly | QFile::Truncate))
    {
        QTextStream data( &f );
        QStringList strList;
        for( int r = 0; r < ui->tableListe->rowCount(); ++r )
        {
            strList.clear();
            for( int c = 0; c < ui->tableListe->columnCount(); ++c )
            {
                strList <<"\""+ui->tableListe->item(r,c)->text()+"\"";
            }
            data << strList.join( ";" )+"\n";
        }
        f.close();
    }
}

void listele::sinavListeleOncesi(QString dersIsmi)
{
    setWindowTitle("sınav listesi ("+dersIsmi+")");
    yuklemeSinav();

    QSqlQuery query,query2;
    query.exec(QString("select sinavisim,sorusayisi,sinavpuan,sinav.sinavid from sinav,derssinav where derssinav.sinavid=sinav.sinavid and dersid=(select dersid from ders where dersisim='%1')").arg(dersIsmi));
    while(query.next())
    {
        const int currentRow = ui->tableListe->rowCount();
        ui->tableListe->setRowCount(currentRow + 1);
        QTableWidgetItem *itm0=new QTableWidgetItem(query.value(0).toString());
        ui->tableListe->setItem(currentRow,0,itm0);
        QTableWidgetItem *itm1=new QTableWidgetItem(query.value(1).toString());
        ui->tableListe->setItem(currentRow,1,itm1);
        QTableWidgetItem *itm2=new QTableWidgetItem(query.value(2).toString());
        ui->tableListe->setItem(currentRow,2,itm2);
        query2.exec(QString("select avg (toplampuan) from sinavogrenci where sinavid='%1'").arg(query.value(3).toString()));
        query2.next();
        QTableWidgetItem *itm3=new QTableWidgetItem(QString::number(query2.value(0).toDouble(),'f',2));
        ui->tableListe->setItem(currentRow,3,itm3);
        itm0->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        itm1->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        itm2->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        itm3->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    }
}

void listele::ogrenciListeleOncesi(QString dersIsmi)
{
    QSqlQuery query,query2;
    setWindowTitle("öğrenci listesi ("+dersIsmi+")");
    yuklemeOgrenci();

    //ogrenci listesine sınavları ekliyor
    query.exec(QString("select sinavisim from derssinav,sinav where derssinav.sinavid=sinav.sinavid and dersid=(select dersid from ders where dersisim='%1')").arg(dersIsmi));
    while(query.next())
    {
        const int currentColumn = ui->tableListe->columnCount();
        ui->tableListe->setColumnCount(currentColumn + 1);
        QTableWidgetItem *itm=new QTableWidgetItem(query.value(0).toString());
        ui->tableListe->setHorizontalHeaderItem(currentColumn,itm);
        itm->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    }

    int i=0;
    query.exec(QString("select ogrenci.ogrenciid,isim from ogrenci,dersogrenci where ogrenci.ogrenciid=dersogrenci.ogrenciid and dersid=(select dersid from ders where dersisim='%1')").arg(dersIsmi));
    while(query.next())
    {
        int j=2;
        const int currentRow = ui->tableListe->rowCount();
        ui->tableListe->setRowCount(currentRow + 1);
        QTableWidgetItem *itm0=new QTableWidgetItem(query.value(0).toString());
        ui->tableListe->setItem(currentRow,0,itm0);
        QTableWidgetItem *itm1=new QTableWidgetItem(query.value(1).toString());
        ui->tableListe->setItem(currentRow,1,itm1);

        query2.exec(QString("select toplampuan from sinavogrenci where ogrenciid='%1'").arg(ui->tableListe->item(i,0)->text()));
        while(query2.next())
        {
            QTableWidgetItem *itm=new QTableWidgetItem(query2.value(0).toString());
            ui->tableListe->setItem(i,j,itm);
            itm->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            j++;
        }
        itm0->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        itm1->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        i++;
    }
}

void listele::yuklemeSinav()
{
    ui->tableListe->setRowCount(0);
    ui->tableListe->setColumnCount(4);
    QStringList baslik;
    baslik<<tr("İsim")<<tr("Soru Sayısı")<<tr("Toplam Puan")<<tr("Ortalama");
    ui->tableListe->setHorizontalHeaderLabels(baslik);
}

void listele::yuklemeOgrenci()
{
    ui->tableListe->setRowCount(0);
    ui->tableListe->setColumnCount(2);
    QStringList baslik;
    baslik<<tr("ID")<<tr("İsim");
    ui->tableListe->setHorizontalHeaderLabels(baslik);
}

listele::~listele()
{
    delete ui;
}
