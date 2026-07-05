#include <iostream>
#include <iomanip>
#include <vector>   
#include <cstring>  
#include "cpu.h"
#include "memory.h"

// ?? PASTE THE DAY 13 FETCH FUNCTION RIGHT HERE:
// ============================================================================
// DAY 13: THE INSTRUCTION FETCH UNIT
// ============================================================================
// This function reads a 32-bit instruction word from Big-Endian memory using 
// the current Program Counter, then advances the PC forward by 4 bytes.
uint32_t fetchInstruction(CPUState& cpu, MemoryManager& mem) {
    // 1. Read the 32-bit instruction from memory at the current PC address
    uint32_t instruction = mem.read32(cpu.pc);
    
    // 2. Automatically advance the PC by 4 bytes for the next instruction cycle
    cpu.pc += 4;
    
    return instruction;
}

// ============================================================================
// DAY 14: THE INSTRUCTION DECODER STRUCTURE & TOOL
// ============================================================================
// This structure holds a 32-bit instruction broken down into readable fields.
struct DecodedInstruction {
    uint32_t raw;          // The original unbroken 32-bit instruction
    uint8_t  opcode;       // Primary operation identifier (Top 6 bits)
    uint8_t  rd;           // Target/Destination Register index (Next 5 bits)
    uint8_t  ra;           // Source Register A index (Next 5 bits)
    int16_t  immediate;    // The 16-bit constant/offset number (Lowest 16 bits)
};

// This function acts as the bit-slicer to break up the instruction
DecodedInstruction decodeInstruction(uint32_t rawValue) {
    DecodedInstruction inst;
    inst.raw       = rawValue;
    inst.opcode    = (rawValue >> 26) & 0x3F; // Shift right by 26, keep only 6 bits
    inst.rd        = (rawValue >> 21) & 0x1F; // Shift right by 21, keep only 5 bits
    inst.ra        = (rawValue >> 16) & 0x1F; // Shift right by 16, keep only 5 bits
    inst.immediate = rawValue & 0xFFFF;       // Keep only the bottom 16 bits
    return inst;
}

