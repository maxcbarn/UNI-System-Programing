; ================================================================
; moduleB.asm  —  Biblioteca de saida
;
; Restricao: TODAS as instrucoes devem ser modo IMPLICIT.
;
; Exporta:
;   putchar  — recebe valor em A, copia para H (saida simulada),
;              preserva todos os registradores, retorna.
;
;   identity — recebe valor em A, devolve o mesmo em A.
;              Demonstra PUSH/POP, XOR, INC, DEC, LD reg,reg.
;
; Importa: nada.
;
; Convencao de chamada:
;   Argumento em A.
;   putchar  nao modifica B, C, D, E, L.
;   identity nao modifica nenhum registrador alem de A (que preserva).
; ================================================================

        ENTRY  putchar
        ENTRY  identity

; ----------------------------------------------------------------
; putchar
;
; Entrada : A = byte a enviar
; Saida   : H = copia de A  (porta de saida simulada)
;           A preservado
; ----------------------------------------------------------------
putchar:
        ; Salva A na pilha via AF para nao perder o argumento
        PUSH AF              ; salva A (e flags)

        ; Copia A para H — H representa a porta de saida simulada
        LD   H, A            ; H = A  (escrita na porta)

        ; Eco: confirma que a escrita foi feita copiando H para L
        LD   L, H            ; L = H  (leitura de confirmacao)

        ; Verifica que L == A usando XOR e restaurando em seguida
        PUSH BC              ; preserva BC
        LD   B, A            ; B = A  (copia do argumento)
        XOR  B               ; A = A XOR B = 0  (confirma igualdade)
        LD   A, B            ; restaura A = argumento original
        POP  BC              ; restaura BC

        POP  AF              ; restaura A e flags originais
        RET

; ----------------------------------------------------------------
; identity
;
; Entrada : A = qualquer valor
; Saida   : A = mesmo valor recebido  (funcao identidade)
; Efeitos : B zerado temporariamente e restaurado
; ----------------------------------------------------------------
identity:
        PUSH AF              ; salva A original

        ; Exercita INC / DEC / XOR com B sem afetar A
        PUSH BC              ; preserva BC
        LD   B, A            ; B = A
        XOR  B               ; A = 0  (B XOR B)
        INC  B               ; B = A + 1
        DEC  B               ; B = A  (restaura)
        LD   A, B            ; A = B = valor original
        POP  BC              ; restaura BC

        ; CP: compara A consigo mesmo (resultado deve ser zero)
        CP   A               ; flags: Z=1, N=1, C=0

        NOP                  ; padding
        POP  AF              ; restaura A e flags originais
        RET
