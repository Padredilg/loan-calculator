/*
Author: Luiz Padredi
Date: 02/15/2023
Assignment 2: Loan Calculator
Repository: https://github.com/Padredilg/loan-calculator

source for learning how to organize table
https://stackoverflow.com/questions/14765155/how-can-i-easily-format-my-data-table-in-c

source for checking string input
https://stackoverflow.com/questions/6605282/how-can-i-check-if-a-string-has-special-characters-in-c-effectively
*/

#include <iostream>
#include <cmath>
#include <sstream>
#include <fstream>

using namespace std;

void start();
void askForNewLoan();
void getUserInput(double& P, double& r, int& n, double& annualRatePercent);
double calculateAmortization(double P, double r, int n);
void outputTable(double P, double r, int n, double M, double annualRatePercent);
bool askIfWritingToFile();
void promptForFileName(string& fileName);
void calculateStartOfMonth(double& interest, double beginningBalance, double rate, double& principal, double amortization, double& endingBalance);
void trackPayments(double interest, double& totalInterestPaid, double principal, double& totalAmountPaid);
string writeSummary(double M, int nOfMonths, double totalAmountPaid, double totalInterestPaid);
string writeRow(int i, double beginningBalance, double interest, double principal, double endingBalance);
string writeLabel(double P, double r, int n, string fileName, double annualRatePercent);

string columnLabel(const string s);
string rowDoubleValue(const double x);
string rowIntValue(const int x);
string writeEndYear(int year);

const int TABLE_WIDTH = 100;
const int CELL_WIDTH = 17;

int main()
{
    cout << endl
         << "Greetings User! I can help you plan for your loan."
         << endl << endl;

    start();

    cout << endl
         << "Thank you for your time!"
         << endl;

    return 0;
}

//starts loan table creation flow
void start(){
    double P; //the principal loan amount
    double r; //monthly interest rate
    int n; //n = number of payments in months
    double annualRatePercent; // annual rate percentage, such as 10%/year

    getUserInput(P, r, n, annualRatePercent);

    double M = calculateAmortization(P, r, n);

    outputTable(P, r, n, M, annualRatePercent);

    askForNewLoan();
}

