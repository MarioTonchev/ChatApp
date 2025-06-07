#include <iostream>

#include "UserUtils.h"
#include "User.h"
#include "RegularUser.h"

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

bool setCurrentUser(const MyString& username, const MyString& password, MyVector<User*>& users, User*& loggedUser) {
	User* user = findUser(username, users);

	if (!user || user->getPassword() != password)
	{
		return false;
	}

	loggedUser = user;

	return true;
}

const MyString getUserType(User*& user) {
	MyString classType = typeid(*user).name();
	MyString userType = classType.split(' ')[1];

	return userType;
}

void createRegularAccount(const MyString& username, const MyString& password, MyVector<User*>& users) {
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

	user = new RegularUser(username, password);

	users.push_back(user);

	saveUserToFile(user, getUserType(user));

	cout << "Account created successfully." << endl;
}

void createAdminAccout(const MyString& username, const MyString& password, MyVector<User*>& users) {
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

	user = new Admin(Admin::createAdminId(users), username, password);

	users.push_back(user);

	saveUserToFile(user, getUserType(user));

	cout << "Account created successfully." << endl;
}

void login(const MyString& username, const MyString& password, MyVector<User*>& users, User*& loggedUser, MyString& userType) {
	if (username == "" || password == "")
	{
		cout << "Username and password cannot be empty!" << endl;
		return;
	}
	
	while (true)
	{
		bool result = setCurrentUser(username, password, users, loggedUser);

		if (result)
		{
			userType = getUserType(loggedUser);

			clearConsole();

			if (userType == "Admin")
			{
				cout << "You logged in as an admin." << endl << "Username: " << loggedUser->getUsername() << endl << endl;
			}
			else
			{
				cout << "You logged in as a regular user." << endl << "Username: " << loggedUser->getUsername() << endl << endl;
			}

			printActions(userType);
			cout << endl;

			break;
		}
		else
		{
			cout << "Account does not exist. Create? (y/n)" << endl;

			MyString input;
			input.getline(cin);

			if (input == "y" || input == "yes")
			{
				createRegularAccount(username, password, users);
				break;
			}
			else if (input == "n" || input == "no")
			{
				break;
			}
			else
			{
				cout << "Error: Wrong command." << endl;
				break;
			}
		}
	}
}