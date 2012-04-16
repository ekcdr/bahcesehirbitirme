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

void sinavIstatistik::ilkYukleme()
{
    viewCizelge=new QStandardItemModel(this);
    ui->tableViewCizelge->hide();
}

void sinavIstatistik::cizelgeOlustur(QString dersisim,int sayi, QString kistas)
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
    ui->cizelge->setModel(viewCizelge);
    ui->cizelge->setBarScale(0.85);
    ui->cizelge->setBarType(QSint::BarChartPlotter::Columns);
    ui->cizelge->repaint();
    ui->cizelge->show();
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

sinavIstatistik::~sinavIstatistik()
{
    delete ui;
}
