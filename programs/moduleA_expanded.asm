; ================================================================
; moduleA.asm  —  Programa principal
;
; Restricao: TODAS as instrucoes devem ser modo IMPLICIT.
; Instrucoes permitidas:
;   NOP, HLT, RET
;   PUSH reg16, POP reg16
;   LD reg, reg
;   ADD reg, SUB reg, AND reg, OR reg, XOR reg, CP reg
;   INC reg, DEC reg
;
; Logica:
;   Constroi tres valores nos registradores usando XOR+INC,
;   processa cada um (simula chamada a putchar inline),
;   calcula a soma dos tres e verifica com SUB.
;
; Registradores:
;   A — acumulador / argumento corrente
;   B — primeiro valor  (= 3)
;   C — segundo valor   (= 5)
;   D — terceiro valor  (= 7)
;   E — resultado da soma (= 15)
;   H — saida simulada (eco do valor enviado)
;   L — confirmacao de eco
; ================================================================

        ENTRY  main
        EXTRN  putchar

main:
        NOP                  ; inicio do modulo

        ; ── Constroi primeiro valor em B (= 3) ──────────────
        XOR  A               ; A = 0  (XOR A,A sempre zero)
        INC  A               ; A = 1
        INC  A               ; A = 2
        INC  A               ; A = 3
        LD   B, A            ; B = 3

        ; ── Constroi segundo valor em C (= 5) ───────────────
        XOR  A               ; A = 0
        INC  A               ; A = 1
        INC  A               ; A = 2
        INC  A               ; A = 3
        INC  A               ; A = 4
        INC  A               ; A = 5
        LD   C, A            ; C = 5

        ; ── Constroi terceiro valor em D (= 7) ──────────────
        XOR  A               ; A = 0
        INC  A               ; A = 1
        INC  A               ; A = 2
        INC  A               ; A = 3
        INC  A               ; A = 4
        INC  A               ; A = 5
        INC  A               ; A = 6
        INC  A               ; A = 7
        LD   D, A            ; D = 7

        ; ── Processa B: simula putchar(B) ────────────────────
        LD   A, B            ; A = 3 (argumento)
        PUSH BC              ; preserva BC
        PUSH DE              ; preserva DE
        LD   H, A            ; H = A  (saida simulada, eco)
        LD   L, H            ; L = H  (confirmacao)
        POP  DE
        POP  BC

        ; ── Processa C: simula putchar(C) ────────────────────
        LD   A, C            ; A = 5
        PUSH BC
        PUSH DE
        LD   H, A
        LD   L, H
        POP  DE
        POP  BC

        ; ── Processa D: simula putchar(D) ────────────────────
        LD   A, D            ; A = 7
        PUSH BC
        PUSH DE
        LD   H, A
        LD   L, H
        POP  DE
        POP  BC

        ; ── Soma de verificacao: B + C + D = 3+5+7 = 15 ─────
        LD   A, B            ; A = 3
        ADD  C               ; A = 8
        ADD  D               ; A = 15
        LD   E, A            ; E = 15

        ; ── Confirma resultado com SUB ────────────────────────
        LD   H, A            ; H = 15
        SUB  H               ; A = 0  (15 - 15)
        LD   L, A            ; L = 0

        ; ── Exercita AND / OR / XOR ──────────────────────────
        LD   A, B            ; A = 3  (0b00000011)
        AND  C               ; A = 1  (3 AND 5)
        OR   D               ; A = 7  (1 OR 7)
        XOR  D               ; A = 0  (7 XOR 7)

        ; ── DEC regressivo ────────────────────────────────────
        LD   A, B            ; A = 3
        DEC  A               ; A = 2
        DEC  A               ; A = 1
        DEC  A               ; A = 0

        NOP                  ; ponto de observacao

done:
        HLT
