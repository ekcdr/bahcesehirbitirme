#include "raporla2.h"
#include "ui_raporla2.h"

raporla2::raporla2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::raporla2)
{
    ui->setupUi(this);
    connect(ui->cbSinav,SIGNAL(currentIndexChanged(QString)),this,SLOT(sinavDegisti()));
    connect(ui->btnKapat,SIGNAL(clicked()),this,SLOT(kapat()));
    connect(ui->cbKistas,SIGNAL(currentIndexChanged(int)),this,SLOT(raporTuru(int)));
    connect(ui->btnOlustur,SIGNAL(clicked()),this,SLOT(raporOlusturDenetleme()));
    yukleme();
}

void raporla2::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Escape)
    {
        kapat();
    }
    else if(e->key()==Qt::Key_Return)
    {
        raporOlusturDenetleme();
    }
}

bool raporla2::denetle()
{
    if(ui->txtYuzdeAlt->text()!=QString::number(ui->txtYuzdeAlt->text().toDouble()) || ui->txtYuzdeUst->text()!=QString::number(ui->txtYuzdeUst->text().toDouble()))
    {
        QMessageBox::warning(this,"proje","hatalı giriş","Tamam");
        return false;
    }
    return true;
}

void raporla2::raporOlusturDenetleme()
{
    if(denetle()==true)
    {
        raporOlustur(ui->cbKistas->currentIndex());
    }
}

void raporla2::konulariEkle()//grafiğin yanındaki table a konuları ekliyor
{
    ui->tableGrafikKonular->setRowCount(0);
    QSqlQuery query;
    query.exec(QString("SELECT konuisim FROM soru WHERE sinavid=(select sinavid from derssinav where dersid='%1')").arg(dersid));
    while(query.next())
    {
        const int currentRow = ui->tableGrafikKonular->rowCount();
        ui->tableGrafikKonular->setRowCount(currentRow + 1);
        QRadioButton *rb=new QRadioButton(query.value(0).toString());
        ui->tableGrafikKonular->setCellWidget(currentRow,0,rb);
        connect(rb,SIGNAL(clicked()),this,SLOT(rbCizelge()));
    }
    QRadioButton *rb=qobject_cast<QRadioButton *>(ui->tableGrafikKonular->cellWidget(0,0));
    rb->setChecked(true);
    rbCizelge();
}

void raporla2::raporTuru(int a)
{
    ui->tableWidget->setRowCount(0);
    if(a==0)
    {
        ui->btnOlustur->setEnabled(false);
        ui->txtYuzdeAlt->setEnabled(false);
        ui->txtYuzdeUst->setEnabled(false);
        ui->txtYuzde->setEnabled(false);
        ui->lblAlt->setEnabled(false);
        ui->lblUst->setEnabled(false);
        ui->lblYuzde->setEnabled(false);
        ui->cbKistasYuzde->setEnabled(false);
        ui->tableGrafikKonular->hide();
    }
    else if(a==1 || a==2)
    {
        viewCizelge->clear();
        ui->cizelge->repaint();
        ui->label->clear();
        ui->txtYuzdeAlt->setEnabled(false);
        ui->txtYuzdeUst->setEnabled(false);
        ui->txtYuzde->setEnabled(false);
        ui->btnOlustur->setEnabled(true);
        ui->lblAlt->setEnabled(false);
        ui->lblUst->setEnabled(false);
        ui->lblYuzde->setEnabled(false);
        ui->cbKistasYuzde->setEnabled(false);
        ui->tableGrafikKonular->hide();
        if(a==2)
        {
            ui->lbl1->hide();
            ui->lbl2->hide();
            ui->lbl3->hide();
            ui->lbl4->hide();
            ui->lbl1p->hide();
            ui->lbl2p->hide();
            ui->lbl3p->hide();
            ui->lbl4p->hide();
            ui->cizelge->hide();
            //ui->tabGrafik->setEnabled(false);
            //ui->tabGrafik->hide();
        }
        else if(a==1)
        {
            ui->cizelge->show();
            ui->lbl1->show();
            ui->lbl2->show();
            ui->lbl3->show();
            ui->lbl4->show();
            ui->lbl1p->show();
            ui->lbl2p->show();
            ui->lbl3p->show();
            ui->lbl4p->show();
            ui->lbl1p->setText("0-26");
            ui->lbl2p->setText("26-51");
            ui->lbl3p->setText("51-76");
            ui->lbl4p->setText("76-100");
        }
    }
    else if(a==3 || a==4)
    {
        viewCizelge->clear();
        ui->cizelge->repaint();
        ui->btnOlustur->setEnabled(true);
        ui->txtYuzdeAlt->setEnabled(true);
        ui->txtYuzdeUst->setEnabled(true);
        ui->txtYuzde->setEnabled(true);
        ui->lblAlt->setEnabled(true);
        ui->lblUst->setEnabled(true);
        ui->lblYuzde->setEnabled(true);
        ui->cbKistasYuzde->setEnabled(true);
        ui->tableGrafikKonular->hide();
        ui->cizelge->show();
        ui->lbl1->show();
        ui->lbl2->show();
        ui->lbl1p->show();
        ui->lbl2p->show();
        ui->lbl1p->setText("başarısız");
        ui->lbl2p->setText("başarılı");
        ui->lbl3->hide();
        ui->lbl4->hide();
        ui->lbl3p->hide();
        ui->lbl4p->hide();
        if(a==4)
        {
            ui->tableGrafikKonular->show();
            konulariEkle();
        }
    }
}

