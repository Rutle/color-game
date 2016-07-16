#ifndef BUTTONSWIDGET_HH
#define BUTTONSWIDGET_HH

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QTimer>
#include <QLabel>
#include <QPair>

class ButtonsWidget : public QWidget {
        Q_OBJECT
    public:
        enum {Yellow, Green, Red, Blue};
        explicit ButtonsWidget(QWidget *parent = 0);
        ~ButtonsWidget();
        void start_game();
    signals:
        void random_value(int value);
        void game_has_stopped();
    public slots:
        void choose_random_color();
        void time_over();
        void on_red_button_clicked();
        void on_blue_button_clicked();
        void on_green_button_clicked();
        void on_yellow_button_clicked();
        void button_toggle_on(int number);
        void toggle_button_off();
        void button_clicked(int number);
    private:

        void generate_new_time();
        void stop_game();
        void set_stylesheets();
        void create_buttons();

        QLabel *red_label_small;
        QLabel *blue_label_small;
        QLabel *green_label_small;
        QLabel *yellow_label_small;

        QLabel *status_label_;

        QTimer *game_timer_;
        QTimer *button_highlight_timer_;

        int player_iter_;
        int game_iter_;
        int correct_;
        QList<int> numbers_;
        QList<QPushButton *> buttons_;
};

#endif // BUTTONSWIDGET_HH
