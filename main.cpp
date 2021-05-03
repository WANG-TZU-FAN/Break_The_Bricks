#include <QApplication>
#include "breakout.h"

int main(int argc, char *argv[]) {

	QApplication app(argc, argv);  

	Breakout gamewindow;
	
	gamewindow.setWindowTitle("Let's Play a game!");

	gamewindow.resize(600, 800);
  
	gamewindow.show();

	return app.exec();
}
