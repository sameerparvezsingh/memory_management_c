/**
 * @file memory_manager.c
 * @brief Memory Management Utility Implementation
 */

 #include "memory_manager.h"

 // Global memory tracker
 static MemoryTracker g_memory_tracker = {0};
 
 // Internal utility functions
 static uint64_t get_current_timestamp(void) {
     static uint64_t timestamp = 0;
     return ++timestamp;
 }
 
 static int find_available_slot(void) {
     for (size_t i = 0; i < MAX_TRACKED_BLOCKS; i++) {
         if (g_memory_tracker.blocks[i].pointer == NULL) {
             return i;
         }
     }
     return -1;
 }
 
 void memory_manager_init(void) {
     memset(&g_memory_tracker, 0, sizeof(MemoryTracker));
 }
 
 void* safe_memory_allocate(
     size_t size, 
     const char* filename, 
     int line_number, 
     MemoryAllocationType type
 ) {
     // Validation checks
     if (size == 0) {
         fprintf(stderr, "WARNING: Zero-byte allocation\n");
         return NULL;
     }
 
     if (g_memory_tracker.current_block_count >= MAX_TRACKED_BLOCKS) {
         fprintf(stderr, "ERROR: Memory tracker full\n");
         return NULL;
     }
 
     // Allocate memory
     void* memory = malloc(size);
     if (!memory) {
         fprintf(
             stderr, 
             "CRITICAL: Allocation failed at %s:%d\n", 
             filename, 
             line_number
         );
         return NULL;
     }
 
     // Find tracking slot
     int slot = find_available_slot();
     if (slot == -1) {
         free(memory);
         return NULL;
     }
 
     // Populate memory block
     MemoryBlock* block = &g_memory_tracker.blocks[slot];
     block->pointer = memory;
     block->size = size;
     block->type = type;
     block->status = MEMORY_STATUS_ALLOCATED;
     block->line_number = line_number;
     block->timestamp = get_current_timestamp();
 
     strncpy(block->filename, filename, MAX_FILENAME_LENGTH - 1);
     block->filename[MAX_FILENAME_LENGTH - 1] = '\0';
 
     // Update tracker
     g_memory_tracker.current_block_count++;
     g_memory_tracker.total_allocated_memory += size;
 
     return memory;
 }
 
 void safe_memory_free(
     void* memory, 
     const char* filename, 
     int line_number
 ) {
     if (!memory) {
         fprintf(
             stderr, 
             "WARNING: Freeing NULL pointer at %s:%d\n", 
             filename, 
             line_number
         );
         return;
     }
 
     // Find and update memory block
     for (size_t i = 0; i < MAX_TRACKED_BLOCKS; i++) {
         MemoryBlock* block = &g_memory_tracker.blocks[i];
         
         if (block->pointer == memory) {
             // Update tracker
             g_memory_tracker.total_allocated_memory -= block->size;
             g_memory_tracker.current_block_count--;
 
             block->status = MEMORY_STATUS_FREED;
             free(memory);
             
             // Clear block
             memset(block, 0, sizeof(MemoryBlock));
             return;
         }
     }
 
     // Untracked memory
     fprintf(
         stderr, 
         "WARNING: Untracked memory free at %s:%d\n", 
         filename, 
         line_number
     );
     free(memory);
 }
 
 void generate_memory_report(void) {
     printf("\n--- MEMORY ALLOCATION REPORT ---\n");
     printf("Total Blocks: %zu\n", g_memory_tracker.current_block_count);
     printf("Total Allocated: %zu bytes\n", 
            g_memory_tracker.total_allocated_memory);
 
     for (size_t i = 0; i < MAX_TRACKED_BLOCKS; i++) {
         MemoryBlock* block = &g_memory_tracker.blocks[i];
         
         if (block->pointer) {
             printf(
                 "Block %zu: %p, %zu bytes, Type: %d, Status: %d\n",
                 i, block->pointer, block->size, 
                 block->type, block->status
             );
         }
     }
 }
 
 size_t get_total_allocated_memory(void) {
     return g_memory_tracker.total_allocated_memory;
 }
 
 size_t get_current_block_count(void) {
     return g_memory_tracker.current_block_count;
 }