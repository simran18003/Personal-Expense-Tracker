#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <limits>

using namespace std;

class Transaction {
private:
    int dd, mm, yy;
    string type;
    string head;
    float amount;

public:
    Transaction() {}
    Transaction(string t, string h, float a, int d, int m, int y)
        : type(t), head(h), amount(a), dd(d), mm(m), yy(y) {}

    // ---- Getters ----
    int getMonth() const { return mm; }
    int getYear() const { return yy; }
    string getType() const { return type; }
    float getAmount() const { return amount; }

    // ---- Display Function ----
    void display() const {
        cout << setw(10) << dd << "-" << setw(2) << setfill('0') << mm << "-" << yy << setfill(' ')
             << setw(15) << type
             << setw(25) << head
             << setw(15) << amount << endl;
    }

    // ---- Save transaction (comma-separated) ----
    void saveToFile(ofstream &file) const {
        file << dd << "," << mm << "," << yy << ","
             << type << "," << head << "," << amount << "\n";
    }

    // ---- Load all transactions safely ----
    static vector<Transaction> loadAll() {
        vector<Transaction> records;
        ifstream file("transaction.txt");
        if (!file) return records;

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string t, h, temp;
            int d, m, y;
            float a;
            char comma;

            getline(ss, temp, ','); d = stoi(temp);
            getline(ss, temp, ','); m = stoi(temp);
            getline(ss, temp, ','); y = stoi(temp);
            getline(ss, t, ',');
            getline(ss, h, ',');
            ss >> a;

            records.emplace_back(t, h, a, d, m, y);
        }
        file.close();
        return records;
    }
};

// ===================== Utility Functions =====================

bool isValidDate(int d, int m, int y) {
    return (d >= 1 && d <= 31 && m >= 1 && m <= 12 && y >= 1900);
}

void recordTransaction(const string &type) {
    string head;
    float amount;
    int d, m, y;

    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // flush buffer
    cout << "\nEnter Head Name: ";
    getline(cin, head);

    cout << "Enter Amount: ";
    cin >> amount;

    cout << "Enter Date (DD MM YYYY): ";
    cin >> d >> m >> y;

    if (!isValidDate(d, m, y)) {
        cout << " Invalid date! Please re-enter.\n";
        return;
    }

    ofstream wr("transaction.txt", ios::app);
    if (!wr) {
        cout << " Error opening file!\n";
        return;
    }

    Transaction obj(type, head, amount, d, m, y);
    obj.saveToFile(wr);
    wr.close();

    cout << " " << type << " Recorded Successfully!\n";
}

void showTransaction() {
    vector<Transaction> records = Transaction::loadAll();

    if (records.empty()) {
        cout << "\n⚠️ No transactions found!\n";
        return;
    }

    int m, y;
    cout << "\nEnter Month and Year to View (MM YYYY): ";
    cin >> m >> y;

    float total_income = 0, total_expense = 0, total_saving = 0;
    bool found = false;

    cout << "\n----------------------------------------------------------------------------------------";
    cout << "\nDate          Type                 Head                    Amount";
    cout << "\n----------------------------------------------------------------------------------------\n";

    // --- Loop through each record only once ---
    for (const auto &t : records) {
        if (t.getMonth() == m && t.getYear() == y) {
            found = true;
            t.display();
            if (t.getType() == "Income")
                total_income += t.getAmount();
            else if (t.getType() == "Expense")
                total_expense += t.getAmount();
            else if (t.getType() == "Saving")
                total_saving += t.getAmount();
        }
    }

    // --- Print summary only once ---
    cout << "----------------------------------------------------------------------------------------\n";
    if (!found) {
        cout << "⚠️ No transactions found for this month/year.\n";
    } else {
        cout << "Total Income : " << total_income
             << "\nTotal Expense: " << total_expense
             << "\nTotal Saving : " << total_saving
             << "\nNet Balance  : " << (total_income - total_expense)
             << "\n----------------------------------------------------------------------------------------\n";
    }
}

// ===================== MAIN MENU =====================

int main() {
    int choice;
    while (true) {
        cout << "\n===================================================================";
        cout << "\n       PERSONAL FINANCE MANAGER ";
        cout << "\n===================================================================";
        cout << "\n1. Record Income";
        cout << "\n2. Record Expense";
        cout << "\n3. Record Saving";
        cout << "\n4. Show Monthly Transactions";
        cout << "\n5. Exit";
        cout << "\n-------------------------------------------------------------------";
        cout << "\nEnter Your Choice: ";
        cin >> choice;

        switch (choice) {
            case 1: recordTransaction("Income"); break;
            case 2: recordTransaction("Expense"); break;
            case 3: recordTransaction("Saving"); break;
            case 4: showTransaction(); break;
            case 5: cout << " Exiting... Goodbye!\n"; return 0;
            default: cout << " Invalid Option! Try again.\n";
        }
    }
}





