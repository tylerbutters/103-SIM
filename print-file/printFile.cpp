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
			if (i == 2) {
				student.lastName = fileCell;
			}
			if (i == 3) {
				student.yearNum = stoi(fileCell);
			}
			if (i == 4) {
				student.fatherName = fileCell;
			}
			if (i == 5) {
				student.motherName = fileCell;
			}
			if (i == 6) {
				student.classNum = stoi(fileCell);
			}
			if (i == 7) {
				student.teacherName = fileCell;
			}
			if (i == 8) {
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
	int columnLength = 14;
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
	int columnLength = 13;
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

void outputAllStudents(vector<StudentDetails>& listOfStudents) {
	cout << title("[NAME]")
		<< title("[SURNAME]")
		<< title("[YEAR]")
		<< title("[FATHER]")
		<< title("[MOTHER]")
		<< title("[CLASS NO.]")
		<< title("[TEACHER]")
		<< title("[REPORT]") << '\n'
		<< "________________________________________________________________________________________________________________" << '\n';

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
