#pragma once

#include "User.h"
#include "RegularUser.h"
#include "Admin.h"
#include "MyString.h"
#include "MyVector.hpp"
#include "UserUtils.h"
#include "ChatUtils.h"
#include "FileConstants.h"

class FileHandler {
public:
	virtual ~FileHandler() = default;
	
	virtual void saveUserToFile(User* user, const MyString& userType) = 0;
	virtual void loadUsers(MyVector<User*>& users) = 0;

	virtual void saveChatToFile(Chat* chat, const MyString& chatType) = 0;
	virtual void loadChats(MyVector<Chat*>& chats, MyVector<User*>& users) = 0;

	virtual void saveChatIdToFile(Chat* chat) = 0;
	virtual MyVector<int> getChatIds() = 0;

	virtual void saveUserToChatApprovalList(const MyString& username, int chatId) = 0;
	virtual void loadChatApprovalList(MyVector<Chat*>& chats, MyVector<User*>& users) = 0;

	virtual void saveUserChatToFile(const MyString& username, int chatId) = 0;
	virtual void loadUserChats(MyVector<User*>& users, MyVector<Chat*>& chats) = 0;

	virtual void deleteChatIdFromFile(int chatId) = 0;
	virtual void deleteChatFromFile(int chatId) = 0;
	virtual void deleteUserFromFile(const MyString& username) = 0;
	virtual void deleteUserChatRelation(const MyString& username, int chatId) = 0;
	virtual void deleteUserFromApprovaList(const MyString& username, int chatId) = 0;
};