void raporla2::kapat()
{
    close();
}

void raporla2::sinavDegisti()
{
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);
    ui->cbKistas->setCurrentIndex(0);
}

void raporla2::rbCizelge()//rb değiştiğinde cizelgelisti güncelliyor
{
    viewCizelge->setColumnCount(0);
    QString seciliKonu;
    QSqlQuery query;

    query.exec(QString("select sorusayisi from sinav,derssinav where sinav.sinavid=derssinav.sinavid and sinavisim='%1' and dersid='%2'").arg(ui->cbSinav->currentText()).arg(dersid));
    query.next();
    for(int m=0;m<query.value(0).toInt();m++)
    {
        QRadioButton *rb=qobject_cast<QRadioButton *>(ui->tableGrafikKonular->cellWidget(m,0));
        if(rb->isChecked()==true)
        {
            seciliKonu=rb->text();
            break;
        }
    }
    for(int i=0;i<sonuc2Yuzdeler->rowCount();i++)
    {
        if(sonuc2Yuzdeler->data(sonuc2Yuzdeler->index(i,0),Qt::DisplayRole).toString()==seciliKonu)
        {
            QStringList cizelgeList;
            cizelgeList.append(sonuc2Yuzdeler->data(sonuc2Yuzdeler->index(i,1),Qt::DisplayRole).toString());
            cizelgeList.append(sonuc2Yuzdeler->data(sonuc2Yuzdeler->index(i,2),Qt::DisplayRole).toString());
            cizelgeList.append(sonuc2Yuzdeler->data(sonuc2Yuzdeler->index(i,3),Qt::DisplayRole).toString());
            if(!cizelgeList.isEmpty())
            {
                cizelgeViewOlustur(4,cizelgeList);
            }
        }
    }
    cizelgeOlustur();
}

void raporla2::cizelgeOlustur()
{
    ui->cizelge->setModel(viewCizelge);
    ui->cizelge->setBarScale(0.85);
    ui->cizelge->setBarType(QSint::BarChartPlotter::Columns);
    ui->cizelge->repaint();
    ui->cizelge->show();
}

