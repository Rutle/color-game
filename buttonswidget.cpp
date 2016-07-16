#include "buttonswidget.hh"
#include <QHBoxLayout>
#include <QtGlobal>
#include <QDebug>

#include <QDateTime>

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
    QWidget(parent), player_iter_{0},
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

    create_buttons();
    set_stylesheets();

    for ( auto button : buttons_ ) {
        base_layout_big_buttons->addWidget(button);
        button->setDisabled(true);
    }

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
            this, SLOT(button_toggle_on(int)));

    button_highlight_timer_ = new QTimer(this);
    button_highlight_timer_->setSingleShot(true);
    connect(button_highlight_timer_, SIGNAL(timeout()), this, SLOT(toggle_button_off()));

    int counter{0};
    while ( counter < buttons_.size() ) {
        // Lambda so we can find the correct button:
        connect(buttons_.at(counter), &QPushButton::clicked,
                [this, counter]() { button_clicked(counter); });
        ++counter;
    }

}

ButtonsWidget::~ButtonsWidget() {

    delete red_label_small;
    delete blue_label_small;
    delete green_label_small;
    delete yellow_label_small;

    delete game_timer_;
    delete button_highlight_timer_;
    for ( auto button : buttons_ ) {
        delete button;
    }
}

void ButtonsWidget::start_game() {
    qDebug() << "Game started!";
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
    qDebug() << "game_iter: [" << game_iter_ << "]" << "numbers_.at(game_iter_) = [" << numbers_.at(game_iter_) << "]";
    game_iter_ += 1;
    emit random_value(random_number);

}

void ButtonsWidget::time_over() {
    qDebug() << "Timer over:";
    status_label_->setText("Game Over!");
}


void ButtonsWidget::button_toggle_on(int random_number) {

    buttons_.at(random_number)->setStyleSheet(COLORS_LIGHT.at(random_number));
    button_highlight_timer_->start(150);
}

void ButtonsWidget::toggle_button_off() {
    int number{numbers_.at(game_iter_ - 1)};
    buttons_.at(number)->setStyleSheet(COLORS_DARK.at(number));
}

void ButtonsWidget::button_clicked(int number) {
    qDebug() << "player_iter: [" << player_iter_ << "]" << "numbers_.at(player_iter_) = ["<< numbers_.at(player_iter_) << "]";
    if ( numbers_.at(player_iter_) == number ) {
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
    /*
    yellow_button = new QPushButton();
    yellow_button->setObjectName("Yellow");
    yellow_button->setFixedSize(150,150);

    green_button = new QPushButton();
    green_button->setObjectName("Green");
    green_button->setFixedSize(150,150);

    red_button = new QPushButton();
    red_button->setObjectName("Red");
    red_button->setFixedSize(150,150);

    blue_button = new QPushButton();
    blue_button->setObjectName("Blue");
    blue_button->setFixedSize(150,150);

    buttons_.push_back(yellow_button);
    buttons_.push_back(green_button);
    buttons_.push_back(red_button);
    buttons_.push_back(blue_button);
    */
}

