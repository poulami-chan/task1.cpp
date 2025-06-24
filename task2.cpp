#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <mutex>

using namespace std;

mutex mtx;

string runLengthEncode(const string &input) {
    string output;
    int count = 1;
    for (size_t i = 1; i <= input.size(); ++i) {
        if (i < input.size() && input[i] == input[i - 1]) {
            count++;
        } else {
            output += input[i - 1] + to_string(count);
            count = 1;
        }
    }
    return output;
}

string runLengthDecode(const string &input) {
    string output;
    for (size_t i = 0; i < input.size(); i++) {
        char ch = input[i];
        string countStr;
        while (++i < input.size() && isdigit(input[i]))
            countStr += input[i];
        i--;
        output += string(stoi(countStr), ch);
    }
    return output;
}

void compressChunk(const string &chunk, string &result) {
    string encoded = runLengthEncode(chunk);
    lock_guard<mutex> lock(mtx);
    result += encoded;
}

void decompressChunk(const string &chunk, string &result) {
    string decoded = runLengthDecode(chunk);
    lock_guard<mutex> lock(mtx);
    result += decoded;
}

void processFile(const string &inputFile, const string &outputFile, bool compress) {
    ifstream inFile(inputFile);
    if (!inFile) {
        cerr << "Error opening input file!\n";
        return;
    }

    string data((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
    inFile.close();

    const int numThreads = 4;
    size_t chunkSize = data.size() / numThreads;
    vector<thread> threads;
    string result;

    for (int i = 0; i < numThreads; ++i) {
        size_t start = i * chunkSize;
        size_t end = (i == numThreads - 1) ? data.size() : start + chunkSize;
        string chunk = data.substr(start, end - start);

        if (compress)
            threads.emplace_back(compressChunk, chunk, ref(result));
        else
            threads.emplace_back(decompressChunk, chunk, ref(result));
    }

    for (auto &t : threads)
        t.join();

    ofstream outFile(outputFile);
    outFile << result;
    outFile.close();

    cout << (compress ? "Compression" : "Decompression") << " completed successfully.\n";
}

int main() {
    int choice;
    string inputFile, outputFile;

    cout << "=== MULTITHREADED FILE COMPRESSION TOOL ===\n";
    cout << "1. Compress File\n";
    cout << "2. Decompress File\n";
    cout << "Enter your choice: ";
    cin >> choice;

    cout << "Enter input file name: ";
    cin >> inputFile;
    cout << "Enter output file name: ";
    cin >> outputFile;

    if (choice == 1)
        processFile(inputFile, outputFile, true);
    else if (choice == 2)
        processFile(inputFile, outputFile, false);
    else
        cout << "Invalid choice!\n";

    return 0;
}