#ifndef LISTELE_H
#define LISTELE_H

#include <QDialog>
#include <QKeyEvent>
#include <QTextCodec>
#include <QtSql>

namespace Ui {
class listele;
}

class listele : public QDialog
{
    Q_OBJECT
    
public:
    explicit listele(QWidget *parent = 0);
    ~listele();
    Ui::listele *ui;
    void keyPressEvent(QKeyEvent *e);
    void yuklemeOgrenci();
    void yuklemeSinav();
    void ogrenciListeleOncesi(QString);
    void sinavListeleOncesi(QString);

public slots:
    void kapat();
    
private:
};

#endif // LISTELE_H
