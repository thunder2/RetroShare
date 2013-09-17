#include "PlainChat.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	PlainChat w;
	w.show();

	return a.exec();
}
