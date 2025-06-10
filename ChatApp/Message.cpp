#include <ctime>

#include "Message.h"
#include "HelperFunctions.h"

using namespace std;

Message::Message(const MyString& sender, const MyString& content) {
	this->sender = sender;
	this->content = content;
	this->dateSent = getCurrentDate();
	this->timeSent = getCurrentTime();
}

Message::Message(const MyString& sender, const MyString& content, const MyString& date, const MyString& time) {
	this->sender = sender;
	this->content = content;
	this->dateSent = date;
	this->timeSent = time;
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