#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qaesencryption.h"
#include "QDebug"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->openFileButton, SIGNAL(clicked()), this, SLOT(openFileButtonClicked()));
    connect(ui->setKeyButton,   SIGNAL(clicked()), this, SLOT(setKeyButtonClicked()));
    connect(ui->clearButton,    SIGNAL(clicked()), this, SLOT(clearButtonClicked()));
    connect(ui->decryptButton,  SIGNAL(clicked()), this, SLOT(decryptButtonClicked()));
    connect(ui->duckCppRB,      SIGNAL(toggled(bool)), this, SLOT(radioButtonToggled()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openFileButtonClicked()
{
    QString filename = QFileDialog::getOpenFileName();
    if (filename.isEmpty())
        return;
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    ui->textEdit->setPlainText(file.readAll());
}

void MainWindow::setKeyButtonClicked()
{
    QByteArray newKey = keyDialog.getKey();
    if(!newKey.isEmpty())
        key = newKey;
}

void MainWindow::clearButtonClicked()
{
    ui->textEdit->clear();
}

QString MainWindow::decryptAes(QString input, bool CBC)
{
    QByteArray inputBytes = QByteArray::fromHex(input.toLatin1());
    QByteArray result;

    if(CBC)
    {
        if(inputBytes.length() < 16)
            return QString();
        QByteArray IV(16, Qt::Initialization::Uninitialized);
        for (int i = inputBytes.length() - 16, j = 0; i < inputBytes.length(); i++, j++)
            IV[j] = inputBytes[i];
        inputBytes.resize(inputBytes.size() - 16);
        result = QAESEncryption::Decrypt(QAESEncryption::AES_256, QAESEncryption::CBC, inputBytes, key, IV);
        result = QAESEncryption::RemovePadding(result, QAESEncryption::Padding::PKCS7);
    }
    else
    {
        result = QAESEncryption::Decrypt(QAESEncryption::AES_256, QAESEncryption::ECB, inputBytes, key);
    }

    return QString(result);
}

void MainWindow::decryptButtonClicked()
{
    if(key.isEmpty())
    {
        QMessageBox::warning(this, "Error", "You should set AES key to decrypt");
        return;
    }
    QString input = ui->textEdit->toPlainText();
    QStringList list = input.split(QString("\nENDBLOCK\n"), Qt::SkipEmptyParts);
    QString output;
    for(QString block : list)
        output += decryptAes(block, useCBC) + "\n";
    ui->textEdit->setPlainText(output);
}

void MainWindow::radioButtonToggled()
{
    useCBC = ui->duckSharpRB->isChecked();
}
