#include <QPainter>
#include <stdio.h>

#include "cmath"
#include "window.h"

#define DEFAULT_A -10
#define DEFAULT_B 10
#define DEFAULT_N 10

static double f_0(double x, int t)
{
    switch (t) {
    case 1:
    case 2:
        return 0;
        break;
    default:
        return (x == x);

    }

}

static double f_1(double x, int t)
{
    switch (t) {
    case 1:
        return 1;
        break;
    case 2:
        return 0;
        break;
    default:
        return x;

    }

}

static double f_2(double x, int t)
{
    switch (t) {
    case 1:
        return 2 * x;
        break;
    case 2:
        return 2;
        break;
    default:
        return x * x;

    }
}

static double f_3(double x, int t)
{
    switch (t) {
    case 1:
        return 3 * x * x;
        break;
    case 2:
        return 6 * x;
        break;
    default:
        return x * x * x;

    }
}
static double f_4(double x, int t)
{
    switch (t) {
    case 1:
        return 4 * x * x * x;
        break;
    case 2:
        return 12 * x * x;
        break;
    default:
        return x * x * x * x;

    }
}
static double f_5(double x, int t)
{
    switch (t) {
    case 1:
    case 2:
    default:
        return exp(x);

    }
}
static double f_6(double x, int t)
{
    switch (t) {
    case 1:
        return -50 * x / ((25 * x * x + 1) * (25 * x * x + 1));
        break;
    case 2:
        return 50 * (75 * x * x - 1) / ((25 * x * x + 1) * (25 * x * x + 1) * (25 * x * x + 1));
        break;
    default:
        return 1 / (25 * x * x + 1);

    }

}

void Window::mem()
{
    y = (double *)malloc(n * sizeof(double));
    fy = (double *)malloc(n * sizeof(double));
    c = (double *)malloc((n + 1) * 4 * sizeof(double));
    ks = (double *)malloc((n + 1) * sizeof(double));
    d = (double *)malloc((n + 1) * sizeof(double));
}

Window::Window(char *argv[], QWidget *parent) : QWidget(parent)
{
    a = DEFAULT_A;
    b = DEFAULT_B;
    if (sscanf(argv[4], "%d", &func_id) != 1) {
        func_id = 0;
    }
    func_id--;
    n = atoi(argv[3]);
    mem();
    z = 0.0;
    q = 0;
    s = 0;
    m = 0;
    change_func();
}
Window::~Window()
{
    del();
}
void Window::del()
{
    free(y);
    free(fy);
    free(c);
    free(d);
    free(ks);
}
QSize Window::minimumSizeHint() const
{
    return QSize(100, 100);
}

QSize Window::sizeHint() const
{
    return QSize(1000, 1000);
}

int Window::parse_command_line(int argc, char *argv[])
{
    if (argc != 5) {
        return -1;
    }
    if (sscanf(argv[1], "%lf", &a) != 1 || sscanf(argv[2], "%lf", &b) != 1 || b - a < 1.e-6 || sscanf(argv[3], "%d", &n) != 1 ||
        sscanf(argv[4], "%d", &func_id) != 1 || n <= 0 || func_id < 0 || func_id > 6) {
        return -2;
    }
    p = 0;
    return 0;
}

void Window::size_plus()
{
    if (max_funk(2) > 1.e+20) {
        return;
    } else {
        a *= 2;
        b *= 2;
        p = p != 0 ? p - 1 : p;
        s++;
        change_metod();
    }
}
void Window::size_minus()
{
    if (s < -22) {
        return;
    } else {
        a /= 2;
        b /= 2;
        s--;
        p = p != 0 ? p - 1 : p;
        change_metod();
    }
}
void Window::n_plus()
{
    del();
    n *= 2;
    mem();
    p = p != 0 ? p - 1 : p;
    change_metod();
}
void Window::n_minus()
{
    if (n <= 5) {
        return;
    } else {
        del();
        n /= 2;
        mem();
        p = p != 0 ? p - 1 : p;
        change_metod();
    }
}
double Window::max_funk(int t)
{
    double x, y;
    double max_y;
    double delta_x;
    delta_x = (b - a) / n;
    max_y = 0;
    for (x = a; x - b < 1.e-6; x += delta_x) {
        y = f(x, 0);
        max_y = y > max_y ? y : max_y;
    }
    z = t ? t == 1 ? z + 0.1 * max_y : z : z - 0.1 * max_y;
    return max_y;
}
void Window::f_plus()
{
    max_funk(1);
    p = p != 0 ? p - 1 : p;
    q = 1;
    change_metod();
}
void Window::f_minus()
{
    max_funk(0);
    p = p != 0 ? p - 1 : p;
    q = 1;
    change_metod();
}

