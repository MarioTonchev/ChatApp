#include <ctime>

#include "Message.h"

using namespace std;

#pragma warning(disable:4996)

Message::Message(const MyString& sender, const MyString& content) {
	this->sender = sender;
	this->content = content;
	setCurrentDate();
	setCurrentTime();
}

void Message::setCurrentDate() {
	time_t t = time(nullptr);
	tm* now = localtime(&t);

	dateSent += now->tm_mday;
	dateSent += '.';
	dateSent += (now->tm_mon + 1);
	dateSent += '.';
	dateSent += (now->tm_year + 1900);
}

void Message::setCurrentTime() {
	time_t t = time(nullptr);
	tm* now = localtime(&t);

	timeSent += now->tm_hour;
	timeSent += ':';

	if (now->tm_min < 10) {
		timeSent += '0';
	}

	timeSent += now->tm_min;
}

const MyString& Message::getSender() const {
	return sender;
}

const MyString& Message::getContent() const {
	return content;
}

const MyString& Message::getDate() const {
	return dateSent;
}

const MyString& Message::getTime() const {
	return timeSent;
}