#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <vector>
#include <string>
#include <fstream>
#include <ctype.h>
#include <windows.h>
#include <ctime>
#include <cwchar>

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
void getMainMenuInput();

struct AccountDetails {
	int ID = 0;
	string username;
	string password;
	int accountType = 0;

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

struct TeacherDetails {
	int ID = 0;
	string firstName;
	string lastName;
	string title;
};

string g_accountsFileName = ("account-database.csv");
string g_studentsFileName = ("student-database.csv");
string g_teachersFileName = ("teacher-database.csv");
int g_columnWidth = 13;
vector<string> g_columnNames = { "surname", "name", "year", "father", "mother", "teacher", "maths","english","science","chem","history","overall" };

//____________________________________________________________________________________________________________________________
// FORMATTING
//____________________________________________________________________________________________________________________________

void setConsoleStyle() {
	system("color 1f");

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.dwFontSize.X = 16;        // Width of each character in the font
	cfi.dwFontSize.Y = 24;                  // Height
	wcscpy_s(cfi.FaceName, L"Terminal"); // Choose your font
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

void setNiceStyle() {
	system("color f0");

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.dwFontSize.X = 0;
	cfi.dwFontSize.Y = 30;
	cfi.FontWeight = FW_BOLD;
	wcscpy_s(cfi.FaceName, L"Consolas");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

	SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FULLSCREEN_MODE, 0);
}
void clear()
{
	cout << "\x1B[2J\x1B[H";
}

void line() {
	cout << '\n' << "---------------------------------------------------------------------" << '\n';
}

string lowercase(string inputString) {
	for (size_t i = 0; i < inputString.length(); i++) {
		inputString[i] = tolower(inputString[i]);
	}
	return inputString;
}

string uppercase(string inputString) {
	for (size_t i = 0; i < inputString.length(); i++) {
		inputString[i] = toupper(inputString[i]);
	}
	return inputString;
}

string category(string inputString) {
	int columnLength = g_columnWidth - 1;
	string outputString = "[";

	for (size_t i = 0; i < inputString.length(); i++) {
		outputString += inputString[i];
		columnLength--;
	}
	outputString = uppercase(outputString);
	outputString += "]";
	while (columnLength != 0) {
		outputString += " ";
		columnLength--;
	}

	return outputString;
}

string column(string inputString) {
	int columnLength = g_columnWidth;
	string outputString;

	// makes column longer if it cant fit string
	if (inputString.length() > columnLength) {
		columnLength = inputString.length();
	}

	// makes first letter capital
	inputString[0] = toupper(inputString[0]);

	for (size_t i = 0; i < inputString.length(); i++) {
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
	for (size_t i = 0; i < g_columnNames.size(); i++) {
		for (int i = 0; i < g_columnWidth; i++) {
			outputString += "_";
		}
		outputString += "_";
	}
	return outputString;
}

//____________________________________________________________________________________________________________________________
// LOADING
//____________________________________________________________________________________________________________________________


vector<StudentDetails> loadStudents() {
	string fileRow;
	string fileCell;
	vector<StudentDetails> listOfStudents;
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
				student.lastName = lowercase(fileCell);
				break;
			case 2:
				student.firstName = lowercase(fileCell);
				break;
			case 3:
				student.yearNum = stoi(fileCell);
				break;
			case 4:
				student.fatherName = lowercase(fileCell);
				break;
			case 5:
				student.motherName = lowercase(fileCell);
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
		listOfStudents.push_back(student);
	}
	studentsFile.close();

	return listOfStudents;
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
		AccountDetails account;

		// loops through each cell of row and adds to struct
		int i = 0;
		while (getline(stream, fileCell, ',')) {
			switch (i) {
			case 0:
				account.ID = stoi(fileCell);
				break;
			case 1:
				account.username = fileCell;
				break;
			case 2:
				account.password = fileCell;
				break;
			case 3:
				account.accountType = stoi(fileCell);
				break;
			}

			i++;
		}
		// adds populated struct to vector
		listOfAccounts.push_back(account);
	}
	accountsFile.close();

	return listOfAccounts;
}

