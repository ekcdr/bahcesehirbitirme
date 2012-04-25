#include "proje.h"
#include "ui_proje.h"
#include "ui_ekleogrenci.h"
#include "ui_ekleders.h"
#include "ui_eklesinav.h"
#include "ui_raporla.h"
#include "ui_veritabanigoster.h"
#include "ui_verigirisi.h"
#include "ui_eklesonuc.h"
#include "ui_listele.h"
#include "ui_krapor.h"
#include "ui_sinavistatistik.h"

proje::proje(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::proje)
{
    ui->setupUi(this);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));
    connect(ui->btnVeritabani,SIGNAL(clicked()),this,SLOT(veritabani()));
    connect(ui->btnDersEkle,SIGNAL(clicked()),this,SLOT(fEkleDers()));
    connect(ui->cbDonem,SIGNAL(currentIndexChanged(QString)),this,SLOT(dersleriYazdir()));
    connect(ui->cbYil,SIGNAL(currentIndexChanged(QString)),this,SLOT(dersleriYazdir()));
    connect(ui->tableDersler,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(veriGirisiYap(int,int)));
    connect(formEkleOgrenci.ui->btnTamam,SIGNAL(clicked()),this,SLOT(tamamOgrenci()));
    connect(formEkleDers.ui->btnTamam,SIGNAL(clicked()),this,SLOT(tamamDers()));
    connect(formEkleSinav.ui->btnTamam,SIGNAL(clicked()),this,SLOT(tamamSinav()));
    connect(formEkleSonuc.ui->btnTamam,SIGNAL(clicked()),this,SLOT(tamamSonuc2()));
    connect(formEkleSonuc.ui->btnIptal,SIGNAL(clicked()),this,SLOT(sonucKapat()));
    connect(formEkleSonuc.ui->cbSinav,SIGNAL(currentIndexChanged(QString)),this,SLOT(sonucDoldur()));
    connect(formRaporla.ui->btnRaporla,SIGNAL(clicked()),this,SLOT(raporOlustur2()));
    connect(formRaporla.ui->cbSinav,SIGNAL(currentIndexChanged(QString)),this,SLOT(filtreSinav()));
    connect(formRaporla.ui->btnGrafik,SIGNAL(clicked()),this,SLOT(sinavGrafikGoster()));
    connect(formVeriGirisi.ui->btnOgrenciEkle,SIGNAL(clicked()),this,SLOT(fEkleOgrenci()));
    connect(formVeriGirisi.ui->btnSinavEkle,SIGNAL(clicked()),this,SLOT(fEkleSinav()));
    connect(formVeriGirisi.ui->btnSonuclariGir,SIGNAL(clicked()),this,SLOT(fEkleSonuc()));
    connect(formVeriGirisi.ui->btnOgrenciListe,SIGNAL(clicked()),this,SLOT(ogrenciListele()));
    connect(formVeriGirisi.ui->btnSinavListe,SIGNAL(clicked()),this,SLOT(sinavListele()));
    connect(formVeriGirisi.ui->btnRapor,SIGNAL(clicked()),this,SLOT(raporDers()));
    connect(formVeriGirisi.ui->btnKRapor,SIGNAL(clicked()),this,SLOT(krapor()));
    yukleme();
}

void proje::closeEvent(QCloseEvent *event)
{
    formSinavIstatistik.kapat();
    formEkleOgrenci.kapat();
    formEkleDers.kapat();
    formEkleSinav.kapat();
    formEkleSonuc.kapat();
    formRaporla.kapat();
    formVeritabani.kapat();
    formListele.kapat();
    formKRapor.kapat();
    formVeriGirisi.kapat();
    event->accept();
}

void proje::sinavGrafikGoster()
{
    /*
    formSinavIstatistik.viewCizelge->setColumnCount(0);
    QSqlQuery query;
    query.exec(QString("select sinavpuan from sinav where sinavid=(SELECT sinav.sinavid FROM sinav,derssinav WHERE sinav.sinavid=derssinav.sinavid AND sinavisim='%1' AND dersid=(select dersid from ders where dersisim='%2'))").arg(formRaporla.ui->cbSinav->currentText()).arg(ui->tableDersler->currentItem()->text()));
    query.next();
    QString dersisim=ui->tableDersler->currentItem()->text();
    formSinavIstatistik.setWindowTitle(formRaporla.ui->cbSinav->currentText()+" sınavı grafiği");
    formSinavIstatistik.cizelgeYukleme(query.value(0).toInt());
    //formSinavIstatistik.cizelgeOlustur(dersisim,formRaporla.ui->txtSinavPuan->text().toInt(),formRaporla.ui->cbKistas->currentText());
    */
    formSinavIstatistik.show();
}

void proje::krapor()
{
    formKRapor.yukleme2();
    QSqlQuery query;
    query.exec(QString("select sinavisim,dersid from sinav,derssinav where sinav.sinavid=derssinav.sinavid and dersid=(select dersid from ders where dersisim='%1')").arg(ui->tableDersler->currentItem()->text()));
    while(query.next())
    {
        formKRapor.dersid=query.value(1).toString();
        formKRapor.ui->cbSinav->addItem(query.value(0).toString());
    }
    formKRapor.show();
}

void proje::kalinFont(QTableWidgetItem *itm)
{
    QFont font;
    font.setBold(true);
    itm->setFont(font);
}

void proje::ogrenciListele()
{
    formListele.setWindowTitle(ui->tableDersler->item(ui->tableDersler->currentRow(),0)->text()+" dersi için öğrenci listesi");
    formListele.yuklemeOgrenci();

    QSqlQuery query,query2;
    QString dersisim=ui->tableDersler->currentItem()->text();

    //ogrenci listesine sınavları ekliyor
    query.exec(QString("select sinavisim from derssinav,sinav where derssinav.sinavid=sinav.sinavid and dersid=(select dersid from ders where dersisim='%1')").arg(dersisim));
    while(query.next())
    {
        const int currentColumn = formListele.ui->tableListe->columnCount();
        formListele.ui->tableListe->setColumnCount(currentColumn + 1);
        QTableWidgetItem *itm=new QTableWidgetItem(query.value(0).toString());
        formListele.ui->tableListe->setHorizontalHeaderItem(currentColumn,itm);
        itm->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    }

    int i=0;
    query.exec(QString("select ogrenci.ogrenciid,isim from ogrenci,dersogrenci where ogrenci.ogrenciid=dersogrenci.ogrenciid and dersid=(select dersid from ders where dersisim='%1')").arg(dersisim));
    while(query.next())
    {
        int j=2;
        const int currentRow = formListele.ui->tableListe->rowCount();
        formListele.ui->tableListe->setRowCount(currentRow + 1);
        QTableWidgetItem *itm0=new QTableWidgetItem(query.value(0).toString());
        formListele.ui->tableListe->setItem(currentRow,0,itm0);
        QTableWidgetItem *itm1=new QTableWidgetItem(query.value(1).toString());
        formListele.ui->tableListe->setItem(currentRow,1,itm1);

        query2.exec(QString("select toplampuan from sinavogrenci where ogrenciid='%1'").arg(formListele.ui->tableListe->item(i,0)->text()));
        while(query2.next())
        {
            QTableWidgetItem *itm=new QTableWidgetItem(query2.value(0).toString());
            formListele.ui->tableListe->setItem(i,j,itm);
            itm->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            j++;
        }
        itm0->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        itm1->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        i++;
    }
    //formListele.exec();
    formListele.show();
}

