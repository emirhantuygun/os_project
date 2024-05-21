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



int main() {

}
