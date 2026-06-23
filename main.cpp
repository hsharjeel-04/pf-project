#include <iostream>
#include "student_ops.h"
#include "course_ops.h"
#include "attendance.h"
#include "grades.h"
#include "fee_tracker.h"
#include "reports.h"

using namespace std;

void studentMenu() {
    int choice;
    do {
        cout << "\n--- Student Management ---\n";
        cout << "1. Add Student\n2. Search by Roll\n3. Search by Name\n";
        cout << "4. Update Student\n5. Soft Delete\n6. List Active\n";
        cout << "7. Search As You Type (Bonus)\n0. Back\nChoice: ";
        cin >> choice;

        if (choice == 1) {
            string roll, name, dept, sem, cgpa;
            cout << "Roll (BSAI-YY-XXX): "; cin >> roll;
            cin.ignore();
            cout << "Name: "; getline(cin, name);
            cout << "Department: "; getline(cin, dept);
            cout << "Semester: "; cin >> sem;
            cout << "CGPA: "; cin >> cgpa;
            addStudent(roll, name, dept, sem, cgpa);
        } else if (choice == 2) {
            string roll;
            cout << "Roll: "; cin >> roll;
            vector<string> s = searchByRoll(roll);
            if (s.size() > 0) {
                cout << s[0] << " | " << s[1] << " | " << s[2]
                     << " | Sem " << s[3] << " | CGPA " << s[4]
                     << " | " << s[5] << endl;
            } else cout << "Not found.\n";
        } else if (choice == 3) {
            string name;
            cin.ignore();
            cout << "Name part: "; getline(cin, name);
            vector<vector<string>> results = searchByName(name);
            for (int i = 0; i < results.size(); i++) {
                cout << results[i][0] << " | " << results[i][1] << endl;
            }
        } else if (choice == 4) {
            string roll, field, val;
            cout << "Roll: "; cin >> roll;
            cout << "Field (name/department/semester/cgpa): "; cin >> field;
            cin.ignore();
            cout << "New value: "; getline(cin, val);
            updateStudent(roll, field, val);
        } else if (choice == 5) {
            string roll;
            cout << "Roll: "; cin >> roll;
            softDelete(roll);
        } else if (choice == 6) {
            vector<vector<string>> list = listActiveStudents();
            for (int i = 0; i < list.size(); i++) {
                cout << list[i][0] << " | " << list[i][1]
                     << " | CGPA " << list[i][4] << endl;
            }
        } else if (choice == 7) {
            cin.ignore();
            searchAsYouType();
        }
    } while (choice != 0);
}

void courseMenu() {
    int choice;
    do {
        cout << "\n--- Course Management ---\n";
        cout << "1. Enroll Student\n2. Drop Course\n3. Credit Load\n";
        cout << "4. List Enrolled\n0. Back\nChoice: ";
        cin >> choice;

        if (choice == 1) {
            string roll, code, sem, date;
            cout << "Roll: "; cin >> roll;
            cout << "Course code: "; cin >> code;
            cout << "Semester: "; cin >> sem;
            cout << "Date (DD-MM-YYYY): "; cin >> date;
            EnrollResult r = enrollStudent(roll, code, sem, date);
            cout << enrollResultMessage(r) << endl;
        } else if (choice == 2) {
            string roll, code, sem;
            cout << "Roll: "; cin >> roll;
            cout << "Course: "; cin >> code;
            cout << "Semester: "; cin >> sem;
            dropCourse(roll, code, sem);
        } else if (choice == 3) {
            string roll, sem;
            cout << "Roll: "; cin >> roll;
            cout << "Semester: "; cin >> sem;
            cout << "Credit load: " << getCreditLoad(roll, sem) << endl;
        } else if (choice == 4) {
            string code;
            cout << "Course code: "; cin >> code;
            vector<vector<string>> list = listEnrolledStudents(code);
            for (int i = 0; i < list.size(); i++) {
                cout << list[i][0] << " | " << list[i][1] << endl;
            }
        }
    } while (choice != 0);
}

void attendanceMenu() {
    int choice;
    do {
        cout << "\n--- Attendance ---\n";
        cout << "1. Mark Attendance\n2. Attendance %\n3. Shortage List\n";
        cout << "4. Undo Last Session\n5. Daily Sheet\n0. Back\nChoice: ";
        cin >> choice;

        if (choice == 1) {
            string code, date;
            cout << "Course: "; cin >> code;
            cout << "Date (DD-MM-YYYY): "; cin >> date;
            markAttendance(code, date);
        } else if (choice == 2) {
            string roll, code;
            cout << "Roll: "; cin >> roll;
            cout << "Course: "; cin >> code;
            cout << "Attendance: " << getAttendancePct(roll, code) << "%\n";
        } else if (choice == 3) {
            vector<vector<string>> list = getShortageList();
            for (int i = 0; i < list.size(); i++) {
                cout << list[i][0] << " | " << list[i][1]
                     << " | " << list[i][2] << "%\n";
            }
        } else if (choice == 4) {
            undoLastSession();
        } else if (choice == 5) {
            string code, date;
            cout << "Course: "; cin >> code;
            cout << "Date: "; cin >> date;
            printDailySheet(code, date);
        }
    } while (choice != 0);
}

