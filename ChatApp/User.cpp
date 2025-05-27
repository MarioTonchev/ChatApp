#include "User.h"

User::User(const MyString& username, const MyString& password) {
	this->username = username;
	this->password = password;
}

User::~User() {}

const MyString& User::getUsername() const {
	return username;
}

const MyString& User::getPassword() const {
	return password;
}

const MyVector<Chat*>& User::getChats() const {
	return chats;
}