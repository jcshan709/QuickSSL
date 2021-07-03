#include "quickssl.h"
#include "yaml-cpp/yaml.h"
#include <QtWidgets/QApplication>
#include <QtCore/QTranslator>
#include <QtWidgets/QMessagebox>
#include <QtCore/QFile>
#include <QtCore/QStandardPaths>
#include <QDebug>

YAML::Node config;
QString lang;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    try {
        config = YAML::LoadFile((QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation)[0] + "config.yml").toStdString());
        lang = QString::fromStdString(config["config"]["locale"].as<std::string>());
    }
    catch (YAML::Exception& e) {
        QFile f((QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation)[0] + "config.yml"));
        if (f.open(QIODevice::WriteOnly)) {
            f.write("config:\n\
  locale: en");
            f.close();
            config = YAML::LoadFile((QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation)[0] + "config.yml").toStdString());
            lang = "en";
        }
        else {
            QMessageBox::critical(nullptr, "An exception occurred while the program was running", "Cannot open file to write.");
            return 1;
        }
    }
    if (lang != "en") {
        if (!translator.load("quickssl_" + lang + ".qm")) {
            QMessageBox::warning(nullptr, "An exception occurred while the program was running", QString("Translation file ") + "quickssl_" + lang + ".qm" + " not found!\nNow using default language settings.");
            lang = "en";
        }
        if (!a.installTranslator(&translator)) {
            QMessageBox::critical(nullptr, "An exception occurred while the program was running", "Cannot install translation file!\nNow using default language settings.");
            lang = "en";
        }
    }
    QuickSSL w;
    w.show();
    return a.exec();
}
