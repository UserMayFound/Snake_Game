#include "snake_game.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Snake_game w;
    w.show();
    return a.exec();
}
