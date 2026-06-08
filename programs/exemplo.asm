; Programa de exemplo — etapa 2
; Carrega valores nos registradores, faz operacoes e para

        NOP              ; nenhuma operacao
        LD A, 10         ; A = 10
        LD B, 5          ; B = 5
        ADD B            ; A = A + B  (A vira 15)
        DEC B            ; B = B - 1  (B vira 4)
        HLT              ; para a execucao
