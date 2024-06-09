#include <iostream>
#include <vector>
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
};



bool sortByArrivalTime(const Process& a, const Process& b) 
{
    return a.arrival < b.arrival;
}

void fcfs(vector<Process>& processes)
{
    cout << "First Come First Serve Scheduling:" << endl<<endl;
    int n = processes.size();
    int currentTime = 1,startTime=1,endTime=0;

   cout<< "Time\t| Process" <<endl;
   cout<< "------------------" <<endl;

    for (int i = 0; i < n; i++)                // If the process arrives after the current time, wait for it
    {                 
       if (processes[i].arrival > currentTime) 
          currentTime = processes[i].arrival;
        
      startTime = currentTime;
      endTime = startTime + processes[i].service - 1;  

      cout << startTime << "-" << endTime << "\t| P" << processes[i].id << endl;

      currentTime = endTime + 1;  // Move to the next time unit
      processes[i].Turnaround=(endTime - processes[i].arrival);
    
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
    
    sort(processes.begin(), processes.end(), sortByArrivalTime);

    fcfs(processes);
    
    TurnaroundTime(processes);

    return 0;
}
