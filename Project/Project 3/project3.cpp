# include <iostream>
# include <string>
using namespace std;

class CPUJob {
    public:
    int job_id; // Unique identifier for the job
    int priority; // Priority level of the job (1-10)
    int job_type; // Job type (1-10)
    int cpu_time_consumed; // Total CPU time consumed by the job
    int memory_consumed; // Total memory consumed thus far

        CPUJob(int id, int pri, int type, int cpu_time, int mem) {
            job_id = id;
            priority = pri;
            job_type = type;
            cpu_time_consumed = cpu_time;
            memory_consumed = mem;
        }

    // Method to display the job details
    void display() const {
        cout << "Job ID: " << job_id << ", Priority: " << priority 
             << ", Job Type: " << job_type << ", CPU Time Consumed: " << cpu_time_consumed
             << ", Memory Consumed: " << memory_consumed << endl;
    }
};

template <class DT>
class Queue{
public: 
    DT JobPointer; // Job (e.g., CPUJob)
    Queue<DT>* next; // Pointer to the next node in the queue

    Queue(DT job) {
        JobPointer = job;
        next = nullptr;
    }
};


template <class DT>
class NovelQueue {

private:
    void updateNodePtrs(){
        // Implementation for updating NodePtrs
        delete[] NodePtrs;
        NodePtrs = new Queue<DT>*[size];
        Queue<DT>* temp = front;
        for (int i = 0; i < size; ++i) {
            NodePtrs[i] = temp;
            temp = temp->next;
        }

        // Bubble sort to sort NodePtrs based on job_id
        for (int i = 0; i < size - 1; ++i) {
            for (int j = 0; j < size - i - 1; ++j) {
                if (NodePtrs[j]->JobPointer->job_id > NodePtrs[j + 1]->JobPointer->job_id) {
                    // Swap NodePtrs[j] and NodePtrs[j + 1]
                    Queue<DT>* temp = NodePtrs[j];
                    NodePtrs[j] = NodePtrs[j + 1];
                    NodePtrs[j + 1] = temp;
                }
            }
        }
    }
    
public:
    int findJobIndex(int job_id) const
    {
        for (int i = 0; i < size; ++i) {
            if (NodePtrs[i]->JobPointer->job_id == job_id) {
                return i;
            }
        }
        return -1; // Job not found
        
        //return 0;
    }
    

public:
    Queue<DT>* front; // Pointer to the front of the queue
    Queue<DT>** NodePtrs; // Array of pointers to Queue nodes
    int size; // Number of elements in the queue)


    // Constructor and Destructor
    NovelQueue(){
        front = nullptr;
        NodePtrs = nullptr;
        size = 0;
    }
    
    ~NovelQueue() {
        while (front != nullptr) {
            Queue<DT>* temp = front;
            front = front->next;
            delete temp->JobPointer;
            delete temp;
        }
        delete[] NodePtrs;
    }


