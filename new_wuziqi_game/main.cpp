//////////////////////////main.cpp//////////////////////////////
#include <QApplication>
#include <QWidget>
#include <QPaintEvent>
#include <QTimer>
#include <QDebug>
#include <QPainter>
#include <QMouseEvent>
#include <QPainterPath>
#include <QPainter>
#include <QPen>
#include <QColor>
#include <QBrush>
#include <QMessageBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <Qpalette>
#include <QtDebug>
#include <QPushButton>
#include <QLabel>
#include <QMainWindow>
#include <bits/stdc++.h>
#include "Game.h"
using namespace std;
/**
 * @link        [link]
 * @description 使用 Qt 嵌 C++ 做的五子棋小游戏 主要定及功能实现在 Game.h 和 Game.cpp 中 
 * @title       new_wuziqi_game
 * @solve       AI 用的是评分表 详见注释
 * @DateTime    2018-05-25
 * @Author      Krone
 */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Game w;
    w.show();
    return app.exec();
}
