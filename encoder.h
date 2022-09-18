#ifndef ENCODER_H
#define ENCODER_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class Encoder;
}
QT_END_NAMESPACE

class Encoder : public QWidget {
    Q_OBJECT

public:
    Encoder(QWidget *parent = nullptr);

    ~Encoder();

private
    slots:
            void on_choose_file_button_clicked();
            void on_encode_or_decode_button_clicked();

private:
    Ui::Encoder *ui;
};
#endif // ENCODER_H
