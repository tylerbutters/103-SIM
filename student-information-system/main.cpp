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

void printMainMenuOptions();

struct AccountDetails {
	int accountType = 0;
	string username;
	string password;
	int ID = 0;
  
	bool isValid() {
		return username != "";
	};
};

struct StudentDetails {
	int ID = 0;
	string firstName;
	string lastName;
	int yearNum = 0;
	string fatherName;
	string motherName;
	string teacherName;
	int mathsGrade = 0;
	int englishGrade = 0;
	int scienceGrade = 0;
	int chemistryGrade = 0;
	int historyGrade = 0;
	int overallGrade = 0;
};

string g_accountsFileName = ("account-database.csv");
string g_studentsFileName = ("student-database.csv");
int g_columnWidth = 10;
vector<string> g_columnNames = { "[SURNAME]", "[NAME]", "[YEAR]", "[FATHER]", "[MOTHER]", "[TEACHER]", "[MATHS]","[ENGLISH]","[SCIENCE]","[CHEM]","[HISTORY]","[OVERALL]" };

//____________________________________________________________________________________________________________________________
// RECURSSIVE
//____________________________________________________________________________________________________________________________

void checkFile(fstream file) {
	if (!file.is_open()) {
		throw std::runtime_error("Warning file is not open");
	}
}

void printLine() {
	cout << '\n' << "---------------------------------------------------------------------" << '\n';
}

string title(string inputString) {
	int columnLength = g_columnWidth + 1;
	string outputString;

	for (int i = 0; i < inputString.length(); i++) {
		outputString += inputString[i];
		columnLength--;
	}

	while (columnLength != 0) {
		outputString += " ";
		columnLength--;
	}

	return outputString;
}

string column(string inputString) {
	int columnLength = g_columnWidth;
	string outputString;

	for (int i = 0; i < inputString.length(); i++) {
		outputString += inputString[i];
		columnLength--;
	}

	while (columnLength != 0) {
		outputString += " ";
		columnLength--;
	}

	return outputString += "|";
}

string printTableLine() {
	string outputString;
	for (int i = 0; i < g_columnNames.size(); i++) {
		for (int i = 0; i < g_columnWidth; i++) {
			outputString += "_";
		}
		outputString += "_";
	}
	return outputString;
}

vector<StudentDetails> loadStudents() {
	string fileRow;
	string fileCell;
	vector<StudentDetails> listOfAccounts;
	fstream studentsFile;

	studentsFile.open(g_studentsFileName, ios::in | ios::app);

	if (!studentsFile.is_open()) {
		throw std::runtime_error("Warning file is not open");
	}

	// loops through each row of student file
	while (getline(studentsFile, fileRow, '\n')) {
		stringstream stream(fileRow);
		StudentDetails student;

		// loops through each cell of row and adds to struct
		int i = 0;
		while (getline(stream, fileCell, ',')) {
			switch (i) {
			case 0:
				student.ID = stoi(fileCell);
				break;
			case 1:
				student.lastName = fileCell;
				break;
			case 2:
				student.firstName = fileCell;
				break;
			case 3:
				student.yearNum = stoi(fileCell);
				break;
			case 4:
				student.fatherName = fileCell;
				break;
			case 5:
				student.motherName = fileCell;
				break;
			case 6:
				student.teacherName = fileCell;
				break;
			case 7:
				student.mathsGrade = stoi(fileCell);
				break;
			case 8:
				student.englishGrade = stoi(fileCell);
				break;
			case 9:
				student.scienceGrade = stoi(fileCell);
				break;
			case 10:
				student.chemistryGrade = stoi(fileCell);
				break;
			case 11:
				student.historyGrade = stoi(fileCell);
				break;
			case 12:
				student.overallGrade = stoi(fileCell);
				break;
			}

			i++;
		}
		// adds populated struct to vector
		listOfAccounts.push_back(student);
	}
	studentsFile.close();

	return listOfAccounts;
}

