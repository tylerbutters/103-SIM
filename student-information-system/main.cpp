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
	using std::stoi;
	using std::to_string;
}

using namespace stdprefixes;

void printMainMenu();
int main();

struct AccountDetails {
	int accountType = 0;
	string username;
	string password;
};

string g_accountsFile = ("user-accounts.csv");

void drawLine() {
	// just draws line
	cout << "---------------------------------------------------------------------" << '\n';
}

void printStudentDetails(int accountType) {

}

void StudentAccountMenu() {
	int userChoice;
	int accountType = 1;

	drawLine();
	cout << "STUDENT";
	drawLine();

	cout << "Heres your information" << '\n';
	printStudentDetails(accountType);
	cout << "[LOG OUT = 0]";
	cin >> userChoice;

	switch (userChoice) {
	case 0:
		printMainMenu();
	}
}

void switchAccount(AccountDetails userAccountDetails) {

}

bool authenticateUser(vector<AccountDetails> &listOfAccounts, AccountDetails &userInfoToAuthenticate) {
	// loops through each row of file and vector
	for (AccountDetails userLoginDetails : listOfAccounts) {
		// checks if inputed details match in database
		if (userInfoToAuthenticate.username == userLoginDetails.username && userInfoToAuthenticate.password == userLoginDetails.password) {
			return true;
		}
	}
	listOfAccounts.clear();

	// if authentication fails
	return false;
}

AccountDetails getLoginDetailsFromUser() {
	AccountDetails userLoginDetails;

	cout << '\n' << "Enter your username: ";
	cin >> userLoginDetails.username;
	cout << "Enter your password: ";
	cin >> userLoginDetails.password;

	return userLoginDetails;
}

vector<AccountDetails> loadAccounts() {
	string fileRow;
	string fileCell;
	vector<AccountDetails> listOfAccounts;
	fstream AccountsFile;

	AccountsFile.open(g_accountsFile, ios::in | ios::app);

	if (!AccountsFile.is_open()) {
		throw std::runtime_error("Warning file is not open");
	}

	// loops through each row of file
	while (getline(AccountsFile, fileRow, '\n')) {
		stringstream stream(fileRow);
		AccountDetails details;

		// loops through each cell of file and adds to struct
		int i = 1;
		while (getline(stream, fileCell, ',')) {
			if (i == 1) {
				details.username = fileCell;
			}
			if (i == 2) {
				details.password = fileCell;
			}
			if (i == 3) {
				details.accountType = stoi(fileCell);
			}

			i++;
		}
		// adds populated struct to vector
		listOfAccounts.push_back(details);
	}
	AccountsFile.close();

	return listOfAccounts;
}

AccountDetails getLoginDetailsFromUserAndAuthenticate() {
	int loginAttempts = 3;

	cout << '\n';
	drawLine();
	cout << "LOGIN" << '\n';
	drawLine();

	vector<AccountDetails> listOfAccounts = loadAccounts();

	while (loginAttempts > 0) {
		//gets user input
		AccountDetails userLoginDetails = getLoginDetailsFromUser();
		//authenticates details
		bool isAuthenticated = authenticateUser(listOfAccounts, userLoginDetails);

		if (isAuthenticated) {
			cout << '\n' << "Logged in!" << '\n';
			return userLoginDetails;
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
	int menuChoice;

	cout << '\n';
	drawLine();
	cout << "WELLINGTON HIGH SCHOOL STUDENT INFORMATION SYSTEM" << '\n';
	drawLine();

	cout << '\n' << "Please input the number to the following option" << '\n';
	cout << '\n' << "[LOGIN = 1] [EXIT = 0]" << '\n';
	cin >> menuChoice;

	return menuChoice;
}

void printMainMenu() {
	AccountDetails userAccountDetails;

	switch (getMainMenuChoiceFromUser()) {
	case 1:
		userAccountDetails = getLoginDetailsFromUserAndAuthenticate();
		switchAccount(userAccountDetails);

		break;
	case 0:
		cout << "Shutting down application..." << '\n';
		exit(0);
		break;
	default:
		cout << '\n' << "Please choose one of the options";
		printMainMenu();
		break;
	}
}

int main() {
	printMainMenu();

	
}