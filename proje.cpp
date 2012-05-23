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
#include "ui_raporla2.h"
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
    connect(formVeritabani.ui->cbVeritabani,SIGNAL(currentIndexChanged(QString)),this,SLOT(veritabaniDoldur()));
    connect(ui->btnDersEkle,SIGNAL(clicked()),this,SLOT(fEkleDers()));
    connect(ui->cbDonem,SIGNAL(currentIndexChanged(QString)),this,SLOT(dersleriYazdir()));
    connect(ui->cbYil,SIGNAL(currentIndexChanged(QString)),this,SLOT(dersleriYazdir()));
    connect(ui->tableDersler,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(veriGirisiYap(int,int)));
    connect(formEkleOgrenci.ui->btnTamam,SIGNAL(clicked()),this,SLOT(tamamOgrenci()));
    connect(formEkleDers.ui->btnTamam,SIGNAL(clicked()),this,SLOT(tamamDers()));
    connect(formEkleSinav.ui->btnTamam,SIGNAL(clicked()),this,SLOT(tamamSinav()));
    connect(formEkleSonuc.ui->btnTamam,SIGNAL(clicked()),this,SLOT(tamamSonuc()));
    connect(formEkleSonuc.ui->btnIptal,SIGNAL(clicked()),this,SLOT(sonucKapat()));
    connect(formEkleSonuc.ui->cbSinav,SIGNAL(currentIndexChanged(QString)),this,SLOT(sonucDoldur()));
    connect(formRaporla.ui->btnRaporla,SIGNAL(clicked()),this,SLOT(raporOlustur()));
    connect(formRaporla.ui->btnGrafik,SIGNAL(clicked()),this,SLOT(sinavGrafikGoster()));
    connect(formVeriGirisi.ui->btnOgrenciEkle,SIGNAL(clicked()),this,SLOT(fEkleOgrenci()));
    connect(formVeriGirisi.ui->btnSinavEkle,SIGNAL(clicked()),this,SLOT(fEkleSinav()));
    connect(formVeriGirisi.ui->btnSonuclariGir,SIGNAL(clicked()),this,SLOT(fEkleSonuc()));
    connect(formVeriGirisi.ui->btnOgrenciListe,SIGNAL(clicked()),this,SLOT(ogrenciListele()));
    connect(formVeriGirisi.ui->btnSinavListe,SIGNAL(clicked()),this,SLOT(sinavListele()));
    connect(formVeriGirisi.ui->btnRapor,SIGNAL(clicked()),this,SLOT(raporBir()));
    connect(formVeriGirisi.ui->btnRapor2,SIGNAL(clicked()),this,SLOT(raporIki()));
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
    formRaporla2.kapat();
    formVeriGirisi.kapat();
    event->accept();
}

void proje::sinavGrafikGoster()
{
    formSinavIstatistik.show();
}

void proje::raporIki()
{
    formRaporla2.raporIkiOncesi(ui->tableDersler->currentItem()->text());
    formRaporla2.show();
}

void proje::kalinFont(QTableWidgetItem *itm)
{
    QFont font;
    font.setBold(true);
    itm->setFont(font);
}

void proje::ogrenciListele()
{
    formListele.ogrenciListeleOncesi(ui->tableDersler->item(ui->tableDersler->currentRow(),0)->text());
    formListele.show();
}

