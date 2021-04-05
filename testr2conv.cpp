#include "testr2conv.h"
#include "window.h"
#include <QMainWindow>
#include <QRandomGenerator>

// test point
#define PT 100

void TestR2Conv::testZero()
{
    QMainWindow main;
    Window window(&main);

    QTest::mouseClick(&window, Qt::MiddleButton, Qt::NoModifier,
                      QPoint(PT, PT));
    QTest::mouseClick(&window, Qt::RightButton, Qt::NoModifier, QPoint(PT, PT));
}

void TestR2Conv::testOne()
{
    QMainWindow main;
    Window window(&main);

    QTest::mouseClick(&window, Qt::LeftButton, Qt::NoModifier, QPoint(PT, PT));

    QTest::mouseClick(&window, Qt::RightButton, Qt::NoModifier, QPoint(PT, PT));
    QTest::mouseClick(&window, Qt::RightButton, Qt::NoModifier,
                      QPoint(2 * PT, 2 * PT));
}

void TestR2Conv::testTwo()
{
    QMainWindow main;
    Window window(&main);

    QTest::mouseClick(&window, Qt::LeftButton, Qt::NoModifier, QPoint(PT, PT));
    QTest::mouseClick(&window, Qt::LeftButton, Qt::NoModifier,
                      QPoint(PT, 3 * PT));

    QTest::mouseClick(&window, Qt::RightButton, Qt::NoModifier, QPoint(PT, PT));
    QTest::mouseClick(&window, Qt::RightButton, Qt::NoModifier,
                      QPoint(PT, 2 * PT));
    QTest::mouseClick(&window, Qt::RightButton, Qt::NoModifier,
                      QPoint(2 * PT, 2 * PT));
}

void TestR2Conv::testThree()
{
    QMainWindow main;
    Window window(&main);

    QTest::mouseClick(&window, Qt::LeftButton, Qt::NoModifier, QPoint(PT, PT));
    QTest::mouseClick(&window, Qt::LeftButton, Qt::NoModifier, QPoint(PT, PT));
    QTest::mouseClick(&window, Qt::LeftButton, Qt::NoModifier,
                      QPoint(4 * PT, PT));
    QTest::mouseClick(&window, Qt::LeftButton, Qt::NoModifier,
                      QPoint(PT, 4 * PT));
    QTest::mouseClick(&window, Qt::LeftButton, Qt::NoModifier,
                      QPoint(4 * PT, 4 * PT));

    QTest::mouseClick(&window, Qt::RightButton, Qt::NoModifier, QPoint(PT, PT));
    QTest::mouseClick(&window, Qt::RightButton, Qt::NoModifier,
                      QPoint(PT, 2 * PT));
    QTest::mouseClick(&window, Qt::RightButton, Qt::NoModifier,
                      QPoint(2 * PT, 2 * PT));
    QTest::mouseClick(&window, Qt::RightButton, Qt::NoModifier,
                      QPoint(10 * PT, 10 * PT));
}

#define HUNDRED 100
void TestR2Conv::testHundred()
{
    QMainWindow main;
    Window window(&main);

    for (int i = 0; i < HUNDRED; i++) {
        QPoint pt(QRandomGenerator::global()->bounded(PT, 50 * PT),
                  QRandomGenerator::global()->bounded(PT, 50 * PT));
        QTest::mouseClick(&window, Qt::LeftButton, Qt::NoModifier, pt);

        QPoint pt2(QRandomGenerator::global()->bounded(0, 100 * PT),
                   QRandomGenerator::global()->bounded(0, 100 * PT));
        QTest::mouseClick(&window, Qt::RightButton, Qt::NoModifier, pt2);
    }
}

QTEST_MAIN(TestR2Conv)
