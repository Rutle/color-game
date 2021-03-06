#include "buttonswidget.hh"
#include <QHBoxLayout>
#include <QtGlobal>
#include <QDebug>
#include <QMessageBox>
#include <QDateTime>
#include <QFile>


const QList<QString> COLORS{{"Yellow", "Green", "Red", "Blue"}};
const QList<QString> COLORS_DARK{{
        "QPushButton#Yellow {background-color:rgb(94, 97, 12);}",
        "QPushButton#Green {background-color:rgb(21, 81, 53);}",
        "QPushButton#Red {background-color:rgb(83, 20, 20);}",
        "QPushButton#Blue {background-color:rgb(26, 68, 113);}"}};
const QList<QString> COLORS_LIGHT{{
        "QPushButton#Yellow {background-color:rgb(244, 255, 33);}",
        "QPushButton#Green {background-color:rgb(44, 170, 113);}",
        "QPushButton#Red {background-color:rgb(255, 64, 64);}",
        "QPushButton#Blue {background-color:rgb(60, 154, 255);}"}};

ButtonsWidget::ButtonsWidget(QWidget *parent) :
    QWidget(parent), player_turn_{0},
    game_turn_{0}, correct_{0}{


    QVBoxLayout *base_layout{new QVBoxLayout(this)};
    QVBoxLayout *base_layout_status_labels{new QVBoxLayout()};

    base_layout_status_labels->setContentsMargins(0, 0, 0, 0);
    base_layout_status_labels->setMargin(0);
    base_layout_status_labels->setSpacing(0);
    QHBoxLayout *base_layout_big_buttons{new QHBoxLayout()};

    status_label_ = new QLabel(this);
    status_label_->setText("Start a game!");
    status_label_->setFixedHeight(30);
    status_label_->setAlignment(Qt::AlignCenter);
    status_label_->setObjectName("status_label");
    QFont font{status_label_->font()};
    font.setBold(true);
    font.setPixelSize(12);
    status_label_->setFont(font);

    base_layout_status_labels->addWidget(status_label_);

    QWidget *temp_w = new QWidget();
    temp_w->setContentsMargins(0, 0, 0, 0);
    temp_w->setObjectName("status_bottom_shadow");
    temp_w->setFixedHeight(4);
    base_layout_status_labels->addWidget(temp_w);
    base_layout_status_labels->addStretch();

    create_buttons();
    set_stylesheets();

    for ( auto button : buttons_ ) {
        base_layout_big_buttons->addWidget(button);
        button->setDisabled(true);
        button->setFocusPolicy(Qt::NoFocus);
    }

    base_layout->addLayout(base_layout_status_labels);
    base_layout->addLayout(base_layout_big_buttons);
    base_layout->setContentsMargins(0, 0, 0, 5);
    setLayout(base_layout);
    setObjectName("top_widget");

    game_timer_ = new QTimer(this);

    connect(game_timer_, SIGNAL(timeout()),
            this, SLOT(choose_random_color()));
    connect(this, SIGNAL(random_value(int)),
            this, SLOT(button_toggle_on(int)));

    button_highlight_timer_ = new QTimer(this);
    button_highlight_timer_->setSingleShot(true);
    connect(button_highlight_timer_, SIGNAL(timeout()), this, SLOT(toggle_button_off()));

    int counter{0};
    while ( counter < buttons_.size() ) {
        // Lambda so we can find the correct button that was clicked:
        connect(buttons_.at(counter), &QPushButton::clicked,
                [this, counter]() { button_clicked(counter); });
        ++counter;
    }

}

ButtonsWidget::~ButtonsWidget() {

    delete game_timer_;
    delete button_highlight_timer_;
    for ( auto button : buttons_ ) {
        delete button;
    }
}

void ButtonsWidget::start_game() {
    //qDebug() << "Game started!";
    for ( auto button : buttons_ ) {
        button->setDisabled(false);
    }
    status_label_->setText("Game has started! Correct clicks in row: [0]");
    game_timer_->start(2000);

}

