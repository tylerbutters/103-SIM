#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <vector>
#include <string>
#include <fstream>
#include <ctype.h>

namespace stdprefixes {
	using std::cout;
	using std::cin;
	using std::string;
	using std::vector;
	using std::endl;
	using std::ifstream;
	using std::fstream;
	using std::ostream;
	using std::ios;
	using std::stringstream;
}

using namespace stdprefixes;

void printMainMenu();

struct UserInfo {
	int accountType;
	string username;
	string password;
};

string g_userDatabaseFile = ("user-database.csv");

void drawLine() {
	// just draws line
	cout << '\n' << "---------------------------------------------------------------------" << '\n';
}

bool authenticateUser(vector<UserInfo> userList, UserInfo userInfoToAuthenticate) {
	// goes through each row of user vector
	for (UserInfo user : userList) {
		//checks if inputed details match in database
		if (userInfoToAuthenticate.username == user.username && userInfoToAuthenticate.password == user.password && userInfoToAuthenticate.accountType == user.accountType) {
			return true;
		}
	}
	// databaseContent.clear();

	return false;
}

UserInfo getLoginDetailsFromUser() {
	UserInfo loginDetailsFromUser;

	cout << '\n' << "Select account type" << '\n';
	cout << '\n' << "[STUDENT = 1] [PARENT = 2] [TEACHER = 3] [ADMIN = 4] [BACK = 0]" << '\n';
	cin >> loginDetailsFromUser.accountType;

	// back to main menu
	if (loginDetailsFromUser.accountType == 0) {
		printMainMenu();
	}

	cout << '\n' << "Enter your username: ";
	cin >> loginDetailsFromUser.username;
	cout << "Enter your password: ";
	cin >> loginDetailsFromUser.password;

	return loginDetailsFromUser;
}

vector<UserInfo> loadAllStudents() {
	string databaseRow;
	string databaseCell;
	vector<UserInfo> userList;

	fstream userDatabase;

	userDatabase.open(g_userDatabaseFile, ios::in | ios::app);

	if (!userDatabase.is_open()) {
		throw std::runtime_error("Warning file is not open");
	}

	// loops through each row of database file
	while (getline(userDatabase, databaseRow, '\n')) {
		UserInfo info;
		//vectorRow.clear();
		stringstream stream(databaseRow);

		// loops through each cell of file, adds to struct
		int i = 1;
		while (getline(stream, databaseCell, ',')) {
			if (i == 1) {
				info.username = databaseCell;
			}
			if (i == 2) {
				info.password = databaseCell;
			}
			if (i == 3) {
				info.accountType = std::stoi(databaseCell);
			}

			i++;
		}

		userList.push_back(info);
	}
	userDatabase.close();

	return userList;
}

void getLoginDetailsFromUserAndAuthenticate() {
	int loginAttempts = 3;
	vector<UserInfo> allUsers = loadAllStudents();

	drawLine();
	cout << "LOGIN" << '\n';
	drawLine();

	while (loginAttempts > 0) {
		UserInfo userLoginDetails = getLoginDetailsFromUser();
		bool isAuthenticated = authenticateUser(allUsers, userLoginDetails);

		if (isAuthenticated) {
			return;
		}

		loginAttempts--;
		cout << '\n' << "Wrong username, password or associated account type";
		cout << '\n' << loginAttempts << " attempts left";
	}

	if (loginAttempts == 0) {
		cout << '\n' << "Shutting down application..." << '\n';
		exit(0);
	}
}

int getMainMenuChoiceFromUser() {
	int userChoice;

	drawLine();
	cout << "WELLINGTON HIGH SCHOOL STUDENT INFORMATION SYSTEM" << '\n';
	drawLine();

	cout << '\n' << "Please input the number to the following option" << '\n';
	cout << '\n' << "[LOGIN = 1] [EXIT = 0]" << '\n';
	cin >> userChoice;

	return userChoice;
}

void printMainMenu() {
	switch (getMainMenuChoiceFromUser()) {
	case 1:
		getLoginDetailsFromUserAndAuthenticate();
		break;
	case 0:
		cout << "Shutting down application..." << '\n';
		break;
	default:
		cout << '\n' << "Please choose one of the options";
		printMainMenu();
		break;
	}
}

int main() {
	printMainMenu();

	cout << '\n' << "Logged in!" << '\n';
}