void raporla2::cizelgeViewOlustur(int tur, QStringList liste) //viewcizelge ye cizelgede gösterilecek verileri yazıyor
{   //1:krapor1
    //2:krapor2
    //3:sonucrapor1
    //4:sonucrapor2
    if(tur==1)
    {
        ui->cizelge->hide();//hızlıca cizelgeyi yazdırsın diye önce gizliyor sonunda açıyorum
        viewCizelge->setVerticalHeaderLabels(QStringList()<<"0-26"<<"26-51"<<"51-76"<<"76-101");
        ui->tableViewCizelge->setModel(viewCizelge);

        viewCizelge->setHeaderData(0, Qt::Vertical, Qt::red, Qt::BackgroundRole);
        viewCizelge->setHeaderData(1, Qt::Vertical, Qt::green, Qt::BackgroundRole);
        viewCizelge->setHeaderData(2, Qt::Vertical, Qt::blue, Qt::BackgroundRole);
        viewCizelge->setHeaderData(3, Qt::Vertical, Qt::black, Qt::BackgroundRole);

        viewCizelge->insertColumns(viewCizelge->columnCount(),1);
        viewCizelge->setHeaderData(viewCizelge->columnCount()-1, Qt::Horizontal,liste.at(0));
        viewCizelge->setData(viewCizelge->index(0,viewCizelge->columnCount()-1),liste.at(1));
        viewCizelge->setData(viewCizelge->index(1,viewCizelge->columnCount()-1),liste.at(2));
        viewCizelge->setData(viewCizelge->index(2,viewCizelge->columnCount()-1),liste.at(3));
        viewCizelge->setData(viewCizelge->index(3,viewCizelge->columnCount()-1),liste.at(4));
    }
    else if(tur==3)
    {
        ui->cizelge->hide();//hızlıca cizelgeyi yazdırsın diye önce gizliyor sonunda açıyorum

        viewCizelge->setVerticalHeaderLabels(QStringList()<<"başarısız"<<"başarılı");
        ui->tableViewCizelge->setModel(viewCizelge);

        viewCizelge->setHeaderData(0, Qt::Vertical, Qt::red, Qt::BackgroundRole);
        viewCizelge->setHeaderData(1, Qt::Vertical, Qt::green, Qt::BackgroundRole);

        viewCizelge->insertColumns(viewCizelge->columnCount(),1);
        viewCizelge->setHeaderData(viewCizelge->columnCount()-1, Qt::Horizontal,liste.at(0));
        viewCizelge->setData(viewCizelge->index(0,viewCizelge->columnCount()-1),liste.at(1));
        viewCizelge->setData(viewCizelge->index(1,viewCizelge->columnCount()-1),liste.at(2));
    }
    else if(tur==4)
    {
        ui->cizelge->hide();
        viewCizelge->setVerticalHeaderLabels(QStringList()<<"başarısız"<<"başarılı");
        ui->tableViewCizelge->setModel(viewCizelge);

        viewCizelge->setHeaderData(0, Qt::Vertical, Qt::red, Qt::BackgroundRole);
        viewCizelge->setHeaderData(1, Qt::Vertical, Qt::green, Qt::BackgroundRole);

        viewCizelge->insertColumns(viewCizelge->columnCount(),1);
        viewCizelge->setHeaderData(viewCizelge->columnCount()-1, Qt::Horizontal,liste.at(0));
        viewCizelge->setData(viewCizelge->index(0,viewCizelge->columnCount()-1),liste.at(1));
        viewCizelge->setData(viewCizelge->index(1,viewCizelge->columnCount()-1),liste.at(2));
    }
}

void raporla2::raporOlustur(int a)
{
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);
    sonuc2Yuzdeler->setRowCount(0);
    if(a==1)
    {
        rapor2birer();
    }
    else if(a==2)
    {
        ui->tableWidget->setColumnCount(4);
        rapor2ikiser();
    }
    else if(a==3)
    {
        ui->tableWidget->setColumnCount(1);
        rapor3birer();
    }
    else if(a==4)
    {
        ui->tableWidget->setColumnCount(1);
        rapor3ikiser();
        rbCizelge();
    }
}