//prompts for new loan
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
void getUserInput(double& P, double& r, int& n, double& annualRatePercent){
    cout << "================================================================================"
         << endl << endl
         << "Planning for your Loan."
         << endl << endl
         << "================================================================================"
         << endl << endl;

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
         << "( If your Annual Interest Rate is 10%, enter 10 )" << endl
         << " Annual Interest Rate Percentage: ";
    cin >> annualRatePercent;
    while (cin.fail() || r <= 0){
        cin.clear(); // clear input buffer to restore cin to a usable state
        cin.ignore(INT_MAX, '\n'); // ignore last input
        cout << "That is not a valid entry." << endl
             << "Your entry should be a number greater than 0." << endl
             << "Try again." << endl
             << endl
             << "( If your Annual Interest Rate is 10%, enter 10 )" << endl
             << " Annual Interest Rate Percentage: ";
        cin >> annualRatePercent;
    }
    r = (annualRatePercent/100)/12;


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
void outputTable(double P, double r, int n, double M, double annualRatePercent){
    ofstream out_stream;
    bool isWritingToFile = askIfWritingToFile();
    string fileName = "Loan";

    if(isWritingToFile){
        promptForFileName(fileName);
        out_stream.open(fileName + ".txt");

        if (out_stream.fail( ))
        {
            cout << "Output file opening failed.\n";
            exit(1);
        }
    }


    string label = writeLabel(P, r, n, fileName, annualRatePercent);
    cout << label;
    if(isWritingToFile){
        out_stream << label;
    }

    double beginningBalance = P;
    double interest;
    double principal;
    double endingBalance = beginningBalance;
    double totalInterestPaid = 0;
    double totalAmountPaid = 0;

    for(int i=1; i <= n; i++){
        calculateStartOfMonth(interest, beginningBalance, r, principal, M, endingBalance);

        string row = writeRow(i, beginningBalance, interest, principal, endingBalance);
        cout << row;
        if(isWritingToFile){
            out_stream << row;
        }

        if(i%12 == 0){
            string endYear = writeEndYear(i/12);
            cout << endYear;
            if(isWritingToFile){
                out_stream << endYear;
            }
        }

        trackPayments(interest, totalInterestPaid, principal, totalAmountPaid);
        beginningBalance = endingBalance;
    }

    string summary = writeSummary(M, n, totalAmountPaid, totalInterestPaid);
    cout << summary;
    if(isWritingToFile){
        out_stream << summary;
        cout << endl << endl
             << "The table above has been written to " << fileName << ".txt" << endl;
    }

    out_stream.close( );
}

//collect file name input from user
void promptForFileName(string& fileName){
    cout << endl
         << "Please choose a name for your file.\nDo not include the file extension." << endl
         << "file name: ";
    cin >> fileName;

    while (
            cin.fail()
            || ( fileName.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_")!= std::string::npos )
    ){
        cin.clear(); // clear input buffer to restore cin to a usable state
        cin.ignore(INT_MAX, '\n'); // ignore last input
        cout << endl
             << "That is not a valid entry."
             << endl << endl
             << "Your file name can only contain lowercase letters, uppercase letters, numbers, and underscores." << endl
             << "Please choose a name for your file.\nDo not include the file extension." << endl
             << "File name: ";
        cin >> fileName;
    }
}

//checks if user wishes to write table to a file
bool askIfWritingToFile(){
    int choice;
    cout << endl
         << "Would you like your loan table to be printed into a text file as well as the terminal screen?" << endl
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
             << "1 - Write table to text file." << endl
             << "2 - Do not write table to text file." << endl
             << endl << endl;

        cin >> choice;
    }

    switch (choice){
        case 1:
            return true;
        default:
            return false;
    }
}

//returns string that makes up initial row of table containing labels
string writeLabel(double P, double r, int n, string fileName, double annualRatePercent){

    stringstream nOfMonths;
    nOfMonths << n;

    stringstream loanAmount;
    loanAmount << fixed;
    loanAmount.precision(2);
    loanAmount << P;

    stringstream rate;
    rate << r;

    stringstream annualRate;
    annualRate << ceil(100.0 * annualRatePercent)/100;

    string label =   "\nName: " + fileName
                     + "\nRequested Loan: $" + loanAmount.str()
                     + "\nAnnual Interest Rate Percentage: " + annualRate.str() + "%"
                     + "\nMonthly Interest Rate: " + rate.str()
                     + "\nNumber of Payments: " + nOfMonths.str()
                     + "\n\n"
                     + columnLabel(" ") + " | "
                     + columnLabel("Beginning Balance") + " | "
                     + columnLabel("Interest") + " | "
                     + columnLabel("Principal") + " | "
                     + columnLabel("Ending Balance") + "\n"
                     + string(TABLE_WIDTH, '-') + "\n";

    return label;
}

//returns string that makes up one row of the table
string writeRow(int i, double beginningBalance, double interest, double principal, double endingBalance){
    return  rowIntValue(i) + " | "
            + rowDoubleValue(beginningBalance) + " | "
            + rowDoubleValue(interest) + " | "
            + rowDoubleValue(principal) + " | "
            + rowDoubleValue(endingBalance) + "\n";

}

//returns string with payment summary information
string writeSummary(double M, int n, double totalAmountPaid, double totalInterestPaid){
    stringstream amortization;
    amortization << fixed;
    amortization.precision(2);
    amortization << M;

    stringstream nOfMonths;
    nOfMonths << fixed;
    nOfMonths.precision(2);
    nOfMonths << n;

    stringstream totalAmount;
    totalAmount << fixed;
    totalAmount.precision(2);
    totalAmount << totalAmountPaid;

    stringstream totalInterest;
    totalInterest << fixed;
    totalInterest.precision(2);
    totalInterest << totalInterestPaid;


    return "\n\nPayment Every Month: $" + amortization.str()
         + "\nTotal of " + nOfMonths.str() + " Payments: $" + totalAmount.str()
         + "\nTotal Interest paid: $" + totalInterest.str() + "\n\n";
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
string writeEndYear(int year) {
    stringstream ss, spaces;
    int padding = TABLE_WIDTH - 9;                 // count excess room to pad
    for(int i=0; i<padding/2; ++i)
        spaces << " ";
    ss << spaces.str() << "END YEAR " << year << spaces.str();    // format with padding
    if(padding>0 && padding%2!=0)               // if odd #, add 1 space
        ss << " ";
    string endYearLine = ss.str();

    return string(TABLE_WIDTH, '-') + "\n"
           + endYearLine  + "\n"
           + string(TABLE_WIDTH, '-') + "\n";
}
