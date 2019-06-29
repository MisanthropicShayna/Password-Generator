#include <QApplication>
#include <QSettings>

#include <iostream>
#include <fstream>

#include "main_window.hxx"

// QString someValue = settings.value("some/config/key", "default value if unset").toString(); // settings.value() returns QVariant

int main(int argc, char* argv[]) {
    QApplication application(argc, argv);

    MainWindow main_window;
    main_window.show();
    return application.exec();

}

