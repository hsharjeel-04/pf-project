#ifndef REPORTS_H
#define REPORTS_H

#include <string>

using namespace std;

void printMeritList();
void printAttendanceDefaulters();
void printFeeDefaulters();
void printSemesterResult(string semester);
void printDepartmentSummary();
void exportReportToFile(int reportChoice, string filename);

#endif
