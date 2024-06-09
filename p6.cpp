#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

struct Process {
    int id;
    int arrival;
    int service;
    int Turnaround;
    int Ts;
};
bool sortByArrivalTime(const Process& a, const Process& b) 
{
    return a.arrival < b.arrival;
}
void roundRobinScheduling(vector<Process>& processes, int timeQuantum) {
    int n = processes.size();
    queue<Process> readyQueue;
    
    int currentTime = 0;

    vector<bool> processed(n, false);
    cout<<"Round Robin Scheduling when q=2:"<<endl<<endl;
    cout << "  Time \t| Process" << endl;
    cout<<"------------------"<<endl;
    while (true) {
        bool allProcessesDone = true;
        for (int i = 0; i < n; i++) {
            if (!processed[i] && processes[i].arrival <= currentTime) 
            {
                allProcessesDone = false;

                Process& currentProcess = processes[i];
                if (currentProcess.service <= timeQuantum)
                 {
                    // Process completes within the time quantum
                    currentTime += currentProcess.service;
                    cout<<" " << currentTime - currentProcess.service << "-" << currentTime << "\t|  P" << currentProcess.id << endl;
                    processes[i].Turnaround = currentTime - currentProcess.arrival; //turnaround time
                    processed[i] = true;
                } 
                else 
                {
                    // Process still needs more time
                    currentTime += timeQuantum;
                    currentProcess.service -= timeQuantum;
                    cout<<" " << currentTime - timeQuantum << "-" << currentTime << " \t|  P" << currentProcess.id << endl;
                }
                
            }
            
        }

        if (allProcessesDone)
            break;
    }
}


vector<Process> readFromFile(const string& filename) 
{
    vector<Process> processes;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Cannot open the file." << endl;
        return processes;
    }

    string line;
    getline(file, line); // Skip the header line

    while (getline(file, line)) 
    {
        istringstream ss(line);
        Process process;

        if (ss >> process.id && ss.ignore(2) // Ignore the comma and space
            && ss >> process.service && ss.ignore(2)
            && ss >> process.arrival) {
            processes.push_back(process);
        }
       else 
         cout << "Error: Invalid line format: " << line << endl;
        
    }

    file.close();
    return processes;
}

void TurnaroundTime(const vector<Process>& processes) 
{
    
    cout<<"Turn Around Times" <<endl;
    int n = processes.size();
    float trts=0.0;
    for (int i = 0; i < n; i++) 
    {
      cout << "Process " << processes[i].id << ":" << endl;
      cout << fixed << setprecision(4);      
      cout << "\tTr: " << processes[i].Turnaround << endl;
      trts = static_cast<float>(processes[i].Turnaround) / processes[i].Ts;
      cout << "\tTr/Ts: " << trts << endl;
    }

}



int main() {
  
    string filename = "Processes.txt"; 
    
    vector<Process> processes = readFromFile(filename);
    
    sort(processes.begin(), processes.end(), sortByArrivalTime);
    
    for(int i=0;i<processes.size();i++)
      processes[i].Ts=processes[i].service;

    int timeQuantum = 2;

    roundRobinScheduling(processes, timeQuantum);
    
    TurnaroundTime(processes);

    return 0;
}
