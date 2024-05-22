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


int main() {

}
