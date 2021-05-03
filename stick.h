#include <QImage>
#include <QRect>

using namespace std;

class Stick {
	public:
	
		// ctor
		Stick();
		
		// dtor
		~Stick();

	public:
	
		// 遊戲開始和重置
		void resetStickState();
		
		// 移動板子
		void stick_move();
		
		// 設定位置
		void setStick_dx(int);
		
		// 設定矩形
		QRect getRect();
		
		// 拿 Stick 的圖
		QImage & getImage();

	private:
	
		QImage image;
		QRect rect;
		
		// 和之後在 breakout.cpp 中的按鍵設定搭配
		int dx_stick;
		
		// ******** 板子初始位置 ********
		static const int INITIAL_X = 220;
		
		// ******** 板子初始位置 ********
		static const int INITIAL_Y = 750;
		
		// ******** 板子極限位置 ********
		static const int RIGHT_EDGE = 440;
		static const int LEFT_EDGE = 0;
};
