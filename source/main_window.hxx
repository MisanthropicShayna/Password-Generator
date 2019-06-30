#ifndef MAIN_WINDOW_HXX
#define MAIN_WINDOW_HXX

#include <QtGui/private/qcssparser_p.h>
#include <QMainWindow>
#include <QClipboard>
#include <QMessageBox>
#include <QTextStream>
#include <QSettings>
#include <QFile>
#include <QDir>

#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <random>
#include <string>
#include <vector>
#include <array>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
private:
    Q_OBJECT
    Ui::MainWindow* ui;
    QSettings settings_;

    static constexpr const char* upperCase_ = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    static constexpr const char* lowerCase_ = "abcdefghijklmnopqrstuvwxyz";
    static constexpr const char* special_   = "!@#$%^&*()_+-=[]{}/,.";
    static constexpr const char* numerical_ = "1234567890";

    int32_t randomNumber_(const int32_t&, const int32_t&) const;

private slots:
    void setLcdToSum_();
    void on_btn_generate_clicked();
    void on_btn_distribute_clicked();
    void on_btn_genBySeed_clicked();
    void on_btn_copy_clicked();

    void on_spin_sizeBySeed_valueChanged(int);

public:
    bool LoadStylesheet(const std::string& = "./style.qss");

    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
};

#endif // MAIN_WINDOW_HXX
