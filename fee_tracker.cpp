#include "fee_tracker.h"
#include "filehandler.h"
#include "student_ops.h"
#include <iostream>
#include <iomanip>

// parse DD-MM-YYYY to day number (simple approach from year 0)
int parseDay(string date) {
    int day = stoi(date.substr(0, 2));
    int month = stoi(date.substr(3, 2));
    int year = stoi(date.substr(6, 4));

    int daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int total = year * 365;

    for (int y = 1; y < year; y++) {
        if (y % 4 == 0) total = total + 1;
    }
    for (int m = 1; m < month; m++) {
        total = total + daysInMonth[m - 1];
        if (m == 2 && year % 4 == 0) total = total + 1;
    }
    total = total + day;
    return total;
}

bool isValidDate(string date) {
    if (date.length() != 10) return false;
    if (date[2] != '-' || date[5] != '-') return false;
    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) continue;
        if (date[i] < '0' || date[i] > '9') return false;
    }
    return true;
}

int daysBetween(string date1, string date2) {
    return parseDay(date2) - parseDay(date1);
}

vector<string> findFeeRow(string roll, string semester) {
    vector<vector<string>> fees = readTXT(FEES_FILE);
    for (int i = 0; i < fees.size(); i++) {
        if (fees[i][1] == roll && fees[i][2] == semester) {
            return fees[i];
        }
    }
    vector<string> empty;
    return empty;
}

bool recordPayment(string roll, string semester, double amount, string payDate) {
    if (!isValidDate(payDate)) {
        cout << "Invalid date format. Use DD-MM-YYYY.\n";
        return false;
    }

    vector<vector<string>> fees = readTXT(FEES_FILE);
    bool found = false;
    for (int i = 0; i < fees.size(); i++) {
        if (fees[i][1] == roll && fees[i][2] == semester) {
            double paid = stod(fees[i][4]) + amount;
            double total = stod(fees[i][3]);
            fees[i][4] = to_string((int)paid);
            fees[i][6] = payDate;
            if (paid >= total) fees[i][8] = "paid";
            else fees[i][8] = "partial";
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "Fee record not found.\n";
        return false;
    }
    writeTXT(FEES_FILE, FEES_HEADER, fees);
    cout << "Payment recorded.\n";
    return true;
}

double computeLateFine(string roll, string semester) {
    vector<string> fee = findFeeRow(roll, semester);
    if (fee.size() == 0) return 0;

    string dueDate = fee[5];
    string paidDate = fee[6];
    if (paidDate == "00-00-0000" || paidDate == "") return 0;

    int days = daysBetween(dueDate, paidDate);
    if (days <= 0) return 0;

    int weeks = days / 7;
    double totalFee = stod(fee[3]);
    return totalFee * 0.02 * weeks;
}

void generateReceipt(string roll, string semester) {
    vector<string> fee = findFeeRow(roll, semester);
    vector<string> student = searchByRoll(roll);
    if (fee.size() == 0) {
        cout << "No fee record found.\n";
        return;
    }

    double totalFee = stod(fee[3]);
    double paid = stod(fee[4]);
    double fine = computeLateFine(roll, semester);
    double balance = totalFee + fine - paid;

    cout << fixed << setprecision(2);
    cout << "\n========== FEE RECEIPT ==========\n";
    if (student.size() > 0) {
        cout << "Student: " << student[1] << " (" << roll << ")\n";
    }
    cout << "Semester: " << semester << endl;
    cout << setw(25) << left << "Tuition Fee:" << totalFee << endl;
    cout << setw(25) << left << "Late Fine:" << fine << endl;
    cout << setw(25) << left << "Total Due:" << (totalFee + fine) << endl;
    cout << setw(25) << left << "Amount Paid:" << paid << endl;
    cout << setw(25) << left << "Balance:" << balance << endl;
    cout << "Due Date: " << fee[5] << " | Paid: " << fee[6] << endl;
    cout << "=================================\n";
}

vector<vector<string>> getDefaulters() {
    vector<vector<string>> fees = readTXT(FEES_FILE);
    vector<vector<string>> defaulters;

    for (int i = 0; i < fees.size(); i++) {
        double total = stod(fees[i][3]);
        double paid = stod(fees[i][4]);
        double balance = total - paid;
        if (balance > 0) {
            vector<string> row = fees[i];
            row.push_back(to_string(balance));
            defaulters.push_back(row);
        }
    }

    // bubble sort by outstanding amount descending
    for (int i = 0; i < defaulters.size() - 1; i++) {
        for (int j = 0; j < defaulters.size() - i - 1; j++) {
            double b1 = stod(defaulters[j][defaulters[j].size() - 1]);
            double b2 = stod(defaulters[j + 1][defaulters[j + 1].size() - 1]);
            if (b1 < b2) {
                vector<string> temp = defaulters[j];
                defaulters[j] = defaulters[j + 1];
                defaulters[j + 1] = temp;
            }
        }
    }
    return defaulters;
}
