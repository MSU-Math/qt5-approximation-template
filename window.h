#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

class Window : public QWidget
{
    Q_OBJECT

  private:
    int func_id;
    int p;
    const char *f_name;
    double a;
    double b;
    int n;
    int q;
    int s;
    int m;
    double z;
    double *y;
    double *fy;
    double *c;
    double *d;
    double *ks;
    double (*f)(double, int);
    void CoefSet24();
    void CoefSet45();
    
public:
    double Approx_24(double x);
    double Approx_45(double x);
    void metod_24();
    void metod_45();
    Window(char *argv[], QWidget *parent);
    ~Window();
    void del();
    void mem();
    double max_funk(int t);
    QSize minimumSizeHint() const;
    QSize sizeHint() const; 
    int parse_command_line(int argc, char *argv[]);
    double residual(int t);

    void BorderCond(int t);

    void DiffPoly();

    void SolveSystem();



public slots:
    void change_metod();
    void change_func();
    void size_plus();
    void size_minus();
    void n_plus();
    void n_minus();
    void f_plus();
    void f_minus();
  protected:
    void paintEvent(QPaintEvent *event);
};

#endif
