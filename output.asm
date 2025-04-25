
obj/cbc.o:     file format elf64-littleaarch64


Disassembly of section .text:

0000000000000000 <_load_rom>:
        REG_H, REG_L, HL(), _fetch_item(HL()), 
        REG_PC, REG_SP);
    
}

size_t _load_rom(const char *rom_path){
       0:	a9bd7bfd 	stp	x29, x30, [sp, #-48]!
    FILE * rom = fopen(rom_path, "rb");
       4:	90000001 	adrp	x1, 0 <_load_rom>
       8:	91000021 	add	x1, x1, #0x0
size_t _load_rom(const char *rom_path){
       c:	910003fd 	mov	x29, sp
      10:	a90153f3 	stp	x19, x20, [sp, #16]
      14:	a9025bf5 	stp	x21, x22, [sp, #32]
      18:	aa0003f5 	mov	x21, x0
    FILE * rom = fopen(rom_path, "rb");
      1c:	94000000 	bl	0 <fopen>
    if (!rom) {
      20:	b4000620 	cbz	x0, e4 <_load_rom+0xe4>
        log_error("Failed to open ROM: %s", rom_path);
        exit(1);
    }

    fseek(rom, 0, SEEK_END);
      24:	52800042 	mov	w2, #0x2                   	// #2
      28:	d2800001 	mov	x1, #0x0                   	// #0
      2c:	aa0003f3 	mov	x19, x0
      30:	94000000 	bl	0 <fseek>
    long size = ftell(rom);
      34:	aa1303e0 	mov	x0, x19
      38:	94000000 	bl	0 <ftell>
    fseek(rom, 0, SEEK_SET);
      3c:	52800002 	mov	w2, #0x0                   	// #0
    long size = ftell(rom);
      40:	aa0003f4 	mov	x20, x0
    fseek(rom, 0, SEEK_SET);
      44:	d2800001 	mov	x1, #0x0                   	// #0
      48:	aa1303e0 	mov	x0, x19
      4c:	94000000 	bl	0 <fseek>
  size_t sz = __glibc_objsize0 (__ptr);
  if (__glibc_safe_or_unknown_len (__n, __size, sz))
    return __fread_alias (__ptr, __size, __n, __stream);
  if (__glibc_unsafe_len (__n, __size, sz))
    return __fread_chk_warn (__ptr, sz, __size, __n, __stream);
  return __fread_chk (__ptr, sz, __size, __n, __stream);
      50:	90000000 	adrp	x0, 0 <ROM>
      54:	f9400000 	ldr	x0, [x0]
      58:	aa1303e4 	mov	x4, x19
      5c:	aa1403e3 	mov	x3, x20
      60:	d2800022 	mov	x2, #0x1                   	// #1
      64:	d2a00401 	mov	x1, #0x200000              	// #2097152
      68:	94000000 	bl	0 <__fread_chk>
      6c:	aa0003f6 	mov	x22, x0

    size_t n = fread(ROM, 1, size, rom);
    fclose(rom);
      70:	aa1303e0 	mov	x0, x19


    log_info("Loaded ROM: %s", rom_path);
      74:	90000013 	adrp	x19, 0 <_load_rom>
    fclose(rom);
      78:	94000000 	bl	0 <fclose>
    log_info("Loaded ROM: %s", rom_path);
      7c:	91000273 	add	x19, x19, #0x0
      80:	aa1503e4 	mov	x4, x21
      84:	aa1303e1 	mov	x1, x19
      88:	52800682 	mov	w2, #0x34                  	// #52
      8c:	52800040 	mov	w0, #0x2                   	// #2
      90:	90000003 	adrp	x3, 0 <_load_rom>
      94:	91000063 	add	x3, x3, #0x0
      98:	94000000 	bl	0 <log_log>
    log_info("ROM Size: %d bytes", size);
      9c:	aa1403e4 	mov	x4, x20
      a0:	aa1303e1 	mov	x1, x19
      a4:	528006a2 	mov	w2, #0x35                  	// #53
      a8:	52800040 	mov	w0, #0x2                   	// #2
      ac:	90000003 	adrp	x3, 0 <_load_rom>
      b0:	91000063 	add	x3, x3, #0x0
      b4:	94000000 	bl	0 <log_log>
    fdump_memory("rom.txt", ROM, size);
      b8:	90000001 	adrp	x1, 0 <ROM>
      bc:	f9400021 	ldr	x1, [x1]
      c0:	aa1403e2 	mov	x2, x20
      c4:	90000000 	adrp	x0, 0 <_load_rom>
      c8:	91000000 	add	x0, x0, #0x0
      cc:	94000000 	bl	0 <fdump_memory>
    return n;
}
      d0:	a94153f3 	ldp	x19, x20, [sp, #16]
      d4:	aa1603e0 	mov	x0, x22
      d8:	a9425bf5 	ldp	x21, x22, [sp, #32]
      dc:	a8c37bfd 	ldp	x29, x30, [sp], #48
      e0:	d65f03c0 	ret
        log_error("Failed to open ROM: %s", rom_path);
      e4:	aa1503e4 	mov	x4, x21
      e8:	90000003 	adrp	x3, 0 <_load_rom>
      ec:	90000001 	adrp	x1, 0 <_load_rom>
      f0:	91000063 	add	x3, x3, #0x0
      f4:	91000021 	add	x1, x1, #0x0
      f8:	52800502 	mov	w2, #0x28                  	// #40
      fc:	52800080 	mov	w0, #0x4                   	// #4
     100:	94000000 	bl	0 <log_log>
        exit(1);
     104:	52800020 	mov	w0, #0x1                   	// #1
     108:	94000000 	bl	0 <exit>
     10c:	d503201f 	nop

0000000000000110 <gameboy_init>:

void gameboy_init(const char *rom_path){
     110:	a9be7bfd 	stp	x29, x30, [sp, #-32]!

    // Set Internals
    randomize(WRAM, MAX_RAM_SIZE);
     114:	d2a00081 	mov	x1, #0x40000               	// #262144
void gameboy_init(const char *rom_path){
     118:	910003fd 	mov	x29, sp
     11c:	f9000bf3 	str	x19, [sp, #16]
     120:	aa0003f3 	mov	x19, x0
    randomize(WRAM, MAX_RAM_SIZE);
     124:	90000000 	adrp	x0, 0 <WRAM>
     128:	f9400000 	ldr	x0, [x0]
     12c:	94000000 	bl	0 <randomize>
    randomize(VRAM, MAX_VRAM_SIZE);
     130:	90000000 	adrp	x0, 0 <VRAM>
     134:	f9400000 	ldr	x0, [x0]
     138:	d2880001 	mov	x1, #0x4000                	// #16384
     13c:	94000000 	bl	0 <randomize>
    randomize(SRAM, MAX_RAM_SIZE);
     140:	90000000 	adrp	x0, 0 <SRAM>
     144:	f9400000 	ldr	x0, [x0]
     148:	d2a00081 	mov	x1, #0x40000               	// #262144
     14c:	94000000 	bl	0 <randomize>

    REG_A = 0x11;
    FLAG_Z_SET();
    FLAG_N_RESET();
    FLAG_H_RESET();
    FLAG_C_RESET();
     150:	90000002 	adrp	x2, 0 <REG_F>
     154:	f9400042 	ldr	x2, [x2]
    REG_B = 0x00;
    REG_C = 0x00;
    REG_D = 0xFF;
     158:	12800010 	mov	w16, #0xffffffff            	// #-1
     15c:	90000007 	adrp	x7, 0 <REG_D>
     160:	f94000e7 	ldr	x7, [x7]
    REG_E = 0x56;
     164:	52800acf 	mov	w15, #0x56                  	// #86
    FLAG_C_RESET();
     168:	39400041 	ldrb	w1, [x2]
    REG_H = 0x00;
    REG_L = 0x0D;
     16c:	528001ae 	mov	w14, #0xd                   	// #13
    REG_E = 0x56;
     170:	90000006 	adrp	x6, 0 <REG_E>
     174:	f94000c6 	ldr	x6, [x6]
    FLAG_C_RESET();
     178:	12197021 	and	w1, w1, #0xffffff8f
     17c:	32196021 	orr	w1, w1, #0xffffff80
     180:	39000041 	strb	w1, [x2]
    REG_H = 0x00;
     184:	90000005 	adrp	x5, 0 <REG_H>
     188:	f94000a5 	ldr	x5, [x5]
    REG_PC = 0x0100;
     18c:	5280200d 	mov	w13, #0x100                 	// #256
    REG_L = 0x0D;
     190:	90000004 	adrp	x4, 0 <REG_L>
     194:	f9400084 	ldr	x4, [x4]
    REG_SP = 0xFFFE;
     198:	1280002c 	mov	w12, #0xfffffffe            	// #-2
    REG_PC = 0x0100;
     19c:	90000003 	adrp	x3, 0 <REG_PC>
     1a0:	f9400063 	ldr	x3, [x3]

    IO[0x44] = 145;
     1a4:	12800dcb 	mov	w11, #0xffffff91            	// #-111
    REG_SP = 0xFFFE;
     1a8:	90000002 	adrp	x2, 0 <REG_SP>
     1ac:	f9400042 	ldr	x2, [x2]
    REG_D = 0xFF;
     1b0:	390000f0 	strb	w16, [x7]
    IO[0x44] = 145;
     1b4:	90000001 	adrp	x1, 0 <IO>
     1b8:	f9400021 	ldr	x1, [x1]
    REG_E = 0x56;
     1bc:	390000cf 	strb	w15, [x6]
    REG_H = 0x00;
     1c0:	390000bf 	strb	wzr, [x5]

    _load_rom(rom_path);
     1c4:	aa1303e0 	mov	x0, x19
    REG_L = 0x0D;
     1c8:	3900008e 	strb	w14, [x4]
    REG_A = 0x11;
     1cc:	52800231 	mov	w17, #0x11                  	// #17
    REG_PC = 0x0100;
     1d0:	7900006d 	strh	w13, [x3]
    REG_SP = 0xFFFE;
     1d4:	7900004c 	strh	w12, [x2]
    IO[0x44] = 145;
     1d8:	3901102b 	strb	w11, [x1, #68]
    REG_A = 0x11;
     1dc:	9000000a 	adrp	x10, 0 <REG_A>
     1e0:	f940014a 	ldr	x10, [x10]
}
     1e4:	f9400bf3 	ldr	x19, [sp, #16]
     1e8:	a8c27bfd 	ldp	x29, x30, [sp], #32
    REG_A = 0x11;
     1ec:	39000151 	strb	w17, [x10]
    REG_B = 0x00;
     1f0:	90000009 	adrp	x9, 0 <REG_B>
     1f4:	f9400129 	ldr	x9, [x9]
    REG_C = 0x00;
     1f8:	90000008 	adrp	x8, 0 <REG_C>
     1fc:	f9400108 	ldr	x8, [x8]
    REG_B = 0x00;
     200:	3900013f 	strb	wzr, [x9]
    REG_C = 0x00;
     204:	3900011f 	strb	wzr, [x8]
    _load_rom(rom_path);
     208:	14000000 	b	0 <_load_rom>
     20c:	d503201f 	nop

0000000000000210 <_fetch_item>:
    }

    return opcycles;
}

uint8_t _fetch_item(address_t address){
     210:	12003c01 	and	w1, w0, #0xffff
    // log_trace("Fetching item at address: %04X", address);
    switch (address) {
     214:	529dffe0 	mov	w0, #0xefff                	// #61439
     218:	6b00003f 	cmp	w1, w0
     21c:	54000248 	b.hi	264 <_fetch_item+0x54>  // b.pmore
     220:	529bffe0 	mov	w0, #0xdfff                	// #57343
     224:	6b00003f 	cmp	w1, w0
     228:	540004a8 	b.hi	2bc <_fetch_item+0xac>  // b.pmore
     22c:	5297ffe0 	mov	w0, #0xbfff                	// #49151
     230:	6b00003f 	cmp	w1, w0
     234:	54000728 	b.hi	318 <_fetch_item+0x108>  // b.pmore
     238:	5293ffe0 	mov	w0, #0x9fff                	// #40959
     23c:	6b00003f 	cmp	w1, w0
     240:	54000b88 	b.hi	3b0 <_fetch_item+0x1a0>  // b.pmore
     244:	37780461 	tbnz	w1, #15, 2d0 <_fetch_item+0xc0>
     248:	5287ffe0 	mov	w0, #0x3fff                	// #16383
     24c:	6b00003f 	cmp	w1, w0
     250:	540007c8 	b.hi	348 <_fetch_item+0x138>  // b.pmore
        case 0x0000 ... 0x3FFF:
            return ROM_GET(0, address);
     254:	90000000 	adrp	x0, 0 <ROM>
     258:	f9400000 	ldr	x0, [x0]
     25c:	3861c800 	ldrb	w0, [x0, w1, sxtw]

        default:
            log_error("Invalid fetch address: %d", address);
            exit(1);       
    }
}
     260:	d65f03c0 	ret
    switch (address) {
     264:	529fefe0 	mov	w0, #0xff7f                	// #65407
     268:	6b00003f 	cmp	w1, w0
     26c:	54000448 	b.hi	2f4 <_fetch_item+0xe4>  // b.pmore
     270:	529fdfe0 	mov	w0, #0xfeff                	// #65279
     274:	6b00003f 	cmp	w1, w0
     278:	54000848 	b.hi	380 <_fetch_item+0x170>  // b.pmore
     27c:	529fd3e2 	mov	w2, #0xfe9f                	// #65183
     280:	52800000 	mov	w0, #0x0                   	// #0
     284:	6b02003f 	cmp	w1, w2
     288:	54fffec8 	b.hi	260 <_fetch_item+0x50>  // b.pmore
     28c:	529fbfe0 	mov	w0, #0xfdff                	// #65023
     290:	6b00003f 	cmp	w1, w0
     294:	54000828 	b.hi	398 <_fetch_item+0x188>  // b.pmore
            return WRAM_GET(WRAM_CURRENT_BANK, address - 0xF000);
     298:	90000000 	adrp	x0, 0 <WRAM_CURRENT_BANK>
     29c:	f9400000 	ldr	x0, [x0]
     2a0:	90000002 	adrp	x2, 0 <WRAM>
     2a4:	f9400042 	ldr	x2, [x2]
     2a8:	39400000 	ldrb	w0, [x0]
     2ac:	0b003420 	add	w0, w1, w0, lsl #13
     2b0:	51403c00 	sub	w0, w0, #0xf, lsl #12
     2b4:	3860c840 	ldrb	w0, [x2, w0, sxtw]
}
     2b8:	d65f03c0 	ret
            return WRAM_GET(0, address - 0xE000);
     2bc:	90000000 	adrp	x0, 0 <WRAM>
     2c0:	f9400000 	ldr	x0, [x0]
     2c4:	51403821 	sub	w1, w1, #0xe, lsl #12
     2c8:	3861c800 	ldrb	w0, [x0, w1, sxtw]
}
     2cc:	d65f03c0 	ret
            return VRAM_GET(VRAM_CURRENT_BANK, address - 0x8000);
     2d0:	90000000 	adrp	x0, 0 <VRAM_CURRENT_BANK>
     2d4:	f9400000 	ldr	x0, [x0]
     2d8:	90000002 	adrp	x2, 0 <VRAM>
     2dc:	f9400042 	ldr	x2, [x2]
     2e0:	39400000 	ldrb	w0, [x0]
     2e4:	0b003420 	add	w0, w1, w0, lsl #13
     2e8:	51402000 	sub	w0, w0, #0x8, lsl #12
     2ec:	3860c840 	ldrb	w0, [x2, w0, sxtw]
}
     2f0:	d65f03c0 	ret
    switch (address) {
     2f4:	529fffe0 	mov	w0, #0xffff                	// #65535
     2f8:	6b00003f 	cmp	w1, w0
     2fc:	540006c0 	b.eq	3d4 <_fetch_item+0x1c4>  // b.none
            return HRAM[address - 0xFF80];
     300:	90000000 	adrp	x0, 0 <HRAM>
     304:	f9400000 	ldr	x0, [x0]
     308:	129fefe2 	mov	w2, #0xffff0080            	// #-65408
     30c:	0b020021 	add	w1, w1, w2
     310:	3861c800 	ldrb	w0, [x0, w1, sxtw]
}
     314:	d65f03c0 	ret
    switch (address) {
     318:	5299ffe0 	mov	w0, #0xcfff                	// #53247
     31c:	6b00003f 	cmp	w1, w0
     320:	54000269 	b.ls	36c <_fetch_item+0x15c>  // b.plast
            return WRAM_GET(WRAM_CURRENT_BANK, address - 0xD000);
     324:	90000000 	adrp	x0, 0 <WRAM_CURRENT_BANK>
     328:	f9400000 	ldr	x0, [x0]
     32c:	90000002 	adrp	x2, 0 <WRAM>
     330:	f9400042 	ldr	x2, [x2]
     334:	39400000 	ldrb	w0, [x0]
     338:	0b003420 	add	w0, w1, w0, lsl #13
     33c:	51403400 	sub	w0, w0, #0xd, lsl #12
     340:	3860c840 	ldrb	w0, [x2, w0, sxtw]
}
     344:	d65f03c0 	ret
            return ROM_GET(ROM_CURRENT_BANK, address - 0x4000);
     348:	90000000 	adrp	x0, 0 <ROM_CURRENT_BANK>
     34c:	f9400000 	ldr	x0, [x0]
     350:	90000002 	adrp	x2, 0 <ROM>
     354:	f9400042 	ldr	x2, [x2]
     358:	39400000 	ldrb	w0, [x0]
     35c:	0b003820 	add	w0, w1, w0, lsl #14
     360:	51401000 	sub	w0, w0, #0x4, lsl #12
     364:	3860c840 	ldrb	w0, [x2, w0, sxtw]
}
     368:	d65f03c0 	ret
            return WRAM_GET(0, address - 0xC000);
     36c:	90000000 	adrp	x0, 0 <WRAM>
     370:	f9400000 	ldr	x0, [x0]
     374:	51403021 	sub	w1, w1, #0xc, lsl #12
     378:	3861c800 	ldrb	w0, [x0, w1, sxtw]
}
     37c:	d65f03c0 	ret
            return IO[address - 0xFF00];
     380:	90000000 	adrp	x0, 0 <IO>
     384:	f9400000 	ldr	x0, [x0]
     388:	129fdfe3 	mov	w3, #0xffff0100            	// #-65280
     38c:	0b030021 	add	w1, w1, w3
     390:	3861c800 	ldrb	w0, [x0, w1, sxtw]
}
     394:	d65f03c0 	ret
            return OAM[address - 0xFE00];
     398:	90000000 	adrp	x0, 0 <OAM>
     39c:	f9400000 	ldr	x0, [x0]
     3a0:	129fbfe4 	mov	w4, #0xffff0200            	// #-65024
     3a4:	0b040021 	add	w1, w1, w4
     3a8:	3861c800 	ldrb	w0, [x0, w1, sxtw]
}
     3ac:	d65f03c0 	ret
            return SRAM_GET(SRAM_CURRENT_BANK, address - 0xA000);
     3b0:	90000000 	adrp	x0, 0 <SRAM_CURRENT_BANK>
     3b4:	f9400000 	ldr	x0, [x0]
     3b8:	90000002 	adrp	x2, 0 <SRAM>
     3bc:	f9400042 	ldr	x2, [x2]
     3c0:	39400000 	ldrb	w0, [x0]
     3c4:	0b003420 	add	w0, w1, w0, lsl #13
     3c8:	51402800 	sub	w0, w0, #0xa, lsl #12
     3cc:	3860c840 	ldrb	w0, [x2, w0, sxtw]
}
     3d0:	d65f03c0 	ret
            return IE;
     3d4:	90000000 	adrp	x0, 0 <IE>
     3d8:	f9400000 	ldr	x0, [x0]
     3dc:	39400000 	ldrb	w0, [x0]
}
     3e0:	d65f03c0 	ret

00000000000003e4 <dump_registers.part.0>:
void dump_registers(){
     3e4:	d101c3ff 	sub	sp, sp, #0x70
    log_trace(str, 
     3e8:	90000001 	adrp	x1, 0 <REG_B>
     3ec:	f9400021 	ldr	x1, [x1]
     3f0:	90000000 	adrp	x0, 0 <REG_C>
     3f4:	f9400000 	ldr	x0, [x0]
void dump_registers(){
     3f8:	a9067bfd 	stp	x29, x30, [sp, #96]
     3fc:	910183fd 	add	x29, sp, #0x60
    log_trace(str, 
     400:	39400026 	ldrb	w6, [x1]
     404:	39400007 	ldrb	w7, [x0]
     408:	2a0620e0 	orr	w0, w7, w6, lsl #8
     40c:	2a0003e8 	mov	w8, w0
     410:	94000000 	bl	210 <_fetch_item>
     414:	90000001 	adrp	x1, 0 <REG_D>
     418:	f9400021 	ldr	x1, [x1]
     41c:	12001c0d 	and	w13, w0, #0xff
     420:	90000000 	adrp	x0, 0 <REG_E>
     424:	f9400000 	ldr	x0, [x0]
     428:	3940002c 	ldrb	w12, [x1]
     42c:	39400005 	ldrb	w5, [x0]
     430:	2a0c20a0 	orr	w0, w5, w12, lsl #8
     434:	2a0003ee 	mov	w14, w0
     438:	94000000 	bl	210 <_fetch_item>
     43c:	90000001 	adrp	x1, 0 <REG_H>
     440:	f9400021 	ldr	x1, [x1]
     444:	12001c0b 	and	w11, w0, #0xff
     448:	90000000 	adrp	x0, 0 <REG_L>
     44c:	f9400000 	ldr	x0, [x0]
     450:	3940002a 	ldrb	w10, [x1]
     454:	39400009 	ldrb	w9, [x0]
     458:	2a0a2120 	orr	w0, w9, w10, lsl #8
     45c:	2a0003ef 	mov	w15, w0
     460:	94000000 	bl	210 <_fetch_item>
     464:	90000003 	adrp	x3, 0 <REG_A>
     468:	f9400063 	ldr	x3, [x3]
     46c:	b90003e8 	str	w8, [sp]
     470:	90000002 	adrp	x2, 0 <REG_SP>
     474:	f9400042 	ldr	x2, [x2]
     478:	12001c08 	and	w8, w0, #0xff
     47c:	90000001 	adrp	x1, 0 <REG_PC>
     480:	f9400021 	ldr	x1, [x1]
     484:	b9000bed 	str	w13, [sp, #8]
     488:	90000000 	adrp	x0, 0 <REG_F>
     48c:	f9400000 	ldr	x0, [x0]
     490:	b90013ec 	str	w12, [sp, #16]
     494:	b9001be5 	str	w5, [sp, #24]
     498:	39400064 	ldrb	w4, [x3]
     49c:	90000003 	adrp	x3, 0 <_load_rom>
     4a0:	7940004c 	ldrh	w12, [x2]
     4a4:	91000063 	add	x3, x3, #0x0
     4a8:	7940002d 	ldrh	w13, [x1]
     4ac:	52800382 	mov	w2, #0x1c                  	// #28
     4b0:	39400005 	ldrb	w5, [x0]
     4b4:	90000001 	adrp	x1, 0 <_load_rom>
     4b8:	b90023ee 	str	w14, [sp, #32]
     4bc:	91000021 	add	x1, x1, #0x0
     4c0:	b9002beb 	str	w11, [sp, #40]
     4c4:	52800000 	mov	w0, #0x0                   	// #0
     4c8:	53047ca5 	lsr	w5, w5, #4
     4cc:	b90033ea 	str	w10, [sp, #48]
     4d0:	b9003be9 	str	w9, [sp, #56]
     4d4:	b90043ef 	str	w15, [sp, #64]
     4d8:	b9004be8 	str	w8, [sp, #72]
     4dc:	b90053ed 	str	w13, [sp, #80]
     4e0:	b9005bec 	str	w12, [sp, #88]
     4e4:	94000000 	bl	0 <log_log>
}
     4e8:	a9467bfd 	ldp	x29, x30, [sp, #96]
     4ec:	9101c3ff 	add	sp, sp, #0x70
     4f0:	d65f03c0 	ret

00000000000004f4 <dump_registers>:
void dump_registers(){
     4f4:	a9bf7bfd 	stp	x29, x30, [sp, #-16]!
     4f8:	910003fd 	mov	x29, sp
    if (get_log_level() > LOG_TRACE){ return; }
     4fc:	94000000 	bl	0 <get_log_level>
     500:	7100001f 	cmp	w0, #0x0
     504:	5400006d 	b.le	510 <dump_registers+0x1c>
}
     508:	a8c17bfd 	ldp	x29, x30, [sp], #16
     50c:	d65f03c0 	ret
     510:	a8c17bfd 	ldp	x29, x30, [sp], #16
     514:	17ffffb4 	b	3e4 <dump_registers.part.0>
     518:	d503201f 	nop
     51c:	d503201f 	nop

0000000000000520 <_write_item>:

void _write_item(address_t address, uint8_t value){
     520:	a9bd7bfd 	stp	x29, x30, [sp, #-48]!
     524:	910003fd 	mov	x29, sp
     528:	a90153f3 	stp	x19, x20, [sp, #16]
     52c:	12001c34 	and	w20, w1, #0xff
     530:	12003c13 	and	w19, w0, #0xffff
    // write serial to stdout 
    if (address == 0xFF02 && value == 0x81){
     534:	7102069f 	cmp	w20, #0x81
     538:	529fe040 	mov	w0, #0xff02                	// #65282
     53c:	7a400260 	ccmp	w19, w0, #0x0, eq	// eq = none
     540:	54000500 	b.eq	5e0 <_write_item+0xc0>  // b.none
        printf("%c", IO[0x01]);
    }

    switch (address) {
     544:	529dffe0 	mov	w0, #0xefff                	// #61439
     548:	6b00027f 	cmp	w19, w0
     54c:	540002c8 	b.hi	5a4 <_write_item+0x84>  // b.pmore
     550:	529bffe0 	mov	w0, #0xdfff                	// #57343
     554:	6b00027f 	cmp	w19, w0
     558:	540008e8 	b.hi	674 <_write_item+0x154>  // b.pmore
     55c:	5297ffe0 	mov	w0, #0xbfff                	// #49151
     560:	6b00027f 	cmp	w19, w0
     564:	54000708 	b.hi	644 <_write_item+0x124>  // b.pmore
     568:	5293ffe0 	mov	w0, #0x9fff                	// #40959
     56c:	6b00027f 	cmp	w19, w0
     570:	54000a68 	b.hi	6bc <_write_item+0x19c>  // b.pmore
     574:	36780133 	tbz	w19, #15, 598 <_write_item+0x78>
        case 0x4000 ... 0x7FFF:
            // Write to Cartridge
            return;

        case 0x8000 ... 0x9FFF:
            VRAM_SET(VRAM_CURRENT_BANK, address - 0x8000, value);
     578:	90000000 	adrp	x0, 0 <VRAM_CURRENT_BANK>
     57c:	f9400000 	ldr	x0, [x0]
     580:	90000001 	adrp	x1, 0 <VRAM>
     584:	f9400021 	ldr	x1, [x1]
     588:	39400000 	ldrb	w0, [x0]
     58c:	0b003660 	add	w0, w19, w0, lsl #13
     590:	51402000 	sub	w0, w0, #0x8, lsl #12
     594:	3820c834 	strb	w20, [x1, w0, sxtw]

        default:
            log_error("Invalid fetch address: %d", address);
            exit(1);       
    }
}
     598:	a94153f3 	ldp	x19, x20, [sp, #16]
     59c:	a8c37bfd 	ldp	x29, x30, [sp], #48
     5a0:	d65f03c0 	ret
    switch (address) {
     5a4:	529fdfe0 	mov	w0, #0xfeff                	// #65279
     5a8:	6b00027f 	cmp	w19, w0
     5ac:	54000348 	b.hi	614 <_write_item+0xf4>  // b.pmore
     5b0:	529fd3e0 	mov	w0, #0xfe9f                	// #65183
     5b4:	6b00027f 	cmp	w19, w0
     5b8:	54ffff08 	b.hi	598 <_write_item+0x78>  // b.pmore
     5bc:	529fbfe0 	mov	w0, #0xfdff                	// #65023
     5c0:	6b00027f 	cmp	w19, w0
     5c4:	54000629 	b.ls	688 <_write_item+0x168>  // b.plast
            OAM[address - 0xFE00] = value;
     5c8:	90000000 	adrp	x0, 0 <OAM>
     5cc:	f9400000 	ldr	x0, [x0]
     5d0:	129fbfe3 	mov	w3, #0xffff0200            	// #-65024
     5d4:	0b030273 	add	w19, w19, w3
     5d8:	3833c814 	strb	w20, [x0, w19, sxtw]
            return;
     5dc:	17ffffef 	b	598 <_write_item+0x78>
  return __printf_chk (__USE_FORTIFY_LEVEL - 1, __fmt, __va_arg_pack ());
     5e0:	f90013f5 	str	x21, [sp, #32]
     5e4:	90000001 	adrp	x1, 0 <_load_rom>
        printf("%c", IO[0x01]);
     5e8:	90000015 	adrp	x21, 0 <IO>
     5ec:	f94002b5 	ldr	x21, [x21]
     5f0:	91000021 	add	x1, x1, #0x0
     5f4:	52800040 	mov	w0, #0x2                   	// #2
     5f8:	394006a2 	ldrb	w2, [x21, #1]
     5fc:	94000000 	bl	0 <__printf_chk>
            IO[address - 0xFF00] = value;
     600:	129fdfe2 	mov	w2, #0xffff0100            	// #-65280
     604:	0b020273 	add	w19, w19, w2
     608:	3833cab4 	strb	w20, [x21, w19, sxtw]
            return;
     60c:	f94013f5 	ldr	x21, [sp, #32]
     610:	17ffffe2 	b	598 <_write_item+0x78>
    switch (address) {
     614:	529fffe0 	mov	w0, #0xffff                	// #65535
     618:	6b00027f 	cmp	w19, w0
     61c:	54000480 	b.eq	6ac <_write_item+0x18c>  // b.none
     620:	529fefe0 	mov	w0, #0xff7f                	// #65407
     624:	6b00027f 	cmp	w19, w0
     628:	54000669 	b.ls	6f4 <_write_item+0x1d4>  // b.plast
            HRAM[address - 0xFF80] = value;
     62c:	90000000 	adrp	x0, 0 <HRAM>
     630:	f9400000 	ldr	x0, [x0]
     634:	129fefe1 	mov	w1, #0xffff0080            	// #-65408
     638:	0b010273 	add	w19, w19, w1
     63c:	3833c814 	strb	w20, [x0, w19, sxtw]
            return;
     640:	17ffffd6 	b	598 <_write_item+0x78>
    switch (address) {
     644:	5299ffe0 	mov	w0, #0xcfff                	// #53247
     648:	6b00027f 	cmp	w19, w0
     64c:	540004a9 	b.ls	6e0 <_write_item+0x1c0>  // b.plast
            WRAM_SET(WRAM_CURRENT_BANK, address - 0xD000, value);
     650:	90000000 	adrp	x0, 0 <WRAM_CURRENT_BANK>
     654:	f9400000 	ldr	x0, [x0]
     658:	90000001 	adrp	x1, 0 <WRAM>
     65c:	f9400021 	ldr	x1, [x1]
     660:	39400000 	ldrb	w0, [x0]
     664:	0b003660 	add	w0, w19, w0, lsl #13
     668:	51403400 	sub	w0, w0, #0xd, lsl #12
     66c:	3820c834 	strb	w20, [x1, w0, sxtw]
            return;
     670:	17ffffca 	b	598 <_write_item+0x78>
            WRAM_SET(0, address - 0xE000, value);
     674:	90000000 	adrp	x0, 0 <WRAM>
     678:	f9400000 	ldr	x0, [x0]
     67c:	51403a73 	sub	w19, w19, #0xe, lsl #12
     680:	3833c814 	strb	w20, [x0, w19, sxtw]
            return;
     684:	17ffffc5 	b	598 <_write_item+0x78>
            WRAM_SET(WRAM_CURRENT_BANK, address - 0xF000, value);
     688:	90000000 	adrp	x0, 0 <WRAM_CURRENT_BANK>
     68c:	f9400000 	ldr	x0, [x0]
     690:	90000001 	adrp	x1, 0 <WRAM>
     694:	f9400021 	ldr	x1, [x1]
     698:	39400000 	ldrb	w0, [x0]
     69c:	0b003660 	add	w0, w19, w0, lsl #13
     6a0:	51403c00 	sub	w0, w0, #0xf, lsl #12
     6a4:	3820c834 	strb	w20, [x1, w0, sxtw]
            return;
     6a8:	17ffffbc 	b	598 <_write_item+0x78>
            IE = value;
     6ac:	90000000 	adrp	x0, 0 <IE>
     6b0:	f9400000 	ldr	x0, [x0]
     6b4:	39000014 	strb	w20, [x0]
            return;
     6b8:	17ffffb8 	b	598 <_write_item+0x78>
            SRAM_SET(SRAM_CURRENT_BANK, address - 0xA000, value);
     6bc:	90000000 	adrp	x0, 0 <SRAM_CURRENT_BANK>
     6c0:	f9400000 	ldr	x0, [x0]
     6c4:	90000001 	adrp	x1, 0 <SRAM>
     6c8:	f9400021 	ldr	x1, [x1]
     6cc:	39400000 	ldrb	w0, [x0]
     6d0:	0b003660 	add	w0, w19, w0, lsl #13
     6d4:	51402800 	sub	w0, w0, #0xa, lsl #12
     6d8:	3820c834 	strb	w20, [x1, w0, sxtw]
            return;
     6dc:	17ffffaf 	b	598 <_write_item+0x78>
            WRAM_SET(0, address - 0xC000, value);
     6e0:	90000000 	adrp	x0, 0 <WRAM>
     6e4:	f9400000 	ldr	x0, [x0]
     6e8:	51403273 	sub	w19, w19, #0xc, lsl #12
     6ec:	3833c814 	strb	w20, [x0, w19, sxtw]
            return;
     6f0:	17ffffaa 	b	598 <_write_item+0x78>
     6f4:	f90013f5 	str	x21, [sp, #32]
     6f8:	90000015 	adrp	x21, 0 <IO>
     6fc:	f94002b5 	ldr	x21, [x21]
     700:	17ffffc0 	b	600 <_write_item+0xe0>

0000000000000704 <_execute>:

inline opcycles_t _execute(opcode_t opcode, uint16_t value){
     704:	a9bd7bfd 	stp	x29, x30, [sp, #-48]!
     708:	12001c00 	and	w0, w0, #0xff
     70c:	910003fd 	mov	x29, sp
     710:	a90153f3 	stp	x19, x20, [sp, #16]
     714:	12003c33 	and	w19, w1, #0xffff
    bool prev_carry = false;
    
    switch (opcode) {
     718:	90000001 	adrp	x1, 0 <_load_rom>
     71c:	91000021 	add	x1, x1, #0x0
     720:	78605821 	ldrh	w1, [x1, w0, uxtw #1]
     724:	10000062 	adr	x2, 730 <_execute+0x2c>
     728:	8b21a841 	add	x1, x2, w1, sxth #2
     72c:	d61f0020 	br	x1
            REG_SP -= 2;
            REG_PC = 0x0038;
            return MCYCLE_4;

        default:
            log_error("Opcode: %02X[%d length], Value: %04X | %s", opcode, OPCODE_LENGTH[(size_t)opcode], value, OPCODE_NAMES[(size_t)opcode]);
     730:	90000002 	adrp	x2, 0 <OPCODE_NAMES>
     734:	f9400042 	ldr	x2, [x2]
     738:	2a0003e3 	mov	w3, w0
     73c:	90000001 	adrp	x1, 0 <OPCODE_LENGTH>
     740:	f9400021 	ldr	x1, [x1]
     744:	2a0003e4 	mov	w4, w0
     748:	f8637847 	ldr	x7, [x2, x3, lsl #3]
     74c:	2a1303e6 	mov	w6, w19
     750:	38636825 	ldrb	w5, [x1, x3]
     754:	90000003 	adrp	x3, 0 <_load_rom>
     758:	90000001 	adrp	x1, 0 <_load_rom>
     75c:	91000063 	add	x3, x3, #0x0
     760:	91000021 	add	x1, x1, #0x0
     764:	52805822 	mov	w2, #0x2c1                 	// #705
     768:	52800080 	mov	w0, #0x4                   	// #4
     76c:	f90013f5 	str	x21, [sp, #32]
     770:	94000000 	bl	0 <log_log>
            exit(1);
     774:	52800020 	mov	w0, #0x1                   	// #1
     778:	94000000 	bl	0 <exit>
            if (!FLAG_Z_IS_SET()){
     77c:	90000000 	adrp	x0, 0 <REG_F>
     780:	f9400000 	ldr	x0, [x0]
     784:	39c00000 	ldrsb	w0, [x0]
     788:	37f80ea0 	tbnz	w0, #31, 95c <_execute+0x258>
                REG_PC += U8(value);
     78c:	90000001 	adrp	x1, 0 <REG_PC>
     790:	f9400021 	ldr	x1, [x1]
     794:	79400020 	ldrh	w0, [x1]
     798:	0b330000 	add	w0, w0, w19, uxtb
     79c:	79000020 	strh	w0, [x1]
            return MCYCLE_3;
     7a0:	d2800060 	mov	x0, #0x3                   	// #3
    }
}
     7a4:	a94153f3 	ldp	x19, x20, [sp, #16]
     7a8:	a8c37bfd 	ldp	x29, x30, [sp], #48
     7ac:	d65f03c0 	ret
            INC_BC();
     7b0:	90000001 	adrp	x1, 0 <REG_C>
     7b4:	f9400021 	ldr	x1, [x1]
     7b8:	39400020 	ldrb	w0, [x1]
     7bc:	11000400 	add	w0, w0, #0x1
     7c0:	12001c00 	and	w0, w0, #0xff
     7c4:	39000020 	strb	w0, [x1]
     7c8:	350000c0 	cbnz	w0, 7e0 <_execute+0xdc>
     7cc:	90000001 	adrp	x1, 0 <REG_B>
     7d0:	f9400021 	ldr	x1, [x1]
     7d4:	39400020 	ldrb	w0, [x1]
     7d8:	11000400 	add	w0, w0, #0x1
     7dc:	39000020 	strb	w0, [x1]
            return MCYCLE_1;
     7e0:	d2800020 	mov	x0, #0x1                   	// #1
     7e4:	17fffff0 	b	7a4 <_execute+0xa0>
            if (!FLAG_Z_IS_SET()){
     7e8:	90000000 	adrp	x0, 0 <REG_F>
     7ec:	f9400000 	ldr	x0, [x0]
     7f0:	39c00000 	ldrsb	w0, [x0]
     7f4:	37fffd60 	tbnz	w0, #31, 7a0 <_execute+0x9c>
            REG_PC = value;
     7f8:	90000000 	adrp	x0, 0 <REG_PC>
     7fc:	f9400000 	ldr	x0, [x0]
     800:	79000013 	strh	w19, [x0]
                return MCYCLE_4;
     804:	d2800080 	mov	x0, #0x4                   	// #4
     808:	17ffffe7 	b	7a4 <_execute+0xa0>
            if (FLAG_Z_IS_SET()){
     80c:	90000000 	adrp	x0, 0 <REG_F>
     810:	f9400000 	ldr	x0, [x0]
     814:	39c00000 	ldrsb	w0, [x0]
     818:	36f80a20 	tbz	w0, #31, 95c <_execute+0x258>
            REG_PC = READ_MEM(REG_SP + 1) << 8 | READ_MEM(REG_SP);
     81c:	90000007 	adrp	x7, 0 <REG_SP>
     820:	f94000e7 	ldr	x7, [x7]
     824:	794000e6 	ldrh	w6, [x7]
     828:	110004c0 	add	w0, w6, #0x1
     82c:	94000000 	bl	210 <_fetch_item>
     830:	12001c05 	and	w5, w0, #0xff
     834:	2a0603e0 	mov	w0, w6
     838:	94000000 	bl	210 <_fetch_item>
     83c:	12001c00 	and	w0, w0, #0xff
     840:	90000001 	adrp	x1, 0 <REG_PC>
     844:	f9400021 	ldr	x1, [x1]
     848:	2a052005 	orr	w5, w0, w5, lsl #8
            REG_SP += 2;
     84c:	110008c6 	add	w6, w6, #0x2
                return MCYCLE_4;
     850:	d2800080 	mov	x0, #0x4                   	// #4
            REG_SP += 2;
     854:	790000e6 	strh	w6, [x7]
            REG_PC = READ_MEM(REG_SP + 1) << 8 | READ_MEM(REG_SP);
     858:	79000025 	strh	w5, [x1]
            return MCYCLE_4;
     85c:	17ffffd2 	b	7a4 <_execute+0xa0>
            REG_E = REG_L;
     860:	90000000 	adrp	x0, 0 <REG_L>
     864:	f9400000 	ldr	x0, [x0]
     868:	90000001 	adrp	x1, 0 <REG_E>
     86c:	f9400021 	ldr	x1, [x1]
     870:	39400000 	ldrb	w0, [x0]
     874:	39000020 	strb	w0, [x1]
            return MCYCLE_1;
     878:	d2800020 	mov	x0, #0x1                   	// #1
     87c:	17ffffca 	b	7a4 <_execute+0xa0>
            REG_D = REG_A;
     880:	90000000 	adrp	x0, 0 <REG_A>
     884:	f9400000 	ldr	x0, [x0]
     888:	90000001 	adrp	x1, 0 <REG_D>
     88c:	f9400021 	ldr	x1, [x1]
     890:	39400000 	ldrb	w0, [x0]
     894:	39000020 	strb	w0, [x1]
            return MCYCLE_1;
     898:	d2800020 	mov	x0, #0x1                   	// #1
     89c:	17ffffc2 	b	7a4 <_execute+0xa0>
            WRITE_MEM(0xFF00 + value, REG_A);
     8a0:	90000001 	adrp	x1, 0 <REG_A>
     8a4:	f9400021 	ldr	x1, [x1]
     8a8:	51040260 	sub	w0, w19, #0x100
     8ac:	39400021 	ldrb	w1, [x1]
     8b0:	94000000 	bl	520 <_write_item>
            return MCYCLE_3;
     8b4:	17ffffbb 	b	7a0 <_execute+0x9c>
            REG_B--;
     8b8:	90000001 	adrp	x1, 0 <REG_B>
     8bc:	f9400021 	ldr	x1, [x1]
     8c0:	39400020 	ldrb	w0, [x1]
     8c4:	51000400 	sub	w0, w0, #0x1
     8c8:	39000020 	strb	w0, [x1]
            return MCYCLE_1;
     8cc:	d2800020 	mov	x0, #0x1                   	// #1
     8d0:	17ffffb5 	b	7a4 <_execute+0xa0>
            REG_D = U8(value >> 8);
     8d4:	90000001 	adrp	x1, 0 <REG_D>
     8d8:	f9400021 	ldr	x1, [x1]
     8dc:	53087e62 	lsr	w2, w19, #8
            REG_E = U8(value & 0xFF);
     8e0:	90000000 	adrp	x0, 0 <REG_E>
     8e4:	f9400000 	ldr	x0, [x0]
            REG_D = U8(value >> 8);
     8e8:	39000022 	strb	w2, [x1]
            REG_E = U8(value & 0xFF);
     8ec:	39000013 	strb	w19, [x0]
            return MCYCLE_3;
     8f0:	17ffffac 	b	7a0 <_execute+0x9c>
            CPU_HALTED = true;
     8f4:	90000001 	adrp	x1, 0 <_load_rom>
     8f8:	91000022 	add	x2, x1, #0x0
     8fc:	52800020 	mov	w0, #0x1                   	// #1
     900:	39000020 	strb	w0, [x1]
            CPU_STUCK = true;
     904:	39000440 	strb	w0, [x2, #1]
            return MCYCLE_1;
     908:	d2800020 	mov	x0, #0x1                   	// #1
     90c:	17ffffa6 	b	7a4 <_execute+0xa0>
            REG_D = REG_L;
     910:	90000000 	adrp	x0, 0 <REG_L>
     914:	f9400000 	ldr	x0, [x0]
     918:	90000001 	adrp	x1, 0 <REG_D>
     91c:	f9400021 	ldr	x1, [x1]
     920:	39400000 	ldrb	w0, [x0]
     924:	39000020 	strb	w0, [x1]
            return MCYCLE_1;
     928:	d2800020 	mov	x0, #0x1                   	// #1
     92c:	17ffff9e 	b	7a4 <_execute+0xa0>
            REG_D = REG_H;
     930:	90000000 	adrp	x0, 0 <REG_H>
     934:	f9400000 	ldr	x0, [x0]
     938:	90000001 	adrp	x1, 0 <REG_D>
     93c:	f9400021 	ldr	x1, [x1]
     940:	39400000 	ldrb	w0, [x0]
     944:	39000020 	strb	w0, [x1]
            return MCYCLE_1;
     948:	d2800020 	mov	x0, #0x1                   	// #1
     94c:	17ffff96 	b	7a4 <_execute+0xa0>
            REG_A = U8(value);
     950:	90000000 	adrp	x0, 0 <REG_A>
     954:	f9400000 	ldr	x0, [x0]
     958:	39000013 	strb	w19, [x0]
            return MCYCLE_2;
     95c:	d2800040 	mov	x0, #0x2                   	// #2
     960:	17ffff91 	b	7a4 <_execute+0xa0>
            REG_A++;
     964:	90000001 	adrp	x1, 0 <REG_A>
     968:	f9400021 	ldr	x1, [x1]
     96c:	39400020 	ldrb	w0, [x1]
     970:	11000400 	add	w0, w0, #0x1
     974:	39000020 	strb	w0, [x1]
            return MCYCLE_1;
     978:	d2800020 	mov	x0, #0x1                   	// #1
     97c:	17ffff8a 	b	7a4 <_execute+0xa0>
            if (FLAG_C_IS_SET()){
     980:	90000000 	adrp	x0, 0 <REG_F>
     984:	f9400000 	ldr	x0, [x0]
     988:	39400000 	ldrb	w0, [x0]
     98c:	3727f000 	tbnz	w0, #4, 78c <_execute+0x88>
            return MCYCLE_2;
     990:	d2800040 	mov	x0, #0x2                   	// #2
     994:	17ffff84 	b	7a4 <_execute+0xa0>
            FLAG_C_SET();
     998:	90000001 	adrp	x1, 0 <REG_F>
     99c:	f9400021 	ldr	x1, [x1]
     9a0:	39400020 	ldrb	w0, [x1]
     9a4:	321c0000 	orr	w0, w0, #0x10
     9a8:	39000020 	strb	w0, [x1]
            return MCYCLE_1;
     9ac:	d2800020 	mov	x0, #0x1                   	// #1
     9b0:	17ffff7d 	b	7a4 <_execute+0xa0>
            WRITE_MEM(HL(), U8(value));
     9b4:	90000000 	adrp	x0, 0 <REG_H>
     9b8:	f9400000 	ldr	x0, [x0]
     9bc:	2a1303e1 	mov	w1, w19
     9c0:	90000002 	adrp	x2, 0 <REG_L>
     9c4:	f9400042 	ldr	x2, [x2]
     9c8:	39400000 	ldrb	w0, [x0]
     9cc:	39400042 	ldrb	w2, [x2]
     9d0:	53181c00 	ubfiz	w0, w0, #8, #8
     9d4:	2a020000 	orr	w0, w0, w2
     9d8:	94000000 	bl	520 <_write_item>
            return MCYCLE_3;
     9dc:	17ffff71 	b	7a0 <_execute+0x9c>
            REG_SP = U16(value);
     9e0:	90000000 	adrp	x0, 0 <REG_SP>
     9e4:	f9400000 	ldr	x0, [x0]
     9e8:	79000013 	strh	w19, [x0]
            return MCYCLE_2;
     9ec:	d2800040 	mov	x0, #0x2                   	// #2
     9f0:	17ffff6d 	b	7a4 <_execute+0xa0>
            FLAG_Z_RESET();
     9f4:	90000000 	adrp	x0, 0 <REG_F>
     9f8:	f9400000 	ldr	x0, [x0]
            if BIT_IS_SET(REG_A, 7){
     9fc:	90000001 	adrp	x1, 0 <REG_A>
     a00:	f9400021 	ldr	x1, [x1]
            FLAG_Z_RESET();
     a04:	39400005 	ldrb	w5, [x0]
            if BIT_IS_SET(REG_A, 7){
     a08:	39400022 	ldrb	w2, [x1]
                FLAG_C_SET();
     a0c:	120010a6 	and	w6, w5, #0x1f
     a10:	321c00c6 	orr	w6, w6, #0x10
     a14:	12000ca3 	and	w3, w5, #0xf
     a18:	f279005f 	tst	x2, #0x80
            REG_A = (REG_A << 1) & 0xff;
     a1c:	531f1844 	ubfiz	w4, w2, #1, #7
                FLAG_C_SET();
     a20:	1a860063 	csel	w3, w3, w6, eq	// eq = none
            REG_A = (REG_A << 1) & 0xff;
     a24:	32000084 	orr	w4, w4, #0x1
     a28:	f27c00bf 	tst	x5, #0x10
     a2c:	531f1842 	ubfiz	w2, w2, #1, #7
     a30:	1a840042 	csel	w2, w2, w4, eq	// eq = none
     a34:	39000003 	strb	w3, [x0]
            return MCYCLE_1;
     a38:	d2800020 	mov	x0, #0x1                   	// #1
            REG_A = (REG_A << 1) & 0xff;
     a3c:	39000022 	strb	w2, [x1]
     a40:	17ffff59 	b	7a4 <_execute+0xa0>
            REG_D = U8(value);
     a44:	90000000 	adrp	x0, 0 <REG_D>
     a48:	f9400000 	ldr	x0, [x0]
     a4c:	39000013 	strb	w19, [x0]
            return MCYCLE_2;
     a50:	d2800040 	mov	x0, #0x2                   	// #2
     a54:	17ffff54 	b	7a4 <_execute+0xa0>
            REG_D--;
     a58:	90000001 	adrp	x1, 0 <REG_D>
     a5c:	f9400021 	ldr	x1, [x1]
     a60:	39400020 	ldrb	w0, [x1]
     a64:	51000400 	sub	w0, w0, #0x1
     a68:	39000020 	strb	w0, [x1]
            return MCYCLE_1;
     a6c:	d2800020 	mov	x0, #0x1                   	// #1
     a70:	17ffff4d 	b	7a4 <_execute+0xa0>
            REG_D++;
     a74:	90000001 	adrp	x1, 0 <REG_D>
     a78:	f9400021 	ldr	x1, [x1]
     a7c:	39400020 	ldrb	w0, [x1]
     a80:	11000400 	add	w0, w0, #0x1
     a84:	39000020 	strb	w0, [x1]
            return MCYCLE_1;
     a88:	d2800020 	mov	x0, #0x1                   	// #1
     a8c:	17ffff46 	b	7a4 <_execute+0xa0>
            INC_DE();
     a90:	90000001 	adrp	x1, 0 <REG_E>
     a94:	f9400021 	ldr	x1, [x1]
     a98:	39400020 	ldrb	w0, [x1]
     a9c:	11000400 	add	w0, w0, #0x1
     aa0:	12001c00 	and	w0, w0, #0xff
     aa4:	39000020 	strb	w0, [x1]
     aa8:	35fff5a0 	cbnz	w0, 95c <_execute+0x258>
     aac:	90000001 	adrp	x1, 0 <REG_D>
     ab0:	f9400021 	ldr	x1, [x1]
     ab4:	39400020 	ldrb	w0, [x1]
     ab8:	11000400 	add	w0, w0, #0x1
     abc:	39000020 	strb	w0, [x1]
            return MCYCLE_2;
     ac0:	d2800040 	mov	x0, #0x2                   	// #2
     ac4:	17ffff38 	b	7a4 <_execute+0xa0>
            WRITE_MEM(DE(), REG_A);
     ac8:	90000000 	adrp	x0, 0 <REG_D>
     acc:	f9400000 	ldr	x0, [x0]
     ad0:	90000001 	adrp	x1, 0 <REG_A>
     ad4:	f9400021 	ldr	x1, [x1]
     ad8:	90000002 	adrp	x2, 0 <REG_E>
     adc:	f9400042 	ldr	x2, [x2]
     ae0:	39400000 	ldrb	w0, [x0]
     ae4:	39400042 	ldrb	w2, [x2]
     ae8:	39400021 	ldrb	w1, [x1]
     aec:	53181c00 	ubfiz	w0, w0, #8, #8
     af0:	2a020000 	orr	w0, w0, w2
     af4:	94000000 	bl	520 <_write_item>
            return MCYCLE_2;
     af8:	d2800040 	mov	x0, #0x2                   	// #2
     afc:	17ffff2a 	b	7a4 <_execute+0xa0>
            WRITE_MEM(REG_SP - 1, REG_PC >> 8);
     b00:	90000014 	adrp	x20, 0 <REG_SP>
     b04:	f9400294 	ldr	x20, [x20]
     b08:	79400280 	ldrh	w0, [x20]
     b0c:	f90013f5 	str	x21, [sp, #32]
     b10:	90000015 	adrp	x21, 0 <REG_PC>
     b14:	f94002b5 	ldr	x21, [x21]
     b18:	51000400 	sub	w0, w0, #0x1
     b1c:	394006a1 	ldrb	w1, [x21, #1]
     b20:	94000000 	bl	520 <_write_item>
            WRITE_MEM(REG_SP - 2, REG_PC & 0xFF);
     b24:	394002a1 	ldrb	w1, [x21]
     b28:	79400280 	ldrh	w0, [x20]
     b2c:	51000800 	sub	w0, w0, #0x2
     b30:	94000000 	bl	520 <_write_item>
            REG_PC = value;
     b34:	790002b3 	strh	w19, [x21]
            REG_SP -= 2;
     b38:	79400281 	ldrh	w1, [x20]
            return MCYCLE_6;
     b3c:	d28000c0 	mov	x0, #0x6                   	// #6
     b40:	f94013f5 	ldr	x21, [sp, #32]
            REG_SP -= 2;
     b44:	51000821 	sub	w1, w1, #0x2
     b48:	79000281 	strh	w1, [x20]
            return MCYCLE_6;
     b4c:	17ffff16 	b	7a4 <_execute+0xa0>
            REG_L = READ_MEM(REG_SP);
     b50:	90000006 	adrp	x6, 0 <REG_SP>
     b54:	f94000c6 	ldr	x6, [x6]
     b58:	794000c5 	ldrh	w5, [x6]
     b5c:	2a0503e0 	mov	w0, w5
     b60:	94000000 	bl	210 <_fetch_item>
     b64:	90000001 	adrp	x1, 0 <REG_L>
     b68:	f9400021 	ldr	x1, [x1]
     b6c:	2a0003e2 	mov	w2, w0
            REG_H = READ_MEM(REG_SP + 1);
     b70:	110004a0 	add	w0, w5, #0x1
            REG_SP += 2;
     b74:	110008a5 	add	w5, w5, #0x2
            REG_L = READ_MEM(REG_SP);
     b78:	39000022 	strb	w2, [x1]
            REG_H = READ_MEM(REG_SP + 1);
     b7c:	94000000 	bl	210 <_fetch_item>
     b80:	90000001 	adrp	x1, 0 <REG_H>
     b84:	f9400021 	ldr	x1, [x1]
            REG_SP += 2;
     b88:	790000c5 	strh	w5, [x6]
            REG_H = READ_MEM(REG_SP + 1);
     b8c:	39000020 	strb	w0, [x1]
            return MCYCLE_3;
     b90:	17ffff04 	b	7a0 <_execute+0x9c>
            REG_A = REG_B;
     b94:	90000000 	adrp	x0, 0 <REG_B>
     b98:	f9400000 	ldr	x0, [x0]
     b9c:	90000001 	adrp	x1, 0 <REG_A>
     ba0:	f9400021 	ldr	x1, [x1]
     ba4:	39400000 	ldrb	w0, [x0]
     ba8:	39000020 	strb	w0, [x1]
            return MCYCLE_1;
     bac:	d2800020 	mov	x0, #0x1                   	// #1
     bb0:	17fffefd 	b	7a4 <_execute+0xa0>
            REG_L = REG_A;
     bb4:	90000000 	adrp	x0, 0 <REG_A>
     bb8:	f9400000 	ldr	x0, [x0]
     bbc:	90000001 	adrp	x1, 0 <REG_L>
     bc0:	f9400021 	ldr	x1, [x1]
     bc4:	39400000 	ldrb	w0, [x0]
     bc8:	39000020 	strb	w0, [x1]
            return MCYCLE_1;
     bcc:	d2800020 	mov	x0, #0x1                   	// #1
     bd0:	17fffef5 	b	7a4 <_execute+0xa0>
            REG_L = REG_E;
     bd4:	90000000 	adrp	x0, 0 <REG_E>
     bd8:	f9400000 	ldr	x0, [x0]
     bdc:	90000001 	adrp	x1, 0 <REG_L>
     be0:	f9400021 	ldr	x1, [x1]
     be4:	39400000 	ldrb	w0, [x0]
     be8:	39000020 	strb	w0, [x1]
            return MCYCLE_1;
     bec:	d2800020 	mov	x0, #0x1                   	// #1
     bf0:	17fffeed 	b	7a4 <_execute+0xa0>
            REG_H = REG_A;
     bf4:	90000000 	adrp	x0, 0 <REG_A>
     bf8:	f9400000 	ldr	x0, [x0]
     bfc:	90000001 	adrp	x1, 0 <REG_H>
     c00:	f9400021 	ldr	x1, [x1]
     c04:	39400000 	ldrb	w0, [x0]
     c08:	39000020 	strb	w0, [x1]
            return MCYCLE_1;
     c0c:	d2800020 	mov	x0, #0x1                   	// #1
     c10:	17fffee5 	b	7a4 <_execute+0xa0>
            REG_H = REG_D;
     c14:	90000000 	adrp	x0, 0 <REG_D>
     c18:	f9400000 	ldr	x0, [x0]
     c1c:	90000001 	adrp	x1, 0 <REG_H>
     c20:	f9400021 	ldr	x1, [x1]
     c24:	39400000 	ldrb	w0, [x0]
     c28:	39000020 	strb	w0, [x1]
            return MCYCLE_1;
     c2c:	d2800020 	mov	x0, #0x1                   	// #1
     c30:	17fffedd 	b	7a4 <_execute+0xa0>
            REG_E = REG_A;
     c34:	90000000 	adrp	x0, 0 <REG_A>
     c38:	f9400000 	ldr	x0, [x0]
     c3c:	90000001 	adrp	x1, 0 <REG_E>
     c40:	f9400021 	ldr	x1, [x1]
     c44:	39400000 	ldrb	w0, [x0]
     c48:	39000020 	strb	w0, [x1]
            return MCYCLE_1;
     c4c:	d2800020 	mov	x0, #0x1                   	// #1
     c50:	17fffed5 	b	7a4 <_execute+0xa0>
            REG_B = READ_MEM(REG_SP + 1);
     c54:	90000006 	adrp	x6, 0 <REG_SP>
     c58:	f94000c6 	ldr	x6, [x6]
     c5c:	794000c5 	ldrh	w5, [x6]
     c60:	110004a0 	add	w0, w5, #0x1
     c64:	94000000 	bl	210 <_fetch_item>
     c68:	90000001 	adrp	x1, 0 <REG_B>
     c6c:	f9400021 	ldr	x1, [x1]
     c70:	2a0003e2 	mov	w2, w0
            REG_C = READ_MEM(REG_SP);
     c74:	2a0503e0 	mov	w0, w5
            REG_SP += 2;
     c78:	110008a5 	add	w5, w5, #0x2
            REG_B = READ_MEM(REG_SP + 1);
     c7c:	39000022 	strb	w2, [x1]
            REG_C = READ_MEM(REG_SP);
     c80:	94000000 	bl	210 <_fetch_item>
     c84:	90000001 	adrp	x1, 0 <REG_C>
     c88:	f9400021 	ldr	x1, [x1]
            REG_SP += 2;
     c8c:	790000c5 	strh	w5, [x6]
            REG_C = READ_MEM(REG_SP);
     c90:	39000020 	strb	w0, [x1]
            return MCYCLE_3;
     c94:	17fffec3 	b	7a0 <_execute+0x9c>
            REG_SP = (address_t)HL();
     c98:	90000000 	adrp	x0, 0 <REG_H>
     c9c:	f9400000 	ldr	x0, [x0]
     ca0:	90000002 	adrp	x2, 0 <REG_L>
     ca4:	f9400042 	ldr	x2, [x2]
     ca8:	39400000 	ldrb	w0, [x0]
     cac:	90000001 	adrp	x1, 0 <REG_SP>
     cb0:	f9400021 	ldr	x1, [x1]
     cb4:	39400042 	ldrb	w2, [x2]
     cb8:	53181c00 	ubfiz	w0, w0, #8, #8
     cbc:	2a020000 	orr	w0, w0, w2
     cc0:	79000020 	strh	w0, [x1]
            return MCYCLE_2;
     cc4:	d2800040 	mov	x0, #0x2                   	// #2
     cc8:	17fffeb7 	b	7a4 <_execute+0xa0>
            REG_H = U8((REG_SP + i8(value)) >> 8);
     ccc:	90000000 	adrp	x0, 0 <REG_SP>
     cd0:	f9400000 	ldr	x0, [x0]
            REG_L = U8((REG_SP + i8(value)) & 0xFF);
     cd4:	90000002 	adrp	x2, 0 <REG_L>
     cd8:	f9400042 	ldr	x2, [x2]
            REG_H = U8((REG_SP + i8(value)) >> 8);
     cdc:	79400000 	ldrh	w0, [x0]
     ce0:	90000001 	adrp	x1, 0 <REG_H>
     ce4:	f9400021 	ldr	x1, [x1]
            REG_L = U8((REG_SP + i8(value)) & 0xFF);
     ce8:	0b130003 	add	w3, w0, w19
            REG_H = U8((REG_SP + i8(value)) >> 8);
     cec:	0b338000 	add	w0, w0, w19, sxtb
            REG_L = U8((REG_SP + i8(value)) & 0xFF);
     cf0:	39000043 	strb	w3, [x2]
            REG_H = U8((REG_SP + i8(value)) >> 8);
     cf4:	13087c00 	asr	w0, w0, #8
     cf8:	39000020 	strb	w0, [x1]
            return MCYCLE_3;
     cfc:	17fffea9 	b	7a0 <_execute+0x9c>
            OR_SET_FLAGS(REG_A, value);
     d00:	90000000 	adrp	x0, 0 <REG_F>
     d04:	f9400000 	ldr	x0, [x0]
     d08:	90000001 	adrp	x1, 0 <REG_A>
     d0c:	f9400021 	ldr	x1, [x1]
     d10:	39400002 	ldrb	w2, [x0]
     d14:	39400023 	ldrb	w3, [x1]
     d18:	32196044 	orr	w4, w2, #0xffffff80
     d1c:	12001c84 	and	w4, w4, #0xff
     d20:	12001842 	and	w2, w2, #0x7f
     d24:	7100007f 	cmp	w3, #0x0
            REG_A |= (uint8_t)value;
     d28:	2a130063 	orr	w3, w3, w19
            OR_SET_FLAGS(REG_A, value);
     d2c:	1a841042 	csel	w2, w2, w4, ne	// ne = any
            REG_A |= (uint8_t)value;
     d30:	39000023 	strb	w3, [x1]
            OR_SET_FLAGS(REG_A, value);
     d34:	12197042 	and	w2, w2, #0xffffff8f
     d38:	39000002 	strb	w2, [x0]
            return MCYCLE_2;
     d3c:	d2800040 	mov	x0, #0x2                   	// #2
     d40:	17fffe99 	b	7a4 <_execute+0xa0>
            WRITE_MEM(REG_SP-1, REG_A);
     d44:	90000000 	adrp	x0, 0 <REG_A>
     d48:	f9400000 	ldr	x0, [x0]
     d4c:	90000013 	adrp	x19, 0 <REG_SP>
     d50:	f9400273 	ldr	x19, [x19]
     d54:	39400001 	ldrb	w1, [x0]
     d58:	79400260 	ldrh	w0, [x19]
     d5c:	51000400 	sub	w0, w0, #0x1
     d60:	94000000 	bl	520 <_write_item>
            WRITE_MEM(REG_SP-2, REG_F);
     d64:	90000001 	adrp	x1, 0 <REG_F>
     d68:	f9400021 	ldr	x1, [x1]
     d6c:	79400260 	ldrh	w0, [x19]
     d70:	39400021 	ldrb	w1, [x1]
     d74:	51000800 	sub	w0, w0, #0x2
     d78:	94000000 	bl	520 <_write_item>
            REG_SP -= 2;
     d7c:	79400260 	ldrh	w0, [x19]
     d80:	51000800 	sub	w0, w0, #0x2
     d84:	79000260 	strh	w0, [x19]
                return MCYCLE_4;
     d88:	d2800080 	mov	x0, #0x4                   	// #4
     d8c:	17fffe86 	b	7a4 <_execute+0xa0>
            IE = 0x00;
     d90:	90000000 	adrp	x0, 0 <IE>
     d94:	f9400000 	ldr	x0, [x0]
     d98:	3900001f 	strb	wzr, [x0]
            return MCYCLE_1;
     d9c:	d2800020 	mov	x0, #0x1                   	// #1
     da0:	17fffe81 	b	7a4 <_execute+0xa0>
            REG_H = U8(value >> 8);
     da4:	90000001 	adrp	x1, 0 <REG_H>
     da8:	f9400021 	ldr	x1, [x1]
     dac:	53087e62 	lsr	w2, w19, #8
            REG_L = U8(value & 0xFF);
     db0:	90000000 	adrp	x0, 0 <REG_L>
     db4:	f9400000 	ldr	x0, [x0]
            REG_H = U8(value >> 8);
     db8:	39000022 	strb	w2, [x1]
            REG_L = U8(value & 0xFF);
     dbc:	39000013 	strb	w19, [x0]
            return MCYCLE_3;
     dc0:	17fffe78 	b	7a0 <_execute+0x9c>
            if (!FLAG_C_IS_SET()){  
     dc4:	90000000 	adrp	x0, 0 <REG_F>
     dc8:	f9400000 	ldr	x0, [x0]
     dcc:	39400000 	ldrb	w0, [x0]
     dd0:	3627cde0 	tbz	w0, #4, 78c <_execute+0x88>
            return MCYCLE_2;
     dd4:	d2800040 	mov	x0, #0x2                   	// #2
     dd8:	17fffe73 	b	7a4 <_execute+0xa0>
            CP_SET_FLAGS(REG_A, value);
     ddc:	90000000 	adrp	x0, 0 <REG_F>
     de0:	f9400000 	ldr	x0, [x0]
     de4:	528009e3 	mov	w3, #0x4f                  	// #79
     de8:	90000001 	adrp	x1, 0 <REG_A>
     dec:	f9400021 	ldr	x1, [x1]
     df0:	39400002 	ldrb	w2, [x0]
     df4:	39400021 	ldrb	w1, [x1]
     df8:	0a020063 	and	w3, w3, w2
     dfc:	321a0062 	orr	w2, w3, #0x40
     e00:	39000002 	strb	w2, [x0]
     e04:	4b130024 	sub	w4, w1, w19
     e08:	72001c9f 	tst	w4, #0xff
     e0c:	54000061 	b.ne	e18 <_execute+0x714>  // b.any
     e10:	321a0462 	orr	w2, w3, #0xc0
     e14:	39000002 	strb	w2, [x0]
     e18:	4a130021 	eor	w1, w1, w19
     e1c:	4a040021 	eor	w1, w1, w4
     e20:	36200061 	tbz	w1, #4, e2c <_execute+0x728>
     e24:	321b0042 	orr	w2, w2, #0x20
     e28:	39000002 	strb	w2, [x0]
     e2c:	3647d984 	tbz	w4, #8, 95c <_execute+0x258>
     e30:	321c0042 	orr	w2, w2, #0x10
     e34:	39000002 	strb	w2, [x0]
            return MCYCLE_2;
     e38:	d2800040 	mov	x0, #0x2                   	// #2
     e3c:	17fffe5a 	b	7a4 <_execute+0xa0>
            REG_A = READ_MEM((address_t)value);
     e40:	2a1303e0 	mov	w0, w19
     e44:	94000000 	bl	210 <_fetch_item>
     e48:	90000001 	adrp	x1, 0 <REG_A>
     e4c:	f9400021 	ldr	x1, [x1]
     e50:	39000020 	strb	w0, [x1]
                return MCYCLE_4;
     e54:	d2800080 	mov	x0, #0x4                   	// #4
     e58:	17fffe53 	b	7a4 <_execute+0xa0>
            FLAG_Z_RESET();
     e5c:	90000002 	adrp	x2, 0 <REG_F>
     e60:	f9400042 	ldr	x2, [x2]
            if BIT_IS_SET(REG_A, 7){
     e64:	90000003 	adrp	x3, 0 <REG_A>
     e68:	f9400063 	ldr	x3, [x3]
            FLAG_C_RESET();
     e6c:	39400040 	ldrb	w0, [x2]
            if BIT_IS_SET(REG_A, 7){
     e70:	39400061 	ldrb	w1, [x3]
            FLAG_C_RESET();
     e74:	12000c00 	and	w0, w0, #0xf
     e78:	39000040 	strb	w0, [x2]
                REG_A = (REG_A << 1) + 1;
     e7c:	531f1825 	ubfiz	w5, w1, #1, #7
     e80:	531f1824 	ubfiz	w4, w1, #1, #7
            if BIT_IS_SET(REG_A, 7){
     e84:	37382761 	tbnz	w1, #7, 1370 <_execute+0xc6c>
            return MCYCLE_1;
     e88:	d2800020 	mov	x0, #0x1                   	// #1
                REG_A <<= 1;
     e8c:	39000065 	strb	w5, [x3]
     e90:	17fffe45 	b	7a4 <_execute+0xa0>
            REG_B = value;
     e94:	90000000 	adrp	x0, 0 <REG_B>
     e98:	f9400000 	ldr	x0, [x0]
     e9c:	39000013 	strb	w19, [x0]
            return MCYCLE_2;
     ea0:	d2800040 	mov	x0, #0x2                   	// #2
     ea4:	17fffe40 	b	7a4 <_execute+0xa0>
            WRITE_MEM(BC(), REG_A);
     ea8:	90000000 	adrp	x0, 0 <REG_B>
     eac:	f9400000 	ldr	x0, [x0]
     eb0:	90000001 	adrp	x1, 0 <REG_A>
     eb4:	f9400021 	ldr	x1, [x1]
     eb8:	90000002 	adrp	x2, 0 <REG_C>
     ebc:	f9400042 	ldr	x2, [x2]
     ec0:	17ffff08 	b	ae0 <_execute+0x3dc>
            REG_B = U8(value >> 8);
     ec4:	90000001 	adrp	x1, 0 <REG_B>
     ec8:	f9400021 	ldr	x1, [x1]
     ecc:	53087e62 	lsr	w2, w19, #8
            REG_C = U8(value & 0xFF);
     ed0:	90000000 	adrp	x0, 0 <REG_C>
     ed4:	f9400000 	ldr	x0, [x0]
            REG_B = U8(value >> 8);
     ed8:	39000022 	strb	w2, [x1]
            REG_C = U8(value & 0xFF);
     edc:	39000013 	strb	w19, [x0]
            return MCYCLE_3;
     ee0:	17fffe30 	b	7a0 <_execute+0x9c>
            FLAG_Z_RESET();
     ee4:	90000001 	adrp	x1, 0 <REG_F>
     ee8:	f9400021 	ldr	x1, [x1]
            if BIT_IS_SET(REG_A, 0){
     eec:	90000003 	adrp	x3, 0 <REG_A>
     ef0:	f9400063 	ldr	x3, [x3]
            FLAG_C_RESET();
     ef4:	39400020 	ldrb	w0, [x1]
            if BIT_IS_SET(REG_A, 0){
     ef8:	39400062 	ldrb	w2, [x3]
            FLAG_C_RESET();
     efc:	12000c00 	and	w0, w0, #0xf
     f00:	39000020 	strb	w0, [x1]
                REG_A = (REG_A >> 1) + 0x80;
     f04:	53017c44 	lsr	w4, w2, #1
            if BIT_IS_SET(REG_A, 0){
     f08:	360022e2 	tbz	w2, #0, 1364 <_execute+0xc60>
                FLAG_C_SET();
     f0c:	321c0000 	orr	w0, w0, #0x10
                REG_A = (REG_A >> 1) + 0x80;
     f10:	51020084 	sub	w4, w4, #0x80
                FLAG_C_SET();
     f14:	39000020 	strb	w0, [x1]
            return MCYCLE_1;
     f18:	d2800020 	mov	x0, #0x1                   	// #1
                REG_A = (REG_A >> 1) + 0x80;
     f1c:	39000064 	strb	w4, [x3]
     f20:	17fffe21 	b	7a4 <_execute+0xa0>
            REG_C = value;
     f24:	90000000 	adrp	x0, 0 <REG_C>
     f28:	f9400000 	ldr	x0, [x0]
     f2c:	39000013 	strb	w19, [x0]
            return MCYCLE_2;
     f30:	d2800040 	mov	x0, #0x2                   	// #2
     f34:	17fffe1c 	b	7a4 <_execute+0xa0>
            DEC_BC();
     f38:	90000001 	adrp	x1, 0 <REG_C>
     f3c:	f9400021 	ldr	x1, [x1]
     f40:	39400020 	ldrb	w0, [x1]
     f44:	51000400 	sub	w0, w0, #0x1
     f48:	12001c00 	and	w0, w0, #0xff
     f4c:	39000020 	strb	w0, [x1]
     f50:	7103fc1f 	cmp	w0, #0xff
     f54:	54ffd041 	b.ne	95c <_execute+0x258>  // b.any
     f58:	90000001 	adrp	x1, 0 <REG_B>
     f5c:	f9400021 	ldr	x1, [x1]
     f60:	39400020 	ldrb	w0, [x1]
     f64:	51000400 	sub	w0, w0, #0x1
     f68:	39000020 	strb	w0, [x1]
            return MCYCLE_2;
     f6c:	d2800040 	mov	x0, #0x2                   	// #2
     f70:	17fffe0d 	b	7a4 <_execute+0xa0>
            REG_A = READ_MEM(BC());
     f74:	90000000 	adrp	x0, 0 <REG_B>
     f78:	f9400000 	ldr	x0, [x0]
     f7c:	90000001 	adrp	x1, 0 <REG_C>
     f80:	f9400021 	ldr	x1, [x1]
     f84:	39400000 	ldrb	w0, [x0]
     f88:	39400021 	ldrb	w1, [x1]
     f8c:	53181c00 	ubfiz	w0, w0, #8, #8
     f90:	2a010000 	orr	w0, w0, w1
     f94:	94000000 	bl	210 <_fetch_item>
     f98:	90000001 	adrp	x1, 0 <REG_A>
     f9c:	f9400021 	ldr	x1, [x1]
     fa0:	39000020 	strb	w0, [x1]
            return MCYCLE_2;
     fa4:	d2800040 	mov	x0, #0x2                   	// #2
     fa8:	17fffdff 	b	7a4 <_execute+0xa0>
            ADD_SET_FLAGS16(HL(), BC());
     fac:	90000000 	adrp	x0, 0 <REG_F>
     fb0:	f9400000 	ldr	x0, [x0]
     fb4:	90000004 	adrp	x4, 0 <REG_H>
     fb8:	f9400084 	ldr	x4, [x4]
     fbc:	90000005 	adrp	x5, 0 <REG_L>
     fc0:	f94000a5 	ldr	x5, [x5]
     fc4:	90000007 	adrp	x7, 0 <REG_B>
     fc8:	f94000e7 	ldr	x7, [x7]
     fcc:	90000003 	adrp	x3, 0 <REG_C>
     fd0:	f9400063 	ldr	x3, [x3]
            ADD_SET_FLAGS16(HL(), DE());
     fd4:	39400002 	ldrb	w2, [x0]
     fd8:	39400068 	ldrb	w8, [x3]
     fdc:	394000e7 	ldrb	w7, [x7]
     fe0:	12197042 	and	w2, w2, #0xffffff8f
     fe4:	39400081 	ldrb	w1, [x4]
     fe8:	12001c42 	and	w2, w2, #0xff
     fec:	394000a6 	ldrb	w6, [x5]
     ff0:	39000002 	strb	w2, [x0]
     ff4:	2a07210a 	orr	w10, w8, w7, lsl #8
     ff8:	2a0120c3 	orr	w3, w6, w1, lsl #8
     ffc:	0b0a0069 	add	w9, w3, w10
    1000:	36800069 	tbz	w9, #16, 100c <_execute+0x908>
    1004:	321c0042 	orr	w2, w2, #0x10
    1008:	39000002 	strb	w2, [x0]
    100c:	4a0a0063 	eor	w3, w3, w10
    1010:	4a090063 	eor	w3, w3, w9
    1014:	36600063 	tbz	w3, #12, 1020 <_execute+0x91c>
    1018:	321b0042 	orr	w2, w2, #0x20
    101c:	39000002 	strb	w2, [x0]
            REG_H = U8((U16(HL()) + U16(DE())) >> 8);
    1020:	2a0120c0 	orr	w0, w6, w1, lsl #8
    1024:	2a072107 	orr	w7, w8, w7, lsl #8
    1028:	0b070000 	add	w0, w0, w7
            REG_L = U8((U16(HL()) + U16(DE())) & 0xFF);
    102c:	0b0800c6 	add	w6, w6, w8
    1030:	390000a6 	strb	w6, [x5]
            REG_H = U8((U16(HL()) + U16(DE())) >> 8);
    1034:	13087c00 	asr	w0, w0, #8
    1038:	39000080 	strb	w0, [x4]
            return MCYCLE_2;
    103c:	d2800040 	mov	x0, #0x2                   	// #2
    1040:	17fffdd9 	b	7a4 <_execute+0xa0>
            WRITE_MEM(value, U8(REG_SP & 0xFF));
    1044:	90000014 	adrp	x20, 0 <REG_SP>
    1048:	f9400294 	ldr	x20, [x20]
    104c:	2a1303e0 	mov	w0, w19
    1050:	39400281 	ldrb	w1, [x20]
    1054:	94000000 	bl	520 <_write_item>
            WRITE_MEM(value + 1, U8(REG_SP >> 8));
    1058:	39400681 	ldrb	w1, [x20, #1]
    105c:	11000660 	add	w0, w19, #0x1
    1060:	94000000 	bl	520 <_write_item>
            return MCYCLE_5;
    1064:	d28000a0 	mov	x0, #0x5                   	// #5
    1068:	17fffdcf 	b	7a4 <_execute+0xa0>
            REG_C--;
    106c:	90000001 	adrp	x1, 0 <REG_C>
    1070:	f9400021 	ldr	x1, [x1]
    1074:	39400020 	ldrb	w0, [x1]
    1078:	51000400 	sub	w0, w0, #0x1
    107c:	39000020 	strb	w0, [x1]
            return MCYCLE_1;
    1080:	d2800020 	mov	x0, #0x1                   	// #1
    1084:	17fffdc8 	b	7a4 <_execute+0xa0>
            REG_C++;
    1088:	90000001 	adrp	x1, 0 <REG_C>
    108c:	f9400021 	ldr	x1, [x1]
    1090:	39400020 	ldrb	w0, [x1]
    1094:	11000400 	add	w0, w0, #0x1
    1098:	39000020 	strb	w0, [x1]
            return MCYCLE_1;
    109c:	d2800020 	mov	x0, #0x1                   	// #1
    10a0:	17fffdc1 	b	7a4 <_execute+0xa0>
            WRITE_MEM(REG_SP - 1, REG_PC >> 8);
    10a4:	90000014 	adrp	x20, 0 <REG_PC>
    10a8:	f9400294 	ldr	x20, [x20]
    10ac:	90000013 	adrp	x19, 0 <REG_SP>
    10b0:	f9400273 	ldr	x19, [x19]
    10b4:	39400681 	ldrb	w1, [x20, #1]
    10b8:	79400260 	ldrh	w0, [x19]
    10bc:	51000400 	sub	w0, w0, #0x1
    10c0:	94000000 	bl	520 <_write_item>
            WRITE_MEM(REG_SP - 2, REG_PC & 0xFF);
    10c4:	39400281 	ldrb	w1, [x20]
    10c8:	79400260 	ldrh	w0, [x19]
    10cc:	51000800 	sub	w0, w0, #0x2
    10d0:	94000000 	bl	520 <_write_item>
            REG_SP -= 2;
    10d4:	79400260 	ldrh	w0, [x19]
            REG_PC = 0x0038;
    10d8:	52800701 	mov	w1, #0x38                  	// #56
    10dc:	79000281 	strh	w1, [x20]
            REG_SP -= 2;
    10e0:	51000800 	sub	w0, w0, #0x2
    10e4:	79000260 	strh	w0, [x19]
                return MCYCLE_4;
    10e8:	d2800080 	mov	x0, #0x4                   	// #4
    10ec:	17fffdae 	b	7a4 <_execute+0xa0>
            ADD_SET_FLAGS16(HL(), DE());
    10f0:	90000000 	adrp	x0, 0 <REG_F>
    10f4:	f9400000 	ldr	x0, [x0]
    10f8:	90000004 	adrp	x4, 0 <REG_H>
    10fc:	f9400084 	ldr	x4, [x4]
    1100:	90000005 	adrp	x5, 0 <REG_L>
    1104:	f94000a5 	ldr	x5, [x5]
    1108:	90000007 	adrp	x7, 0 <REG_D>
    110c:	f94000e7 	ldr	x7, [x7]
    1110:	90000003 	adrp	x3, 0 <REG_E>
    1114:	f9400063 	ldr	x3, [x3]
    1118:	17ffffaf 	b	fd4 <_execute+0x8d0>
            FLAG_Z_RESET();
    111c:	90000000 	adrp	x0, 0 <REG_F>
    1120:	f9400000 	ldr	x0, [x0]
            if BIT_IS_SET(REG_A, 0){
    1124:	90000001 	adrp	x1, 0 <REG_A>
    1128:	f9400021 	ldr	x1, [x1]
            FLAG_Z_RESET();
    112c:	39400005 	ldrb	w5, [x0]
            if BIT_IS_SET(REG_A, 0){
    1130:	39400022 	ldrb	w2, [x1]
                FLAG_C_SET();
    1134:	120010a3 	and	w3, w5, #0x1f
    1138:	321c0063 	orr	w3, w3, #0x10
    113c:	12000ca4 	and	w4, w5, #0xf
    1140:	f240005f 	tst	x2, #0x1
            REG_A = (REG_A >> 1) & 0xFF;
    1144:	53017c42 	lsr	w2, w2, #1
                FLAG_C_SET();
    1148:	1a830084 	csel	w4, w4, w3, eq	// eq = none
            REG_A = (REG_A >> 1) & 0xFF;
    114c:	32196043 	orr	w3, w2, #0xffffff80
    1150:	f27c00bf 	tst	x5, #0x10
    1154:	39000004 	strb	w4, [x0]
    1158:	1a821060 	csel	w0, w3, w2, ne	// ne = any
    115c:	39000020 	strb	w0, [x1]
            return MCYCLE_1;
    1160:	d2800020 	mov	x0, #0x1                   	// #1
    1164:	17fffd90 	b	7a4 <_execute+0xa0>
            REG_E = U8(value);
    1168:	90000000 	adrp	x0, 0 <REG_E>
    116c:	f9400000 	ldr	x0, [x0]
    1170:	39000013 	strb	w19, [x0]
            return MCYCLE_2;
    1174:	d2800040 	mov	x0, #0x2                   	// #2
    1178:	17fffd8b 	b	7a4 <_execute+0xa0>
            if (FLAG_Z_IS_SET()){
    117c:	90000000 	adrp	x0, 0 <REG_F>
    1180:	f9400000 	ldr	x0, [x0]
    1184:	39c00000 	ldrsb	w0, [x0]
    1188:	37ffb020 	tbnz	w0, #31, 78c <_execute+0x88>
            return MCYCLE_2;
    118c:	d2800040 	mov	x0, #0x2                   	// #2
    1190:	17fffd85 	b	7a4 <_execute+0xa0>
            INC_HL();
    1194:	90000001 	adrp	x1, 0 <REG_L>
    1198:	f9400021 	ldr	x1, [x1]
    119c:	39400020 	ldrb	w0, [x1]
    11a0:	11000800 	add	w0, w0, #0x2
    11a4:	12001c00 	and	w0, w0, #0xff
    11a8:	39000020 	strb	w0, [x1]
    11ac:	35ffb1a0 	cbnz	w0, 7e0 <_execute+0xdc>
    11b0:	90000001 	adrp	x1, 0 <REG_H>
    11b4:	f9400021 	ldr	x1, [x1]
    11b8:	39400020 	ldrb	w0, [x1]
    11bc:	11000400 	add	w0, w0, #0x1
    11c0:	39000020 	strb	w0, [x1]
            return MCYCLE_1;
    11c4:	d2800020 	mov	x0, #0x1                   	// #1
    11c8:	17fffd77 	b	7a4 <_execute+0xa0>
            REG_A = READ_MEM((address_t)(0xFF00 + value));
    11cc:	51040260 	sub	w0, w19, #0x100
    11d0:	94000000 	bl	210 <_fetch_item>
    11d4:	90000001 	adrp	x1, 0 <REG_A>
    11d8:	f9400021 	ldr	x1, [x1]
    11dc:	39000020 	strb	w0, [x1]
            return MCYCLE_3;
    11e0:	17fffd70 	b	7a0 <_execute+0x9c>
            WRITE_MEM(value, REG_A);
    11e4:	90000001 	adrp	x1, 0 <REG_A>
    11e8:	f9400021 	ldr	x1, [x1]
    11ec:	2a1303e0 	mov	w0, w19
    11f0:	39400021 	ldrb	w1, [x1]
    11f4:	94000000 	bl	520 <_write_item>
                return MCYCLE_4;
    11f8:	d2800080 	mov	x0, #0x4                   	// #4
    11fc:	17fffd6a 	b	7a4 <_execute+0xa0>
            AND_SET_FLAGS(REG_A, value);
    1200:	90000004 	adrp	x4, 0 <REG_F>
    1204:	f9400084 	ldr	x4, [x4]
    1208:	528005e0 	mov	w0, #0x2f                  	// #47
    120c:	90000001 	adrp	x1, 0 <REG_A>
    1210:	f9400021 	ldr	x1, [x1]
    1214:	12800be5 	mov	w5, #0xffffffa0            	// #-96
    1218:	39400083 	ldrb	w3, [x4]
    121c:	39400022 	ldrb	w2, [x1]
    1220:	0a030000 	and	w0, w0, w3
    1224:	321b0003 	orr	w3, w0, #0x20
    1228:	2a050000 	orr	w0, w0, w5
    122c:	7100005f 	cmp	w2, #0x0
            REG_A &= (uint8_t)value;
    1230:	0a130042 	and	w2, w2, w19
            AND_SET_FLAGS(REG_A, value);
    1234:	1a801063 	csel	w3, w3, w0, ne	// ne = any
            return MCYCLE_2;
    1238:	d2800040 	mov	x0, #0x2                   	// #2
            REG_A &= (uint8_t)value;
    123c:	39000022 	strb	w2, [x1]
            AND_SET_FLAGS(REG_A, value);
    1240:	39000083 	strb	w3, [x4]
            return MCYCLE_2;
    1244:	17fffd58 	b	7a4 <_execute+0xa0>
            XOR_SET_FLAGS(REG_A, value);
    1248:	90000002 	adrp	x2, 0 <REG_F>
    124c:	f9400042 	ldr	x2, [x2]
    1250:	90000001 	adrp	x1, 0 <REG_A>
    1254:	f9400021 	ldr	x1, [x1]
    1258:	39400040 	ldrb	w0, [x2]
    125c:	39400023 	ldrb	w3, [x1]
    1260:	12000c00 	and	w0, w0, #0xf
    1264:	32196001 	orr	w1, w0, #0xffffff80
    1268:	7100007f 	cmp	w3, #0x0
    126c:	1a800020 	csel	w0, w1, w0, eq	// eq = none
    1270:	39000040 	strb	w0, [x2]
            return MCYCLE_1;
    1274:	d2800020 	mov	x0, #0x1                   	// #1
    1278:	17fffd4b 	b	7a4 <_execute+0xa0>
            XOR_SET_FLAGS(REG_A, REG_L);
    127c:	90000004 	adrp	x4, 0 <REG_F>
    1280:	f9400084 	ldr	x4, [x4]
    1284:	90000001 	adrp	x1, 0 <REG_A>
    1288:	f9400021 	ldr	x1, [x1]
            REG_A ^= REG_L;
    128c:	90000003 	adrp	x3, 0 <REG_L>
    1290:	f9400063 	ldr	x3, [x3]
            XOR_SET_FLAGS(REG_A, REG_L);
    1294:	39400082 	ldrb	w2, [x4]
    1298:	39400020 	ldrb	w0, [x1]
            REG_A ^= REG_L;
    129c:	39400065 	ldrb	w5, [x3]
            XOR_SET_FLAGS(REG_A, REG_L);
    12a0:	12000c42 	and	w2, w2, #0xf
    12a4:	7100001f 	cmp	w0, #0x0
    12a8:	32196043 	orr	w3, w2, #0xffffff80
            REG_A ^= REG_L;
    12ac:	4a050000 	eor	w0, w0, w5
            XOR_SET_FLAGS(REG_A, REG_L);
    12b0:	1a820062 	csel	w2, w3, w2, eq	// eq = none
    12b4:	39000082 	strb	w2, [x4]
            REG_A ^= REG_L;
    12b8:	39000020 	strb	w0, [x1]
            return MCYCLE_1;
    12bc:	d2800020 	mov	x0, #0x1                   	// #1
    12c0:	17fffd39 	b	7a4 <_execute+0xa0>
            REG_A = REG_L;
    12c4:	90000000 	adrp	x0, 0 <REG_L>
    12c8:	f9400000 	ldr	x0, [x0]
    12cc:	90000001 	adrp	x1, 0 <REG_A>
    12d0:	f9400021 	ldr	x1, [x1]
    12d4:	39400000 	ldrb	w0, [x0]
    12d8:	39000020 	strb	w0, [x1]
            return MCYCLE_1;
    12dc:	d2800020 	mov	x0, #0x1                   	// #1
    12e0:	17fffd31 	b	7a4 <_execute+0xa0>
            REG_A = REG_H;
    12e4:	90000000 	adrp	x0, 0 <REG_H>
    12e8:	f9400000 	ldr	x0, [x0]
    12ec:	90000001 	adrp	x1, 0 <REG_A>
    12f0:	f9400021 	ldr	x1, [x1]
    12f4:	39400000 	ldrb	w0, [x0]
    12f8:	39000020 	strb	w0, [x1]
            return MCYCLE_1;
    12fc:	d2800020 	mov	x0, #0x1                   	// #1
    1300:	17fffd29 	b	7a4 <_execute+0xa0>
            REG_A = REG_E;
    1304:	90000000 	adrp	x0, 0 <REG_E>
    1308:	f9400000 	ldr	x0, [x0]
    130c:	90000001 	adrp	x1, 0 <REG_A>
    1310:	f9400021 	ldr	x1, [x1]
    1314:	39400000 	ldrb	w0, [x0]
    1318:	39000020 	strb	w0, [x1]
            return MCYCLE_1;
    131c:	d2800020 	mov	x0, #0x1                   	// #1
    1320:	17fffd21 	b	7a4 <_execute+0xa0>
            REG_A = REG_D;
    1324:	90000000 	adrp	x0, 0 <REG_D>
    1328:	f9400000 	ldr	x0, [x0]
    132c:	90000001 	adrp	x1, 0 <REG_A>
    1330:	f9400021 	ldr	x1, [x1]
    1334:	39400000 	ldrb	w0, [x0]
    1338:	39000020 	strb	w0, [x1]
            return MCYCLE_1;
    133c:	d2800020 	mov	x0, #0x1                   	// #1
    1340:	17fffd19 	b	7a4 <_execute+0xa0>
            REG_A = REG_C;
    1344:	90000000 	adrp	x0, 0 <REG_C>
    1348:	f9400000 	ldr	x0, [x0]
    134c:	90000001 	adrp	x1, 0 <REG_A>
    1350:	f9400021 	ldr	x1, [x1]
    1354:	39400000 	ldrb	w0, [x0]
    1358:	39000020 	strb	w0, [x1]
            return MCYCLE_1;
    135c:	d2800020 	mov	x0, #0x1                   	// #1
    1360:	17fffd11 	b	7a4 <_execute+0xa0>
    1364:	d2800020 	mov	x0, #0x1                   	// #1
                REG_A >>= 1;
    1368:	39000064 	strb	w4, [x3]
    136c:	17fffd0e 	b	7a4 <_execute+0xa0>
                FLAG_C_SET();
    1370:	321c0000 	orr	w0, w0, #0x10
                REG_A = (REG_A << 1) + 1;
    1374:	11000484 	add	w4, w4, #0x1
                FLAG_C_SET();
    1378:	39000040 	strb	w0, [x2]
            return MCYCLE_1;
    137c:	d2800020 	mov	x0, #0x1                   	// #1
                REG_A = (REG_A << 1) + 1;
    1380:	39000064 	strb	w4, [x3]
    1384:	17fffd08 	b	7a4 <_execute+0xa0>
    1388:	d503201f 	nop
    138c:	d503201f 	nop

0000000000001390 <_executeCB>:

opcycles_t _executeCB(opcode_t opcode){
    1390:	a9be7bfd 	stp	x29, x30, [sp, #-32]!
    1394:	12001c00 	and	w0, w0, #0xff
    1398:	910003fd 	mov	x29, sp
    139c:	f9000bf3 	str	x19, [sp, #16]
    // uint16_t buf16 = 0x00;
    uint8_t buf8 = 0x00;
    switch (opcode) {
    13a0:	7103b81f 	cmp	w0, #0xee
    13a4:	540003a0 	b.eq	1418 <_executeCB+0x88>  // b.none
    13a8:	7103d81f 	cmp	w0, #0xf6
    13ac:	540000e0 	b.eq	13c8 <_executeCB+0x38>  // b.none
    13b0:	d2800033 	mov	x19, #0x1                   	// #1
    13b4:	35000540 	cbnz	w0, 145c <_executeCB+0xcc>

        default:
            log_error("CB Opcode: CB:%02X[%d length] | %s", opcode, OPCODE_LENGTH[(size_t)(opcode)], OPCODE_NAMES[(size_t)(opcode)]);
            exit(1);
    }
    13b8:	aa1303e0 	mov	x0, x19
    13bc:	f9400bf3 	ldr	x19, [sp, #16]
    13c0:	a8c27bfd 	ldp	x29, x30, [sp], #32
    13c4:	d65f03c0 	ret
            buf8 = _fetch_item(HL());
    13c8:	90000001 	adrp	x1, 0 <REG_H>
    13cc:	f9400021 	ldr	x1, [x1]
            return MCYCLE_4;
    13d0:	d2800093 	mov	x19, #0x4                   	// #4
            buf8 = _fetch_item(HL());
    13d4:	90000000 	adrp	x0, 0 <REG_L>
    13d8:	f9400000 	ldr	x0, [x0]
    13dc:	39400025 	ldrb	w5, [x1]
    13e0:	39400000 	ldrb	w0, [x0]
    13e4:	53181ca5 	ubfiz	w5, w5, #8, #8
    13e8:	2a0000a5 	orr	w5, w5, w0
    13ec:	12003ca5 	and	w5, w5, #0xffff
    13f0:	2a0503e0 	mov	w0, w5
    13f4:	94000000 	bl	210 <_fetch_item>
    13f8:	12001c01 	and	w1, w0, #0xff
            _write_item(HL(), buf8);
    13fc:	2a0503e0 	mov	w0, w5
    1400:	321a0021 	orr	w1, w1, #0x40
    1404:	94000000 	bl	520 <_write_item>
    1408:	aa1303e0 	mov	x0, x19
    140c:	f9400bf3 	ldr	x19, [sp, #16]
    1410:	a8c27bfd 	ldp	x29, x30, [sp], #32
    1414:	d65f03c0 	ret
            buf8 = _fetch_item(HL());
    1418:	90000001 	adrp	x1, 0 <REG_H>
    141c:	f9400021 	ldr	x1, [x1]
            return MCYCLE_4;
    1420:	d2800093 	mov	x19, #0x4                   	// #4
            buf8 = _fetch_item(HL());
    1424:	90000000 	adrp	x0, 0 <REG_L>
    1428:	f9400000 	ldr	x0, [x0]
    142c:	39400025 	ldrb	w5, [x1]
    1430:	39400000 	ldrb	w0, [x0]
    1434:	53181ca5 	ubfiz	w5, w5, #8, #8
    1438:	2a0000a5 	orr	w5, w5, w0
    143c:	12003ca5 	and	w5, w5, #0xffff
    1440:	2a0503e0 	mov	w0, w5
    1444:	94000000 	bl	210 <_fetch_item>
    1448:	12001c01 	and	w1, w0, #0xff
            _write_item(HL(), buf8);
    144c:	2a0503e0 	mov	w0, w5
    1450:	321b0021 	orr	w1, w1, #0x20
    1454:	94000000 	bl	520 <_write_item>
            return MCYCLE_4;
    1458:	17ffffec 	b	1408 <_executeCB+0x78>
            log_error("CB Opcode: CB:%02X[%d length] | %s", opcode, OPCODE_LENGTH[(size_t)(opcode)], OPCODE_NAMES[(size_t)(opcode)]);
    145c:	90000002 	adrp	x2, 0 <OPCODE_NAMES>
    1460:	f9400042 	ldr	x2, [x2]
    1464:	2a0003e5 	mov	w5, w0
    1468:	90000001 	adrp	x1, 0 <OPCODE_LENGTH>
    146c:	f9400021 	ldr	x1, [x1]
    1470:	2a0003e4 	mov	w4, w0
    1474:	f8657846 	ldr	x6, [x2, x5, lsl #3]
    1478:	90000003 	adrp	x3, 0 <_load_rom>
    147c:	38656825 	ldrb	w5, [x1, x5]
    1480:	91000063 	add	x3, x3, #0x0
    1484:	90000001 	adrp	x1, 0 <_load_rom>
    1488:	91000021 	add	x1, x1, #0x0
    148c:	52805b42 	mov	w2, #0x2da                 	// #730
    1490:	52800080 	mov	w0, #0x4                   	// #4
    1494:	94000000 	bl	0 <log_log>
            exit(1);
    1498:	2a1303e0 	mov	w0, w19
    149c:	94000000 	bl	0 <exit>

00000000000014a0 <_tick_cpu>:
opcycles_t _tick_cpu(){
    14a0:	a9bd7bfd 	stp	x29, x30, [sp, #-48]!
    14a4:	910003fd 	mov	x29, sp
    14a8:	a90153f3 	stp	x19, x20, [sp, #16]
    opcode_t    opcode = (opcode_t)_fetch_item(REG_PC);
    14ac:	90000014 	adrp	x20, 0 <REG_PC>
    14b0:	f9400294 	ldr	x20, [x20]
opcycles_t _tick_cpu(){
    14b4:	a9025bf5 	stp	x21, x22, [sp, #32]
    opcode_t    opcode = (opcode_t)_fetch_item(REG_PC);
    14b8:	79400280 	ldrh	w0, [x20]
    14bc:	94000000 	bl	210 <_fetch_item>
    14c0:	12001c13 	and	w19, w0, #0xff
    if (get_log_level() > LOG_TRACE){ return; }
    14c4:	94000000 	bl	0 <get_log_level>
    14c8:	7100001f 	cmp	w0, #0x0
    14cc:	5400004c 	b.gt	14d4 <_tick_cpu+0x34>
    14d0:	97fffbc5 	bl	3e4 <dump_registers.part.0>
    if(opcode == 0xCB){
    14d4:	71032e7f 	cmp	w19, #0xcb
    14d8:	54000b00 	b.eq	1638 <_tick_cpu+0x198>  // b.none
    switch (OPCODE_LENGTH[opcode_len_addr]) {
    14dc:	90000006 	adrp	x6, 0 <OPCODE_LENGTH>
    14e0:	f94000c6 	ldr	x6, [x6]
    size_t opcode_len_addr = (size_t)(opcode) + offset;
    14e4:	2a1303e8 	mov	w8, w19
    switch (OPCODE_LENGTH[opcode_len_addr]) {
    14e8:	387348c0 	ldrb	w0, [x6, w19, uxtw]
    14ec:	7100081f 	cmp	w0, #0x2
    14f0:	540008e0 	b.eq	160c <_tick_cpu+0x16c>  // b.none
    14f4:	71000c1f 	cmp	w0, #0x3
    14f8:	540003e0 	b.eq	1574 <_tick_cpu+0xd4>  // b.none
    14fc:	7100041f 	cmp	w0, #0x1
    1500:	54000ea1 	b.ne	16d4 <_tick_cpu+0x234>  // b.any
        INCR_PC();
    1504:	79400285 	ldrh	w5, [x20]
    size_t opcode_len_addr = (size_t)(opcode) + offset;
    1508:	aa0803e9 	mov	x9, x8
    bool        cb = false;
    150c:	52800007 	mov	w7, #0x0                   	// #0
    opcycles_t  opcycles = 0;
    1510:	d2800016 	mov	x22, #0x0                   	// #0
            INCR_PC();
    1514:	110004a2 	add	w2, w5, #0x1
    uint16_t    value = 0x0000;
    1518:	52800015 	mov	w21, #0x0                   	// #0
            INCR_PC();
    151c:	12003c42 	and	w2, w2, #0xffff
            break;
    1520:	52800025 	mov	w5, #0x1                   	// #1
            INCR_PC();
    1524:	79000282 	strh	w2, [x20]
        log_trace("CB Opcode: CB:%02X[%d length] | %s", opcode, OPCODE_LENGTH[(size_t)(opcode+offset)], OPCODE_NAMES[(size_t)(opcode)+offset]);
    1528:	90000000 	adrp	x0, 0 <OPCODE_NAMES>
    152c:	f9400000 	ldr	x0, [x0]
    if (cb){
    1530:	34000487 	cbz	w7, 15c0 <_tick_cpu+0x120>
        log_trace("CB Opcode: CB:%02X[%d length] | %s", opcode, OPCODE_LENGTH[(size_t)(opcode+offset)], OPCODE_NAMES[(size_t)(opcode)+offset]);
    1534:	f8697806 	ldr	x6, [x0, x9, lsl #3]
    1538:	2a1303e4 	mov	w4, w19
    153c:	90000003 	adrp	x3, 0 <_load_rom>
    1540:	90000001 	adrp	x1, 0 <_load_rom>
    1544:	91000063 	add	x3, x3, #0x0
    1548:	91000021 	add	x1, x1, #0x0
    154c:	52801542 	mov	w2, #0xaa                  	// #170
    1550:	52800000 	mov	w0, #0x0                   	// #0
    1554:	94000000 	bl	0 <log_log>
        opcycles += _executeCB(opcode);
    1558:	2a1303e0 	mov	w0, w19
    155c:	94000000 	bl	1390 <_executeCB>
}
    1560:	a94153f3 	ldp	x19, x20, [sp, #16]
        opcycles += _executeCB(opcode);
    1564:	8b160000 	add	x0, x0, x22
}
    1568:	a9425bf5 	ldp	x21, x22, [sp, #32]
    156c:	a8c37bfd 	ldp	x29, x30, [sp], #48
    1570:	d65f03c0 	ret
        INCR_PC();
    1574:	79400285 	ldrh	w5, [x20]
    size_t opcode_len_addr = (size_t)(opcode) + offset;
    1578:	aa0803e9 	mov	x9, x8
    bool        cb = false;
    157c:	52800007 	mov	w7, #0x0                   	// #0
    opcycles_t  opcycles = 0;
    1580:	d2800016 	mov	x22, #0x0                   	// #0
            value = (_fetch_item(REG_PC+2) << 8) | _fetch_item(REG_PC+1);
    1584:	110008a0 	add	w0, w5, #0x2
    1588:	94000000 	bl	210 <_fetch_item>
    158c:	110004a1 	add	w1, w5, #0x1
    1590:	12001c15 	and	w21, w0, #0xff
            INCR_PC();
    1594:	11000ca5 	add	w5, w5, #0x3
            value = (_fetch_item(REG_PC+2) << 8) | _fetch_item(REG_PC+1);
    1598:	2a0103e0 	mov	w0, w1
    159c:	94000000 	bl	210 <_fetch_item>
    15a0:	12001c00 	and	w0, w0, #0xff
            INCR_PC();
    15a4:	12003ca2 	and	w2, w5, #0xffff
            INCR_PC();
    15a8:	79000282 	strh	w2, [x20]
            value = (_fetch_item(REG_PC+2) << 8) | _fetch_item(REG_PC+1);
    15ac:	2a152015 	orr	w21, w0, w21, lsl #8
            break;
    15b0:	52800065 	mov	w5, #0x3                   	// #3
        log_trace("CB Opcode: CB:%02X[%d length] | %s", opcode, OPCODE_LENGTH[(size_t)(opcode+offset)], OPCODE_NAMES[(size_t)(opcode)+offset]);
    15b4:	90000000 	adrp	x0, 0 <OPCODE_NAMES>
    15b8:	f9400000 	ldr	x0, [x0]
    if (cb){
    15bc:	35fffbc7 	cbnz	w7, 1534 <_tick_cpu+0x94>
        log_trace("Opcode: %02X[%d length], Value: %04X | %s", opcode, OPCODE_LENGTH[(size_t)opcode], value, OPCODE_NAMES[(size_t)opcode]);
    15c0:	f8687807 	ldr	x7, [x0, x8, lsl #3]
    15c4:	2a1303e4 	mov	w4, w19
    15c8:	386868c5 	ldrb	w5, [x6, x8]
    15cc:	90000003 	adrp	x3, 0 <_load_rom>
    15d0:	2a1503e6 	mov	w6, w21
    15d4:	91000063 	add	x3, x3, #0x0
    15d8:	528015a2 	mov	w2, #0xad                  	// #173
    15dc:	52800000 	mov	w0, #0x0                   	// #0
    15e0:	90000001 	adrp	x1, 0 <_load_rom>
    15e4:	91000021 	add	x1, x1, #0x0
    15e8:	94000000 	bl	0 <log_log>
        opcycles += _execute(opcode, value);
    15ec:	2a1503e1 	mov	w1, w21
    15f0:	2a1303e0 	mov	w0, w19
    15f4:	94000000 	bl	704 <_execute>
    15f8:	8b160000 	add	x0, x0, x22
}
    15fc:	a94153f3 	ldp	x19, x20, [sp, #16]
    1600:	a9425bf5 	ldp	x21, x22, [sp, #32]
    1604:	a8c37bfd 	ldp	x29, x30, [sp], #48
    1608:	d65f03c0 	ret
        INCR_PC();
    160c:	79400285 	ldrh	w5, [x20]
    size_t opcode_len_addr = (size_t)(opcode) + offset;
    1610:	aa0803e9 	mov	x9, x8
    bool        cb = false;
    1614:	52800007 	mov	w7, #0x0                   	// #0
    opcycles_t  opcycles = 0;
    1618:	d2800016 	mov	x22, #0x0                   	// #0
            value = _fetch_item(REG_PC+1);
    161c:	110004a0 	add	w0, w5, #0x1
            INCR_PC();
    1620:	110008a5 	add	w5, w5, #0x2
            value = _fetch_item(REG_PC+1);
    1624:	94000000 	bl	210 <_fetch_item>
    1628:	12001c15 	and	w21, w0, #0xff
            INCR_PC();
    162c:	12003ca2 	and	w2, w5, #0xffff
            break;
    1630:	52800045 	mov	w5, #0x2                   	// #2
    1634:	17ffffbc 	b	1524 <_tick_cpu+0x84>
        INCR_PC();
    1638:	79400285 	ldrh	w5, [x20]
    switch (OPCODE_LENGTH[opcode_len_addr]) {
    163c:	90000006 	adrp	x6, 0 <OPCODE_LENGTH>
    1640:	f94000c6 	ldr	x6, [x6]
        INCR_PC();
    1644:	110004a5 	add	w5, w5, #0x1
    1648:	12003ca5 	and	w5, w5, #0xffff
    164c:	79000285 	strh	w5, [x20]
        opcode = _fetch_item(REG_PC);
    1650:	2a0503e0 	mov	w0, w5
    1654:	94000000 	bl	210 <_fetch_item>
    size_t opcode_len_addr = (size_t)(opcode) + offset;
    1658:	92401c08 	and	x8, x0, #0xff
        opcode = _fetch_item(REG_PC);
    165c:	12001c13 	and	w19, w0, #0xff
    size_t opcode_len_addr = (size_t)(opcode) + offset;
    1660:	91040109 	add	x9, x8, #0x100
    switch (OPCODE_LENGTH[opcode_len_addr]) {
    1664:	386968c7 	ldrb	w7, [x6, x9]
    1668:	710008ff 	cmp	w7, #0x2
    166c:	540000e0 	b.eq	1688 <_tick_cpu+0x1e8>  // b.none
    1670:	71000cff 	cmp	w7, #0x3
    1674:	54000100 	b.eq	1694 <_tick_cpu+0x1f4>  // b.none
    1678:	710004ff 	cmp	w7, #0x1
    167c:	54000121 	b.ne	16a0 <_tick_cpu+0x200>  // b.any
    1680:	d2800036 	mov	x22, #0x1                   	// #1
    1684:	17ffffa4 	b	1514 <_tick_cpu+0x74>
    1688:	52800027 	mov	w7, #0x1                   	// #1
    168c:	d2800036 	mov	x22, #0x1                   	// #1
    1690:	17ffffe3 	b	161c <_tick_cpu+0x17c>
    1694:	52800027 	mov	w7, #0x1                   	// #1
    1698:	d2800036 	mov	x22, #0x1                   	// #1
    169c:	17ffffba 	b	1584 <_tick_cpu+0xe4>
            log_error("Invalid opcode length: %d for opcode: %02X. CB Mode: %s", OPCODE_LENGTH[(size_t)(opcode+offset)], opcode, cb ? "true" : "false");
    16a0:	90000006 	adrp	x6, 0 <_load_rom>
    16a4:	2a0703e4 	mov	w4, w7
    16a8:	910000c6 	add	x6, x6, #0x0
    16ac:	2a1303e5 	mov	w5, w19
    16b0:	90000003 	adrp	x3, 0 <_load_rom>
    16b4:	90000001 	adrp	x1, 0 <_load_rom>
    16b8:	91000063 	add	x3, x3, #0x0
    16bc:	91000021 	add	x1, x1, #0x0
    16c0:	52801482 	mov	w2, #0xa4                  	// #164
    16c4:	52800080 	mov	w0, #0x4                   	// #4
    16c8:	94000000 	bl	0 <log_log>
            exit(1);
    16cc:	52800020 	mov	w0, #0x1                   	// #1
    16d0:	94000000 	bl	0 <exit>
            log_error("Invalid opcode length: %d for opcode: %02X. CB Mode: %s", OPCODE_LENGTH[(size_t)(opcode+offset)], opcode, cb ? "true" : "false");
    16d4:	90000006 	adrp	x6, 0 <_load_rom>
    16d8:	2a0003e4 	mov	w4, w0
    16dc:	910000c6 	add	x6, x6, #0x0
    16e0:	17fffff3 	b	16ac <_tick_cpu+0x20c>

00000000000016e4 <_tick>:
int _tick(){
    16e4:	a9be7bfd 	stp	x29, x30, [sp, #-32]!
    cycles += _tick_cpu(cycles);
    16e8:	d2800000 	mov	x0, #0x0                   	// #0
int _tick(){
    16ec:	910003fd 	mov	x29, sp
    16f0:	f9000bf3 	str	x19, [sp, #16]
    cycles += _tick_cpu(cycles);
    16f4:	94000000 	bl	14a0 <_tick_cpu>
    if (CPU_STUCK){
    16f8:	90000001 	adrp	x1, 0 <_load_rom>
    cycles += _tick_cpu(cycles);
    16fc:	aa0003f3 	mov	x19, x0
    if (CPU_STUCK){
    1700:	39400020 	ldrb	w0, [x1]
    1704:	37000120 	tbnz	w0, #0, 1728 <_tick+0x44>
    if (DEBUG_STEP_MODE) {
    1708:	90000000 	adrp	x0, 0 <DEBUG_STEP_MODE>
    170c:	f9400000 	ldr	x0, [x0]
    1710:	39400000 	ldrb	w0, [x0]
    1714:	37000340 	tbnz	w0, #0, 177c <_tick+0x98>
}
    1718:	2a1303e0 	mov	w0, w19
    171c:	f9400bf3 	ldr	x19, [sp, #16]
    1720:	a8c27bfd 	ldp	x29, x30, [sp], #32
    1724:	d65f03c0 	ret
        log_warn("CPU Stuck at PC: %04X, SP: %04X", REG_PC, REG_SP);
    1728:	90000001 	adrp	x1, 0 <REG_SP>
    172c:	f9400021 	ldr	x1, [x1]
    1730:	52800de2 	mov	w2, #0x6f                  	// #111
    1734:	90000000 	adrp	x0, 0 <REG_PC>
    1738:	f9400000 	ldr	x0, [x0]
    173c:	90000003 	adrp	x3, 0 <_load_rom>
    1740:	79400025 	ldrh	w5, [x1]
    1744:	91000063 	add	x3, x3, #0x0
    1748:	90000001 	adrp	x1, 0 <_load_rom>
    174c:	91000021 	add	x1, x1, #0x0
    1750:	79400004 	ldrh	w4, [x0]
    1754:	52800060 	mov	w0, #0x3                   	// #3
    1758:	94000000 	bl	0 <log_log>

/* Read a character from stdin.  */
__STDIO_INLINE int
getchar (void)
{
  return getc (stdin);
    175c:	90000000 	adrp	x0, 0 <stdin>
    1760:	f9400000 	ldr	x0, [x0]
    1764:	f9400000 	ldr	x0, [x0]
    1768:	94000000 	bl	0 <getc>
    if (DEBUG_STEP_MODE) {
    176c:	90000000 	adrp	x0, 0 <DEBUG_STEP_MODE>
    1770:	f9400000 	ldr	x0, [x0]
    1774:	39400000 	ldrb	w0, [x0]
    1778:	3607fd00 	tbz	w0, #0, 1718 <_tick+0x34>
    177c:	90000000 	adrp	x0, 0 <stdin>
    1780:	f9400000 	ldr	x0, [x0]
    1784:	f9400000 	ldr	x0, [x0]
    1788:	94000000 	bl	0 <getc>
}
    178c:	2a1303e0 	mov	w0, w19
    1790:	f9400bf3 	ldr	x19, [sp, #16]
    1794:	a8c27bfd 	ldp	x29, x30, [sp], #32
    1798:	d65f03c0 	ret
    179c:	d503201f 	nop

00000000000017a0 <gameboy_loop>:
int gameboy_loop(){
    17a0:	a9b97bfd 	stp	x29, x30, [sp, #-112]!
    17a4:	910003fd 	mov	x29, sp
    17a8:	a9046bf9 	stp	x25, x26, [sp, #64]
        log_warn("CPU Stuck at PC: %04X, SP: %04X", REG_PC, REG_SP);
    17ac:	9000001a 	adrp	x26, 0 <_load_rom>
    17b0:	91000340 	add	x0, x26, #0x0
    17b4:	f90037e0 	str	x0, [sp, #104]
    17b8:	90000000 	adrp	x0, 0 <REG_PC>
    17bc:	f9400000 	ldr	x0, [x0]
int gameboy_loop(){
    17c0:	a90153f3 	stp	x19, x20, [sp, #16]
    size_t tick_count = 0;
    17c4:	d2800013 	mov	x19, #0x0                   	// #0
    17c8:	90000019 	adrp	x25, 0 <stdin>
    17cc:	f9400339 	ldr	x25, [x25]
    size_t cycles = 0;
    17d0:	d2800014 	mov	x20, #0x0                   	// #0
int gameboy_loop(){
    17d4:	a9025bf5 	stp	x21, x22, [sp, #32]
    17d8:	90000015 	adrp	x21, 0 <_load_rom>
    17dc:	90000016 	adrp	x22, 0 <_load_rom>
    17e0:	a90363f7 	stp	x23, x24, [sp, #48]
    17e4:	90000017 	adrp	x23, 0 <_load_rom>
    17e8:	90000018 	adrp	x24, 0 <DEBUG_STEP_MODE>
    17ec:	f9400318 	ldr	x24, [x24]
    17f0:	a90573fb 	stp	x27, x28, [sp, #80]
        log_warn("CPU Stuck at PC: %04X, SP: %04X", REG_PC, REG_SP);
    17f4:	9000001c 	adrp	x28, 0 <REG_SP>
    17f8:	f940039c 	ldr	x28, [x28]
    17fc:	f90033e0 	str	x0, [sp, #96]
    1800:	1400000f 	b	183c <gameboy_loop+0x9c>
    if (DEBUG_STEP_MODE) {
    1804:	39400300 	ldrb	w0, [x24]
    1808:	370004a0 	tbnz	w0, #0, 189c <gameboy_loop+0xfc>
        n = _tick();
    180c:	93407f40 	sxtw	x0, w26
        log_trace("Tick End: %d | Cycle Total: %d", ++tick_count, cycles);
    1810:	91000673 	add	x19, x19, #0x1
        cycles += n;
    1814:	8b000294 	add	x20, x20, x0
        if (!n) {
    1818:	340004fa 	cbz	w26, 18b4 <gameboy_loop+0x114>
        log_trace("Tick End: %d | Cycle Total: %d", ++tick_count, cycles);
    181c:	90000003 	adrp	x3, 0 <_load_rom>
    1820:	aa1403e5 	mov	x5, x20
    1824:	aa1303e4 	mov	x4, x19
    1828:	91000063 	add	x3, x3, #0x0
    182c:	910002a1 	add	x1, x21, #0x0
    1830:	52800c02 	mov	w2, #0x60                  	// #96
    1834:	52800000 	mov	w0, #0x0                   	// #0
    1838:	94000000 	bl	0 <log_log>
        log_trace("Tick Begin: %d", tick_count);
    183c:	910002bb 	add	x27, x21, #0x0
    1840:	aa1303e4 	mov	x4, x19
    1844:	aa1b03e1 	mov	x1, x27
    1848:	910002e3 	add	x3, x23, #0x0
    184c:	52800b22 	mov	w2, #0x59                  	// #89
    1850:	52800000 	mov	w0, #0x0                   	// #0
    1854:	94000000 	bl	0 <log_log>
    cycles += _tick_cpu(cycles);
    1858:	d2800000 	mov	x0, #0x0                   	// #0
    185c:	94000000 	bl	14a0 <_tick_cpu>
    if (CPU_STUCK){
    1860:	910002c2 	add	x2, x22, #0x0
    cycles += _tick_cpu(cycles);
    1864:	aa0003fa 	mov	x26, x0
    if (CPU_STUCK){
    1868:	39400440 	ldrb	w0, [x2, #1]
    186c:	3607fcc0 	tbz	w0, #0, 1804 <gameboy_loop+0x64>
        log_warn("CPU Stuck at PC: %04X, SP: %04X", REG_PC, REG_SP);
    1870:	a9460fe0 	ldp	x0, x3, [sp, #96]
    1874:	aa1b03e1 	mov	x1, x27
    1878:	79400385 	ldrh	w5, [x28]
    187c:	52800de2 	mov	w2, #0x6f                  	// #111
    1880:	79400004 	ldrh	w4, [x0]
    1884:	52800060 	mov	w0, #0x3                   	// #3
    1888:	94000000 	bl	0 <log_log>
    188c:	f9400320 	ldr	x0, [x25]
    1890:	94000000 	bl	0 <getc>
    if (DEBUG_STEP_MODE) {
    1894:	39400300 	ldrb	w0, [x24]
    1898:	3607fba0 	tbz	w0, #0, 180c <gameboy_loop+0x6c>
    189c:	f9400320 	ldr	x0, [x25]
        log_trace("Tick End: %d | Cycle Total: %d", ++tick_count, cycles);
    18a0:	91000673 	add	x19, x19, #0x1
    18a4:	94000000 	bl	0 <getc>
        n = _tick();
    18a8:	93407f40 	sxtw	x0, w26
        cycles += n;
    18ac:	8b000294 	add	x20, x20, x0
        if (!n) {
    18b0:	35fffb7a 	cbnz	w26, 181c <gameboy_loop+0x7c>
            log_error("Error occured");
    18b4:	910002a1 	add	x1, x21, #0x0
    18b8:	52800b82 	mov	w2, #0x5c                  	// #92
    18bc:	52800080 	mov	w0, #0x4                   	// #4
    18c0:	90000003 	adrp	x3, 0 <_load_rom>
    18c4:	91000063 	add	x3, x3, #0x0
    18c8:	94000000 	bl	0 <log_log>
}
    18cc:	a94153f3 	ldp	x19, x20, [sp, #16]
    18d0:	52800000 	mov	w0, #0x0                   	// #0
    18d4:	a9425bf5 	ldp	x21, x22, [sp, #32]
    18d8:	a94363f7 	ldp	x23, x24, [sp, #48]
    18dc:	a9446bf9 	ldp	x25, x26, [sp, #64]
    18e0:	a94573fb 	ldp	x27, x28, [sp, #80]
    18e4:	a8c77bfd 	ldp	x29, x30, [sp], #112
    18e8:	d65f03c0 	ret
