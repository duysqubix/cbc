INCLUDE "hardware.inc"

SECTION "Header", ROM0[$100]
EntryPoint:
	nop
	jp Main

SECTION "Title", ROM0[$134]
	db "DEFAULT-ROM"

SECTION "Tileset", ROM0
Tileset:
INCBIN "default_rom.2bpp"

SECTION "Tilemap", ROM0
Tilemap:
Tilemap2: ; Used to test when two symbols point to the same address
	db $40, $41, $42, $43, $44, $45, $46, $41, $41, $41, $47, $41, $41, $41
	db $48, $49, $4A, $4B, $4C, $4D, $4E, $49, $4F, $50, $51, $41, $41, $41

SECTION "CartridgeType", ROM0[$147]
	db $11

SECTION "CartridgeROMCount", ROM0[$148]
	db $00

SECTION "CartridgeRAMCount", ROM0[$149]
	db $05

SECTION "Main", ROM0[$150]
Main:
	nop
	di
	jp .test_begin

.test_begin 
	; 0x01 // LD BC, nn 
	ld de, $01

	ld bc, $1234
	ld a, b 
	cp $12
	jp nz, .fail 
	ld a, c 
	cp $34
	jp nz, .fail 

	; 0x02 // LD (BC), A
	ld de, $02
	ld bc, _SRAM
	ld a, $12 
	ld [bc], a 
	ld a, 0
	ld a, [bc]
	cp $12
	jp nz, .fail

	; 0x03 // INC BC
	ld de, $03

	ld bc, $00FF ; after should be $0100
	inc bc
	ld a, c
	cp $00
	jp nz, .fail 
	ld a, b 
	cp $01
	jp nz, .fail

	; 0x04 // INC B 
	ld de, $04
	ld b, $00
	inc b 
	ld a, b 
	cp $01
	jp nz, .fail 
	ld b, $FF
	inc b 
	ld a, b
	cp $00
	jp nz, .fail

	; 0x05 // DEC B
	ld de, $05
	ld b, $01 
	dec b 
	ld a, b
	cp $00
	jp nz, .fail 
	ld b, $00
	dec b
	ld a, b
	cp $ff
	jp nz, .fail

	; 0x06 // LD B, n 
	ld de, $06
	ld b, $12
	ld a, b
	cp $12
	jp nz, .fail

	; 0x07 // RLCA
	ld de, $07
	ld a, 0b10000000 
	rlca  
	cp a, 0b00000001
	jp nz, .fail
	ld a, 0b00001000
	rlca 
	cp a, 0b00010000
	jp nz, .fail

	; 0x08 // LD (nn), SP 
	ld de, $08
	ld hl, sp+0
	ld sp, $1234
	ld [_SRAM], sp
	ld a, [_SRAM]
	cp $34
	jp nz, .fail 
	ld a, [_SRAM+1]
	cp $12
	jp nz, .fail
	ld sp, hl

	; 0x09 // ADD HL, BC
	ld de, $09
	ld hl, $be00
	ld bc, $ef
	add hl, bc
	ld a, h 
	cp $be 
	jp nz, .fail 
	ld a, l 
	cp $ef 
	jp nz, .fail 
	ld hl, $0000

	; 0x0A // LD A, (BC)
	ld de, $0A
	ld a, $12
	ld [_SRAM], a
	ld bc, _SRAM
	ld a, [bc]
	cp $12
	jp nz, .fail 

	; 0x0B // DEC BC 
	ld de, $0B
	ld bc, $1234 
	dec bc 
	ld a, b 
	cp $12 
	jp nz, .fail 
	ld a, c 
	cp $33 
	jp nz, .fail 
	ld bc, $0100
	dec bc 
	ld a, b 
	cp $00
	jp nz, .fail 
	ld a, c 
	cp $FF
	jp nz, .fail 

	; 0x0C // INC C
	ld de, $0C
	ld c, $00
	inc c 
	ld a, c 
	cp $01
	jp nz, .fail 
	ld c, $FF
	inc c 
	ld a, c 
	cp $00
	jp nz, .fail 

	; 0x0D // DEC C
	ld de, $0D
	ld c, $01
	dec c 
	ld a, c 
	cp $00
	jp nz, .fail 
	ld c, $00
	dec c 
	ld a, c 
	cp $FF
	jp nz, .fail 

	; 0x0E // LD C, n
	ld de, $0E
	ld c, $12
	ld a, c 
	cp $12
	jp nz, .fail 

	; 0x0F // RRCA
	ld de, $0f
	ld a, 0b10000000
	rrca 
	cp a, 0b01000000
	jp nz, .fail 
	ld a, 0b00000001
	rrca 
	cp a, 0b10000000
	jp nz, .fail
	
	; 0x10 // STOP -- skip  

	; 0x11 // LD DE, nn 
	ld de, $11
	ld h, d
	ld l, e 
	ld de, $1234
	ld a, d 
	cp $12 
	jp nz, .fail 
	ld a, e 
	cp $34 
	jp nz, .fail 
	ld d, h 
	ld e, l

	; 0x12 // LD (DE), A
	ld de, $12 
	ld h, d 
	ld l, e 
	ld a, $12 
	ld de, _SRAM 
	ld [de], a 
	ld a, [_SRAM]
	cp $12
	ld de, $12
	jp nz, .fail

	; 0x13 // INC DE
	ld de, $13
	ld d, $00
	ld e, $00
	inc de 
	ld a, d
	cp $00
	jp nz, .fail 
	ld a, e 
	cp $01
	jp nz, .fail 
	ld de, $00ff
	inc de 
	ld a, d 
	cp $01
	jp nz, .fail 
	ld a, e 
	cp $00
	ld de, $13
	jp nz, .fail 

	; 0x14 // INC D
	ld de, $14
	ld d, $00
	inc d 
	ld a, d 
	cp $01
	ld de, $14
	jp nz, .fail 
	ld d, $FF
	inc d 
	ld a, d 
	cp $00
	ld de, $14
	jp nz, .fail 
	

	; 0x15 // DEC D
	ld de, $15
	ld d, $01
	dec d 
	ld a, d 
	cp $00
	ld de, $15
	jp nz, .fail 
	ld d, $00
	dec d 
	ld a, d 
	cp $FF
	ld de, $15
	jp nz, .fail 

	; 0x16 // LD D, n 
	ld de, $16 
	ld d, $12
	ld a, d 
	cp $12
	ld de, $16
	jp nz, .fail 

	; 0x17 // RLA 
	ld de, $17
	ld a, 127
	rla 
	cp a, 254 
	jp nz, .fail

	; 0x18 // JR r8 -- no

	
	
	
	

	; 0x1F // RRA 
	; compare F flag, since it can't be loaded directly, we are doing some hacking
	; push AF to stack, then pop it to HL, then compare F with H
	ld de, $1F 
	ld a, 127
	rra 
	push af 
	pop hl
	ld a, h 
	cp 63 
	jp nz, .fail 
	ld a, l 
	cp 16 
	jp nz, .fail




	jp .finished