vector<AccountDetails> loadAccounts() {
	string fileRow;
	string fileCell;
	vector<AccountDetails> listOfAccounts;
	fstream accountsFile;

	accountsFile.open(g_accountsFileName, ios::in | ios::app);

	if (!accountsFile.is_open()) {
		throw std::runtime_error("Warning file is not open");
	}

	// loops through each row of accounts file
	while (getline(accountsFile, fileRow, '\n')) {
		stringstream stream(fileRow);
		AccountDetails details;

		// loops through each cell of row and adds to struct
		int i = 1;
		while (getline(stream, fileCell, ',')) {
			switch (i) {
			case 1:
				details.username = fileCell;
				break;
			case 2:
				details.password = fileCell;
				break;
			case 3:
				details.accountType = stoi(fileCell);
				break;
			case 4:
				details.ID = stoi(fileCell);
				break;
			}

			i++;
		}
		// adds populated struct to vector
		listOfAccounts.push_back(details);
	}
	accountsFile.close();

	return listOfAccounts;
}

vector<AccountDetails> loadNewAccounts(vector<AccountDetails> listOfAccounts, AccountDetails replacementDetails, AccountDetails currentLoginDetails) {
	vector<AccountDetails> newListOfAccounts;

	// loops through each account in database
	for (AccountDetails account : listOfAccounts) {
		// finds current account details and skips
		if (account.username == currentLoginDetails.username && account.password == currentLoginDetails.password) {
			continue;
		}
		// add all accounts into new vector, exculding current details
		newListOfAccounts.push_back(account);
	}
	// then appends on the new account details
	newListOfAccounts.push_back(replacementDetails);

	return newListOfAccounts;
}

void writeNewAccountsToFile(vector<AccountDetails> newListOfAccounts) {
	string tempAccountsFile = "temp-" + g_accountsFileName;
	char* accountsFileNamePtr = &g_accountsFileName[0];
	char* tempFileNamePtr = &tempAccountsFile[0];
	fstream accountsFile(tempAccountsFile, ios::out);

	if (!accountsFile.is_open()) {
		throw std::runtime_error("Warning file is not open");
	}

	// writes account details to file
	for (AccountDetails account : newListOfAccounts) {
		accountsFile << account.username << "," << account.password << "," << account.accountType << "," << account.ID << '\n';
	}
	accountsFile.close();

	remove(accountsFileNamePtr); // deleting original file
	rename(tempFileNamePtr, accountsFileNamePtr); //rename new file
}

//____________________________________________________________________________________________________________________________
// ACCOUNT OPTIONS
//____________________________________________________________________________________________________________________________

void changeLoginDetails(AccountDetails currentLoginDetails) {
	vector<AccountDetails> listOfAccounts = loadAccounts();
	AccountDetails replacementDetails;

	cout << '\n' << "Current login details" << '\n';
	cout << '\n' << "Username: " << currentLoginDetails.username << '\n';
	cout << "Password: " << currentLoginDetails.password << '\n';
	cout << '\n' << "Enter new login details" << '\n';
	cout << '\n' << "Username: ";
	cin >> replacementDetails.username; // adds new login details to struct
	cout << "Password: ";
	cin >> replacementDetails.password;

	// makes the rest of struct members = current struct members, otherwise they'll just be 0
	replacementDetails.accountType = currentLoginDetails.accountType;
	replacementDetails.ID = currentLoginDetails.ID;

	vector<AccountDetails> newListOfAccounts = loadNewAccounts(listOfAccounts, replacementDetails, currentLoginDetails);

	writeNewAccountsToFile(newListOfAccounts);

	cout << '\n' << "Details successfully changed!" << '\n';
}

//____________________________________________________________________________________________________________________________
// ACCOUNT INTERANCE
//____________________________________________________________________________________________________________________________

void printPersonalDetails(StudentDetails student) {
	cout << '\n';
	// prints the titles of each collumn
	for (int i = 0; i < g_columnNames.size(); i++) {
		cout << title(g_columnNames[i]);
	}
	cout << '\n' << printTableLine() << '\n';

	//prints content for student
	cout << column(student.lastName)
		<< column(student.firstName)
		<< column(to_string(student.yearNum))
		<< column(student.fatherName)
		<< column(student.motherName)
		<< column(student.teacherName)
		<< column(to_string(student.mathsGrade))
		<< column(to_string(student.englishGrade))
		<< column(to_string(student.scienceGrade))
		<< column(to_string(student.chemistryGrade))
		<< column(to_string(student.historyGrade))
		<< column(to_string(student.overallGrade)) << '\n';

	// FYI: "column" and "title" functions set the column size
}

