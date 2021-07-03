#include "quickssl.h"
#include "openssl/sha.h"
#include "openssl/md5.h"
#include "openssl/rsa.h"
#include "openssl/pem.h"
#include "yaml-cpp/yaml.h"
#include <QMessagebox>
#include <fstream>
#include <QFileDialog>
#include <QStandardPaths>

extern QString lang;
extern YAML::Node config;

QuickSSL::QuickSSL(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.locale->setText(lang);
}

void QuickSSL::on_sha1_encode_clicked() {
    QString source = ui.sha1_source->toPlainText();
    SHA_CTX ctx;
    SHA1_Init(&ctx);
    SHA1_Update(&ctx, source.toStdString().c_str(), source.length());
    unsigned char* decoded = new unsigned char[SHA_DIGEST_LENGTH]();
    SHA1_Final(decoded, &ctx);
    QByteArray result = QByteArray::fromRawData((const char*)decoded, SHA_DIGEST_LENGTH);
    ui.sha1_encoded->setPlainText(QString::fromLatin1(result.toHex()));
    delete[] decoded;
}

void QuickSSL::on_sha256_encode_clicked() {
    QString source = ui.sha256_source->toPlainText();
    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, source.toStdString().c_str(), source.length());
    unsigned char* decoded = new unsigned char[SHA256_DIGEST_LENGTH]();
    SHA256_Final(decoded, &ctx);
    QByteArray result = QByteArray::fromRawData((const char*)decoded, SHA256_DIGEST_LENGTH);
    ui.sha256_encoded->setPlainText(QString::fromLatin1(result.toHex()));
    delete[] decoded;
}

void QuickSSL::on_sha512_encode_clicked() {
    QString source = ui.sha512_source->toPlainText();
    SHA512_CTX ctx;
    SHA512_Init(&ctx);
    SHA512_Update(&ctx, source.toStdString().c_str(), source.length());
    unsigned char* decoded = new unsigned char[SHA512_DIGEST_LENGTH]();
    SHA512_Final(decoded, &ctx);
    QByteArray result = QByteArray::fromRawData((const char*)decoded, SHA512_DIGEST_LENGTH);
    ui.sha512_encoded->setPlainText(QString::fromLatin1(result.toHex()));
    delete[] decoded;
}

void QuickSSL::on_md5_encode_clicked() {
    QString source = ui.md5_source->toPlainText();
    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, source.toStdString().c_str(), source.length());
    unsigned char* decoded = new unsigned char[MD5_DIGEST_LENGTH]();
    MD5_Final(decoded, &ctx);
    QByteArray result = QByteArray::fromRawData((const char*)decoded, MD5_DIGEST_LENGTH);
    ui.md5_encoded->setPlainText(QString::fromLatin1(result.toHex()));
    delete[] decoded;
}

void QuickSSL::on_save_clicked() {
    config["config"]["locale"] = ui.locale->text().toStdString();
    std::fstream conf((QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation)[0] + "config.yml").toStdString());
    if (conf.is_open()) {
        conf << config;
        QMessageBox::information(this, tr("Information"), tr("You should restart this program to enable these settings."));
    }
    else QMessageBox::critical(this, tr("An exception occurred while the program was running"), tr("Cannot save settings!"));
}

void QuickSSL::on_rsa_gen_clicked() {
    QString path1 = QFileDialog::getSaveFileName(this, tr("Saving private key"), QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)[0], tr("RSA key file (*.pem)"));
    QString path2 = QFileDialog::getSaveFileName(this, tr("Saving public key"), QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)[0], tr("RSA key file (*.pem)"));
    size_t len = 0;
    char* key = nullptr;
    RSA* keypair = RSA_generate_key((ui.bitsettings->currentIndex() + 1) * 1024, RSA_3, nullptr, nullptr);
    BIO* out = BIO_new_file(path1.toStdString().c_str(), "w");
    PEM_write_bio_RSAPrivateKey(out, keypair, nullptr, nullptr, 0, nullptr, nullptr);
    BIO_flush(out);
    BIO_free(out);
    out = BIO_new_file(path2.toStdString().c_str(), "w");
    PEM_write_bio_RSA_PUBKEY(out, keypair);
    BIO_flush(out);
    BIO_free(out);
    RSA_free(keypair);
    QMessageBox::information(this, tr("Information"), tr("Private key and public key generated successfully!"));
}

void QuickSSL::on_base64_encode_clicked() {
    ui.base64_encoded->setPlainText(QString::fromLatin1(ui.base64_source->toPlainText().toUtf8().toBase64()));
}
void QuickSSL::on_base64_decode_clicked() {
    ui.base64_source->setPlainText(QString::fromUtf8(QByteArray::fromBase64(ui.base64_encoded->toPlainText().toLatin1())));
}
