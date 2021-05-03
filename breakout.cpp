#include <QPainter>
#include <QApplication>
//#include <QSound>
#include "breakout.h"

// 生命值
int Breakout::count_life = 0;

// 時間
int Breakout::count_time = 0;

// 關卡
int Breakout::stage = 1;

// 磚塊 & 強化磚塊
int Breakout::hit[N_OF_BRICKS_1] = {0};

// 遊戲設定
Breakout::Breakout(QWidget *parent) : QWidget(parent) {
	// x 歸零
	x = 0;
	// 設定遊戲狀態
	gameOver = false;
	gameWon = false;
	paused = false;
	gameStarted = false;
	gameReset = false;
	// 生成物件
	ball = new Ball();
	stick = new Stick();
	setBricks();
}

// 遊戲關閉
Breakout::~Breakout() {
	delete ball;
	delete stick;
	if(stage == 1){
		for (int i = 0; i < N_OF_BRICKS_1; i++) {
			delete bricks_1[i];
		}
	}
	if(stage == 2){
		for (int i = 0; i < N_OF_BRICKS_2; i++) {
			delete bricks_2[i];
		}
	}
	if(stage == 3){
		for (int i = 0; i < N_OF_BRICKS_3; i++) {
			delete bricks_3[i];
		}
	}
}

// 設定視窗
void Breakout::paintEvent(QPaintEvent *event) {
	Q_UNUSED(event); 
	
	QPainter painter(this);
	// 遊戲結束
	if (gameOver) {
		finishGame(&painter, "Game Over");    
	} 
	// 遊戲勝利
	else if(gameWon) {
		finishGame(&painter, "Victory");
	}
	// 遊戲重置
	else if(gameReset) {
		drawObjects(&painter);
	}
	// 畫 Stick、Ball 跟 Bricks
	else {
		drawObjects(&painter);
	}
}

// 設定輸出勝利與遊戲結束的字體
void Breakout::finishGame(QPainter *painter, QString message) {

	QFont font("Courier", 15, QFont::DemiBold);
	QFontMetrics fm(font);
	int textWidth = fm.width(message);

	painter -> setFont(font);
	int h = height();
	int w = width();

	painter -> translate( QPoint( w / 2, h / 2));
	painter -> drawText(- textWidth / 2, 0, message);    
}

// 畫出視窗上的物件
void Breakout::drawObjects(QPainter *painter) {
	// 畫球
	painter -> drawImage( ball -> getRect(), ball -> getImage());
	// 畫板
	painter -> drawImage( stick -> getRect(), stick -> getImage());
	// 畫方塊
	if(stage == 1){
		for (int i = 0; i < N_OF_BRICKS_1; i++) {
			if (!bricks_1[i] -> isDestroyed()) {
				painter -> drawImage(bricks_1[i] -> getRect(), bricks_1[i] -> getImage());
			}
		}
	}
	if(stage == 2){
		for (int i = 0; i < N_OF_BRICKS_2; i++) {
			if (!bricks_2[i] -> isDestroyed()) {
				painter -> drawImage(bricks_2[i] -> getRect(), bricks_2[i] -> getImage());
			}
		} 
	}
	if(stage == 3){
		for (int i = 0; i < N_OF_BRICKS_3; i++) {
			if (!bricks_3[i] -> isDestroyed()) {
				painter -> drawImage(bricks_3[i] -> getRect(), bricks_3[i] -> getImage());
			}
		} 
	}
	// 第幾關
	painter -> drawText(0, 10, QString("Stage : %1").arg(stage));
	// 剩幾條命
	painter -> drawText(67.5, 10, QString("|  Left_Life : %1").arg(3 - count_life));
	// 剩多少時間
	painter -> drawText(175, 10, QString("|  Left_Time : %1").arg(160 - stage * 10 - (count_time += timerInstantaneous * 5) / 1000));
}

// 每微秒要做的事
void Breakout::timerEvent(QTimerEvent *event) {
	
	Q_UNUSED(event);  
	// 劃出物件
	moveObjects();
	// 有沒有碰撞
	checkCollision();
	// 重新再畫
	repaint();
	
}

// 設定移動與否
void Breakout::moveObjects() {

	ball -> edgeMove();
	stick -> stick_move();
	
	
	if(stage == 2){
		for (int i = 0; i < N_OF_BRICKS_2; i++) {
			bricks_2[i] -> brick_move();
		} 
	}
	if(stage == 3){
		for (int i = 0; i < N_OF_BRICKS_3; i++) {
			bricks_3[i] -> brick_move();
		} 
	}
}

