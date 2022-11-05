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

struct AccountDetails {
	int accountType;
	string username;
	string password;
};

string g_accountsFile = ("user-database.csv");

void drawLine() {
	// just draws line
	cout << "---------------------------------------------------------------------" << '\n';
}

void addUserToDatabase(AccountDetails userRegistrationDetails) {
	fstream AccountsFile(g_accountsFile, ios::in | ios::app);

	if (!AccountsFile.is_open()) {
		throw std::runtime_error("Warning file is not open");
	}

	// writes user input to file
	AccountsFile << userRegistrationDetails.username << "," << userRegistrationDetails.password << "," << userRegistrationDetails.accountType << '\n';
	AccountsFile.close();
}

AccountDetails getRegistrationDetailsFromUser() {
	AccountDetails userRegistrationDetails;

	cout << '\n';
	drawLine();
	cout << "REGISTER" << '\n';
	drawLine();

	cout << '\n' << "Choose the account you want to create" << '\n';
	cout << '\n' << "[STUDENT = 1] [PARENT = 2] [TEACHER = 3] [ADMIN = 4] [BACK = 0]" << '\n';
	cin >> userRegistrationDetails.accountType;

	switch (userRegistrationDetails.accountType) {
	case 1:
		cout << "Student selected" << '\n';
		break;
	case 2:
		cout << "Parent selected" << '\n';
		break;
	case 3:
		cout << "Teacher selected" << '\n';
		break;
	case 4:
		cout << "Admin selected" << '\n';
		break;
	case 0:
		// back to main menu
		printMainMenu();
		break;
	default:
		cout << '\n' << "Invalid user type selected";
		break;
	}

	cout << '\n' << "Create a username: ";
	cin >> userRegistrationDetails.username;
	cout << "Create a password: ";
	cin >> userRegistrationDetails.password;

	return userRegistrationDetails;
}

void registerNewUserThenReturn() {
	// assigns new user info into struct
	AccountDetails userRegistrationDetails = getRegistrationDetailsFromUser();

	addUserToDatabase(userRegistrationDetails);

	cout << '\n' << "Account created!" << '\n';
	printMainMenu();
}

bool authenticateUser(vector<AccountDetails> listOfAccounts, AccountDetails userInfoToAuthenticate) {
	// loops through each row of file and vector
	for (AccountDetails userLoginDetails : listOfAccounts) {
		// checks if inputed details match in database
		if (userInfoToAuthenticate.username == userLoginDetails.username && userInfoToAuthenticate.password == userLoginDetails.password && userInfoToAuthenticate.accountType == userLoginDetails.accountType) {
			return true;
		}
	}
	listOfAccounts.clear();

	// if authentication fails
	return false;
}

AccountDetails getLoginDetailsFromUser() {
	AccountDetails userLoginDetails;

	cout << '\n' << "Select account type" << '\n';
	cout << '\n' << "[STUDENT = 1] [PARENT = 2] [TEACHER = 3] [ADMIN = 4] [BACK = 0]" << '\n';
	cin >> userLoginDetails.accountType;

	// back to main menu
	if (userLoginDetails.accountType == 0) {
		printMainMenu();
	}

	cout << '\n' << "Enter your username: ";
	cin >> userLoginDetails.username;
	cout << "Enter your password: ";
	cin >> userLoginDetails.password;

	return userLoginDetails;
}

vector<AccountDetails> loadAllUsers() {
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
				details.accountType = std::stoi(fileCell);
			}

			i++;
		}
		// adds populated struct to vector
		listOfAccounts.push_back(details);
	}
	AccountsFile.close();

	return listOfAccounts;
}

void getLoginDetailsFromUserAndAuthenticate() {
	int loginAttempts = 3;
	vector<AccountDetails> listOfAccounts = loadAllUsers();

	cout << '\n';
	drawLine();
	cout << "LOGIN" << '\n';
	drawLine();

	while (loginAttempts > 0) {
		//gets user input
		AccountDetails userLoginDetails = getLoginDetailsFromUser();
		//authenticates details
		bool isAuthenticated = authenticateUser(listOfAccounts, userLoginDetails);

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
	int menuChoice;

	cout << '\n';
	drawLine();
	cout << "WELLINGTON HIGH SCHOOL STUDENT INFORMATION SYSTEM" << '\n';
	drawLine();

	cout << '\n' << "Please input the number to the following option" << '\n';
	cout << '\n' << "[LOGIN = 1] [REGISTER = 2] [EXIT = 0]" << '\n';
	cin >> menuChoice;

	return menuChoice;
}

void printMainMenu() {
	switch (getMainMenuChoiceFromUser()) {
	case 1:
		getLoginDetailsFromUserAndAuthenticate();
		break;
	case 2:
		registerNewUserThenReturn();
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