    void enqueue(DT job){

        // Create a new queue node
        Queue<DT>* newNode = new Queue<DT>(job);
        
        if (front == nullptr) {
            front = newNode;
        } 
        else {
            Queue<DT>* temp = front;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
        size++;
        updateNodePtrs();
    }   


    DT dequeue(){
       
       if (front == nullptr){
            return nullptr;
        } 
        Queue<DT>* temp = front;
        front = front->next;
        DT job = temp->JobPointer;
        delete temp;
        size--;
        updateNodePtrs();
        return job;
    }

    void modify(int job_id, int new_priority, int new_job_type, int new_cpu_time, int new_mem) {
        int index = findJobIndex(job_id);
        if (index == -1){
            return;
        }
        Queue<DT>* node = NodePtrs[index];
        node->JobPointer->priority = new_priority;
        node->JobPointer->job_type = new_job_type;
        node->JobPointer->cpu_time_consumed = new_cpu_time;
        node->JobPointer->memory_consumed = new_mem;
        updateNodePtrs();
        
    }

    void change(int job_id, int field_index, int new_value) {
        int index = findJobIndex(job_id);
        if (index == -1){
            return;
        }
        Queue<DT>* node = NodePtrs[index];
        switch (field_index) {
            case 1: node->JobPointer->priority = new_value; break;
            case 2: node->JobPointer->job_type = new_value; break;
            case 3: node->JobPointer->cpu_time_consumed = new_value; break;
            case 4: node->JobPointer->memory_consumed = new_value; break;
        }
        updateNodePtrs();
    }


    void promote(int job_id, int positions) {
        if (front == nullptr || front->JobPointer->job_id == job_id) {
            return; // If the queue is empty or the job is already at the front, do nothing
        }

        Queue<DT>* previous = nullptr;
        Queue<DT>* current = front;

        // Find the job in the queue
        int currentPosition = 0;
        while (current != nullptr && current->JobPointer->job_id != job_id) {
            previous = current;
            current = current->next;
            currentPosition++;
        }

        if (current == nullptr) {
            return; // Job not found
        }

        // If number of positions exceeds its current position, move it to the front
        if (positions >= currentPosition) {
            if (previous != nullptr) {
                previous->next = current->next;
            }
            current->next = front;
            front = current;
        } else {
            // Move the job up by the specified number of positions
            if (previous != nullptr) {
                previous->next = current->next;
            }

            Queue<DT>* temp = front;
            Queue<DT>* prev = nullptr;
            for (int i = 0; i < currentPosition - positions; ++i) {
                prev = temp;
                temp = temp->next;
            }

            if (prev == nullptr) {
                current->next = front;
                front = current;
            } else {
                current->next = prev->next;
                prev->next = current;
            }
        }

        updateNodePtrs();
    }


    NovelQueue<DT>* reorder(int attribute_index) {
        // Create an array to hold all nodes for easier sorting
    Queue<DT>** tempArr = new Queue<DT>*[size];
    
    // Fill the array with pointers to the current queue nodes
    Queue<DT>* current = front;
    for (int i = 0; i < size; ++i) {
        tempArr[i] = current;
        current = current->next;
    }

    // Sort the array based on the chosen attribute index
    for (int i = 0; i < size - 1; ++i) {
        for (int j = 0; j < size - i - 1; ++j) {
            bool swapNeeded = false;
            switch (attribute_index) {
                case 1: swapNeeded = (tempArr[j]->JobPointer->job_id > tempArr[j + 1]->JobPointer->job_id); break;
                case 2: swapNeeded = (tempArr[j]->JobPointer->priority > tempArr[j + 1]->JobPointer->priority); break;
                case 3: swapNeeded = (tempArr[j]->JobPointer->job_type > tempArr[j + 1]->JobPointer->job_type); break;
                case 4: swapNeeded = (tempArr[j]->JobPointer->cpu_time_consumed > tempArr[j + 1]->JobPointer->cpu_time_consumed); break;
                case 5: swapNeeded = (tempArr[j]->JobPointer->memory_consumed > tempArr[j + 1]->JobPointer->memory_consumed); break;
                default: break;
            }
            if (swapNeeded) {
                Queue<DT>* temp = tempArr[j];
                tempArr[j] = tempArr[j + 1];
                tempArr[j + 1] = temp;
            }
        }
    }

    // Rebuild the linked list from the sorted array
    front = tempArr[0];
    current = front;
    for (int i = 1; i < size; ++i) {
        current->next = tempArr[i];
        current = current->next;
    }
    current->next = nullptr; // Set the last node's next to nullptr
    
    // Free the temporary array
    delete[] tempArr;

    // Update NodePtrs to reflect the new order
    updateNodePtrs();
    
    return this;

        // // Create a new NovelQueue to store the reordered elements

        // NovelQueue<DT>* newQueue = new NovelQueue<DT>();

        // // Temporary pointer to traverse the current queue
        // Queue<DT>* current = front;

        // // Traverse through the current queue and insert each job into the new queue in sorted order
        // while (current != nullptr) {
        //     DT currentJob = current->JobPointer; // Get the current job
            
        //     // Insert currentJob into the correct position in the new queue
        //     Queue<DT>* newNode = new Queue<DT>(currentJob); // Create a new node for the new queue
            
        //     // If newQueue is empty or currentJob should be placed at the front
        //     if (newQueue->front == nullptr || 
        //         (attribute_index == 1 && newNode->JobPointer->job_id < newQueue->front->JobPointer->job_id) ||
        //         (attribute_index == 2 && newNode->JobPointer->priority < newQueue->front->JobPointer->priority) ||
        //         (attribute_index == 3 && newNode->JobPointer->job_type < newQueue->front->JobPointer->job_type) ||
        //         (attribute_index == 4 && newNode->JobPointer->cpu_time_consumed < newQueue->front->JobPointer->cpu_time_consumed) ||
        //         (attribute_index == 5 && newNode->JobPointer->memory_consumed < newQueue->front->JobPointer->memory_consumed)) {
                
        //         newNode->next = newQueue->front; // Insert at the front
        //         newQueue->front = newNode;
        //     } 
        //     else {
        //         // Find the correct position in the new queue to insert the new node
        //         Queue<DT>* temp = newQueue->front;
        //         while (temp->next != nullptr) {
        //             bool condition = false;
        //             switch (attribute_index) {
        //                 case 1: condition = (newNode->JobPointer->job_id < temp->next->JobPointer->job_id); break;
        //                 case 2: condition = (newNode->JobPointer->priority < temp->next->JobPointer->priority); break;
        //                 case 3: condition = (newNode->JobPointer->job_type < temp->next->JobPointer->job_type); break;
        //                 case 4: condition = (newNode->JobPointer->cpu_time_consumed < temp->next->JobPointer->cpu_time_consumed); break;
        //                 case 5: condition = (newNode->JobPointer->memory_consumed < temp->next->JobPointer->memory_consumed); break;
        //             }
        //             if (condition) {
        //                 break; // Found the correct position to insert
        //             }
        //             temp = temp->next;
        //         }
        //         newNode->next = temp->next; // Insert at the correct position
        //         temp->next = newNode;
        //     }
        //     current = current->next; // Move to the next node in the original queue
        // }
        // return newQueue; // Return the reordered queue
    
    }

    void display() {
        Queue<DT>* temp = front;
        while (temp != nullptr) {
            temp->JobPointer->display();
            temp = temp->next;
        }
    }

    int count() const {
        return size;
    }

    void listJobs() {
        for (int i = 0; i < size; ++i) {
            NodePtrs[i]->JobPointer->display();
        }
    }
};

int main() {
    int n; // Number of commands
    cin >> n; // Read the number of commands

    // Instantiate a NovelQueue for CPUJob pointers
    NovelQueue<CPUJob*>* myNovelQueue = new NovelQueue<CPUJob*>();
    char command; // Variable to store the command type
    
    // Variables for job attributes
    int job_id, priority, job_type, cpu_time_consumed, memory_consumed;
    
    // Variables for modifying a job
    int new_priority, new_job_type, new_cpu_time_consumed;
    int new_memory_consumed;
    int field_index, new_value;
    
    // Variable for the number of positions in the 'Promote' command
    int positions;
    int attribute_index; // Variable for the 'Reorder' command


    for (int i = 0; i < n; ++i) {

    cin >> command; // Read the command type
    switch (command) {
        case 'A': { // Add (Enqueue)
            cin >> job_id >> priority >> job_type;
            cin >> cpu_time_consumed >> memory_consumed;
            CPUJob* newJob = new CPUJob(job_id, priority, job_type,
            cpu_time_consumed, memory_consumed);
            cout << "Enqueued Job:" << endl;
            (*myNovelQueue).enqueue(newJob);
            (*newJob).display();
            cout << "Jobs after enqueue:" << endl;
            (*myNovelQueue).display();
            break;
        }
        case 'R': { // Remove (Dequeue)
            CPUJob* removedJob = (*myNovelQueue).dequeue();
            if (removedJob) {
                cout << "Dequeued Job:" << endl;
                (*removedJob).display();
                delete removedJob; // Clean up memory after use
                 cout << "Jobs after dequeue:" << endl;
                (*myNovelQueue).display();
            }
            break;
        }
        case 'M': { // Modify
            cin >> job_id >> new_priority >> new_job_type;
            cin >> new_cpu_time_consumed >> new_memory_consumed;
            (*myNovelQueue).modify(job_id, new_priority, new_job_type,
            new_cpu_time_consumed, new_memory_consumed);
            int index = myNovelQueue->findJobIndex(job_id);
            if(index != -1){
                cout << "Modified Job ID " << job_id << ":" << endl;
                myNovelQueue->NodePtrs[index]->JobPointer->display();
                cout << "Jobs after modification:" << endl;
                (*myNovelQueue).display();
            }
            else{
                cout << "Job with ID " << job_id << " not found in the queue." << endl;
            }
            break;
        }
        case 'C': { // Change Job Values
            cin >> job_id >> field_index >> new_value;
            (*myNovelQueue).change(job_id, field_index, new_value);
            int index = myNovelQueue->findJobIndex(job_id);
            if(index != -1){
                cout << "Changed Job ID " << job_id << " field " << field_index << " to " << new_value << ":" << endl;
                myNovelQueue->NodePtrs[index]->JobPointer->display();
                cout << "Jobs after changing field:" << endl;
                (*myNovelQueue).display();
            }
            else{
                cout << "Job with ID " << job_id << " not found in the queue." << endl;
            }
            break;
        }
        case 'P': { // Promote
            cin >> job_id >> positions;
            (*myNovelQueue).promote(job_id, positions);
            int index = myNovelQueue->findJobIndex(job_id);
            if(index != -1){
                cout << "Promoted Job ID " << job_id << " by " << positions << " Position(s):" << endl;
                myNovelQueue->NodePtrs[index]->JobPointer->display();
                cout << "Jobs after promotion:" << endl;
                (*myNovelQueue).display();
            }
            else{
                cout << "Job with ID " << job_id << " not found in the queue." << endl;
            }
            break;
        }
        case 'O': { // Reorder
            cin >> attribute_index;
            NovelQueue<CPUJob*>* reorderedQueue = (*myNovelQueue).reorder(attribute_index);
            cout << "Reordered Queue by attribute " << attribute_index << ":"<< endl;
            (*reorderedQueue).display();
            break;
        }
        case 'D': { // Display
            cout << "Displaying all jobs in the queue:" << endl;
            (*myNovelQueue).display();
            break;
        }
        case 'N': { // Count
            cout << "Number of elements in the queue: " <<
            (*myNovelQueue).count() << endl;
            break;
        }
        case 'L': { // List Jobs
        cout << "List of jobs sorted by job IDs:" << endl;
            (*myNovelQueue).listJobs();
            break;
        }
        default:
            cout << "Invalid command!" << endl;
        }
    }
}



/* LLM and GitHub Copilot Usage Documentation :


*/


/* Debugging and Testing Plan :

*/

/*
cd Project\Project 3
g++ project3.cpp
a.exe < input1.txt
*/

//cd Project\Project 3
//g++ project3.cpp
//a.exe < input1.txt
