#pragma once

#include "MyString.h"
#include "User.h"
#include "Chat.h"

void clearConsole();
void printLoginScreen();
MyString getCurrentDate();
MyString getCurrentTime();
void freeMemory(MyVector<User*>& users, MyVector<Chat*>& chats);