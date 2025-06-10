#pragma once

#include "MyString.h"

class Message {
private:
	MyString sender;
	MyString content;
	MyString dateSent;
	MyString timeSent;

public:
	Message(const MyString& sender = "", const MyString& content = "");
	Message(const MyString& sender, const MyString& content, const MyString& date, const MyString& time);
	const MyString& getSender() const;
	const MyString& getContent() const;
	const MyString& getDate() const;
	const MyString& getTime() const;
};