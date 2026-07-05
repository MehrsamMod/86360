#ifndef MEMORY_H
#define MEMORY_H

#include <vector>
#include <cstdint>
#include <cstring>  // ?? MAKE SURE THIS LINE IS HERE FOR MEMCPY!
#include <iostream>

// ============================================================================
// DAY 8: CREATE THE MEMORY MANAGER
// ============================================================================
class MemoryManager {
private:
    std::vector<uint8_t> ram; 
    uint32_t ramSize;         

    // Days 9-10 Utilities: Fast hardware byte-flippers
    inline uint16_t swap16(uint16_t val) { return __builtin_bswap16(val); } 
    inline uint32_t swap32(uint32_t val) { return __builtin_bswap32(val); } 

public: // ?? Make sure your private area is done here!

    // ?? MAKE SURE THIS EXACT LINE IS INSIDE YOUR PUBLIC SECTION:
    MemoryManager(uint32_t size = 16 * 1024 * 1024) : ramSize(size) {
        ram.resize(ramSize, 0);
    }


    // Day 8 Helper: Returns the size of our allocated memory
    uint32_t getSize() const {
        return ramSize;
    }


    // Day 8 Extension: Read a raw byte directly from the allocation vector
    uint8_t readRawByte(uint32_t address) const {
        if (address < ramSize) {
            return ram[address];
        }
        return 0; 
    } // ?? Check if this closing bracket is missing!
    
    // Days 11 & 12: Virtual Memory Framework & Address Translation
    // This takes a virtual memory address and translates it to a real host RAM pointer.
    // If the address goes past our allocated space, it returns a nullptr (simulating a page fault).
    uint8_t* translate(uint32_t virtualAddress) {
        if (virtualAddress >= ramSize) {
            std::cout << "\n[!] Memory Exception: Page Fault or Alignment Error at 0x" 
                      << std::hex << virtualAddress << std::dec << std::endl;
            return nullptr;
        }
        // Direct mapping translation: return a pointer to the exact slot in our RAM vector
        return &ram[virtualAddress];
    }

    // Day 9: Implement Big-Endian Memory Reads
    uint16_t read16(uint32_t address) {
        if (address + 2 > ramSize) return 0; 
        uint16_t raw;
        std::memcpy(&raw, &ram[address], 2);
        return swap16(raw);
    }

    uint32_t read32(uint32_t address) {
        if (address + 4 > ramSize) return 0; // Simple out-of-bounds safety check
        uint32_t raw;
        // Copy the raw 4 bytes out of our RAM vector
        std::memcpy(&raw, &ram[address], 4);
        // Swap them because host PC is Little-Endian, but target is Big-Endian
        return swap32(raw);
    }

    // Day 10: Implement Big-Endian Memory Writes
    void write16(uint32_t address, uint16_t value) {
        if (address + 2 > ramSize) return;
        // Swap the value first into Big-Endian layout
        uint16_t swapped = swap16(value);
        // Write the bytes down into our RAM vector
        std::memcpy(&ram[address], &swapped, 2);
    }

    void write32(uint32_t address, uint32_t value) {
        if (address + 4 > ramSize) return;
        uint32_t swapped = swap32(value);
        std::memcpy(&ram[address], &swapped, 4);
    } // Closes write32
    
}; // Closes the MemoryManager class

#endif // Closes the header file guard

