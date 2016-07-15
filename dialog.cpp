#include "dialog.hh"
#include "ui_dialog.h"



Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog) {
    ui->setupUi(this);

    ButtonsWidget *widget{new ButtonsWidget(this)};
    buttons_widget_ = widget;
    ui->top_grid->addWidget(widget);
    connect(buttons_widget_, SIGNAL(game_has_stopped()), this, SLOT(game_stopped()));

}

Dialog::~Dialog() {
    delete ui;

}


void Dialog::on_start_button_clicked() {
    buttons_widget_->start_game();
    ui->start_button->setDisabled(true);
}

void Dialog::game_stopped() {
    ui->start_button->setDisabled(false);

}
