/*
Author: Luiz Padredi
Date: 02/11/2023
Assignment 2: Loan Calculator
Repository: https://github.com/Padredilg/loan-calculator

source organizing table
https://stackoverflow.com/questions/14765155/how-can-i-easily-format-my-data-table-in-c
*/

#include <iostream>
#include <cmath>
#include <sstream>

using namespace std;

void start();
void askForNewLoan();
void getUserInput(double& P, double& r, int& n);
double calculateAmortization(double P, double r, int n);
void outputTable(double P, double r, int n, double M);
void calculateStartOfMonth(double& interest, double beginningBalance, double rate, double& principal, double amortization, double& endingBalance);
void trackPayments(double interest, double& totalInterestPaid, double principal, double& totalAmountPaid);
void printSummary(double M, int nOfMonths, double totalAmountPaid, double totalInterestPaid);
string writeRow(int i, double beginningBalance, double interest, double principal, double endingBalance);
string writeLabel(string label1, string label2, string label3, string label4, string label5);

string columnLabel(const string s);
string rowDoubleValue(const double x);
string rowIntValue(const int x);
string printYearEnd(string s, int month);

const int TABLE_WIDTH = 100;
const int CELL_WIDTH = 17;
/*

Functions that are needed to run the program:

1. (DONE)- An input function which prompts and fills the input with values for P, r and n.

2. (DONE)- A function that calculates and returns amortization M

3. (IN PROCESS) An output function which displays the table to the screen and writes it to a file.

4. (DONE) A function that calculates the interest and balance for each month.

5. (DONE) A function that keeps track of total payment and total interest accumulated.
   This can be a call-by-reference function.
*/

/*

Your program should output to a file
as well display on the screen
a table that shows the interest
and principal portion of the loan for every month
as well as the total interest paid up to that month and the remaining balance.

Finally
the program outputs the total interest paid over the life of the loan like follows:
Payment Every Month $922.90
Total of 24 Payments $22,149.56
Total Interest $2,149.56

6. (START) Your program should allow the user to repeat this calculation as often as desired

*/

/*

End purpose in my own words:

display, in a table like format

Balance left,
interest for current month off,
principal off of M - interest,
balance after payment

*/

int main()
{
    cout << endl
         << "Greetings User! I can help you plan for your loan."
         << endl << endl;

    start();

    cout << endl
         << "Thank you for your time!"
         << endl << endl;

    return 0;
}

void start(){
    double P; //the principal loan amount
    double r; //monthly interest rate
    int n; //n = number of payments in months

    getUserInput(P, r, n);

    double M = calculateAmortization(P, r, n);

    outputTable(P, r, n, M);

    askForNewLoan();
}

void askForNewLoan(){
    int choice;
    cout << endl
         << "Would you like to calculate a new Loan?" << endl
         << "Please select the number corresponding to your choice."
         << endl << endl
         << "1 - Yes" << endl
         << "2 - No"
         << endl << endl;
    cin >> choice;

    while (cin.fail() || choice < 1 || choice > 2){
        cin.clear(); // clear input buffer to restore cin to a usable state
        cin.ignore(INT_MAX, '\n'); // ignore last input
        cout << "That is not a valid entry."
             << endl << endl
             << "Please select the number corresponding to the service you are looking for."
             << endl << endl
             << "1 - Calculate new Loan." << endl
             << "2 - Exit." << endl
             << endl << endl;

        cin >> choice;
    }

    switch (choice){
        case 1:
            start();
            break;
        default:
            return;
    }

}

// Collects user input for Principal Amount, Monthly Rate, and Number of Payments in months
void getUserInput(double& P, double& r, int& n){
    ///Collect Principal Amount - P
    cout << "Please provide the following information: "<< endl
         << endl
         << "( The value you seek to borrow )" << endl
         << " Loan Principal Amount: $";
    cin >> P;
    while (cin.fail() || P <= 0){
        cin.clear(); // clear input buffer to restore cin to a usable state
        cin.ignore(INT_MAX, '\n'); // ignore last input
        cout << "That is not a valid entry." << endl
             << "Your entry should be a number greater than 0." << endl
             << "Try again." << endl
             << endl
             << "( The value you seek to borrow )" << endl
             << " Loan Principal Amount: $";
        cin >> P;
    }


    ///Collect Annual Interest Rate - r
    cout << endl
         << "( Annual Interest Rate divided by 12 --> for 10% annual interest, that would be 0.1/12 ~= 0.0083333 )" << endl
         << " Monthly Interest Rate: ";
    cin >> r;
    while (cin.fail() || r <= 0){
        cin.clear(); // clear input buffer to restore cin to a usable state
        cin.ignore(INT_MAX, '\n'); // ignore last input
        cout << "That is not a valid entry." << endl
             << "Your entry should be a number greater than 0." << endl
             << "Try again." << endl
             << endl
             << "( Annual Interest Rate divided by 12 --> for 10% annual interest, that would be 0.1/12 ~= 0.0083333 )" << endl
             << " Monthly Interest Rate: ";;
        cin >> r;
    };


    ///Number of months to finish paying for Loan
    cout << endl
         << "( Number of Months under which you wish to finish paying your loan )" << endl
         << " Number of Months to Pay: ";
    cin >> n;
    while (cin.fail() || n <= 0){
        cin.clear(); // clear input buffer to restore cin to a usable state
        cin.ignore(INT_MAX, '\n'); // ignore last input
        cout << "That is not a valid entry." << endl
             << "Your entry should be a number greater than 0." << endl
             << "Try again." << endl
             << endl
             << "( Number of Months under which you wish to finish paying your loan )" << endl
             << " Number of Months to Pay: ";

        cin >> n;
    }
}

