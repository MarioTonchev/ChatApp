#pragma once

#include "MyString.h"
#include "MyVector.hpp"
#include "Chat.h"
#include "IndividualChat.h"
#include "GroupChat.h"

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

	void addChat(Chat*& chat);
	void viewChats() const;

	void createIndividualChat(const MyString& username, MyVector<User*>& users, MyVector<Chat*>& chats);
	void selectChat(int chatId, MyVector<Chat*>& chats);
	void createGroupChat(const MyString& groupName, MyVector<MyString>& usernames, MyVector<User*>& users, MyVector<Chat*> chats);
	void leaveGroupChat(int chatId);
};