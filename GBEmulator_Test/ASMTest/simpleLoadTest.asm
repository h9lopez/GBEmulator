    SECTION "start", HOME[$0000]
    nop
    jp begin

; Loads the value 0xC9 into memory address 0xBABF at end of program
; Build with rgbasm
; rgbasm -osimpleLoadTest.o simpleLoadTest.asm
; rgblink -p00 -osimpleLoadTest.gb simpleLoadTest.o
begin:
    LD A, $BE
    ADD A, $01
    LD L, A
    LD A, $BA
    LD H, A

    LD A, $C9
    LD [HL],A
    HALT
    NOP