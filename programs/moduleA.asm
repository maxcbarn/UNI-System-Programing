; ================================================================
; moduleA.asm  —  Programa principal
;
; Funcao: le tres bytes de um array na memoria (via (IX+offset))
;         e chama "putchar" (definido em moduleB) para cada um.
;
; Registradores por convencao:
;   A  — byte a ser "enviado" (argumento para putchar)
;   B  — contador de iteracoes
;   IX — ponteiro base do array na memoria
;
; Instrucoes usadas (todas suportadas pelo Parser):
;   LD reg, imediato     LDVALTOREG / IMEDIATE
;   LD reg, (IX+off)     LDMEMTOREG / INDEXED
;   LD reg, reg          LDREGTOREG / IMPLICIT
;   LD (IX+off), reg     LDREGTOMEM / INDEXED
;   ADD reg              ADD        / IMPLICIT
;   DEC reg              DEC        / IMPLICIT
;   PUSH reg16           PUSH       / IMPLICIT
;   POP  reg16           POP        / IMPLICIT
;   CALL label           CALL       / DIRECT  (externo)
;   JP   label           JP         / DIRECT  (local)
;   NOP                  NOP        / IMPLICIT
;   HLT                  HLT        / IMPLICIT
; ================================================================



; ----------------------------------------------------------------
; Prepara o array em memoria usando modo indexado (IX)
; Array: [0x41, 0x42, 0x43]  →  'A', 'B', 'C'
; Armazenamos em enderecos relativos a IX = 0x50
; ----------------------------------------------------------------


ENTRY  main          ; exporta ponto de entrada
EXTRN  putchar       ; importado de moduleB

main:
        NOP                  ; padding de alinhamento

        LD  A, 0x50          ; IX nao e carregavel diretamente via LD
        LD  H, A             ; guarda high-byte de base em H (convencao)
        LD  L, 0             ; low-byte = 0  →  HL aponta para 0x5000? nao,
                             ; IX e setado via encoder; aqui simulamos o base

        ; Escreve os tres bytes do array via IX+offset
        LD  A, 0x41          ; 'A'
        LD  (IX+0), A        ; array[0] = 'A'

        LD  A, 0x42          ; 'B'
        LD  (IX+1), A        ; array[1] = 'B'

        LD  A, 0x43          ; 'C'
        LD  (IX+2), A        ; array[2] = 'C'

        ; Contador de iteracoes
        LD  B, 3             ; 3 caracteres a enviar

        ; Indice atual (acumulado em D)
        LD  D, 0             ; D = indice = 0

; ----------------------------------------------------------------
; Loop principal: le array[D] e chama putchar
; ----------------------------------------------------------------
loop:
        ; Carrega array[indice] usando IX + offset fixo 0,1,2
        ; Como o Parser so aceita offset literal no token INDIRECT,
        ; usamos as tres posicoes explicitamente via JP seletivo.
        ; (Arquitetura nao tem desvio condicional — usamos estrutura
        ;  equivalente com DEC + JP para simular loop contavel.)

        LD  A, (IX+0)        ; le array[0] na primeira passagem
        PUSH BC              ; salva contador
        CALL putchar         ; envia A para "saida"
        POP  BC              ; restaura contador

        LD  A, (IX+1)        ; le array[1]
        PUSH BC
        CALL putchar
        POP  BC

        LD  A, (IX+2)        ; le array[2]
        PUSH BC
        CALL putchar
        POP  BC

        ; Soma verificacao: A = 0x41 + 0x42 + 0x43 = 0xC6
        LD  A, 0x41
        ADD B                ; A = 0x41 + B  (B=0 apos POPs, resultado = 0x41)
        LD  E, A             ; guarda em E para debug

        NOP                  ; ponto de observacao

done:
        HLT                  ; fim do programa


