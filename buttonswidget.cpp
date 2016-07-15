#include "buttonswidget.hh"
#include <QHBoxLayout>
#include <QtGlobal>
#include <QDebug>

#include <QDateTime>

ButtonsWidget::ButtonsWidget(QWidget *parent) : QWidget(parent), player_iter_{0},
    game_iter_{0}, correct_{0}{


    QVBoxLayout *base_layout{new QVBoxLayout(this)};
    QVBoxLayout *base_layout_status_labels{new QVBoxLayout()};
    QHBoxLayout *base_layout_small_labels{new QHBoxLayout()};

    base_layout_status_labels->setContentsMargins(0, 0, 0, 0);
    base_layout_status_labels->setMargin(0);
    base_layout_status_labels->setSpacing(0);
    QHBoxLayout *base_layout_big_buttons{new QHBoxLayout()};

    status_label_ = new QLabel(this);
    status_label_->setText("Start a game!");
    status_label_->setFixedHeight(30);
    status_label_->setAlignment(Qt::AlignCenter);
    status_label_->setObjectName("status_label");

    base_layout_status_labels->addWidget(status_label_);

    QWidget *temp_w = new QWidget();
    temp_w->setContentsMargins(0, 0, 0, 0);
    temp_w->setObjectName("status_bottom_shadow");
    temp_w->setFixedHeight(4);
    base_layout_status_labels->addWidget(temp_w);
    base_layout_status_labels->addStretch();


    yellow_label_small = new QLabel(this);
    yellow_label_small->setObjectName("yellow_label_small");
    yellow_label_small->setMinimumHeight(25);
    yellow_label_small->setFixedWidth(150);
    yellow_label_small->setText("Yellow");
    yellow_label_small->setAlignment(Qt::AlignCenter);

    green_label_small = new QLabel(this);
    green_label_small->setObjectName("green_label_small");
    green_label_small->setMinimumHeight(25);
    green_label_small->setFixedWidth(150);
    green_label_small->setText("Green");
    green_label_small->setAlignment(Qt::AlignCenter);

    red_label_small = new QLabel(this);
    red_label_small->setObjectName("red_label_small");
    red_label_small->setMinimumHeight(25);
    red_label_small->setFixedWidth(150);
    red_label_small->setText("Red");
    red_label_small->setAlignment(Qt::AlignCenter);

    blue_label_small = new QLabel(this);
    blue_label_small->setObjectName("blue_label_small");
    blue_label_small->setMinimumHeight(25);
    blue_label_small->setFixedWidth(150);
    blue_label_small->setText("Blue");
    blue_label_small->setAlignment(Qt::AlignCenter);

    base_layout_small_labels->addWidget(yellow_label_small);
    base_layout_small_labels->addWidget(green_label_small);
    base_layout_small_labels->addWidget(red_label_small);
    base_layout_small_labels->addWidget(blue_label_small);

    yellow_button = new QPushButton();
    yellow_button->setObjectName("yellow_button");
    yellow_button->setFixedSize(150,150);

    green_button = new QPushButton();
    green_button->setObjectName("green_button");
    green_button->setFixedSize(150,150);

    red_button = new QPushButton();
    red_button->setObjectName("red_button");
    red_button->setFixedSize(150,150);

    blue_button = new QPushButton();
    blue_button->setObjectName("blue_button");
    blue_button->setFixedSize(150,150);

    base_layout_big_buttons->addWidget(yellow_button);
    base_layout_big_buttons->addWidget(green_button);
    base_layout_big_buttons->addWidget(red_button);
    base_layout_big_buttons->addWidget(blue_button);

    base_layout->addLayout(base_layout_status_labels);
    base_layout->addLayout(base_layout_small_labels);
    base_layout->addLayout(base_layout_big_buttons);
    base_layout->setContentsMargins(0, 0, 0, 5);
    setLayout(base_layout);
    setObjectName("top_widget");

    game_timer_ = new QTimer(this);

    connect(game_timer_, SIGNAL(timeout()),
            this, SLOT(choose_random_color()));
    connect(this, SIGNAL(random_value(int)),
            this, SLOT(label_toggle(int)));

    label_highlight_timer_ = new QTimer(this);
    label_highlight_timer_->setSingleShot(true);
    connect(label_highlight_timer_, SIGNAL(timeout()), this, SLOT(toggle_small_label_off()));

    connect(yellow_button, SIGNAL(clicked(bool)), this, SLOT(on_yellow_button_clicked()));
    connect(green_button, SIGNAL(clicked(bool)), this, SLOT(on_green_button_clicked()));
    connect(red_button, SIGNAL(clicked(bool)), this, SLOT(on_red_button_clicked()));
    connect(blue_button, SIGNAL(clicked(bool)), this, SLOT(on_blue_button_clicked()));

    yellow_button->setDisabled(true);
    red_button->setDisabled(true);
    green_button->setDisabled(true);
    blue_button->setDisabled(true);

}