// 釋放按鍵的功能
void Breakout::keyReleaseEvent(QKeyEvent *event) {

    int dx_stick = 0;

    switch (event -> key()) {
        case Qt::Key_Left:
            dx_stick = 0;
            stick -> setStick_dx(dx_stick);        
            break;       

        case Qt::Key_Right:
            dx_stick = 0;
            stick -> setStick_dx(dx_stick);        
            break;    
    }
}

// 按下按鍵的功能
void Breakout::keyPressEvent(QKeyEvent *event) {

    int dx_stick = 0;
	
	
    switch (event -> key()) {
	// 左
    case Qt::Key_Left:

        dx_stick = -1;
        stick -> setStick_dx(dx_stick);
        break;
	
	// 右
    case Qt::Key_Right:

        dx_stick = 1;
        stick -> setStick_dx(dx_stick);        
        break;
	
	// 暫停（按P）
    case Qt::Key_P:

        pauseGame();
        break;
	
	// 開始（按空白鍵）
    case Qt::Key_Space:

        startGame();
        break; 
		
	// 關閉視窗（按ESC）
    case Qt::Key_Escape:

        qApp -> exit();
        break;

    default:
        QWidget::keyPressEvent(event);
    }
}

// 設定磚塊
void Breakout::setBricks() {
	if(stage == 1){
		int k = 0;
		// 設定 6 * 3 的矩形排序
		for (int i = 0; i < 14; i++) {
			for (int j = 0; j < 3; j++) {
				if(i == 0 || i == 1 || i == 12 || i == 13){
					bricks_1[k] = new Brick(j * Brick_Size_x + 2.25 * Brick_Size_x, i * Brick_Size_y + 4 * Brick_Size_y, k % 2);
					k++;
				}
				
				else{
					if(j == 0){
						bricks_1[k] = new Brick(( 600 - Brick_Size_x) / 2 , i * Brick_Size_y + 4 * Brick_Size_y, k % 2 );
						k++;
					}
				}
			}
		}
	}
	if(stage == 2){
		int k = 0;
		
		for (int i = 0; i < 14; i++) {
			for (int j = 0; j < 5; j++) {
				if(i == 0 || i == 1){
					if(j <= 1){
						bricks_2[k] = new Brick(2.5 * j * Brick_Size_x + 2 * Brick_Size_x, i * Brick_Size_y + 4 * Brick_Size_y, 1);
						k++;
					}
				}
				if(i == 2 || i == 3){
					if(j == 0 || j == 1){
						bricks_2[k] = new Brick(j * Brick_Size_x + 1.5 * Brick_Size_x, i * Brick_Size_y + 4 * Brick_Size_y, 1);
						k++;
					}
					if(j == 2 || j == 3){
						bricks_2[k] = new Brick(j * Brick_Size_x + 2 * Brick_Size_x, i * Brick_Size_y + 4 * Brick_Size_y, 1);
						k++;
					}
				}
				if(i == 4 || i == 5){
					bricks_2[k] = new Brick(j * Brick_Size_x + 1.25 * Brick_Size_x, i * Brick_Size_y + 4 * Brick_Size_y, 1);
					k++;
				}
				if(i == 6 || i == 7){
					if(j <= 3){
						bricks_2[k] = new Brick(j * Brick_Size_x + 1.75 * Brick_Size_x, i * Brick_Size_y + 4 * Brick_Size_y, 1);
						k++;
					}
				}
				if(i == 8 || i == 9){
					if(j <= 2){
						bricks_2[k] = new Brick(j * Brick_Size_x + 2.25 * Brick_Size_x, i * Brick_Size_y + 4 * Brick_Size_y, 1);
						k++;
					}
				}
				if(i == 10 || i == 11){
					if(j <= 1){
						bricks_2[k] = new Brick(j * Brick_Size_x + 2.75 * Brick_Size_x, i * Brick_Size_y + 4 * Brick_Size_y, 1);
						k++;
					}
				}
				if(i == 12 || i == 13){
					if(j == 0){
						bricks_2[k] = new Brick(( 600 - Brick_Size_x) / 2 , i * Brick_Size_y + 4 * Brick_Size_y, 1);
						k++;
					}
				}
			}
		}
	} 
	if(stage == 3){
		int k = 0;
		
		for (int i = 0; i < 14; i++) {
			for (int j = 0; j < 5; j++) {
				if (i < 8){
					if(j != 2){
						bricks_3[k] = new Brick(j * Brick_Size_x + 1.25 * Brick_Size_x, i * Brick_Size_y + 4 * Brick_Size_y, 1);
						k++;
					}
				}
				else{
					bricks_3[k] = new Brick(j * Brick_Size_x + 1.25 * Brick_Size_x, i * Brick_Size_y + 4 * Brick_Size_y, 1);
					k++;
				}
			}
		}
	} 
}

