#ifndef EKLEOGRENCI_H
#define EKLEOGRENCI_H

#include <QDialog>
#include <QKeyEvent>
#include <QMessageBox>
#include <QTextCodec>

namespace Ui {
    class ekleOgrenci;
}

class ekleOgrenci : public QDialog
{
    Q_OBJECT

public:
    explicit ekleOgrenci(QWidget *parent = 0);
    ~ekleOgrenci();
    Ui::ekleOgrenci *ui;
    void keyPressEvent(QKeyEvent *e);
    void uyari(int i);

public slots:
    void kapat();

private:

};

#endif // EKLEOGRENCI_H
