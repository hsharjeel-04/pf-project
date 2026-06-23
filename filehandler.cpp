#include "filehandler.h"
#include <fstream>

// split one csv line into fields using character loop
vector<string> parseLine(string line) {
    vector<string> fields;
    string current = "";
    bool inQuotes = false;

    for (int i = 0; i < line.length(); i++) {
        char c = line[i];
        if (c == '"') {
            inQuotes = !inQuotes;
        } else if (c == ',' && !inQuotes) {
            fields.push_back(current);
            current = "";
        } else {
            current = current + c;
        }
    }
    // strip carriage return from last field (Windows line endings)
    if (current.length() > 0 && current[current.length() - 1] == '\r') {
        current = current.substr(0, current.length() - 1);
    }
    fields.push_back(current);
    return fields;
}

vector<vector<string>> readTXT(string filename) {
    vector<vector<string>> rows;
    ifstream file(filename);
    if (!file.is_open()) {
        return rows;
    }

    string line;
    getline(file, line); // skip header

    while (getline(file, line)) {
        if (line.length() > 0) {
            rows.push_back(parseLine(line));
        }
    }
    file.close();
    return rows;
}

void writeTXT(string filename, string header, vector<vector<string>> rows) {
    ofstream file(filename);
    file << header << endl;
    for (int i = 0; i < rows.size(); i++) {
        for (int j = 0; j < rows[i].size(); j++) {
            if (j > 0) file << ",";
            // wrap field in quotes if it has a comma
            if (rows[i][j].find(',') != string::npos) {
                file << "\"" << rows[i][j] << "\"";
            } else {
                file << rows[i][j];
            }
        }
        file << endl;
    }
    file.close();
}

void appendTXT(string filename, vector<string> row) {
    ofstream file(filename, ios::app);
    for (int i = 0; i < row.size(); i++) {
        if (i > 0) file << ",";
        if (row[i].find(',') != string::npos) {
            file << "\"" << row[i] << "\"";
        } else {
            file << row[i];
        }
    }
    file << endl;
    file.close();
}

vector<string> findRow(string filename, int colIndex, string value) {
    vector<vector<string>> rows = readTXT(filename);
    for (int i = 0; i < rows.size(); i++) {
        if (rows[i].size() > colIndex && rows[i][colIndex] == value) {
            return rows[i];
        }
    }
    vector<string> empty;
    return empty;
}

bool rowExists(string filename, int colIndex, string value) {
    vector<string> row = findRow(filename, colIndex, value);
    return row.size() > 0;
}
