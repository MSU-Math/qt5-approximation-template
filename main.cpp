
#include <QApplication>
#include <QMainWindow>

#include "window.h"

#define TOOLBAR_HEIGHT 30

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow *window = new QMainWindow;
    Window *graph_area = new Window(window);

    window->setCentralWidget(graph_area);
    window->setWindowTitle("R2Conv");

    window->show();
    return QApplication::exec();
}
