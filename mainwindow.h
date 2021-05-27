#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>

#include "keydialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QByteArray key = QByteArray::fromHex("cdf851f8efcc9c58c980642ecb43665c1b3779754e22462c0bf63cd66429cdc9"); //Default key
    QFileDialog fileDialog;
    KeyDialog keyDialog;
    bool useCBC = false;   //Decryption mode
    QString decryptAes(QString input, bool CBC);

private slots:
    void openFileButtonClicked();
    void setKeyButtonClicked();
    void clearButtonClicked();
    void decryptButtonClicked();
    void radioButtonToggled();
};
#endif // MAINWINDOW_H
