        PUBLIC calculaFreq
        

        SECTION .text : CODE (2)
        THUMB
        EXPORT calculaFreq
     
GPIO_PORTK_AHB_DATA_BITS_R  EQU    0x400613fc

;Salva valor do numero de leituras
;Le pino K7
;Le pino K7
;Se as duas leituras foram iguais, espera ate a mudanca das duas leituras
;
calculaFreq:
        PUSH {R4, R5, R6}
        MOV R0, #0
        LDR R1,=GPIO_PORTK_AHB_DATA_BITS_R
        LDR R5,[R1]
espera_mudanca
        LDR R6,[R1]
        CMP R5,R6
        BEQ espera_mudanca
medida_final
        LDR R6,[R1]
        ADD R0,R0,#1
        CMP R5,R6
        BNE medida_final
medida_final_2
        LDR R6,[R1]
        ADD R0,R0,#1
        CMP R6,R5
        BEQ medida_final_2
final:  
        POP {R4, R5, R6}
        BX LR
        END