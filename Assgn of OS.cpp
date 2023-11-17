#include <iostream>
#include <vector>

using namespace std;

// Function to check if the system is in a safe state
bool isSafe(const vector<vector<int>>& max, const vector<vector<int>>& allocated, const vector<int>& available, vector<int>& work, vector<bool>& finish, vector<int>& safeSeq) {
    int processes = max.size();
    int resources = max[0].size();
    bool found = false;

    for (int i = 0; i < processes; ++i) {
        if (!finish[i]) {
            // Check if the resources for the current process can be satisfied
            bool canExecute = true;
            for (int j = 0; j < resources; ++j) {
                if (max[i][j] - allocated[i][j] > work[j]) {
                    canExecute = false;
                    break;
                }
            }

            if (canExecute) {
                // Execute the process
                finish[i] = true;
                safeSeq.push_back(i);

                for (int j = 0; j < resources; ++j) {
                    work[j] += allocated[i][j];
                }

                // Recursive call to check the next process
                if (isSafe(max, allocated, available, work, finish, safeSeq)) {
                    found = true;
                    break;  // Stop searching if a safe sequence is found
                }

                // Backtrack if the recursive call fails
                finish[i] = false;
                safeSeq.pop_back();
                for (int j = 0; j < resources; ++j) {
                    work[j] -= allocated[i][j];
                }
            }
        }
    }

    // If all processes are finished, the system is in a safe state
    if (!found) {
        for (int i = 0; i < processes; ++i) {
            if (!finish[i]) {
                return false;
            }
        }
    }

    return true;
}

// Function to display a matrix
void displayMatrix(const vector<vector<int>>& matrix, const string& name) {
    cout << name << " matrix:" << endl;
    for (const auto& row : matrix) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
    cout << endl;
}

// Overloaded function to display a vector
void displayVector(const vector<int>& vector, const string& name) {
    cout << name << " vector:" << endl;
    for (int val : vector) {
        cout << val << " ";
    }
    cout << endl << endl;
}

// Function to calculate and display the "need" matrix
void displayNeedMatrix(const vector<vector<int>>& max, const vector<vector<int>>& allocated) {
    cout << "Need matrix:" << endl;
    int processes = max.size();
    int resources = max[0].size();

    for (int i = 0; i < processes; ++i) {
        for (int j = 0; j < resources; ++j) {
            cout << max[i][j] - allocated[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

int main() {
    // Number of processes and resources
    int processes, resources;

    while (true) {
        cout << "Enter the number of processes (-1 to terminate): ";
        cin >> processes;

        // Check for termination
        if (processes == -1) {
            cout << "Program terminated by the user." << endl;
            break;
        }

        cout << "Enter the number of resources: ";
        cin >> resources;

        // Maximum resources that each process can request
        vector<vector<int>> max(processes, vector<int>(resources, 0));
        cout << "Enter the maximum resources each process can request:" << endl;
        for (int i = 0; i < processes; ++i) {
            cout << "Process " << i << ": ";
            for (int j = 0; j < resources; ++j) {
                cin >> max[i][j];
            }
        }

        // Resources currently allocated to each process
        vector<vector<int>> allocated(processes, vector<int>(resources, 0));
        cout << "Enter the resources currently allocated to each process:" << endl;
        for (int i = 0; i < processes; ++i) {
            cout << "Process " << i << ": ";
            for (int j = 0; j < resources; ++j) {
                cin >> allocated[i][j];
            }
        }

        // Total available resources in the system
        vector<int> available(resources, 0);
        cout << "Enter the total available resources in the system: ";
        for (int j = 0; j < resources; ++j) {
            cin >> available[j];
        }

        // Work vector used in the safety check
        vector<int> work = available;

        // Finish vector to track the completion status of each process
        vector<bool> finish(processes, false);

        // Vector to store the safe sequence
        vector<int> safeSeq;

        // Check if the system is in a safe state and get the safe sequence
        if (isSafe(max, allocated, available, work, finish, safeSeq)) {
            // Display the safe sequence
            cout << "Safe Sequence: ";
            for (int i : safeSeq) {
                cout << "P" << i << " ";
            }
            cout << endl;

            // Display the matrices
            displayMatrix(max, "Max");
            displayMatrix(allocated, "Allocated");
            displayVector(available, "Available");

            // Display the "need" matrix
            displayNeedMatrix(max, allocated);
        } else {
            cout << "The system is in an unsafe state." << endl;
        }
    }

    return 0;
}