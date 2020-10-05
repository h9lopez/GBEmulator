SECTION "start", HOME[$0000]
nop
jp begin


; Testing writing into VRAM
; rgbasm -osdlBaseline.o sdlBaseline.asm
; rgblink -p00 -osdlBaseline.gb sdlBaseline.o
begin:
    ld hl, $9000 + 16
    ld de, smiley_sprite
    ld b, 16
copy_loop:
    ld a, [de]
    inc de
    ld [hl+], a
    dec b
    jp nz, copy_loop

    ld hl, $9800
    ld [hl], 1

    HALT
    NOP


smiley_sprite: db $00, $00, $00, $00, $24, $24, $00, $00, $81, $81, $7e, $7e, $00, $00, $00, $00