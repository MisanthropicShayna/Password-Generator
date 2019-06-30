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
    // Calculates the total size of the password.
    const int32_t password_size =
        std::accumulate(charsetSelections_.begin(), charsetSelections_.end(), 0, [](const auto& acc, const auto& curr) {
            return curr.second->isChecked() ? curr.first->value() + acc : acc;
        });

    // Sets the lcd to dislpay the calculated size.
    ui->lcd_size->display(password_size);
}

void MainWindow::on_btn_generate_clicked() {
    // Calculates the total password size ahead of time.
    const int32_t password_size =
        std::accumulate(charsetSelections_.begin(), charsetSelections_.end(), 0, [](const auto& acc, const auto& curr) {
            return  acc + curr.first->value();
        });

    /* A temporary buffer that will store the characters of the generated buffer
     * before being written to the output textEdit. This is done so that a the dialogue
     * does not have to re-draw and update for each character, leading to faster efficiency. */
    QString password_generation_buffer;

    // As the password size is known, the required amount of bytes to store the generated password can be allocated beforehand.
    password_generation_buffer.reserve(password_size);

    /* Goes through every character set, checks if its enabled, and inserts the selected
     * amount of characters from each character set into the password generation buffer. */
    if(ui->check_numerical->isChecked()) {
        for(int32_t i=0; i<ui->spin_numericalSize->value(); ++i) {
            const int32_t& random_index = randomNumber_(0, static_cast<int32_t>(strlen(numerical_)-1));
            const QChar& random_character = numerical_[random_index];
            password_generation_buffer.push_back(random_character);
        }
    }

    if(ui->check_special->isChecked()) {
        for(int32_t i=0; i<ui->spin_specialSize->value(); ++i) {
            const int32_t& random_index = randomNumber_(0, static_cast<int32_t>(strlen(numerical_)-1));
            const QChar& random_character = special_[random_index];
            password_generation_buffer.push_back(random_character);
        }
    }

    if(ui->check_special->isChecked()) {
        for(int32_t i=0; i<ui->spin_specialSize->value(); ++i) {
            const int32_t& random_index = randomNumber_(0, static_cast<int32_t>(strlen(upperCase_)-1));
            const QChar& random_character = upperCase_[random_index];
            password_generation_buffer.push_back(random_character);
        }
    }

    if(ui->check_special->isChecked()) {
        for(int32_t i=0; i<ui->spin_specialSize->value(); ++i) {
            const int32_t& random_index = randomNumber_(0, static_cast<int32_t>(strlen(lowerCase_)-1));
            const QChar& random_character = lowerCase_[random_index];
            password_generation_buffer.push_back(random_character);
        }
    }

    // Randomly shuffles the generated password, as otherwise it would be in ordered by character set.
    std::random_shuffle(password_generation_buffer.begin(), password_generation_buffer.end());

    // Writes the generated password to the output textEdit.
    ui->textEdit_Output->setText(password_generation_buffer);
}

void MainWindow::on_btn_distribute_clicked() {
    // The target password size. This should be the target number that is divided accross the different character sets.
    const int32_t password_size = ui->spin_distSize->value();

    // Counts the amount of character sets that are enabled.
    const int32_t enabled =
        std::accumulate(charsetSelections_.begin(), charsetSelections_.end(), 0, [](const auto& acc, const auto& curr){
            return acc + curr.second->isChecked();
        });

    // Letting distribution happen with no chracter sets enabled would cause a division by zero error.
    if(enabled == 0) {
        QMessageBox::warning(this, "Cannot distribute.", "Please enable at least one character set in order to distribute.");
        return;
    }

    // Divides the target password size, by the amount of selected character sets.
    const int32_t division = static_cast<int32_t>(std::round(password_size / enabled));

    // Sets the occurance count of each character set to the result of the division.
    for(const auto& charsetSelections_ : charsetSelections_) {
        if(charsetSelections_.second->isChecked()) {
            charsetSelections_.first->setValue(division);
        }
    }
}