vector<TeacherDetails> loadTeachers() {
	string fileRow;
	string fileCell;
	vector<TeacherDetails> listOfTeachers;
	fstream teachersFile;

	teachersFile.open(g_teachersFileName, ios::in | ios::app);

	if (!teachersFile.is_open()) {
		throw std::runtime_error("Warning file is not open");
	}

	// loops through each row of accounts file
	while (getline(teachersFile, fileRow, '\n')) {
		stringstream stream(fileRow);
		TeacherDetails teacher;

		// loops through each cell of row and adds to struct
		int i = 0;
		while (getline(stream, fileCell, ',')) {
			switch (i) {
			case 0:
				teacher.ID = stoi(fileCell);
				break;
			case 1:
				teacher.title = fileCell;
				break;
			case 2:
				teacher.firstName = lowercase(fileCell);
				break;
			case 3:
				teacher.lastName = lowercase(fileCell);
				break;
			}

			i++;
		}
		// adds populated struct to vector
		listOfTeachers.push_back(teacher);
	}
	teachersFile.close();

	return listOfTeachers;
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

vector<StudentDetails> loadClass(TeacherDetails teacherDetails) {
	vector<StudentDetails> listOfStudents = loadStudents();
	vector<StudentDetails> listOfStudentsInClass;

	// loops through each row of student file
	for (StudentDetails student : listOfStudents) {
		if (student.teacherName == teacherDetails.title + teacherDetails.lastName) {
			listOfStudentsInClass.push_back(student);
		}
	}

	return listOfStudentsInClass;
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
		accountsFile << account.ID << "," << account.username << "," << account.password << "," << account.accountType << '\n';
	}
	accountsFile.close();

	remove(accountsFileNamePtr); // deleting original file
	int x = rename(tempFileNamePtr, accountsFileNamePtr); //rename new file
}

//____________________________________________________________________________________________________________________________
// ACCOUNT OPTIONS
//____________________________________________________________________________________________________________________________

void changeLoginDetails(AccountDetails currentLoginDetails) {
	vector<AccountDetails> listOfAccounts = loadAccounts();
	AccountDetails replacementDetails;

	clear();
	line();
	cout << "CHANGE DETAILS";
	line();

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

	clear();
	cout << '\n' << "Details successfully changed!" << '\n';
}

//____________________________________________________________________________________________________________________________
// REGISTER
//____________________________________________________________________________________________________________________________


void writeNewAccountToDabase(AccountDetails newAccountDetails) {
	fstream AccountsFile(g_accountsFileName, ios::in | ios::app);

	if (!AccountsFile.is_open()) {
		throw std::runtime_error("Warning file is not open");
	}

	// writes user input to file
	AccountsFile << newAccountDetails.ID << ","
		<< newAccountDetails.username << ","
		<< newAccountDetails.password << ","
		<< newAccountDetails.accountType << '\n';

	AccountsFile.close();
}

void writeNewTeacherToDabase(TeacherDetails newTeacherDetails) {
	fstream TeachersFile(g_teachersFileName, ios::in | ios::app);

	if (!TeachersFile.is_open()) {
		throw std::runtime_error("Warning file is not open");
	}

	// writes user input to file
	TeachersFile << newTeacherDetails.ID << ","
		<< newTeacherDetails.title << "," 
		<< newTeacherDetails.firstName << ","
		<< newTeacherDetails.lastName << '\n';

	TeachersFile.close();
}

void writeNewStudentToDabase(StudentDetails newStudentDetails) {
	fstream StudentsFile(g_studentsFileName, ios::in | ios::app);

	if (!StudentsFile.is_open()) {
		throw std::runtime_error("Warning file is not open");
	}

	// writes user input to file
	StudentsFile << newStudentDetails.ID << ","
		<< newStudentDetails.lastName << ","
		<< newStudentDetails.firstName << ","
		<< newStudentDetails.yearNum << ","
		<< newStudentDetails.fatherName << ","
		<< newStudentDetails.motherName << ","
		<< newStudentDetails.teacherName << ","
		<< newStudentDetails.mathsGrade << ","
		<< newStudentDetails.englishGrade << ","
		<< newStudentDetails.scienceGrade << ","
		<< newStudentDetails.chemistryGrade << ","
		<< newStudentDetails.historyGrade << ","
		<< newStudentDetails.overallGrade << '\n';

	StudentsFile.close();
}

