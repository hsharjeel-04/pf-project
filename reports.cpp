#include "reports.h"
#include "student_ops.h"
#include "attendance.h"
#include "fee_tracker.h"
#include "grades.h"
#include "course_ops.h"
#include "filehandler.h"
#include <iostream>
#include <fstream>
#include <iomanip>

void printMeritList() {
    vector<vector<string>> students = listActiveStudents();

    // sort by CGPA descending (selection sort)
    for (int i = 0; i < students.size() - 1; i++) {
        int maxIdx = i;
        for (int j = i + 1; j < students.size(); j++) {
            double cgpaJ = stod(students[j][4]);
            double cgpaMax = stod(students[maxIdx][4]);
            if (cgpaJ > cgpaMax) maxIdx = j;
        }
        if (maxIdx != i) {
            vector<string> temp = students[i];
            students[i] = students[maxIdx];
            students[maxIdx] = temp;
        }
    }

    cout << "\n======== MERIT LIST ========\n";
    cout << setw(5) << "Rank" << setw(14) << "Roll" << setw(22) << "Name"
         << setw(8) << "CGPA" << endl;
    cout << "------------------------------------------\n";
    for (int i = 0; i < students.size(); i++) {
        cout << setw(5) << (i + 1) << setw(14) << students[i][0]
             << setw(22) << students[i][1] << setw(8) << students[i][4] << endl;
    }
    cout << "------------------------------------------\n";
}

void printAttendanceDefaulters() {
    vector<vector<string>> shortage = getShortageList();
    cout << "\n=== ATTENDANCE DEFAULTERS (<75%) ===\n";
    cout << setw(14) << "Roll" << setw(12) << "Course" << setw(10) << "Pct%" << endl;
    for (int i = 0; i < shortage.size(); i++) {
        vector<string> student = searchByRoll(shortage[i][0]);
        string name = student.size() > 1 ? student[1] : "Unknown";
        cout << setw(14) << shortage[i][0] << setw(22) << name
             << setw(12) << shortage[i][1] << setw(10) << shortage[i][2] << endl;
    }
    if (shortage.size() == 0) cout << "No defaulters.\n";
}

void printFeeDefaulters() {
    vector<vector<string>> defaulters = getDefaulters();
    cout << "\n=== FEE DEFAULTERS ===\n";
    cout << setw(14) << "Roll" << setw(12) << "Semester"
         << setw(14) << "Outstanding" << endl;
    for (int i = 0; i < defaulters.size(); i++) {
        string balance = defaulters[i][defaulters[i].size() - 1];
        int weeks = 0;
        if (defaulters[i][6] != "00-00-0000") {
            weeks = daysBetween(defaulters[i][5], defaulters[i][6]) / 7;
        }
        cout << setw(14) << defaulters[i][1] << setw(12) << defaulters[i][2]
             << setw(14) << balance << " (" << weeks << " wks)" << endl;
    }
    if (defaulters.size() == 0) cout << "No defaulters.\n";
}

void printSemesterResult(string semester) {
    vector<vector<string>> enrollments = readTXT(ENROLLMENTS_FILE);
    cout << "\n+============================================================+\n";
    cout << "|              SEMESTER " << semester << " RESULT SHEET                  |\n";
    cout << "+==========+====================+=======+=====+============+\n";
    cout << "| Roll     | Name               | Course| Grd | Attend%    |\n";
    cout << "+==========+====================+=======+=====+============+\n";

    for (int i = 0; i < enrollments.size(); i++) {
        if (enrollments[i][3] != semester) continue;
        if (enrollments[i][5] != "active" && enrollments[i][5] != "enrolled") continue;

        string roll = enrollments[i][1];
        string course = enrollments[i][2];
        vector<string> student = searchByRoll(roll);
        string name = student.size() > 1 ? student[1] : "Unknown";
        if (name.length() > 18) name = name.substr(0, 18);

        vector<string> gradeRow;
        string grade = "-";
        vector<vector<string>> allGrades = readTXT(GRADES_FILE);
        for (int g = 0; g < allGrades.size(); g++) {
            if (allGrades[g][0] == roll && allGrades[g][1] == course) {
                grade = allGrades[g][4];
                break;
            }
        }
        double pct = getAttendancePct(roll, course);
        cout << "| " << setw(8) << roll << " | " << setw(18) << name
             << " | " << setw(5) << course << " | " << setw(3) << grade
             << " | " << setw(8) << fixed << setprecision(1) << pct << "% |\n";
    }
    cout << "+==========+====================+=======+=====+============+\n";
}

void printDepartmentSummary() {
    vector<vector<string>> students = listActiveStudents();

    // collect unique departments
    string depts[50];
    int deptCount = 0;

    for (int i = 0; i < students.size(); i++) {
        string d = students[i][2];
        bool found = false;
        for (int j = 0; j < deptCount; j++) {
            if (depts[j] == d) { found = true; break; }
        }
        if (!found) {
            depts[deptCount] = d;
            deptCount++;
        }
    }

    cout << "\n=== DEPARTMENT SUMMARY ===\n";
    for (int d = 0; d < deptCount; d++) {
        int count = 0;
        double cgpaSum = 0;
        int passCount = 0;

        for (int i = 0; i < students.size(); i++) {
            if (students[i][2] == depts[d]) {
                count++;
                double cgpa = stod(students[i][4]);
                cgpaSum = cgpaSum + cgpa;
                if (cgpa >= 2.0) passCount++;
            }
        }
        double avgCgpa = count > 0 ? cgpaSum / count : 0;
        double passRate = count > 0 ? (passCount * 100.0 / count) : 0;

        cout << depts[d] << ": Students=" << count
             << " Avg CGPA=" << fixed << setprecision(2) << avgCgpa
             << " Pass Rate=" << passRate << "%\n";
    }
}

void exportReportToFile(int reportChoice, string filename) {
    ofstream outFile(filename);
    streambuf* coutBackup = cout.rdbuf();
    cout.rdbuf(outFile.rdbuf());

    if (reportChoice == 1) printMeritList();
    else if (reportChoice == 2) printAttendanceDefaulters();
    else if (reportChoice == 3) printFeeDefaulters();
    else if (reportChoice == 4) printDepartmentSummary();

    cout.rdbuf(coutBackup);
    outFile.close();
    cout << "Report saved to " << filename << endl;
}
