// Emirhan Tuygun
// Emirhan Cebiroğlu
// Şeyma Handekli
// Efe Dortluğlu

#include <stdio.h>    // Standard input/output library for file operations and printing.
#include <stdlib.h>   // Standard library for memory allocation, process control, conversions, etc.
#include <string.h>   // Library for string handling functions.

#define MAX_PROCESSES 100          // Maximum number of processes the program can handle.
#define MAX_LINE_LENGTH 100        // Maximum length of a line in the input file.
#define TOTAL_RAM 2048             // Total available RAM.
#define RESERVED_RAM_FOR_PRIORITY_0 512  // RAM reserved for processes with priority 0.
#define CPU_LIMIT 100              // Total CPU capacity.

typedef struct {
    char id[10];        // Process ID, a string with a maximum length of 9 characters plus null terminator.
    int arrival_time;   // Arrival time of the process.
    int priority;       // Priority of the process (0 to 3).
    int burst_time;     // Burst time of the process (execution time).
    int ram;            // RAM required by the process.
    int cpu;            // CPU required by the process.
} Process;              // Definition of a Process structure to hold process information.

Process processes[MAX_PROCESSES];           // Array to store processes.
int process_count = 0;                      // Counter to keep track of the number of processes read from the file.

int available_ram_priority_0 = RESERVED_RAM_FOR_PRIORITY_0;            // RAM available for priority 0 processes.
int available_ram_other = TOTAL_RAM - RESERVED_RAM_FOR_PRIORITY_0;     // RAM available for other priority processes.
int available_cpu = CPU_LIMIT;                            // Available CPU capacity.

void log_to_file(const char* message) {             // Function to log a message to a file.
    FILE* file = fopen("output.txt", "a");          // Open the output file in append mode.
    if (!file) {                                    // Check if the file was opened successfully.
        perror("Could not open output file");       // Print error message if the file could not be opened.
        exit(EXIT_FAILURE);                         // Exit the program with failure status.
    }
    fprintf(file, "%s\n", message);           // Write the message to the file, followed by a newline.
    fclose(file);                             // Close the output file.
}

void log_process_queues() {                 // Function to log the current state of process queues.
    char log_message[MAX_LINE_LENGTH];      // Buffer to store the log message.

    strcpy(log_message, "CPU-1 que1 (priority-0) (FCFS) → ");       // Initialize the log message for priority 0 processes.
    for (int i = 0; i < process_count; i++) {                       // Loop through all processes.
        if (processes[i].priority == 0) {                           // Check if the process has priority 0.
            strcat(log_message, processes[i].id);                   // Append the process ID to the log message.
            strcat(log_message, "-");                               // Append a dash to separate process IDs.
        }
    }
    log_message[strlen(log_message) - 1] = '\0';          // Remove the last dash by setting it to the null character.
    printf("%s\n", log_message);                          // Print the log message to the console.

    strcpy(log_message, "CPU-2 que2 (priority-1) (SJF) → ");      // Initialize the log message for priority 1 processes.
    for (int i = 0; i < process_count; i++) {                     // Loop through all processes.
        if (processes[i].priority == 1) {                         // Check if the process has priority 1.
            strcat(log_message, processes[i].id);                 // Append the process ID to the log message.
            strcat(log_message, "-");                             // Append a dash to separate process IDs.
        }
    }
    log_message[strlen(log_message) - 1] = '\0';           // Remove the last dash by setting it to the null character.
    printf("%s\n", log_message);                           // Print the log message to the console.

    strcpy(log_message, "CPU-2 que3 (priority-2) (RR-q8) → ");       // Initialize the log message for priority 2 processes.
    for (int i = 0; i < process_count; i++) {                        // Loop through all processes.
        if (processes[i].priority == 2) {                            // Check if the process has priority 2.
            strcat(log_message, processes[i].id);                    // Append the process ID to the log message.
            strcat(log_message, "-");                                // Append a dash to separate process IDs.
        }
    }
    log_message[strlen(log_message) - 1] = '\0';          // Remove the last dash by setting it to the null character.
    printf("%s\n", log_message);                          // Print the log message to the console.

    strcpy(log_message, "CPU-2 que4 (priority-3) (RR-q16) → ");         // Initialize the log message for priority 3 processes.
    for (int i = 0; i < process_count; i++) {                           // Loop through all processes.
        if (processes[i].priority == 3) {                               // Check if the process has priority 3.
            strcat(log_message, processes[i].id);                       // Append the process ID to the log message.
            strcat(log_message, "-");                                   // Append a dash to separate process IDs.
        }
    }
    log_message[strlen(log_message) - 1] = '\0';           // Remove the last dash by setting it to the null character.
    printf("%s\n", log_message);                           // Print the log message to the console.
}