void raporla2::rapor3ikiser()
{
    QSqlQuery query,query2,query3,query4;
    QStringList sinirlarAlt,sinirlarUst;
    sinirlarAlt<<"0"<<ui->txtYuzdeAlt->text();
    sinirlarUst<<ui->txtYuzdeUst->text()<<"101";
    ui->label->setText("başarısız: 0 - "+ui->txtYuzdeAlt->text()+"\n"+"başarılı: "+ui->txtYuzdeUst->text()+" - 100");
    viewCizelge->setRowCount(0);
    viewCizelge->removeColumns(0,viewCizelge->columnCount());

    query.exec(QString("select sorusayisi,sinav.sinavid from sinav,derssinav where sinav.sinavid=derssinav.sinavid and sinavisim='%1' and dersid='%2'").arg(ui->cbSinav->currentText()).arg(dersid));
    query.next();
    int sorusayisi=query.value(0).toInt();
    int sinavid=query.value(1).toInt();

    for(int i=1;i<=sorusayisi;i++)
    {
        query3.exec(QString("select konuisim,puan from soru where sorunumarasi='%1' and sinavid=(select sinavid from sinav where sinavisim='%2')").arg(i).arg(ui->cbSinav->currentText()));
        query3.next();
        const int currentRow = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(currentRow + 1);
        QTableWidgetItem *itm=new QTableWidgetItem(query3.value(0).toString());
        QFont fnt;
        fnt.setBold(true);
        fnt.setPointSize(12);
        itm->setFont(fnt);
        ui->tableWidget->setItem(currentRow,0,itm);

        for(int j=1;j<=sorusayisi;j++)
        {
            if(i!=j)
            {
                double sayac0Toplam=0;
                double sayac00=0;
                double sayac01=0;
                query.exec(QString("select ogrenciid from sonuc where sorunumarasi='%1' and sinavid='%2' and yuzde>='%3' and yuzde<'%4'").arg(i).arg(sinavid).arg(sinirlarAlt.at(0).toInt()).arg(sinirlarAlt.at(1).toInt()));
                while(query.next())
                {
                    sayac0Toplam++;
                    query2.exec(QString("select yuzde from sonuc where ogrenciid='%1' and sinavid='%2' and sorunumarasi='%3'").arg(query.value(0).toString()).arg(sinavid).arg(j));
                    query2.next();
                    if(query2.value(0).toDouble()>=sinirlarAlt.at(0).toDouble() && query2.value(0).toDouble()<sinirlarAlt.at(1).toDouble())
                    {
                        sayac00++;
                    }
                    if(query2.value(0).toDouble()>=sinirlarUst.at(0).toDouble() && query2.value(0).toDouble()<sinirlarUst.at(1).toDouble())
                    {
                        sayac01++;
                    }
                }
                double yuzde00=sayac00/sayac0Toplam*100;
                double yuzde01=sayac01/sayac0Toplam*100;

                double sayac1Toplam=0;
                double sayac10=0;
                double sayac11=0;
                query.exec(QString("select ogrenciid from sonuc where sorunumarasi='%1' and sinavid='%2' and yuzde>='%3' and yuzde<'%4'").arg(i).arg(sinavid).arg(sinirlarUst.at(0).toInt()).arg(sinirlarUst.at(1).toInt()));
                while(query.next())
                {
                    sayac1Toplam++;
                    query2.exec(QString("select yuzde from sonuc where ogrenciid='%1' and sinavid='%2' and sorunumarasi='%3'").arg(query.value(0).toString()).arg(sinavid).arg(j));
                    query2.next();
                    if(query2.value(0).toDouble()>=sinirlarAlt.at(0).toDouble() && query2.value(0).toDouble()<sinirlarAlt.at(1).toDouble())
                    {
                        sayac10++;
                    }
                    if(query2.value(0).toDouble()>=sinirlarUst.at(0).toDouble() && query2.value(0).toDouble()<sinirlarUst.at(1).toDouble())
                    {
                        sayac11++;
                    }
                }
                double yuzde10=sayac10/sayac1Toplam*100;
                double yuzde11=sayac11/sayac1Toplam*100;

                query3.exec(QString("select konuisim from soru where sorunumarasi='%1' and sinavid=(select sinavid from sinav where sinavisim='%2')").arg(i).arg(ui->cbSinav->currentText()));
                query3.next();
                QString KonuBir=query3.value(0).toString();
                query4.exec(QString("select konuisim from soru where sorunumarasi='%1' and sinavid=(select sinavid from sinav where sinavisim='%2')").arg(j).arg(ui->cbSinav->currentText()));
                query4.next();
                QString KonuIki=query4.value(0).toString();

                if(ui->cbKistasYuzde->currentText()==">")
                {
                    if(yuzde00>=ui->txtYuzde->text().toInt())
                    {
                        rapor3ikiserYazdir(sayac0Toplam,sayac00,yuzde00,KonuBir,KonuIki,0);
                    }
                    if(yuzde01>=ui->txtYuzde->text().toInt())
                    {
                        rapor3ikiserYazdir(sayac0Toplam,sayac01,yuzde01,KonuBir,KonuIki,1);
                    }
                    if(yuzde10>=ui->txtYuzde->text().toInt())
                    {
                        rapor3ikiserYazdir(sayac1Toplam,sayac10,yuzde10,KonuBir,KonuIki,2);
                    }
                    if(yuzde11>=ui->txtYuzde->text().toInt())
                    {
                        rapor3ikiserYazdir(sayac1Toplam,sayac11,yuzde11,KonuBir,KonuIki,3);
                    }
                }
                else if(ui->cbKistasYuzde->currentText()=="<")
                {
                    if(yuzde00<=ui->txtYuzde->text().toInt())
                    {
                        rapor3ikiserYazdir(sayac0Toplam,sayac00,yuzde00,KonuBir,KonuIki,0);
                    }
                    if(yuzde01<=ui->txtYuzde->text().toInt())
                    {
                        rapor3ikiserYazdir(sayac0Toplam,sayac01,yuzde01,KonuBir,KonuIki,1);
                    }
                    if(yuzde10<=ui->txtYuzde->text().toInt())
                    {
                        rapor3ikiserYazdir(sayac1Toplam,sayac10,yuzde10,KonuBir,KonuIki,2);
                    }
                    if(yuzde11<=ui->txtYuzde->text().toInt())
                    {
                        rapor3ikiserYazdir(sayac1Toplam,sayac11,yuzde11,KonuBir,KonuIki,3);
                    }
                }
                sonuc2Yuzdeler->insertRows(0,1);
                sonuc2Yuzdeler->setData(sonuc2Yuzdeler->index(0,0),KonuBir);
                sonuc2Yuzdeler->setData(sonuc2Yuzdeler->index(0,1),KonuIki);
                sonuc2Yuzdeler->setData(sonuc2Yuzdeler->index(0,2),yuzde00);
                sonuc2Yuzdeler->setData(sonuc2Yuzdeler->index(0,3),yuzde11);
            }
        }
    }
    ui->tableViewCizelge->setModel(sonuc2Yuzdeler);
    ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
}