void Window::CoefSet24()
{
    for (int i = 0; i < n - 1; i++) {
        c[i * 4] = fy[i];
        c[i * 4 + 2] = (3 * (fy[i + 1] - fy[i]) / (y[i + 1] - y[i])
                - 2 * c[i * 4 + 1] - c[(i + 1) * 4 + 1]) / (y[i + 1] - y[i]);
        c[i * 4 + 3] = (c[i * 4 + 1] + c[(i + 1) * 4 + 1] - 2 * (fy[i + 1] - fy[i])
                / (y[i + 1] - y[i])) / ((y[i + 1] - y[i]) * (y[i + 1] - y[i]));
    }
}

void Window::BorderCond(int t)
{
    if (t == 1) {
        c[4 * 0 + 1] = f(y[0], 1);
        c[(n - 1) * 4 + 1] = f(y[n - 1], 1);
        c[4 * 1 + 1] = 3.0 * (fy[1] - fy[0]) / (y[1] - y[0])
                - 0.5 * f(y[0], 2) * (y[1] - y[0]) - 2.0 * c[4 * 0 + 1];
        c[4 * (n - 2) + 1] = 3.0 * (fy[n - 1] - fy[n - 2]) / (y[n - 1] - y[n - 2])
                + 0.5 * f(y[n - 1], 2) * (y[n - 1] - y[n - 2]) - 2.0 * c[4 * (n - 1) + 1];
    } else {
        c[0] = 1.0;
        c[1] = 0.0;
        c[4 * n] = 1.0;
        c[4 * (n - 1) + 2] = 0.0;

        c[3] = f(ks[0], 0);
        c[4 * n + 3] = f(ks[n], 0);
    }
}

void Window::DiffPoly()
{
    double t1;
    double t2;
    for (int i = 1; i < n - 1; i++)
    {
        t1 = (fy[i + 1] - fy[i]) / (y[i + 1] - y[i]);
        t2 = (fy[i] - fy[i - 1]) / (y[i] - y[i - 1]);
        c[i * 4] = fabs(t1 - t2);
    }
}

void Window::metod_24()
{
    DiffPoly();

    BorderCond(1);

    for (int i = 2; i < n - 2; i++) {
        if (c[(i + 1) * 4] * c[(i + 1) * 4] + c[(i - 1) * 4] *  c[(i - 1) * 4] < 1e-40) {
            c[i * 4 + 1] = ((y[i + 1] - y[i]) * (fy[i] - fy[i - 1]) / (y[i] - y[i - 1])
                    + (y[i] - y[i - 1]) * (fy[i + 1] - fy[i]) / (y[i + 1] - y[i]))
                    / (y[i + 1] - y[i - 1]);
        } else {
            c[i * 4 + 1] = (c[(i + 1) * 4] * (fy[i] - fy[i - 1]) / (y[i] - y[i - 1])
                    + c[(i - 1) * 4] * (fy[i + 1] - fy[i]) / (y[i + 1] - y[i]))
                    / (c[(i + 1) * 4] + c[(i - 1) * 4]);
        }

    }
    CoefSet24();
}
double Window::Approx_24(double x)
{
    int i = 0;
    for (i = 0; i < n - 2; i++) {
        if (x <= y[i + 1]) {
            break;
        }
    }
    double t = x - y[i];
    return c[i * 4] + c[i * 4 + 1] * t + c[i * 4 + 2] * t * t + c[i * 4 + 3] * t * t * t;
}

void Window::SolveSystem()
{
    c[1] /= c[0];

    for (int i = 1; i < n; i++)
    {
        c[i * 4] -= c[(i - 1) * 4 + 2] * c[(i - 1) * 4 + 1];
        c[i * 4 + 1] /= c[i * 4];
    }
    c[n * 4] -= c[(n - 1) * 4 + 2] * c[(n - 1) * 4 + 1];

    d[0] = c[3] / c[0];
    int l;
    for (l = 1; l < n + 1; l++) {
        d[l] = (c[l * 4 + 3] - c[(l - 1) * 4 + 2] * d[l - 1]) / c[l * 4];
        if (l == n) {
            for (int j = n - 1; j >= 0; j--) {
                d[j] -= c[j * 4 + 1] * d[j + 1];
            }
        }
    }
}

