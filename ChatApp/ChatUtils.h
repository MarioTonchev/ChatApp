#pragma once

#include "MyString.h"
#include "MyVector.hpp"
#include "User.h"
#include "FileHandler.h"

class Chat;

Chat* findChatById(int id, MyVector<Chat*>& chats);
Chat* findIndividualChatByUsernames(const MyString& username1, const MyString& username2, MyVector<Chat*>& chats);
Chat* findGroupChatByName(const MyString& groupName, MyVector<Chat*>& chats);
const MyString getChatType(Chat* chat);
void deleteChat(Chat* chat, MyVector<Chat*>& chats, FileHandler* fileHandler);