void MainWindow::on_btn_genBySeed_clicked() {
    // The target size of the password about to be generated.
    const int32_t password_size = ui->spin_sizeBySeed->value();

    // The character set / seed that will be used to randomly draw characters from.
    const QString& seed = ui->line_seed->text();

    // Without a supplied seed, there would be no characters to generate a password from.
    if(seed.size() <= 0) {
        QMessageBox::warning(this, "No seed.", "Please supply a seed to generate by seed.");
        return;
    }

    /* A temporary buffer that will store the characters of the generated buffer
     * before being written to the output textEdit. This is done so that a the dialogue
     * does not have to re-draw and update for each character, leading to faster efficiency. */
    QString password_generation_buffer;

    // As the password size is known, the required amount of bytes to store the generated password can be allocated beforehand.
    password_generation_buffer.reserve(password_size);


    for(int32_t i=0; i<password_size; ++i) { // The seed's total size is subtracted as the index would start from zero.
        const int32_t random_index = randomNumber_(0, seed.size()-1);
        const QChar& random_character = seed.at(random_index);
        password_generation_buffer.append(random_character);
    }

    // Sets the output textEdit's text to the generated password.
    ui->textEdit_Output->setText(password_generation_buffer);
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
        // Constructs a QString from an std::string that is being constructed with a start/end iterator of the input stream.
        const QString& qss_string = QString::fromStdString(std::string(
            (std::istreambuf_iterator<char>(input_stream)),
            (std::istreambuf_iterator<char>())
        ));

        /* QCss::Parser/StyleSheet are both part of the private QtGui and QtCore libraries.
         * There is no official public way to check the validity of a Qt StyleSheet. */
        QCss::Parser qss_parser(qss_string);
        QCss::StyleSheet stylesheet;

        // If the parser returns true, the stylesheet is valid, and is applied, otherwise an error message is shown.
        if(qss_parser.parse(&stylesheet)) {
            setStyleSheet(qss_string);
            return true;
        } else {
            QMessageBox::warning(this, "QStyleSheet parsing failure.",
                "There was an error when trying to parse style.qss, verify that the QSS is valid, or delete the file to disable styling.");
            return false;
        }
    } else {
        return false;
    }
}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    LoadStylesheet();
    ui->setupUi(this);

    /* Pairs of character set occurance spinboxes, and their corresponding checkbox to enable/disable
     * said character set. These are pointers, not copies.  */
    charsetSelections_ = {
        {ui->spin_numericalSize, ui->check_numerical},
        {ui->spin_specialSize, ui->check_special},
        {ui->spin_lowerSize, ui->check_lower},
        {ui->spin_upperSize, ui->check_upper}
    };

    /* These connect the valueChanged signals from the spinboxes that count the amount of characters
     * per character set, to update the LCD that displays the total password length. */
    connect(ui->spin_numericalSize, SIGNAL(valueChanged(int)), SLOT(setLcdToSum_()));
    connect(ui->spin_specialSize, SIGNAL(valueChanged(int)), SLOT(setLcdToSum_()));
    connect(ui->spin_upperSize, SIGNAL(valueChanged(int)), SLOT(setLcdToSum_()));
    connect(ui->spin_lowerSize, SIGNAL(valueChanged(int)), SLOT(setLcdToSum_()));

    /* These connect the stateChanged signals from the checkboxes that enable a character set,
     * to update the LCD that displays the total password length. */
    connect(ui->check_numerical, SIGNAL(stateChanged(int)), SLOT(setLcdToSum_()));
    connect(ui->check_special, SIGNAL(stateChanged(int)), SLOT(setLcdToSum_()));
    connect(ui->check_upper, SIGNAL(stateChanged(int)), SLOT(setLcdToSum_()));
    connect(ui->check_lower, SIGNAL(stateChanged(int)), SLOT(setLcdToSum_()));
}

MainWindow::~MainWindow() {
    delete ui;
}
