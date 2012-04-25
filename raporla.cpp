#include "raporla.h"
#include "ui_raporla.h"

#include <QPrinter>
#include <QPainter>
#include <QPixmap>
#include <QTextDocument>
#include <QFileDialog>

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
    connect(ui->btnYazdir,SIGNAL(clicked()),this,SLOT(yazdir()));
    yukleme();
}

void raporla::yazdir()
{
    /*
    QPixmap pix = QPixmap::grabWidget(ui->tableWidget);

    QPrinter printer(QPrinter::HighResolution);
    QPainter painter;
    painter.begin(&printer);
    //painter.drawPixmap (0, 0, &pix);
    painter.drawPixmap(0,0,pix);
    painter.end();
    */
/*
    //QPixmap pix = QPixmap::grabWidget(ui->tableWidget);
    //String fileName = QFileDialog::getSaveFileName(this,tr("Export as> Pdf.."),"", tr("PDF files (*.pdf)"));
    QPrinter printer(QPrinter::HighResolution);

    printer.setOutputFileName("nnn");
    printer.setOutputFormat( QPrinter::PdfFormat );

    QPainter p( &printer );
    //p.drawPixmap(0,0,pix);


    p.drawText(0, 0 , 250, 200,Qt::TextSingleLine,"sss");
    p.drawText(0, 0 , 250, 200,Qt::TextSingleLine,"dddd");

*/


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