void proje::sinavListele()
{
    formListele.setWindowTitle(ui->tableDersler->item(ui->tableDersler->currentRow(),0)->text()+" dersi için sınav listesi");
    formListele.yuklemeSinav();

    //YANLIŞ  OLABİLİR query
    QSqlQuery query,query2;
    query.exec(QString("select sinavisim,sorusayisi,sinavpuan,sinav.sinavid from sinav,derssinav where derssinav.sinavid=sinav.sinavid and dersid=(select dersid from ders where dersisim='%1')").arg(ui->tableDersler->currentItem()->text()));
    while(query.next())
    {
        const int currentRow = formListele.ui->tableListe->rowCount();
        formListele.ui->tableListe->setRowCount(currentRow + 1);
        QTableWidgetItem *itm0=new QTableWidgetItem(query.value(0).toString());
        formListele.ui->tableListe->setItem(currentRow,0,itm0);
        QTableWidgetItem *itm1=new QTableWidgetItem(query.value(1).toString());
        formListele.ui->tableListe->setItem(currentRow,1,itm1);
        QTableWidgetItem *itm2=new QTableWidgetItem(query.value(2).toString());
        formListele.ui->tableListe->setItem(currentRow,2,itm2);
        query2.exec(QString("select avg (toplampuan) from sinavogrenci where sinavid='%1'").arg(query.value(3).toString()));
        query2.next();
        QTableWidgetItem *itm3=new QTableWidgetItem(query2.value(0).toString());
        formListele.ui->tableListe->setItem(currentRow,3,itm3);
        itm0->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        itm1->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        itm2->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        itm3->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    }
    //formListele.exec();
    formListele.show();
}

void proje::sonucKapat()
{
    if(formEkleSonuc.ekleSonucDegisiklikVar==true)
    {
        if(formEkleSonuc.ekleSonucDegisiklikVar==true)
        {
            QMessageBox msgBox;
            msgBox.setText("Değişiklikler kaydedilsin mi?");
            msgBox.setIcon(QMessageBox::Warning);
            QPushButton *tamam = msgBox.addButton("Tamam", QMessageBox::ActionRole);
            QPushButton *iptal = msgBox.addButton("İptal", QMessageBox::ActionRole);
            msgBox.exec();
            if (msgBox.clickedButton() == tamam)
            {
                tamamSonuc2();
            }
            else if (msgBox.clickedButton() == iptal)
            {
                formEkleSonuc.ekleSonucDegisiklikVar=false;
                formEkleSonuc.degisenIDOgrenci.clear();
                msgBox.close();
            }
        }
    }
    formEkleSonuc.close();
}

void proje::tamamSonuc2()
{
    QSqlQuery query,query2;
    if(formEkleSonuc.ekleSonucDegisiklikVar==true)
    {
        for(int h=0;h<formEkleSonuc.degisenIDOgrenci.count();h++)
        {
            int ayrac=formEkleSonuc.degisenIDOgrenci.at(h).indexOf("|");
            QString s=formEkleSonuc.degisenIDOgrenci.at(h);
            QString ogrenciid=s.remove(ayrac,formEkleSonuc.degisenIDOgrenci.at(h).length());
            s=formEkleSonuc.degisenIDOgrenci.at(h);
            QString sinavid=s.remove(0,ogrenciid.length()+1);

            query.exec(QString("select sorusayisi from sinav where sinavid='%1'").arg(sinavid));
            query.next();
            int sorusayisi=query.value(0).toInt();

            int toplamPuan=0;
            for(int i=0;i<sorusayisi;i++)
            {
                QList<QTableWidgetItem *> liste=formEkleSonuc.ui->tableSonuclar->findItems(ogrenciid,Qt::MatchExactly);
                for(int k=0;k<liste.count();k++)
                {
                    if(liste.at(k)->column()==0)
                    {
                        query2.exec(QString("select puan from soru where sinavid='%1' and sorunumarasi='%2'").arg(sinavid).arg(i+1));
                        query2.next();
                        double yuzde=formEkleSonuc.ui->tableSonuclar->item(liste.at(k)->row(),i+1)->text().toDouble()/query2.value(0).toDouble()*100;
                        if(formEkleSonuc.ui->tableSonuclar->item(liste.at(k)->row(),i+1)->text()=="--")
                        {
                            query.exec(QString("update sonuc set alinanpuan='%1', yuzde='%2' where sinavid='%3' and ogrenciid='%4' and sorunumarasi='%5'").arg(0).arg(yuzde).arg(sinavid).arg(ogrenciid).arg(i+1));
                        }
                        else
                        {
                            query.exec(QString("update sonuc set alinanpuan='%1', yuzde='%2' where sinavid='%3' and ogrenciid='%4' and sorunumarasi='%5'").arg(formEkleSonuc.ui->tableSonuclar->item(liste.at(k)->row(),i+1)->text()).arg(yuzde).arg(sinavid).arg(ogrenciid).arg(i+1));
                        }
                        toplamPuan=toplamPuan+formEkleSonuc.ui->tableSonuclar->item(liste.at(k)->row(),i+1)->text().toInt();
                    }
                }
            }
            query.exec(QString("update sinavogrenci set toplampuan='%1' where sinavid='%2' and ogrenciid='%3'").arg(toplamPuan).arg(sinavid).arg(ogrenciid));
        }
        formEkleSonuc.uyari(1);
        formEkleSonuc.ekleSonucDegisiklikVar=false;
        formEkleSonuc.degisenIDOgrenci.clear();
    }
    else
    {
        formEkleSonuc.uyari(0);
    }
    veritabaniGuncelle();
}

