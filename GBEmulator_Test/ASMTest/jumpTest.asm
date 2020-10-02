SECTION "start", HOME[$0000]
nop
jp begin


; Testing basic branching
; rgbasm -ojumpTest.o jumpTest.asm
; rgblink -p00 -ojumpTest.gb jumpTest.o
begin:
    ; Load an initial canary that we expect to be the same at the end
    ; Canary at address 0xBEEF
    ld HL, $BEEF
    ld [HL], $22


    ; Loop through each number in the data section
    ld a, $01
    xor a, $01
    jp z, jumped_branch
    ; This should not execute
    ld [HL], $24


jumped_branch:
    ld HL, $CAAA
    ld [HL], $25
    HALT
    NOP