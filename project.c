#include <stdio.h>
#include <stdlib.h>

// Structure to represent a process
struct Process {
    int id;
    int priority;
    int burst_time;
    int remaining_time;
    int time_quantum;
};

// Constants
#define QUEUE1_SIZE 10
#define QUEUE2_SIZE 10
#define TIME_QUANTUM 2

// Queues
struct Process queue1[QUEUE1_SIZE];
int start1 = 0, end1 = 0;

struct Process queue2[QUEUE2_SIZE];
int start2 = 0, end2 = 0;

// Function to enqueue a process in Queue 1
void enqueueQueue1(struct Process p) {
    if (end1 == QUEUE1_SIZE) {
        printf("Queue 1 is full.\n");
        return;
    }
    queue1[end1++] = p;
}

// Function to enqueue a process in Queue 2
void enqueueQueue2(struct Process p) {
    if (end2 == QUEUE2_SIZE) {
        printf("Queue 2 is full.\n");
        return;
    }
    queue2[end2++] = p;
}

// Function to dequeue a process from Queue 1
struct Process dequeueQueue1() {
    if (start1 == end1) {
        printf("Queue 1 is empty.\n");
        exit(1);
    }
    struct Process p = queue1[start1];
    for (int i = start1; i < end1 - 1; i++) {
        queue1[i] = queue1[i + 1];
    }
    end1--;
    return p;
}

// Function to dequeue a process from Queue 2
struct Process dequeueQueue2() {
    if (start2 == end2) {
        printf("Queue 2 is empty.\n");
        exit(1);
    }
    struct Process p = queue2[start2];
    for (int i = start2; i < end2 - 1; i++) {
        queue2[i] = queue2[i + 1];
    }
    end2--;
    return p;
}

// Function to execute processes in Queue 1 (Fixed priority preemptive scheduling)
void executeQueue1() {
    if (start1 == end1) {
        return; // Queue 1 is empty
    }

    // Find the highest priority process in Queue 1
    int higher_priority = -1;
    int index = -1;
    for (int i = start1; i < end1; i++) {
        if (queue1[i].priority > higher_priority) {
            higher_priority = queue1[i].priority;
            index = i;
        }
    }

    struct Process current_process = dequeueQueue1();
    printf("Executing process P%d from Queue 1 with priority %d.\n", current_process.id, current_process.priority);
    current_process.remaining_time -= TIME_QUANTUM;

    if (current_process.remaining_time <= 0) {
        printf("Process P%d from Queue 1 has completed.\n", current_process.id);
    } else {
        enqueueQueue2(current_process); // Move to Queue 2
    }
}

// Function to execute processes in Queue 2 (Round Robin scheduling)
void executeQueue2() {
    if (start2 == end2) {
        return; // Queue 2 is empty
    }

    struct Process current_process = dequeueQueue2();
    printf("Executing process P%d from Queue 2.\n", current_process.id);
    current_process.remaining_time -= TIME_QUANTUM;

    if (current_process.remaining_time <= 0) {
        printf("Process P%d from Queue 2 has completed.\n", current_process.id);
    } else {
        enqueueQueue2(current_process); // Re-enqueue in Queue 2
    }
}

int main() {
    // Initialize processes (you can add more)
    struct Process processes[] = {
        {1, 0, 6, 6, 0},
        {2, 1, 4, 4, 0},
        {3, 2, 8, 8, 0},
    };
    int num_processes = sizeof(processes) / sizeof(struct Process);

    // Enqueue processes in Queue 1
    for (int i = 0; i < num_processes; i++) {
        enqueueQueue1(processes[i]);
    }

    while (start1 != end1 || start2 != end2) {
        if (start1 != end1) {
            executeQueue1(); // Execute processes in Queue 1
        } else {
            executeQueue2(); // Execute processes in Queue 2
        }
    }

    return 0;
}
