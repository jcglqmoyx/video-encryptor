#include "encoder.h"
#include "ui_encoder.h"

#include <QDebug>
#include <QDirIterator>
#include <QFile>
#include <QFileDialog>
#include <QSet>
#include <QString>
#include <QTextStream>
#include <QDateTime>

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

    QString password = this->ui->password->text();
    if (password.isEmpty()) {
        this->ui->info->setText("请输入密码");
        return;
    }

    int mod = 1e8 + 7;
    int x = 0;
    for (QChar c: password) {
        x = x * 10 + c.toLatin1();
        x %= mod;
    }
    unsigned char key = (unsigned char) x;

    QSet <QString> audio_file_suffice;
    audio_file_suffice.insert(".mp3");
    audio_file_suffice.insert(".wav");
    audio_file_suffice.insert(".wma");
    audio_file_suffice.insert(".ogg");
    audio_file_suffice.insert(".aac");
    audio_file_suffice.insert(".mod");
    audio_file_suffice.insert(".flac");
    audio_file_suffice.insert(".alac");
    while (it.hasNext()) {
        QString dir = it.next();
        QFile f(dir);
        if (f.fileName().endsWith("_info.txt")) {
            continue;
        }
        f.open(QIODevice::ReadOnly);
        QFile file(f.fileName());
        file.open(QIODevice::ReadOnly);
        if (!file.isReadable()) continue;

        int len = 100;
        for (auto &suffix: audio_file_suffice) {
            if (file.fileName().endsWith(suffix)) {
                len = file.size();
            }
        }

        QByteArray byteArray = file.read(len);
        for (int i = 0; i < len; i++) {
            byteArray[i] = byteArray[i] ^ key;
        }
        file.close();

        QFile encrypted(f.fileName());
        encrypted.open(QIODevice::ReadWrite);
        encrypted.write(byteArray);
        encrypted.close();
        this->ui->info->setText("File " + file.fileName() + " 处理完成");
    }
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss");

    QFile file(filename + "/" + current_date + "_info.txt");
    file.open(QIODevice::ReadWrite);
    QTextStream out(&file);
    out << "操作时间：" + current_date << '\n';
    out << "执行加密/解密操作的文件夹：" + filename << '\n';
    out << "密码：" << password << '\n';
    out << '\n';

    this->ui->info->setText(current_date + ": 处理完成.");
}
