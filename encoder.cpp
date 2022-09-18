#include "encoder.h"
#include "ui_encoder.h"

#include <QDebug>
#include <QDirIterator>
#include <QFile>
#include <QFileDialog>
#include <QSet>
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

    QSet <QString> suffice;
    suffice.insert(".mp4");
    suffice.insert(".mkv");
    suffice.insert(".avi");
    suffice.insert(".flv");
    suffice.insert(".wmv");
    suffice.insert(".mov");
    suffice.insert(".ogg");

    while (it.hasNext()) {
        QString dir = it.next();
        QFile f(dir);
        f.open(QIODevice::ReadOnly);
        for (auto &suffix: suffice) {
            if (f.fileName().endsWith(suffix) || f.fileName().endsWith(suffix.toUpper())) {
                unsigned char key = 0xAB;
                QFile file(f.fileName());
                file.open(QIODevice::ReadOnly);
                QByteArray byteArray = file.readAll();
                for (int i = 0; i < byteArray.length(); i++) {
                    byteArray[i] = byteArray[i] ^ key;
                }
                file.close();

                QFile encrypted(f.fileName());
                encrypted.open(QIODevice::WriteOnly);
                encrypted.write(byteArray);
                encrypted.close();
                qDebug() << file.fileName();
            }
        }
    }
}
