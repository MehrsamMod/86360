# 86360 – PowerPC JIT for Xbox 360 Emulator

86360 is a **PowerPC Just-In-Time (JIT) compiler** written in C++.

It focuses on translating PowerPC instructions into x86-64 machine code using AsmJit.

---

Current Version:

<img src="images/c.png" alt="86360 Logo" width="459px">

WIP Version:

<img src="images/a.png" alt="86360 Logo" width="459px">

##  Current Features

- CPU state emulation:
  - General Purpose Registers (GPRs)
  - Floating Point Registers (FPRs)
  - PC, LR, CTR
  - Condition Register (CR)
  - XER, FPSCR, MSR

- Big-endian memory handling
- Instruction fetch and decoding
- Basic JIT code generation using AsmJit

---

##  Project Status

**Status:** Beta / Early Development

This JIT is not complete and does not yet support full Xbox 360 game execution.

---

##  Planned Features

- Full PowerPC instruction coverage
- Block-based JIT compilation
- Register allocation improvements
- Exception handling (DSI, ISI, etc.)
- VMX/AltiVec support
- Performance optimizations
- Advanced caching system

---

##  Build Information

- Language: C++
- Standard: C++20 (GNU C++2a)
- Compiler: TDM-GCC 9.2.0
- Platform: Windows 64-bit
- IDE: Dev-C++

---

##  Third-Party

This project uses **AsmJit**:

See `THIRD_PARTY_NOTICES.md` for license details.

---

##  Disclaimer

This project is for **educational and research purposes only**.  
It is not affiliated with Microsoft or Xbox.

---

##  Author

Created by MehrsamMod with ❤️
