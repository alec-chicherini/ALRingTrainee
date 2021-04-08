
#include "window.h"

int main(int argc, char** argv) {
	QApplication app(argc,argv);

	window mW;
	mW.show();
	

	return app.exec();
}