void proje::sonucDoldur()
{
    if(formEkleSonuc.ekleSonucDegisiklikVar==true)
    {
        QMessageBox msgBox;
        msgBox.setText("Değişiklikler kaydedilsin mi?");
        msgBox.setIcon(QMessageBox::Warning);
        QPushButton *tamam = msgBox.addButton("Tamam", QMessageBox::ActionRole);
        QPushButton *iptal = msgBox.addButton("İptal", QMessageBox::ActionRole);
        msgBox.exec();
        if (msgBox.clickedButton() == tamam)
        {
            tamamSonuc2();
        }
        else if (msgBox.clickedButton() == iptal)
        {
            formEkleSonuc.degisenIDOgrenci.clear();
            formEkleSonuc.ekleSonucDegisiklikVar=false;
            msgBox.close();
        }
    }

    if(formEkleSonuc.ui->cbSinav->currentIndex()!=-1)//cbSinavda olmayan bir seçenek olursa fonksiyona girmesin
    {
        QString sinavisim=formEkleSonuc.ui->cbSinav->currentText();
        QString dersisim=ui->tableDersler->currentItem()->text();

        formEkleSonuc.ilkAcilis=true;
        formEkleSonuc.ui->tableSonuclar->setRowCount(0);
        formEkleSonuc.ui->tableSonuclar->setColumnCount(0);

        int sira=0;
        int toplamPuan=0;
        QSqlQuery query,query2;

        query.exec(QString("select sorusayisi,sinav.sinavid from sinav,derssinav where sinav.sinavid=derssinav.sinavid and dersid=(select dersid from ders where dersisim='%1') and sinavisim='%2'").arg(dersisim).arg(sinavisim));
        query.next();
        formEkleSonuc.sinavID=query.value(1).toString();

        const int currentRow = formEkleSonuc.ui->tableSonuclar->rowCount();
        formEkleSonuc.ui->tableSonuclar->setRowCount(currentRow + 1);

        for(int i=0;i<=query.value(0).toInt();i++)//sonuc ekranındaki sutun başlıkları
        {
            const int currentColumn = formEkleSonuc.ui->tableSonuclar->columnCount();
            formEkleSonuc.ui->tableSonuclar->setColumnCount(currentColumn + 1);

            query2.exec(QString("select puan from soru where sorunumarasi='%1' and sinavid=(select sinav.sinavid from sinav,derssinav where sinav.sinavid=derssinav.sinavid and dersid=(select dersid from ders where dersisim='%2') and sinavisim='%3')").arg(i).arg(dersisim).arg(sinavisim));
            query2.next();

            QTableWidgetItem *itm=new QTableWidgetItem(QString::number(i)+" ("+query2.value(0).toString()+")");
            formEkleSonuc.ui->tableSonuclar->setItem(0,i,itm);
            itm->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            kalinFont(itm);

            toplamPuan=toplamPuan+query2.value(0).toInt();
        }

        const int currentColumn = formEkleSonuc.ui->tableSonuclar->columnCount();
        formEkleSonuc.ui->tableSonuclar->setColumnCount(currentColumn + 1);
        QTableWidgetItem *itmt=new QTableWidgetItem("Toplam ("+QString::number(toplamPuan)+")");
        formEkleSonuc.ui->tableSonuclar->setItem(0,currentColumn,itmt);
        itmt->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        kalinFont(itmt);

        query.exec(QString("select ogrenci.ogrenciid from dersogrenci,ogrenci where dersogrenci.ogrenciid=ogrenci.ogrenciid and dersid=(select dersid from ders where dersisim='%1')").arg(dersisim));
        while(query.next())
        {
            sira=sira+1;
            const int currentRow = formEkleSonuc.ui->tableSonuclar->rowCount();
            formEkleSonuc.ui->tableSonuclar->setRowCount(currentRow + 1);
            QTableWidgetItem *itm=new QTableWidgetItem(query.value(0).toString());
            formEkleSonuc.ui->tableSonuclar->setItem(sira,0,itm);
            itm->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            kalinFont(itm);
        }

        for(int j=1;j<formEkleSonuc.ui->tableSonuclar->rowCount();j++)
        {
            for(int i=1;i<formEkleSonuc.ui->tableSonuclar->columnCount();i++)
            {
                if(i!=formEkleSonuc.ui->tableSonuclar->columnCount()-1)//toplam sutununda birşey yazmasın
                {
                    query.exec(QString("select alinanpuan from sonuc where ogrenciid='%1' and sorunumarasi='%2' and sinavid=(select sinav.sinavid from sinav,derssinav where sinav.sinavid=derssinav.sinavid and dersid=(select dersid from ders where dersisim='%3') and sinavisim='%4')").arg(formEkleSonuc.ui->tableSonuclar->item(j,0)->text()).arg(i).arg(dersisim).arg(sinavisim));
                    query.next();

                    QTableWidgetItem *itm=new QTableWidgetItem(query.value(0).toString());
                    formEkleSonuc.ui->tableSonuclar->setItem(j,i,itm);
                }
                else//toplamı yazdırıyor
                {
                    query.exec(QString("select toplampuan from sinavogrenci where ogrenciid='%1' and sinavid=(select sinav.sinavid from sinav,derssinav where sinav.sinavid=derssinav.sinavid and dersid=(select dersid from ders where dersisim='%2') and sinavisim='%3')").arg(formEkleSonuc.ui->tableSonuclar->item(j,0)->text()).arg(dersisim).arg(sinavisim));
                    query.next();
                    QTableWidgetItem *itm=new QTableWidgetItem(query.value(0).toString());
                    formEkleSonuc.ui->tableSonuclar->setItem(j,formEkleSonuc.ui->tableSonuclar->columnCount()-1,itm);

                    itm->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                    kalinFont(itm);
                }
            }
        }

        QTableWidgetItem *itm=new QTableWidgetItem("Öğrenci\\Soru");
        formEkleSonuc.ui->tableSonuclar->setItem(0,0,itm);
        itm->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        kalinFont(itm);

        formEkleSonuc.ilkAcilis=false; // toplamı yeniden hesaplayan fonk her acilista tekrarlanmasın diye
    }
}

void proje::fEkleSonuc()
{
    formEkleSonuc.setWindowTitle(ui->tableDersler->item(ui->tableDersler->currentRow(),0)->text()+" dersi için sonuçları gir");
    formEkleSonuc.ui->cbSinav->clear();
    QSqlQuery query;
    query.exec(QString("select sinavisim from sinav,derssinav where sinav.sinavid=derssinav.sinavid and dersid=(select dersid from ders where dersisim='%1')").arg(ui->tableDersler->currentItem()->text()));
    while(query.next())
    {
        formEkleSonuc.ui->cbSinav->addItem(query.value(0).toString());
    }
    formEkleSonuc.degisenIDOgrenci.clear();
    formEkleSonuc.ekleSonucDegisiklikVar=false;
    //formEkleSonuc.exec();
    formEkleSonuc.show();
}

