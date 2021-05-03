#include <iostream>
#include "ball.h"

using namespace std;

// 設定球一開始往右上走
int Ball::ballxdirection = 1;
int Ball::ballydirection = -1;

Ball::Ball() {
	
	image.load("./dataset/ball.png");
	rect = image.rect();
	resetBallState();
	
}

Ball::~Ball() {
	// 在介面輸出球已被刪除
	cout << ("Ball deleted") << std::endl;
}

void Ball::edgeMove() {
	// 轉向
	rect.translate(ballxdirection, ballydirection);
	
	if (rect.left() == 0) {
		ballxdirection = 1;
	}

	if (rect.right() == RIGHT_EDGE) {
		ballxdirection = -1;
	}

	if (rect.top() == 0) {
		ballydirection = 1;
	}
}

void Ball::resetBallState() {
	rect.moveTo(INITIAL_X, INITIAL_Y);
}

void Ball::setBallXDir(int x) {
	
	ballxdirection = x;
}

void Ball::setBallYDir(int y) {
	
	ballydirection = y;
}


QRect Ball::getRect() {
	
	return rect;
}

QImage & Ball::getImage() {

	return image;
}
