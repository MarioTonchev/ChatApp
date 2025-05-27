#pragma once

#include "MyString.h"
#include "MyVector.hpp"
#include "Chat.h"

class User {
protected:
	MyString username;
	MyString password;
	MyVector<Chat*> chats;

public:
	User(const MyString& username = "", const MyString& password = "");
	virtual ~User();
	const MyString& getUsername() const;
	const MyString& getPassword() const;
	const MyVector<Chat*>& getChats() const;
};