string chooseTeacher(vector<TeacherDetails> listOfTeachers) {
	int counter = 1;
	int teacherChoice;
	string teacherString;

	for (TeacherDetails teacher : listOfTeachers) {
		cout << "[";
		// makes uppercase
		for (int i = 0; i < teacher.title.length(); i++) {
			teacher.title[i] = toupper(teacher.title[i]);
			cout << teacher.title[i];
		}
		for (int i = 0; i < teacher.lastName.length(); i++) {
			teacher.lastName[i] = toupper(teacher.lastName[i]);
			cout << teacher.lastName[i];
		}
		cout << " = " << counter << " ] ";

		counter++;
	}

	cin >> teacherChoice;

	counter = 1;
	for (TeacherDetails teacher : listOfTeachers) {
		if (counter == teacherChoice) {
			teacherString = teacher.title + teacher.lastName;
			return teacherString;
		}

		counter++;
	}
}

StudentDetails getNewStudentDetails(int randomID) {
	StudentDetails newStudentDetails;
	vector<TeacherDetails> listOfTeachers;

	clear();
	line();
	cout << "ADD STUDENT";
	line();

	cout << '\n' << "Enter new student information" << '\n';
	cout << '\n' << "First name: ";
	cin >> newStudentDetails.firstName;
	cout << "Lastname: ";
	cin >> newStudentDetails.lastName;
	cout << "Year: ";
	cin >> newStudentDetails.yearNum;
	cout << "Father: ";
	cin >> newStudentDetails.fatherName;
	cout << "Mother: ";
	cin >> newStudentDetails.motherName;
	cout << "Teacher: " << '\n' << '\n';

	listOfTeachers = loadTeachers();
	newStudentDetails.teacherName = chooseTeacher(listOfTeachers);
	newStudentDetails.ID = randomID;

	return newStudentDetails;
}

TeacherDetails getNewTeacherDetails(int randomID) {
	TeacherDetails newTeacherDetails;
	int chooseTitle;

	clear();
	line();
	cout << "ADD TEACHER";
	line();

	cout << '\n' << "Choose the teacher's title" << '\n';
	cout << '\n' << "[MR = 1] [MRS = 2] [MS = 3] [BACK = 0]" << '\n';
	cin >> chooseTitle;
	
	switch (chooseTitle) {
	case 0:
		return newTeacherDetails;
	case 1:
		newTeacherDetails.title = "mr. ";
		break;
	case 2:
		newTeacherDetails.title = "mrs. ";
		break;
	case 3:
		newTeacherDetails.title = "ms. ";
		break;
	}
	cout << '\n' << "Enter the teacher's name" << '\n';
	cout << '\n' << "First name: ";
	cin >> newTeacherDetails.firstName;
	cout << "Last name: ";
	cin >> newTeacherDetails.lastName;

	newTeacherDetails.ID = randomID;

	return newTeacherDetails;
}

AccountDetails getNewAccountDetails(int accountType) {
	AccountDetails newAccountDetails;

	clear();
	line();
	cout << "ADD ADMIN";
	line();

	cout << '\n' << "Enter new account details" << '\n';
	cout << '\n' << "Username: ";
	cin >> newAccountDetails.username;
	cout << "Password: ";
	cin >> newAccountDetails.password;
	newAccountDetails.accountType = accountType + 1;
	
	return newAccountDetails;
}

