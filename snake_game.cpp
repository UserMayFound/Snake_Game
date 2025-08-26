#include "snake_game.h"
#include <QPainter>
#include <QDebug>
#include <QMessageBox>
#include <QRandomGenerator>
#include "ui_snake_game.h"
#include <QTimer>

Snake_game::Snake_game(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::Snake_game)
{
    ui->setupUi(this);

    //Changing the name and size of the window
    setWindowTitle("Snake Game");
    setFixedSize(width_of_window, height_of_window);

    //Initializing the game
    size_of_snake = 3;
    basespeed = 180;
    Game_on = true;
    direction = up;

    //Clearing the scoreboard
    score = 0;
    peach_number = 0;
    orange_number = 0;
    apple_number = 0;

    //Puting our snake and its body to middle of the window
    for(int i = 0; i < size_of_snake; i++) {
        snakeX[i] = width_of_window / 2 - i * pixels_taken;
        snakeY[i] = height_of_window / 2;
    }

    //generates the first fruit
    place_gen();

    //Here we have made a timer that will call our gameloop
    //The time it uses to call out gameloop will be our snakes speed
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &Snake_game::gameloop);
    gameTimer->start(basespeed);

    //This contoltimer will check every 250ms if the fruit is in a valid position
    controlTimer = new QTimer(this);
    connect(controlTimer, &QTimer::timeout, this, &Snake_game::checkpos);
    controlTimer->start(250);

}

Snake_game::~Snake_game()
{
    delete ui;
    delete gameTimer;
    delete controlTimer;
}

//When we press a key it will come here to Movement
void Snake_game::keyPressEvent(QKeyEvent *event){
    Movement(event);
}

//This function is used to change the direction of snake
void Snake_game::Movement(QKeyEvent *Key_pressed){
    switch(Key_pressed->key()){

        case Qt::Key_Up:
        case Qt::Key_W:
            if(direction!=down)//If you are going oposite you cannot turn 180, you would eat yourself
                direction=up;
            break;

        case Qt::Key_Right:
        case Qt::Key_D:
            if(direction!=left)
                direction=right;
            break;

        case Qt::Key_Down:
        case Qt::Key_S:
            if(direction!=up)
                direction=down;
            break;

        case Qt::Key_Left:
        case Qt::Key_A:
            if(direction!=right)
                direction=left;
            break;
    }
}

//This function chooses our fruit with %15 peach,%25 orange and %60 apple
void Snake_game::fruit_rng(){

    int percentage=(rand()%100)+1;
    if(percentage>85){
        currentFruit=peach;
    }
    else if(percentage>60){
        currentFruit=orange;
    }
    else{
        currentFruit=apple;
    }
}

//We will change the fruit type and put it on the map
void Snake_game::place_gen(){

    fruit_rng();

    fruit_place_changer();

    checkpos();
}

//Paints
void Snake_game::paintEvent(QPaintEvent* event){
    QPainter painter(this);

    //Background
    painter.fillRect(rect(), QColor(144, 238, 144));  // light green

    //Snake
    painter.setBrush(Qt::darkGreen);//head
    painter.drawRect(snakeX[0], snakeY[0], pixels_taken, pixels_taken);

    painter.setBrush(Qt::yellow);//tail
    painter.drawRect(snakeX[size_of_snake - 1], snakeY[size_of_snake - 1], pixels_taken, pixels_taken);

    painter.setBrush(Qt::green);//body
    for(int i = 1; i < size_of_snake - 1; i++) {
        painter.drawRect(snakeX[i], snakeY[i], pixels_taken, pixels_taken);
    }

    //Fruits
    switch(currentFruit) {
    case apple:
        painter.setBrush(Qt::red);
        break;
    case peach:
        painter.setBrush(QColor(255, 218, 185));
        break;
    case orange:
        painter.setBrush(QColor(255, 165, 0));
        break;
    }
    painter.drawRect(fruitX, fruitY, pixels_taken, pixels_taken);

    //Score
    painter.setPen(Qt::black);
    painter.setFont(QFont("Outfit", 16));
    painter.drawText(20, 30, QString("Score: %1").arg(score));
}

//Point function that will change the score, speed and the size of our snake
void Snake_game::point(){

    //Increase the score and number of fruits eaten
    switch(currentFruit){
        case apple: {
            apple_number++;
            score++;
            break;
        }
        case orange: {
            orange_number++;
            score += 2;
            break;
        }
        case peach: {
            peach_number++;
            score += 3;
            break;
        }
    }

        //Create a new body part and increase the size variable
        snakeX[size_of_snake] = snakeX[size_of_snake - 1];
        snakeY[size_of_snake] = snakeY[size_of_snake - 1];
        size_of_snake++;

        //Change speed since our score changed
        int speed = basespeed - (score / 3);
        if (speed < 50)
            speed = 50;
        gameTimer->start(speed);
}

//Gameloop will work while game is still on
void Snake_game::gameloop(){
    if (!Game_on)
        return;

    for(int i = size_of_snake - 1; i > 0; i--) {
        snakeX[i] = snakeX[i-1];
        snakeY[i] = snakeY[i-1];
    }

    //moving the snake depending on snake
    switch(direction){
    case up:
        snakeY[0] -= pixels_taken;
        break;
    case down:
        snakeY[0] += pixels_taken;
        break;
    case left:
        snakeX[0] -= pixels_taken;
        break;
    case right:
        snakeX[0] += pixels_taken;
        break;
    }

    //Check if snake hit the wall
    if(snakeX[0] < 0 || snakeY[0] < 0 || snakeX[0] >= width_of_window || snakeY[0] >= height_of_window){
        finish_game();
        return;
    }

    //Check if snake hit itself
    for(int i=1;i<size_of_snake;i++){
        if(snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]){
            finish_game();
            return;
        }
    }

    //Check if snake eat the fruit
    if(snakeX[0] == fruitX && snakeY[0] == fruitY){
        point();
        place_gen();
    }

    //We update the gui
    update();
}

//Check if the position of fruit is suitable;
void Snake_game::checkpos(){
    bool suitable_pos=false;
    int tries=0;

    //If the position is not suitable we try again untill 100 tries
    while(!suitable_pos && tries<100){
        suitable_pos=true;

        for(int i=0;i<size_of_snake;i++){
            if(fruitX==snakeX[i]&&fruitY==snakeY[i]){

                fruit_place_changer();

                suitable_pos=false;
                tries++;
                break;
            }
        }
    }
    return;
}

//This function finishes the game and gives the score
void Snake_game::finish_game(){
    Game_on = false;
    gameTimer->stop();
    controlTimer->stop();
    QMessageBox::information(this, "Game Over", QString("Score: %1 \nApples: %2\nOranges: %3\nPeaches: %4")
                                                    .arg(score).arg(apple_number).arg(orange_number).arg(peach_number));
}

//This function changes the fruits place on the map
void Snake_game::fruit_place_changer(){
    fruitX=(rand()%(width_of_window/pixels_taken))*pixels_taken;
    fruitY=(rand()%(height_of_window/pixels_taken))*pixels_taken;
}
