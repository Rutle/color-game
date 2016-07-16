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
        void label_toggle(int number);
        void toggle_small_label_off();
        void button_clicked(int number);
    private:

        void generate_new_time();
        void stop_game();
        void set_stylesheets();
        void set_buttons();
        QPushButton *red_button;
        QPushButton *blue_button;
        QPushButton *green_button;
        QPushButton *yellow_button;

        QLabel *red_label_small;
        QLabel *blue_label_small;
        QLabel *green_label_small;
        QLabel *yellow_label_small;

        QLabel *status_label_;

        QTimer *game_timer_;
        QTimer *player_press_timer_;
        QTimer *label_highlight_timer_;

        int player_iter_;
        int game_iter_;
        int correct_;
        QList<int> numbers_;
        QList<QPushButton *> buttons_;
};

#endif // BUTTONSWIDGET_HH
