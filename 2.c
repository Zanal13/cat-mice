//sfj
#include<iostream> 
using namespace std;
#include<conio.h> 
float arr[10][6];  

int main() 
{ 
    float x;
    cout<<"Enter number of Process: "; 
    cin>>x; 
    cout<<"Enter process ID\n"; 
    for(int i=0; i<x; i++) 
    { 
        cout<<"Process "<<i+1<<"\n"; 
        cout<<"Enter Process Id: \n"; 
        cin>>arr[i][0]; 
        cout<<"Enter Arrival Time: \n"; 
        cin>>arr[i][1]; 
        cout<<"Enter Burst Time: \n"; 
        cin>>arr[i][2]; 
    } 
    cout<<"Before Arrange\n"; 
    cout<<"Process ID  Arrival Time Burst Time\n"; 
    for(int i=0; i<x; i++) 
    { 
        cout<<arr[i][0]<<"\n"<<arr[i][1]<<"\n"<<arr[i][2]<<"\n"; 
    }   
    Aval(x, arr); 
    ct(x, arr); 
    cout<<"Final Result\n"; 
    cout<<"Process ID\nArrival Time\nBurst Time\nWaiting Time\nTurnaround Time\n"; 
    for(int i=0; i<x; i++) 
    { 
        cout<<arr[i][0]<<"\n"<<arr[i][1]<<"\n"<<arr[i][2]<<"\n"<<arr[i][4]<<"\n"<<arr[i][5]<<"\n"; 
    }
    float twt = 0, ttat = 0; 
    for (int i=0 ; i<x; i++) 
    { 
        twt = twt + arr[i][4]; 
        ttat = ttat + arr[i][5]; 
    }
    cout << "Average waiting time = "<< (float)twt / (float)x; 
    cout << "\nAverage turn around time = "<< (float)ttat / (float)x;
    getch();
} 
void rp(int *x, int *y) 
{ 
    int z = *x; 
    *x= *y; 
    *y = z; 
} 
  
void Aval(float x, float arr[][6]) 
{ 
    for(int i=0; i<x; i++) 
    { 
        for(int j=0; j<x-i-1; j++) 
        { 
            if(arr[j][1] > arr[j+1][1]) 
            { 
                for(int k=0; k<5; k++) 
                { 
                    rp(arr[j][k], arr[j+1][k]); 
                } 
            } 
        } 
    } 
}  
void ct(float x, float arr[][6]) 
{ 
    int z, a; 
    arr[0][3] = arr[0][1] + arr[0][2]; 
    arr[0][5] = arr[0][3] - arr[0][1]; 
    arr[0][4] = arr[0][5] - arr[0][2];   
    for(int i=1; i<x; i++) 
    { 
        z = arr[i-1][3]; 
        float b  = arr[i][2]; 
        for(int j=i; j<x; j++) 
        { 
            if(z >= arr[j][1] && b >= arr[j][2]) 
            { 
                b = arr[j][2]; 
                a = j; 
            } 
        } 
        arr[a][3] = z + arr[a][2]; 
        arr[a][5] = arr[a][3] - arr[a][1]; 
        arr[a][4] = arr[a][5] - arr[a][2]; 
        for(int k=0; k<6; k++) 
        { 
            rp(arr[a][k], arr[i][k]); 
        } 
    } 
}  