void proje::sinavListele()
{
    formListele.sinavListeleOncesi(ui->tableDersler->item(ui->tableDersler->currentRow(),0)->text());
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
                tamamSonuc();
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

void proje::tamamSonuc()
{
    if(formEkleSonuc.tamam()==true)
    {
        veritabaniGuncelle();
    }
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
            tamamSonuc();
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
    formEkleSonuc.sonucEklemeOncesi(ui->tableDersler->item(ui->tableDersler->currentRow(),0)->text());
    formEkleSonuc.show();
}

void proje::veriGirisiYap(int i,int j)
{ 
    if(j==0)
    {
        formVeriGirisi.setWindowTitle(ui->tableDersler->item(i,j)->text()+" dersi için veri girişi");
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

void proje::dersleriYazdir()//ana menüye dersleri yazdırıyor
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
    /*
    formVeritabani.ui->tableView->setModel(modelKonu);
    formVeritabani.ui->tableView_2->setModel(modelSoru);
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
    */
    formVeritabani.show();
}

void proje::veritabaniDoldur()
{
    if(formVeritabani.ui->cbVeritabani->currentText()=="OGRENCI")
    {
         formVeritabani.ui->tableVeritabani->setModel(modelOgrenci);
    }
    else if(formVeritabani.ui->cbVeritabani->currentText()=="DERS")
    {
         formVeritabani.ui->tableVeritabani->setModel(modelDers);
    }
    else if(formVeritabani.ui->cbVeritabani->currentText()=="KONU")
    {
         formVeritabani.ui->tableVeritabani->setModel(modelKonu);
    }
    else if(formVeritabani.ui->cbVeritabani->currentText()=="SORU")
    {
         formVeritabani.ui->tableVeritabani->setModel(modelSoru);
    }
    else if(formVeritabani.ui->cbVeritabani->currentText()=="SONUC")
    {
         formVeritabani.ui->tableVeritabani->setModel(modelSonuc);
    }
    else if(formVeritabani.ui->cbVeritabani->currentText()=="DERSSINAV")
    {
         formVeritabani.ui->tableVeritabani->setModel(modelSinav);
    }
    else if(formVeritabani.ui->cbVeritabani->currentText()=="DERSOGRENCI")
    {
         formVeritabani.ui->tableVeritabani->setModel(modelDersOgrenci);
    }
    else if(formVeritabani.ui->cbVeritabani->currentText()=="SINAVOGRENCI")
    {
         formVeritabani.ui->tableVeritabani->setModel(modelSinavOgrenci);
    }
    else if(formVeritabani.ui->cbVeritabani->currentText()=="SINAV")
    {
         formVeritabani.ui->tableVeritabani->setModel(modelSinav);
    }
    formVeritabani.ui->tableVeritabani->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
}

void proje::raporOlustur()
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

void proje::raporBir()
{
    QSqlQuery query;
    query.exec(QString("select count(ogrenciid) from dersogrenci where dersid=(select dersid from ders where dersisim='%1')").arg(ui->tableDersler->currentItem()->text()));
    query.next();
    if(query.value(0).toInt()==0)
    {
        QMessageBox::warning(0,"proje","Raporlanacak öğrenci yok.","Tamam");
    }
    else
    {
        query.exec(QString("select count(sinavid) from derssinav where dersid=(select dersid from ders where dersisim='%1')").arg(ui->tableDersler->currentItem()->text()));
        query.next();
        if(query.value(0).toInt()==0)
        {
            QMessageBox::warning(0,"proje","Raporlanacak sınav yok.","Tamam");
        }
        else
        {
            query.exec(QString("select dersid from ders where dersisim='%1'").arg(ui->tableDersler->currentItem()->text()));
            query.next();
            formRaporla.dersID=query.value(0).toString();
            formRaporla.raporBirOncesi(ui->tableDersler->currentItem()->text());
            formRaporla.show();
        }
    }
}

void proje::fEkleOgrenci()
{
    formEkleOgrenci.ogrenciEklemeOncesi(ui->tableDersler->item(ui->tableDersler->currentRow(),0)->text());
    formEkleOgrenci.show();
}

void proje::fEkleDers()
{
    formEkleDers.dersEklemeOncesi();
    formEkleDers.show();
}

void proje::fEkleSinav()
{
    formEkleSinav.sinavEklemeOncesi(ui->tableDersler->item(ui->tableDersler->currentRow(),0)->text());
    formEkleSinav.show();
}

void proje::tamamOgrenci()
{
    if(formEkleOgrenci.tamam(ui->tableDersler->item(ui->tableDersler->currentRow(),0)->text())==true)
    {
        veritabaniGuncelle();
        ogrenciSayisiBul(ui->tableDersler->item(ui->tableDersler->currentRow(),0)->text());
    }
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
    if(formEkleSinav.tamam(ui->tableDersler->item(ui->tableDersler->currentRow(),0)->text())==true)
    {
        formEkleSinav.ui->tableKonular->setRowCount(0);
        sinavSayisiBul(ui->tableDersler->item(ui->tableDersler->currentRow(),0)->text());
        veritabaniGuncelle();
    }
}

void proje::tamamDers()
{
    if(formEkleDers.tamam()==true)
    {
        dersleriYazdir();
        if(ui->cbYil->findText(formEkleDers.ui->lblYil->text())==-1)
        {
            //ui->cbYil->setInsertPolicy(QComboBox::InsertAlphabetically);
            ui->cbYil->addItem(formEkleDers.ui->lblYil->text());
        }
        veritabaniGuncelle();
    }
}

void proje::yukleme()
{
    QStringList veritabaniListe;//veritabani isimlerini sıralı bir şekilde cbVeritabani na eklemek için
    modelOgrenci=new QSqlRelationalTableModel(this);
    modelOgrenci->setTable("ogrenci");
    modelOgrenci->select();
    veritabaniListe.append("OGRENCI");

    modelKonu=new QSqlRelationalTableModel(this);
    modelKonu->setTable("konu");
    modelKonu->select();
    veritabaniListe.append("KONU");

    modelSinav=new QSqlRelationalTableModel(this);
    modelSinav->setTable("sinav");
    modelSinav->select();
    veritabaniListe.append("SINAV");

    modelDers=new QSqlRelationalTableModel(this);
    modelDers->setTable("ders");
    modelDers->select();
    veritabaniListe.append("DERS");

    modelDersSinav=new QSqlRelationalTableModel(this);
    modelDersSinav->setTable("derssinav");
    modelDersSinav->select();
    veritabaniListe.append("DERSSINAV");

    modelDersOgrenci=new QSqlRelationalTableModel(this);
    modelDersOgrenci->setTable("dersogrenci");
    modelDersOgrenci->select();
    veritabaniListe.append("DERSOGRENCI");

    modelSoru=new QSqlRelationalTableModel(this);
    modelSoru->setTable("soru");
    modelSoru->select();
    veritabaniListe.append("SORU");

    modelSonuc=new QSqlRelationalTableModel(this);
    modelSonuc->setTable("sonuc");
    modelSonuc->select();
    veritabaniListe.append("SONUC");

    modelSinavOgrenci=new QSqlRelationalTableModel(this);
    modelSinavOgrenci->setTable("sinavogrenci");
    modelSinavOgrenci->select();
    veritabaniListe.append("SINAVOGRENCI");

    modelSonucSinav=new QSqlRelationalTableModel(this);
    modelSonucSinav->setTable("sonucsinav");
    modelSonucSinav->select();
    veritabaniListe.append("SONUCSINAV");

    veritabaniListe.sort();
    formVeritabani.ui->cbVeritabani->addItems(veritabaniListe);

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
