/*
* MS549 Data Structures and Testing
* 12/5/24
* Jeff O'Hara
* Hash Tables - This program will create a user interface that allows the user to input values into a hash table
* which will be stored, displayed, and then allow the user to make changes to the table by retrieving, inserting new,
* or deleting items from the hash table from a basic GUI. The program will also use performance analysis to determine 
* the efficiency of the program and the data flow.
*/

// Libraries
#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <unordered_map>
#include <limits>
#include "MyHashTable.h"

using namespace std;

// This function measures how long a given operation takes in nanoseconds
template<typename Func>
long long measureTime(Func func) {
    auto start = chrono::high_resolution_clock::now(); // Start timer
    func(); // Execute the passed-in function
    auto end = chrono::high_resolution_clock::now();   // End timer
    return chrono::duration_cast<chrono::nanoseconds>(end - start).count();
}

// Print a menu of operations the user can perform on the hash table
void printMenu() {
    cout << "\n-----------------------------------------\n";
    cout << "           CUSTOM HASH TABLE MENU         \n";
    cout << "-----------------------------------------\n";
    cout << "1. Insert a key-value pair\n";
    cout << "2. Retrieve a value by key\n";
    cout << "3. Remove a key-value pair by key\n";
    cout << "4. Show current number of elements\n";
    cout << "5. Run performance tests and compare with std::unordered_map\n";
    cout << "6. Exit\n";
    cout << "-----------------------------------------\n";
    cout << "Please choose an option: ";
}

int main() {
    cout << "Welcome to the Custom Hash Table Program!\n\n";
    cout << "This program uses a templated hash table with separate chaining.\n";
    cout << "You can insert, retrieve, and remove (key, value) pairs.\n";
    cout << "You can also run performance tests to compare this table\n";
    cout << "against the built-in std::unordered_map.\n";
    cout << "--------------------------------------------------------------------------------\n";

    // Create a hash table with int keys and string values for the user to interact with
    MyHashTable<int, string> myTable(101);

    bool running = true;
    while (running) {
        printMenu();
        int choice;
        cin >> choice;

        // Validate user input for the menu choice
        if (cin.fail()) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid input. Please try again.\n";
            continue;
        }

        switch (choice) {
        case 1: {
            // Insert operation
            cout << "Enter an integer key: ";
            int key;
            cin >> key;

            // Clear input buffer before reading string
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            cout << "Enter a string value: ";
            string val;
            getline(cin, val); // Read full line for value

            // Insert the (key, value) into the hash table
            myTable.insert(key, val);
            cout << "Inserted (" << key << ", " << val << ") into the hash table.\n";
            break;
        }
        case 2: {
            // Retrieve operation
            cout << "Enter the key you want to retrieve: ";
            int key;
            cin >> key;

            // Attempt to retrieve value pointer
            auto val = myTable.retrieve(key);
            if (val) {
                cout << "Value for key " << key << " is: " << *val << "\n";
            }
            else {
                cout << "No value found for key " << key << ".\n";
            }
            break;
        }
        case 3: {
            // Remove operation
            cout << "Enter the key you want to remove: ";
            int key;
            cin >> key;

            // Attempt to remove from table
            if (myTable.remove(key)) {
                cout << "Key " << key << " was removed successfully.\n";
            }
            else {
                cout << "Key " << key << " not found, nothing to remove.\n";
            }
            break;
        }
        case 4: {
            // Show current size
            cout << "The hash table currently contains " << myTable.size() << " elements.\n";
            break;
        }
        case 5: {
            // Run performance tests against various input sizes
            cout << "\nRunning performance tests. Please wait...\n";

            vector<int> testSizes = { 100, 1000, 10000 }; // Different scales of operations
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<int> dist_key(1, 1000000);
            uniform_int_distribution<int> dist_val(1, 1000000);

            for (auto n : testSizes) {
                // Prepare a new custom hash table and random data
                MyHashTable<int, int> testTable(2 * n);
                vector<int> keys;
                vector<int> vals;
                keys.reserve(n);
                vals.reserve(n);

                // Generate random keys and values
                for (int i = 0; i < n; ++i) {
                    keys.push_back(dist_key(gen));
                    vals.push_back(dist_val(gen));
                }

                // Measure insert performance for custom hash table
                long long insertTime = measureTime([&]() {
                    for (int i = 0; i < n; ++i) {
                        testTable.insert(keys[i], vals[i]);
                    }
                    });

                // Measure retrieve performance for custom hash table
                long long retrieveTime = measureTime([&]() {
                    for (int i = 0; i < n; ++i) {
                        testTable.retrieve(keys[i]);
                    }
                    });

                // Measure remove performance for custom hash table
                long long removeTime = measureTime([&]() {
                    for (int i = 0; i < n; ++i) {
                        testTable.remove(keys[i]);
                    }
                    });

                // Compare with std::unordered_map
                unordered_map<int, int> builtIn;
                long long builtInInsertTime = measureTime([&]() {
                    for (int i = 0; i < n; ++i) {
                        builtIn[keys[i]] = vals[i];
                    }
                    });

                long long builtInRetrieveTime = measureTime([&]() {
                    for (int i = 0; i < n; ++i) {
                        builtIn.find(keys[i]);
                    }
                    });

                long long builtInRemoveTime = measureTime([&]() {
                    for (int i = 0; i < n; ++i) {
                        builtIn.erase(keys[i]);
                    }
                    });

                // Print performance results for both tables
                cout << "\n-------------------------------------------------\n";
                cout << "Performance Results for " << n << " operations:\n";
                cout << "-------------------------------------------------\n";
                cout << "Custom Hash Table (Separate Chaining):\n";
                cout << " Insert time (total ns):   " << insertTime << "\n";
                cout << " Retrieve time (total ns): " << retrieveTime << "\n";
                cout << " Remove time (total ns):   " << removeTime << "\n\n";
                cout << "std::unordered_map:\n";
                cout << " Insert time (total ns):   " << builtInInsertTime << "\n";
                cout << " Retrieve time (total ns): " << builtInRetrieveTime << "\n";
                cout << " Remove time (total ns):   " << builtInRemoveTime << "\n";
                cout << "-------------------------------------------------\n";
            }

            cout << "\nPerformance tests completed. You can see how our custom hash table compares to std::unordered_map.\n";
            break;
        }
        case 6: {
            // User chose to exit
            cout << "Exiting the program. Thank you for using the Custom Hash Table!\n";
            running = false;
            break;
        }
        default: {
            cout << "Invalid option. Please try again.\n";
            break;
        }
        }
    }

    return 0;
}
 