void raporla2::rapor3ikiserYazdir(int sayacIlk, int sayac, double yuzde, QString konuBir,QString konuIki, int tur)
{
    QString sonuc;
    sonuc=QString("<b>%1 -> %2 </b>").arg(konuBir).arg(konuIki);
    if(tur==0)
    {
        sonuc=sonuc+QString("<br>başarısız: %1").arg(sayacIlk);
        sonuc=sonuc+QString("<br>başarısız: %1 (%%2)").arg(sayac).arg(QString::number(yuzde,'f',2));
    }
    else if(tur==1)
    {
        sonuc=sonuc+QString("<br>başarısız: %1").arg(sayacIlk);
        sonuc=sonuc+QString("<br>başarılı: %1 (%%2)").arg(sayac).arg(QString::number(yuzde,'f',2));
    }
    else if(tur==2)
    {
        sonuc=sonuc+QString("<br>başarılı: %1").arg(sayacIlk);
        sonuc=sonuc+QString("<br>başarısız: %1 (%%2)").arg(sayac).arg(QString::number(yuzde,'f',2));
    }
    else if(tur==3)
    {
        sonuc=sonuc+QString("<br>başarılı: %1").arg(sayacIlk);
        sonuc=sonuc+QString("<br>başarılı: %1 (%%2)").arg(sayac).arg(QString::number(yuzde,'f',2));
    }
    const int currentRow = ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(currentRow + 1);
    QLabel *lbl=new QLabel();
    lbl->setText(sonuc);
    ui->tableWidget->setCellWidget(currentRow,0,lbl);
}

