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
	int teacherID = 0;
	int mathsGrade = 0;
	int englishGrade = 0;
	int scienceGrade = 0;
	int chemistryGrade = 0;
	int historyGrade = 0;
	int overallGrade = 0;

	bool isValid() {
		return firstName != "";
	};
};

struct TeacherDetails {
	int ID = 0;
	string firstName;
	string lastName;
	string title;

	bool isValid() {
		return firstName != "";
	};
};

string g_accountsFileName = ("account-database.csv");
string g_studentsFileName = ("student-database.csv");
string g_teachersFileName = ("teacher-database.csv");
const int g_columnWidth = 13;
const vector<string> g_categories = { "surname", "name", "year", "father", "mother", "teacher", "maths","english","science","chem","history","overall" };

void printTeacherMenu(AccountDetails& userAccountDetails);
void printAdminMenu(AccountDetails& userAccountDetails);
void printStudentMenu(AccountDetails& userAccountDetails);
bool printEditStudentMenu(StudentDetails& newStudentDetails);
void printEditGradesMenu(StudentDetails& studentDetails);
void printEditOrDeleteMenu(StudentDetails studentDetails);
void printEditGradesMenu(StudentDetails& studentDetails);
void printStartMenu();
void printThemeMenu();
void switchToAccount(AccountDetails& userAccount);

//____________________________________________________________________________________________________________________________
// FORMATTING
//____________________________________________________________________________________________________________________________

