//FCFS
#include<iostream> 
#include<conio.h>

int main() 
{ 
    int pros[] = {1, 2, 3}; 
    int n = sizeof prs/sizeof pros[0];  
    float bursttime[] = {8, 4, 1};
    float arrivaltime[] = {0.0,0.4,1.0};
    AvrageWaitTime(pros, n, bursttime, arrivaltime); 
    getch();
    return 0; 
} 
void AvrageWaitTime(int pros[], int n, float bt[], float at[]) 
{ 
    float wt[n], tat[n]; 
    WaitingTime(pros, n, bt, wt, at);
    TurnAroundTime(pros, n, bt, wt, tat); 
    cout << "Processes " << " Burst Time " << " Arrival Time "<< " Waiting Time " << " Turn-Around Time "<< " Completion Time \n"; 
    float twt = 0, ttat = 0; 
    for (int i = 0 ; i < n ; i++) 
    { 
        twt = twt + wt[i]; 
        ttat = ttat + tat[i]; 
        float compl_time = tat[i] + at[i]; 
        cout<< i+1 << endl;
        cout<< bt[i] << endl;
        cout<< at[i] << endl;
        cout<< wt[i] << endl;
        cout<< tat[i]  << endl;
        cout<< compl_time << endl; 
    } 
    cout << "Average waiting time = "<< (float)twt / (float)n; 
    cout << "Average turn around time = "<< (float)ttat / (float)n; 
} 
void WaitingTime(int pros[], int n, float bt[], float wt[], float at[]) 
{ 
    float st[n]; 
    st[0] = 0; 
    wt[0] = 0; 
   
    for (int i = 1; i < n ; i++) 
    { 
        st[i] = st[i-1] + bt[i-1];
        wt[i] = st[i] - at[i]; 
        if (wt[i] < 0)
         {
            wt[i] = 0; 
         }
    } 
} 
void TurnAroundTime(int pros[], int n, float bt[], float wt[], float tat[]) 
{  
    for (int i = 0; i < n; i++) 
        {
        tat[i] = bt[i] + wt[i];
         }
} 
