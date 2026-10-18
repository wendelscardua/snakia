    .section .chr_rom,"aR",@progbits

    ; CHR data

    .global bg_chr
    bg_chr:
    .incbin "bg.chr"

    .global spr_chr
    spr_chr:
    .incbin "spr.chr"

    .section .prg_rom_fixed,"aR",@progbits

    ; Palettes

    .global bg_pal
    bg_pal:
    .incbin "bg.pal"

    .global spr_pal
    spr_pal:
    .incbin "spr.pal"

    ; Nametables

    .global screen_nam
    screen_nam:
    .incbin "screen.nam"