void proje::veriGirisiYap(int i,int j)
{ 
    if(j==0)
    {
        formVeriGirisi.setWindowTitle(ui->tableDersler->item(i,j)->text()+" dersi için veri girişi");
        //formVeriGirisi.exec();
        formVeriGirisi.show();
    }
    if(j==1)
    {
        fEkleOgrenci();
    }
    if(j==2)
    {
        fEkleSinav();
    }
}

void proje::dersleriYazdir()
{
    QSqlQuery query;
    ui->tableDersler->setRowCount(0);

    if(ui->cbDonem->currentText()=="Tüm Dönemler")
    {
        if(ui->cbYil->currentText()=="Tüm Yıllar")
        {
            query.exec("SELECT dersisim FROM ders ");
            while(query.next())
            {
                const int currentRow = ui->tableDersler->rowCount();
                ui->tableDersler->setRowCount(currentRow + 1);
                QTableWidgetItem *itm0=new QTableWidgetItem(query.value(0).toString());
                ui->tableDersler->setItem(currentRow,0,itm0);

                itm0->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                ogrenciSayisiBul(query.value(0).toString());
                sinavSayisiBul(query.value(0).toString());
            }
        }
        else
        {
            query.exec(QString("SELECT dersisim FROM ders WHERE dersyil='%1'").arg(ui->cbYil->currentText()));
            while(query.next())
            {
                const int currentRow = ui->tableDersler->rowCount();
                ui->tableDersler->setRowCount(currentRow + 1);
                QTableWidgetItem *itm0=new QTableWidgetItem(query.value(0).toString());
                ui->tableDersler->setItem(currentRow,0,itm0);

                itm0->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                ogrenciSayisiBul(query.value(0).toString());
                sinavSayisiBul(query.value(0).toString());
            }
        }
    }
    else
    {
        if(ui->cbYil->currentText()=="Tüm Yıllar")
        {
            query.exec(QString("SELECT dersisim FROM ders WHERE dersdonem='%1'").arg(ui->cbDonem->currentText()));
            while(query.next())
            {
                const int currentRow = ui->tableDersler->rowCount();
                ui->tableDersler->setRowCount(currentRow + 1);
                QTableWidgetItem *itm0=new QTableWidgetItem(query.value(0).toString());
                ui->tableDersler->setItem(currentRow,0,itm0);

                itm0->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                ogrenciSayisiBul(query.value(0).toString());
                sinavSayisiBul(query.value(0).toString());
            }
        }
        else
        {
            query.exec(QString("SELECT dersisim FROM ders WHERE dersdonem='%1' AND dersyil='%2'").arg(ui->cbDonem->currentText()).arg(ui->cbYil->currentText()));
            while(query.next())
            {
                const int currentRow = ui->tableDersler->rowCount();
                ui->tableDersler->setRowCount(currentRow + 1);
                QTableWidgetItem *itm0=new QTableWidgetItem(query.value(0).toString());
                ui->tableDersler->setItem(currentRow,0,itm0);

                itm0->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                ogrenciSayisiBul(query.value(0).toString());
                sinavSayisiBul(query.value(0).toString());
            }
        }
    }
}

void proje::veritabaniGuncelle()
{
    modelOgrenci->select();
    modelKonu->select();
    modelSinav->select();
    modelDers->select();
    modelDersSinav->select();
    modelDersOgrenci->select();
    modelSoru->select();
    modelSonuc->select();
    modelSinavOgrenci->select();
    modelSonucSinav->select();
}

void proje::veritabani()
{
    formVeritabani.ui->tableView->setModel(modelSoru);
    formVeritabani.ui->tableView_2->setModel(modelDers);
    formVeritabani.ui->tableView_3->setModel(modelDersOgrenci);
    formVeritabani.ui->tableView_4->setModel(modelSinavOgrenci);
    formVeritabani.ui->tableView_5->setModel(modelSinav);
    formVeritabani.ui->tableView_6->setModel(modelSonuc);
    formVeritabani.ui->tableView_7->setModel(modelDersSinav);
    formVeritabani.ui->tableView_8->setModel(modelOgrenci);
    formVeritabani.ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    formVeritabani.ui->tableView_2->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    formVeritabani.ui->tableView_3->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    formVeritabani.ui->tableView_4->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    formVeritabani.ui->tableView_5->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    formVeritabani.ui->tableView_6->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    formVeritabani.ui->tableView_7->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    formVeritabani.ui->tableView_8->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    formVeritabani.show();
}

