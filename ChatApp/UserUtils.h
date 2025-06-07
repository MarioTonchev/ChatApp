#pragma once

#include "MyString.h"
#include "MyVector.hpp"
#include "HelperFunctions.h"
#include "FileHandler.h"

class User;

User* findUser(const MyString& username, const MyVector<User*>& users);
const MyString getUserType(User*& user);
bool setCurrentUser(const MyString& username, const MyString& password, MyVector<User*>& users, User*& loggedUser);
void createRegularAccount(const MyString& username, const MyString& password, MyVector<User*>& users, FileHandler* fileHandler);
void createAdminAccout(const MyString& username, const MyString& password, MyVector<User*>& users, FileHandler* fileHandler);
void login(const MyString& username, const MyString& password, MyVector<User*>& users, User*& loggedUser, MyString& userType, FileHandler* fileHandler);