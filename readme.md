# System Programing

This project will implemente the operations of the architecture Z80.

# How To Build

```bash

cmake -S . -B build
cmake --build build

```

# How To Run

```bash

build/App

```

# Tips For Extending This Project

User header guards please, use the enums and struct already defined, and if need create new ones repeting the style of the already implemented.

# Implementation

Word of 8 bits

Adress size of 16 bits

Memory size of 64KB

Numbers are in complement of two (need to work on that)

Files And What They Do:

- SystemArchitecture.hpp & SystemArchitecture.cpp - Setup types for the architecture, enums for better organization of registers and important definitions for the project.
- Memory.hpp & Memory.cpp - Defines the memory and how to acess, it's a singleton;
- Registers.hpp & Registers.cpp - Defines the registers and how to acess, it's a singleton; (not finished)(flags, indexs)(Max)
- FunctionalUnit.hpp & FunctionalUnit.cpp - The functional unit of the system, will recive de data and return data, it will be generic and other class called AdressingTypes will implement how to treat diferent adressint types, it's a singleton; (not finished)(operations, flags)(Max)
- InstructionDispatcher.hpp & InstructionDispatcher.cpp - Dispaches instructions retrivin values from registers and calling the functional unit, it's a singleton (not finished)(Max)
- AdressingTypes.hpp & AdressingTypes.cpp - This is the base class of the adpaters that will treat the adressing types, it's propurse is to recive the instructions in any adressing type and translate to instruction in the architeture using the accumulator, it's a adapter (not finished)(Max)
- Processor.hpp & Processor.cpp - Wrapper of all functions of the processor, interface that will interact with the processor, it's a singleton (not finished)(Max)