void Window::CoefSet45()
{
    double tmp1, tmp2;
    for (int i = 0; i < n; i ++)
    {
        c[i * 4] = d[i];
        tmp2 = (y[i] - ks[i]);
        tmp1 = ((d[i + 1] - fy[i]) / (ks[i + 1] - y[i]) - (fy[i] - d[i]) / tmp2) / (ks[i + 1] - ks[i]);
        c[i * 4 + 2] = tmp1;
        c[i * 4 + 1] = (fy[i] - d[i]) / tmp2 - (y[i] - ks[i]) * tmp1;

    }
}

void Window::metod_45()
{


    for (int i = 1; i < n; i++)
        ks[i] = 0.5 * (y[i - 1] + y[i]);
    ks[0] = a - (ks[2] - ks[1]);
    ks[n] = b + (ks[n - 1] - ks[n - 2]);

    for (int i = 1; i < n; i++)
    {
        c[4 * i] = 1.0/(ks[i] - y[i - 1]) + 1.0/(ks[i] - ks[i - 1]) +
                1.0/(y[i] - ks[i]) + 1.0/(ks[i + 1] - ks[i]);
        c[4 * i + 1] = 1.0/(ks[i + 1] - y[i]) + 1.0/(ks[i] - ks[i + 1]);
        c[(i - 1) * 4 + 2] = 1.0/(y[i - 1] - ks[i - 1]) + 1.0/(ks[i - 1] - ks[i]);
        c[i * 4 + 3] = fy[i] * (1.0/(y[i] - ks[i]) + 1.0/(ks[i + 1] - y[i])) +
                fy[i - 1] * (1.0/(y[i - 1] - ks[i - 1]) + 1.0/(ks[i] - y[i - 1]));
    }

    BorderCond(2);

    SolveSystem();

    CoefSet45();
}
double Window::Approx_45(double x)
{
    int i = 0;
    for (i = 0; i < n - 2; i++) {
        if (x <= ks[i + 1]) {
            break;
        }
    }
    double t = x - ks[i];
    return c[i * 4] + c[i * 4 + 1] * t + c[i * 4 + 2] * t * t;
}
void Window::change_metod()
{
    y[0] = a;
    y[n - 1] = b;
    double h = (b - a) / (n - 1);
    for (int i = 1; i < n - 1; i++) {
        y[i] = a + i * h;
    }
    for (int i = 0; i < n; i++) {
        fy[i] = f(y[i], 0);
    }
    if (q == 1) {
        fy[n / 2] += z;
    }
    p %= 4;
    p++;
    p == 2 ? metod_45() : metod_24();
    m = 1;
    update();
}

void Window::change_func()
{
    func_id = (func_id + 1) % 7;
    p = 0;
    switch (func_id) {
    case 0:
        f_name = "f (x) = 1";
        f = f_0;
        break;
    case 1:
        f_name = "f (x) = x";
        f = f_1;
        break;
    case 2:
        f_name = "f (x) = x * x";
        f = f_2;
        break;
    case 3:
        f_name = "f (x) = x * x * x";
        f = f_3;
        break;
    case 4:
        f_name = "f (x) = x * x * x * x";
        f = f_4;
        break;
    case 5:
        f_name = "f (x) = exp(x)";
        f = f_5;
        break;
    case 6:
        f_name = "f (x) = 1 / (25 * x * x + 1)";
        f = f_6;
        break;
    }
    z = 0.0;
    q = 0;
    m ? change_metod() : update();
}
double Window::residual(int t)
{
    double max_y = 0;
    double x1, y1, d;
    d = (b - a) / width();
    for (x1 = a; x1 - b < 1.e-6; x1 += d) {
        if (t) {
            y1 = Approx_24(x1);
        } else {
            y1 = Approx_45(x1);
        }
        y1 = fabs(y1 - f(x1, 0));
        if (y1 > max_y)
            max_y = y1;
    }
    return max_y;
}

