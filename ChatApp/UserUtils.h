#pragma once

#include "MyString.h"
#include "MyVector.hpp"
#include "HelperFunctions.h"
#include "IOManager.h"

class User;

User* findUser(const MyString& username, MyVector<User*>& users);
const MyString getUserType(User*& user);
bool setCurrentUser(const MyString& username, const MyString& password, MyVector<User*>& users, User*& loggedUser);
void createAccount(const MyString& username, const MyString& password, MyVector<User*>& users);
void login(const MyString& username, const MyString& password, MyVector<User*>& users, User*& loggedUser, MyString& userType);