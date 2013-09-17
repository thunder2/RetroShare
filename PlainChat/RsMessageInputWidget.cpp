#include <QAbstractButton>

#include "RsMessageInputWidget.h"
#include "ui_RsMessageInputWidget.h"

RsMessageInputWidget::RsMessageInputWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::RsMessageInputWidget)
{
	mSendButton = NULL;
	mSendType = SENDTYPE_CTRLENTER;

	ui->setupUi(this);

	connect(ui->plainTextEdit, SIGNAL(textChanged()), this, SLOT(slotTextChanged()));
	connect(this, SIGNAL(sendText()), this, SLOT(slotSendText()));

	ui->plainTextEdit->installEventFilter(this);
}

RsMessageInputWidget::~RsMessageInputWidget()
{
	delete ui;
}

void RsMessageInputWidget::focusInEvent (QFocusEvent */*event*/)
{
	ui->plainTextEdit->setFocus();
}

bool RsMessageInputWidget::eventFilter(QObject *obj, QEvent *event)
{
	if (obj == ui->plainTextEdit) {
		if (event->type() == QEvent::KeyPress) {
			QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
			if (keyEvent) {
				if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return) {
					// Enter pressed
					if (mSendType == SENDTYPE_CTRLENTER) {
						if (keyEvent->modifiers() & Qt::ControlModifier) {
							// send message with Ctrl+Enter
							emit sendText();
							return true; // eat event
						}
					} else {
						if (keyEvent->modifiers() & Qt::ControlModifier) {
							// insert return
							ui->plainTextEdit->textCursor().insertText("\n");
						} else {
							// send message with Enter
							emit sendText();
						}
						return true; // eat event
					}
				}
			}
		}
	}

	// pass the event on to the parent class
	return QWidget::eventFilter(obj, event);
}

void RsMessageInputWidget::slotTextChanged()
{
	updateSendButton();
	emit textChanged();
}

void RsMessageInputWidget::setSendButton(QAbstractButton *button)
{
	mSendButton = button;
	updateSendButton();
}

void RsMessageInputWidget::updateSendButton()
{
	if (mSendButton) {
		mSendButton->setDisabled(getText().isEmpty());
	}
}

QString RsMessageInputWidget::getText()
{
	return ui->plainTextEdit->toPlainText();
}

void RsMessageInputWidget::clearText()
{
	ui->plainTextEdit->clear();
}

void RsMessageInputWidget::slotSendText()
{
	if (mSendButton) {
		mSendButton->click();
	}
}
