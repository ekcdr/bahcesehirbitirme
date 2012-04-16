#include "verigirisi.h"
#include "ui_verigirisi.h"

veriGirisi::veriGirisi(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::veriGirisi)
{
    ui->setupUi(this);
    connect(ui->btnKapat,SIGNAL(clicked()),this,SLOT(kapat()));
}

void veriGirisi::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Escape)
    {
        kapat();
    }
}

void veriGirisi::kapat()
{
    close();
}

veriGirisi::~veriGirisi()
{
    delete ui;
}
