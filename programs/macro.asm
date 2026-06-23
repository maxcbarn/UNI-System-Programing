; Test with macros

; --- Simple macro: loads value immediately into register ---
LOAD_IMM MACRO &REG, &VAL
        LD &REG, &VAL
ENDM

; --- Macro with a call to another macro inside (nested call)---
INIT_REGS MACRO &V1, &V2
        LOAD_IMM A, &V1
        LOAD_IMM B, &V2
ENDM

; --- A macro that contains an internal macro definition (nested macro). ---
OUTER MACRO &X
        INNER MACRO &Y
                LD A, &Y
        ENDM
        LD H, &X
        INNER &X
ENDM


; Program

        INIT_REGS 10, 20     ; LD A,10  LD B,20

        OUTER 5              ; INNER, LD H,5, LD A,5

        LOAD_IMM L, 100      ; LD L, 100
        LD (HL), A

        LOAD_IMM L, 101      ; LD L, 101
        LD (HL), B
        HLT