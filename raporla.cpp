#include "raporla.h"
#include "ui_raporla.h"

raporla::raporla(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::raporla)
{
    ui->setupUi(this);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));
    connect(ui->btnKapat,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->tableWidget->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(sirala(int)));
    connect(ui->btnDisariAktar,SIGNAL(clicked()),this,SLOT(disariAktar()));
    connect(ui->cbSinav,SIGNAL(currentIndexChanged(QString)),this,SLOT(filtreSinav()));
    yukleme();
}

void raporla::disariAktar()
{
    QString dosya = QFileDialog::getSaveFileName(this,tr("Dosyayı Kaydet"),QCoreApplication::applicationDirPath()+"/untitled.csv",tr("(*.csv);;Tüm Dosyalar(*.*)"));
    QFile f(dosya);
    if (f.open(QFile::WriteOnly | QFile::Truncate))
    {
        QTextStream data( &f );
        QStringList strList;
        for( int r = 0; r < ui->tableWidget->rowCount(); ++r )
        {
            strList.clear();
            for( int c = 0; c < ui->tableWidget->columnCount(); ++c )
            {
                strList <<"\""+ui->tableWidget->item(r,c)->text()+"\"";
            }
            data << strList.join( ";" )+"\n";
        }
        f.close();
    }
}

void raporla::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Escape)
    {
        kapat();
    }
    else if(e->key()==Qt::Key_Return)
    {
        ui->btnRaporla->click();
    }
}

void raporla::kapat()
{
    close();
}

void raporla::filtreSinav()
{
    ui->tableWidget->setRowCount(0);
    ui->tableWidgetKonular->setRowCount(0);
    ui->txtSinavPuan->setText("0");
    ui->lblOgrenciSayi->clear();

    QSqlQuery query;
    //query.exec(QString("SELECT sorunumarasi,konuisim,puan FROM soru WHERE sinavid=(select sinavid from sinav where sinavisim='%1')").arg(ui->cbSinav->currentText()));
    query.exec(QString("select sorunumarasi,konuisim,puan from soru,derssinav where soru.sinavid=derssinav.sinavid and soru.sinavid=(select sinavid from sinav where sinavisim='%1') and dersid='%2'").arg(ui->cbSinav->currentText()).arg(dersID));
    while(query.next())
    {
        const int currentRow = ui->tableWidgetKonular->rowCount();
        ui->tableWidgetKonular->setRowCount(currentRow + 1);

        QCheckBox *cb=new QCheckBox();
        ui->tableWidgetKonular->setCellWidget(currentRow,0,cb);

        QTableWidgetItem *itm1=new QTableWidgetItem(query.value(0).toString());
        ui->tableWidgetKonular->setItem(currentRow,1,itm1);
        QTableWidgetItem *itm2=new QTableWidgetItem(query.value(1).toString());
        ui->tableWidgetKonular->setItem(currentRow,2,itm2);
        QTableWidgetItem *itm3=new QTableWidgetItem(query.value(2).toString());
        ui->tableWidgetKonular->setItem(currentRow,3,itm3);

        QComboBox *com=new QComboBox();
        com->addItems(QStringList()<<">"<<"<"<<">="<<"<="<<"="<<"!=");
        ui->tableWidgetKonular->setCellWidget(currentRow,4,com);

        QTableWidgetItem *itm4=new QTableWidgetItem("0");
        ui->tableWidgetKonular->setItem(currentRow,5,itm4);
        itm1->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        itm2->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        itm3->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        connect(cb,SIGNAL(clicked()),this,SLOT(konuEtkin()));
    }
    //query.exec(QString("select sinavpuan from sinav where sinavid=(SELECT sinav.sinavid FROM sinav,derssinav WHERE sinav.sinavid=derssinav.sinavid AND sinavisim='%1' AND dersid=(select dersid from ders where dersisim='%2'))").arg(ui->cbSinav->currentText()).arg(ui->tableDersler->currentItem()->text()));
    query.exec(QString("select sinavpuan from sinav where sinavid=(SELECT sinav.sinavid FROM sinav,derssinav WHERE sinav.sinavid=derssinav.sinavid AND sinavisim='%1' AND dersid='%2')").arg(ui->cbSinav->currentText()).arg(dersID));
    query.next();
    ui->lblSinavToplamPuan->setText("("+query.value(0).toString()+")");
    konuEtkin();
}

