; Program with different addressing modes
; Sums a series of numbers stored in memory

        LD H, 0          ; H = 0 (Immediate)
        LD L, 100        ; HL = 100 (Immediate)
        LD A, 5          ; A = 5 (Immediate)
        LD (HL), A       ; Mem[100] = 5 (Indirect Register)

        INC L            ; HL = 101 (Implicit)
        LD A, 8          ; A = 8 (Immediate)
        LD (HL), A       ; Mem[101] = 8 (Indirect Register)

        INC L            ; HL = 102 (Implicit)
        LD A, 3          ; A = 3 (Immediate)
        LD (HL), A       ; Mem[102] = 3 (Indirect Register)

        LD L, 100        ; HL = 100 (Immediate)
        XOR A            ; A = 0 (Implicit)

        LD C, (HL)       ; C = Mem[100] (Indirect Register)
        ADD A, C         ; A = A + C (Implicit)
        INC L            ; HL = 101 (Implicit)

        LD C, (HL)       ; C = Mem[101] (Indirect Register)
        ADD A, C         ; A = A + C (Implicit)
        INC L            ; HL = 102 (Implicit)

        LD C, (HL)       ; C = Mem[102] (Indirect Register)
        ADD A, C         ; A = A + C (Implicit)

        LD L, 200        ; HL = 200 (Immediate)
        LD (HL), A       ; Mem[200] = A (Indirect Register)
        HLT              ; Halts execution (Implicit)
