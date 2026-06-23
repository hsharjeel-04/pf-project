#include "course_ops.h"
#include "filehandler.h"
#include "student_ops.h"
#include <iostream>

string enrollResultMessage(EnrollResult result) {
    if (result == ENROLL_OK) return "Enrolled successfully.";
    if (result == ENROLL_STUDENT_INACTIVE) return "Student is not active.";
    if (result == ENROLL_COURSE_NOT_FOUND) return "Course not found.";
    if (result == ENROLL_NO_SEATS) return "No seats available.";
    if (result == ENROLL_ALREADY_ENROLLED) return "Already enrolled in this course.";
    if (result == ENROLL_CREDIT_OVERLOAD) return "Credit load exceeds 21 hours.";
    if (result == ENROLL_PREREQ_FAIL) return "Prerequisite not passed.";
    return "Unknown error.";
}

bool checkPrerequisite(string roll, string courseCode) {
    vector<string> course = findRow(COURSES_FILE, 0, courseCode);
    if (course.size() == 0) return false;

    string prereq = course[6];
    if (prereq == "NONE" || prereq == "") return true;

    vector<vector<string>> grades = readTXT(GRADES_FILE);
    for (int i = 0; i < grades.size(); i++) {
        if (grades[i][0] == roll && grades[i][1] == prereq) {
            if (grades[i].size() > 4 && grades[i][4] != "F") {
                return true;
            }
        }
    }
    return false;
}

int getCreditLoad(string roll, string semester) {
    vector<vector<string>> enrollments = readTXT(ENROLLMENTS_FILE);
    vector<vector<string>> courses = readTXT(COURSES_FILE);
    int total = 0;

    for (int i = 0; i < enrollments.size(); i++) {
        if (enrollments[i][1] == roll && enrollments[i][3] == semester) {
            if (enrollments[i][5] == "active" || enrollments[i][5] == "enrolled") {
                string code = enrollments[i][2];
                for (int j = 0; j < courses.size(); j++) {
                    if (courses[j][0] == code) {
                        total = total + stoi(courses[j][2]);
                        break;
                    }
                }
            }
        }
    }
    return total;
}

int countEnrolledInCourse(string courseCode) {
    vector<vector<string>> enrollments = readTXT(ENROLLMENTS_FILE);
    int count = 0;
    for (int i = 0; i < enrollments.size(); i++) {
        if (enrollments[i][2] == courseCode) {
            if (enrollments[i][5] == "active" || enrollments[i][5] == "enrolled") {
                count++;
            }
        }
    }
    return count;
}

EnrollResult enrollStudent(string roll, string courseCode, string semester, string date) {
    vector<string> student = searchByRoll(roll);
    if (student.size() == 0 || student[5] != "active") {
        return ENROLL_STUDENT_INACTIVE;
    }

    vector<string> course = findRow(COURSES_FILE, 0, courseCode);
    if (course.size() == 0) {
        return ENROLL_COURSE_NOT_FOUND;
    }

    int capacity = stoi(course[4]);
    int enrolled = countEnrolledInCourse(courseCode);
    if (enrolled >= capacity) {
        return ENROLL_NO_SEATS;
    }

    vector<vector<string>> enrollments = readTXT(ENROLLMENTS_FILE);
    for (int i = 0; i < enrollments.size(); i++) {
        if (enrollments[i][1] == roll && enrollments[i][2] == courseCode
            && enrollments[i][3] == semester) {
            if (enrollments[i][5] == "active" || enrollments[i][5] == "enrolled") {
                return ENROLL_ALREADY_ENROLLED;
            }
        }
    }

    int credits = stoi(course[2]);
    if (getCreditLoad(roll, semester) + credits > 21) {
        return ENROLL_CREDIT_OVERLOAD;
    }

    if (!checkPrerequisite(roll, courseCode)) {
        return ENROLL_PREREQ_FAIL;
    }

    // generate enrollment id
    int nextId = enrollments.size() + 1;
    string eid = "E";
    if (nextId < 1000) eid = eid + "0";
    if (nextId < 100) eid = eid + "0";
    if (nextId < 10) eid = eid + "0";
    eid = eid + to_string(nextId);

    vector<string> row;
    row.push_back(eid);
    row.push_back(roll);
    row.push_back(courseCode);
    row.push_back(semester);
    row.push_back(date);
    row.push_back("active");
    appendTXT(ENROLLMENTS_FILE, row);

    return ENROLL_OK;
}

bool dropCourse(string roll, string courseCode, string semester) {
    // check no attendance exists
    vector<vector<string>> attendance = readTXT("Data/attendance_log.txt");
    for (int i = 0; i < attendance.size(); i++) {
        if (attendance[i][1] == roll && attendance[i][2] == courseCode) {
            cout << "Cannot drop: attendance records exist.\n";
            return false;
        }
    }

    vector<vector<string>> enrollments = readTXT(ENROLLMENTS_FILE);
    bool found = false;
    for (int i = 0; i < enrollments.size(); i++) {
        if (enrollments[i][1] == roll && enrollments[i][2] == courseCode
            && enrollments[i][3] == semester) {
            enrollments[i][5] = "dropped";
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "Enrollment not found.\n";
        return false;
    }
    writeTXT(ENROLLMENTS_FILE, ENROLLMENTS_HEADER, enrollments);
    cout << "Course dropped.\n";
    return true;
}

vector<vector<string>> listEnrolledStudents(string courseCode) {
    vector<vector<string>> enrollments = readTXT(ENROLLMENTS_FILE);
    vector<vector<string>> result;

    for (int i = 0; i < enrollments.size(); i++) {
        if (enrollments[i][2] == courseCode) {
            if (enrollments[i][5] == "active" || enrollments[i][5] == "enrolled") {
                vector<string> student = searchByRoll(enrollments[i][1]);
                if (student.size() > 0 && student[5] == "active") {
                    result.push_back(student);
                }
            }
        }
    }
    return result;
}