void proje::raporOlustur2()
{
    QSqlQuery query,query2;
    QString dersisim=ui->tableDersler->currentItem()->text();
    QString sinavisim=formRaporla.ui->cbSinav->currentText();
    QStringList liste,liste2,baslik;//liste sınavdan kıstası geçemler, liste2 sorulardan kıstası geçenler
    QString bilgi;
    formSinavIstatistik.viewCizelge->setColumnCount(0);
    formSinavIstatistik.viewCizelge->setRowCount(0);
    formRaporla.ui->tableWidget->setRowCount(0);
    baslik<<"Öğrenci"<<"Sınav Puanı";

    query.exec(QString("select count (distinct ogrenci.ogrenciid) from ogrenci,dersogrenci,sinavogrenci where ogrenci.ogrenciid=dersogrenci.ogrenciid and ogrenci.ogrenciid=sinavogrenci.ogrenciid and dersid=(select dersid from ders where dersisim='%1') and toplampuan!='--'").arg(dersisim));
    query.next();

    double ogrenciSayisi=query.value(0).toInt();
    bilgi=QString("öğrenci: %1").arg(ogrenciSayisi);

    query.exec(QString("select sinavid from sinav where sinavisim='%1'").arg(sinavisim));
    query.next();
    QString sinavid=query.value(0).toString();

    query.exec(QString("select dersid from ders where dersisim='%1'").arg(dersisim));
    query.next();
    QString dersid=query.value(0).toString();

    double sayac=0;
    double yuzde=0;

    query.exec(QString("select ogrenciid from sinavogrenci,derssinav where sinavogrenci.sinavid=derssinav.sinavid and sinavogrenci.sinavid='%1' and dersid='%2' and toplampuan %3 '%4' and toplampuan!='--'").arg(sinavid).arg(dersid).arg(formRaporla.ui->cbKistas->currentText()).arg(formRaporla.ui->txtSinavPuan->text()));
    while(query.next())
    {
        liste.append(query.value(0).toString());
        sayac++;
    }
    yuzde=(sayac/ogrenciSayisi)*100;
    bilgi=bilgi+QString("\nsınav: %1 (%%2)").arg(sayac).arg(QString::number(yuzde,'f',2));

    query.exec(QString("select sorusayisi from sinav,derssinav where sinav.sinavid=derssinav.sinavid and sinav.sinavid and sinav.sinavid='%1' and dersid='%2'").arg(sinavid).arg(dersid));
    query.next();
    int sorusayisi=query.value(0).toInt();
    formRaporla.ui->tableWidget->setColumnCount(sorusayisi+2);

    QStringList etkinSorular;//hangi sorular etkin, kıstasa alınacak
    for(int i=0;i<sorusayisi;i++)
    {
        QCheckBox *cb=qobject_cast<QCheckBox *>(formRaporla.ui->tableWidgetKonular->cellWidget(i,0));
        if(cb->isChecked()==true)
        {
            etkinSorular.append(QString::number(i+1));
        }
        baslik.append(QString::number(i+1)+". soru");
    }
    formRaporla.ui->tableWidget->setHorizontalHeaderLabels(baslik);

    if(!etkinSorular.isEmpty()) //sınav sonucu + kıstastaki sorular
    {
        formSinavIstatistik.viewCizelge->setVerticalHeaderLabels(QStringList()<<"toplam"<<etkinSorular);
        formSinavIstatistik.ui->tableViewCizelge->setModel(formSinavIstatistik.viewCizelge);
        formSinavIstatistik.viewCizelge->setHeaderData(0, Qt::Vertical, Qt::red, Qt::BackgroundRole);

        for(int i=0;i<etkinSorular.count();i++)
        {
            formSinavIstatistik.viewCizelge->setHeaderData(i+1, Qt::Vertical, Qt::green, Qt::BackgroundRole);
            sayac=0;
            QComboBox *com=qobject_cast<QComboBox *>(formRaporla.ui->tableWidgetKonular->cellWidget(i,4));
            query.exec(QString("select ogrenciid from sonuc,derssinav where sonuc.sinavid=derssinav.sinavid and sorunumarasi='%1' and alinanpuan %2 '%3' and alinanpuan!='--' and sonuc.sinavid='%4' and dersid='%5'").arg(etkinSorular.at(i)).arg(com->currentText()).arg(formRaporla.ui->tableWidgetKonular->item(etkinSorular.at(i).toInt()-1,5)->text().toInt()).arg(sinavid).arg(dersid));
            while(query.next())
            {
                liste.append(query.value(0).toString());
                sayac++;
            }
            if(sayac==0)//hiç kimse kıstasa giremez ise alttaki else in içindeki kod yanlış yazdırıyor.
            {
                bilgi=bilgi+QString("\n%1. soru: 0 (%2%3)").arg(etkinSorular.at(i)).arg("%").arg(0);
            }
            else
            {
                yuzde=sayac/ogrenciSayisi*100;
                bilgi=bilgi+QString("\n%3. soru: %1 (%%2)").arg(sayac).arg(QString::number(yuzde,'f',2)).arg(etkinSorular.at(i));
            }
        }
        for(int i=0;i<liste.count();i++)
        {
            if(liste.count(liste.at(i))==etkinSorular.count()+1)//+1 sınav kıstasını sağayan öğrenci için
            {
                if(!liste2.contains(liste.at(i)))
                {
                    liste2.append(liste.at(i));
                }
            }
        }
        yuzde=liste2.count()/ogrenciSayisi*100;
        bilgi=bilgi+QString("\nhepsi: %1 (%%2)").arg(liste2.count()).arg(QString::number(yuzde,'f',2));

        QSqlQuery query3;
        query3.exec(QString("select toplampuan,ogrenciid from sinavogrenci where sinavid='%1' order by toplampuan").arg(sinavid));
        while(query3.next())
        {
            if(liste2.contains(query3.value(1).toString()) && query3.value(0).toString()!="--")//sınava girmeyenler listelenmesin
            {
                const int currentRow = formRaporla.ui->tableWidget->rowCount();
                formRaporla.ui->tableWidget->setRowCount(currentRow + 1);

                QTableWidgetItem *itm1=new QTableWidgetItem();
                itm1->setData(Qt::DisplayRole,query3.value(1).toInt());
                formRaporla.ui->tableWidget->setItem(currentRow,0,itm1); //ogrenciid leri yazdırıyor

                formSinavIstatistik.viewCizelge->insertColumns(formSinavIstatistik.viewCizelge->columnCount(),1);
                formSinavIstatistik.viewCizelge->setHeaderData(formSinavIstatistik.viewCizelge->columnCount()-1,Qt::Horizontal, query3.value(1).toString());

                QTableWidgetItem *itm2=new QTableWidgetItem();
                itm2->setData(Qt::DisplayRole,query3.value(0).toInt());
                formRaporla.ui->tableWidget->setItem(currentRow,1,itm2); //sınav sonucunu yazdırıyor
                formSinavIstatistik.viewCizelge->setData(formSinavIstatistik.viewCizelge->index(0,formSinavIstatistik.viewCizelge->columnCount()-1),query3.value(0).toString());
                for(int i=0;i<sorusayisi;i++)
                {
                    query2.exec(QString("select alinanpuan from sonuc where ogrenciid='%1' and sinavid='%2' and sorunumarasi='%3'").arg(query3.value(1).toString()).arg(sinavid).arg(i+1));
                    query2.next();
                    QTableWidgetItem *itm=new QTableWidgetItem();
                    itm->setData(Qt::DisplayRole,query2.value(0).toInt());
                    formRaporla.ui->tableWidget->setItem(currentRow,i+2,itm); //soruların sonuclarını yazdırıyor
                    if(etkinSorular.contains(QString::number(i+1)))
                    {
                        formSinavIstatistik.viewCizelge->setData(formSinavIstatistik.viewCizelge->index(etkinSorular.indexOf(QString::number(i+1))+1,formSinavIstatistik.viewCizelge->columnCount()-1),query2.value(0).toInt());
                    }
                }
            }
        }
    }
    else //sadece sınav sonucu
    {
        formSinavIstatistik.viewCizelge->setVerticalHeaderLabels(QStringList()<<"toplam");
        formSinavIstatistik.ui->tableViewCizelge->setModel(formSinavIstatistik.viewCizelge);
        formSinavIstatistik.viewCizelge->setHeaderData(0, Qt::Vertical, Qt::red, Qt::BackgroundRole);

        QSqlQuery query3;
        query3.exec(QString("select toplampuan,ogrenciid from sinavogrenci where sinavid='%1' order by toplampuan").arg(sinavid));
        while(query3.next())
        {
            if(liste.contains(query3.value(1).toString()) && query3.value(0).toString()!="--")//sınava girmeyenler listelenmesin
            {
                const int currentRow = formRaporla.ui->tableWidget->rowCount();
                formRaporla.ui->tableWidget->setRowCount(currentRow + 1);

                QTableWidgetItem *itm1=new QTableWidgetItem();
                itm1->setData(Qt::DisplayRole,query3.value(1).toInt());
                formRaporla.ui->tableWidget->setItem(currentRow,0,itm1); //ogrenciid leri yazdırıyor

                formSinavIstatistik.viewCizelge->insertColumns(formSinavIstatistik.viewCizelge->columnCount(),1);
                formSinavIstatistik.viewCizelge->setHeaderData(formSinavIstatistik.viewCizelge->columnCount()-1,Qt::Horizontal, query3.value(1).toString());

                QTableWidgetItem *itm2=new QTableWidgetItem();
                itm2->setData(Qt::DisplayRole,query3.value(0).toInt());
                formRaporla.ui->tableWidget->setItem(currentRow,1,itm2); //sınav sonucunu yazdırıyor
                formSinavIstatistik.viewCizelge->setData(formSinavIstatistik.viewCizelge->index(0,formSinavIstatistik.viewCizelge->columnCount()-1),query3.value(0).toString());
                for(int i=0;i<sorusayisi;i++)
                {
                    query2.exec(QString("select alinanpuan from sonuc where ogrenciid='%1' and sinavid='%2' and sorunumarasi='%3'").arg(query3.value(1).toString()).arg(sinavid).arg(i+1));
                    query2.next();
                    QTableWidgetItem *itm=new QTableWidgetItem();
                    itm->setData(Qt::DisplayRole,query2.value(0).toInt());
                    formRaporla.ui->tableWidget->setItem(currentRow,i+2,itm); //soruların sonuclarını yazdırıyor
                }
            }
        }
    }
    formRaporla.ui->lblOgrenciSayi->setText(bilgi);
    formRaporla.ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    formRaporla.ui->tableWidget->horizontalHeader()->setStretchLastSection(true);

    query.exec(QString("select sinavpuan from sinav where sinavid=(SELECT sinav.sinavid FROM sinav,derssinav WHERE sinav.sinavid=derssinav.sinavid AND sinavisim='%1' AND dersid=(select dersid from ders where dersisim='%2'))").arg(formRaporla.ui->cbSinav->currentText()).arg(ui->tableDersler->currentItem()->text()));
    query.next();
    formSinavIstatistik.setWindowTitle(formRaporla.ui->cbSinav->currentText()+" sınavı grafiği");
    formSinavIstatistik.cizelgeYukleme(query.value(0).toInt());
    formSinavIstatistik.cizelgeOlustur();
}

