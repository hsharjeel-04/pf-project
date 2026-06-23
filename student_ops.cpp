#include "student_ops.h"
#include "filehandler.h"
#include <iostream>

bool isValidRoll(string roll) {
    // format: BSAI-YY-XXX
    if (roll.length() != 11) return false;
    if (roll.substr(0, 5) != "BSAI-") return false;
    if (roll[5] < '0' || roll[5] > '9') return false;
    if (roll[6] < '0' || roll[6] > '9') return false;
    if (roll[7] != '-') return false;
    if (roll[8] < '0' || roll[8] > '9') return false;
    if (roll[9] < '0' || roll[9] > '9') return false;
    if (roll[10] < '0' || roll[10] > '9') return false;
    return true;
}

bool nameHasDigits(string name) {
    for (int i = 0; i < name.length(); i++) {
        if (name[i] >= '0' && name[i] <= '9') return true;
    }
    return false;
}

bool addStudent(string roll, string name, string dept, string semester, string cgpa) {
    if (!isValidRoll(roll)) {
        cout << "Invalid roll format. Use BSAI-YY-XXX\n";
        return false;
    }
    if (rowExists(STUDENTS_FILE, 0, roll)) {
        cout << "Student already exists.\n";
        return false;
    }
    if (nameHasDigits(name)) {
        cout << "Name cannot contain digits.\n";
        return false;
    }
    double cgpaVal = 0;
    try {
        cgpaVal = stod(cgpa);
    } catch (...) {
        cout << "Invalid CGPA.\n";
        return false;
    }
    if (cgpaVal < 0.0 || cgpaVal > 4.0) {
        cout << "CGPA must be between 0.0 and 4.0.\n";
        return false;
    }

    vector<string> row;
    row.push_back(roll);
    row.push_back(name);
    row.push_back(dept);
    row.push_back(semester);
    row.push_back(cgpa);
    row.push_back("active");
    appendTXT(STUDENTS_FILE, row);
    cout << "Student added successfully.\n";
    return true;
}

vector<string> searchByRoll(string roll) {
    return findRow(STUDENTS_FILE, 0, roll);
}

vector<vector<string>> searchByName(string namePart) {
    vector<vector<string>> all = readTXT(STUDENTS_FILE);
    vector<vector<string>> result;
    for (int i = 0; i < all.size(); i++) {
        if (all[i].size() > 1 && all[i][1].find(namePart) != string::npos) {
            result.push_back(all[i]);
        }
    }
    return result;
}

bool updateStudent(string roll, string field, string newValue) {
    vector<vector<string>> rows = readTXT(STUDENTS_FILE);
    bool found = false;

    int col = -1;
    if (field == "name") col = 1;
    else if (field == "department") col = 2;
    else if (field == "semester") col = 3;
    else if (field == "cgpa") col = 4;
    else if (field == "status") col = 5;
    else {
        cout << "Cannot update that field.\n";
        return false;
    }

    if (field == "name" && nameHasDigits(newValue)) {
        cout << "Name cannot contain digits.\n";
        return false;
    }
    if (field == "cgpa") {
        double cgpaVal = stod(newValue);
        if (cgpaVal < 0.0 || cgpaVal > 4.0) {
            cout << "CGPA must be between 0.0 and 4.0.\n";
            return false;
        }
    }

    for (int i = 0; i < rows.size(); i++) {
        if (rows[i][0] == roll) {
            rows[i][col] = newValue;
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "Student not found.\n";
        return false;
    }
    writeTXT(STUDENTS_FILE, STUDENTS_HEADER, rows);
    cout << "Student updated.\n";
    return true;
}

bool softDelete(string roll) {
    return updateStudent(roll, "status", "inactive");
}

vector<vector<string>> listActiveStudents() {
    vector<vector<string>> all = readTXT(STUDENTS_FILE);
    vector<vector<string>> active;

    for (int i = 0; i < all.size(); i++) {
        if (all[i].size() > 5 && all[i][5] == "active") {
            active.push_back(all[i]);
        }
    }

    // selection sort by roll number
    for (int i = 0; i < active.size() - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < active.size(); j++) {
            if (active[j][0] < active[minIdx][0]) {
                minIdx = j;
            }
        }
        if (minIdx != i) {
            vector<string> temp = active[i];
            active[i] = active[minIdx];
            active[minIdx] = temp;
        }
    }
    return active;
}

void searchAsYouType() {
    string query = "";
    cout << "Search as you type (empty line to exit)\n";

    while (true) {
        cout << "\nQuery: \"" << query << "\"\n";
        vector<vector<string>> matches;

        // prefix match on name
        vector<vector<string>> all = readTXT(STUDENTS_FILE);
        for (int i = 0; i < all.size(); i++) {
            if (all[i].size() > 1 && all[i][5] == "active") {
                string name = all[i][1];
                if (query.length() <= name.length()) {
                    if (name.substr(0, query.length()) == query) {
                        matches.push_back(all[i]);
                    }
                }
            }
        }

        cout << "Matches (" << matches.size() << "):\n";
        for (int i = 0; i < matches.size(); i++) {
            cout << matches[i][0] << " | " << matches[i][1] << endl;
        }

        cout << "Type one char (# = backspace, Enter = done): ";
        string input;
        getline(cin, input);
        if (input == "") break;
        if (input == "#") {
            if (query.length() > 0) {
                query = query.substr(0, query.length() - 1);
            }
        } else {
            query = query + input[0];
        }
    }
}
