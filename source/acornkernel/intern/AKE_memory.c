#include "AKE_memory.h"

#include "LOG_log.h"

#include "PLT_platform.h"

// TODO: Custom string lib
#include <stdio.h>
#include <string.h>

struct AKE_MEMORY_Stats
{
    uint64 totalAllocated;
    uint64 taggedAllocations[AKE_MEMORY_TAG_MAX_TAGS];
} AKE_MEMORY_Stats;

static const char* AKE_MEMORY_TagStrings[AKE_MEMORY_TAG_MAX_TAGS] = 
{
    "UNKNOWN          ",
    "ARRAY            ",
    "DARRAY           ",
    "DICTIONARY       ",
    "QUEUE            ",
    "BST              ",
    "STRING           ",
    "JOB              ",
    "TEXTURE          ",
    "MATERIAL_INSTANCE",
    "RENDERER         ",
    "TRANSFORM        ",
    "ENTITY           ",
    "ENTITY_NODE      ",
    "SCENE            "
};

static struct AKE_MEMORY_Stats memoryStats;

// Initialise our memory system
void AKE_MEMORY_Init()
{
    PLT_ZeroMemory(&memoryStats, sizeof(memoryStats));
}

// And terminate our memory system
void AKE_MEMORY_Terminate()
{

}

// Function to allocate memory
void* AKE_MEMORY_Allocate(uint64 size, AKE_MEMORY_Tag tag)
{
    if(tag == AKE_MEMORY_TAG_UNKNOWN)
    {
        LOG_WARN("Called AKE_MEORY_Allocate using the AKE_MEMORY_TAG_UNKNOWN. Please reclassify this allocation.");
    }

    memoryStats.totalAllocated += size;
    memoryStats.taggedAllocations[tag] += size;

    // TODO: Memory alignment
    void* block = PLT_AllocateMemory(size, FALSE);
    PLT_ZeroMemory(block, size);
    return block;
}

// Function to free memory
void AKE_MEMORY_Free(void* block, uint64 size, AKE_MEMORY_Tag tag)
{
    if(tag == AKE_MEMORY_TAG_UNKNOWN)
    {
        LOG_WARN("Called AKE_MEORY_Allocate using the AKE_MEMORY_TAG_UNKNOWN. Please reclassify this allocation.");
    }

    // TODO: Memory alignment
    memoryStats.totalAllocated -= size;
    memoryStats.taggedAllocations[tag] -= size;
    PLT_FreeMemory(block, FALSE);
}

// Function to zero memory
void* AKE_MEMORY_Zero(void* block, uint64 size)
{
    return PLT_ZeroMemory(block, size);
}

// Function to copy memory
void* AKE_MEMORY_Copy(void* desintation, const void* source, uint64 size)
{
    return PLT_CopyMemory(desintation, source, size);
}

// Function to set memory
void* AKE_MEMORY_Set(void* destination, int32 value, uint64 size)
{
    return PLT_SetMemory(destination, value, size);
}

// This function will enable us to print out sone usage statistics
char* AKE_MEMORY_GetUsageInfo()
{
    const uint64 GiB = 1024 * 1024 * 1024;
    const uint64 MiB = 1024 * 1024;
    const uint64 KiB = 1024;

    char buffer[8000] = "System memory use (tagged):\n";
    uint64 offset = strlen(buffer);

    for(uint32 i = 0; i < AKE_MEMORY_TAG_MAX_TAGS; ++i)
    {
        char unit[4] = "XiB";
        float amount = 1.0f;

        // Which bit category should we be in?
        if(memoryStats.taggedAllocations[i] >= GiB)
        {
            unit[0] = 'G';
            amount = memoryStats.taggedAllocations[i] / (float)GiB;
        }
        else if(memoryStats.taggedAllocations[i] >= MiB)
        {
            unit[0] = 'M';
            amount = memoryStats.taggedAllocations[i] / (float)MiB;
        }
        else if(memoryStats.taggedAllocations[i] >= KiB)
        {
            unit[0] = 'K';
            amount = memoryStats.taggedAllocations[i] / (float)KiB;
        }
        else
        {
            unit[0] = 'B';
            unit[1] = 0;
            amount = memoryStats.taggedAllocations[i];
        }
        
        int32 length = snprintf(buffer + offset, 8000, " %s: %.2f%s\n", AKE_MEMORY_TagStrings[i], amount, unit);
        offset += length;
    }

    char* memoryUsageString = _strdup(buffer);

    return memoryUsageString;
}