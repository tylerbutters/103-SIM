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
	using std::to_string;
}

using namespace stdprefixes;

string g_studentsFile = "student-database.csv";
int g_columnWidth = 19;
vector<string> g_columnNames = { "[NAME]", "[SURNAME]", "[YEAR]", "[FATHER]", "[MOTHER]", "[CLASS NO.]", "[TEACHER]", "[REPORT]" };

struct StudentDetails {
	string firstName;
	string lastName;
	int yearNum;
	string fatherName;
	string motherName;
	int classNum;
	string teacherName;
	int reportNum;
};

vector<StudentDetails> loadAllStudents() {
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
		int i = 1;
		while (getline(stream, fileCell, ',')) {
			if (i == 1) {
				student.firstName = fileCell;
			}
			else if (i == 2) {
				student.lastName = fileCell;
			}
			else if (i == 3) {
				student.yearNum = stoi(fileCell);
			}
			else if (i == 4) {
				student.fatherName = fileCell;
			}
			else if (i == 5) {
				student.motherName = fileCell;
			}
			else if (i == 6) {
				student.classNum = stoi(fileCell);
			}
			else if (i == 7) {
				student.teacherName = fileCell;
			}
			else if (i == 8) {
				student.reportNum = stoi(fileCell);
			}

			i++;
		}
		// adds populated struct to vector
		listOfAccounts.push_back(student);
	}
	AccountsFile.close();

	return listOfAccounts;
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
	string outputString  ;

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
		outputString+= "_";
	}
	return outputString;
}

void outputAllStudents(vector<StudentDetails>& listOfStudents) {
	for (int i = 0; i < g_columnNames.size(); i++) {
		cout << title(g_columnNames[i]);
			
	}
	cout << '\n'<<generateLine() << '\n';

	//for each row in filecontent
	for (StudentDetails student : listOfStudents) {
		cout << column(student.firstName)
			<< column(student.lastName)
			<< column(to_string(student.yearNum))
			<< column(student.fatherName)
			<< column(student.motherName)
			<< column(to_string(student.classNum))
			<< column(student.teacherName)
			<< column(to_string(student.reportNum)) << '\n';
	}
}

int main()
{
	vector<StudentDetails> listOfStudents = loadAllStudents();

	outputAllStudents(listOfStudents);
}