void Breakout::startGame() {

	if (!gameStarted) {
	  
		ball->resetBallState();
		stick->resetStickState();

		if(stage == 1){
			for (int i=0; i < N_OF_BRICKS_1; i++) {
				bricks_1[i] -> setDestroyed(false);
			}
		}
		if(stage == 2){
			for (int i=0; i < N_OF_BRICKS_2; i++) {
				bricks_2[i] -> setDestroyed(false);
			}
		}
		if(stage == 3){
			for (int i=0; i < N_OF_BRICKS_3; i++) {
				bricks_3[i] -> setDestroyed(false);
			}
		}
		
		gameOver = false; 
		gameWon = false; 
		gameStarted = true;
		timerInstantaneous = startTimer(DELAY); 
	}      
}

void Breakout::pauseGame() {
	if (paused) {
		timerInstantaneous = startTimer(DELAY);
		paused = false;
		
	} 
	else {
		paused = true;
		killTimer(timerInstantaneous);
	}        
}

void Breakout::stopGame() {

	killTimer(timerInstantaneous);
	gameOver = true;
	gameStarted = false;
	
}

void Breakout::resetGame() {
	
	ball->resetBallState();
	
	ball -> ballxdirection = 1;
	ball -> ballydirection = -1;
    stick -> resetStickState();
	
}

void Breakout::victory() {
	
	killTimer(timerInstantaneous);    
	gameWon = true;  
	gameStarted = false;    
}

