#include "attendance.h"
#include "filehandler.h"
#include "course_ops.h"
#include <iostream>

// backup for undo
static vector<vector<string>> attendanceBackup;

string nextLogId() {
    vector<vector<string>> rows = readTXT(ATTENDANCE_FILE);
    int next = rows.size() + 1;
    string id = "L";
    if (next < 10000) id = id + "0";
    if (next < 1000) id = id + "0";
    if (next < 100) id = id + "0";
    if (next < 10) id = id + "0";
    return id + to_string(next);
}

void markAttendance(string courseCode, string date) {
    vector<vector<string>> enrolled = listEnrolledStudents(courseCode);
    if (enrolled.size() == 0) {
        cout << "No enrolled students in this course.\n";
        return;
    }

    // save backup before changes
    attendanceBackup = readTXT(ATTENDANCE_FILE);

    int baseCount = attendanceBackup.size();
    for (int i = 0; i < enrolled.size(); i++) {
        string roll = enrolled[i][0];
        string name = enrolled[i][1];
        char status;
        cout << roll << " (" << name << ") - P/A/L: ";
        cin >> status;
        if (status == 'p' || status == 'P') status = 'P';
        else if (status == 'a' || status == 'A') status = 'A';
        else if (status == 'l' || status == 'L') status = 'L';
        else {
            cout << "Invalid, marking Absent.\n";
            status = 'A';
        }

        int next = baseCount + i + 1;
        string id = "L";
        if (next < 10000) id = id + "0";
        if (next < 1000) id = id + "0";
        if (next < 100) id = id + "0";
        if (next < 10) id = id + "0";
        id = id + to_string(next);

        vector<string> row;
        row.push_back(id);
        row.push_back(roll);
        row.push_back(courseCode);
        row.push_back(date);
        row.push_back(string(1, status));
        appendTXT(ATTENDANCE_FILE, row);
    }
    cout << "Attendance saved.\n";
}

double getAttendancePct(string roll, string courseCode) {
    vector<vector<string>> logs = readTXT(ATTENDANCE_FILE);
    int total = 0;
    double present = 0;

    for (int i = 0; i < logs.size(); i++) {
        if (logs[i][1] == roll && logs[i][2] == courseCode) {
            total++;
            if (logs[i][4] == "P") present = present + 1.0;
            else if (logs[i][4] == "L") present = present + 0.5;
        }
    }
    if (total == 0) return 100.0;
    return (present / total) * 100.0;
}

vector<vector<string>> getShortageList() {
    vector<vector<string>> enrollments = readTXT(ENROLLMENTS_FILE);
    vector<vector<string>> shortage;

    for (int i = 0; i < enrollments.size(); i++) {
        if (enrollments[i][5] == "active" || enrollments[i][5] == "enrolled") {
            string roll = enrollments[i][1];
            string course = enrollments[i][2];
            double pct = getAttendancePct(roll, course);
            if (pct < 75.0) {
                vector<string> row;
                row.push_back(roll);
                row.push_back(course);
                row.push_back(to_string(pct));
                shortage.push_back(row);
            }
        }
    }
    return shortage;
}

bool undoLastSession() {
    if (attendanceBackup.size() == 0) {
        cout << "No backup available.\n";
        return false;
    }
    writeTXT(ATTENDANCE_FILE, ATTENDANCE_HEADER, attendanceBackup);
    attendanceBackup.clear();
    cout << "Last session undone.\n";
    return true;
}

void printDailySheet(string courseCode, string date) {
    vector<vector<string>> logs = readTXT(ATTENDANCE_FILE);
    vector<vector<string>> enrolled = listEnrolledStudents(courseCode);

    cout << "\n--- Daily Sheet: " << courseCode << " | " << date << " ---\n";
    cout << "Roll No       | Name                  | Status\n";
    cout << "----------------------------------------------\n";

    for (int i = 0; i < enrolled.size(); i++) {
        string roll = enrolled[i][0];
        string name = enrolled[i][1];
        string status = "-";
        for (int j = 0; j < logs.size(); j++) {
            if (logs[j][1] == roll && logs[j][2] == courseCode && logs[j][3] == date) {
                status = logs[j][4];
                break;
            }
        }
        cout << roll << " | " << name << " | " << status << endl;
    }
    cout << "----------------------------------------------\n";
}