//Calculates the Amortization value
double calculateAmortization(double P, double r, int n){
    double w = pow( (1+r) , n );
    double M = P * r * w / ( w - 1 );
    return M;
}

//displays info in table format
void outputTable(double P, double r, int n, double M){
    string label = writeLabel(" ", "Beginning Balance", "Interest", "Principal", "Ending Balance");

    cout << label;
    //FIXME --> add label to file

    double beginningBalance = P;
    double interest;
    double principal;
    double endingBalance = beginningBalance;
    double totalInterestPaid = 0;
    double totalAmountPaid = 0;

    for(int i=1; i <= n; i++){
        calculateStartOfMonth(interest, beginningBalance, r, principal, M, endingBalance);

        //MAYBE GOOD WAY TO TRANSLATE TO FILE WOULD BE
        //TO HAVE TEXT BE RETURNED FROM A FUNCTION THAT BUILDS A STRING,
        //THEN IN HERE WE CALL COUT WITH THE STRING,
        //AND PRINT TO FILE WITH SAME STRING

        string row = writeRow(i, beginningBalance, interest, principal, endingBalance);

        cout << row;
        //FIXME --> add row to file

        if(i%12 == 0){
            cout << string(TABLE_WIDTH, '-') << endl
                 << printYearEnd("END YEAR", i/12)  <<endl
                 << string(TABLE_WIDTH, '-') << endl;
        }

        trackPayments(interest, totalInterestPaid, principal, totalAmountPaid);
        beginningBalance = endingBalance;
    }

    printSummary(M, n, totalAmountPaid, totalInterestPaid);
}

//returns string that makes up initial row of table containing labels
string writeLabel(string label1, string label2, string label3, string label4, string label5){
    string label = "\n\n" + columnLabel(label1) + " | "
                     + columnLabel(label2) + " | "
                     + columnLabel(label3) + " | "
                     + columnLabel(label4) + " | "
                     + columnLabel(label5) + "\n"
                     + string(TABLE_WIDTH, '-') + "\n";

    return label;
}

//returns string that makes up one row of the table
string writeRow(int i, double beginningBalance, double interest, double principal, double endingBalance){
    return rowIntValue(i) + " | "
            + rowDoubleValue(beginningBalance) + " | "
            + rowDoubleValue(interest) + " | "
            + rowDoubleValue(principal) + " | "
            + rowDoubleValue(endingBalance) + "\n";

}

void printSummary(double M, int nOfMonths, double totalAmountPaid, double totalInterestPaid){
    cout << endl << endl
         << "Payment Every Month: $" << M << endl
         << "Total of " << nOfMonths << " Payments: $" << totalAmountPaid << endl
         << "Total Interest paid: $" << totalInterestPaid << endl
         << endl;

     //FIXME --> ADD SUMMARY TO FILE
}

//tracks the amount paid in interest, and in total
void trackPayments(double interest, double& totalInterestPaid, double principal, double& totalAmountPaid){
    totalInterestPaid += interest;
    totalAmountPaid += principal + interest;
}

// calculates and updates the interest rate for the given balance and monthly rate
void calculateStartOfMonth(double& interest, double beginningBalance, double rate, double& principal, double amortization, double& endingBalance){
    interest = beginningBalance * rate;
    principal = amortization - interest;
    endingBalance -= principal;
}



//Converts double to string and fits in row space of specified width
string rowDoubleValue(const double x) {
    stringstream ss;
    ss << fixed << right;
    ss.fill(' ');        // fill space around displayed #
    ss.width(CELL_WIDTH);     // set  width around displayed #
    ss.precision(2); // set # places after decimal
    ss << x;
    return "$" + ss.str();
}

//Converts int to string and fits in row space of specified width
string rowIntValue(const int x) {
    stringstream ss;
    ss << fixed << right;
    ss.fill(' ');        // fill space around displayed #
    ss.width(CELL_WIDTH - 2);     // set  width around displayed #
    ss << x;
    return ss.str();
}

//Center-aligns string within a field of width w.
string columnLabel(string s) {
    stringstream ss, spaces;
    int padding = CELL_WIDTH - s.size();                 // count excess room to pad
    for(int i=0; i<padding/2; ++i)
        spaces << " ";
    ss << spaces.str() << s << spaces.str();    // format with padding
    if(padding>0 && padding%2!=0)               // if odd #, add 1 space
        ss << " ";
    return ss.str();
}

//Prints a separation in the table signalizing end of a year
string printYearEnd(string s, int month) {
    stringstream ss, spaces;
    int padding = TABLE_WIDTH - s.size();                 // count excess room to pad
    for(int i=0; i<padding/2; ++i)
        spaces << " ";
    ss << spaces.str() << s << " " << month << spaces.str();    // format with padding
    if(padding>0 && padding%2!=0)               // if odd #, add 1 space
        ss << " ";
    return ss.str();
}
