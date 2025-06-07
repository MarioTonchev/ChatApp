#pragma once

#include "MyString.h"
#include "User.h"
#include "Chat.h"

void clearConsole();
void printActions(const MyString& userType);
void freeMemory(MyVector<User*>& users, MyVector<Chat*>& chats);