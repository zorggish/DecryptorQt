#ifndef KEYDIALOG_H
#define KEYDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QDebug>

namespace Ui {
class KeyDialog;
}

class KeyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit KeyDialog(QWidget *parent = nullptr);
    ~KeyDialog();
    QByteArray getKey();

private:
    Ui::KeyDialog *ui;
    QByteArray key;
    bool ok;

private slots:
    void okButtonClicked();
    void cancelButtonClicked();

signals:
    void buttonClickProcessed();
};

#endif // KEYDIALOG_H
