#ifndef RSMESSAGEINPUTWIDGET_H
#define RSMESSAGEINPUTWIDGET_H

#include <QWidget>

class QAbstractButton;

namespace Ui {
class RsMessageInputWidget;
}

class RsMessageInputWidget : public QWidget
{
	Q_OBJECT

public:
	enum SendType
	{
		SENDTYPE_ENTER,
		SENDTYPE_CTRLENTER
	};

public:
	explicit RsMessageInputWidget(QWidget *parent = 0);
	~RsMessageInputWidget();

	void setSendButton(QAbstractButton *button);
	void setSendType(SendType sendType) { mSendType = sendType; }

	QString getText();
	void clearText();

signals:
	void textChanged();
	void sendText();

private slots:
	void slotTextChanged();
	void slotSendText();

protected:
	void focusInEvent (QFocusEvent *event);
	bool eventFilter(QObject *obj, QEvent *event);

private:
	void updateSendButton();

private:
	SendType mSendType;
	QAbstractButton *mSendButton;
	Ui::RsMessageInputWidget *ui;
};

#endif // RSMESSAGEINPUTWIDGET_H
