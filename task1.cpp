#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void writeToFile(const string& filename) {
    ofstream outFile(filename); // overwrite existing content
    if (!outFile) {
        cerr << "Error opening file for writing!\n";
        return;
    }
    string data;
    cout << "Enter data to write into the file:\n";
    cin.ignore(); // Clear input buffer
    getline(cin, data);
    outFile << data << endl;
    outFile.close();
    cout << "Data written successfully.\n";
}

void appendToFile(const string& filename) {
    ofstream outFile(filename, ios::app); // append mode
    if (!outFile) {
        cerr << "Error opening file for appending!\n";
        return;
    }
    string data;
    cout << "Enter data to append into the file:\n";
    cin.ignore(); // Clear input buffer
    getline(cin, data);
    outFile << data << endl;
    outFile.close();
    cout << "Data appended successfully.\n";
}

void readFromFile(const string& filename) {
    ifstream inFile(filename);
    if (!inFile) {
        cerr << "Error opening file for reading!\n";
        return;
    }
    string line;
    cout << "\n--- File Content ---\n";
    while (getline(inFile, line)) {
        cout << line << endl;
    }
    inFile.close();
    cout << "--------------------\n";
}

int main() {
    string filename = "data.txt";
    int choice;

    do {
        cout << "\n=== FILE MANAGEMENT TOOL ===\n";
        cout << "1. Write to File\n";
        cout << "2. Append to File\n";
        cout << "3. Read from File\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                writeToFile(filename);
                break;
            case 2:
                appendToFile(filename);
                break;
            case 3:
                readFromFile(filename);
                break;
            case 4:
                cout << "Exiting program...\n";
                break;
            default:
                cout << "Invalid choice! Try again.\n";
        }

    } while (choice != 4);

    return 0;
}
