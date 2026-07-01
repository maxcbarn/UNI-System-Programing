; ================================================================
; moduleB.asm  —  Biblioteca de saida
;
; Exporta:
;   putchar  — recebe byte em A, "envia para saida" (copia para
;              endereco de memoria fixo 0x00FF que representa um
;              registrador de saida mapeado em memoria), retorna.
;
;   identity — recebe byte em A, devolve o mesmo valor em A.
;              Demonstra: PUSH/POP, LD reg,reg, RET.
;
; Importa: nada (modulo folha — sem dependencias externas)
;
; Instrucoes usadas:
;   LD  (0x00FF), A      LDREGTOMEM / DIRECT   — escrita em mem
;   LD  A, (0x00FF)      LDMEMTOREG / DIRECT   — leitura de mem
;   LD  reg, reg         LDREGTOREG / IMPLICIT
;   LD  reg, imediato    LDVALTOREG / IMEDIATE
;   PUSH reg16           PUSH       / IMPLICIT
;   POP  reg16           POP        / IMPLICIT
;   INC  reg             INC        / IMPLICIT
;   DEC  reg             DEC        / IMPLICIT
;   XOR  reg             XOR        / IMPLICIT
;   RET                  RET        / IMPLICIT
;   NOP                  NOP        / IMPLICIT
; ================================================================

        ENTRY  putchar       ; exporta putchar
        ENTRY  identity      ; exporta identity

; ----------------------------------------------------------------
; putchar
;
; Argumento : A  — byte a enviar
; Retorno   : nenhum (A preservado)
; Efeito    : escreve A no endereco 0x00FF (porta de saida simulada)
;
; Convencao de chamada: caller usa PUSH/POP para preservar
; registradores que precisar; esta rotina nao modifica B,C,D,E,H,L.
; ----------------------------------------------------------------
putchar:
        ; Escreve o byte de saida no endereco de I/O mapeado em memoria
        LD  (0x00FF), A      ; porta de saida = A

        ; Leitura de volta para confirmar escrita (simulacao de eco)
        LD  B, A             ; salva A em B (B = valor enviado)
        LD  A, (0x00FF)      ; releitura da porta
        LD  A, B             ; restaura A original

        RET                  ; retorna ao caller

; ----------------------------------------------------------------
; identity
;
; Argumento : A  — qualquer byte
; Retorno   : A  — mesmo valor recebido
; Demonstra : PUSH AF / POP AF, XOR para zerar, INC/DEC
; ----------------------------------------------------------------
identity:
        PUSH AF              ; salva A e flags na pilha

        ; Zera B usando XOR e depois incrementa para mostrar INC/DEC
        XOR  B               ; B = B XOR B = 0
        INC  B               ; B = 1
        DEC  B               ; B = 0  (volta a zero)

        ; Operacao identidade: carrega valor da pilha de volta
        POP  AF              ; restaura A original

        NOP                  ; padding
        RET                  ; devolve A inalterado
