#ifndef ATTENDANCE_H
#define ATTENDANCE_H

#include <string>
#include <vector>

using namespace std;

const string ATTENDANCE_FILE = "Data/attendance_log.txt";
const string ATTENDANCE_HEADER = "log_id,roll_no,course_code,session_date,status";

void markAttendance(string courseCode, string date);
double getAttendancePct(string roll, string courseCode);
vector<vector<string>> getShortageList();
bool undoLastSession();
void printDailySheet(string courseCode, string date);

#endif