ButtonsWidget::~ButtonsWidget() {
    delete red_button;
    delete blue_button;
    delete green_button;
    delete yellow_button;

    delete red_label_small;
    delete blue_label_small;
    delete green_label_small;
    delete yellow_label_small;

    delete game_timer_;
    delete label_highlight_timer_;
}

void ButtonsWidget::start_game() {
    qDebug() << "Game started!";

    yellow_button->setDisabled(false);
    red_button->setDisabled(false);
    green_button->setDisabled(false);
    blue_button->setDisabled(false);
    status_label_->setText("Game has started! Correct clicks in row: [0]");
    game_timer_->start(2000);

}

void ButtonsWidget::choose_random_color() {
    int random_number = qrand() % 4;
    generate_new_time();
    numbers_.push_back(random_number);
    qDebug() << "game_iter: [" << game_iter_ << "]" << "numbers_.at(game_iter_) = [" << numbers_.at(game_iter_) << "]";
    game_iter_ += 1;
    emit random_value(random_number);

}

void ButtonsWidget::time_over() {
    qDebug() << "Timer over:";
    status_label_->setText("Game Over!");
}

void ButtonsWidget::on_red_button_clicked() {
    qDebug() << "player_iter: [" << player_iter_ << "]" << "numbers_.at(player_iter_) = ["<< numbers_.at(player_iter_) << "]";
    if ( numbers_.at(player_iter_) == RedLabel ) {
        //qDebug() << "Correct number: [2] == [" << numbers_.at(player_iter_) << "]";
        player_iter_ += 1;
        correct_ += 1;
        status_label_->setText(QString("Correct clicks in row: %1").arg(QString::number(correct_)));
        //qDebug() << "Correct: " << correct_;
    } else {
        qDebug() << "Wrong color! Game Over! Correct choice was: [" << numbers_.at(player_iter_) << "]";
        stop_game();
    }
}

void ButtonsWidget::on_blue_button_clicked() {
    qDebug() << "player_iter: [" << player_iter_ << "]" << "numbers_.at(player_iter_) = ["<< numbers_.at(player_iter_) << "]";
    if ( numbers_.at(player_iter_) == BlueLabel ) {
        //qDebug() << "Correct number: [3] == [" << numbers_.at(player_iter_) << "]";
        player_iter_ += 1;
        correct_ += 1;
        status_label_->setText(QString("Correct clicks in row: %1").arg(QString::number(correct_)));
        //qDebug() << "Correct: " << correct_;
    } else {
        qDebug() << "Wrong color! Game Over! Correct choice was: [" << numbers_.at(player_iter_) << "]";
        stop_game();
    }

}

void ButtonsWidget::on_green_button_clicked() {
    qDebug() << "player_iter: [" << player_iter_ << "]" << "numbers_.at(player_iter_) = ["<< numbers_.at(player_iter_) << "]";
    if ( numbers_.at(player_iter_) == GreenLabel ) {
        //qDebug() << "Correct number: [1] == [" << numbers_.at(player_iter_) << "]";
        player_iter_ += 1;
        correct_ += 1;
        status_label_->setText(QString("Correct clicks in row: %1").arg(QString::number(correct_)));
        //qDebug() << "Correct: " << correct_;
    } else {
        qDebug() << "Wrong color! Game Over! Correct choice was: [" << numbers_.at(player_iter_) << "]";
        stop_game();
    }

}

