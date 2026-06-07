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


## Commom Files

- SystemArchitecture.hpp & SystemArchitecture.cpp - Setup types for the architecture, enums for better organization of registers, instructions, and important definitions for the project.


## Instructions

| opcode | 0......n ( Registers or Imediate Values in Differente words )

The first word is the opcode of the instruction, decode it to know how many more words would have to be writen, it changes depending on the adressin type.

the folder instructions contains the files to encode to binary all the instructions of all adressing types.

- AdressingTypesFactoryInstructions.hpp & AdressingTypesFactoryInstructions.cpp - factory for creating the adressing types instructions accordingly to what is going to be used it's a factory and a singleton. (Done)

- AdressingTypesInstructions.hpp & AdressingTypesInstructions.cpp - This is the base class of the adpaters that will treat the adressing types for encoding the instructions outside the processor, it will define the functions that the adressing types has to implement, it's a adapter (Done)
  - DirectInstructions.hpp & DirectInstructions.cpp - (Done)
  - ImplicitInstructions.hpp & ImplicitInstructions.cpp - (Done)
  - IndexedInstructions.hpp & IndexedInstructions.cpp - (Done)
  - IndirectRegisterInstructions.hpp & IndirectRegisterInstructions.cpp - (Done)
  - ImediateInstructions.hpp & ImediateInstructions.cpp - (Done)

## Processor

Word of 8 bits

Adress size of 16 bits

Memory size of 64KB

Numbers are in complement of two

All the files are inside de processor folder in the src and include folder, all the files regrading the function should the inside these folders, they must not communicate with outside implementations, thats the function of the facade, the file processor.cpp and processor.hpp.

Files And What They Do:

- InputAdressingTypes.hpp & InputAdressingTypes.cpp - Defines structs for better inputs of the adressing types, using a base struct for the parameters of the adressing types, and especializing based on what type is being used. 

- Memory.hpp & Memory.cpp - Defines the memory and how to acess, it's a singleton; (Done)

- Registers.hpp & Registers.cpp - Defines the registers and how to acess, it's a singleton; (Done)

- FunctionalUnit.hpp & FunctionalUnit.cpp - The functional unit of the system, will recive de data and return data, it will be generic and other class called AdressingTypes will implement how to treat diferent adressint types, it's a singleton; (Done)

- AdressingTypesFactory.hpp & AdressingTypesFactory.cpp - factory for creating the adressing types accordingly to what is going to be used, all the parameters are a struct pointer that will be converted to the type that we want that is a specialized struct, so that we override functions and not overload for better flow, it's a factory and a singleton. (Done)

- AdressingTypes.hpp & AdressingTypes.cpp - This is the base class of the adpaters that will treat the adressing types, it will define the functions that the adressing types has to implement, they will call the functional unit, get the values and store the values accordingly, decode instructions, it's a adapter (Done)
  - Direct.hpp & Direct.cpp - (Done)
  - Implicit.hpp & Implicit.cpp - (Done)
  - Indexed.hpp & Indexed.cpp - (Done)
  - IndirectRegister.hpp & IndirectRegister.cpp - (Done)
  - Imediate.hpp & Imediate.cpp - (Done)

- Processor.hpp & Processor.cpp - Wrapper of all functions of the processor, interface that will interact with the processor, recive program and store it into memory, call next instruction, halt, reset memory and registers, it's a singleton and a facade (Done)

## Two Pass Assembler