void raporla2::rapor3birer()
{
    QSqlQuery query,query2;
    QStringList sinirlarAlt,sinirlarUst;
    sinirlarAlt<<"0"<<ui->txtYuzdeAlt->text();
    sinirlarUst<<ui->txtYuzdeUst->text()<<"101";
    ui->label->setText("başarısız: 0 - "+ui->txtYuzdeAlt->text()+"\n"+"başarılı: "+ui->txtYuzdeUst->text()+" - 100");
    viewCizelge->setRowCount(0);
    viewCizelge->removeColumns(0,viewCizelge->columnCount());

    query.exec(QString("select sorusayisi,sinav.sinavid from sinav,derssinav where sinav.sinavid=derssinav.sinavid and sinavisim='%1' and dersid='%2'").arg(ui->cbSinav->currentText()).arg(dersid));
    query.next();
    int sorusayisi=query.value(0).toInt();
    int sinavid=query.value(1).toInt();

    //query.exec(QString("select count (distinct ogrenci.ogrenciid) from ogrenci,dersogrenci where ogrenci.ogrenciid=dersogrenci.ogrenciid and dersid='%1'").arg(dersid));
    query.exec(QString("select count (distinct ogrenci.ogrenciid) from ogrenci,dersogrenci,sinavogrenci where ogrenci.ogrenciid=dersogrenci.ogrenciid and ogrenci.ogrenciid=sinavogrenci.ogrenciid and dersid='%1' and toplampuan!='--'").arg(dersid));
    query.next();
    double ogrenciSayisi=query.value(0).toInt();

    for(int i=0;i<sorusayisi;i++)
    {
        QStringList cizelgeListe;
        //query.exec(QString("select konuisim,puan from soru where sorunumarasi='%1' and sinavid=(select sinavid from sinav where sinavisim='%2')").arg(i+1).arg(ui->cbSinav->currentText()));
        query.exec(QString("select konuisim from soru where sorunumarasi='%1' and sinavid=(select sinavid from sinav where sinavisim='%2')").arg(i+1).arg(ui->cbSinav->currentText()));
        query.next();
        QString konuIsim=query.value(0).toString();
        cizelgeListe.append(konuIsim);

        int sayac0=0;//her asamada kac ogrenci var
        double yuzde0;
        query2.exec(QString("select ogrenciid from sonuc where sorunumarasi='%1' and sinavid='%2' and yuzde>='%3' and yuzde<'%4'").arg(i+1).arg(sinavid).arg(sinirlarAlt.at(0).toInt()).arg(sinirlarAlt.at(1).toInt()));
        while(query2.next())
        {
            sayac0++;
        }
        yuzde0=sayac0/ogrenciSayisi*100;
        int sayac1=0;//her asamada kac ogrenci var
        double yuzde1;
        query2.exec(QString("select ogrenciid from sonuc where sorunumarasi='%1' and sinavid='%2' and yuzde>='%3' and yuzde<'%4'").arg(i+1).arg(sinavid).arg(sinirlarUst.at(0).toInt()).arg(sinirlarUst.at(1).toInt()));
        while(query2.next())
        {
            sayac1++;
        }
        yuzde1=sayac1/ogrenciSayisi*100;
        cizelgeListe.append("0");
        cizelgeListe.append("0");//if lere girmezse cizelge oluştururken uyg. çökmesin

        if(ui->cbKistasYuzde->currentText()==">")
        {
            if(sayac0!=0 && yuzde0>ui->txtYuzde->text().toInt())
            {
                cizelgeListe=rapor3birerYazdir(cizelgeListe,sayac0,yuzde0,konuIsim,0);
            }
            if(sayac1!=0 && yuzde1>ui->txtYuzde->text().toInt())
            {
                cizelgeListe=rapor3birerYazdir(cizelgeListe,sayac1,yuzde1,konuIsim,1);
            }
        }
        else if(ui->cbKistasYuzde->currentText()=="<")
        {
            if(sayac0!=0 && yuzde0<ui->txtYuzde->text().toInt())
            {
                cizelgeListe=rapor3birerYazdir(cizelgeListe,sayac0,yuzde0,konuIsim,0);
            }
            if(sayac1!=0 && yuzde1<ui->txtYuzde->text().toInt())
            {
                cizelgeListe=rapor3birerYazdir(cizelgeListe,sayac1,yuzde1,konuIsim,1);
            }
        }

        cizelgeViewOlustur(3,cizelgeListe);
        cizelgeOlustur();
    }
    ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
}

QStringList raporla2::rapor3birerYazdir(QStringList cizelgeListe,int sayac,double yuzde,QString konuIsim,int tur)
{
    //tur=0 başarısız
    //tur=1 başarılı
    QString sonuc;
    sonuc=QString("<b>%1</b>").arg(konuIsim);
    if(tur==0)
    {
        sonuc+=QString("<br>başarısız : %1 (%%2)").arg(sayac).arg(QString::number(yuzde,'f',2));
        cizelgeListe.replace(1,QString::number(yuzde));
    }
    else if(tur==1)
    {
        sonuc+=QString("<br>başarılı : %1 (%%2)").arg(sayac).arg(QString::number(yuzde,'f',2));
        cizelgeListe.replace(2,QString::number(yuzde));
    }

    QLabel *lbl=new QLabel();
    lbl->setText(sonuc);

    const int currentRow = ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(currentRow + 1);
    ui->tableWidget->setCellWidget(currentRow,0,lbl);

    return cizelgeListe;
}

