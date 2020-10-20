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

    ld hl, $9801
    ld [hl], 1

    ld hl, $9800
    ld [hl], 1

    ld hl, $9802
    ld [hl], 1

    ld hl, $9803
    ld [hl], 1

    ld hl, $9804
    ld [hl], 1

    ld hl, $9805
    ld [hl], 1

    ld hl, $9806
    ld [hl], 1

    ld hl, $9807
    ld [hl], 1

    ld hl, $9808
    ld [hl], 1

    ld hl, $9809
    ld [hl], 1

    ld hl, $980A
    ld [hl], 1

    ld hl, $980B
    ld [hl], 1

    ld hl, $980C
    ld [hl], 1

    ld hl, $980D
    ld [hl], 1

    ld hl, $980E
    ld [hl], 1

    ld hl, $980F
    ld [hl], 1

    ld hl, $981F
    ld [hl], 1

    ld hl, $982F
    ld [hl], 1

    ld hl, $983F
    ld [hl], 1

    ld hl, $984F
    ld [hl], 1

    ld hl, $985F
    ld [hl], 1

    ld hl, $986F
    ld [hl], 1

    ld hl, $987F
    ld [hl], 1

    ld hl, $988F
    ld [hl], 1

    ld hl, $989F
    ld [hl], 1

    ld hl, $98AF
    ld [hl], 1

    ld hl, $98BF
    ld [hl], 1

    ld hl, $98CF
    ld [hl], 1

    ld hl, $98DF
    ld [hl], 1

    ld hl, $98EF
    ld [hl], 1

    ld hl, $98FF
    ld [hl], 1


    HALT
    NOP


smiley_sprite: db $00, $00, $00, $00, $24, $24, $00, $00, $81, $81, $7e, $7e, $00, $00, $00, $00