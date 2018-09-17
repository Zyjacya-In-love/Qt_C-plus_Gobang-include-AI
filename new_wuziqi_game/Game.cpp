//////////////////////////Game.cpp//////////////////////////////
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

Game::Game(QWidget *parent): QMainWindow(parent){
    //窗口标题
    setWindowTitle("五子棋");
    //窗口大小
    setMinimumSize(1000,800);
    setMaximumSize(1000,800);

    //游戏信息显示
    QPushButton *button_s = new QPushButton(this);
    button_s->setStyleSheet("font:Bold;font-size:36px;color:white;background-color: rgb(105, 105, 105);border:2px ;border-radius:10px;padding:2px 4px;");

    button_s->setGeometry(QRect(780, 100, 170, 50));
    button_s->setText("游戏类型:");

    QLabel *Label = new QLabel(this);
    Label->setText("@Zyjacya-In-love");
    Label->setGeometry(785, 700, 200, 50);
    Label->setStyleSheet("color: black;font-size:24px;");

    //游戏类型选择按钮
    game_type = 1;
    SelectModeGroup = new QButtonGroup(this);
    
    QRadioButton *PVC = new QRadioButton(tr("PVC"), this);
    SelectModeGroup->addButton(PVC, 1);
    PVC->setStyleSheet("font:Bold;font-size:36px;color:black;");
    PVC->setGeometry(825, 200, 100, 30);
    connect(PVC, SIGNAL(clicked()), this, SLOT(SelectRadio())); 

    QRadioButton *PVP = new QRadioButton(tr("PVP"), this);
    SelectModeGroup->addButton(PVP, 2);
    PVP->setStyleSheet("font:Bold;font-size:36px;color:black;");
    PVP->setGeometry(825, 270, 100, 30);
    connect(PVP, SIGNAL(clicked()), this, SLOT(SelectRadio())); 
    
    PVC->setChecked(true);//初始选项 PVC  

    //游戏 开始 结束 按钮
    flag = 0;
    button = new QPushButton(this);
    button->setStyleSheet("font:Bold;font-size:36px;color:white;background-color: rgb(30, 144, 255);border:2px ;border-radius:10px;padding:2px 4px;");

    button->setGeometry(QRect(800, 390, 170, 50));
    button->setText("开始");
    connect(button, SIGNAL(clicked()), this, SLOT(oper())); 

    // 悔棋 按钮
    while(!sta.empty()) sta.pop();
    QPushButton *back_button = new QPushButton(this);
    back_button->setStyleSheet("QPushButton{font:Bold;font-size:30px;color:white;background-color:rgb(191, 62, 255);border:2px ;border-radius:10px;padding:2px 4px;}""QPushButton:hover{color:white;background-color:rgb(178, 58, 238);}""QPushButton:pressed{color:white;background-color:rgb(154, 50, 205);padding-left:3px;padding-top:3px;}");
    back_button->setGeometry(QRect(830, 520, 120, 40));
    back_button->setText("悔棋");
    connect(back_button, SIGNAL(clicked()), this, SLOT(back())); 
    
    //配置END
    
    //开启鼠标监听
    setMouseTracking(true);
    lock = 1;
    memset(chessboard, 0, sizeof(chessboard));
    
}

//PVC PVP 选择 槽
void Game::SelectRadio(){
    switch(SelectModeGroup->checkedId()){
        case 1 : 
            game_type = 1;// PVC
            break;
        case 2 :
            game_type = 2;// PVP
            break;
    }
}

// 开始结束按钮 槽
void Game::oper(){
    if(flag == 0){
        player = 0;
        lock = 0;
        currentX = -100;
        currentY = -100;
        moveX = -100;
        moveY = -100;
        while(!sta.empty()) sta.pop();
        memset(chessboard, 0, sizeof(chessboard));
        button->setText("结束");
        button->setStyleSheet("font:Bold;font-size:36px;color:white;background-color: rgb(255, 0, 0);border:2px ;border-radius:10px;padding:2px 4px;"); 
    }
    else {
        button->setText("开始");
        button->setStyleSheet("font:Bold;font-size:36px;color:white;background-color: rgb(30, 144, 255);border:2px ;border-radius:10px;padding:2px 4px;");
        lock = 1;
        memset(chessboard, 0, sizeof(chessboard));
        update();
    }
    flag = !flag;
}

