#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_quickssl.h"

class QuickSSL : public QMainWindow
{
    Q_OBJECT

public:
    QuickSSL(QWidget *parent = Q_NULLPTR);

private slots:
    void on_sha1_encode_clicked();
    void on_sha256_encode_clicked();
    void on_sha512_encode_clicked();
    void on_md5_encode_clicked();
    void on_save_clicked();
    void on_rsa_gen_clicked();
    void on_base64_encode_clicked();
    void on_base64_decode_clicked();
private:
    Ui::QuickSSLClass ui;
};
