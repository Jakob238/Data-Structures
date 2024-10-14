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
    }
    
    int findJobIndex(int job_id) const
    {
        return 0;
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


    void enqueue(DT* job){
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

    DT* dequeue(){
        if (front == nullptr){
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

    void modify(int job_id, int new_priority, int new_job_type, int new_cpu_time, int new_mem) {
        int index = findJobIndex(job_id);
        if (index == -1) return;
        Queue<DT>* node = NodePtrs[index];
        node->JobPointer->priority = new_priority;
        node->JobPointer->job_type = new_job_type;
        node->JobPointer->cpu_time_consumed = new_cpu_time_consumed;
        node->JobPointer->memory_consumed = new_memory_consumed;
        updateNodePtrs();
    }

    void change(int job_id, int field_index, int new_value) {
        int index = findJobIndex(job_id);
        if (index == -1) return;
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
        int index = findJobIndex(job_id);
        if (index == -1) return;
        Queue<DT>* node = NodePtrs[index];
        if (index - positions < 0) {
            // Move to the front
            if (front != node) {
                Queue<DT>* temp = front;
                while (temp->next != node) {
                    temp = temp->next;
                }
                temp->next = node->next;
                node->next = front;
                front = node;
            }
        } else {
            // Move up by positions
            Queue<DT>* temp = front;
            for (int i = 0; i < index - positions - 1; ++i) {
                temp = temp->next;
            }
            Queue<DT>* prev = temp;
            temp = temp->next;
            prev->next = node->next;
            node->next = temp;
            prev->next = node;
        }
        updateNodePtrs();
    }

    NovelQueue<DT>* reorder(int attribute_index) {
        // Create a new NovelQueue
        NovelQueue<DT>* newQueue = new NovelQueue<DT>();
        // Copy the current queue to a vector
        vector<DT*> jobs;
        Queue<DT>* temp = front;
        while (temp != nullptr) {
            jobs.push_back(temp->JobPointer);
            temp = temp->next;
        }
        // Sort the vector based on the specified attribute
        sort(jobs.begin(), jobs.end(), [attribute_index](DT* a, DT* b) {
            switch (attribute_index) {
                case 1: return a->priority < b->priority;
                case 2: return a->job_type < b->job_type;
                case 3: return a->cpu_time_consumed < b->cpu_time_consumed;
                case 4: return a->memory_consumed < b->memory_consumed;
                default: return a->job_id < b->job_id;
            }
        });
        // Enqueue the sorted jobs into the new queue
        for (DT* job : jobs) {
            newQueue->enqueue(new DT(*job));
        }
        return newQueue;
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
