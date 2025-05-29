#pragma once

#include <iostream>
#include <fstream>
#include <stdexcept>

#include "User.h"
#include "RegularUser.h"
#include "Admin.h"
#include "MyString.h"
#include "MyVector.hpp"
#include "FileConstants.h"

void writeMyString(std::ofstream& os, const MyString& str);
void saveUserToFile(User* user, const MyString& userType);
void saveUserToBinFile(User* user, const MyString& userType);

MyString readMyString(std::ifstream& is);
void loadUsers(MyVector<User*>& users);
void loadUsersBin(MyVector<User*>& users);