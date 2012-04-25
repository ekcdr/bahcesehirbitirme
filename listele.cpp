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

void listele::yuklemeSinav()
{
    ui->tableListe->setRowCount(0);
    ui->tableListe->setColumnCount(4);
    QStringList baslik;
    baslik<<tr("İsim")<<tr("Soru Sayisi")<<tr("Toplam Puan")<<tr("Ortalama");
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