void addNewAccountThenReturn(int randomID) {
	int userChoice;
	AccountDetails newAccountDetails;
	TeacherDetails newTeacherDetails;
	int confirm = 0;

	clear();
	line();
	cout << "ADD ACCOUNT";
	line();

	cout << '\n' << "What type of account are you creating?" << '\n';
	cout << '\n' << "[TEACHER = 1] [ADMIN = 2] [BACK = 0]" << '\n';
	cin >> userChoice;

	switch (userChoice)
	{
	case 0: 
		return;
	case 1:
		while (confirm != 1) {
			newTeacherDetails = getNewTeacherDetails(randomID);

			cout << '\n' << newTeacherDetails.title << newTeacherDetails.firstName << " " << newTeacherDetails.lastName << '\n';
			cout << "Conirm?" << '\n';
			cout << '\n' << "[YES = 1] [NO = 0]" << '\n';
			cin >> confirm;
		}
		newAccountDetails.username = newTeacherDetails.firstName;
		newAccountDetails.password = newTeacherDetails.lastName;
		newAccountDetails.ID = newTeacherDetails.ID;
		newAccountDetails.accountType = 2;

		clear();
		cout << '\n' << "Teacher added!" << '\n';
		cout << "Note: Teacher's default login details are their first name and last name. This can be changed later" << '\n';

		writeNewTeacherToDabase(newTeacherDetails);
		break;
	case 2:
		newAccountDetails = getNewAccountDetails(userChoice);
		clear();
		cout << '\n' << "Admin account added!" << '\n';
		break;
	default:
		cout << '\n' << "Please choose one of the options" << '\n';
		addNewAccountThenReturn(randomID);
	}

	writeNewAccountToDabase(newAccountDetails);
}

void addNewStudentThenReturn(int randomID) {
	AccountDetails newAccountDetails;
	StudentDetails newStudentDetails;

	line();
	cout << "NEW STUDENT";
	line();

	newStudentDetails = getNewStudentDetails(randomID);
	newAccountDetails.username = newStudentDetails.firstName;
	newAccountDetails.password = newStudentDetails.lastName;
	newAccountDetails.ID = newStudentDetails.ID;
	newAccountDetails.accountType = 1;

	writeNewStudentToDabase(newStudentDetails);
	writeNewAccountToDabase(newAccountDetails);

	cout << '\n' << "Student added!" << '\n';
	cout << "Note: Student's default login details are their first name and last name. This can be changed later" << '\n';
}

//____________________________________________________________________________________________________________________________
// ACCOUNT INTERFACE
//____________________________________________________________________________________________________________________________