void gradesMenu() {
    int choice;
    do {
        cout << "\n--- Grades ---\n";
        cout << "1. Enter Marks\n2. Compute GPA\n3. Class Stats\n0. Back\nChoice: ";
        cin >> choice;

        if (choice == 1) {
            string roll, code, sem;
            cout << "Roll: "; cin >> roll;
            cout << "Course: "; cin >> code;
            cout << "Semester: "; cin >> sem;
            enterMarks(roll, code, sem);
        } else if (choice == 2) {
            string roll, sem;
            cout << "Roll: "; cin >> roll;
            cout << "Semester: "; cin >> sem;
            cout << "GPA: " << computeGPA(roll, sem) << endl;
        } else if (choice == 3) {
            string code, sem;
            cout << "Course: "; cin >> code;
            cout << "Semester: "; cin >> sem;
            Stats s = computeClassStats(code, sem);
            cout << "Highest: " << s.highest << " Lowest: " << s.lowest
                 << " Mean: " << s.mean << " Median: " << s.median << endl;
        }
    } while (choice != 0);
}

void feesMenu() {
    int choice;
    do {
        cout << "\n--- Fee Tracker ---\n";
        cout << "1. Record Payment\n2. Late Fine\n3. Receipt\n";
        cout << "4. Defaulters\n0. Back\nChoice: ";
        cin >> choice;

        if (choice == 1) {
            string roll, sem, date;
            double amt;
            cout << "Roll: "; cin >> roll;
            cout << "Semester: "; cin >> sem;
            cout << "Amount: "; cin >> amt;
            cout << "Date (DD-MM-YYYY): "; cin >> date;
            recordPayment(roll, sem, amt, date);
        } else if (choice == 2) {
            string roll, sem;
            cout << "Roll: "; cin >> roll;
            cout << "Semester: "; cin >> sem;
            cout << "Fine: " << computeLateFine(roll, sem) << endl;
        } else if (choice == 3) {
            string roll, sem;
            cout << "Roll: "; cin >> roll;
            cout << "Semester: "; cin >> sem;
            generateReceipt(roll, sem);
        } else if (choice == 4) {
            vector<vector<string>> d = getDefaulters();
            for (int i = 0; i < d.size(); i++) {
                cout << d[i][1] << " owes " << d[i][d[i].size()-1] << endl;
            }
        }
    } while (choice != 0);
}

void reportsMenu() {
    int choice;
    do {
        cout << "\n--- Reports ---\n";
        cout << "1. Merit List\n2. Attendance Defaulters\n3. Fee Defaulters\n";
        cout << "4. Semester Result\n5. Department Summary\n6. Export to File\n0. Back\nChoice: ";
        cin >> choice;

        if (choice == 1) printMeritList();
        else if (choice == 2) printAttendanceDefaulters();
        else if (choice == 3) printFeeDefaulters();
        else if (choice == 4) {
            string sem;
            cout << "Semester: "; cin >> sem;
            printSemesterResult(sem);
        } else if (choice == 5) printDepartmentSummary();
        else if (choice == 6) {
            int r;
            string fname;
            cout << "Report (1=Merit,2=Attend,3=Fee,4=Dept): "; cin >> r;
            cout << "Filename: "; cin >> fname;
            exportReportToFile(r, fname);
        }
    } while (choice != 0);
}

int main() {
    int choice;
    cout << "====================================\n";
    cout << "   CAMPUS ANALYTICS ENGINE v1.0\n";
    cout << "====================================\n";

    do {
        cout << "\n=== MAIN MENU ===\n";
        cout << "1. Students\n2. Courses\n3. Attendance\n";
        cout << "4. Grades\n5. Fees\n6. Reports\n0. Exit\nChoice: ";
        cin >> choice;

        if (choice == 1) studentMenu();
        else if (choice == 2) courseMenu();
        else if (choice == 3) attendanceMenu();
        else if (choice == 4) gradesMenu();
        else if (choice == 5) feesMenu();
        else if (choice == 6) reportsMenu();
        else if (choice == 0) cout << "Goodbye!\n";
        else cout << "Invalid choice.\n";
    } while (choice != 0);

    return 0;
}
