#include "raporla.h"
#include "ui_raporla.h"
#include <QTextCodec>
#include <QSqlQuery>
#include <QDebug>

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
    yukleme();
}

void raporla::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Escape)
    {
        kapat();
    }
}

void raporla::kapat()
{
    close();
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
