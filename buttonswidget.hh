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
        void button_toggle_on(int number);
        void toggle_button_off();
        void button_clicked(int number);
    private:

        void generate_new_time();
        void stop_game();
        void set_stylesheets();
        void create_buttons();

        QLabel *status_label_;

        QTimer *game_timer_;
        QTimer *button_highlight_timer_;

        int player_turn_;
        int game_turn_;
        int correct_;
        QList<int> numbers_;
        QList<QPushButton *> buttons_;
};

#endif // BUTTONSWIDGET_HH
