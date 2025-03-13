/**
 * @file memory_management_tool.c
 * @brief A robust memory management utility with tracking and reporting
 * @author Sameer Parvez Singh
 * @date March 13, 2025
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <stdbool.h>
 
 // Constant definitions for improved readability
 #define MAX_FILENAME_LENGTH 256
 #define MEMORY_TRACKING_ENABLED 1
 
 /**
  * @enum MemoryAllocationType
  * @brief Defines different types of memory allocations
  */
 typedef enum {
     MEMORY_TYPE_STATIC,
     MEMORY_TYPE_DYNAMIC,
     MEMORY_TYPE_TEMPORARY
 } MemoryAllocationType;
 
 /**
  * @struct MemoryBlock
  * @brief Represents a tracked memory allocation
  */
 typedef struct MemoryBlock {
     void* pointer;              // Actual memory pointer
     size_t size;                // Size of allocated memory
     char filename[MAX_FILENAME_LENGTH];  // Source file of allocation
     int line_number;            // Line number of allocation
     MemoryAllocationType type;  // Type of memory allocation
     struct MemoryBlock* next;   // Pointer to next memory block
 } MemoryBlock;
 
 // Global memory tracking structure
 static MemoryBlock* g_memory_tracker = NULL;
 
 /**
  * @brief Safely allocates memory with enhanced tracking
  * @param size Size of memory to allocate
  * @param filename Source file name
  * @param line_number Source line number
  * @param type Memory allocation type
  * @return Pointer to allocated memory
  */
 void* safe_memory_allocate(
     size_t size, 
     const char* filename, 
     int line_number, 
     MemoryAllocationType type
 ) {
     // Validate allocation size
     if (size == 0) {
         fprintf(stderr, "Warning: Attempting to allocate zero bytes\n");
         return NULL;
     }
 
     // Attempt memory allocation
     void* allocated_memory = malloc(size);
     if (allocated_memory == NULL) {
         fprintf(
             stderr, 
             "CRITICAL: Memory allocation failed at %s:%d\n", 
             filename, 
             line_number
         );
         exit(EXIT_FAILURE);
     }
 
     // Create memory tracking block
     #if MEMORY_TRACKING_ENABLED
     MemoryBlock* tracking_block = malloc(sizeof(MemoryBlock));
     if (tracking_block == NULL) {
         free(allocated_memory);
         fprintf(
             stderr, 
             "CRITICAL: Tracking block allocation failed\n"
         );
         exit(EXIT_FAILURE);
     }
 
     // Populate tracking information
     tracking_block->pointer = allocated_memory;
     tracking_block->size = size;
     strncpy(
         tracking_block->filename, 
         filename, 
         MAX_FILENAME_LENGTH - 1
     );
     tracking_block->line_number = line_number;
     tracking_block->type = type;
     
     // Link to global tracker
     tracking_block->next = g_memory_tracker;
     g_memory_tracker = tracking_block;
     #endif
 
     return allocated_memory;
 }
 
 /**
  * @brief Safely frees allocated memory
  * @param memory Pointer to memory to be freed
  * @param filename Source file name
  * @param line_number Source line number
  */
 void safe_memory_free(
     void* memory, 
     const char* filename, 
     int line_number
 ) {
     // Validate input
     if (memory == NULL) {
         fprintf(
             stderr, 
             "Warning: Attempting to free NULL pointer at %s:%d\n", 
             filename, 
             line_number
         );
         return;
     }
 
     #if MEMORY_TRACKING_ENABLED
     // Find and remove from tracking
     MemoryBlock* current = g_memory_tracker;
     MemoryBlock* previous = NULL;
 
     while (current != NULL) {
         if (current->pointer == memory) {
             // Unlink from tracking list
             if (previous == NULL) {
                 g_memory_tracker = current->next;
             } else {
                 previous->next = current->next;
             }
 
             // Free tracking block
             free(current);
             break;
         }
         previous = current;
         current = current->next;
     }
     #endif
 
     // Actually free the memory
     free(memory);
 }
 
 /**
  * @brief Generates comprehensive memory usage report
  */
 void generate_memory_report(void) {
     printf("\n--- MEMORY ALLOCATION REPORT ---\n");
     
     MemoryBlock* current = g_memory_tracker;
     size_t total_allocated = 0;
     int block_count = 0;
 
     while (current != NULL) {
         printf(
             "Memory Block: \n"
             "  Pointer: %p\n"
             "  Size: %zu bytes\n"
             "  Allocated in: %s:%d\n"
             "  Type: %d\n\n",
             current->pointer, 
             current->size, 
             current->filename, 
             current->line_number,
             current->type
         );
         
         total_allocated += current->size;
         block_count++;
         current = current->next;
     }
 
     printf(
         "Summary:\n"
         "  Total Blocks: %d\n"
         "  Total Memory Allocated: %zu bytes\n", 
         block_count, 
         total_allocated
     );
 }
 
 // Macro definitions for convenient usage
 #define ALLOCATE(size, type) \
     safe_memory_allocate(size, __FILE__, __LINE__, type)
 #define DEALLOCATE(ptr) \
     safe_memory_free(ptr, __FILE__, __LINE__)
 
 /**
  * @brief Demonstration of memory management tool
  * @return Exit status
  */
 int main(void) {
     // Demonstrate memory allocation and tracking
     int* dynamic_array = ALLOCATE(
         5 * sizeof(int), 
         MEMORY_TYPE_DYNAMIC
     );
     
     char* string_buffer = ALLOCATE(
         50 * sizeof(char), 
         MEMORY_TYPE_TEMPORARY
     );
 
     // Populate data
     for (int i = 0; i < 5; i++) {
         dynamic_array[i] = i * 10;
     }
     strcpy(string_buffer, "Memory Management Demonstration");
 
     // Generate initial report
     generate_memory_report();
 
     // Free allocated memory
     DEALLOCATE(dynamic_array);
     DEALLOCATE(string_buffer);
 
     // Final memory report
     generate_memory_report();
 
     return EXIT_SUCCESS;
 }