#include <iostream>
#include <fstream>
#include <cstdio>
#include "String.h"
#include "Vector.h"
using namespace std;

class FileHandler {
private:
    FILE* dataset;          // File pointer for handling the file
    char firstLine[1024]; // Buffer to store the first line
    int headings;           // Stores the number of headings
    int selecteted;         // Stores selected value (name adjusted to match provided spelling)

public:
    // Constructor initializing member variables and opening file
    FileHandler(String& fileName)
        : dataset(nullptr), headings(0), selecteted(0) {
        openFile(fileName);
    }

    void openFile(String& fileName) 
    {
        dataset = fopen(fileName.data, "r"); // Open file in read mode
        if (dataset == nullptr) {
            cerr << "Error: Unable to open file." << endl;
            return;
        }

        // Read and display the first line
        if (fgets(firstLine, sizeof(firstLine), dataset) != nullptr) {
            cout << "First Line: " << firstLine;
            cout << "Length of First Line: " << getFirstLineLength(fileName) << endl;
        }
        else {
            cerr << "Error: Unable to read the first line or file is empty." << endl;
        }

        fclose(dataset); // Close the file
    }

    int getFirstLineLength(String& fileName) {
        FILE* file = fopen(fileName.data, "r"); // Open the file in read mode
        if (file == nullptr) {
            cerr << "Error: Unable to open file." << endl;
            return -1; // Indicate failure to open the file
        }

        char ch;
        int length = 0;

        // Read characters until newline or EOF
        while ((ch = fgetc(file)) != EOF) {
            if (ch == '\n') { // Stop at the newline character
                break;
            }
            ++length; // Increment the length counter
        }

        fclose(file); // Close the file
        return length; // Return the calculated length
    }
    
};

