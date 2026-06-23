#ifndef FEE_TRACKER_H
#define FEE_TRACKER_H

#include <string>
#include <vector>

using namespace std;

const string FEES_FILE = "Data/fees.txt";
const string FEES_HEADER = "fee_id,roll_no,semester,total_fee,amount_paid,due_date,payment_date,payment_method,status";

bool recordPayment(string roll, string semester, double amount, string payDate);
double computeLateFine(string roll, string semester);
int daysBetween(string date1, string date2);
void generateReceipt(string roll, string semester);
vector<vector<string>> getDefaulters();

#endif
