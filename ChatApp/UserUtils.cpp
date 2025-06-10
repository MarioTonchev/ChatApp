#include <iostream>

#include "UserUtils.h"
#include "User.h"
#include "RegularUser.h"
#include "Admin.h"

using namespace std;

User* findUser(const MyString& username, const MyVector<User*>& users) {
	User* user = nullptr;

	for (size_t i = 0; i < users.getSize(); i++)
	{
		if (users[i]->getUsername() == username)
		{
			user = users[i];
			break;
		}
	}

	return user;
}

const MyString getUserType(User*& user) {
	MyString classType = typeid(*user).name();
	MyString userType = classType.split(' ')[1];

	return userType;
}

void createRegularAccount(const MyString& username, const MyString& password, MyVector<User*>& users, FileHandler* fileHandler) {
	if (username == "" || password == "")
	{
		cout << "Username and password cannot be empty!" << endl;
		return;
	}

	User* user = findUser(username, users);

	if (user)
	{
		cout << "User with given username already exists! Please try again." << endl;
		return;
	}

	user = new RegularUser(username, password, fileHandler);

	users.push_back(user);
	fileHandler->saveUserToFile((RegularUser*)user);

	cout << "Account created successfully." << endl;
}

void createAdminAccout(const MyString& username, const MyString& password, MyVector<User*>& users, FileHandler* fileHandler) {
	if (username == "" || password == "")
	{
		cout << "Username and password cannot be empty!" << endl;
		return;
	}

	User* user = findUser(username, users);

	if (user)
	{
		cout << "User with given username already exists! Please try again." << endl;
		return;
	}

	user = new Admin(Admin::createAdminId(users), username, password, fileHandler);

	users.push_back(user);
	fileHandler->saveUserToFile((Admin*)user);

	cout << "Account created successfully." << endl;
}

void login(const MyString& username, const MyString& password, MyVector<User*>& users, User*& loggedUser, MyString& userType, FileHandler* fileHandler) {
	if (username == "" || password == "")
	{
		cout << "Username and password cannot be empty!" << endl;
		return;
	}

	User* user = findUser(username, users);

	bool isLoginSuccessful = false;

	if (user && password == user->getPassword())
	{
		isLoginSuccessful = true;
		loggedUser = user;
		userType = getUserType(loggedUser);
	}
	else if (user && password != user->getPassword())
	{
		cout << "Username or password is incorrect!" << endl;
		return;
	}

	if (isLoginSuccessful)
	{
		clearConsole();

		if (userType == "Admin")
		{
			cout << "You logged in as an admin." << endl << "Username: " << loggedUser->getUsername() << endl << endl;
		}
		else
		{
			cout << "You logged in as a regular user." << endl << "Username: " << loggedUser->getUsername() << endl << endl;
		}

		loggedUser->printActions();
		cout << endl;
	}
	else
	{
		cout << "Account does not exist. Create? (y/n)" << endl;

		MyString input;
		input.getline(cin);

		if (input == "y" || input == "yes")
		{
			createRegularAccount(username, password, users, fileHandler);
		}
	}
}