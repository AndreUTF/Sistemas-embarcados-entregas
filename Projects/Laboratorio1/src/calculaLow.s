        PUBLIC calculaLow

        SECTION .text : CODE (2)
        THUMB
        EXPORT calculaLow
        
GPIO_PORTK_AHB_DATA_BITS_R  EQU    0x400613fc

;le pino K7 duas vezes e compara as leituras
calculaLow:
        ;PUSH {R4,R5,R6, R7}
        LDR R1,=GPIO_PORTK_AHB_DATA_BITS_R
        MOV R0,#0
espera_borda_subida
        LDR R2,[R1]
        CMP R2,#0
        BEQ espera_borda_subida
espera_borda_descida
        LDR R2,[R1]
        CMP R2,#0
        BNE espera_borda_descida
ciclo_leitura_low
        LDR R2,[R1]
        ADD R0,R0,#1
        CMP R2,#0
        BEQ ciclo_leitura_low
final_High 
        ;MOV R0, R7
        ;POP {R7, R6, R5, R4}
        BX LR
        END