void printPersonalDetails(StudentDetails student) {
	cout << '\n';
	// prints the titles of each collumn
	for (size_t i = 0; i < g_columnNames.size(); i++) {
		cout << category(g_columnNames[i]);
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

void printClassDetails(vector<StudentDetails> listOfStudentsInClass) {
	cout << '\n';
	// prints the titles of each collumn
	for (size_t i = 0; i < g_columnNames.size(); i++) {
		cout << category(g_columnNames[i]);
	}
	cout << '\n' << printTableLine() << '\n';

	for (StudentDetails student : listOfStudentsInClass) {
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
	}
	
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

TeacherDetails findTeacher(int accountID) {
	vector<TeacherDetails> listOfTeachers = loadTeachers();

	// loops through each student in database
	for (TeacherDetails teacher : listOfTeachers) {
		// finds student ID which matches account ID
		if (accountID == teacher.ID) {
			return teacher;
		}
	}
}


void printStudentMenu(AccountDetails userAccount);

void getStudentMenuInput(AccountDetails userAccountDetails) {
	int userChoice;

	cin >> userChoice;

	switch (userChoice) {
	case 0:
		clear();
		cout << '\n' << "Logging out..." << '\n';
		return;
	case 1:
		changeLoginDetails(userAccountDetails);
		return;
	default:
		cout << '\n' << "Please choose one of the options" << '\n';
		printStudentMenu(userAccountDetails);
	}
}

void printStudentMenu(AccountDetails userAccountDetails) {
	StudentDetails userStudentDetails = findStudent(userAccountDetails.ID);

	line();
	cout << "STUDENT";
	line();

	cout << '\n' << "Heres your information" << '\n';
	printPersonalDetails(userStudentDetails);
	cout << '\n' << "[CHANGE LOGIN DETAILS = 1] [LOG OUT = 0]" << '\n';

	getStudentMenuInput(userAccountDetails);
}

void printTeacherMenu(AccountDetails userAccountDetails);

void getTeacherMenuInput(AccountDetails userAccountDetails) {
	int userChoice;

	cin >> userChoice;

	switch (userChoice) {
	case 0:
		clear();
		cout << '\n' << "Logging out..." << '\n';
		return;
	case 1:
		changeLoginDetails(userAccountDetails);
		return;
	case 2:
		//edit report
		break;
	default:
		cout << '\n' << "Please choose one of the options" << '\n';
		printStudentMenu(userAccountDetails);
	}
}

void printTeacherMenu(AccountDetails userAccountDetails) {
	TeacherDetails userTeacherDetails = findTeacher(userAccountDetails.ID);
	vector<StudentDetails> listOfStudentsInClass = loadClass(userTeacherDetails);

	clear();
	line();
	cout << "TEACHER";
	line();

	cout << '\n' << "Heres your classes information" << '\n';
	printClassDetails(listOfStudentsInClass);
	cout << '\n' << "[CHANGE LOGIN DETAILS = 1] [EDIT REPORTS = 2] [LOG OUT = 0]" << '\n';

	getTeacherMenuInput(userAccountDetails);
}

void switchToAccount(AccountDetails userAccount) {
	switch (userAccount.accountType) {
	case 1:
		printStudentMenu(userAccount);
		return;
	case 2:
		printTeacherMenu(userAccount);
		break;
	case 3:
		//printAdminMenu();
		break;
	default:
		cout << "No account type";
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

	clear();
	line();
	cout << "LOGIN";
	line();
	cout << '\n' << "Enter your details" << '\n';

	while (loginAttempts > 0) {
		AccountDetails userInputtedAccountDetails = getLoginDetailsFromUser();
		authenticatedUserAccountDetails = authenticateUser(listOfAccounts, userInputtedAccountDetails);
		if (authenticatedUserAccountDetails.isValid()) {
			clear();
			cout << '\n' << "Logged in!" << '\n';
			return authenticatedUserAccountDetails;
		}

		loginAttempts--;
		clear();
		cout << '\n' << "Wrong username or password";
		cout << '\n' << loginAttempts << " attempts left" << '\n';
	}
	// when no attempts left, return empty struct
	return authenticatedUserAccountDetails;
}

void getMainMenuInput();

void printMainMenuOptions() {
	line();
	cout << "WELLINGTON HIGH SCHOOL STUDENT INFORMATION SYSTEM";
	line();

	cout << '\n' << "Please input the number to the following option" << '\n';
	cout << '\n' << "[LOGIN = 1] (admin =2) [EXIT = 0]" << '\n';
	getMainMenuInput();
}

void admin() {
	srand((unsigned int)time(0));
	int userChoice;
	bool isAvaliableID = false;
	int randomID = rand() & 1000;
	vector<AccountDetails> listOfAccounts = loadAccounts();

	while (!isAvaliableID) {
		for (AccountDetails account : listOfAccounts) {
			if (account.ID != randomID) {
				break;
			}
			randomID = rand() & 1000;
		}
		isAvaliableID = true;
	}

	cout << "(add student =2) (create account =3)";
	cin >> userChoice;

	switch (userChoice) {
	case 2:
		addNewStudentThenReturn(randomID);
		break;
	case 3:
		addNewAccountThenReturn(randomID);
		break;
	}
}

void getMainMenuInput() {
	int userChoice;
	AccountDetails userAccountDetails;

	cin >> userChoice;

	switch (userChoice) {
	case 0:
		cout << '\n' << "Shutting down application..." << '\n';
		return;
	case 1:
		userAccountDetails = getLoginDetailsFromUserAndAuthenticate();
		if (!userAccountDetails.isValid()) {
			cout << "Shutting down application..." << '\n';
			return;
		}
		switchToAccount(userAccountDetails);
		break;
	case 2:
		admin();
		break;
	default:
		cout << '\n' << "Please choose one of the options";
		getMainMenuInput();
	}
	printMainMenuOptions();
}

int main() {
	//setConsoleStyle();
	setNiceStyle();

	printMainMenuOptions();
}