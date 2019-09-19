        PUBLIC calculaHigh

        SECTION .text : CODE (2)
        THUMB
        EXPORT calculaHigh       
        
GPIO_PORTK_AHB_DATA_BITS_R  EQU    0x400613fc

;R0 ->Retorno de contagens
;R1 ->Endereço do port
;R2 ->Leitura

calculaHigh:
        ;PUSH {R4,R5,R6, R7}
        LDR R1,=GPIO_PORTK_AHB_DATA_BITS_R
        MOV R0, #0
espera_borda_descida
        LDR R2,[R1]
        CMP R2,#0
        BNE espera_borda_descida
espera_borda_subida
        LDR R2,[R1]
        CMP R2,#0
        BEQ espera_borda_subida
ciclo_leitura_high
        LDR R2,[R1]
        ADD R0,R0,#1
        CMP R2,#0
        BNE ciclo_leitura_high
final_High 
        ;MOV R0, R7
        ;POP {R7, R6, R5, R4}
        BX LR
        END