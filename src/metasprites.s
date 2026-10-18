.section .prg_rom_fixed,"axR",@progbits

.global banked_oam_meta_spr_horizontal

; void banked_oam_meta_spr_horizontal(int x, char y, char bank, const void *data);
; A/rc6: x.l
; X/rc7: x.h
; rc2: y
; rc4: data.l
; rc5: data.h
; rc3: extend signal for sprite delta x

banked_oam_meta_spr_horizontal:
  sta __rc6
  stx __rc7

  ldx SPRID
  ldy #0
1:
  lda (__rc4),y  ;x offset
  cmp #$80
  beq 2f

  bcs 5f ; go to extend negative

  lda #0
  sta __rc3 ; extend positive
  jmp 6f
5:
  lda #$ff
  sta __rc3
6: ; proceed with x offset
  lda (__rc4),y ; restore x offset
  iny
  clc
  adc __rc6
  sta OAM_BUF+3,x

  lda __rc7
  adc __rc3

  ; high byte zero = on screen (?)
  beq 4f

  ; not on screen, go to next sprite
  iny
  iny
  iny
  jmp 1b

4:

  lda (__rc4),y  ; y offset
  iny
  clc
  adc __rc2
  sta OAM_BUF+0,x

  lda (__rc4),y  ;tile
  iny
  sta OAM_BUF+1,x
  lda (__rc4),y  ;attribute
  iny
  sta OAM_BUF+2,x
  inx
  inx
  inx
  inx
  jmp 1b
2:
  stx SPRID
  rts