int main() {
    std::cout << "[Project Setup] Testing Days 2-7 CPU Register Core..." << std::endl;

    // 1. Initialize our CPUState structure
    CPUState cpu;
    cpu.reset(); // Safely zero out all registers to remove junk data

    // Day 5: Load mock control registers
    cpu.pc  = 0x0000000000401000; // Mock entry address
    cpu.lr  = 0x00000000004055A4; // Mock return function pointer
    cpu.ctr = 5;                  // Mock loop counter countdown value

    // Day 6: Load mock condition and integer exception registers
    cpu.cr  = 0x80000000;         // Mock negative condition active
    cpu.xer = 0x20000000;         // Mock exception tracking carry flag

    // Day 7: Load mock floating-point status and machine status registers
    cpu.fpscr = 0x00000000;       // Mock healthy status (no math errors)
    cpu.msr   = 0x00000030;       // Mock user privilege execution mode

    // Day 3: Load sample values into General Purpose Registers (GPRs)
    cpu.gpr[0] = 0x123456789ABCDEF0; 
    cpu.gpr[3] = 42;                 
    cpu.gpr[31] = 0xDEADBEEFCAFEBABE; 

    // Day 4: Load sample values into Floating-Point Registers (FPRs)
    cpu.fpr[0].d = 3.14159;          
    cpu.fpr[1].d = -98.6;            
    cpu.fpr[31].d = 12345.6789;      

    // Day 2-7 Verification: Print the complete CPU state dashboard to the screen
    cpu.dumpState();

    // 2. Day 8: Initialize our 4 Megabyte virtual memory manager
    MemoryManager mem(4 * 1024 * 1024); 
    std::cout << "\n[Day 8] Virtual Memory initialized with: " 
              << (mem.getSize() / (1024 * 1024)) << " MB" << std::endl;

    // 3. Days 9-10: Big-Endian Read & Write Verification Tracker
    std::cout << "\n=== [TESTING DAYS 9-10: BIG-ENDIAN MEMORY] ===" << std::endl;

    // Day 10 Test: Write a 32-bit hex number into memory address 0x1000
    uint32_t testValue = 0x12345678;
    mem.write32(0x1000, testValue); 
    std::cout << "[Day 10] Wrote 32-bit value: 0x" << std::hex << testValue << std::dec << std::endl;

    // Day 9 Test: Read it back using our custom Big-Endian reader
    uint32_t returnedValue = mem.read32(0x1000);
    std::cout << "[Day 9] Read 32-bit value:  0x" << std::hex << returnedValue << std::dec << std::endl;

    // The Ultimate Proof: Check the raw bytes stored sequentially on your host PC.
    // In Big-Endian layout, the most important byte (0x12) must sit first in memory!
    std::cout << "[Proof] Inspecting raw physical bytes at address 0x1000:" << std::hex << std::endl;
    std::cout << "        Byte at 0x1000: 0x" << (int)mem.readRawByte(0x1000) << " (Expected: 12)" << std::endl;
    std::cout << "        Byte at 0x1001: 0x" << (int)mem.readRawByte(0x1001) << " (Expected: 34)" << std::endl;
    std::cout << "        Byte at 0x1002: 0x" << (int)mem.readRawByte(0x1002) << " (Expected: 56)" << std::endl;
    std::cout << "        Byte at 0x1003: 0x" << (int)mem.readRawByte(0x1003) << " (Expected: 78)" << std::endl;
    
    std::cout << std::dec << "\n[+] All Core Architecture Tests Complete. Clean Exit." << std::endl;
    
    std::cout << "\n=== [TESTING DAYS 11-12: VIRTUAL MEMORY TRANSLATION] ===" << std::endl;

    // Day 12 Test: Try to translate a valid address into a real pointer
    uint32_t validAddr = 0x2000;
    uint8_t* hostPointer = mem.translate(validAddr);
    
    if (hostPointer != nullptr) {
        std::cout << "[Day 12] Address 0x" << std::hex << validAddr 
                  << " successfully translated to a valid host pointer!" << std::dec << std::endl;
    }

    // Day 11 Test: Try to trigger a virtual memory exception with an invalid address
    uint32_t invalidAddr = 0x99999999; // Way past our 4MB limit
    std::cout << "[Day 11] Testing safety check with out-of-bounds address 0x" << std::hex << invalidAddr << std::dec << "..." << std::endl;
    
    mem.translate(invalidAddr); // This should print our memory exception message instead of crashing

    // ============================================================================
    // DAY 13: TESTING THE INSTRUCTION FETCH UNIT
    // ============================================================================
    std::cout << "\n=== [DAY 13: INSTRUCTION FETCH UNIT] ===" << std::endl;

    // Set our CPU's Program Counter to a clean starting address in memory
    cpu.pc = 0x5000;

    // Inject a real 32-bit instruction word (0x38600064) at that address
    mem.write32(cpu.pc, 0x38600064); 
    std::cout << "[Day 13] Injected mock instruction 0x38600064 at memory address 0x5000" << std::endl;

    // Execute our permanent Fetch Unit function!
    uint32_t currentOpcode = fetchInstruction(cpu, mem);

    // Print out the results to verify the memory read and the PC increment worked perfectly
    std::cout << "[Day 13] Fetched Opcode Word        : 0x" << std::hex << currentOpcode << std::dec << std::endl;
    std::cout << "[Day 13] Updated CPU Program Counter: 0x" << std::hex << cpu.pc << std::dec << std::endl;

    // ============================================================================
    // DAY 14: TESTING THE INSTRUCTION DECODER
    // ============================================================================
    std::cout << "\n=== [DAY 14: INSTRUCTION DECODER] ===" << std::endl;

    // Run our freshly fetched opcode through our bit-slicer function!
    DecodedInstruction decoded = decodeInstruction(currentOpcode);

    // Print out the sliced bits to prove our decoder maps things correctly!
    std::cout << "[Day 14] Sliced Opcode Number : " << std::dec << (int)decoded.opcode << std::endl;
    std::cout << "[Day 14] Target Register (rd) : r" << (int)decoded.rd << std::endl;
    std::cout << "[Day 14] Source Register (ra) : r" << (int)decoded.ra << std::endl;
    std::cout << "[Day 14] Constant/Immediate   : " << decoded.immediate << std::endl;
    
    // ============================================================================
    // DAY 15: OPCODE DISPATCH TABLE
    // ============================================================================
    std::cout << "\n=== [DAY 15: OPCODE DISPATCH TABLE] ===" << std::endl;

    // Route the instruction based on its primary opcode number
    switch (decoded.opcode) {
        case 14: // Opcode 14 represents the Add Immediate (addi) instruction
            std::cout << "[Day 15] Routing Match: Opcode 14 -> addi (Add Immediate)" << std::endl;
            std::cout << "         Instruction Details: Place (r" << (int)decoded.ra 
                      << " + " << decoded.immediate << ") into register r" << (int)decoded.rd << std::endl;
            break;

        default:
            std::cout << "[Day 15] Unknown Opcode (" << (int)decoded.opcode 
                      << ") detected! Forwarding to Exception Handler." << std::endl;
            break;
    }

    return 0;
}
