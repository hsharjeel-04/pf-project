#ifndef COURSE_OPS_H
#define COURSE_OPS_H

#include <string>
#include <vector>

using namespace std;

enum EnrollResult {
    ENROLL_OK,
    ENROLL_STUDENT_INACTIVE,
    ENROLL_COURSE_NOT_FOUND,
    ENROLL_NO_SEATS,
    ENROLL_ALREADY_ENROLLED,
    ENROLL_CREDIT_OVERLOAD,
    ENROLL_PREREQ_FAIL
};

const string COURSES_FILE = "Data/courses.txt";
const string COURSES_HEADER = "course_code,course_name,credit_hours,instructor,capacity,enrolled,prerequisite";
const string ENROLLMENTS_FILE = "Data/enrollments.txt";
const string ENROLLMENTS_HEADER = "enrollment_id,roll_no,course_code,semester,enrollment_date,status";
const string GRADES_FILE = "Data/grades.txt";
const string GRADES_HEADER = "roll_no,course_code,semester,total,letter_grade";

EnrollResult enrollStudent(string roll, string courseCode, string semester, string date);
bool dropCourse(string roll, string courseCode, string semester);
int getCreditLoad(string roll, string semester);
bool checkPrerequisite(string roll, string courseCode);
vector<vector<string>> listEnrolledStudents(string courseCode);

string enrollResultMessage(EnrollResult result);

#endif