StudentDetails findStudent(int accountID) {
	vector<StudentDetails> listOfStudents = loadStudents();

	// loops through each student in database
	for (StudentDetails student : listOfStudents) {
		// finds student ID which matches account ID
		if (accountID == student.ID) {
			return student;
		}
	}
}

void printStudentMenu(AccountDetails userAccount);

void getStudentMenuInput(AccountDetails userAccount) {
	int userChoice;

	cin >> userChoice;

	switch (userChoice) {
	case 0:
		cout << '\n' << "Logging out..." << '\n';
		printMainMenuOptions();
		return;
	case 1:
		changeLoginDetails(userAccount);
		printStudentMenu(userAccount);
		break;
	default:
		cout << '\n' << "Please choose one of the options" << '\n';
		printStudentMenu(userAccount);
		break;
	}
}

void printStudentMenu(AccountDetails userAccount) {
	StudentDetails userStudentDetails = findStudent(userAccount.ID);

	printLine();
	cout << '\n' << "STUDENT";
	printLine();

	cout << '\n' << "Heres your information" << '\n';
	printPersonalDetails(userStudentDetails);
	cout << '\n' << "[CHANGE LOGIN DETAILS = 1] [LOG OUT = 0]" << '\n';

	getStudentMenuInput(userAccount);
}

void switchToAccount(AccountDetails userAccount) {
	switch (userAccount.accountType) {
	case 1:
		printStudentMenu(userAccount);
	case 2:
		//parent
		break;
	case 3:
		//teacher
		break;
	case 4:
		//admin
		break;
	}
}

//____________________________________________________________________________________________________________________________
// MAIN MENU STUFF
//____________________________________________________________________________________________________________________________

AccountDetails authenticateUser(vector<AccountDetails> listOfAccounts, AccountDetails userInfoToAuthenticate) {
	AccountDetails nothing;
	// loops through each row of accounts file
	for (AccountDetails account : listOfAccounts) {
		// checks if inputed details match details in file
		if (userInfoToAuthenticate.username == account.username && userInfoToAuthenticate.password == account.password) {
			return account;
		}
	}
	listOfAccounts.clear();

	// if authentication fails
	return nothing;
}

AccountDetails getLoginDetailsFromUser() {
	AccountDetails userLoginDetails;

	cout << '\n' << "Username: ";
	cin >> userLoginDetails.username;
	cout << "Password: ";
	cin >> userLoginDetails.password;

	return userLoginDetails;
}

AccountDetails getLoginDetailsFromUserAndAuthenticate() {
	int loginAttempts = 3;
	AccountDetails authenticatedUserAccountDetails;
	vector<AccountDetails> listOfAccounts = loadAccounts();

	printLine();
	cout << "LOGIN";
	printLine();
	cout << "Enter your details" << '\n';

//vgsdg
	while (loginAttempts > 0) {
		AccountDetails userInputtedAccountDetails = getLoginDetailsFromUser();
		authenticatedUserAccountDetails = authenticateUser(listOfAccounts, userInputtedAccountDetails);
		if (authenticatedUserAccountDetails.isValid()) {
			cout << '\n' << "Logged in!" << '\n';
			return authenticatedUserAccountDetails;
		}

		loginAttempts--;
		cout << '\n' << "Wrong username, password or associated account type";
		cout << '\n' << loginAttempts << " attempts left" << '\n';
	}
	// when no attempts left, return empty struct
	return authenticatedUserAccountDetails;
}

void getMainMenuInput();

void printMainMenuOptions() {
	printLine();
	cout << "WELLINGTON HIGH SCHOOL STUDENT INFORMATION SYSTEM";
	printLine();

	cout << '\n' << "Please input the number to the following option" << '\n';
	cout << '\n' << "[LOGIN = 1] [EXIT = 0]" << '\n';

	getMainMenuInput();
}

void getMainMenuInput() {
	int userChoice;
	AccountDetails userAccountDetails;

	cin >> userChoice;

	switch (userChoice) {
	case 1:
		userAccountDetails = getLoginDetailsFromUserAndAuthenticate();
		if (!userAccountDetails.isValid()) {
			cout << '\n' << "Shutting down application..." << '\n';
			return;
		}
		switchToAccount(userAccountDetails);
		break;
	case 0:
		cout << '\n' << "Shutting down application..." << '\n';
		return;
	default:
		cout << '\n' << "Please choose one of the options";
		getMainMenuInput();
		break;
	}
}

int main() {
	printMainMenuOptions();
}