void setStyleDefault() {
	system("color 0f");
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.dwFontSize.Y = 21;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Cascadia");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

void setStyleModern() {
	system("color f0");
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.dwFontSize.Y = 22;
	cfi.FontWeight = FW_BOLD;
	wcscpy_s(cfi.FaceName, L"Consolas");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

void setStyleClassic() {
	system("color 1f");
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.dwFontSize.X = 15;
	cfi.dwFontSize.Y = 24;
	wcscpy_s(cfi.FaceName, L"Terminal");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

void setStyleHacker() {
	system("color 02");
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.dwFontSize.X = 16;
	cfi.dwFontSize.Y = 24;
	wcscpy_s(cfi.FaceName, L"Terminal");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

void clear() {
	cout << "\x1B[2J\x1B[H";
}

void line() {
	cout << '\n' << "____________________________________________________________" << '\n' << '\n';
}

void fullscreen() {
	system("mode con COLS=700");
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
	SendMessage(GetConsoleWindow(), WM_SYSKEYDOWN, VK_RETURN, 0x20000000);
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

	// adds string to new variable
	for (size_t i = 0; i < inputString.length(); i++) {
		outputString += inputString[i];
		columnLength--;
	}

	outputString = uppercase(outputString);
	outputString += "]";

	//adds whitespace until the end of the column length
	while (columnLength != 0) {
		outputString += " ";
		columnLength--;
	}

	return outputString;
}

string column(string inputString) {
	size_t columnLength = g_columnWidth;
	string outputString;

	// makes first letter capital
	inputString[0] = toupper(inputString[0]);

	// if string is bigger then column length then itll cut it off and add ".." to the end
	if (inputString.length() > columnLength) {
		int i = 0;
		while (columnLength - 2 != 0) {
			outputString += inputString[i];
			columnLength--;
			i++;
		}
		return outputString += "..|";
	}
	else {
		// adds string to new variable
		for (size_t i = 0; i < inputString.length(); i++) {
			outputString += inputString[i];
			columnLength--;
		}
		//adds whitespace until the end of the column length
		while (columnLength != 0) {
			outputString += " ";
			columnLength--;
		}
	}

	return outputString += "|";
}

string printTableLine() {
	string outputString;
	for (size_t i = 0; i < g_categories.size(); i++) {
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
		cout << "file not open";
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
				student.teacherID = stoi(fileCell);
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
				student.overallGrade = student.mathsGrade + student.englishGrade + student.scienceGrade + student.chemistryGrade + student.historyGrade;
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
		cout << "file not open";
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
		cout << "file not open";
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

vector<AccountDetails> loadNewAccountsAfterEdited(AccountDetails& replacementDetails, AccountDetails& currentLoginDetails) {
	vector<AccountDetails> listOfAccounts = loadAccounts();
	vector<AccountDetails> newListOfAccounts;

	// loops through each account in database
	for (AccountDetails account : listOfAccounts) {
		// finds current account details and skips
		if (account.ID == currentLoginDetails.ID) {
			continue;
		}
		// add all accounts into new vector, exculding current details
		newListOfAccounts.push_back(account);
	}
	// then appends on the new account details
	newListOfAccounts.push_back(replacementDetails);

	return newListOfAccounts;
}

vector<AccountDetails> loadNewAccountsAfterDeleted(AccountDetails& newLoginDetails) {
	vector<AccountDetails> listOfAccounts = loadAccounts();
	vector<AccountDetails> newListOfAccounts;

	// loops through each account in databas
	for (AccountDetails account : listOfAccounts) {
		// finds current account details and skips
		if (account.ID == newLoginDetails.ID) {
			continue;
		}
		// add all accounts into new vector, exculding current details
		newListOfAccounts.push_back(account);
	}

	return newListOfAccounts;
}

vector<StudentDetails> loadNewStudentsAfterDeleted(StudentDetails& newStudentDetails) {
	vector<StudentDetails> listOfStudents = loadStudents();
	vector<StudentDetails> newListOfStudents;

	// loops through each Student in database
	for (StudentDetails student : listOfStudents) {
		// finds current Student details and skips
		if (student.ID == newStudentDetails.ID) {
			continue;
		}
		// add all Students into new vector, exculding current details
		newListOfStudents.push_back(student);
	}

	return newListOfStudents;
}

vector<StudentDetails> loadNewStudentsAfterEdited(StudentDetails& replacementDetails, StudentDetails& currentStudentDetails) {
	vector<StudentDetails> listOfStudents = loadStudents();
	vector<StudentDetails> newListOfStudents;

	// loops through each Student in database
	for (StudentDetails student : listOfStudents) {
		// finds current Student details and skips
		if (student.ID == currentStudentDetails.ID) {
			continue;
		}
		// add all Students into new vector, exculding current details
		newListOfStudents.push_back(student);
	}
	// then appends on the new student details
	newListOfStudents.push_back(replacementDetails);

	return newListOfStudents;
}

vector<StudentDetails> loadClass(TeacherDetails teacher) {
	vector<StudentDetails> listOfStudents = loadStudents();
	vector<StudentDetails> listOfStudentsInClass;

	// loops through each row of student file
	for (StudentDetails student : listOfStudents) {
		if (student.teacherID == teacher.ID) {
			listOfStudentsInClass.push_back(student);
		}
	}

	return listOfStudentsInClass;
}

//____________________________________________________________________________________________________________________________
// WRITING
//____________________________________________________________________________________________________________________________

void writeNewListOfAccountsToFile(vector<AccountDetails> newListOfAccounts) {
	string tempAccountsFile = "temp-" + g_accountsFileName;
	char* accountsFileNamePtr = &g_accountsFileName[0];
	char* tempFileNamePtr = &tempAccountsFile[0];
	fstream accountsFile(tempAccountsFile, ios::out);

	if (!accountsFile.is_open()) {
		cout << "file not open";
	}

	// writes account details to file
	for (AccountDetails account : newListOfAccounts) {
		accountsFile << account.ID << ","
			<< account.username << ","
			<< account.password << ","
			<< account.accountType << '\n';
	}
	accountsFile.close();

	remove(accountsFileNamePtr); // deleting original file
	int x = rename(tempFileNamePtr, accountsFileNamePtr); //rename new file
}

void writeNewListOfStudentsToFile(vector<StudentDetails>& newListOfStudents) {
	string tempStudentsFile = "temp-" + g_studentsFileName;
	char* studentsFileNamePtr = &g_studentsFileName[0];
	char* tempFileNamePtr = &tempStudentsFile[0];
	fstream studentsFile(tempStudentsFile, ios::out);

	if (!studentsFile.is_open()) {
		cout << "file not open";
	}

	// writes account details to file
	for (StudentDetails student : newListOfStudents) {
		studentsFile << student.ID << ","
			<< student.lastName << ","
			<< student.firstName << ","
			<< student.yearNum << ","
			<< student.fatherName << ","
			<< student.motherName << ","
			<< student.teacherID << ","
			<< student.mathsGrade << ","
			<< student.englishGrade << ","
			<< student.scienceGrade << ","
			<< student.chemistryGrade << ","
			<< student.historyGrade << ","
			<< student.overallGrade << '\n';
	}
	studentsFile.close();

	remove(studentsFileNamePtr); // deleting original file
	int x = rename(tempFileNamePtr, studentsFileNamePtr); //rename new file
}

void writeNewAccountToFile(AccountDetails& newAccountDetails) {
	fstream accountsFile(g_accountsFileName, ios::in | ios::app);

	if (!accountsFile.is_open()) {
		cout << "file not open";
	}

	// writes user input to file
	accountsFile << newAccountDetails.ID << ","
		<< newAccountDetails.username << ","
		<< newAccountDetails.password << ","
		<< newAccountDetails.accountType << '\n';

	accountsFile.close();
}

void writeNewTeacherToFile(TeacherDetails& newTeacherDetails) {
	fstream teachersFile(g_teachersFileName, ios::in | ios::app);

	if (!teachersFile.is_open()) {
		cout << "file not open";
	}

	// writes user input to file
	teachersFile << newTeacherDetails.ID << ","
		<< newTeacherDetails.title << ","
		<< newTeacherDetails.firstName << ","
		<< newTeacherDetails.lastName << '\n';

	teachersFile.close();
}

void writeNewStudentToFile(StudentDetails& newStudentDetails) {
	fstream studentsFile(g_studentsFileName, ios::in | ios::app);

	if (!studentsFile.is_open()) {
		cout << "file not open";
	}

	// writes user input to file
	studentsFile << newStudentDetails.ID << ","
		<< newStudentDetails.lastName << ","
		<< newStudentDetails.firstName << ","
		<< newStudentDetails.yearNum << ","
		<< newStudentDetails.fatherName << ","
		<< newStudentDetails.motherName << ","
		<< newStudentDetails.teacherID << ","
		<< newStudentDetails.mathsGrade << ","
		<< newStudentDetails.englishGrade << ","
		<< newStudentDetails.scienceGrade << ","
		<< newStudentDetails.chemistryGrade << ","
		<< newStudentDetails.historyGrade << ","
		<< newStudentDetails.overallGrade << '\n';

	studentsFile.close();
}

//____________________________________________________________________________________________________________________________
// ACCOUNT CONTROLS
//____________________________________________________________________________________________________________________________

void changeLoginDetails(AccountDetails& currentLoginDetails) {
	AccountDetails replacementDetails;

	clear();
	line();
	cout << "CHANGE DETAILS";
	line();

	cout << "Current login details" << '\n';
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

	vector<AccountDetails> newListOfAccounts = loadNewAccountsAfterEdited(replacementDetails, currentLoginDetails);

	writeNewListOfAccountsToFile(newListOfAccounts);

	clear();
	cout << '\n' << "Details successfully changed!";
}

void printStudentDetails(StudentDetails& student) {
	vector<TeacherDetails> listOfTeachers = loadTeachers();
	string teacherName;
	string lastname;

	cout << '\n' << printTableLine() << '\n';
	cout << '\n';
	// prints the category of each collumn
	for (size_t i = 0; i < g_categories.size(); i++) {
		cout << category(g_categories[i]);
	}
	cout << '\n' << printTableLine() << '\n';

	for (TeacherDetails teacher : listOfTeachers) {
		if (teacher.ID == student.teacherID) {
			lastname = teacher.lastName;
			lastname[0] = toupper(teacher.lastName[0]);
			teacherName = teacher.title + lastname;
		}
	}

	//prints content for student
	cout << column(student.lastName)
		<< column(student.firstName)
		<< column(to_string(student.yearNum))
		<< column(student.fatherName)
		<< column(student.motherName)
		<< column(teacherName)
		<< column(to_string(student.mathsGrade))
		<< column(to_string(student.englishGrade))
		<< column(to_string(student.scienceGrade))
		<< column(to_string(student.chemistryGrade))
		<< column(to_string(student.historyGrade))
		<< column(to_string(student.overallGrade)) << '\n';
	cout << printTableLine() << '\n';
	// FYI: "column" and "title" functions set the column size
}

void switchToAccount(AccountDetails& userAccount) {
	switch (userAccount.accountType) {
	case 1:
		printStudentMenu(userAccount);
		return;
	case 2:
		printTeacherMenu(userAccount);
		break;
	case 3:
		printAdminMenu(userAccount);
		break;
	default:
		cout << "No account type";
		break;
	}
}

StudentDetails findStudentWithName(StudentDetails& studentDetailsToFind) {
	vector<StudentDetails> listOfStudents = loadStudents();
	StudentDetails inValid;

	// loops through each student in database
	for (StudentDetails student : listOfStudents) {
		if (lowercase(studentDetailsToFind.firstName) == student.firstName && lowercase(studentDetailsToFind.lastName) == student.lastName) {
			return student;
		}
	}

	return inValid;
}

StudentDetails getSelectStudentInput() {
	StudentDetails studentDetailsToFind;
	vector<StudentDetails> listOfStudents = loadStudents();

	cout << '\n' << "Enter the details of the student you want to find" << '\n';
	cout << '\n' << "Last name: ";
	cin >> studentDetailsToFind.lastName;
	cout << "First name: ";
	cin >> studentDetailsToFind.firstName;

	StudentDetails student = findStudentWithName(studentDetailsToFind);

	if (!student.isValid()) {
		clear();
		cout << '\n' << "No student found!";
		return student;
	}

	return student;
}

//____________________________________________________________________________________________________________________________
// ADMIN
//____________________________________________________________________________________________________________________________

int chooseTeacher(vector<TeacherDetails>& listOfTeachers) {
	int counter = 1;
	int teacherChoice;
	int teacherID;

	for (TeacherDetails teacher : listOfTeachers) {
		cout << "[";
		// makes uppercase
		for (size_t i = 0; i < teacher.title.length(); i++) {
			teacher.title[i] = toupper(teacher.title[i]);
			cout << teacher.title[i];
		}
		for (size_t i = 0; i < teacher.lastName.length(); i++) {
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
			teacherID = teacher.ID;
			return teacherID;
		}
		counter++;
	}

	return NULL;
}

AccountDetails findAccountWithID(int& accountID) {
	vector<AccountDetails> listOfAccounts = loadAccounts();
	AccountDetails inValid;

	// loops through each student in database
	for (AccountDetails account : listOfAccounts) {
		// finds student ID which matches account ID

		if (accountID == account.ID) {
			return account;
		}
	}

	// should never happen
	return inValid;
}

TeacherDetails getNewTeacherDetails(int randomID) {
	TeacherDetails newTeacherDetails;
	int chooseTitle;

	clear();
	line();
	cout << "ADD TEACHER";
	line();

	cout << "Choose the teacher's title" << '\n';
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
	default:
		cout << '\n' << "Please choose one of the options" << '\n';
		getNewTeacherDetails(randomID);
	}

	cout << '\n' << "Enter the teacher's name" << '\n';
	cout << '\n' << "First name: ";
	cin >> newTeacherDetails.firstName;
	cout << "Last name: ";
	cin >> newTeacherDetails.lastName;

	newTeacherDetails.ID = randomID;

	return newTeacherDetails;
}

AccountDetails getNewAdminDetails() {
	AccountDetails newAccountDetails;

	clear();
	line();
	cout << "ADD ADMIN";
	line();

	cout << "Enter new account details" << '\n';
	cout << '\n' << "Username: ";
	cin >> newAccountDetails.username;
	cout << "Password: ";
	cin >> newAccountDetails.password;
	newAccountDetails.accountType = 3;

	return newAccountDetails;
}

StudentDetails getNewStudentDetails(int randomID) {
	StudentDetails newStudentDetails;
	vector<TeacherDetails> listOfTeachers;
	bool isValid = false;

	clear();
	line();
	cout << "ADD STUDENT";
	line();

	cout << "Enter new student information" << '\n';
	cout << '\n' << "First name: ";
	cin >> newStudentDetails.firstName;
	cout << "Last name: ";
	cin >> newStudentDetails.lastName;

	while (!isValid) {
		cout << "Year (9 - 13): ";
		cin >> newStudentDetails.yearNum;
		if (newStudentDetails.yearNum > 13 || newStudentDetails.yearNum < 9) {
			cout << '\n' << "Please enter withing the range";
			continue;
		}
		isValid = true;
	}

	cout << "Father: ";
	cin >> newStudentDetails.fatherName;
	cout << "Mother: ";
	cin >> newStudentDetails.motherName;
	cout << "Teacher: " << '\n' << '\n';

	listOfTeachers = loadTeachers();

	while (newStudentDetails.teacherID == NULL) {
		newStudentDetails.teacherID = chooseTeacher(listOfTeachers);
		if (newStudentDetails.teacherID == NULL) {
			clear();
			cout << '\n' << "Please choose one of the options";
			getNewStudentDetails(randomID);
		}
	}
	newStudentDetails.ID = randomID;

	return newStudentDetails;
}

void addNewStudent(int& randomID) {
	AccountDetails newAccountDetails;
	StudentDetails newStudentDetails;
	int confirm = 0;

	line();
	cout << "NEW STUDENT";
	line();

	newStudentDetails = getNewStudentDetails(randomID);
	// should never happen
	if (!newStudentDetails.isValid()) {
		clear();
		cout << '\n' << "Student not added";
		return;
	}
	// sets up automatic account
	newAccountDetails.username = newStudentDetails.firstName;
	newAccountDetails.password = newStudentDetails.lastName;
	newAccountDetails.ID = newStudentDetails.ID;
	newAccountDetails.accountType = 1;

	cout << '\n' << "Confirm new student?" << '\n';
	cout << '\n' << "[YES = 1] [NO = 0]" << '\n';
	cin >> confirm;

	if (confirm == 0) {
		clear();
		cout << '\n' << "Cancelled";
		return;
	}

	writeNewStudentToFile(newStudentDetails);
	writeNewAccountToFile(newAccountDetails);
	clear();

	cout << '\n' << "Student added!";
	cout << '\n' << "Note: Student's default login details are their first name and last name. This can be changed later" << '\n';
}

void addNewAdmin() {
	AccountDetails newAccountDetails;
	int confirm = 0;

	newAccountDetails = getNewAdminDetails();

	if (!newAccountDetails.isValid()) {
		clear();
		return;
	}

	cout << '\n' << "Confirm new admin?" << '\n';
	cout << '\n' << "[YES = 1] [NO = 0]" << '\n';
	cin >> confirm;

	if (confirm == 0) {
		clear();
		cout << '\n' << "Cancelled";
		return;
	}

	writeNewAccountToFile(newAccountDetails);

	clear();
	cout << '\n' << "Admin account added!";
}

void addNewTeacher(int& randomID) {
	AccountDetails newAccountDetails;
	TeacherDetails newTeacherDetails;
	int confirm = 0;

	newTeacherDetails = getNewTeacherDetails(randomID);

	if (!newTeacherDetails.isValid()) {
		clear();
		return;
	}

	cout << '\n' << "Confirm new teacher?" << '\n';
	cout << '\n' << "[YES = 1] [NO = 0]" << '\n';
	cin >> confirm;

	if (confirm == 0) {
		clear();
		cout << '\n' << "Cancelled";
		return;
	}

	// sets up automatic account
	newAccountDetails.username = newTeacherDetails.firstName;
	newAccountDetails.password = newTeacherDetails.lastName;
	newAccountDetails.ID = newTeacherDetails.ID;
	newAccountDetails.accountType = 2;

	clear();
	cout << '\n' << "Teacher added!";
	cout << '\n' << "Note: Teacher's default login details are their first name and last name. This can be changed later";

	writeNewTeacherToFile(newTeacherDetails);
	writeNewAccountToFile(newAccountDetails);
}

void deleteStudent(StudentDetails& studentDetailsToDelete) {
	AccountDetails studentAccountToDelete = findAccountWithID(studentDetailsToDelete.ID);
	if (!studentAccountToDelete.isValid()) {
		clear();
		cout << '\n' << "No account linked to student found";
	}

	vector<AccountDetails> newListOfAccounts = loadNewAccountsAfterDeleted(studentAccountToDelete);
	vector<StudentDetails> newListOfStudents = loadNewStudentsAfterDeleted(studentDetailsToDelete);

	
	writeNewListOfStudentsToFile(newListOfStudents);
	writeNewListOfAccountsToFile(newListOfAccounts);

	clear();
	cout << '\n' << "Student successfully deleted!" << '\n';
}

void editStudentDetail(StudentDetails newStudentDetails, StudentDetails currentStudentDetails) {
	vector<StudentDetails> listOfStudents = loadStudents();
	vector<StudentDetails> newListOfStudents = loadNewStudentsAfterEdited(newStudentDetails, currentStudentDetails);

	writeNewListOfStudentsToFile(newListOfStudents);

	clear();
	cout << '\n' << "Student detail changed!" << '\n';
}

StudentDetails getNewStudentDetailInput(StudentDetails currentStudentDetails) {
	StudentDetails newStudentDetails = currentStudentDetails;
	StudentDetails inValid;
	int category;
	string newDetail;

	cin >> category;

	if (category > g_categories.size() - 1 || category < 0) {
		clear();
		cout << '\n' << "Please choose one of the options";
		return inValid;
	}
	else if (category == 0) {
		clear();
		return inValid;
	}
	else if (category == 6) {
		vector<TeacherDetails> listOfTeachers = loadTeachers();
		cout << '\n' << "Choose new teacher " << '\n' << '\n';
		newStudentDetails.teacherID = chooseTeacher(listOfTeachers);

		return newStudentDetails;
	}

	cout << '\n' << "Enter new detail: ";
	cin >> newDetail;

	switch (category) {
	case 1:
		newStudentDetails.lastName = newDetail;
		break;
	case 2:
		newStudentDetails.firstName = newDetail;
		break;
	case 3:
		newStudentDetails.yearNum = stoi(newDetail);
		break;
	case 4:
		newStudentDetails.fatherName = newDetail;
		break;
	case 5:
		newStudentDetails.motherName = newDetail;
		break;
	case 7:
		newStudentDetails.mathsGrade = stoi(newDetail);
		break;
	case 8:
		newStudentDetails.englishGrade = stoi(newDetail);
		break;
	case 9:
		newStudentDetails.scienceGrade = stoi(newDetail);
		break;
	case 10:
		newStudentDetails.chemistryGrade = stoi(newDetail);
		break;
	case 11:
		newStudentDetails.historyGrade = stoi(newDetail);
		break;
	default:
		clear();
		cout << '\n' << "Please choose one of the options";
	}

	
	return newStudentDetails;
}

bool printDeleteStudentMenu(StudentDetails studentDetails, string studentName) {
	int confirm;

	cout << '\n' << "Are you sure you want to delete " << studentName << "?" << '\n';
	cout << '\n' << "[YES = 1] [NO = 0]" << '\n';
	cin >> confirm;

	if (confirm == 0) {
		clear();
		cout << '\n' << "Cancelled";
		return true;
	}
	deleteStudent(studentDetails);
	return false;
}

bool printEditStudentMenu(StudentDetails& studentDetails) {
	StudentDetails newStudentDetails;
	bool shouldContinue = true;
	int confirm =0;

	while (confirm != 1) {
		cout << '\n' << "What do you want to edit?" << '\n' << '\n';

		for (size_t i = 0; i < g_categories.size() - 1; i++) {
			cout << "[" << uppercase(g_categories[i]) << " = " << i + 1 << "] ";
		}

		cout << "[BACK = 0]" << '\n';

		newStudentDetails = getNewStudentDetailInput(studentDetails);
		if (!newStudentDetails.isValid()) {
			return true;
		}
		cout << '\n' << "Confirm edit detail?" << '\n';
		cout << '\n' << "[YES = 1] [NO = 0]" << '\n';
		cin >> confirm;

		if (confirm == 0) {
			clear();
			cout << '\n' << "Cancelled";
			return true;
		} 
	}
	editStudentDetail(newStudentDetails, studentDetails);
	return false;
}

void printAllStudents(vector<StudentDetails>& listOfStudents) {
	vector<TeacherDetails> listOfTeachers = loadTeachers();
	string teacherName;
	string lastname;
	cout << '\n' << printTableLine() << '\n';
	cout << '\n';
	// prints the category of each collumn
	for (size_t i = 0; i < g_categories.size(); i++) {
		cout << category(g_categories[i]);
	}
	cout << '\n' << printTableLine() << '\n';

	for (StudentDetails student : listOfStudents) {
		// turns teacher id into name
		for (TeacherDetails teacher : listOfTeachers) {
			if (teacher.ID == student.teacherID) {
				lastname = teacher.lastName;
				lastname[0] = toupper(teacher.lastName[0]);
				teacherName = teacher.title + lastname;
				break;
			}
		}
		//prints content for student
		cout << column(student.lastName)
			<< column(student.firstName)
			<< column(to_string(student.yearNum))
			<< column(student.fatherName)
			<< column(student.motherName)
			<< column(teacherName)
			<< column(to_string(student.mathsGrade))
			<< column(to_string(student.englishGrade))
			<< column(to_string(student.scienceGrade))
			<< column(to_string(student.chemistryGrade))
			<< column(to_string(student.historyGrade))
			<< column(to_string(student.overallGrade)) << '\n';
	}
	cout << printTableLine() << '\n';

	// FYI: "column" and "category" functions set the column size
}

void printEditOrDeleteMenu(StudentDetails studentDetails) {
	vector<StudentDetails> listOfStudents = loadStudents();
	bool shouldContinue = true;

	string firstName = studentDetails.firstName;
	string lastName = studentDetails.lastName;
	firstName[0] = toupper(firstName[0]);
	lastName[0] = toupper(lastName[0]);
	string studentName = firstName + " " + lastName; // TitleCase

	while (shouldContinue) {
		line();
		cout << uppercase(studentName) << "'S DETAILS";
		line();

		printStudentDetails(studentDetails);

		cout << '\n' << "What do you want to do?" << '\n';
		cout << '\n' << "[EDIT = 1] [DELETE = 2] [BACK = 0]" << '\n';

		int userChoice;
		cin >> userChoice;

		switch (userChoice) {
		case 0:
			clear();
			return;
		case 1:
			shouldContinue = printEditStudentMenu(studentDetails);
			break;
		case 2:
			shouldContinue = printDeleteStudentMenu(studentDetails, studentName);
			break;
		default:
			clear();
			cout << '\n' << "Please choose one of the options";
		}
	}
}

void getAdminMenuInput(AccountDetails& userAccountDetails) {
	srand((unsigned int)time(0));
	bool isAvaliableID = false;
	int randomID = rand() & 1000;
	vector<AccountDetails> listOfAccounts = loadAccounts();
	StudentDetails studentDetails;

	while (!isAvaliableID) {
		for (AccountDetails account : listOfAccounts) {
			if (account.ID != randomID) {
				break;
			}
			randomID = rand() & 1000;
		}
		isAvaliableID = true;
	}

	int userChoice;
	cin >> userChoice;

	switch (userChoice) {
	case 0:
		clear();
		cout << '\n' << "Logged out!";
		return;
	case 1:
		changeLoginDetails(userAccountDetails);
		return;
	case 2:
		studentDetails = getSelectStudentInput();
		if (!studentDetails.isValid()) {
			break;
		}
		clear();
		printEditOrDeleteMenu(studentDetails);
		break;
	case 3:
		addNewStudent(randomID);
		break;
	case 4:
		addNewTeacher(randomID);
		break;
	case 5: 
		addNewAdmin();
		break;
	default:
		clear();
		cout << '\n' << "Please choose one of the options";
		break;
	}
	printAdminMenu(userAccountDetails);
}

void printAdminMenu(AccountDetails& userAccountDetails) {
	vector<StudentDetails> listOfStudents = loadStudents();

	line();
	cout << "ADMIN DASHBOARD";
	line();

	cout << "All students in the school:";
	printAllStudents(listOfStudents);
	cout << '\n' << "[CHANGE LOGIN DETAILS = 1] [SELECT STUDENT = 2] [ADD STUDENT = 3] [ADD TEACHER = 4] [ADD ADMIN = 5] [LOG OUT = 0]" << '\n';

	getAdminMenuInput(userAccountDetails);
}

//____________________________________________________________________________________________________________________________
// TEACHER
//____________________________________________________________________________________________________________________________

TeacherDetails findTeacherWithID(int& accountID) {
	vector<TeacherDetails> listOfTeachers = loadTeachers();
	TeacherDetails inValid;

	// loops through each teacher in database
	for (TeacherDetails teacher : listOfTeachers) {
		// finds account ID which matches teacher ID
		if (accountID == teacher.ID) {
			return teacher;
		}
	}

	// should never happen
	return inValid;
}

StudentDetails getNewGradeInput(StudentDetails studentDetails) {
	StudentDetails newStudentDetails = studentDetails;
	StudentDetails inValid;
	vector<StudentDetails> listOfStudents = loadStudents();

	int chosenCategory;
	cin >> chosenCategory;

	if (chosenCategory == 0) {
		clear();
		return inValid;
	}

	cout << '\n' << "Enter new grade (0 - 20): ";

	int newGrade;
	cin >> newGrade;

	if (newGrade > 20) {
		clear();
		cout << '\n' << "The max grade is 20";
		printEditGradesMenu(newStudentDetails);
	}
	else if (newGrade < 0) {
		clear();
		cout << '\n' << "Grade cannot be below 0";
		printEditGradesMenu(newStudentDetails);
	}

	switch (chosenCategory) {
	case 0:
		clear();
		return inValid;
	case 1:
		newStudentDetails.mathsGrade = newGrade;
		break;
	case 2:
		newStudentDetails.englishGrade = newGrade;
		break;
	case 3:
		newStudentDetails.scienceGrade = newGrade;
		break;
	case 4:
		newStudentDetails.chemistryGrade = newGrade;
		break;
	case 5:
		newStudentDetails.historyGrade = newGrade;
		break;
	default:
		cout << '\n' << "Please choose one of the options";
		printEditGradesMenu(newStudentDetails);
	}

	return newStudentDetails;
}

bool changeGrade(StudentDetails studentDetails) {	
	int confirm = 0;
	StudentDetails newStudentDetails;

	while (confirm != 1) {
		newStudentDetails = getNewGradeInput(studentDetails);
		if (!newStudentDetails.isValid()) {
			return false;
		}

		cout << '\n' << "Confirm grade change?" << '\n';
		cout << '\n' << "[YES = 1] [NO = 0]" << '\n';

		cin >> confirm;

		if (confirm == 0) {
			clear();
			cout << '\n' << "Cancelled";

			return true;
		}
	}

	vector<StudentDetails> newListOfStudents = loadNewStudentsAfterEdited(newStudentDetails, studentDetails);

	writeNewListOfStudentsToFile(newListOfStudents);

	clear();
	cout << '\n' << "Grade changed!";

	return false;
}

void printEditGradesMenu(StudentDetails& studentDetails) {
	string studentName = studentDetails.firstName + " " + studentDetails.lastName;
	bool shouldContinue = true;

	while (shouldContinue) {
		line();
		cout << "EDIT " << uppercase(studentName) << "'S GRADES";
		line();

		printStudentDetails(studentDetails);

		cout << '\n' << "What do you want to edit?" << '\n' << '\n';

		// prints out only the reports as options
		for (size_t i = 6; i < g_categories.size() - 1; i++) {
			cout << "[" << uppercase(g_categories[i]) << " = " << i - 5 << "] ";;
		}
		cout << "[BACK = 0]" << '\n';

		shouldContinue = changeGrade(studentDetails);
	}
}

void printClassDetails(vector<StudentDetails>& listOfStudentsInClass, string teacherName) {
	cout << '\n' << printTableLine() << '\n';
	cout << '\n';
	// prints the category of each collumn
	for (size_t i = 0; i < g_categories.size(); i++) {
		cout << category(g_categories[i]);
	}
	cout << '\n' << printTableLine() << '\n';

	for (StudentDetails student : listOfStudentsInClass) {
		//prints content for student
		cout << column(student.lastName)
			<< column(student.firstName)
			<< column(to_string(student.yearNum))
			<< column(student.fatherName)
			<< column(student.motherName)
			<< column(teacherName)
			<< column(to_string(student.mathsGrade))
			<< column(to_string(student.englishGrade))
			<< column(to_string(student.scienceGrade))
			<< column(to_string(student.chemistryGrade))
			<< column(to_string(student.historyGrade))
			<< column(to_string(student.overallGrade)) << '\n';
	}
	cout << printTableLine() << '\n';
	// FYI: "column" and "category" functions set the column size
}

void getTeacherMenuInput(AccountDetails& userAccountDetails, vector<StudentDetails>& listOfStudentsInClass) {
	StudentDetails studentDetails;

	int userChoice;
	cin >> userChoice;

	switch (userChoice) {
	case 0:
		clear();
		cout << '\n' << "Logged out!" << '\n';
		return;
	case 1:
		changeLoginDetails(userAccountDetails);
		return;
	case 2:
		studentDetails = getSelectStudentInput();
		if (!studentDetails.isValid()) {
			break;
		}
		clear();
		printEditGradesMenu(studentDetails);
		break;
	default:
		clear();
		cout << '\n' << "Please choose one of the options" << '\n';
		break;
	}
	printTeacherMenu(userAccountDetails);
}

void printTeacherMenu(AccountDetails& userAccountDetails) {
	TeacherDetails userTeacherDetails = findTeacherWithID(userAccountDetails.ID);
	vector<StudentDetails> listOfStudentsInClass = loadClass(userTeacherDetails);

	if (!userTeacherDetails.isValid()) {
		cout << '\n' << "No teacher found linked to this account" << '\n';
	}

	string lastname = userTeacherDetails.lastName;
	lastname[0] = toupper(userTeacherDetails.lastName[0]);
	string teacherName = userTeacherDetails.title + lastname;

	line();
	cout << uppercase(teacherName) << "'S DASHBOARD";
	line();
	cout <<"All students in your class:";
	printClassDetails(listOfStudentsInClass, teacherName);
	cout << '\n' << "[CHANGE LOGIN DETAILS = 1] [SELECT STUDENT = 2] [LOG OUT = 0]" << '\n';

	getTeacherMenuInput(userAccountDetails, listOfStudentsInClass);
}

//____________________________________________________________________________________________________________________________
// STUDENT
//____________________________________________________________________________________________________________________________

StudentDetails findStudentWithID(int& accountID) {
	vector<StudentDetails> listOfStudents = loadStudents();
	StudentDetails inValid;

	// loops through each student in database
	for (StudentDetails student : listOfStudents) {
		// finds student ID which matches account ID

		if (accountID == student.ID) {
			return student;
		}
	}

	// should never happen
	return inValid;
}

void getStudentMenuInput(AccountDetails& userAccountDetails) {
	int userChoice;
	cin >> userChoice;

	switch (userChoice) {
	case 0:
		clear();
		cout << '\n' << "Logged out!" << '\n';
		return;
	case 1:
		changeLoginDetails(userAccountDetails);
		return;
	default:
		clear();
		cout << '\n' << "Please choose one of the options" << '\n';
		break;
	}
	printStudentMenu(userAccountDetails);
}

void printStudentMenu(AccountDetails& userAccountDetails) {
	StudentDetails userStudentDetails = findStudentWithID(userAccountDetails.ID);

	if (!userAccountDetails.isValid()) {
		cout << '\n' << "No student found linked to this account" << '\n';
	}

	line();
	cout << uppercase(userStudentDetails.firstName) << "'S DASHBOARD";
	line();

	cout << "Personal details:";
	printStudentDetails(userStudentDetails);
	cout << '\n' << "[CHANGE LOGIN DETAILS = 1] [LOG OUT = 0]" << '\n';

	getStudentMenuInput(userAccountDetails);
}

//____________________________________________________________________________________________________________________________
// MAIN MENU STUFF
//____________________________________________________________________________________________________________________________

AccountDetails authenticateUser(vector<AccountDetails>& listOfAccounts, AccountDetails& userInfoToAuthenticate) {
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

AccountDetails getLoginDetailsInput() {
	AccountDetails userLoginDetails;

	cout << '\n' << "Username: ";
	cin >> userLoginDetails.username;
	cout << "Password: ";
	cin >> userLoginDetails.password;

	return userLoginDetails;
}

AccountDetails printLoginMenuAndAuthenticate() {
	int loginAttempts = 3;
	AccountDetails authenticatedUserAccountDetails;

	while (loginAttempts > 0) {
		line();
		cout << "LOGIN";
		line();
		cout << "Enter your details" << '\n';

		vector<AccountDetails> listOfAccounts = loadAccounts();
		AccountDetails userInputtedAccountDetails = getLoginDetailsInput();
		authenticatedUserAccountDetails = authenticateUser(listOfAccounts, userInputtedAccountDetails);
		if (authenticatedUserAccountDetails.isValid()) {
			clear();
			cout << '\n' << "Logged in!";
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

void getThemeMenuInput() {
	int userChoice;
	cin >> userChoice;

	switch (userChoice) {
	case 0:
		clear();
		return;
	case 1:
		clear();
		setStyleDefault();
		break;
	case 2:
		clear();
		setStyleModern();
		break;
	case 3:
		clear();
		setStyleClassic();
		break;
	case 4:
		clear();
		setStyleHacker();
		break;
	default:
		clear();
		cout << '\n' << "Please choose one of the options" << '\n';
		break;
	}
	printThemeMenu();
}

void printThemeMenu() {
	line();
	cout << "THEMES";
	line();

	cout << "Choose theme" << '\n';
	cout << '\n' << "[DEFAULT = 1] [MODERN = 2] [CLASSIC (UNSTABLE) = 3] [HACKER (UNSTABLE) = 4] [BACK = 0]" << '\n';

	getThemeMenuInput();
}

void getStartMenuInput() {
	AccountDetails userAccountDetails;

	int userChoice;
	cin >> userChoice;

	switch (userChoice) {
	case 0:
		cout << '\n' << "Shutting down application..." << '\n';
		return;
	case 1:
		clear();
		userAccountDetails = printLoginMenuAndAuthenticate();
		if (!userAccountDetails.isValid()) {
			clear();
			cout << '\n' << "Too many failed attempts" << '\n';
			break;
		}
		switchToAccount(userAccountDetails);
		break;
	case 2:
		clear();
		printThemeMenu();
		break;
	default:
		clear();
		cout << '\n' << "Please choose one of the options";
		break;
	}
	printStartMenu();
}

void printStartMenu() {
	line();
	cout << "WELLINGTON HIGH SCHOOL STUDENT INFORMATION SYSTEM";
	line();

	cout << "Please input the number to the following options" << '\n';
	cout << '\n' << "[LOGIN = 1] [THEME = 2] [EXIT = 0]" << '\n';
	getStartMenuInput();
}

int main() {
	fullscreen();
	setStyleModern();
	printStartMenu();
}