void ButtonsWidget::choose_random_color() {
    int random_number = qrand() % 4;
    generate_new_time();
    numbers_.push_back(random_number);
    //qDebug() << "game_iter: [" << game_turn_ << "]" << "numbers_.at(game_turn_) = [" << numbers_.at(game_turn_) << "]";
    game_turn_ += 1;
    emit random_value(random_number);

}

void ButtonsWidget::button_toggle_on(int random_number) {

    buttons_.at(random_number)->setStyleSheet(COLORS_LIGHT.at(random_number));
    button_highlight_timer_->start(150);
}

void ButtonsWidget::toggle_button_off() {
    int number{numbers_.at(game_turn_ - 1)};
    buttons_.at(number)->setStyleSheet(COLORS_DARK.at(number));
}

void ButtonsWidget::button_clicked(int number) {
    qDebug() << "player_iter: [" << player_turn_ << "]" << "numbers_.at(player_turn_) = ["<< numbers_.at(player_turn_) << "]";
    if ( numbers_.at(player_turn_) == number ) {
        //qDebug() << "Correct number: [0] == [" << numbers_.at(player_turn_) << "]";
        player_turn_ += 1;
        correct_ += 1;
        status_label_->setText(QString("Correct clicks in row: %1").arg(QString::number(correct_)));
    } else {
        //qDebug() << "Wrong color! Game Over! Correct choice was: [" << numbers_.at(player_turn_) << "]";
        stop_game();
    }
}

void ButtonsWidget::save_highscore() const {

    QFile highscore_file("highscore.txt");
    if (!highscore_file.open(QIODevice::WriteOnly | QFile::Truncate)) {
        //qWarning("Couldn't open highscore file.");
        //return false;
    }

    QTextStream out(&highscore_file);
    QList<QString>::size_type hs_size{0};
    while ( hs_size < highscore_.size() ) {
        out << highscore_.at(hs_size) << "\n";
        qDebug() << out.status();
        ++hs_size;
    }

    highscore_file.close();
    //return true;


}

void ButtonsWidget::load_highscore() {

    QFile highscore_file("highscore.txt");
    if ( !highscore_file.open(QIODevice::ReadOnly) ) {
        //qWarning("Couldn't open highscore file.");
        //return false;
    }
    QTextStream in(&highscore_file);
    QString line;


    while ( in.readLineInto(&line) ) {
        highscore_.push_back(line);
        qDebug() << line << " size: " << highscore_.size();
    }
    highscore_file.close();
    //return true;
}

void ButtonsWidget::generate_new_time() {
    int low{300};
    int high{1200};
    int random_number = (qrand() % ((high + 1) - low) + low);
    //qDebug() << random_number;

}

void ButtonsWidget::stop_game() {
    qsrand( QDateTime::currentDateTime().toTime_t() );
    game_timer_->stop();
    button_highlight_timer_->stop();
    status_label_->setText(QString("Game over! Correct clicks in row: %1").arg(QString::number(correct_)));
    QMessageBox result;
    result.setText(QString("Game Over! You managed to get %1 correct clicks!").arg(correct_));
    result.setInformativeText("Would like to save highscore?");
    result.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    result.setDefaultButton(QMessageBox::Save);
    int ret = result.exec();
    qDebug() << ret;
    switch (ret) {
      case QMessageBox::Save:
          // Save was clicked
          break;
      case QMessageBox::Discard:
          // Don't Save was clicked
          break;
      case QMessageBox::Cancel:
          // Cancel was clicked
          break;
      default:
          // should never be reached
          break;
    }
    correct_ = 0;
    numbers_.clear();
    player_turn_ = 0;
    game_turn_ = 0;
    emit game_has_stopped();
    //qDebug() << "loppu";
}

void ButtonsWidget::set_stylesheets() {
    // enum {YellowLabel, GreenLabel, RedLabel, BlueLabel};
    QList<QString>::size_type button_counter{0};
    while ( button_counter < buttons_.size() ) {
        buttons_.at(button_counter)->setStyleSheet(COLORS_DARK.at(button_counter));
        ++button_counter;
    }

}

void ButtonsWidget::create_buttons() {
    for ( auto color : COLORS ) {
        QPushButton *temp_button{new QPushButton()};
        temp_button->setObjectName(color);
        temp_button->setFixedSize(150, 150);
        buttons_.push_back(temp_button);
    }

}

