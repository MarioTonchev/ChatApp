#pragma once

#include <iostream>
#include <fstream>
#include <stdexcept>

#include "User.h"
#include "RegularUser.h"
#include "Admin.h"
#include "MyString.h"
#include "MyVector.hpp"
#include "UserUtils.h"
#include "ChatUtils.h"
#include "FileConstants.h"

void writeMyString(std::ofstream& os, const MyString& str);
void saveUserToFile(User* user, const MyString& userType);
void saveUserToBinFile(User* user, const MyString& userType);

void saveChatToFile(Chat* chat);
void saveChatIdToFile(Chat* chat);
void saveUserChatToFile(const MyString& username, int chatId);
void saveUserToApprovalList(const MyString& username, int chatId);

MyString readMyString(std::ifstream& is);
void loadUsers(MyVector<User*>& users);
void loadUsersBin(MyVector<User*>& users);

MyVector<int> getChatIds();
void loadChats(MyVector<Chat*>& chats, MyVector<User*>& users);
void loadUserChats(MyVector<User*>& users, MyVector<Chat*>& chats);
void loadApprovalList(MyVector<Chat*>& chats, MyVector<User*>& users);

void deleteChatIdFromFile(int chatId);
void deleteChatFromFile(int chatId);
void deleteUserChatRelation(const MyString& username, int chatId);
void deleteUserFromApprovaList(const MyString& username, int chatId);