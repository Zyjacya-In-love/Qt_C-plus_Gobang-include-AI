/////////////////Game.h/////////////////
#ifndef GAME_H
#define GAME_H

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
using namespace std;

class Game : public QMainWindow {

    Q_OBJECT

private:
    int chessboard[15][15];
    int player; // 玩家 player = 偶数 第一个玩家， player == 奇数 第二个
    int moveX, moveY; // 鼠标Move位置
    int currentX, currentY; // 上次 落子点位置
    bool lock;// 棋盘锁
    int flag; // 游戏现状 0 是 按钮是开始键， 1 结束键
    int game_type; // 游戏类型 1 是 PVC , 2 是 PVP 
    QPushButton *button;
    QButtonGroup *SelectModeGroup;
    QString winner;
    stack<QPoint > sta; // 悔棋
public:
    // 构造函数
    Game(QWidget *parent = 0);

    // 更新棋盘
    void update_chessboard(int x, int y);

    // 8 个方向
    QPoint m_dir[8] = {  
        QPoint(0,-1),QPoint(1,-1),QPoint(1,0),QPoint(1,1),  
        QPoint(0,1),QPoint(-1,1),QPoint(-1,0),QPoint(-1,-1)  
    };  
    
    // dir 方向 offset 位移 的 棋子类型  
    int getPointAt(QPoint p, int dir, int offset); 

    // 有 人/AI 赢了吗
    void Is_Someone_Win(int x, int y);

    // 是否五子连珠
    int isWin(QPoint p);
    
    // 人下棋 重载的鼠标按压 获得位置
    void PERSON_time(QMouseEvent *e);

    // AI 下棋
    void AI_time();

    // AI 核心 评分
    int Assess(QPoint now, int me);

    // 绘图事件
    void paintEvent(QPaintEvent*);

    // 鼠标移动事件
    void mouseMoveEvent(QMouseEvent *e);

    // 鼠标按压事件
    void mousePressEvent(QMouseEvent *e);
    
public slots:
    void oper();
    void SelectRadio();
    void back();
};

#endif // GAME_H


//