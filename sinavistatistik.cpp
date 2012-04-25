#include "sinavistatistik.h"
#include "ui_sinavistatistik.h"

sinavIstatistik::sinavIstatistik(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sinavIstatistik)
{
    ui->setupUi(this);
    connect(ui->btnKapat,SIGNAL(clicked()),this,SLOT(kapat()));
    ilkYukleme();
}

void sinavIstatistik::kapat()
{
    close();
}

//void sinavIstatistik::cizelgeOlustur(QString dersisim,int sayi, QString kistas, QStringList etkinSorular)
void sinavIstatistik::cizelgeOlustur()
{
    /*
    viewCizelge->setColumnCount(0);
    viewCizelge->setRowCount(0);
    if(etkinSorular.count()==0)
    {
        viewCizelge->setVerticalHeaderLabels(QStringList()<<"toplampuan");
        ui->tableViewCizelge->setModel(viewCizelge);

        ui->cizelge->hide();//hızlıca cizelgeyi yazdırsın diye önce gizliyor sonunda açıyorum
        viewCizelge->setHeaderData(0, Qt::Vertical, Qt::red, Qt::BackgroundRole);

        QSqlQuery query;
        query.exec(QString("select ogrenci.ogrenciid,toplampuan from ogrenci,dersogrenci,sinavogrenci where ogrenci.ogrenciid=dersogrenci.ogrenciid and ogrenci.ogrenciid=sinavogrenci.ogrenciid and dersid=(select dersid from ders where dersisim='%1') and toplampuan!='--' and toplampuan %2 '%3' order by toplampuan").arg(dersisim).arg(kistas).arg(sayi));
        while(query.next())
        {
            viewCizelge->insertColumns(viewCizelge->columnCount(),1);
            viewCizelge->setHeaderData(viewCizelge->columnCount()-1,Qt::Horizontal, query.value(0).toString());
            viewCizelge->setData(viewCizelge->index(0,viewCizelge->columnCount()-1),query.value(1).toString());
        }
    }
    else
    {
        etkinSorular.insert(0,"toplam");

        viewCizelge->setVerticalHeaderLabels(etkinSorular);
        ui->tableViewCizelge->setModel(viewCizelge);

        ui->cizelge->hide();
        viewCizelge->setHeaderData(0, Qt::Vertical, Qt::red, Qt::BackgroundRole);

        QSqlQuery query;
        //query2.exec(QString("select toplampuan,alinanpuan from sonuc,sinavogrenci where sinavogrenci.ogrenciid=sonuc.ogrenciid and sinavogrenci.sinavid=sonuc.sinavid and sonuc.ogrenciid='%1' and sonuc.sinavid=%2 and sorunumarasi='%3'").arg(liste2.at(a)).arg(sinavid).arg(i+1));
        query.exec(QString("select ogrenci.ogrenciid,toplampuan from ogrenci,dersogrenci,sinavogrenci where ogrenci.ogrenciid=dersogrenci.ogrenciid and ogrenci.ogrenciid=sinavogrenci.ogrenciid and dersid=(select dersid from ders where dersisim='%1') and toplampuan!='--' and toplampuan %2 '%3' order by toplampuan").arg(dersisim).arg(kistas).arg(sayi));
        while(query.next())
        {
            viewCizelge->insertColumns(viewCizelge->columnCount(),1);
            viewCizelge->setHeaderData(viewCizelge->columnCount()-1,Qt::Horizontal, query.value(0).toString());
            viewCizelge->setData(viewCizelge->index(0,viewCizelge->columnCount()-1),query.value(1).toString());
        }
    }
    ui->cizelge->setModel(viewCizelge);
    ui->cizelge->setBarScale(0.85);
    ui->cizelge->setBarType(QSint::BarChartPlotter::Columns);
    ui->cizelge->repaint();
    ui->cizelge->show();
    */


    ui->cizelge->setModel(viewCizelge);
    ui->cizelge->setBarScale(0.85);
    ui->cizelge->setBarType(QSint::BarChartPlotter::Columns);
    ui->cizelge->repaint();
}

void sinavIstatistik::cizelgeYukleme(int toplampuan)
{
    ui->cizelge->axisY()->setRanges(0,toplampuan);
    ui->cizelge->axisY()->setTicks(2, 10);
    ui->cizelge->axisY()->setPen(QPen(Qt::darkGray));
    ui->cizelge->axisY()->setMinorTicksPen(QPen(Qt::gray));
    ui->cizelge->axisY()->setMajorTicksPen(QPen(Qt::darkGray));
    ui->cizelge->axisY()->setMinorGridPen(QPen(Qt::lightGray));
    ui->cizelge->axisY()->setMajorGridPen(QPen(Qt::gray));
    ui->cizelge->axisY()->setTextColor(Qt::black);

    ui->cizelge->axisX()->setPen(QPen(Qt::darkGray));
    ui->cizelge->axisX()->setMinorTicksPen(QPen(Qt::gray));
    ui->cizelge->axisX()->setMajorTicksPen(QPen(Qt::darkGray));
    ui->cizelge->axisX()->setMajorGridPen(QPen(Qt::lightGray));
    ui->cizelge->axisX()->setTextColor(Qt::black);

    ui->cizelge->setBarSize(32, 128);
    ui->cizelge->setBarOpacity(0.75);

    QLinearGradient bg(0,0,0,1);
    bg.setCoordinateMode(QGradient::ObjectBoundingMode);
    bg.setColorAt(1, Qt::white);
    bg.setColorAt(0, Qt::white);
    ui->cizelge->setBackground(QBrush(bg));
}

void sinavIstatistik::ilkYukleme()
{
    viewCizelge=new QStandardItemModel(this);
    ui->tableViewCizelge->hide();
}

sinavIstatistik::~sinavIstatistik()
{
    delete ui;
}
