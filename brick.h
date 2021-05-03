#include <QImage>
#include <QRect>

class Brick {

	public:
		// ctor
		Brick(int, int, int);
		// dtor
		~Brick();
	
	public:
		// 用來判斷磚塊是否被打碎
		bool isDestroyed();
		
		// 用來設定磚塊是否被打碎的狀態
		void setDestroyed(bool);
		
		// 設定矩形
		QRect getRect();
		void setRect(QRect);
		
		// 拿 brick 的圖
		QImage & getImage();
		
		// **** 加分項目：磚塊的移動 ****
		void brick_move();
		
		// 存取磚塊的座標資訊
		static int xPos;
		static int yPos;
		

	private:
	
		QImage image;
		QRect rect;
		
		bool destroyed;
		
		// ******* 磚塊的極限位置 *******
		static const int RIGHT_EDGE = 520;
		static const int LEFT_EDGE = 0;
};
