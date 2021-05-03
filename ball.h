#include <QImage>
#include <QRect>

class Ball {
	public:
		// ctor
		Ball();
		// dtor
		~Ball();
		
	public:
		// 遊戲開始和重置
		void resetBallState();
		
		// 碰壁要轉彎
		void edgeMove();
		
		// 設定方向
		void setBallXDir(int);
		void setBallYDir(int);
		
		// 設定矩形
		QRect getRect();
		
		// 拿 ball 的圖
		QImage & getImage();
		
		// 定義球的方向
		// 因為在 breakout.cpp 還會用到，所以用 static 定在 public
		static int ballxdirection;
		static int ballydirection;
		
	private:
	
		QImage image;
		QRect rect;
		
		// ******** 球的初始位置 ********
		// *** 螢幕的一半再減掉球半徑 ***
		static const int INITIAL_X = 290;
		
		// ******** 球的初始位置 ********
		// *** 板子初始位置 - 球的直徑 **
		static const int INITIAL_Y = 730; 
		
		// ******** 設定螢幕邊緣 ********		
		static const int RIGHT_EDGE = 600;
};
