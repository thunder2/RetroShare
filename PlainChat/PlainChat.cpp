#include "PlainChat.h"
#include "ui_PlainChat.h"

#include "markdown_lib.h"

PlainChat::PlainChat(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PlainChat)
{
	ui->setupUi(this);

	connect(ui->sendPushButton, SIGNAL(clicked()), this, SLOT(sendText()));

	ui->inputWidget->setSendButton(ui->sendPushButton);
	ui->inputWidget->setSendType(RsMessageInputWidget::SENDTYPE_ENTER);
}

PlainChat::~PlainChat()
{
	delete ui;
}

void PlainChat::sendText()
{
	QString text = ui->inputWidget->getText();
	ui->inputWidget->clearText();
	ui->inputWidget->setFocus();

	char *markdown = markdown_to_string(text.toAscii().data(), 0, HTML_FORMAT);

	ui->textBrowser->append(markdown);

	free(markdown);
}