int check_resources(int priority, int required_ram, int required_cpu) {         // Function to check if resources are available for a process.
    if (priority == 0) {                                                        // Check if the process has priority 0.
        return (required_ram <= available_ram_priority_0 && required_cpu <= available_cpu);    // Return true if both RAM and CPU requirements can be met for priority 0.
    } else {                                                                                   // For processes with other priorities.
        return (required_ram <= available_ram_other && required_cpu <= available_cpu);         // Return true if both RAM and CPU requirements can be met for non-priority 0.
    }
}

void allocate_resources(int priority, int required_ram, int required_cpu) {      // Function to allocate resources to a process.
    if (priority == 0) {                                                         // Check if the process has priority 0.
        available_ram_priority_0 -= required_ram;          // Deduct the required RAM from the available RAM for priority 0.
    } else {                                               // For processes with other priorities.
        available_ram_other -= required_ram;               // Deduct the required RAM from the available RAM for non-priority 0.
    }
    available_cpu -= required_cpu;                         // Deduct the required CPU from the available CPU.
}

void release_resources(int priority, int released_ram, int released_cpu) {       // Function to release resources back to the pool after a process finishes.
    if (priority == 0) {                                 // Check if the process has priority 0.
        available_ram_priority_0 += released_ram;        // Add the released RAM back to the available RAM for priority 0.
    } else {                                             // For processes with other priorities.
        available_ram_other += released_ram;             // Add the released RAM back to the available RAM for non-priority 0.
    }
    available_cpu += released_cpu;                       // Add the released CPU back to the available CPU.
}

void fcfs(Process* queue, int count) {            // Function to handle processes using the First-Come, First-Served (FCFS) scheduling algorithm.
    for (int i = 0; i < count; i++) {             // Iterate over each process in the queue.
        char log_message[MAX_LINE_LENGTH];        // Buffer for log messages.

        if (!check_resources(queue[i].priority, queue[i].ram, queue[i].cpu)) {      // Check if there are sufficient resources for the current process.
            snprintf(log_message, sizeof(log_message), "Process %s cannot be assigned to CPU-1 due to insufficient resources.", queue[i].id);  // Prepare a log message indicating insufficient resources.
            log_to_file(log_message);             // Log the message to the output file.
            printf("%s\n", log_message);          // Print the log message to the console.
            continue;                             // Skip to the next process.
        }

        snprintf(log_message, sizeof(log_message), "Process %s is queued to be assigned to CPU-1.", queue[i].id);  // Prepare a log message indicating the process is queued.
        log_to_file(log_message);        // Log the message to the output file.
        printf("%s\n", log_message);     // Print the log message to the console.

        allocate_resources(queue[i].priority, queue[i].ram, queue[i].cpu);  // Allocate resources for the current process.

        snprintf(log_message, sizeof(log_message), "Process %s is assigned to CPU-1.", queue[i].id);  // Prepare a log message indicating the process is assigned to CPU-1.
        log_to_file(log_message);        // Log the message to the output file.
        printf("%s\n", log_message);     // Print the log message to the console.

        snprintf(log_message, sizeof(log_message), "Process %s is completed and terminated.", queue[i].id);  // Prepare a log message indicating the process is completed and terminated.
        log_to_file(log_message);         // Log the message to the output file.
        printf("%s\n", log_message);      // Print the log message to the console.

        release_resources(queue[i].priority, queue[i].ram, queue[i].cpu);  // Release resources used by the current process.
    }
}

Process* sort_by_burst_time(Process* queue, int count) {             // Function to sort processes by their burst time using bubble sort.
    Process* sorted = (Process*)malloc(count * sizeof(Process));     // Allocate memory for the sorted array of processes.
    memcpy(sorted, queue, count * sizeof(Process));          // Copy the original queue into the sorted array.
    for (int i = 0; i < count - 1; i++) {                    // Outer loop for bubble sort, iterates over the entire array.
        for (int j = 0; j < count - i - 1; j++) {            // Inner loop for bubble sort, performs the comparison and swapping.
            if (sorted[j].burst_time > sorted[j + 1].burst_time) {       // Compare the burst times of adjacent processes.
                Process temp = sorted[j];         // Temporary variable to hold one process during the swap.
                sorted[j] = sorted[j + 1];        // Swap the processes if the current one has a greater burst time.
                sorted[j + 1] = temp;             // Complete the swap.
            }
        }
    }
    return sorted;  // Return the sorted array.
}

