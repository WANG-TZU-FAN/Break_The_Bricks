#include <iostream>
#include "stick.h"

using namespace std;

Stick::Stick() {
	
	image.load("./dataset/stick.png"); 
	rect = image.rect();
	resetStickState();
	
	dx_stick = 0; 
}

Stick::~Stick() {
	// 在介面輸出板子已被刪除
	cout << ("Stick deleted") << endl;
}

void Stick::setStick_dx(int x) {
	dx_stick = x;
}

void Stick::stick_move() {
	
	int x = rect.x() + 2 * dx_stick;
	if(x >= RIGHT_EDGE){
		x = RIGHT_EDGE;
	}
	if(x <= LEFT_EDGE){
		x = LEFT_EDGE;
	}
	int y = rect.top();
    rect.moveTo(x, y);
}

void Stick::resetStickState() {
	// 重置到初始位置
	rect.moveTo(INITIAL_X, INITIAL_Y);
}

// 取得 Rect 的各參數
QRect Stick::getRect() {
	return rect;
}

// 取得 Image
QImage & Stick::getImage() {
	
	return image;
}