void raporla::konuEtkin()
{
    for(int i=0;i<ui->tableWidgetKonular->rowCount();i++)
    {
        QCheckBox *cb=qobject_cast<QCheckBox *>(ui->tableWidgetKonular->cellWidget(i,0));
        if(cb->isChecked()==false)
        {
            for(int h=1;h<6;h++)//6 sutun var
            {
                if(h==4)//4. combobox sutunu aşağıdaki else in içindeki kod hata verdiği için ayrı
                {
                    QComboBox *com=qobject_cast<QComboBox *>(ui->tableWidgetKonular->cellWidget(i,4));
                    com->setEnabled(false);
                }
                else
                {
                    QTableWidgetItem *itm= new QTableWidgetItem();
                    itm=ui->tableWidgetKonular->item(i,h);
                    itm->setFlags(Qt::NoItemFlags);
                    if(h==5)//5. puan sutunu
                    {
                        itm->setData(Qt::DisplayRole,"--");
                    }
                }
            }
        }
        if(cb->isChecked()==true)
        {
            for(int h=1;h<6;h++)
            {
                if(h==4)
                {
                    QComboBox *com=qobject_cast<QComboBox *>(ui->tableWidgetKonular->cellWidget(i,4));
                    com->setEnabled(true);
                }
                else
                {
                    QTableWidgetItem *itm= new QTableWidgetItem();
                    itm=ui->tableWidgetKonular->item(i,h);
                    if(h==5)
                    {
                        itm->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled);
                    }
                    else
                    {
                        itm->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                    }
                }
            }
        }
    }
}

void raporla::raporBirOncesi(QString dersIsim)
{
    setWindowTitle("rapor 1 ("+dersIsim+")");
    ui->tableWidget->setRowCount(0);
    ui->cbSinav->clear();
    ui->lblOgrenciSayi->clear();
    QSqlQuery query;
    query.exec(QString("SELECT sinavisim FROM sinav,derssinav WHERE derssinav.sinavid=sinav.sinavid and dersid=(select dersid from ders where dersisim='%1')").arg(dersIsim));
    while(query.next())
    {
        ui->cbSinav->addItem(query.value(0).toString());
    }
}

void raporla::sirala(int sutun)
{
    ui->tableWidget->sortByColumn(sutun);
}

void raporla::yukleme()
{
    ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableWidgetKonular->setRowCount(0);
    ui->tableWidgetKonular->setColumnCount(6);
    ui->tableWidgetKonular->setHorizontalHeaderLabels(QStringList()<<""<<"#"<<"isim"<<"puan"<<"kıstas"<<"alınan puan");
    ui->tableWidgetKonular->horizontalHeader()->setResizeMode(0,QHeaderView::ResizeToContents);
    ui->tableWidgetKonular->horizontalHeader()->setResizeMode(1,QHeaderView::ResizeToContents);
    ui->tableWidgetKonular->horizontalHeader()->setResizeMode(2,QHeaderView::ResizeToContents);
    ui->tableWidgetKonular->horizontalHeader()->setResizeMode(3,QHeaderView::ResizeToContents);
    ui->tableWidgetKonular->horizontalHeader()->setResizeMode(4,QHeaderView::ResizeToContents);
    ui->tableWidgetKonular->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidgetKonular->verticalHeader()->hide();
    ui->tableWidget->sortByColumn(1);
}

raporla::~raporla()
{
    delete ui;
}