void raporla2::rapor2ikiser()
{
    QSqlQuery query,query2;
    QStringList sinirlar;//basari sinirlari(4 asama var)
    sinirlar<<"0"<<"26"<<"51"<<"76"<<"101";

    query.exec(QString("select sorusayisi,sinav.sinavid from sinav,derssinav where sinav.sinavid=derssinav.sinavid and sinavisim='%1' and dersid='%2'").arg(ui->cbSinav->currentText()).arg(dersid));
    query.next();
    int sorusayisi=query.value(0).toInt();
    int sinavid=query.value(1).toInt();

    for(int i=1;i<=sorusayisi;i++)
    {
        for(int j=1;j<=sorusayisi;j++)
        {
            if(i!=j)
            {
                const int currentRow = ui->tableWidget->rowCount();
                ui->tableWidget->setRowCount(currentRow + 1);
                for(int k=0;k<4;k++)
                {
                    QString sonuc;
                    query.exec(QString("select konuisim,puan from soru where sorunumarasi='%1' and sinavid=(select sinavid from sinav where sinavisim='%2')").arg(i).arg(ui->cbSinav->currentText()));
                    query.next();
                    query2.exec(QString("select konuisim,puan from soru where sorunumarasi='%1' and sinavid=(select sinavid from sinav where sinavisim='%2')").arg(j).arg(ui->cbSinav->currentText()));
                    query2.next();
                    sonuc=QString("<b>%1 -> %2</b>").arg(query.value(0).toString()).arg(query2.value(0).toString());

                    int liste[3][3];
                    for(int y=0;y<5;y++)
                    {
                        for(int z=0;z<5;z++)
                        {
                            liste[y][z]=0;
                        }
                    }
                    int sayac=0;
                    query.exec(QString("select ogrenciid from sonuc where sorunumarasi='%1' and sinavid='%2' and yuzde>='%3' and yuzde<'%4'").arg(i).arg(sinavid).arg(sinirlar.at(k).toInt()).arg(sinirlar.at(k+1).toInt()));
                    while(query.next())
                    {
                        sayac++;
                        query2.exec(QString("select yuzde from sonuc where ogrenciid='%1' and sinavid='%2' and sorunumarasi='%3'").arg(query.value(0).toString()).arg(sinavid).arg(j));
                        query2.next();

                        for(int l=0;l<4;l++)
                        {
                            if(query2.value(0).toDouble()>=sinirlar.at(l).toDouble() && query2.value(0).toDouble()<sinirlar.at(l+1).toDouble())
                            {
                                int varolan=liste[k][l];
                                varolan++;
                                liste[k][l]=varolan;
                            }
                        }
                    }
                    sonuc=sonuc+QString("<br>(%1 - %2) : %3").arg(sinirlar.at(k)).arg(sinirlar.at(k+1)).arg(sayac);
                    sonuc=sonuc+"<br>------------";

                    for(int h=0;h<4;h++)
                    {
                        double yuzde=double(liste[k][h])/double(sayac)*100;
                        sonuc=sonuc+QString("<br>(%1 - %2) : %3 (%%4)").arg(sinirlar.at(h)).arg(sinirlar.at(h+1)).arg(liste[k][h]).arg(QString::number(yuzde,'f',2));
                    }
                    QLabel *lbl=new QLabel();
                    lbl->setText(sonuc);
                    ui->tableWidget->setCellWidget(currentRow,k,lbl);
                }
            }
        }
    }
    ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
}