void proje::filtreSinav() //rapor1 de sınav değişince konuları dolduruyor
{
    formRaporla.ui->tableWidget->setRowCount(0);
    formRaporla.ui->tableWidgetKonular->setRowCount(0);
    formRaporla.ui->txtSinavPuan->setText("0");

    //YANLIŞ
    QSqlQuery query;
    query.exec(QString("SELECT sorunumarasi,konuisim,puan FROM soru WHERE sinavid=(select sinavid from sinav where sinavisim='%1')").arg(formRaporla.ui->cbSinav->currentText()));
    while(query.next())
    {
        const int currentRow = formRaporla.ui->tableWidgetKonular->rowCount();
        formRaporla.ui->tableWidgetKonular->setRowCount(currentRow + 1);

        QCheckBox *cb=new QCheckBox();
        formRaporla.ui->tableWidgetKonular->setCellWidget(currentRow,0,cb);

        QTableWidgetItem *itm1=new QTableWidgetItem(query.value(0).toString());
        formRaporla.ui->tableWidgetKonular->setItem(currentRow,1,itm1);
        QTableWidgetItem *itm2=new QTableWidgetItem(query.value(1).toString());
        formRaporla.ui->tableWidgetKonular->setItem(currentRow,2,itm2);
        QTableWidgetItem *itm3=new QTableWidgetItem(query.value(2).toString());
        formRaporla.ui->tableWidgetKonular->setItem(currentRow,3,itm3);

        QComboBox *com=new QComboBox();
        com->addItems(QStringList()<<">"<<"<"<<">="<<"<="<<"="<<"!=");
        formRaporla.ui->tableWidgetKonular->setCellWidget(currentRow,4,com);

        QTableWidgetItem *itm4=new QTableWidgetItem("0");
        formRaporla.ui->tableWidgetKonular->setItem(currentRow,5,itm4);
        itm1->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        itm2->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        itm3->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        connect(cb,SIGNAL(clicked()),this,SLOT(konuEtkin()));
    }
    query.exec(QString("select sinavpuan from sinav where sinavid=(SELECT sinav.sinavid FROM sinav,derssinav WHERE sinav.sinavid=derssinav.sinavid AND sinavisim='%1' AND dersid=(select dersid from ders where dersisim='%2'))").arg(formRaporla.ui->cbSinav->currentText()).arg(ui->tableDersler->currentItem()->text()));
    query.next();
    formRaporla.ui->lblSinavToplamPuan->setText("("+query.value(0).toString()+")");
    konuEtkin();
}

