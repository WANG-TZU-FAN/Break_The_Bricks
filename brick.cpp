#include <iostream>
#include <cstdlib>
#include "brick.h"

int Brick::xPos = 0;
int Brick::yPos = 0;

Brick::Brick(int x, int y, int double_hits) {
	xPos = x;
	yPos = y;
	
	if(double_hits % 2 == 1){
		image.load("./dataset/brick.png");
	}
	else{
		image.load("./dataset/brick_2.png");
	}
	
	destroyed = false;
	rect = image.rect();
	rect.translate(xPos, yPos);
}

Brick::~Brick() {
	// 在介面輸出磚塊已被刪除
	std::cout << ("Brick deleted") << std::endl;
}

QRect Brick::getRect() {
	
	return rect;
}

void Brick::setRect(QRect rect_Brick) {

	rect = rect_Brick;
}

QImage & Brick::getImage() {
	
	return image;
}


void Brick::brick_move() {
	// 磚塊右移一
	int x = rect.x();
	x = x + 1;
	// 碰到右邊界回到左邊界
	if(x >= RIGHT_EDGE){
		x = LEFT_EDGE;
	}
	// y 方向不變
	float y = rect.top();
    rect.moveTo(x, y);
	
}

bool Brick::isDestroyed() {
	
	return destroyed;
}

void Brick::setDestroyed(bool destroyed_brick) {

	destroyed = destroyed_brick;
}
