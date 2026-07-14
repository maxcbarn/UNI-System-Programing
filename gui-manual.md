# Z80 Virtual Machine Simulator - GUI Manual

This manual explains how to use each feature of the interface, how to properly configure the processor, and how to interact with memory and registers during program execution.

## Overview

The GUI is divided into three main panels:
1. **Controls & Registers (Left Panel):** Used for loading programs, controlling execution flow, and monitoring processor state.
2. **Information and Status (Center Panel):** Displays system logs, error messages, and basic instructions.
3. **Memory (Right Panel):** A fully interactive hexadecimal viewer and editor for the system's RAM.

---

## Controls Panel

This section allows you to configure the Virtual Machine before running a program.

- **Program Path:** A text input field where you define the path to your Assembly file (e.g., `programs/directProgram.asm`). If you launch the simulator via the terminal with a path argument (`build/App programs/directProgram.asm`), this field is automatically pre-filled.
- **Addressing Mode:** A dropdown menu to select the addressing mode the processor will use. **Note:** Z80 instructions often rely on Direct addressing, so make sure to select `Direct` when running programs like `directProgram.asm`.
- **Absolute / Relocatable:** Radio buttons to select the linker mode. Absolute mode fixes the memory addresses exactly as specified, while Relocatable mode allows the linker to assign memory dynamically.
- **Load Program:** Compiles the assembly code, links the modules, and loads the final machine code into the simulator's memory. Always press this button after configuring the above fields.

### Execution Flow
- **Step:** Executes exactly one instruction per click. This is highly recommended for debugging and watching data flow through the registers.
- **Auto Run:** Starts executing the program automatically without requiring manual clicks. Click **Pause** to stop the execution at any time.
- **Speed (s/inst):** A slider to control the delay (in seconds) between each instruction when **Auto Run** is active. Range: `0.01s` to `2.00s`.
- **Reset VM:** Completely clears the memory and resets all registers to zero, halting any ongoing execution. Use this before loading a new program or restarting a test.

---

## Registers Panel

This section provides real-time monitoring of the Z80 processor's internal state.

- **8-bit Registers:** `A`, `B`, `C`, `D`, `E`, `H`, `L`. 
- **Flags Register (F):** Displays the status flags in real-time (`S` = Sign, `Z` = Zero, `H` = Half-Carry, `P` = Parity/Overflow, `N` = Add/Subtract, `C` = Carry).
- **16-bit Registers:**
  - **PC (Program Counter):** Points to the memory address of the next instruction to be executed.
  - **SP (Stack Pointer):** Points to the top of the stack.
  - **IX / IY:** Index registers used for offset addressing.

---

## Memory Panel (RAM Viewer & Editor)

This panel allows you to inspect and modify the entire 64KB memory space in real-time.

- **Go to:** Type a hexadecimal address (e.g., `80`) and press `Enter`. The memory table will instantly scroll down to show that specific memory block.
- **Edit RAM:** Allows you to manually insert test data into the memory. 
  - **Address:** The target memory address (in Hex).
  - **Value:** The data byte to insert (in Hex).
  - **Set:** Click to apply the value to the specified address.
- **Use Data Segment Offset (Checkbox):** 
  - **When checked:** The address you type in `Edit RAM` or `Go to` is treated as a Data Segment offset. The simulator will automatically add the size of the loaded program (`StartMemory`) to your address. This prevents you from accidentally overwriting your own compiled code and matches the processor's internal data protection logic. **Leave this checked** for standard program testing.
  - **When unchecked:** Accesses raw, physical memory addresses directly.