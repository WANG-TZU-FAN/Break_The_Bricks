#include <QWidget>
#include <vector>
#include <QKeyEvent>
#include "ball.h"
#include "brick.h"
#include "stick.h"

class Breakout : public QWidget {

	public:
		// Default Ctor
		Breakout(QWidget *parent = 0);
		// Dtor
		~Breakout();

	protected:
		
		// 把物件的即時資料輸出到視窗上
		void paintEvent(QPaintEvent *);
		void timerEvent(QTimerEvent *);
		void keyPressEvent(QKeyEvent *);
		void keyReleaseEvent(QKeyEvent *);
		void drawObjects(QPainter *);
		void finishGame(QPainter *, QString);
		void moveObjects();
		void setBricks();
		void startGame();
		void pauseGame();
		void stopGame();
		void resetGame();
		void victory();
		void checkCollision();

	private:
		int x;
		int timerInstantaneous = 0;
		// 間隔時間（偵數）
		static const int DELAY = 1;
		// 各關的磚塊數
		static const int N_OF_BRICKS_1 = 22;
		static const int N_OF_BRICKS_2 = 42;
		static const int N_OF_BRICKS_3 = 62;
		// 螢幕的極限
		static const int BOTTOM_EDGE = 800;
		// 磚塊的大小
		static const int Brick_Size_x = 80;
		static const int Brick_Size_y = 20;
		// 計算生命值
		static int count_life;
		// 計算執行的時間，用來球剩餘時間
		static int count_time;
		// 設定關卡
		static int stage;
		// 設定硬化磚塊
		static int hit[N_OF_BRICKS_1];
		// 產生物件
		Ball *ball;
		Ball *ball_2;
		Stick *stick;
		Brick *bricks_1[N_OF_BRICKS_1];
		Brick *bricks_2[N_OF_BRICKS_2];
		Brick *bricks_3[N_OF_BRICKS_3];
		// 遊戲的狀態
		bool gameOver;
		bool gameWon;
		bool gameStarted;
		bool gameReset;
		bool paused;
};

