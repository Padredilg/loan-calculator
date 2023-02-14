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

void getUserInput(double& P, double& r, int& n);
double calculateAmortization(double P, double r, int n);
void outputTable(double P, double r, int n, double M);

string columnLable(const string s);
string rowDoubleValue(const double x);
string rowIntValue(const int x);
string printYearEnd(string s, int month);

const int tableWidth = 100;
const int cellWidth = 17;
/*

Functions that are needed to run the program:

1. (DONE)- An input function which prompts and fills the input with values for P, r and n.

2. (DONE)- A function that calculates and returns amortization M

3. (IN PROCESS) An output function which displays the table to the screen and writes it to a file.

- A function that calculates the interest and balance for each month.
Why?

    This can be a call-by reference function.

- A function that keeps track of total payment and total interest accumulated.
Why?
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

Your program should allow the user to repeat this calculation as often as desired

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

    double P; //the principal loan amount
    double r; //monthly interest rate
    int n; //n = number of payments in months

    getUserInput(P, r, n);

    double M = calculateAmortization(P, r, n);

    outputTable(P, r, n, M);

    cout << P << " " << r << " " << n << " " << M;

    return 0;
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
    cout << columnLable(" ") << " | "
         << columnLable("Beginning Balance") << " | "
         << columnLable("Interest") << " | "
         << columnLable("Principal") << " | "
         << columnLable("Ending Balance") << endl
         << string(tableWidth, '-') << endl;

    double beginningBalance = P;
    double interest = 10;
    double principal = 10;
    double endingBalance = 0;


    for(int i=1; i <= n; i++){
        cout << rowIntValue(i) << " | "
             << rowDoubleValue(beginningBalance) << " | "
             << rowDoubleValue(interest) << " | "
             << rowDoubleValue(principal) << " | "
             << rowDoubleValue(endingBalance) << "\n";

        if(i%12 == 0){
            cout << string(tableWidth, '-') << endl
                 << printYearEnd("END YEAR", i/12)  <<endl
                 << string(tableWidth, '-') << endl;
        }

    }
}


//Converts double to string and fits in row space of specified width
string rowDoubleValue(const double x) {
    const int width = 17;

    stringstream ss;
    ss << fixed << right;
    ss.fill(' ');        // fill space around displayed #
    ss.width(cellWidth);     // set  width around displayed #
    ss.precision(2); // set # places after decimal
    ss << x;
    return ss.str();
}

//Converts int to string and fits in row space of specified width
string rowIntValue(const int x) {
    stringstream ss;
    ss << fixed << right;
    ss.fill(' ');        // fill space around displayed #
    ss.width(cellWidth);     // set  width around displayed #
    ss << x;
    return ss.str();
}

//Center-aligns string within a field of width w.
string columnLable(string s) {
    stringstream ss, spaces;
    int padding = cellWidth - s.size();                 // count excess room to pad
    for(int i=0; i<padding/2; ++i)
        spaces << " ";
    ss << spaces.str() << s << spaces.str();    // format with padding
    if(padding>0 && padding%2!=0)               // if odd #, add 1 space
        ss << " ";
    return ss.str();
}

string printYearEnd(string s, int month) {
    stringstream ss, spaces;
    int padding = tableWidth - s.size();                 // count excess room to pad
    for(int i=0; i<padding/2; ++i)
        spaces << " ";
    ss << spaces.str() << s << " " << month << spaces.str();    // format with padding
    if(padding>0 && padding%2!=0)               // if odd #, add 1 space
        ss << " ";
    return ss.str();
}
