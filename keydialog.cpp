#include "keydialog.h"
#include "ui_keydialog.h"

KeyDialog::KeyDialog(QWidget *parent) :QDialog(parent),ui(new Ui::KeyDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(okButtonClicked()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancelButtonClicked()));
}

KeyDialog::~KeyDialog()
{
    delete ui;
}

QByteArray KeyDialog::getKey()
{
    key = QByteArray();
    ui->lineEdit->clear();
    show();
    QEventLoop loop;
    connect(this, SIGNAL(buttonClickProcessed()), &loop, SLOT(quit()));
    loop.exec();
    disconnect(this, SIGNAL(buttonClickProcessed()), &loop, SLOT(quit()));
    close();
    return key;
}

void KeyDialog::okButtonClicked()
{
    QString hex = ui->lineEdit->text();
    if(hex.size() != 64)
    {
        QMessageBox::warning(this, "Error", "Key length should be 256");
        return;
    }
    QByteArray result = QByteArray::fromHex(hex.toLatin1());
    if(result.length() != 32)
    {
        QMessageBox::warning(this, "Error", "Wrong format");
        return;
    }
    key = result;
    ok = true;
    emit buttonClickProcessed();
}

void KeyDialog::cancelButtonClicked()
{
    ok = false;
    emit buttonClickProcessed();
}
