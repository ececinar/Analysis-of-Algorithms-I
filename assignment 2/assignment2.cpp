//ECE CINAR, 150150138, Assignment 2
#include <iostream>
#include <fstream>  //to perform file operations
#include <string>   //to store event_name and event_state
#include <vector>   //to perform pop and push operations on arrays easily
using namespace std;

typedef struct Node{    //struct node holds data of an event
    int time;     
    string event_name;
    string event_type;
} Node;

class Heap{             
    private:
        vector<Node> array;     //heap array
        int virtual_clock;
        int latest_scheduled;   //holds the latest scheduling time
    public:
        Heap(){    virtual_clock = 0;    }   //virtual clock initialized to 0
        void setArray(Node element){    array.push_back(element);    }    //setArray is used to fill the array node by node
        void display(); 
        void minHeapify(int);
        void buildMinHeap();
        void scheduleEvents();
        bool isEventScheduled();
};

void Heap::display(){   //can be used to view contents of array at any instant. array size is obtained dynamically
    for(int i = 0; i < array.size(); i++)
        cout << array[i].event_name << " " << array[i].event_type << " " << array[i].time << endl;
}

void Heap::minHeapify(int idx){ //function to min heapify any node
    int l = 2 * idx + 1;    //left index calculation formula
    int r = 2 * idx + 2;    //right index calculation formula
    int smallest = idx;
    Node temp;
    int size = array.size();

    if(l < size && array[l].time < array[idx].time)   
        smallest = l;
    if(r < size && array[r].time < array[smallest].time)
        smallest = r;
    if(smallest != idx){
        temp = array[idx];
        array[idx] = array[smallest];
        array[smallest] = temp;
        minHeapify(smallest);
    }      
}

void Heap::buildMinHeap(){  //function to build min heap
    int size = array.size();
    for(int i = size / 2 - 1; i >= 0; i--)
        minHeapify(i);
}

void Heap::scheduleEvents(){    //function to schedule the nodes
    int size = array.size();
    Node temp;
    virtual_clock++;    //virtual clock starts ticking, currently = 1
    buildMinHeap();
    for(int i = size - 1; i >= 0; i--){ //for loop will be executed #size times, each time smallest element will be popped
        while(!isEventScheduled()){ //while execution time of top node(smallest node) is larger than virtual clock
            if(latest_scheduled!=virtual_clock) 
                cout << "TIME " << virtual_clock << ": " << "NO EVENT" << endl;            
            virtual_clock++;    //clock is incremented
        }         
        cout << "TIME " << virtual_clock << ": " << array[0].event_name << " " << array[0].event_type << "ED" << endl;    
        temp = array[0];
        array[0] = array.back();
        array.back() = temp;
        array.pop_back();   //smallest node is moved to the end scheduled and popped from the array
        if(i)
            minHeapify(0); //heapify the root node
    }
    cout << "TIME " << virtual_clock << ": NO MORE EVENTS, SCHEDULER EXITS" << endl; //heap empty, nothing more to be scheduled
}

bool Heap::isEventScheduled(){
    if(array[0].time == virtual_clock){   
        latest_scheduled = virtual_clock;
        return 1;   //top event is scheduled
    }  
    return 0;   //top event is not scheduled yet    
}

int main(int argc, char **argv){
    char *file = argv[1];
    Heap h;
    Node element;
    string name;    
    int start, end;
    ifstream fp;
    try{
        fp.open(file);
        if(!fp.is_open())
            throw "Error opening file.";    }

    catch(const char *message){
        cout << message << endl;
        return 1;   }   //end of program, not successful

    fp >> name >> start >> end; //since eof reads the last line of a file twice, initial read must be done

    while(!fp.eof()){
        element.event_name = name;
        element.event_type = "START";
        element.time = start;
        h.setArray(element);  //add start node to array
        element.event_type = "END";
        element.time = end;
        h.setArray(element);  //add end node to array
        fp >> name >> start >> end;
    }

    h.scheduleEvents();
    
    fp.close();
    
    return 0;
}