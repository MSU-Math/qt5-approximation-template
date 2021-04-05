#include <QPainter>
#include <stdio.h>

#include "window.h"

#define SIZE_MIN 100
#define SIZE_DEFAULT 1000
#define FIRST_LINE 20

Window::Window(QWidget *parent) : QWidget(parent) {}

QSize Window::minimumSizeHint() const
{
    return QSize(SIZE_MIN, SIZE_MIN);
}

QSize Window::sizeHint() const
{
    return QSize(SIZE_DEFAULT, SIZE_DEFAULT);
}

/// render graph
void Window::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    if (this->conv.size() > 0) {
        auto area = QString("Area = %1").arg(this->conv.area(), 0, 'f', 3);
        painter.drawText(FIRST_LINE, FIRST_LINE, area);

        auto perimeter =
            QString("Perimeter = %1").arg(this->conv.perimeter(), 0, 'f', 3);
        painter.drawText(FIRST_LINE, 2 * FIRST_LINE, perimeter);

        QPen pen("blue");
        pen.setWidth(1); // try 0 if lines are too wide
        painter.setPen(pen);

        auto b = this->conv.begin();
        auto e = this->conv.end();
        auto i = b;
        auto j = b;
        ++j;

        while (i != e) {
            R2Point v0 = *i;
            R2Point v1;

            if (j != e) {
                v1 = *j;
            } else {
                v1 = *b;
            }

            painter.drawLine(v0.x, v0.y, v1.x, v1.y);
            i = j;
            ++j;
        }
    } else {
        painter.drawText(FIRST_LINE, FIRST_LINE, "Add points by mouse click.");
        painter.drawText(FIRST_LINE, 2 * FIRST_LINE,
                         "Initialize by middle button.");
    }
}

void Window::mouseReleaseEvent(QMouseEvent *event)
{
    auto button = event->button();
    if (button == Qt::LeftButton) {
        conv.addPoint(R2Point(event->x(), event->y()));
    } else if (button == Qt::MiddleButton) {
        conv.initialize();
    } else if (button == Qt::RightButton) {
        // TODO
        // throw "Not implemented!";
    }
    this->update();
}