// 悔棋 槽
void Game::back(){
    if(sta.empty() || flag == 0) return ;
    int num = 1;
    if(game_type == 1) num++;
    if(winner == "YOU" && lock == 1) num--;
    for (int i = 0; i < num; ++i){
        chessboard[sta.top().x()][sta.top().y()] = 0;
        sta.pop();
        player--;
    }
    if(!sta.empty()){
        currentX = sta.top().x();
        currentY = sta.top().y();
    }
    else {
        currentX = -100;
        currentY = -100;
    }
    lock = 0;
    update();
}

// 绘图事件
void Game::paintEvent(QPaintEvent*){
    QPainter p(this);
    // 反走样
    p.setRenderHint(QPainter::Antialiasing, true);
     
    //画棋盘

    //上背景色
    QBrush brush;
    brush.setColor(QColor(213, 176, 141));//QQ五子棋棋盘颜色
    brush.setStyle(Qt::SolidPattern);
    p.setBrush(brush);

    //边框加粗（已取消，原因：没有更好看）
    QPen pen = p.pen();
    p.setPen(pen);
    p.drawRect(30, 30, 740, 740 );

    //棋盘网格线
    pen.setColor(Qt::black);
    pen.setWidth(1);
    p.setPen(pen);
    for(int i = 0; i < 15; ++i){
        p.drawLine(50, 50+i*50, 750, 50+i*50);
        p.drawLine(50+i*50, 50, 50+i*50, 750);
    }

    //画五个黑点
    brush.setColor(Qt::black);
    p.setBrush(brush);
    p.drawRect(195, 195, 10, 10);
    p.drawRect(595, 195, 10, 10);
    p.drawRect(195, 595, 10, 10);
    p.drawRect(595, 595, 10, 10);
    p.drawRect(395, 395, 10, 10);

    // 画棋子
    brush.setStyle(Qt::SolidPattern);
    for(int i = 0; i < 15; ++i){
        for(int j = 0; j < 15; ++j){
            if(chessboard[i][j] == 1){
                brush.setColor(Qt::black);
                p.setBrush(brush);
                p.drawEllipse(QPoint( i*50+50, j*50+50 ), 20, 20);
            }
            else if(chessboard[i][j] == 2){
                brush.setColor(Qt::white);
                p.setPen(Qt::NoPen);
                p.setBrush(brush);
                p.drawEllipse(QPoint( i*50+50, j*50+50 ), 20, 20);
            }
        }
    }

    // flag == 1 游戏运行 即 绘制标识框
    if(!flag) return ;
    //画最近一个落子的标识红框
    pen.setColor(Qt::red);
    pen.setWidth(1);
    p.setPen(pen);
    p.setBrush(Qt::NoBrush);
    p.drawRect(currentX*50+50 - 23, currentY*50+50 - 23, 46, 46);

    //  产生胜负/结束游戏  即 锁 棋盘 禁止下棋
    if(lock) return;
    //画选择绿圆标识
    if((moveX >= 0 && moveX <= 14) && (moveY>=0 && moveY <= 14) && chessboard[moveX][moveY] == 0){
        brush.setColor(QColor(0, 255, 0, 150));//84,255,159
        p.setPen(Qt::NoPen);
        p.setBrush(brush);
        p.drawEllipse(QPoint( moveX*50+50, moveY*50+50 ), 20, 20);
    }

}

// 输赢判断
void Game::Is_Someone_Win(int x, int y){
    QPoint p(x, y);
    if(isWin(p)){
        lock = 1;
        if(game_type == 1){
            if(chessboard[x][y] == 1) winner = "YOU";
            else winner = "AI";
        }
        else {
            if(chessboard[x][y] == 1) winner = "BLACK";
            else winner = "WHITE";
        }
        QMessageBox::information(NULL,"GAME OVER",winner + " WIN",QMessageBox::Ok);
    }
}

