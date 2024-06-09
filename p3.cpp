#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <queue>
#include <climits>
using namespace std;

struct Process {
    int id;
    int arrival;
    int service;
    int Turnaround;
    int remainingService;
};

bool sortByArrivalTime(const Process& a, const Process& b) {
    return a.arrival < b.arrival;
}

void srt(vector<Process>& processes) {
    cout << "Shortest Remaining Time Scheduling:" << endl << endl;
    int n = processes.size();
    int currentTime = 1;
    int completed = 0;

    cout << "Time\t| Process" << endl;
    cout << "------------------" << endl;

    while (completed < n) {
        int shortestRemainingTime = INT_MAX;
        int selectedProcess = -1;

        for (int i = 0; i < n; i++) {
            if (processes[i].arrival <= currentTime && processes[i].remainingService > 0) {
                if (processes[i].remainingService < shortestRemainingTime) {
                    shortestRemainingTime = processes[i].remainingService;
                    selectedProcess = i;
                }
            }
        }

        if (selectedProcess != -1) {
            int startTime = currentTime;
            currentTime += min(1, processes[selectedProcess].remainingService);
            processes[selectedProcess].remainingService--;

            if (processes[selectedProcess].remainingService == 0) {
                processes[selectedProcess].Turnaround = currentTime - processes[selectedProcess].arrival;
                completed++;
            }

            int endTime = currentTime - 1;
            cout << startTime << "-" << endTime << "\t| P" << processes[selectedProcess].id << endl;
        } else {
            currentTime++;
        }
    }

    cout << endl << endl;
}

void TurnaroundTime(const vector<Process>& processes) {
    cout << "Turn Around Times" << endl;
    int n = processes.size();
    float trts = 0.0;
    for (int i = 0; i < n; i++) {
        cout << "Process " << processes[i].id << ":" << endl;
        cout << fixed << setprecision(4);
        cout << "\tTr: " << processes[i].Turnaround << endl;
        trts = static_cast<float>(processes[i].Turnaround) / processes[i].service;
        cout << "\tTr/Ts: " << trts << endl;
    }
}

vector<Process> readFromFile(const string& filename) {
    vector<Process> processes;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Cannot open the file." << endl;
        return processes;
    }

    string line;
    getline(file, line); // Skip the header line

    while (getline(file, line)) {
        istringstream ss(line);
        Process process;

        if (ss >> process.id && ss.ignore(2) // Ignore the comma and space
            && ss >> process.service && ss.ignore(2)
            && ss >> process.arrival) {
            process.remainingService = process.service;
            processes.push_back(process);
        } else {
            cout << "Error: Invalid line format: " << line << endl;
        }
    }

    file.close();
    return processes;
}

int main() {
    string filename = "Processes.txt";
    vector<Process> processes = readFromFile(filename);
    sort(processes.begin(), processes.end(), sortByArrivalTime);
    srt(processes);
    TurnaroundTime(processes);

    return 0;
}
