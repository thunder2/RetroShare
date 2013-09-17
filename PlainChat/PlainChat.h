#ifndef PLAINCHAT_H
#define PLAINCHAT_H

#include <QDialog>

namespace Ui {
class PlainChat;
}

class PlainChat : public QDialog
{
	Q_OBJECT

public:
	explicit PlainChat(QWidget *parent = 0);
	~PlainChat();

private slots:
	void sendText();

private:
	Ui::PlainChat *ui;
};

#endif // PLAINCHAT_H
