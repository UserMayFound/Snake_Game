#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include <QMainWindow>
#include <QWidget>
#include <QKeyEvent>
#include <QTimer>
#include <QPainter>
#include <QRandomGenerator>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class Snake_game;
}
QT_END_NAMESPACE

class Snake_game : public QMainWindow
{

    Q_OBJECT

public:
    Snake_game(QWidget *parent = nullptr);
    ~Snake_game();
    Ui::Snake_game *ui;

    //Our movement decider
    void Movement(QKeyEvent *Key_pressed);

    //Functions related with fruit
    void fruit_rng();
    void fruit_place_changer();
    void place_gen();
    void checkpos();

    //Point function
    void point();

    //Our gameloop and the function to give us our end score
    void gameloop();
    void finish_game();

protected:
    void keyPressEvent(QKeyEvent *Key_pressed) override;
    void paintEvent(QPaintEvent* event) override;

private:

    //Timers
    QTimer *gameTimer;
    QTimer *controlTimer;

    //The direction types and the direction our snake will go
    enum Direction {
        up,
        down,
        left,
        right
    };
    Direction direction;

    //Fruit types to get different fruits
    enum fruits{
        apple,
        peach,
        orange
    };
    fruits currentFruit;

    //Used to check whether we are still alive
    bool Game_on;

    //Our snakes basespeed
    int basespeed;

    //Our scores
    int score;
    int peach_number;
    int orange_number;
    int apple_number;

    //The pixel each fruit and part of snake will take
    int pixels_taken=40;

    //The size of our window
    int width_of_window=960;
    int height_of_window=720;

    //The size of our snake and where each part is at
    int size_of_snake;
    int snakeX[128];
    int snakeY[128];

    //Co-ordinates of the fruit
    int fruitX;
    int fruitY;
};
#endif // SNAKE_GAME_H