.fail 
	ld hl, $feeb
	stop
	jp .fail

.finished 
	ld hl, $beef
	stop 
	jp .finished



; .waitVBlank
; 	ldh a, [rLY]
; 	cp 144
; 	jr c, .waitVBlank
; 	ret

; .setup
; 	ld hl, rLCDC
; 	set 6, [hl]
; 	set 5, [hl]
; 	ld hl, rWY
; 	ld [hl], 144
; 	inc hl
; 	ld [hl], 43

; 	ld bc, $8400
; 	ld de, $8700
; 	ld hl, Tileset

; .readTileset
; 	call .waitVBlank
; 	ld a, [hli]
; 	ld [bc], a
; 	inc bc
; 	ld a, b
; 	cp a, d
; 	jr c, .readTileset

; 	ld hl, Tilemap
; 	ld bc, $9C00
; 	ld de, $9C0E

; .readTilemap
; 	call .waitVBlank
; 	ld a, [hli]
; 	ld [bc], a
; 	inc bc
; 	ld a, c
; 	cp a, e
; 	jr c, .readTilemap

; 	add $12
; 	ld c, a
; 	ld a, e
; 	add $20
; 	ld e, a
; 	ld a, e

; 	cp a, $2F
; 	jr c, .readTilemap

; 	ld hl, _OAMRAM

; .clearOAM
; 	call .waitVBlank
; 	ld [hl], 0
; 	inc hl
; 	ld a, l
; 	cp a, $F0
; 	jr nz, .clearOAM

; .loop
; 	ld a, [rWY]
; 	cp a, 90
; 	jr c, .loop
; .inner_loop ; Used in test
; 	ldh a, [rDIV]
; 	cp a, 255
; 	jp z, .move
; 	jr .loop

; .sync
; 	ldh a, [rLY]
; 	cp 144
; 	jr nz, .sync
; 	ret

; .move
; 	call .sync
; 	ld hl, rWY
; 	dec [hl]

; 	ldh [rDIV], a
; 	jr .loop
