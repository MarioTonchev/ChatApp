#pragma once

#include "User.h"
#include "RegularUser.h"
#include "MyString.h"
#include "MyVector.hpp"
#include "UserUtils.h"
#include "ChatUtils.h"
#include "FileConstants.h"

class Admin;

class FileHandler {
public:
	virtual ~FileHandler() = default;
	
	virtual void saveUserToFile(RegularUser* user) = 0;
	virtual void saveUserToFile(Admin* user) = 0;
	virtual void loadUsers(MyVector<User*>& users) = 0;

	virtual void saveChatToFile(IndividualChat* individualChat) = 0;
	virtual void saveChatToFile(GroupChat* groupChat) = 0;
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