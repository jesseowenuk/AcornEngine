#ifndef AKE_MEMORY_H
#define AKE_MEMORY_H

#include "ALI_defines.h"

typedef enum AKE_MEMORY_Tag
{
    // Should be assigned one of the below,
    // If you don't want to use one of these 
    // create a new one
    AKE_MEMORY_TAG_UNKNOWN,
    AKE_MEMORY_TAG_ARRAY,
    AKE_MEMORY_TAG_DARRAY,
    AKE_MEMORY_TAG_DICTIONARY,
    AKE_MEMORY_TAG_QUEUE,
    AKE_MEMORY_TAG_BST,
    AKE_MEMORY_TAG_STRING,
    AKE_MEMORY_TAG_JOB,
    AKE_MEMORY_TAG_TEXTURE,
    AKE_MEMORY_TAG_MATERIAL_INSTANCE,
    AKE_MEMORY_TAG_RENDERER,
    AKE_MEMORY_TAG_TRANSFORM,
    AKE_MEMORY_TAG_ENTITY,
    AKE_MEMORY_TAG_ENTITY_NODE,
    AKE_MEMORY_TAG_SCENE,

    AKE_MEMORY_TAG_MAX_TAGS
} AKE_MEMORY_Tag;

// Initialise our memory system
void AKE_MEMORY_Init();

// And terminate our memory system
void AKE_MEMORY_Terminate();

// Function to allocate memory
void* AKE_MEMORY_Allocate(uint64 size, AKE_MEMORY_Tag tag);

// Function to free memory
void AKE_MEMORY_Free(void* block, uint64 size, AKE_MEMORY_Tag);

// Function to zero memory
void* AKE_MEMORY_Zero(void* block, uint64 size);

// Function to copy memory
void* AKE_MEMORY_Copy(void* desintation, const void* source, uint64 size);

// Function to set memory
void* AKE_MEMORY_Set(void* destination, int32 value, uint64 size);

// This function will enable us to print out sone usage statistics
char* AKE_MEMORY_GetUsageInfo();

#endif /* AKE_MEMORY_H */