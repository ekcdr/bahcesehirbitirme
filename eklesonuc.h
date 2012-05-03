#ifndef EKLESONUC_H
#define EKLESONUC_H

#include <QDialog>
#include <QKeyEvent>
#include <QMessageBox>
#include <QTextCodec>
#include <QtSql>

namespace Ui {
class ekleSonuc;
}

class ekleSonuc : public QDialog
{
    Q_OBJECT
    
public:
    explicit ekleSonuc(QWidget *parent = 0);
    ~ekleSonuc();
    Ui::ekleSonuc *ui;
    void keyPressEvent(QKeyEvent *e);
    void uyari(int i);
    bool ilkAcilis;
    bool ekleSonucDegisiklikVar;//eğer sonucdoldur ekranında hiçbir değişiklik yapılmazsa fonk'a girmesin diye
    QString sinavID;
    QStringList degisenIDOgrenci;//güncelleme sırasında tüm kayıtları kaydetmesin sadece değişenleri güncellesin
    void yukleme();
    void kapat();
    void sonucEklemeOncesi(QString);
    bool tamam();

public slots:
    void puanKontrol(int i,int j);//girilen puanın sayı olup olmadığına bakıyor.
    void toplamiGuncellestir(int i,int j);//sonuc ekranında puanlardan birini değiştirince toplamında değişmesi için

private:
};

#endif // EKLESONUC_H
