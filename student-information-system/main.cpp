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

void mainMenuOptions();
int main();
//vector<StudentDetails> loadStudents();

struct AccountDetails {
	int accountType = 0;
	string username;
	string password;
	int linkID = 0;
};

struct StudentDetails {
	int linkID = 0;
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

string g_accountsFile = ("user-accounts.csv");
string g_studentsFile = ("student-database.csv");
int g_columnWidth = 10;
vector<string> g_columnNames = { "[SURNAME]", "[NAME]", "[YEAR]", "[FATHER]", "[MOTHER]", "[TEACHER]", "[MATHS]","[ENGLISH]","[SCIENCE]","[CHEM]","[HISTORY]","[OVERALL]" };

void drawLine() {
	// just draws line
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

string generateLine() {
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

	fstream AccountsFile;

	AccountsFile.open(g_studentsFile, ios::in | ios::app);

	if (!AccountsFile.is_open()) {
		throw std::runtime_error("Warning file is not open");
	}

	// loops through each row of file
	while (getline(AccountsFile, fileRow, '\n')) {
		stringstream stream(fileRow);
		StudentDetails student;

		// loops through each cell of file and adds to struct
		int i = 0;
		while (getline(stream, fileCell, ',')) {
			switch (i) {
			case 0:
				student.linkID = stoi(fileCell);
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
	AccountsFile.close();

	return listOfAccounts;
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
				details.linkID = stoi(fileCell);
				break;
			}

			i++;
		}
		// adds populated struct to vector
		listOfAccounts.push_back(details);
	}
	AccountsFile.close();

	return listOfAccounts;
}

//____________________________________________________________________________________________________________________________
//____________________________________________________________________________________________________________________________

void changeLoginDetails(AccountDetails userLoginDetails) {
	string tempAccountsFile = "temp-" + g_accountsFile;

	char* fileNamePtr = &g_accountsFile[0];
	char* tempFileNamePtr = &tempAccountsFile[0];

	// creating an instance of the file stream
	fstream fileIn(g_accountsFile, ios::in);
	if (!fileIn.is_open()) {
		cout << "Warning no file is open\n\n"; // out putting warning
		return; // end execution of the function
	}

	fstream fileOut(tempAccountsFile, ios::out); // initialised content assigned from a read file
	if (!fileOut.is_open()) {
		cout << "Warning no file is open\n\n"; // out putting warning
		return; // end execution of the function
	}
	vector<AccountDetails> listOfAccounts = loadAccounts();
	vector<AccountDetails> newListOfAccounts;

	//check to see if file has content
	//if (fileContent.size() == 0) {
	//	cout << "Warning file contains no data\n\n";
	//	fileIn.close(); // close input file
	//	fileOut.close(); // close the out put file
	//	remove(tempFileNamePtr); // removing the output file
	//	return; // end execution of function
	//}
	AccountDetails replacementDetails;

	cout << '\n' << "Current login details" << '\n';
	cout << '\n' << "Username: " << userLoginDetails.username << '\n';
	cout << "Password: " << userLoginDetails.password << '\n';
	cout << '\n' << "Enter new login details" << '\n';
	cout << '\n' << "Username: ";
	cin >> replacementDetails.username;
	cout << "Password: ";
	cin >> replacementDetails.password;

	for (AccountDetails student : listOfAccounts) {
		cout << student.username;
		if (student.username == userLoginDetails.username && student.password == userLoginDetails.password) {
			cout << "continue through\n\n";
			continue;
		}
		else {

			newListOfAccounts.push_back(student); // push back to our replacment content vector
		}
	}

	// loop through replacement content display new data
	//cout << "Data witin new file:\n";
	for (AccountDetails account : newListOfAccounts) {
		fileOut << account.username << "," << account.password << "," << account.accountType << "," << account.linkID << '\n';
	}

	fileOut << replacementDetails.username << "," << replacementDetails.password << "," << userLoginDetails.accountType << "," << userLoginDetails.linkID << '\n';

	// close files
	fileIn.close();
	fileOut.close();
	remove(fileNamePtr); // deleting original file
	rename(tempFileNamePtr, fileNamePtr);

	return;
}

//____________________________________________________________________________________________________________________________
//____________________________________________________________________________________________________________________________

void printPersonalDetails(StudentDetails student) {
	cout << '\n';
	for (int i = 0; i < g_columnNames.size(); i++) {
		cout << title(g_columnNames[i]);
	}
	cout << '\n' << generateLine() << '\n';

	//for each row in filecontent
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
}

void findStudentThenPrintDetails(int linkID) {
	vector<StudentDetails> listOfStudents = loadStudents();
	for (StudentDetails student : listOfStudents) {
		// checks if inputed details match in database
		if (linkID == student.linkID) {
			printPersonalDetails(student);
		}
	}
}

int getStudentMenuChoice(int linkID) {
	int userChoice;

	drawLine();
	cout << "STUDENT";
	drawLine();

	cout << "Heres your information" << '\n';
	findStudentThenPrintDetails(linkID);
	cout << '\n' << "[CHANGE LOGIN DETAILS = 1] [LOG OUT = 0]";
	cin >> userChoice;

	return userChoice;
}

void studentMenuOptions(AccountDetails userAccount) {
	int linkID = userAccount.linkID;

	switch (getStudentMenuChoice(linkID)) {
	case 0:
		mainMenuOptions();
	case 1:
		changeLoginDetails(userAccount);
		break;
	default:
		studentMenuOptions(userAccount);
	}

}

bool linkAccount(AccountDetails& userAccount, vector<StudentDetails>& listOfstudents) {
	if (userAccount.accountType != 1) {
		cout << "not student";
		return true;
	}

	for (StudentDetails student : listOfstudents) {
		if (userAccount.linkID == student.linkID) {
			return true;
		}
	}
}

void linkIDThenOpenAccountMenu(AccountDetails userAccount) {
	bool isLinked = false;

	vector<StudentDetails> listOfstudents = loadStudents();

	while (isLinked == false) {
		isLinked = linkAccount(userAccount, listOfstudents);
	}

	switch (userAccount.accountType) {
	case 1:
		studentMenuOptions(userAccount);
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
//____________________________________________________________________________________________________________________________

void authenticateDetails(vector<AccountDetails> listOfAccounts, AccountDetails userInfoToAuthenticate) {
	// loops through each row of file and vector
	for (AccountDetails account : listOfAccounts) {
		// checks if inputed details match in database
		if (userInfoToAuthenticate.username == account.username && userInfoToAuthenticate.password == account.password) {
			userInfoToAuthenticate.linkID = account.linkID;
			userInfoToAuthenticate.accountType = account.accountType;
			linkIDThenOpenAccountMenu(userInfoToAuthenticate);
			cout << "returned!";
			exit(0);
		}
	}
	listOfAccounts.clear();
	// if authentication fails
	return;
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

	drawLine();
	cout << "LOGIN";
	drawLine();
	cout << "Enter your details";

	vector<AccountDetails> listOfAccounts = loadAccounts();

	while (loginAttempts > 0) {
		//gets user input
		AccountDetails userLoginDetails = getLoginDetailsFromUser();
		//authenticates details
		authenticateDetails(listOfAccounts, userLoginDetails);

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

	drawLine();
	cout << "WELLINGTON HIGH SCHOOL STUDENT INFORMATION SYSTEM";
	drawLine();

	cout << '\n' << "Please input the number to the following option" << '\n';
	cout << '\n' << "[LOGIN = 1] [EXIT = 0]" << '\n';
	cin >> menuChoice;

	return menuChoice;
}

void mainMenuOptions() {
	AccountDetails userAccountDetails;

	switch (getMainMenuChoiceFromUser()) {
	case 1:
		//getLoginDetailsFromUserAndAuthenticate();
		getLoginDetailsFromUserAndAuthenticate();
		break;
	case 0:
		cout << "Shutting down application..." << '\n';
		exit(0);
		break;
	default:
		cout << '\n' << "Please choose one of the options";
		mainMenuOptions();
		break;
	}
}

int main() {
	mainMenuOptions();
}