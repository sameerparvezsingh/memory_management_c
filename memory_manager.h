/**
 * @file memory_manager.h
 * @brief Advanced Memory Management Utility Header
 * @author [Your Name]
 * @date [Current Date]
 */

 #ifndef MEMORY_MANAGER_H
 #define MEMORY_MANAGER_H
 
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <stdbool.h>
 #include <stdint.h>
 
 // Configuration Constants
 #define MAX_FILENAME_LENGTH 256
 #define MAX_TRACKED_BLOCKS 1000
 #define MEMORY_TRACKING_ENABLED 1
 
 // Memory Allocation Types
 typedef enum {
     MEMORY_TYPE_STATIC,     // Compile-time allocated memory
     MEMORY_TYPE_DYNAMIC,    // Runtime heap allocation
     MEMORY_TYPE_TEMPORARY,  // Short-lived allocations
     MEMORY_TYPE_PERSISTENT  // Long-lived allocations
 } MemoryAllocationType;
 
 // Memory Block Status
 typedef enum {
     MEMORY_STATUS_ALLOCATED,
     MEMORY_STATUS_FREED,
     MEMORY_STATUS_CORRUPTED
 } MemoryStatus;
 
 // Memory Block Tracking Structure
 typedef struct {
     void* pointer;              // Memory address
     size_t size;                // Allocated memory size
     char filename[MAX_FILENAME_LENGTH];  // Source file
     int line_number;            // Line number of allocation
     MemoryAllocationType type;  // Allocation category
     MemoryStatus status;        // Current block status
     uint64_t timestamp;         // Allocation timestamp
 } MemoryBlock;
 
 // Memory Tracker Structure
 typedef struct {
     MemoryBlock blocks[MAX_TRACKED_BLOCKS];
     size_t current_block_count;
     size_t total_allocated_memory;
 } MemoryTracker;
 
 /**
  * @brief Initialize memory tracking system
  */
 void memory_manager_init(void);
 
 /**
  * @brief Safely allocate memory with tracking
  * @param size Requested memory size
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
 );
 
 /**
  * @brief Safely deallocate memory
  * @param memory Pointer to memory to be freed
  * @param filename Source file name
  * @param line_number Source line number
  */
 void safe_memory_free(
     void* memory, 
     const char* filename, 
     int line_number
 );
 
 /**
  * @brief Generate comprehensive memory usage report
  */
 void generate_memory_report(void);
 
 /**
  * @brief Get total allocated memory
  * @return Total bytes allocated
  */
 size_t get_total_allocated_memory(void);
 
 /**
  * @brief Get current number of tracked memory blocks
  * @return Number of tracked memory blocks
  */
 size_t get_current_block_count(void);
 
 // Convenient macro definitions
 #define ALLOCATE(size, type) \
     safe_memory_allocate(size, __FILE__, __LINE__, type)
 #define DEALLOCATE(ptr) \
     safe_memory_free(ptr, __FILE__, __LINE__)
 
 #endif // MEMORY_MANAGER_H