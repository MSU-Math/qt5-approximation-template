#ifndef WINDOW_H
#define WINDOW_H

#include <QMouseEvent>
#include <QWidget>

#include "R2Conv.h"

class Window : public QWidget
{
    Q_OBJECT

  private:
    R2Convex conv;

  public:
    Window(QWidget *parent);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

  protected:
    void paintEvent(QPaintEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif
