#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <iostream>
#include <iomanip>

// ============================================================================
// DAY 4: FLOATING-POINT REGISTER REPRESENTATION
// ============================================================================
// We use a union so we can easily look at an FPR as a 64-bit floating-point 
// double (for math) OR as a raw 64-bit integer (for raw binary moves and JIT logic).
union FPR {
    double d;         // Decimal value view
    uint64_t raw;     // Raw binary bits view
};

// ============================================================================
// DAY 2: THE CPU STATE STRUCTURE
// ============================================================================
// We use alignas(16) to ensure the structure sits cleanly on 16-byte boundaries 
// in system RAM. This makes memory access incredibly fast for the host hardware.
struct alignas(16) CPUState {
    
    // Day 3: 32 General Purpose Registers (GPRs)
    // These hold your standard 64-bit integer numbers (r0 through r31).
    uint64_t gpr[32];

    // Day 4: 32 Floating-Point Registers (FPRs)
    // These hold your 64-bit decimal math values (f0 through f31).
    FPR fpr[32];
    
        // Day 5: Control Registers
    uint64_t pc;   // Program Counter: Points to the current instruction
    uint64_t lr;   // Link Register: Holds return addresses for function calls
    uint64_t ctr;  // Count Register: Used for loop counters and indirect jumps

    // Day 6: Condition and Exception Registers
    uint32_t cr;   // Condition Register: Holds comparison flags (LT, GT, EQ)
    uint32_t xer;  // Fixed-Point Exception Register: Tracks integer carries/overflows
    
    // Day 7: Status and Machine Registers
    uint32_t fpscr; // Floating-Point Status and Control Register
    uint32_t msr;   // Machine State Register (manages CPU privilege modes)


    // Reset helper to wipe the CPU back to clean zeros
    void reset() {
    	 pc = 0;
        lr = 0;
        ctr = 0;
        cr = 0;
        xer = 0;
        fpscr = 0;
        msr = 0;


        for (int i = 0; i < 32; ++i) {
            gpr[i] = 0;
            fpr[i].raw = 0; // Clears all binary bits instantly
        }
    };

    // A beautiful debug printer to inspect our register files in the console
    void dumpState() const {
    	std::cout << "\n--- DAY 5-6: CONTROL & STATUS REGISTERS ---" << std::endl;
        std::cout << "PC  : 0x" << std::hex << std::setw(16) << std::setfill('0') << pc << std::endl;
        std::cout << "LR  : 0x" << std::setw(16) << std::setfill('0') << lr << std::endl;
        std::cout << "CTR : 0x" << std::setw(16) << std::setfill('0') << ctr << std::endl;
        std::cout << "CR  : 0x" << std::setw(8) << std::setfill('0') << cr << std::endl;
        std::cout << "XER : 0x" << std::setw(8) << std::setfill('0') << xer << std::endl;
        std::cout << "FPSCR: 0x" << std::setw(8) << std::setfill('0') << fpscr << std::endl;
        std::cout << "MSR  : 0x" << std::setw(8) << std::setfill('0') << msr << std::endl;


        std::cout << "\n==============================================" << std::endl;
        std::cout << "          [DAYS 2-4: CPU REGISTER FILE]       " << std::endl;
        std::cout << "==============================================" << std::endl;
        
           std::cout << "\n--- DAY 3: GENERAL PURPOSE REGISTERS (GPRs) ---" << std::endl;
        std::cout << std::dec; // ?? FORCE THE ENTIRE LOOP COUNTER TO STAY IN DECIMAL
        for (int i = 0; i < 32; i += 4) {
            for (int j = 0; j < 4; ++j) {
                int rIdx = i + j;
                
                std::cout << "r" << std::setw(2) << std::setfill('0') << rIdx << ": 0x" 
                          << std::hex << std::setw(16) << std::setfill('0') << std::right << gpr[rIdx] << "  ";
            }
            std::cout << std::endl;
        }
        std::cout << std::dec; // Reset back to decimal just in case


              std::cout << "\n--- DAY 4: FLOATING-POINT REGISTERS (FPRs) ---" << std::endl;
        std::cout << std::fixed << std::setprecision(4); // Keep decimal places for double float math
        std::cout << std::dec; // ?? FORCE THE TRACKING COUNTER FIELDS TO STAY IN DECIMAL MODE
        
        for (int i = 0; i < 32; i += 4) {
            for (int j = 0; j < 4; ++j) {
                int fIdx = i + j;
                
                std::cout << "f" << std::setw(2) << std::setfill('0') << fIdx << ": " 
                          << std::setw(10) << std::setfill(' ') << std::right << fpr[fIdx].d << "  ";
            }
            std::cout << std::endl;
        }
        std::cout << std::dec; 
    } // Closes dumpState

}; // ?? MAKE SURE THIS HAS A SEMICOLON AT THE END!

#endif