void Window::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    char str[100];
    double x1, x2, y1, y2;
    double max_y, min_y;
    double delta_y;
    double delta_x;

    delta_x = (b - a) / width();
    max_y = min_y = 0;

    for (x1 = a; x1 - b < 1.e-6; x1 += delta_x) {
        y1 = f(x1, 0);
        min_y = y1 < min_y ? y1 : min_y;
        max_y = y1 > max_y ? y1 : max_y;
    }

    delta_y = 0.01 * (max_y - min_y);
    min_y -= delta_y;
    max_y += delta_y;

    // save current Coordinate System
    painter.save();

    // make Coordinate Transformations
    painter.translate(0.5 * width(), 0.5 * height());
    painter.scale(width() / (b - a), -height() / (max_y - min_y));
    painter.translate(-0.5 * (a + b), -0.5 * (min_y + max_y));
    QPen pen("black");
    pen.setWidth(0);
    painter.setPen(pen);
    // draw approximated line for graph
    if (p != 4) {
        x1 = a;
        y1 = f(x1, 0);
        for (x2 = x1 + delta_x; x2 - b < 1.e-6; x2 += delta_x) {
            y2 = f(x2, 0);
            painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));
            x1 = x2, y1 = y2;
        }
        x2 = b;
        y2 = f(x2, 0);

        painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));
    }
    // draw axis
    pen.setWidth(0);
    pen.setColor("red");
    painter.setPen(pen);
    painter.drawLine(a, 0, b, 0);
    painter.drawLine(0, max_y, 0, min_y);
    // restore previously saved Coordinate System
    painter.restore();

    // render function name
    painter.setPen("blue");
    painter.drawText(30, 20, f_name);
    sprintf(str, "%s %d %s %lf %s %lf", "n = ", n, " ,a = ", a, " ,b = ", b);
    painter.drawText(30, 40, str);
    sprintf(str, "%s %lf %s %d", "p = ", z, " ,s = ", s);
    painter.drawText(30, 60, str);

    if (p == 1 || p == 3) {
        delta_x = (b - a) / n;
        max_y = min_y = 0;
        for (x1 = a; x1 - b < 1.e-6; x1 += delta_x) {
            y1 = Approx_24(x1);
            min_y = y1 < min_y ? y1 : min_y;
            max_y = y1 > max_y ? y1 : max_y;
        }
        double res = residual(1);
        double MAX = max_y;
        double MIN = min_y;
        delta_y = 0.01 * (max_y - min_y);
        min_y -= delta_y;
        max_y += delta_y;
        painter.save();
        painter.translate(0.5 * width(), 0.5 * height());
        painter.scale(width() / (b - a), -height() / (max_y - min_y));
        painter.translate(-0.5 * (a + b), -0.5 * (min_y + max_y));

        pen.setColor("darkCyan");
        pen.setWidth(0);
        painter.setPen(pen);

        x1 = a;
        y1 = Approx_24(x1);
        for (x2 = x1 + delta_x; x2 - b < 1.e-6; x2 += delta_x) {
            y2 = Approx_24(x2);
            painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));
            x1 = x2, y1 = y2;
        }
        x2 = b;
        y2 = Approx_24(x2);
        painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));
        painter.restore();

        painter.setPen("blue");

        if (fabs(MAX) < fabs(MIN))
            MAX = MIN;
        sprintf(str, "%s %lf", "metod_24: max = ", MAX);
        painter.drawText(30, 80, str);
        sprintf(str, "%s %10.3e", "residual_24 = ", res);
        painter.drawText(30, 100, str);
        printf("metod_24: MAX = %lf\n", MAX);
    }
    if (p == 2 || p == 3) {
        if (p == 3)
            metod_45();
        delta_x = (b - a) / n;
        max_y = min_y = 0;
        for (x1 = a; x1 - b < 1.e-6; x1 += delta_x) {
            y1 = Approx_45(x1);
            min_y = y1 < min_y ? y1 : min_y;
            max_y = y1 > max_y ? y1 : max_y;
        }
        double res = residual(0);
        double MAX = max_y;
        double MIN = min_y;
        delta_y = 0.01 * (max_y - min_y);
        min_y -= delta_y;
        max_y += delta_y;
        painter.save();
        painter.translate(0.5 * width(), 0.5 * height());
        painter.scale(width() / (b - a), -height() / (max_y - min_y));
        painter.translate(-0.5 * (a + b), -0.5 * (min_y + max_y));

        pen.setColor("darkYellow");
        pen.setWidth(0);
        painter.setPen(pen);

        x1 = a;
        y1 = Approx_45(x1);
        for (x2 = x1 + delta_x; x2 - b < 1.e-6; x2 += delta_x) {
            y2 = Approx_45(x2);
            painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));
            x1 = x2, y1 = y2;
        }
        x2 = b;
        y2 = Approx_45(x2);
        painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));
        painter.restore();

        painter.setPen("blue");
        if (fabs(MAX) < fabs(MIN))
            MAX = MIN;
        if (p == 3) {
            sprintf(str, "%s %lf", "metod_45: max = ", MAX);
            painter.drawText(30, 120, str);
            sprintf(str, "%s %10.3e", "residual_45 = ", res);
            painter.drawText(30, 140, str);
        } else if (p == 2) {
            sprintf(str, "%s %lf", "metod_45: max = ", MAX);
            painter.drawText(30, 80, str);
            sprintf(str, "%s %10.3e", "residual_45 = ", res);
            painter.drawText(30, 100, str);
            printf("metod_25: MAX = %lf\n", MAX);
        }
    }
    if (p == 4) {
        delta_x = (b - a) / width();
        max_y = min_y = 0;
        for (x1 = a; x1 - b < 1.e-6; x1 += delta_x) {
            y1 = f(x1, 0) - Approx_24(x1);
            y1 = fabs(y1);
            min_y = y1 < min_y ? y1 : min_y;
            max_y = y1 > max_y ? y1 : max_y;
        }
        double res = residual(1);
        delta_y = 0.01 * (max_y - min_y);
        min_y -= delta_y;
        max_y += delta_y;
        painter.save();
        painter.translate(0.5 * width(), 0.5 * height());
        painter.scale(width() / (b - a), -height() / (max_y - min_y));
        painter.translate(-0.5 * (a + b), -0.5 * (min_y + max_y));

        pen.setColor("darkCyan");
        pen.setWidth(0);
        painter.setPen(pen);

        x1 = a;
        y1 = f(x1, 0) - Approx_24(x1);
        y1 = fabs(y1);
        for (x2 = x1 + delta_x; x2 - b < 1.e-6; x2 += delta_x) {
            y2 = f(x1, 0) - Approx_24(x1);
            y2 = fabs(y2);
            painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));
            x1 = x2, y1 = y2;
        }
        x2 = b;
        y2 = f(x1, 0) - Approx_24(x1);
        y2 = fabs(y2);
        painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));
        painter.restore();
        painter.setPen("blue");
        sprintf(str, "%s %10.3e", "residual_24 = ", res);
        painter.drawText(30, 80, str);


        metod_45();
        res = residual(0);
        max_y = min_y = 0;
        for (x1 = a; x1 - b < 1.e-6; x1 += delta_x) {
            y1 = f(x1, 0) - Approx_45(x1);
            y1 = fabs(y1);
            min_y = y1 < min_y ? y1 : min_y;
            max_y = y1 > max_y ? y1 : max_y;
        }
        delta_y = 0.01 * (max_y - min_y);
        min_y -= delta_y;
        max_y += delta_y;
        painter.save();
        painter.translate(0.5 * width(), 0.5 * height());
        painter.scale(width() / (b - a), -height() / (max_y - min_y));
        painter.translate(-0.5 * (a + b), -0.5 * (min_y + max_y));

        pen.setColor("darkYellow");
        pen.setWidth(0);
        painter.setPen(pen);

        x1 = a;
        y1 = f(x1, 0) - Approx_45(x1);
        y1 = fabs(y1);
        for (x2 = x1 + delta_x; x2 - b < 1.e-6; x2 += delta_x) {
            y2 = f(x1, 0) - Approx_45(x1);
            y2 = fabs(y2);
            painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));
            x1 = x2, y1 = y2;
        }
        x2 = b;
        y2 = f(x1, 0) - Approx_45(x1);
        y2 = fabs(y2);
        painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));
        painter.restore();
        painter.setPen("blue");
        sprintf(str, "%s %10.3e", "residual_45 = ", res);
        painter.drawText(30, 100, str);
    }
}
