; ================================================================
; directProgram.asm — Modulo autonomo, 100% DIRECT.
;
; Nao depende de nenhum outro modulo (sem EXTRN). Demonstra que
; um programa inteiro pode ser construido apenas com:
;     LD  reg, (endereco)      ; leitura de memoria
;     LD  (endereco), reg      ; escrita em memoria
;     JP  label                ; desvio incondicional
;
; Logica: copia quatro valores de uma area de "entrada" para uma
; area de "saida" na memoria, usando um registrador diferente
; para cada copia (A, B, C, D), depois faz uma leitura de
; verificacao de volta (E) e termina em loop infinito (substituto
; de HLT, que e IMPLICIT e portanto proibido aqui).
;
; Layout de memoria (enderecos fixos, fora do segmento de codigo
; — sao constantes literais, entao NAO entram na relocTable):
;   0x0080 — entrada 1            0x0090 — saida 1 (copia de 0x0080)
;   0x0081 — entrada 2            0x0091 — saida 2 (copia de 0x0081)
;   0x0082 — entrada 3            0x0092 — saida 3 (copia de 0x0082)
;   0x0083 — entrada 4            0x0093 — saida 4 (copia de 0x0083)
;   0x0094 — verificacao (releitura de 0x0090)
;
; Como nao ha LD reg,imediato (IMEDIATE, proibido), os valores em
; 0x0080-0x0083 sao assumidos pre-inicializados pelo ambiente de
; teste antes da execucao.
;
; Unica entrada na relocTable esperada: o uso de "loop" em JP loop
; (referencia a label local -> relocacao interna). Nao ha
; entrada em externTable, pois nao ha EXTRN.
; ================================================================

        ENTRY  main

main:
        LD   A, (0x0080)      ; le entrada 1
        LD   (0x0090), A      ; grava saida 1

        LD   B, (0x0081)      ; le entrada 2
        LD   (0x0091), B      ; grava saida 2

        LD   C, (0x0082)      ; le entrada 3
        LD   (0x0092), C      ; grava saida 3

        LD   D, (0x0083)      ; le entrada 4
        LD   (0x0093), D      ; grava saida 4

        ; Verificacao: releitura de uma saida ja gravada
        LD   E, (0x0090)      ; E deve conter o mesmo valor de 0x0080
        LD   (0x0094), E      ; grava resultado de verificacao

        JP   halt             ; jump para não executar o codigo abaixo

        LD   C, (0x0082)      ; le entrada 3
        LD   (0x0092), C      ; grava saida 3

        LD   D, (0x0083)      ; le entrada 4
        LD   (0x0093), D      ; grava saida 4

        LD   E, (0x0090)      ; E deve conter o mesmo valor de 0x0080
        LD   (0x0094), E      ; grava resultado de verificacao



halt:
        HLT