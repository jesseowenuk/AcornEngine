#ifndef MEM_API_H
#define MEM_API_H

#include "ALI_defines.h"

typedef enum MEM_Tag
{
    // Should be assigned one of the below,
    // If you don't want to use one of these 
    // create a new one
    MEM_TAG_UNKNOWN,
    MEM_TAG_ARRAY,
    MEM_TAG_DARRAY,
    MEM_TAG_DICTIONARY,
    MEM_TAG_QUEUE,
    MEM_TAG_BST,
    MEM_TAG_STRING,
    MEM_TAG_JOB,
    MEM_TAG_TEXTURE,
    MEM_TAG_MATERIAL_INSTANCE,
    MEM_TAG_RENDERER,
    MEM_TAG_TRANSFORM,
    MEM_TAG_ENTITY,
    MEM_TAG_ENTITY_NODE,
    MEM_TAG_SCENE,

    MEM_TAG_MAX_TAGS
} MEM_Tag;

// Initialise our memory system
void MEM_Init();

// And terminate our memory system
void MEM_Terminate();

// Function to allocate memory
void* MEM_Allocate(uint64 size, MEM_Tag tag);

// Function to free memory
void MEM_Free(void* block, uint64 size, MEM_Tag);

// Function to zero memory
void* MEM_Zero(void* block, uint64 size);

// Function to copy memory
void* MEM_Copy(void* desintation, const void* source, uint64 size);

// Function to set memory
void* MEM_Set(void* destination, int32 value, uint64 size);

// This function will enable us to print out sone usage statistics
char* MEM_GetUsageInfo();

#endif /* MEM_API_H */