void raporla2::rapor2birer()
{
    QSqlQuery query;
    QStringList sinirlar;//basari sinirlari(4 asama var)
    sinirlar<<"0"<<"26"<<"51"<<"76"<<"101";
    viewCizelge->setRowCount(0);
    viewCizelge->removeColumns(0,viewCizelge->columnCount());

    query.exec(QString("select sorusayisi,sinav.sinavid from sinav,derssinav where sinav.sinavid=derssinav.sinavid and sinavisim='%1' and dersid='%2'").arg(ui->cbSinav->currentText()).arg(dersid));
    query.next();
    int sorusayisi=query.value(0).toInt();
    int sinavid=query.value(1).toInt();

    if(sorusayisi>3)
    {
        ui->tableWidget->setColumnCount(4);
    }
    else
    {
        ui->tableWidget->setColumnCount(sorusayisi);
    }

    query.exec(QString("select count (distinct ogrenci.ogrenciid) from ogrenci,dersogrenci where ogrenci.ogrenciid=dersogrenci.ogrenciid and dersid='%1'").arg(dersid));
    query.next();
    double ogrenciSayisi=query.value(0).toInt();

    for(int i=0;i<sorusayisi;i++)
    {
        QString sonuc;
        QStringList cizelgeListe;//her dongude yuzdeyi içine atacak. sonra cizelgeyi oluşturan fonksiyona gidecek
        query.exec(QString("select konuisim from soru where sorunumarasi='%1' and sinavid=(select sinavid from sinav where sinavisim='%2')").arg(i+1).arg(ui->cbSinav->currentText()));
        query.next();
        sonuc=QString("<b>%1</b>").arg(query.value(0).toString());
        cizelgeListe.append(query.value(0).toString());

        for(int k=0;k<4;k++)
        {
            int sayac=0;//her asamada kac ogrenci var
            double yuzde;
            query.exec(QString("select count (ogrenciid) from sonuc where sorunumarasi='%1' and sinavid='%2' and yuzde>='%3' and yuzde<'%4'").arg(i+1).arg(sinavid).arg(sinirlar.at(k).toInt()).arg(sinirlar.at(k+1).toInt()));
            query.next();
            sayac=query.value(0).toInt();
            yuzde=sayac/ogrenciSayisi*100;
            cizelgeListe.append(QString::number(yuzde));
            sonuc=sonuc+QString("<br>(%1 - %2) : %3 (%%4)").arg(sinirlar.at(k)).arg(sinirlar.at(k+1)).arg(sayac).arg(QString::number(yuzde,'f',2));
        }
        cizelgeViewOlustur(1,cizelgeListe);
        cizelgeOlustur();

        QLabel *lbl=new QLabel();
        lbl->setText(sonuc);

        const int currentRow = ui->tableWidget->rowCount();
        if(i%4==0)
        {
            ui->tableWidget->setRowCount(currentRow + 1);
            ui->tableWidget->setCellWidget(currentRow,0,lbl);
        }
        else
        {
            ui->tableWidget->setCellWidget(currentRow-1,i%4,lbl);
        }
    }

    //for(int i=0;i<ui->tableWidget->horizontalHeader()->count();i++)
    //{
    //    ui->tableWidget->horizontalHeader()->setResizeMode(i,QHeaderView::ResizeToContents);
    //}
    //ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    //for(int i=0;i<ui->tableWidget->verticalHeader()->count();i++)
    //{
    //    ui->tableWidget->verticalHeader()->setResizeMode(i,QHeaderView::ResizeToContents);
    //}
    //ui->tableWidget->verticalHeader()->setStretchLastSection(true);

    ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
}

void raporla2::cizelgeYukleme()
{
    ui->cizelge->axisY()->setRanges(0,100);
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

void raporla2::raporIkiOncesi(QString dersIsim)
{
    this->setWindowTitle("rapor 2-3 ("+dersIsim+")");
    ui->cbSinav->clear();
    ui->label->clear();
    ui->tableWidget->setRowCount(0);
    ui->lbl1->hide();
    ui->lbl2->hide();
    ui->lbl3->hide();
    ui->lbl4->hide();
    ui->lbl1p->hide();
    ui->lbl2p->hide();
    ui->lbl3p->hide();
    ui->lbl4p->hide();
    viewCizelge->clear();
    ui->tabWidget->setCurrentIndex(0);
    ui->cbKistas->setCurrentIndex(0);

    QSqlQuery query;
    query.exec(QString("select sinavisim,dersid from sinav,derssinav where sinav.sinavid=derssinav.sinavid and dersid=(select dersid from ders where dersisim='%1')").arg(dersIsim));
    while(query.next())
    {
        dersid=query.value(1).toString();
        ui->cbSinav->addItem(query.value(0).toString());
    }
}

void raporla2::yukleme()
{
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->horizontalHeader()->hide();
    ui->tableWidget->verticalHeader()->hide();
    ui->txtYuzdeAlt->setText("0");
    ui->txtYuzdeUst->setText("101");
    ui->txtYuzde->setText("0");
    ui->cbKistas->addItem("seçim yapın");
    ui->cbKistas->addItem("rapor 2 (birer)");
    ui->cbKistas->addItem("rapor 2 (ikişer)");
    ui->cbKistas->addItem("rapor 3 (birer)");
    ui->cbKistas->addItem("rapor 3 (ikişer)");
    ui->tableGrafikKonular->setRowCount(0);
    ui->tableGrafikKonular->setColumnCount(1);
    ui->tableGrafikKonular->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableGrafikKonular->horizontalHeader()->hide();

    cizelgeYukleme();
    viewCizelge=new QStandardItemModel(this);
    sonuc2Yuzdeler=new QStandardItemModel(this);

    sonuc2Yuzdeler->setRowCount(0);
    sonuc2Yuzdeler->setColumnCount(4);
    ui->tableViewCizelge->hide();
}

raporla2::~raporla2()
{
    delete ui;
}
