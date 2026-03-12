#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
using namespace std;

class Payroll_System {
private:
    int id;
    string name;
    double salary, allowance, deduction, bonus, tax, taxPercent, GrossIncome, NetSalary;

public:
    Payroll_System() : id(0), salary(0), allowance(0), deduction(0), bonus(0), tax(0), taxPercent(0), GrossIncome(0), NetSalary(0) {}

    void inputEmployee() {
        cout << "Provide ID: ";        cin >> id;
        cin.ignore();
        cout << "Provide Name: ";      getline(cin, name);
        cout << "Provide Salary: ";    cin >> salary;
        cout << "Provide Allowance: "; cin >> allowance;
        cout << "Provide Deduction: "; cin >> deduction;
        cout << "Provide Bonus: ";     cin >> bonus;
        cout << "Provide Tax %: ";     cin >> taxPercent;
    }

    void Gross_Salary() { 
        GrossIncome = salary + allowance + bonus; }
    void Tax_deduct()   { 
        tax = (GrossIncome * taxPercent) / 100; }
    void Net_salary()   { 
        NetSalary = GrossIncome - tax - deduction; }

    void Calculate() {
        Gross_Salary();
        Tax_deduct();
        Net_salary();
    }

    void Print_Slip() {
        cout << fixed << setprecision(2);
        cout << "\nYour Payslip is as follows" << endl;
        cout << "ID: "                  << id          << endl;
        cout << "Name: "                << name        << endl;
        cout << "Salary: "              << salary      << endl;
        cout << "Allowance: "           << allowance   << endl;
        cout << "Bonus: "               << bonus       << endl;
        cout << "The Gross Salary is: " << GrossIncome << endl;
        cout << "The Taxed amount is: " << tax         << endl;
        cout << "The Net Salary is: "   << NetSalary   << endl;
    }

    void Update() {
        int choice;
        cout << "\n1. Salary\n2. Allowance\n3. Deduction\n4. Bonus\n5. Tax %" << endl;
        cout << "Choose field: "; cin >> choice;
        if      (choice == 1) { cout << "New Salary: ";    cin >> salary;     }
        else if (choice == 2) { cout << "New Allowance: "; cin >> allowance;  }
        else if (choice == 3) { cout << "New Deduction: "; cin >> deduction;  }
        else if (choice == 4) { cout << "New Bonus: ";     cin >> bonus;      }
        else if (choice == 5) { cout << "New Tax %: ";     cin >> taxPercent; }
        else { cout << "Invalid." << endl; return; }
        Calculate();
        cout << "Updated." << endl;
    }

    void saveToFile(const string& filename) const {
        ofstream fout(filename, ios::app);
        fout << id << "|" << name << "|" << salary << "|" << allowance << "|" << deduction << "|" << bonus << "|" << taxPercent << "\n";
        fout.close();
    }

    bool loadFromLine(const string& line) {
        vector<string> t;
        string tmp;
        for (char c : line) { if (c == '|') { t.push_back(tmp); tmp = ""; } else tmp += c; }
        t.push_back(tmp);
        if (t.size() < 7) return false;
        id = stoi(t[0]); name = t[1]; salary = stod(t[2]); allowance = stod(t[3]);
        deduction = stod(t[4]); bonus = stod(t[5]); taxPercent = stod(t[6]);
        Calculate();
        return true;
    }

    int    getId()   const { return id;   }
    string getName() const { return name; }
};

vector<Payroll_System> employees;
const string dataFile = "payroll_data.txt";

void saveAll() {
    ofstream fout(dataFile, ios::trunc); fout.close();
    for (const auto& e : employees) e.saveToFile(dataFile);
}

void loadFromFile() {
    ifstream fin(dataFile); if (!fin) return;
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        Payroll_System e;
        if (e.loadFromLine(line)) employees.push_back(e);
    }
    fin.close();
}

void addEmployee() {
    Payroll_System a;
    a.inputEmployee(); a.Calculate(); a.Print_Slip();
    employees.push_back(a); saveAll();
}

void displayAll() {
    if (employees.empty()) { cout << "\nNo records found." << endl; return; }
    for (auto& e : employees) e.Print_Slip();
}

void searchEmployee() {
    cout << "\nEnter ID to search: ";
    int sid;
    cin >> sid;
    for (auto& e : employees) { if (e.getId() == sid) { e.Print_Slip(); return; } }
    cout << "Employee not found." << endl;
}

void updateEmployee() {
    cout << "\nEnter ID to update: "; int sid; cin >> sid;
    for (auto& e : employees) { if (e.getId() == sid) { e.Update(); saveAll(); return; } }
    cout << "Employee not found." << endl;
}

void deleteEmployee() {
    cout << "\nEnter ID to delete: "; int sid; cin >> sid;
    for (auto it = employees.begin(); it != employees.end(); ++it) {
        if (it->getId() == sid) { employees.erase(it); saveAll(); cout << "Deleted." << endl; return; }
    }
    cout << "Employee not found." << endl;
}

int main() {
    loadFromFile();
    int choice;
    do {
        cout << "\n---- Payroll System ----" << endl;
        cout << "1. Add Employee"            << endl;
        cout << "2. Display All"             << endl;
        cout << "3. Search Employee"         << endl;
        cout << "4. Update Employee"         << endl;
        cout << "5. Delete Employee"         << endl;
        cout << "0. Exit"                    << endl;
        cout << "Choose option: "; cin >> choice;

        if      (choice == 1) addEmployee();
        else if (choice == 2) displayAll();
        else if (choice == 3) searchEmployee();
        else if (choice == 4) updateEmployee();
        else if (choice == 5) deleteEmployee();
        else if (choice != 0) cout << "Invalid option." << endl;
    } while (choice != 0);
    return 0;
}
