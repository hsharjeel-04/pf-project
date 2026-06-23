#ifndef GRADES_H
#define GRADES_H

#include <string>
#include <vector>

using namespace std;

struct Stats {
    double highest;
    double lowest;
    double mean;
    double median;
};

bool enterMarks(string roll, string courseCode, string semester);
double bestThreeOfFive(double marks[], int n);
double computeWeightedTotal(double quiz, double asgn, double mid, double finalMark);
string getLetterGrade(double total);
double computeGPA(string roll, string semester);
Stats computeClassStats(string courseCode, string semester);
string applyAttendancePenalty(string roll, string courseCode, string grade);

#endif