void sjf(Process* queue, int count) {
    Process* sorted_queue = sort_by_burst_time(queue, count);         // Sort the queue by burst time using the sort_by_burst_time function.

    for (int i = 0; i < count; i++) {                // Iterate over each process in the sorted queue.
        char log_message[MAX_LINE_LENGTH];           // Buffer for creating log messages.

        if (!check_resources(sorted_queue[i].priority, sorted_queue[i].ram, sorted_queue[i].cpu)) {  // Check if there are enough resources for the current process.
            snprintf(log_message, sizeof(log_message), "Process %s cannot be assigned to CPU-2 due to insufficient resources.", sorted_queue[i].id);  // Create a log message if resources are insufficient.
            log_to_file(log_message);              // Log the message to a file.
            printf("%s\n", log_message);           // Print the message to the console.
            continue;             // Skip to the next process if resources are insufficient.
        }

        snprintf(log_message, sizeof(log_message), "Process %s is placed in the que1 queue to be assigned to CPU-2.", sorted_queue[i].id);  // Log message for placing the process in the queue.
        log_to_file(log_message);             // Log the message to a file.
        printf("%s\n", log_message);          // Print the message to the console.

        allocate_resources(sorted_queue[i].priority, sorted_queue[i].ram, sorted_queue[i].cpu);  // Allocate the necessary resources for the process.

        snprintf(log_message, sizeof(log_message), "Process %s is assigned to CPU-2.", sorted_queue[i].id);  // Log message for assigning the process to CPU-2.
        log_to_file(log_message);         // Log the message to a file.
        printf("%s\n", log_message);      // Print the message to the console.

        snprintf(log_message, sizeof(log_message), "The operation of process %s is completed and terminated.", sorted_queue[i].id);  // Log message for process completion and termination.
        log_to_file(log_message);         // Log the message to a file.
        printf("%s\n", log_message);      // Print the message to the console.

        release_resources(sorted_queue[i].priority, sorted_queue[i].ram, sorted_queue[i].cpu);  // Release the resources used by the process.
    }
    free(sorted_queue);       // Free the memory allocated for the sorted queue.
}

void round_robin(Process* queue, int count, int quantum) {
    int* remaining_burst_time = (int*)malloc(count * sizeof(int));     // Allocate memory for an array to store remaining burst times for each process.
    for (int i = 0; i < count; i++) {
        remaining_burst_time[i] = queue[i].burst_time;                 // Initialize the remaining burst time for each process to its burst time.
    }

    int time = 0;          // Initialize the time to 0.
    while (1) {            // Continue looping indefinitely.
        int done = 1;      // Flag to indicate if all processes are done.

        for (int i = 0; i < count; i++) {           // Iterate over each process in the queue.
            if (remaining_burst_time[i] > 0) {      // Check if the process still has burst time remaining.
                done = 0;              // Set the done flag to 0 indicating that there are still processes to be executed.

                if (!check_resources(queue[i].priority, queue[i].ram, queue[i].cpu)) {     // Check if there are enough resources for the current process.
                    char log_message[MAX_LINE_LENGTH];     // Buffer for creating log messages.
                    snprintf(log_message, sizeof(log_message), "Process %s cannot be assigned to CPU-2 due to insufficient resources.", queue[i].id);  // Create a log message if resources are insufficient.
                    log_to_file(log_message);          // Log the message to a file.
                    printf("%s\n", log_message);       // Print the message to the console.
                    continue;                          // Skip to the next process if resources are insufficient.
                }

                char log_message[MAX_LINE_LENGTH];       // Buffer for creating log messages.

                snprintf(log_message, sizeof(log_message), "Process %s is placed in the que2 queue to be assigned to CPU-2.", queue[i].id);  // Log message for placing the process in the queue.
                log_to_file(log_message);           // Log the message to a file.
                printf("%s\n", log_message);        // Print the message to the console.

                allocate_resources(queue[i].priority, queue[i].ram, queue[i].cpu);  // Allocate the necessary resources for the process.

                snprintf(log_message, sizeof(log_message), "Process %s is assigned to CPU-2.", queue[i].id);  // Log message for assigning the process to CPU-2.
                log_to_file(log_message);              // Log the message to a file.
                printf("%s\n", log_message);           // Print the message to the console.

                if (remaining_burst_time[i] > quantum) {       // Check if the remaining burst time is greater than the quantum.
                    time += quantum;                           // Increment the time by the quantum.
                    remaining_burst_time[i] -= quantum;        // Decrement the remaining burst time by the quantum.

                    snprintf(log_message, sizeof(log_message), "Process %s ran for %d seconds and is queued again because the process is not completed.", queue[i].id, quantum);  // Log message for process running and queuing again.
                    log_to_file(log_message);             // Log the message to a file.
                    printf("%s\n", log_message);          // Print the message to the console.

                    release_resources(queue[i].priority, queue[i].ram, queue[i].cpu);  // Release the resources used by the process.
                } else {
                    time += remaining_burst_time[i];        // Increment the time by the remaining burst time.
                    snprintf(log_message, sizeof(log_message), "Process %s ran for %d seconds, its operation is completed and terminated.", queue[i].id, remaining_burst_time[i]);  // Log message for process completion and termination.
                    log_to_file(log_message);               // Log the message to a file.
                    printf("%s\n", log_message);            // Print the message to the console.

                    remaining_burst_time[i] = 0;            // Set the remaining burst time to 0 as the process is completed.

                    release_resources(queue[i].priority, queue[i].ram, queue[i].cpu);      // Release the resources used by the process.
                }
            }
        }

        if (done) break;       // If all processes are done, exit the loop.
    }

    free(remaining_burst_time);      // Free the memory allocated for the remaining burst time array.
}

int main() {

}
