#include "encoder.h"
#include "ui_encoder.h"

#include <QDebug>
#include <QDirIterator>
#include <QFile>
#include <QFileDialog>
#include <QVector>
#include <QString>
#include <QTextStream>

Encoder::Encoder(QWidget *parent) : QWidget(parent), ui(new Ui::Encoder) {
    ui->setupUi(this);
}

Encoder::~Encoder() {
    delete ui;
}

void Encoder::on_choose_file_button_clicked() {
    QString filename = QFileDialog::getExistingDirectory(this, "Choose Folder");

    if (filename.isEmpty()) {
        return;
    }
    ui->file_path->setText(filename);
}

void Encoder::on_encode_or_decode_button_clicked() {
    QString filename = ui->file_path->text();
    QDirIterator it(filename, QDirIterator::Subdirectories);

    QVector <QString> suffice;
    suffice.push_back(".mp4");
    suffice.push_back(".mkv");
    suffice.push_back(".avi");
    suffice.push_back(".flv");
    suffice.push_back(".wmv");
    suffice.push_back(".mov");

    suffice.push_back(".mp3");
    suffice.push_back(".wav");
    suffice.push_back(".wma");
    suffice.push_back(".ogg");
    suffice.push_back(".aac");
    suffice.push_back(".mod");
    suffice.push_back(".flac");
    suffice.push_back(".alac");

    suffice.push_back(".gif");
    suffice.push_back(".png");
    suffice.push_back(".jpg");
    suffice.push_back(".bmp");
    suffice.push_back(".svg");
    suffice.push_back(".webp");
    suffice.push_back(".jpeg");

    QString password = this->ui->password->text();
    if (password.isEmpty()) {
        this->ui->info->setText("Please input password!");
        return;
    }

    int mod = 1e8 + 7;
    int x = 0;
    for (QChar c: password) {
       x = x * 10 + c.toLatin1();
       x %= mod;
    }
    unsigned char key = (unsigned char) x;

    while (it.hasNext()) {
        QString dir = it.next();
        QFile f(dir);
        f.open(QIODevice::ReadOnly);
        for (auto &suffix: suffice) {
            if (f.fileName().endsWith(suffix) || f.fileName().endsWith(suffix.toUpper())) {
                QFile file(f.fileName());
                file.open(QIODevice::ReadOnly);

                int len = 100;

                QByteArray byteArray = file.read(len);
                for (int i = 0; i < len; i++) {
                    byteArray[i] = byteArray[i] ^ key;
                }
                file.close();

                QFile encrypted(f.fileName());
                encrypted.open(QIODevice::ReadWrite);
                encrypted.write(byteArray);
                encrypted.close();
                this->ui->info->setText("File " + file.fileName() + " processed");
            }
        }
    }
    this->ui->info->setText("Finished.");
}
