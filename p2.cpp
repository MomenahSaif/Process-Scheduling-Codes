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
    int Turnaround;
};

bool sortByArrivalTime(const Process& a, const Process& b) 
{
    return a.arrival < b.arrival;
}

void spn(vector<Process>& processes,int n)
{
    cout<<" Shortest Process Next(SPN) :"<<endl<<endl;
    int currentTime = 0,i = 0,startTime = 0,endTime = 0;
    
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    cout << " Time \t| Process"<<endl;
    cout << "-----------------"<<endl;

    while (i < n || !pq.empty()) {
        while (i < n && processes[i].arrival <= currentTime) {
            pq.push({processes[i].service, processes[i].id});
            i++;
        }

        if (pq.empty()) {
            currentTime = processes[i].arrival;
            continue;
        }

        pair<int, int> currentProcess = pq.top();
        pq.pop();

        startTime = currentTime;
        endTime = currentTime + currentProcess.first;
        processes[currentProcess.second - 1].Turnaround = endTime - processes[currentProcess.second - 1].arrival;// store turnaround time
        cout<< startTime << "-" << endTime << "\t| P" << currentProcess.second << endl;
        currentTime = endTime;
        
        
    }
      
    cout<< endl<<endl;
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
      trts = static_cast<float>(processes[i].Turnaround) / processes[i].service;
      cout << "\tTr/Ts: " << trts << endl;
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

int main() {
    string filename = "Processes.txt"; 
    
    vector<Process> processes = readFromFile(filename);
    int n=processes.size();;

    sort(processes.begin(), processes.end(), sortByArrivalTime);
    spn(processes,n);
    
    TurnaroundTime(processes);
    return 0;
}
