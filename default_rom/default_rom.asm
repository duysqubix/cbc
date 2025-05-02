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
	jp ._ld_mem_bc_a

._ld_mem_bc_a 
	ld a, $12
	ld bc, _RAM 
	ld [bc], a 
	ld a, [bc]
	cp $12
	jp nz, .fail

.fail
	jp .fail 