void Breakout::checkCollision() {
	// 失敗條件 1：掉下視窗 3 次
	if (ball -> getRect().bottom() > BOTTOM_EDGE) {
		if(count_life == 2){
			stopGame();
		}
		else{
			resetGame();
			count_life ++ ;
		}
	}
	
	// 失敗條件 2：時間到
	if(count_time == 160000 - stage * 10000){
			stopGame();
	}
	if(stage == 1){
		for (int i = 0, j = 0; i < N_OF_BRICKS_1; i++) {
			if (bricks_1[i] -> isDestroyed()) {
				j++;
			}
			// 勝利條件：沒有磚塊
			if (j == N_OF_BRICKS_1) {
				stage++;
				resetGame();
				setBricks();
				count_time = 0;
			}
		}
	}
	if(stage == 2){
		for (int i = 0, j = 0; i < N_OF_BRICKS_2; i++) {
			if (bricks_2[i] -> isDestroyed()) {
				j++;
			}
			// 勝利條件：沒有磚塊
			if (j == N_OF_BRICKS_2) {
				stage++;
				resetGame();
				setBricks();
				count_time = 0;
			}
		}
	}
	if(stage == 3){
		for (int i = 0, j = 0; i < N_OF_BRICKS_3; i++) {
			if (bricks_3[i] -> isDestroyed()) {
				j++;
			}
			// 勝利條件：沒有磚塊
			if (j == N_OF_BRICKS_3) {
				victory();
			}
		}
	}
	
	// 碰到板子 x 方向維持不變，y 方向換邊
	if ((ball -> getRect()).intersects(stick -> getRect()))
	{ 
		int ballLeft = ball -> getRect().left();  
		int ballHeight = ball -> getRect().height(); 
		int ballWidth = ball -> getRect().width();
		int ballTop = ball -> getRect().top();
		
		QPoint pointRightTop(ballLeft + ballWidth + 1, ballTop - 1);
		QPoint pointRightBottom(ballLeft + ballWidth + 1, ballTop + ballHeight + 1);
		
		QPoint pointLeftTop(ballLeft - 1, ballTop - 1); 
		QPoint pointLeftBottom(ballLeft - 1, ballTop + ballHeight + 1); 
		
		QPoint pointTop(ballLeft + ballWidth / 2, ballTop - 1);
		QPoint pointBottom(ballLeft + ballWidth / 2, ballTop + ballHeight + 1); 
		
		QPoint pointRight(ballLeft + ballWidth + 1, ballTop + ballHeight / 2);
		QPoint pointLeft(ballLeft - 1, ballTop + ballTop + ballHeight / 2);   
		
		// 球右邊碰到磚塊左邊
		if(stick -> getRect().contains(pointRight)) {
			ball -> setBallXDir(-1);
		} 
		// 球左邊碰到磚塊右邊
		else if(stick -> getRect().contains(pointLeft)) {
			ball -> setBallXDir(1);
		}
		// 球上面碰到磚塊下面
		else if(stick -> getRect().contains(pointTop)) {
			ball -> setBallYDir(1);
		} 
		// 球下面碰到磚塊上面
		else if(stick -> getRect().contains(pointBottom)) {
			ball -> setBallYDir(-1);
		} 
		// 球右上碰到磚塊左下
		else if(stick -> getRect().contains(pointRightTop)) {
			ball -> setBallXDir(-1);
		} 
		// 球右下碰到磚塊左上
		else if(stick -> getRect().contains(pointRightBottom)) {
			ball -> setBallXDir(-1);
		} 
		// 球左上碰到磚塊右下
		else if(stick -> getRect().contains(pointLeftTop)) {
			ball -> setBallXDir(1);
		} 
		// 球左下碰到磚塊右上
		else if(stick -> getRect().contains(pointLeftBottom)) {
			ball -> setBallXDir(1);
		}
	}
	
	if(stage == 1){
		for(int i = 0; i < N_OF_BRICKS_1; i++){
			if( i % 2 == 1)
				hit[i] = 1;
		}
		for (int i = 0; i < N_OF_BRICKS_1; i++) {
			// 碰到磚塊
			if ((ball -> getRect()).intersects(bricks_1[i] -> getRect())) {
							
				int ballLeft = ball -> getRect().left();  
				int ballHeight = ball -> getRect().height(); 
				int ballWidth = ball -> getRect().width();
				int ballTop = ball -> getRect().top();
				
				QPoint pointRightTop(ballLeft + ballWidth + 1, ballTop - 1);
				QPoint pointRightBottom(ballLeft + ballWidth + 1, ballTop + ballHeight + 1);
				
				QPoint pointLeftTop(ballLeft - 1, ballTop - 1); 
				QPoint pointLeftBottom(ballLeft - 1, ballTop + ballHeight + 1); 
				
				QPoint pointTop(ballLeft + ballWidth / 2, ballTop - 1);
				QPoint pointBottom(ballLeft + ballWidth / 2, ballTop + ballHeight + 1); 
				
				QPoint pointRight(ballLeft + ballWidth + 1, ballTop + ballHeight / 2);
				QPoint pointLeft(ballLeft - 1, ballTop + ballTop + ballHeight / 2); 

				if (!bricks_1[i] -> isDestroyed()) {
					// 球右邊碰到磚塊左邊
					if(bricks_1[i] -> getRect().contains(pointRight)) {
						ball -> setBallXDir(-1);
					} 
					// 球左邊碰到磚塊右邊
					else if(bricks_1[i] -> getRect().contains(pointLeft)) {
						ball -> setBallXDir(1);
					}
					// 球上面碰到磚塊下面
					else if(bricks_1[i] -> getRect().contains(pointTop)) {
						ball -> setBallYDir(1);
					} 
					// 球下面碰到磚塊上面
					else if(bricks_1[i] -> getRect().contains(pointBottom)) {
						ball -> setBallYDir(-1);
					} 
					// 球右上碰到磚塊左下
					else if(bricks_1[i] -> getRect().contains(pointRightTop)) {
						ball -> setBallXDir(-1);
					} 
					// 球右下碰到磚塊左上
					else if(bricks_1[i] -> getRect().contains(pointRightBottom)) {
						ball -> setBallXDir(-1);
					} 
					// 球左上碰到磚塊右下
					else if(bricks_1[i] -> getRect().contains(pointLeftTop)) {
						ball -> setBallXDir(1);
					} 
					// 球左下碰到磚塊右上
					else if(bricks_1[i] -> getRect().contains(pointLeftBottom)) {
						ball -> setBallXDir(1);
					} 
					if(hit[i] == 1){
						bricks_1[i] -> setDestroyed(true);
					}
					else{
						hit[i] = 1;
						//QSound::play("./dataset/boom.wav");
					}
				}
			}
		}
	}
	if(stage == 2){
		for (int i = 0; i < N_OF_BRICKS_2; i++) {
			// 碰到磚塊
			if ((ball -> getRect()).intersects(bricks_2[i] -> getRect())) {
							
				int ballLeft = ball -> getRect().left();  
				int ballHeight = ball -> getRect().height(); 
				int ballWidth = ball -> getRect().width();
				int ballTop = ball -> getRect().top();
				
				QPoint pointRightTop(ballLeft + ballWidth, ballTop);
				QPoint pointRightBottom(ballLeft + ballWidth, ballTop + ballHeight);
				
				QPoint pointLeftTop(ballLeft, ballTop); 
				QPoint pointLeftBottom(ballLeft, ballTop + ballHeight); 
				
				QPoint pointTop(ballLeft + ballWidth / 2, ballTop);
				QPoint pointBottom(ballLeft + ballWidth / 2, ballTop + ballHeight); 
				
				QPoint pointRight(ballLeft + ballWidth, ballTop + ballHeight / 2);
				QPoint pointLeft(ballLeft, ballTop + ballTop + ballHeight / 2); 

				if (!bricks_2[i] -> isDestroyed()) {
					// 球右邊碰到磚塊左邊
					if(bricks_2[i] -> getRect().contains(pointRight)) {
						ball -> setBallXDir(-1);
					} 
					// 球左邊碰到磚塊右邊
					else if(bricks_2[i] -> getRect().contains(pointLeft)) {
						ball -> setBallXDir(1);
					}
					// 球上面碰到磚塊下面
					else if(bricks_2[i] -> getRect().contains(pointTop)) {
						ball -> setBallYDir(1);
					} 
					// 球下面碰到磚塊上面
					else if(bricks_2[i] -> getRect().contains(pointBottom)) {
						ball -> setBallYDir(-1);
					} 
					// 球右上碰到磚塊左下
					else if(bricks_2[i] -> getRect().contains(pointRightTop)) {
						ball -> setBallXDir(-1);
					} 
					// 球右下碰到磚塊左上
					else if(bricks_2[i] -> getRect().contains(pointRightBottom)) {
						ball -> setBallXDir(-1);
					} 
					// 球左上碰到磚塊右下
					else if(bricks_2[i] -> getRect().contains(pointLeftTop)) {
						ball -> setBallXDir(1);
					} 
					// 球左下碰到磚塊右上
					else if(bricks_2[i] -> getRect().contains(pointLeftBottom)) {
						ball -> setBallXDir(1);
					} 
					bricks_2[i] -> setDestroyed(true);
					//QSound::play("./dataset/boom.wav");
				}
			}
		}
	}
	if(stage == 3){
		for (int i = 0; i < N_OF_BRICKS_3; i++) {
			// 碰到磚塊
			if ((ball -> getRect()).intersects(bricks_3[i] -> getRect())) {
							
				int ballLeft = ball -> getRect().left();  
				int ballHeight = ball -> getRect().height(); 
				int ballWidth = ball -> getRect().width();
				int ballTop = ball -> getRect().top();
				
				QPoint pointRightTop(ballLeft + ballWidth + 1, ballTop - 1);
				QPoint pointRightBottom(ballLeft + ballWidth + 1, ballTop + ballHeight + 1);
				
				QPoint pointLeftTop(ballLeft - 1, ballTop - 1); 
				QPoint pointLeftBottom(ballLeft - 1, ballTop + ballHeight + 1); 
				
				QPoint pointTop(ballLeft + ballWidth / 2, ballTop - 1);
				QPoint pointBottom(ballLeft + ballWidth / 2, ballTop + ballHeight + 1); 
				
				QPoint pointRight(ballLeft + ballWidth + 1, ballTop + ballHeight / 2);
				QPoint pointLeft(ballLeft - 1, ballTop + ballTop + ballHeight / 2); 

				if (!bricks_3[i] -> isDestroyed()) {
					// 球右邊碰到磚塊左邊
					if(bricks_3[i] -> getRect().contains(pointRight)) {
						ball -> setBallXDir(-1);
					} 
					// 球左邊碰到磚塊右邊
					else if(bricks_3[i] -> getRect().contains(pointLeft)) {
						ball -> setBallXDir(1);
					}
					// 球上面碰到磚塊下面
					else if(bricks_3[i] -> getRect().contains(pointTop)) {
						ball -> setBallYDir(1);
					} 
					// 球下面碰到磚塊上面
					else if(bricks_3[i] -> getRect().contains(pointBottom)) {
						ball -> setBallYDir(-1);
					} 
					// 球右上碰到磚塊左下
					else if(bricks_3[i] -> getRect().contains(pointRightTop)) {
						ball -> setBallXDir(-1);
					} 
					// 球右下碰到磚塊左上
					else if(bricks_3[i] -> getRect().contains(pointRightBottom)) {
						ball -> setBallXDir(-1);
					} 
					// 球左上碰到磚塊右下
					else if(bricks_3[i] -> getRect().contains(pointLeftTop)) {
						ball -> setBallXDir(1);
					} 
					// 球左下碰到磚塊右上
					else if(bricks_3[i] -> getRect().contains(pointLeftBottom)) {
						ball -> setBallXDir(1);
					} 
					bricks_3[i] -> setDestroyed(true);
					//QSound::play("./dataset/boom.wav");
				}
			}
		}
	}
}
