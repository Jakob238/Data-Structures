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

    // Constructor
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
class Queue {
public: 
    DT JobPointer; // Pointer to the job (e.g., CPUJob)
    Queue<DT>* next; // Pointer to the next node in the queue

    // Constructor
    Queue(DT job) {
        JobPointer = job;
        next = nullptr;
    }
};

template <class DT>
class NovelQueue {
private:
    // Helper method to update an array of pointers
    void updateNodePtrs() {
        delete[] NodePtrs; // Delete old array
        NodePtrs = new Queue<DT>*[size]; // Create a new array with current size
        Queue<DT>* temp = front; // Start from the front of the queue

        // Populate array with pointers to each node and move to the next node
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
    Queue<DT>* front; // Pointer to the front of the queue
    Queue<DT>** NodePtrs; // Array of pointers to Queue nodes
    int size; // Number of elements in the queue

    // Method to find the index of a job by job_id
    int findJobIndex(int job_id) {
        for (int i = 0; i < size; ++i) {
            if (NodePtrs[i]->JobPointer->job_id == job_id) {
                return i;
            }
        }
        return -1; // Job not found
    }

    // Constructor for empty queue
    NovelQueue() {
        front = nullptr;
        NodePtrs = nullptr;
        size = 0;
    }
    
    // Destructor
    ~NovelQueue() {
        while (front != nullptr) {
            Queue<DT>* temp = front;
            front = front->next;
            delete temp->JobPointer;
            delete temp;
        }
        delete[] NodePtrs;
    }

    void enqueue(DT* job) {  // Adds a new job to the end of the queue
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

    DT* dequeue() {  // Removes a job from the front of the queue
        if (front == nullptr) {
            return nullptr;
        } 
        Queue<DT>* temp = front;
        front = front->next;
        DT* job = temp->JobPointer;
        delete temp;
        size--;

        updateNodePtrs();
        return job;
    }

    void modify(int job_id, int new_priority, int new_job_type, int new_cpu_time, int new_mem) { // Modifies the attributes of a job
        int index = findJobIndex(job_id);
        if (index == -1){
            return; // Job not found
        }
        Queue<DT>* node = NodePtrs[index];

        // Update the job attributes
        node->JobPointer->priority = new_priority;
        node->JobPointer->job_type = new_job_type;
        node->JobPointer->cpu_time_consumed = new_cpu_time;
        node->JobPointer->memory_consumed = new_mem;
        updateNodePtrs();
        
    }

    void change(int job_id, int field_index, int new_value) { // Changes a specific field of a job
        int index = findJobIndex(job_id);
        if (index == -1){
            return; // Job not found
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


    void promote(int job_id, int positions) { // Moves a job by a specific number of positions
        if (front == nullptr || front->JobPointer->job_id == job_id) {
            return; // Queue is empty or job is already at the front
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

        // Promotes the job by the specified number of positions
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

            // Goes to the new position
            Queue<DT>* temp = front;
            Queue<DT>* prev = nullptr;
            for (int i = 0; i < currentPosition - positions; ++i) {
                prev = temp;
                temp = temp->next;
            }

            // Inserts the node at the new position
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


    NovelQueue<DT>* reorder(int attribute_index) { // Reorder queue based on a specific attribute
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

    // Other methods (change, promote, reorder, display, etc.) remain unchanged

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
    NovelQueue<CPUJob>* myNovelQueue = new NovelQueue<CPUJob>();
    char command; // Variable to store the command type
    
    // Variables for job attributes
    int job_id, priority, job_type, cpu_time_consumed, memory_consumed;
    
    // Variables for modifying a job
    int new_priority, new_job_type, new_cpu_time_consumed, new_memory_consumed;
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
                int index = myNovelQueue->findJobIndex(job_id);
                if (index == -1) { // Job not found
                    CPUJob* newJob = new CPUJob(job_id, priority, job_type, cpu_time_consumed, memory_consumed);
                    cout << "Enqueued Job:" << endl;
                    myNovelQueue->enqueue(newJob);
                    newJob->display();
                    cout << "Jobs after enqueue:" << endl;
                    myNovelQueue->display();
                }
                else { // Job already exists
                    cout << "Job ID " << job_id << " already exists!" << endl;
                }
                break;
            }
            case 'R': { // Remove (Dequeue)
                CPUJob* removedJob = myNovelQueue->dequeue();
                if (removedJob) { // Job found and removed
                    cout << "Dequeued Job:" << endl;
                    removedJob->display();
                    delete removedJob; // Clean up memory after use
                    cout << "Jobs after dequeue:" << endl;
                    myNovelQueue->display();
                }
                else { // Job not found
                    cout << "Job with ID " << job_id << " not found in the queue." << endl;
                }
                break;
            }
            // Other cases for 'M', 'C', 'P', 'O', 'D', 'N', 'L' remain unchanged
            case 'M': { // Modify
            cin >> job_id >> new_priority >> new_job_type;
            cin >> new_cpu_time_consumed >> new_memory_consumed;
            int index = myNovelQueue->findJobIndex(job_id);
            if(index != -1){ // Job found
                (*myNovelQueue).modify(job_id, new_priority, new_job_type,
                 new_cpu_time_consumed, new_memory_consumed);
                cout << "Modified Job ID " << job_id << ":" << endl;
                myNovelQueue->NodePtrs[index]->JobPointer.display();
                cout << "Jobs after modification:" << endl;
                (*myNovelQueue).display();
            }
            else{ // Job not found
                cout << "Job with ID " << job_id << " not found in the queue." << endl;
            }
            break;
        }
        case 'C': { // Change Job Values
            cin >> job_id >> field_index >> new_value;
            int index = myNovelQueue->findJobIndex(job_id);
            if(index != -1){ // Job found
                (*myNovelQueue).change(job_id, field_index, new_value);
                cout << "Changed Job ID " << job_id << " field " << field_index << " to " << new_value << ":" << endl;
                myNovelQueue->NodePtrs[index]->JobPointer.display();
                cout << "Jobs after changing field:" << endl;
                (*myNovelQueue).display();
            }
            else{ // Job not found
                cout << "Job with ID " << job_id << " not found in the queue." << endl;
            }
            break;
        }
        case 'P': { // Promote
            cin >> job_id >> positions;
            int index = myNovelQueue->findJobIndex(job_id);
            if(index != -1){ // Job found
                (*myNovelQueue).promote(job_id, positions);
                cout << "Promoted Job ID " << job_id << " by " << positions << " Position(s):" << endl;
                myNovelQueue->NodePtrs[index]->JobPointer.display();
                cout << "Jobs after promotion:" << endl;
                (*myNovelQueue).display();
            }
            else{ // Job not found
                cout << "Job with ID " << job_id << " not found in the queue." << endl;
            }
            break;
        }
        case 'O': { // Reorder
            cin >> attribute_index;
            NovelQueue<CPUJob>* reorderedQueue = (*myNovelQueue).reorder(attribute_index);
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
        default: // Invalid command
            cout << "Invalid command!" << endl;
        }
    }
}

/* LLM and GitHub Copilot Usage Documentation :


*/


/* Debugging and Testing Plan :
cd Project\Project 3
g++ test.cpp
a.exe < input1.txt
*/

//cd Project\Project 3
//g++ test.cpp
//a.exe < input1.txt