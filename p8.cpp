#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <queue>
using namespace std;

struct Process {
    int id;
    int arrival;
    int service;
    int priority; // Add priority
    int Turnaround;
};

bool sortByArrivalTime(const Process& a, const Process& b) {
    return a.arrival < b.arrival;
}

bool sortByPriority(const Process& a, const Process& b) {
    return a.priority < b.priority;
}

void multiFeedback(vector<Process>& processes) {
    cout << "Multi-Feedback Scheduling:" << endl << endl;
    int n = processes.size();
    int currentTime = 1;

    cout << "Time\t| Process" << endl;
    cout << "------------------" << endl;

    vector<queue<Process>> queues(4); // Four priority queues
    int timeQuantums[] = {2, 4, 8, 16}; // Time quantums for each queue
    int currentQueue = 0;

    for (int i = 0; i < n; i++) {
        queues[currentQueue].push(processes[i]);
    }

    while (true) {
        if (!queues[currentQueue].empty()) {
            Process currentProcess = queues[currentQueue].front();
            queues[currentQueue].pop();

            int quantum = min(timeQuantums[currentQueue], currentProcess.service);
            int startTime = currentTime;
            int endTime = min(currentTime + quantum, startTime + quantum - 1);

            cout << startTime << "-" << endTime << "\t| P" << currentProcess.id << endl;

            currentProcess.service -= (endTime - startTime + 1);
            currentTime = endTime + 1;

            if (currentProcess.service > 0) {
                // Move the process to the next lower priority queue
                currentProcess.priority = min(currentProcess.priority + 1, 3);
                queues[currentProcess.priority].push(currentProcess);
            } else {
                currentProcess.Turnaround = endTime - currentProcess.arrival + 1;
                processes[currentProcess.id - 1].Turnaround = currentProcess.Turnaround;
            }
        }

        // Check if all queues are empty
        bool allQueuesEmpty = true;
        for (int i = 0; i < 4; i++) {
            if (!queues[i].empty()) {
                allQueuesEmpty = false;
                break;
            }
        }

        if (allQueuesEmpty)
            break;

        // Move to the next priority queue
        currentQueue = (currentQueue + 1) % 4;
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

        if (ss >> process.id && ss.ignore(2)
            && ss >> process.service && ss.ignore(2)
            && ss >> process.arrival && ss.ignore(2)
            && ss >> process.priority) { // Read priority
            processes.push_back(process);
        } else
            cout << "Error: Invalid line format: " << line << endl;
    }

    file.close();
    return processes;
}

int main() {
    string filename = "Processes.txt";

    vector<Process> processes = readFromFile(filename);

    sort(processes.begin(), processes.end(), sortByArrivalTime);

    multiFeedback(processes);
    
            

    TurnaroundTime(processes);

    return 0;
}
