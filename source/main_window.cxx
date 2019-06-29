#include "main_window.hxx"
#include "ui_main_window.h"

int32_t MainWindow::randomNumber_(const int32_t& from, const int32_t& to) const {
    static std::random_device device;
    static std::mt19937 generator(device());

    std::uniform_int_distribution<int32_t> uniform_distributer(from, to);

    int32_t generated = uniform_distributer(generator);
    return generated;
}

void MainWindow::setLcdToSum_() {
    const static std::vector<std::pair<QSpinBox*, QCheckBox*>> spinboxes {
        {ui->spin_numericalSize, ui->check_numerical},
        {ui->spin_specialSize, ui->check_special},
        {ui->spin_lowerSize, ui->check_lower},
        {ui->spin_upperSize, ui->check_upper}
    };

    const int32_t password_size = std::accumulate(spinboxes.begin(), spinboxes.end(), 0, [](int32_t& acc, std::pair<QSpinBox*, QCheckBox*> curr) {
        return acc + (curr.second->isChecked() ? curr.first->value() : 0);
    });

    ui->lcd_size->display(password_size);
}

void MainWindow::on_btn_generate_clicked() {
    const std::size_t password_size = static_cast<std::size_t>(
        ui->spin_numericalSize->value() + ui->spin_specialSize->value() +
        ui->spin_upperSize->value() + ui->spin_lowerSize->value()
    );

    std::string generated_password;
    generated_password.reserve(password_size);

    for(int32_t i=0; i<ui->spin_numericalSize->value(); ++i) {
        generated_password.push_back(numerical_[randomNumber_(0, strlen(numerical_)-1)]);
    }

    for(int32_t i=0; i<ui->spin_specialSize->value() && ui->check_special->isChecked(); ++i) {
        generated_password.push_back(special_[randomNumber_(0, strlen(special_)-1)]);
    }

    for(int32_t i=0; i<ui->spin_upperSize->value() && ui->check_upper->isChecked(); ++i) {
        generated_password.push_back(upperCase_[randomNumber_(0, strlen(upperCase_)-1)]);
    }

    for(int32_t i=0; i<ui->spin_lowerSize->value() && ui->check_lower->isChecked(); ++i) {
        generated_password.push_back(lowerCase_[randomNumber_(0, strlen(lowerCase_)-1)]);
    }

    std::random_shuffle(generated_password.begin(), generated_password.end());

    ui->textEdit_Output->setText(QString::fromStdString(generated_password));
}

void MainWindow::on_btn_distribute_clicked() {
    const static std::vector<std::pair<QSpinBox*, QCheckBox*>> spinboxes {
        {ui->spin_numericalSize, ui->check_numerical},
        {ui->spin_specialSize, ui->check_special},
        {ui->spin_lowerSize, ui->check_lower},
        {ui->spin_upperSize, ui->check_upper}
    };

    const int32_t enabled = std::accumulate(spinboxes.begin(), spinboxes.end(), 0, [](int32_t& acc, std::pair<QSpinBox*, QCheckBox*> curr) {
        return acc + curr.second->isChecked();
    });

    if(!enabled) {
        QMessageBox::warning(this, "Cannot distribute.", "Please enable at least one character set in order to distribute.");
        return;
    }

    const int32_t division_count = static_cast<int32_t>(std::round(ui->spin_distSize->value() / enabled));

    for(const auto& spinbox : spinboxes) {
        if(spinbox.second->isChecked()) {
            spinbox.first->setValue(division_count);
        }
    }
}

void MainWindow::on_btn_genBySeed_clicked() {
    const QString& seed = ui->line_seed->text();

    if(seed.size() <= 0) {
        QMessageBox::warning(this, "No seed.", "Please supply a seed to generate by seed.");
        return;
    }

    QString generated_password;
    generated_password.reserve(ui->spin_sizeBySeed->value());

    for(int32_t i=0; i<ui->spin_sizeBySeed->value(); ++i) {
        generated_password.append(seed.at(randomNumber_(0, seed.size()-1)));
    }

    ui->textEdit_Output->setText(generated_password);
}

void MainWindow::on_btn_copy_clicked() {
    QClipboard* global_clipboard = QApplication::clipboard();
    global_clipboard->setText(ui->textEdit_Output->toPlainText());
}

void MainWindow::on_spin_sizeBySeed_valueChanged(int value) {
    ui->lcd_size->display(value);
}

bool MainWindow::LoadStylesheet(const std::string& qstylesheet_path) {
    std::ifstream input_stream(qstylesheet_path, std::ios::binary);

    if(input_stream.good()) {
        const QString& qss_string = QString::fromStdString(std::string((std::istreambuf_iterator<char>(input_stream)), (std::istreambuf_iterator<char>())));

        QCss::Parser qss_parser(qss_string);
        QCss::StyleSheet stylesheet;

        if(!qss_parser.parse(&stylesheet)) {
            QMessageBox::warning(this, "QStyleSheet parsing failure.",
                "There was an error when trying to parse style.qss, verify that the QSS is valid, or delete the file to disable styling.");
            return false;
        } else {
            setStyleSheet(qss_string);
            return true;
        }
    } else {
        return false;
    }
}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    LoadStylesheet();
    ui->setupUi(this);

    connect(ui->spin_numericalSize, SIGNAL(valueChanged(int)), SLOT(setLcdToSum_()));
    connect(ui->spin_specialSize, SIGNAL(valueChanged(int)), SLOT(setLcdToSum_()));
    connect(ui->spin_upperSize, SIGNAL(valueChanged(int)), SLOT(setLcdToSum_()));
    connect(ui->spin_lowerSize, SIGNAL(valueChanged(int)), SLOT(setLcdToSum_()));

    connect(ui->check_numerical, SIGNAL(stateChanged(int)), SLOT(setLcdToSum_()));
    connect(ui->check_special, SIGNAL(stateChanged(int)), SLOT(setLcdToSum_()));
    connect(ui->check_upper, SIGNAL(stateChanged(int)), SLOT(setLcdToSum_()));
    connect(ui->check_lower, SIGNAL(stateChanged(int)), SLOT(setLcdToSum_()));
}

MainWindow::~MainWindow() {
    delete ui;
}
