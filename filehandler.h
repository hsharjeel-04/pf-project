#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <string>
#include <vector>

using namespace std;

// reads all rows from a txt file (skips header)
vector<vector<string>> readTXT(string filename);

// overwrites file with header + all rows
void writeTXT(string filename, string header, vector<vector<string>> rows);

// appends one row to file
void appendTXT(string filename, vector<string> row);

// finds first row where colIndex matches value
vector<string> findRow(string filename, int colIndex, string value);

// checks if any row has value at colIndex
bool rowExists(string filename, int colIndex, string value);

#endif
