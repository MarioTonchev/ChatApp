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

void writeMyStringBin(std::ofstream& os, const MyString& str);
MyString readMyStringBin(std::ifstream& is);

void saveUserToFile(User* user, const MyString& userType);
void saveUserToBinFile(User* user, const MyString& userType);

void loadUsers(MyVector<User*>& users);
void loadUsersBin(MyVector<User*>& users);

void saveChatToFile(Chat* chat, const MyString& chatType);
void saveChatToBinFile(Chat* chat, const MyString& chatType);

void saveChatIdToFile(Chat* chat);
void saveChatIdToBinFile(Chat* chat);

MyVector<int> getChatIds();
MyVector<int> getChatIdsBin();

void saveUserToChatApprovalList(const MyString& username, int chatId);
void saveUserToChatApprovalListBin(const MyString& username, int chatId);

void loadChatApprovalList(MyVector<Chat*>& chats, MyVector<User*>& users);
void loadChatApprovalListBin(MyVector<Chat*>& chats, MyVector<User*>& users);

void loadChats(MyVector<Chat*>& chats, MyVector<User*>& users);
void loadChatsBin(MyVector<Chat*>& chats, MyVector<User*>& users);

void saveUserChatToFile(const MyString& username, int chatId);
void saveUserChatToBinFile(const MyString& username, int chatId);

void loadUserChats(MyVector<User*>& users, MyVector<Chat*>& chats);
void loadUserChatsBin(MyVector<User*>& users, MyVector<Chat*>& chats);

void deleteChatIdFromFile(int chatId);
void deleteChatIdFromBinFile(int chatId);

void deleteChatFromFile(int chatId);
void deleteChatFromBinFile(int chatId);

void deleteUserFromFile(const MyString& username);
void deleteUserFromBinFile(const MyString& username);

void deleteUserChatRelation(const MyString& username, int chatId);
void deleteUserChatRelationBin(const MyString& username, int chatId);

void deleteUserFromApprovaList(const MyString& username, int chatId);
void deleteUserFromApprovaListBin(const MyString& username, int chatId);