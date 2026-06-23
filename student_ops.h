#ifndef STUDENT_OPS_H
#define STUDENT_OPS_H

#include <string>
#include <vector>

using namespace std;

const string STUDENTS_FILE = "Data/students.txt";
const string STUDENTS_HEADER = "roll_no,name,department,semester,cgpa,status";

bool isValidRoll(string roll);
bool nameHasDigits(string name);

// add new student to students.txt
bool addStudent(string roll, string name, string dept, string semester, string cgpa);

// search by roll number
vector<string> searchByRoll(string roll);

// search by name substring
vector<vector<string>> searchByName(string namePart);

// update one field (not roll)
bool updateStudent(string roll, string field, string newValue);

// set status to inactive
bool softDelete(string roll);

// get all active students sorted by roll (selection sort)
vector<vector<string>> listActiveStudents();

// bonus: search as you type by name prefix
void searchAsYouType();

#endif