// dir方向 offset 位移 的棋子类型
int Game::getPointAt(QPoint p, int dir, int offset) {  
    int y = p.y();  
    int x = p.x();  
  
    y = y + offset * m_dir[dir].y();  
    x = x + offset * m_dir[dir].x();  
  
    if(y<0 || x<0 || x>=15 || y>=15)  
        return -100 ;  
  
    return chessboard[x][y];  
}

// 是否五子连珠  
int Game::isWin(QPoint p){
    int dir_cnt[8] = {0};
    memset(dir_cnt, 0, sizeof(dir_cnt));
    for (int i = 0; i < 8; ++i){
        for (int j = 1; ; ++j){
            if(getPointAt(p, i, j) != chessboard[p.x()][p.y()])
                break;
            dir_cnt[i]++;
        }
    }
    for (int i = 0; i < 4; ++i){
        if(dir_cnt[i] + dir_cnt[i+4] + 1 >= 5){//五子连珠
            return 1;
        }
    }
    return 0;
}  

// 鼠标移动
void Game::mouseMoveEvent(QMouseEvent *e){
    moveX = (e->x() + 25) / 50 - 1;
    moveY = (e->y() + 25) / 50 - 1;
    update();
}

// 鼠标按压 下棋
void Game::mousePressEvent(QMouseEvent *e){
    if(lock) return ;
    if(!(game_type == 1 && (player&1))){
        PERSON_time(e);
        if(lock == 0 && game_type == 1 && (player&1)){
            AI_time();
        }
    }
}

// 更新棋盘
void Game::update_chessboard(int x, int y){
    if(!chessboard[x][y]){
        chessboard[x][y] = player++ % 2 + 1;
        currentX = x;
        currentY = y;
        QPoint p(x, y);
        sta.push(p);
        update();
        Is_Someone_Win(x, y);
    }
}
    

//人 下棋
void Game::PERSON_time(QMouseEvent *e){
    int x, y;
    if((e->x() >= 25 && e->x() <= 775) && (e->y() >= 25 && e->y() <= 775)){
        x = (e->x() + 25) / 50 - 1;
        y = (e->y() + 25) / 50 - 1;
        update_chessboard(x, y);
    }
}


// AI 下棋
void Game::AI_time(){
    QPoint Best_Pos_Attack;  // 最佳的进攻位置
    QPoint Best_Pos_Defend;  // 最佳的防守位置
    int Max_A = 0;  // 当前棋盘中采取 进攻策略 的最高权重
    int Max_D = 0;  // 当前棋盘中采取 防守策略 的最高权重

    // 对于 AI 来说最佳进攻， 能赢的地方
    for (int i = 0; i < 15; ++i){
        for (int j = 0; j < 15; ++j){
            QPoint now(i, j);
            if(chessboard[now.x()][now.y()] != 0) continue;
            int value = Assess(now, 1);// 1 是 AI 
            if(Max_A < value){
                Max_A = value;
                Best_Pos_Attack = now;
            }
        }
    }

    // 对于 AI 来说最佳防守， 需要堵的地方
    for (int i = 0; i < 15; ++i){
        for (int j = 0; j < 15; ++j){
            QPoint now(i, j);
            if(chessboard[now.x()][now.y()] != 0) continue;
            int value = Assess(now, 0);// 0 是 玩家 ： 就是对玩家来说最佳进攻 
            if(Max_D < value){
                Max_D = value;
                Best_Pos_Defend = now;
            }
        }
    }

    // 选择 防守 ， 进攻
    if(Max_A >= Max_D)  // 进攻 优先 
        update_chessboard(Best_Pos_Attack.x(), Best_Pos_Attack.y());
    else // 防守
        update_chessboard(Best_Pos_Defend.x(), Best_Pos_Defend.y());

}

