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

Numbers are in complement of two

Files And What They Do:

- SystemArchitecture.hpp & SystemArchitecture.cpp - Setup types for the architecture, enums for better organization of registers, instructions, and important definitions for the project.
 
- InputAdressingTypes.hpp & InputAdressingTypes.cpp - Defines structs for better inputs of the adressing types, using a base struct for the parameters of the adressing types, and especializing based on what type is being used. 

- Memory.hpp & Memory.cpp - Defines the memory and how to acess, it's a singleton;

- Registers.hpp & Registers.cpp - Defines the registers and how to acess, it's a singleton; (not finished)(pc, indexs)

- FunctionalUnit.hpp & FunctionalUnit.cpp - The functional unit of the system, will recive de data and return data, it will be generic and other class called AdressingTypes will implement how to treat diferent adressint types, it's a singleton; (not finished)(operations, flags)

- InstructionDispatcher.hpp & InstructionDispatcher.cpp - Dispaches instructions retrivin values from registers and calling the functional unit, it's a singleton (not finished)

- AdressingTypesFactory.hpp & AdressingTypesFactory.cpp - factory for creating the adressing types accordingly to what is going to be used, all the parameters are a struct pointer that will be converted to the type that we want that is a specialized struct, so that we override functions and not overload for better flow, it's a factory and a singleton.

- AdressingTypes.hpp & AdressingTypes.cpp - This is the base class of the adpaters that will treat the adressing types, it will define the functions that the adressing types has to implement, they will call the functional unit, get the values and store the values accordingly, decode and encode instructions, it's a adapter (not finished) (Ld and Jp instructions not done)
  - Direct.hpp & Direct.cpp - (not implemented)
  - Implicit.hpp & Implicit.cpp - (not implemented)
  - Indexed.hpp & Indexed.cpp - (not implemented)
  - InddirectRegister.hpp & InddirectRegister.cpp - (not implemented)
  - Direct.hpp & Direct.cpp - (not finished)(Ld and Jp instructions not done)

- Processor.hpp & Processor.cpp - Wrapper of all functions of the processor, interface that will interact with the processor, recive program and store it into memory, call next instruction, halt, reset memory and registers, it's a singleton (not finished)