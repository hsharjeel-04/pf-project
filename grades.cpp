#include "grades.h"
#include "filehandler.h"
#include "attendance.h"
#include "course_ops.h"
#include <iostream>

double bestThreeOfFive(double marks[], int n) {
    if (n == 0) return 0;
    if (n <= 3) {
        double sum = 0;
        for (int i = 0; i < n; i++) sum = sum + marks[i];
        return sum / n;
    }

    // find two lowest and exclude them
    double lowest1 = marks[0];
    double lowest2 = marks[0];
    int idx1 = 0, idx2 = 0;

    for (int i = 1; i < n; i++) {
        if (marks[i] < lowest1) {
            lowest2 = lowest1;
            idx2 = idx1;
            lowest1 = marks[i];
            idx1 = i;
        } else if (marks[i] < lowest2 && i != idx1) {
            lowest2 = marks[i];
            idx2 = i;
        }
    }

    double sum = 0;
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (i != idx1 && i != idx2) {
            sum = sum + marks[i];
            count++;
        }
    }
    if (count == 0) return 0;
    return sum / count;
}

double computeWeightedTotal(double quiz, double asgn, double mid, double finalMark) {
    return quiz * 0.10 + asgn * 0.10 + mid * 0.30 + finalMark * 0.50;
}

string getLetterGrade(double total) {
    if (total >= 85) return "A";
    if (total >= 80) return "B+";
    if (total >= 70) return "B";
    if (total >= 65) return "C+";
    if (total >= 60) return "C";
    if (total >= 50) return "D";
    return "F";
}

double gradeToPoint(string grade) {
    if (grade == "A") return 4.0;
    if (grade == "B+") return 3.5;
    if (grade == "B") return 3.0;
    if (grade == "C+") return 2.5;
    if (grade == "C") return 2.0;
    if (grade == "D") return 1.0;
    return 0.0;
}

bool enterMarks(string roll, string courseCode, string semester) {
    double quizzes[5];
    int quizCount = 0;
    cout << "Enter up to 5 quiz marks (-1 to stop): ";
    for (int i = 0; i < 5; i++) {
        double m;
        cin >> m;
        if (m < 0) break;
        if (m < 0 || m > 10) {
            cout << "Quiz marks must be 0-10.\n";
            return false;
        }
        quizzes[quizCount] = m;
        quizCount++;
    }

    double asgn[3];
    cout << "Enter 3 assignment marks (0-10 each): ";
    for (int i = 0; i < 3; i++) {
        cin >> asgn[i];
        if (asgn[i] < 0 || asgn[i] > 10) {
            cout << "Assignment marks must be 0-10.\n";
            return false;
        }
    }
    double asgnAvg = (asgn[0] + asgn[1] + asgn[2]) / 3.0;

    double mid, finalMark;
    cout << "Enter mid marks (0-40): ";
    cin >> mid;
    cout << "Enter final marks (0-60): ";
    cin >> finalMark;
    if (mid < 0 || mid > 40 || finalMark < 0 || finalMark > 60) {
        cout << "Invalid mid/final range.\n";
        return false;
    }

    double quizAvg = bestThreeOfFive(quizzes, quizCount);
    // scale quiz and asgn to 100 scale for weighting
    double quizScaled = quizAvg * 10;
    double asgnScaled = asgnAvg * 10;
    double midScaled = mid;
    double finalScaled = finalMark;

    double total = computeWeightedTotal(quizScaled, asgnScaled, midScaled, finalScaled);
    string grade = getLetterGrade(total);
    grade = applyAttendancePenalty(roll, courseCode, grade);

    // save to grades file
    vector<vector<string>> grades = readTXT(GRADES_FILE);
    bool updated = false;
    for (int i = 0; i < grades.size(); i++) {
        if (grades[i][0] == roll && grades[i][1] == courseCode && grades[i][2] == semester) {
            grades[i][3] = to_string(total);
            grades[i][4] = grade;
            updated = true;
            break;
        }
    }
    if (!updated) {
        vector<string> row;
        row.push_back(roll);
        row.push_back(courseCode);
        row.push_back(semester);
        row.push_back(to_string(total));
        row.push_back(grade);
        grades.push_back(row);
    }
    writeTXT(GRADES_FILE, GRADES_HEADER, grades);
    cout << "Marks saved. Total: " << total << " Grade: " << grade << endl;
    return true;
}

double computeGPA(string roll, string semester) {
    vector<vector<string>> grades = readTXT(GRADES_FILE);
    vector<vector<string>> courses = readTXT(COURSES_FILE);
    double totalPoints = 0;
    int totalCredits = 0;

    for (int i = 0; i < grades.size(); i++) {
        if (grades[i][0] == roll && grades[i][2] == semester) {
            string code = grades[i][1];
            string grade = grades[i][4];
            for (int j = 0; j < courses.size(); j++) {
                if (courses[j][0] == code) {
                    int credits = stoi(courses[j][2]);
                    totalPoints = totalPoints + gradeToPoint(grade) * credits;
                    totalCredits = totalCredits + credits;
                    break;
                }
            }
        }
    }
    if (totalCredits == 0) return 0.0;
    return totalPoints / totalCredits;
}

Stats computeClassStats(string courseCode, string semester) {
    Stats s;
    s.highest = 0;
    s.lowest = 100;
    s.mean = 0;
    s.median = 0;

    vector<double> totals;
    vector<vector<string>> grades = readTXT(GRADES_FILE);
    for (int i = 0; i < grades.size(); i++) {
        if (grades[i][1] == courseCode && grades[i][2] == semester) {
            double t = stod(grades[i][3]);
            totals.push_back(t);
        }
    }
    if (totals.size() == 0) return s;

    double sum = 0;
    for (int i = 0; i < totals.size(); i++) {
        if (totals[i] > s.highest) s.highest = totals[i];
        if (totals[i] < s.lowest) s.lowest = totals[i];
        sum = sum + totals[i];
    }
    s.mean = sum / totals.size();

    // bubble sort for median
    for (int i = 0; i < totals.size() - 1; i++) {
        for (int j = 0; j < totals.size() - i - 1; j++) {
            if (totals[j] > totals[j + 1]) {
                double temp = totals[j];
                totals[j] = totals[j + 1];
                totals[j + 1] = temp;
            }
        }
    }
    int mid = totals.size() / 2;
    if (totals.size() % 2 == 0) {
        s.median = (totals[mid - 1] + totals[mid]) / 2.0;
    } else {
        s.median = totals[mid];
    }
    return s;
}

string applyAttendancePenalty(string roll, string courseCode, string grade) {
    double pct = getAttendancePct(roll, courseCode);
    if (pct < 75.0) return "F";
    return grade;
}
