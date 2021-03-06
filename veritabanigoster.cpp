#include "veritabanigoster.h"
#include "ui_veritabanigoster.h"

veritabaniGoster::veritabaniGoster(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::veritabaniGoster)
{
    ui->setupUi(this);
    connect(ui->btnKapat,SIGNAL(clicked()),this,SLOT(kapat()));
}

void veritabaniGoster::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Escape)
    {
        kapat();
    }
}

void veritabaniGoster::kapat()
{
    close();
}

veritabaniGoster::~veritabaniGoster()
{
    delete ui;
}