//AI 核心 评分下棋功能
int Game::Assess(QPoint now, int me) {   // now 现在想要下棋的位置，me 从谁的角度来评分
    int value = 0;
    int rival = !me; // 对手

    me += 1; rival += 1;// 棋子
    int empty = 0;   // 无棋子
    int boundary = -100;// 边界

    int cnt_live_2 = 0;//活二的数量

    // 连五 > 活四 > 死四*3 > 活三*2 > 活二*3 （略大于）死三*6 > 死二*4 > 其他棋型 
    // 遍历 8 个方向
    for( int i=0 ; i < 8 ; ++i )  {
        // 活四 必胜 
        // 1.活四  *22220   
        if(getPointAt(now, i, 1) == me && getPointAt(now ,i, 2) == me && getPointAt(now, i, 3) == me && getPointAt(now, i, 4) == me && getPointAt(now, i, 5) == empty){
            value += 300000;
            if(me == 1) value -= 500;// 人 不是当前棋手
            continue ;  
        }
        
        // 死四 ： 如果是己方则下子获得胜利，对手的话要竭力去赌 
        // 2. 死四A *22221  
        if(getPointAt(now, i, 1) == me && getPointAt(now, i, 2) == me && getPointAt(now, i, 3) == me && getPointAt(now, i, 4) == me && (getPointAt(now, i, 5) == rival || getPointAt(now, i, 5) == boundary)){
            value += 250000;
            if(me == 1) value -= 500;
            continue ;
        }            

        // 3. 死四B 2*222 
        if(getPointAt(now, i, -1) == me && getPointAt(now, i, 1) == me && getPointAt(now, i, 2) == me && getPointAt(now, i, 3) == me){
            value += 240000; 
            if(me == 1) value -= 500;
            continue ;
        }            

        // 4. 死四C 22*22  
        if(getPointAt(now, i, -2) == me && getPointAt(now, i, -1) == me && getPointAt(now, i, 1) == me && getPointAt(now, i, 2) == me){
            value += 230000; 
            if(me == 1) value -= 500;
            continue ;
        }        

        // 活三 ： 可直接一手变成活四，当敌方活三出现时，必须进行防守 
        // 5. 活三A 222*0
        if(getPointAt(now, i, -1) == me && getPointAt(now, i, -2) == me && getPointAt(now, i, -3) == me){
            if(getPointAt(now, i, 1) == empty){
                value += 750; 
                // 0222*0
                if(getPointAt(now, i, -4) == empty){
                    value += 3150; 
                    if(me == 1) value -= 500;
                }
            }
            // 0222*1
            if(getPointAt(now, i, -4) == empty && (getPointAt(now, i, 1) == rival || getPointAt(now, i, 1) == boundary))
                value += 500; 
            continue ;
        } 

        // 6. 活三B 2220*   
        if(getPointAt(now, i, -1) == empty && getPointAt(now, i, -2) == me && getPointAt(now, i, -3) == me && getPointAt(now, i, -4) == me){
            value += 350; 
            continue ;
        }

        // 死三 ： 能够得到死四的三，即便不去防守，下一手也只能形成死四，仍然守的住 
        // 7. 死三A 22*2  
        if(getPointAt(now, i, -1) == me && getPointAt(now, i, -2) == me && getPointAt(now, i, 1) == me){
            value += 600; 
            // 022*20
            if(getPointAt(now, i, -3) == empty && getPointAt(now, i, 2) == empty){
                value += 3150; 
                continue ;
            }
            //122*21
            if((getPointAt(now, i, -3) == rival || getPointAt(now, i, -3) == boundary) && (getPointAt(now, i, 2) == rival || getPointAt(now, i, 2) == boundary)){
                continue ;
            }
            else {
                value += 700;
                continue ;
            }
        }

        // 活二 ： 能够形成活三的二，如果能够形成较多的活二棋型，对局势有利 
        // 8. 活二数量 (!1)22*(!1) 
        if(getPointAt(now, i, -1) == me && getPointAt(now, i, -2) == me && getPointAt(now, i, -3) != rival && getPointAt(now, i, 1) != rival)
            cnt_live_2++;

        // 周围如果已有我方棋子数目比较多的话，适当增加一下权值，越多机会越大 
        int surround = 0;
        for (int k = -4; k <= 0; ++k){
            int tmp = 0;
            for (int l = 0; l <= 4; ++l){
                if(getPointAt(now, i, k+l) == me)
                    tmp++;
                else if((getPointAt(now, i, k+l) == rival || getPointAt(now, i, k+l) == boundary)){
                    tmp = 0;
                    break;
                }
            }
            surround += tmp;
        }
        value += surround * 15;
    }
    if(cnt_live_2 >= 2){
        value += 3000;
        if(me == 1) value -= 100;
    }
    return value;
}


/**/
//
