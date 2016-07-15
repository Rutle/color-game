#ifndef DIALOG_HH
#define DIALOG_HH
#include "buttonswidget.hh"
#include <QDialog>
#include <QDebug>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog {
        Q_OBJECT

    public:
        explicit Dialog(QWidget *parent = 0);
        ~Dialog();

    private slots:
        void on_start_button_clicked();
        void game_stopped();
    private:
        Ui::Dialog *ui;
        ButtonsWidget *buttons_widget_;

};

#endif // DIALOG_HH