void proje::konuEtkin()//rapor1 de konu başındaki tik var mı diye bakıp enabled durmunu değiştiriyor
{
    for(int i=0;i<formRaporla.ui->tableWidgetKonular->rowCount();i++)
    {
        QCheckBox *cb=qobject_cast<QCheckBox *>(formRaporla.ui->tableWidgetKonular->cellWidget(i,0));
        if(cb->isChecked()==false)
        {
            for(int h=1;h<6;h++)//6 sutun var
            {
                if(h==4)//4. combobox sutunu aşağıdaki else in içindeki kod hata verdiği için ayrı
                {
                    QComboBox *com=qobject_cast<QComboBox *>(formRaporla.ui->tableWidgetKonular->cellWidget(i,4));
                    com->setEnabled(false);
                }
                else
                {
                    QTableWidgetItem *itm= new QTableWidgetItem();
                    itm=formRaporla.ui->tableWidgetKonular->item(i,h);
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
                    QComboBox *com=qobject_cast<QComboBox *>(formRaporla.ui->tableWidgetKonular->cellWidget(i,4));
                    com->setEnabled(true);
                }
                else
                {
                    QTableWidgetItem *itm= new QTableWidgetItem();
                    itm=formRaporla.ui->tableWidgetKonular->item(i,h);
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

void proje::raporDers()
{
    formRaporla.setWindowTitle(ui->tableDersler->currentItem()->text()+" raporu");
    formRaporla.ui->tableWidget->setRowCount(0);
    formRaporla.ui->cbSinav->clear();
    formRaporla.ui->lblOgrenciSayi->clear();
    QSqlQuery query;
    query.exec(QString("SELECT sinavisim FROM sinav,derssinav WHERE derssinav.sinavid=sinav.sinavid and dersid=(select dersid from ders where dersisim='%1')").arg(ui->tableDersler->currentItem()->text()));
    while(query.next())
    {
        formRaporla.ui->cbSinav->addItem(query.value(0).toString());
    }
    //formRaporla.exec();
    formRaporla.show();
}

void proje::fEkleOgrenci()
{
    formEkleOgrenci.setWindowTitle(ui->tableDersler->item(ui->tableDersler->currentRow(),0)->text()+" dersi için öğrenci ekle");
    //formEkleOgrenci.exec();
    formEkleOgrenci.show();
}

void proje::fEkleDers()
{
    QDate tarih;
    int yil=tarih.currentDate().year();
    formEkleDers.ui->lblYil->setText(QString::number(yil)+" - "+QString::number(yil+1));
    //formEkleDers.exec();
    formEkleDers.show();
    /*
    QSqlQuery query;
    query.exec("SELECT dersyil FROM ders");
    while(query.next())
    {
        if(ui->cbYil->findText(query.value(0).toString())==-1)
        {
            ui->cbYil->addItem(query.value(0).toString());
        }
    }
    dersleriYazdir();*/
}

void proje::fEkleSinav()
{
    formEkleSinav.setWindowTitle(ui->tableDersler->item(ui->tableDersler->currentRow(),0)->text()+" dersi için sınav ekle");
    formEkleSinav.ui->txtSinavIsim->clear();
    formEkleSinav.ui->txtSinavSoruSayisi->clear();
    formEkleSinav.ui->tableKonular->setRowCount(0);
    //formEkleSinav.exec();
    formEkleSinav.show();
}

void proje::tamamOgrenci()
{
    bool kayitVar=false;
    QString ogrenciID=formEkleOgrenci.ui->txtOgrenciID->text();
    QString ogrenciIsim=formEkleOgrenci.ui->txtOgrenciIsim->text();
    QString ogrenciDers=ui->tableDersler->item(ui->tableDersler->currentRow(),0)->text();
    QSqlQuery query;

    query.exec(QString("select ogrenci.ogrenciid from ogrenci,dersogrenci where ogrenci.ogrenciid=dersogrenci.ogrenciid and dersid=(select dersid from ders where dersisim='%1')").arg(ogrenciDers));
    while(query.next())
    {
        if(query.value(0).toString()==ogrenciID)
        {
            kayitVar=true;
        }
    }
    if(ogrenciID=="")
    {
        formEkleOgrenci.uyari(0);
    }
    else
    {
        if(kayitVar==true)
        {
            formEkleOgrenci.uyari(1);
        }
        else if(kayitVar==false)
        {

            QSqlQuery query,query2,query3;
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

            formEkleOgrenci.ui->txtOgrenciID->setText("");
            formEkleOgrenci.ui->txtOgrenciIsim->setText("");

            query.exec(QString("insert into dersogrenci values('%1',(SELECT dersid FROM ders WHERE dersisim='%2'))").arg(ogrenciID).arg(ogrenciDers));
            veritabaniGuncelle();
        }
    }
    ogrenciSayisiBul(ogrenciDers);
}

void proje::ogrenciSayisiBul(QString ders)
{
    QSqlQuery query;
    query.exec(QString("select count (distinct ogrenci.ogrenciid) from ogrenci,dersogrenci where ogrenci.ogrenciid=dersogrenci.ogrenciid and dersid=(select dersid from ders where dersisim='%1')").arg(ders));
    query.next();
    for(int i=0;i<ui->tableDersler->rowCount();i++)
    {
        if(ui->tableDersler->item(i,0)->text()==ders)
        {
            QTableWidgetItem *itm=new QTableWidgetItem(query.value(0).toString());
            ui->tableDersler->setItem(i,1,itm);
            itm->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

            i=ui->tableDersler->rowCount();
        }
    }
}

void proje::sinavSayisiBul(QString ders)
{
    QSqlQuery query;
    query.exec(QString("select count (distinct sinav.sinavid) from sinav,derssinav where sinav.sinavid=derssinav.sinavid and dersid=(select dersid from ders where dersisim='%1')").arg(ders));
    query.next();
    for(int i=0;i<ui->tableDersler->rowCount();i++)
    {
        if(ui->tableDersler->item(i,0)->text()==ders)
        {
            QTableWidgetItem *itm=new QTableWidgetItem(query.value(0).toString());
            ui->tableDersler->setItem(i,2,itm);
            itm->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

            i=ui->tableDersler->rowCount();
        }
    }
}

void proje::tamamSinav()
{
    bool SinavVar=false;
    bool sifirNotVar=false;
    QString sinavIsim=formEkleSinav.ui->txtSinavIsim->text();
    QString sinavSoruSayisi=formEkleSinav.ui->txtSinavSoruSayisi->text();
    QString sinavDers=ui->tableDersler->item(ui->tableDersler->currentRow(),0)->text();
    int sinavToplamPuan=0;

    if(sinavIsim=="" || sinavSoruSayisi=="")
    {
        formEkleSinav.uyari(0);
    }
    else
    {
        QSqlQuery query,query2,query3;
        query.exec(QString("select sinavisim from sinav,derssinav where sinav.sinavid=derssinav.sinavid and dersid=(select dersid from ders where dersisim='%1')").arg(sinavDers));
        while(query.next())
        {
            if(query.value(0)==sinavIsim)
            {
                SinavVar=true;
            }
        }
        if(SinavVar==true)
        {
            formEkleSinav.uyari(1);
        }
        else if(SinavVar==false)
        {
            for(int i=0;i<formEkleSinav.ui->tableKonular->rowCount();i++)
            {
                if(formEkleSinav.ui->tableKonular->item(i,1)->text().toInt()<=0)
                {
                    sifirNotVar=true;
                    i=formEkleSinav.ui->tableKonular->rowCount();
                }
            }
            if(sifirNotVar==true)//soru puanının sıfır olmaması için
            {
                formEkleSinav.uyari(3);
            }
            else if(sifirNotVar==false)
            {
                for(int i=1;i<=sinavSoruSayisi.toInt();i++)
                {
                    sinavToplamPuan=sinavToplamPuan+formEkleSinav.ui->tableKonular->item(i-1,1)->text().toInt();
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

                query3.exec(QString("select dersid from ders where dersisim='%1'").arg(sinavDers));
                query3.next();
                query2.exec(QString("insert into derssinav values ('%1','%2')").arg(query.value(0).toString()).arg(query3.value(0).toString()));

                formEkleSinav.ui->txtSinavIsim->setText("");
                formEkleSinav.ui->txtSinavSoruSayisi->setText("");

                for(int i=1;i<=sinavSoruSayisi.toInt();i++)
                {
                    query.exec(QString("select sinav.sinavid from sinav,derssinav where sinav.sinavid=derssinav.sinavid and sinav.sinavisim='%1' and derssinav.dersid=(select dersid from ders where dersisim='%2')").arg(sinavIsim).arg(sinavDers));
                    query.next();
                    query2.exec(QString("insert into soru values ('%1','%2','%3','%4')").arg(query.value(0).toString()).arg(i).arg(formEkleSinav.ui->tableKonular->item(i-1,0)->text()).arg(formEkleSinav.ui->tableKonular->item(i-1,1)->text()));
                    modelSoru->select();
                }

                for(int i=1;i<=sinavSoruSayisi.toInt();i++)
                {
                    bool konuVar=false;

                    query.exec(QString("select konuisim from konu where dersid=(select dersid from ders where dersisim='%1')").arg(sinavDers));
                    while(query.next())
                    {
                        if(query.value(0)==formEkleSinav.ui->tableKonular->item(i-1,0)->text())
                        {
                            konuVar=true;
                        }
                    }
                    if(konuVar==false)
                    {
                        query.exec(QString("select dersid from ders where dersisim='%1'").arg(sinavDers));
                        query.next();
                        query2.exec(QString("insert into konu (konuisim,dersid) values('%1','%2')").arg(formEkleSinav.ui->tableKonular->item(i-1,0)->text()).arg(query.value(0).toString()));
                        modelKonu->select();
                    }
                }
                formEkleSinav.ui->tableKonular->setRowCount(0);
                sinavSayisiBul(sinavDers);
                veritabaniGuncelle();
            }
        }
    }
}

void proje::tamamDers()
{
    QSqlQuery query;
    query.exec("SELECT dersyil FROM ders");
    while(query.next())
    {
        if(ui->cbYil->findText(query.value(0).toString())==-1)
        {
            ui->cbYil->addItem(query.value(0).toString());
        }
    }

    bool kayitVar=false;
    QString dersID=formEkleDers.ui->txtDersID->text();
    QString dersIsim=formEkleDers.ui->txtDersIsim->text();
    QString dersDonem=formEkleDers.ui->cbDonem->currentText();
    QString dersYil=formEkleDers.ui->lblYil->text();

    query.exec("select dersid from ders");
    while(query.next())
    {
        if(query.value(0).toString()==dersID)
        {
            kayitVar=true;
        }
    }
    if(dersID=="" || dersIsim=="")
    {
        formEkleDers.uyari(0);
    }
    else
    {
        if(kayitVar==true)
        {
            formEkleDers.uyari(1);
        }
        else if(kayitVar==false)
        {
            int sira=modelDers->rowCount();
            modelDers->insertRows(0,1);
            modelDers->setData(modelDers->index(sira,0),dersID);
            modelDers->setData(modelDers->index(sira,1),dersIsim);
            modelDers->setData(modelDers->index(sira,2),dersDonem);
            modelDers->setData(modelDers->index(sira,3),dersYil);
            modelDers->submitAll();

            formEkleDers.ui->txtDersID->setText("");
            formEkleDers.ui->txtDersIsim->setText("");

            dersleriYazdir();
        }
    }
}

void proje::yukleme()
{
    modelOgrenci=new QSqlRelationalTableModel(this);
    modelOgrenci->setTable("ogrenci");
    modelOgrenci->select();

    modelKonu=new QSqlRelationalTableModel(this);
    modelKonu->setTable("konu");
    modelKonu->select();

    modelSinav=new QSqlRelationalTableModel(this);
    modelSinav->setTable("sinav");
    modelSinav->select();

    modelDers=new QSqlRelationalTableModel(this);
    modelDers->setTable("ders");
    modelDers->select();

    modelDersSinav=new QSqlRelationalTableModel(this);
    modelDersSinav->setTable("derssinav");
    modelDersSinav->select();

    modelDersOgrenci=new QSqlRelationalTableModel(this);
    modelDersOgrenci->setTable("dersogrenci");
    modelDersOgrenci->select();

    modelSoru=new QSqlRelationalTableModel(this);
    modelSoru->setTable("soru");
    modelSoru->select();

    modelSonuc=new QSqlRelationalTableModel(this);
    modelSonuc->setTable("sonuc");
    modelSonuc->select();

    modelSinavOgrenci=new QSqlRelationalTableModel(this);
    modelSinavOgrenci->setTable("sinavogrenci");
    modelSinavOgrenci->select();

    modelSonucSinav=new QSqlRelationalTableModel(this);
    modelSonucSinav->setTable("sonucsinav");
    modelSonucSinav->select();

    ui->tableDersler->setRowCount(0);
    ui->tableDersler->setColumnCount(3);
    ui->tableDersler->setHorizontalHeaderLabels(QStringList()<<tr("Ders")<<tr("Öğrenci")<<tr("Sınav"));
    ui->tableDersler->horizontalHeader()->setResizeMode(0,QHeaderView::ResizeToContents);
    ui->tableDersler->horizontalHeader()->setResizeMode(1,QHeaderView::ResizeToContents);
    ui->tableDersler->horizontalHeader()->setStretchLastSection(true);
    ui->tableDersler->setSelectionBehavior(QAbstractItemView::SelectRows);

    QSqlQuery query;
    query.exec("SELECT dersyil FROM ders");
    while(query.next())
    {
        if(ui->cbYil->findText(query.value(0).toString())==-1)
        {
            ui->cbYil->addItem(query.value(0).toString());
        }
    }
    dersleriYazdir();
}

proje::~proje()
{
    delete ui;
}