void ButtonsWidget::on_yellow_button_clicked() {
    qDebug() << "player_iter: [" << player_iter_ << "]" << "numbers_.at(player_iter_) = ["<< numbers_.at(player_iter_) << "]";
    if ( numbers_.at(player_iter_) == YellowLabel ) {
        //qDebug() << "Correct number: [0] == [" << numbers_.at(player_iter_) << "]";
        player_iter_ += 1;
        correct_ += 1;
        status_label_->setText(QString("Correct clicks in row: %1").arg(QString::number(correct_)));
        //qDebug() << "Correct: " << correct_;
    } else {
        qDebug() << "Wrong color! Game Over! Correct choice was: [" << numbers_.at(player_iter_) << "]";
        stop_game();
    }

}

void ButtonsWidget::label_toggle(int random_number) {
    switch(random_number) {
        case YellowLabel:
            //qDebug() << "Case 0 (Yellow) ON:" << random_number;
            yellow_label_small->setStyleSheet(
                        QString("QLabel#yellow_label_small {"
                                "background-color:rgb(244, 255, 33);"
                                "border-style: outset;"
                                "border-width: 1px;"
                                "border-radius: 2px;"
                                "border-color: rgb(69, 71, 9); }"));
            label_highlight_timer_->start(150);
            break;
        case GreenLabel:
            //qDebug() << "Case 1 (Green) ON:" << random_number;
            green_label_small->setStyleSheet(
                        QString("QLabel#green_label_small {"
                                "background-color:rgb(44, 170, 113);"
                                "border-style: outset;"
                                "border-width: 1px;"
                                "border-radius: 2px;"
                                "border-color: rgb(18, 71, 46);}"));
            label_highlight_timer_->start(150);
            break;
        case RedLabel:
            //qDebug() << "Case 2 (Red) ON:" << random_number;
            red_label_small->setStyleSheet(
                        QString("QLabel#red_label_small {"
                               "background-color:rgb(255, 64, 64);"
                               "border-style: outset;"
                               "border-width: 1px;"
                               "border-radius: 2px;"
                               "border-color: rgb(117, 29, 29); }"));
            label_highlight_timer_->start(150);
            break;
        case BlueLabel:
            //qDebug() << "Case 3 (Blue) ON:" << random_number;
            blue_label_small->setStyleSheet(
                        QString("QLabel#blue_label_small {"
                                "background-color:rgb(60, 154, 255);"
                                "border-style: outset;"
                                "border-width: 1px;"
                                "border-radius: 2px;"
                                "border-color: rgb(37, 96, 159); }"));
            label_highlight_timer_->start(150);
            break;
    }
}

void ButtonsWidget::toggle_small_label_off() {
    // qDebug() << "Toggle small label off at [" << numbers_.at(game_iter_ - 1) << "]";
    int number{numbers_.at(game_iter_ - 1)};
    switch(number) {
        case YellowLabel:
            //qDebug() << "Case 0 (Yellow) OFF:" << number;
            yellow_label_small->setStyleSheet("");
            break;
        case GreenLabel:
            //qDebug() << "Case 1 (Green) OFF:" << number;
            green_label_small->setStyleSheet("");
            break;
        case RedLabel:
            //qDebug() << "Case 2 (Red) OFF:" << number;
            red_label_small->setStyleSheet("");
            break;
        case BlueLabel:
            //qDebug() << "Case 3 (Blue) OFF:" << number;
            blue_label_small->setStyleSheet("");
            break;
    }

}

void ButtonsWidget::generate_new_time() {
    int low{300};
    int high{1200};
    int random_number = (qrand() % ((high + 1) - low) + low);
    qDebug() << random_number;

}

void ButtonsWidget::stop_game() {
    qsrand( QDateTime::currentDateTime().toTime_t() );
    game_timer_->stop();
    status_label_->setText(QString("Game over! Correct clicks in row: %1").arg(QString::number(correct_)));
    correct_ = 0;
    player_iter_ = 0;
    game_iter_ = 0;
    emit game_has_stopped();
}

