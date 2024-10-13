# include <iostream>
using namespace std;

class CPUJob {
    int job_id; // Unique identifier for the job
    int priority; // Priority level of the job (1-10)
    int job_type; // Job type (1-10)
    int cpu_time_consumed; // Total CPU time consumed by the job
    int memory_consumed; // Total memory consumed thus far

    public:
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
             << ", Job Type: " << job_type << ", CPU Time: " << cpu_time_consumed
             << ", Memory Consumed: " << memory_consumed << endl;
    }
};

template <class DT>
class Queue{
public: 
    DT* JobPointer; // Pointer to a job (e.g., CPUJob)
    Queue<DT>* next; // Pointer to the next node in the queue

    Queue(DT* job) {
        JobPointer = job;
        next = nullptr;
    }
};

template <class DT>
class NovelQueue {
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
        updatePointers(newNode);
        size++;
    }

    DT dequeue(){
        if (front == nullptr) {
            cout << "Queue is empty!" << endl;
            return nullptr;
        }
        Queue<DT>* temp = front;
        front = front->next;
        DT* job = temp->JobPointer;
        delete temp;
        size--;
        return job;

    }

    void modify(int job_id, int new_priority, int new_job_type, int new_cpu_time, int new_mem) {
        Queue<DT>* temp = front;
        while (temp != nullptr) {
            if (temp->JobPointer->job_id == job_id) {
                temp->JobPointer->priority = new_priority;
                temp->JobPointer->job_type = new_job_type;
                temp->JobPointer->cpu_time_consumed = new_cpu_time;
                temp->JobPointer->memory_consumed = new_mem;
                return;
            }
            temp = temp->next;
        }
        cout << "Job not found!" << endl;
    }

    void change(int job_id, int field_index, int new_value) {
        Queue<DT>* temp = front;
        while (temp != nullptr) {
            if (temp->JobPointer->job_id == job_id) {
                switch (field_index) {
                    case 1:
                        temp->JobPointer->priority = new_value;
                        break;
                    case 2:
                        temp->JobPointer->job_type = new_value;
                        break;
                    case 3:
                        temp->JobPointer->cpu_time_consumed = new_value;
                        break;
                    case 4:
                        temp->JobPointer->memory_consumed = new_value;
                        break;
                    default:
                        cout << "Invalid field index!" << endl;
                        return;
                }
                return;
            }
            temp = temp->next;
        }
        cout << "Job not found!" << endl;
    }

    void promote(int job_id, int positions) {
        Queue<DT>* temp = front;
        Queue<DT>* prev = nullptr;
        while (temp != nullptr) {
            if (temp->JobPointer->job_id == job_id) {
                if (prev == nullptr) {
                    front = temp->next;
                } else {
                    prev->next = temp->next;
                }
                break;
            }
            prev = temp;
            temp = temp->next;
        }
        if (temp == nullptr) {
            cout << "Job not found!" << endl;
            return;
        }
        Queue<DT>* current = front;
        Queue<DT>* prev = nullptr;
        while (current != nullptr && positions > 0) {
            if (current->JobPointer->priority < temp->JobPointer->priority) {
                break;
            }
            prev = current;
            current = current->next;
            positions--;
        }
        if (prev == nullptr) {
            temp->next = front;
            front = temp;
        } else {
            prev->next = temp;
            temp->next = current;
        }
    }

    NovelQueue<DT>* reorder(int attribute_index) {
        NovelQueue<DT>* reorderedQueue = new NovelQueue<DT>();
        Queue<DT>* temp = front;
        while (temp != nullptr) {
            reorderedQueue->enqueue(*temp->JobPointer);
            temp = temp->next;
        }
        switch (attribute_index) {
            case 1:
                reorderedQueue->sortPriority();
                break;
            case 2:
                reorderedQueue->sortJobType();
                break;
            case 3:
                reorderedQueue->sortCPUTime();
                break;
            case 4:
                reorderedQueue->sortMemory();
                break;
            default:
                cout << "Invalid attribute index!" << endl;
                break;
        }
        return reorderedQueue;
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
        Queue<DT>* temp = front;
        while (temp != nullptr) {
            cout << temp->JobPointer->job_id << " ";
            temp = temp->next;
        }
        cout << endl;
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

// Read each command Process 

for (int i = 0; i < n; ++i) {

    cin >> command; // Read the command type
    switch (command) {

    case 'A': { // Add (Enqueue)
        cin >> job_id >> priority >> job_type;
        cin >> cpu_time_consumed >> memory_consumed;
        CPUJob* newJob = new CPUJob(job_id, priority, job_type,
        cpu_time_consumed, memory_consumed);
        (*myNovelQueue).enqueue(newJob);
        break;
    }
    case 'R': { // Remove (Dequeue)
        CPUJob* removedJob = (*myNovelQueue).dequeue();
        if (removedJob) {
        cout << "Dequeued Job: ";
        (*removedJob).display();
        delete removedJob; // Clean up memory after use
        }
        break;
    }
    case 'M': { // Modify
        cin >> job_id >> new_priority >> new_job_type;
        cin >> new_cpu_time_consumed >> new_memory_consumed;
        (*myNovelQueue).modify(job_id, new_priority, new_job_type,
        new_cpu_time_consumed, new_memory_consumed);
        break;
    }
    case 'C': { // Change Job Values
        cin >> job_id >> field_index >> new_value;
        (*myNovelQueue).change(job_id, field_index, new_value);
        break;
    }
    case 'P': { // Promote
        cin >> job_id >> positions;
        (*myNovelQueue).promote(job_id, positions);
        break;
    }
    case 'O': { // Reorder
        cin >> attribute_index;
        NovelQueue<CPUJob*>* reorderedQueue =
        (*myNovelQueue).reorder(attribute_index);
        cout << "Reordered Queue:" << endl;
        (*reorderedQueue).display();
        break;
    }
    case 'D': { // Display
        (*myNovelQueue).display();
        break;
    }
    case 'N': { // Count
        cout << "Number of elements in the queue: " <<
        (*myNovelQueue).count() << endl;
        break;
    }
    case 'L': { // List Jobs
        (*myNovelQueue).listJobs();
        break;
    }
        default:
            cout << "Invalid command!" << endl;
        }
    }
    delete myNovelQueue; // Clean up the NovelQueue after all operations
    return 0;
}
