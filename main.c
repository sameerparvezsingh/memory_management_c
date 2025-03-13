/**
 * @file main.c
 * @brief Demonstration of Memory Management Utility
 */

 #include <stdio.h>
 #include "memory_manager.h"
 
 /**
  * @brief Example structure to demonstrate memory tracking
  */
 typedef struct {
     int id;
     char* name;
     double value;
 } ExampleStruct;
 
 /**
  * @brief Create an example structure
  * @param id Identifier for the structure
  * @return Pointer to created structure
  */
 ExampleStruct* create_example_struct(int id) {
     // Allocate structure with tracking
     ExampleStruct* example = ALLOCATE(
         sizeof(ExampleStruct), 
         MEMORY_TYPE_DYNAMIC
     );
 
     if (!example) {
         fprintf(stderr, "Failed to create structure\n");
         return NULL;
     }
 
     // Allocate name with tracking
     example->name = ALLOCATE(
         50 * sizeof(char), 
         MEMORY_TYPE_TEMPORARY
     );
 
     example->id = id;
     example->value = id * 3.14;
     snprintf(example->name, 50, "Example Structure %d", id);
 
     return example;
 }
 
 /**
  * @brief Free example structure and its components
  * @param example Structure to be freed
  */
 void free_example_struct(ExampleStruct* example) {
     if (example) {
         // Free name first
         DEALLOCATE(example->name);
         
         // Free structure
         DEALLOCATE(example);
     }
 }
 
 /**
  * @brief Main program demonstrating memory management
  * @return Exit status
  */
 int main(void) {
     // Initialize memory manager
     memory_manager_init();
 
     // Create multiple structures
     ExampleStruct* struct1 = create_example_struct(1);
     ExampleStruct* struct2 = create_example_struct(2);
 
     // Print structure details
     printf("Struct 1: ID=%d, Name=%s, Value=%f\n", 
            struct1->id, struct1->name, struct1->value);
     printf("Struct 2: ID=%d, Name=%s, Value=%f\n", 
            struct2->id, struct2->name, struct2->value);
 
     // Generate memory report
     generate_memory_report();
 
     // Free structures
     free_example_struct(struct1);
     free_example_struct(struct2);
 
     // Final memory report
     generate_memory_report();
 
     return 0;
 }