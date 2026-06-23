; Test with macros

; --- Simple macro: loads value immediately into register ---

; --- Macro with a call to another macro inside (nested call)---

; --- A macro that contains an internal macro definition (nested macro). ---


; Program

        LD A, 10
        LD B, 20

        LD H, 5
                LD A, 5

        LD L, 100
        LD (HL), A

        LD L, 101
        LD (HL), B
        HLT
