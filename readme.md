# System Programing

This project will implemente the operations of the architecture Z80.

# How To Build

```bash

cmake -S . -B build
cmake --build build

```

# How To Run

```bash

build/App programs/<arquivo.asm>

```

# Tips For Extending This Project

User header guards please, use the enums and struct already defined, and if need create new ones repeting the style of the already implemented.


# Implementation


## Commom Files

- SystemArchitecture.hpp & SystemArchitecture.cpp - Setup types for the architecture, enums for better organization of registers, instructions, and important definitions for the project.


## Instructions

| opcode | 0......n ( Registers or Imediate Values in Differente words )

The first word is the opcode of the instruction, decode it to know how many more words would have to be writen, it changes depending on the adressin type.

All the files are inside de instructions folder in the src and include folder, they must use the factory to get a instance of the class that you want and use the functionalities implemented.

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

All the files are inside the assembler folder in the src and include folder, all the files regarding the function should be inside these folders, they must not communicate with outside implementations, that's the function of the facade, the file Assembler.cpp and Assembler.hpp.

- Lexer.hpp & Lexer.cpp - Responsible for reading the .asm source file and converting raw text into a flat list of tokens. Strips comments (`;`), trims whitespace, and classifies each word into one of the token types: `LABEL`, `MNEMONIC`, `REGISTER_8B`, `REGISTER_16B`, `REGISTER_ESP`, `NUMBER`, `SYMBOL`, `INDIRECT`, `COMMA`, or `END_OF_FILE`. Handles indirect addressing tokens such as `(HL)`, `(IX+5)`, and `(IY-3)` by collecting fragments until the closing `)` is found. Numbers support decimal, hexadecimal (`0xFF`), and binary (`0b1010`) formats. (Done)

- Parser.hpp & Parser.cpp - Receives the token list from the Lexer and produces a list of `ParsedLine` structs, one per source line. Groups tokens by line number, identifies optional labels, dispatches to specialized handlers per mnemonic, and handles the full variety of `LD` formats through a dedicated `ParseLD` method. Resolves indirect operands (`(HL)`, `(IX+d)`, `(IY+d)`, direct address) via `ParseIndirect`. Stores unresolved label references (`JP LOOP`) as a symbol name in `symbolRef` with `hasSymbol = true`, to be resolved in Pass Two. Provides `GetInstructionSize` as a static helper used by the assembler to advance the location counter during Pass One. (Done)

- Assembler.hpp & Assembler.cpp - Facade and entry point for the two-pass assembly process. Receives the path to a `.asm` file and returns the encoded binary as a `vector<Word>` ready to be loaded into the processor.

  - **Pass One** (`PassOne`): Iterates over all `ParsedLine` entries and maintains a location counter starting at `0x0000`. Each time a label is found, its name and current address are stored in `symbolTable`. The counter advances by the byte size of each instruction as reported by `Parser::GetInstructionSize`. Duplicate labels are recorded as errors.

  - **Pass Two** (`PassTwo`): Iterates over the `ParsedLine` list again. For each line with `hasSymbol = true`, looks up `symbolRef` in `symbolTable` and writes the resolved address into `value`. Converts each resolved `ParsedLine` into a `DecodedInstruction` via `ToDecodedInstruction`, which maps the addressing mode and operands to the fields expected by the encoding layer.

  - **Encoding**: For each `DecodedInstruction`, retrieves the appropriate `AdressingTypesInstructions` encoder from `AdressingTypesFactoryInstructions` and calls `EncodeInstruction`, appending the resulting bytes to the output program. Exposes `symbolTable` and `errors` as public members for debug output. (Done)