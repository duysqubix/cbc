GAS LISTING /tmp/ccAJyxY4.s 			page 1


   1              		.file	"gameboy.c"
   2              		.text
   3              	.Ltext0:
   4              		.file 0 "/home/duys/Documents/cbc" "gameboy.c"
   5              		.section	.rodata
   6              	.LC0:
   7 0000 726200   		.string	"rb"
   8              	.LC1:
   9 0003 4572726F 		.string	"Error: File not found"
   9      723A2046 
   9      696C6520 
   9      6E6F7420 
   9      666F756E 
  10              	.LC2:
  11 0019 4572726F 		.string	"Error: Buffer is NULL"
  11      723A2042 
  11      75666665 
  11      72206973 
  11      204E554C 
  12              	.LC3:
  13 002f 4572726F 		.string	"Error: Buffer is too small"
  13      723A2042 
  13      75666665 
  13      72206973 
  13      20746F6F 
  14              	.LC4:
  15 004a 46696C65 		.string	"File size: %ld\n"
  15      2073697A 
  15      653A2025 
  15      6C640A00 
  16              	.LC5:
  17 005a 42756666 		.string	"Buffer size: %ld\n"
  17      65722073 
  17      697A653A 
  17      20256C64 
  17      0A00
  18              	.LC6:
  19 006c 4572726F 		.string	"Error: Reading file failed"
  19      723A2052 
  19      65616469 
  19      6E672066 
  19      696C6520 
  20              		.text
  21              		.globl	read_file_into_buffer
  22              		.type	read_file_into_buffer, @function
  23              	read_file_into_buffer:
  24              	.LFB6:
  25              		.file 1 "gameboy.c"
   1:gameboy.c     **** #include <stdio.h>
   2:gameboy.c     **** #include <stdint.h>
   3:gameboy.c     **** #include <stdlib.h> 
   4:gameboy.c     **** #include <time.h>
   5:gameboy.c     **** #include <string.h>
   6:gameboy.c     **** 
   7:gameboy.c     **** #include "defs.h"
   8:gameboy.c     **** #include "gameboy.h"
   9:gameboy.c     **** #include "opcodes.h"
GAS LISTING /tmp/ccAJyxY4.s 			page 2


  10:gameboy.c     **** 
  11:gameboy.c     **** /*-------------------- UTILS --------------------------- */
  12:gameboy.c     **** 
  13:gameboy.c     **** long read_file_into_buffer(const char *filename, uint8_t *buffer, long buffer_size) {
  26              		.loc 1 13 85
  27              		.cfi_startproc
  28 0000 F30F1EFA 		endbr64
  29 0004 55       		pushq	%rbp
  30              		.cfi_def_cfa_offset 16
  31              		.cfi_offset 6, -16
  32 0005 4889E5   		movq	%rsp, %rbp
  33              		.cfi_def_cfa_register 6
  34 0008 4883EC40 		subq	$64, %rsp
  35 000c 48897DD8 		movq	%rdi, -40(%rbp)
  36 0010 488975D0 		movq	%rsi, -48(%rbp)
  37 0014 488955C8 		movq	%rdx, -56(%rbp)
  14:gameboy.c     ****     long file_size;
  15:gameboy.c     ****     FILE *file;
  16:gameboy.c     ****     file = fopen(filename, "rb");
  38              		.loc 1 16 12
  39 0018 488B45D8 		movq	-40(%rbp), %rax
  40 001c 488D1500 		leaq	.LC0(%rip), %rdx
  40      000000
  41 0023 4889D6   		movq	%rdx, %rsi
  42 0026 4889C7   		movq	%rax, %rdi
  43 0029 E8000000 		call	fopen@PLT
  43      00
  44 002e 488945E8 		movq	%rax, -24(%rbp)
  17:gameboy.c     **** 
  18:gameboy.c     ****     if (file == NULL) {
  45              		.loc 1 18 8
  46 0032 48837DE8 		cmpq	$0, -24(%rbp)
  46      00
  47 0037 751B     		jne	.L2
  19:gameboy.c     ****         printf("Error: File not found\n");
  48              		.loc 1 19 9
  49 0039 488D0500 		leaq	.LC1(%rip), %rax
  49      000000
  50 0040 4889C7   		movq	%rax, %rdi
  51 0043 E8000000 		call	puts@PLT
  51      00
  20:gameboy.c     ****         return -1;
  52              		.loc 1 20 16
  53 0048 48C7C0FF 		movq	$-1, %rax
  53      FFFFFF
  54 004f E92B0100 		jmp	.L3
  54      00
  55              	.L2:
  21:gameboy.c     ****     }
  22:gameboy.c     **** 
  23:gameboy.c     ****     // get the file size
  24:gameboy.c     ****     fseek(file, 0, SEEK_END);
  56              		.loc 1 24 5
  57 0054 488B45E8 		movq	-24(%rbp), %rax
  58 0058 BA020000 		movl	$2, %edx
  58      00
  59 005d BE000000 		movl	$0, %esi
GAS LISTING /tmp/ccAJyxY4.s 			page 3


  59      00
  60 0062 4889C7   		movq	%rax, %rdi
  61 0065 E8000000 		call	fseek@PLT
  61      00
  25:gameboy.c     ****     file_size = ftell(file);
  62              		.loc 1 25 17
  63 006a 488B45E8 		movq	-24(%rbp), %rax
  64 006e 4889C7   		movq	%rax, %rdi
  65 0071 E8000000 		call	ftell@PLT
  65      00
  66 0076 488945F0 		movq	%rax, -16(%rbp)
  26:gameboy.c     ****     rewind(file);
  67              		.loc 1 26 5
  68 007a 488B45E8 		movq	-24(%rbp), %rax
  69 007e 4889C7   		movq	%rax, %rdi
  70 0081 E8000000 		call	rewind@PLT
  70      00
  27:gameboy.c     **** 
  28:gameboy.c     ****     // check if buffer is large enough
  29:gameboy.c     ****     if (buffer == NULL) {
  71              		.loc 1 29 8
  72 0086 48837DD0 		cmpq	$0, -48(%rbp)
  72      00
  73 008b 7527     		jne	.L4
  30:gameboy.c     ****         printf("Error: Buffer is NULL\n");
  74              		.loc 1 30 9
  75 008d 488D0500 		leaq	.LC2(%rip), %rax
  75      000000
  76 0094 4889C7   		movq	%rax, %rdi
  77 0097 E8000000 		call	puts@PLT
  77      00
  31:gameboy.c     ****         fclose(file);
  78              		.loc 1 31 9
  79 009c 488B45E8 		movq	-24(%rbp), %rax
  80 00a0 4889C7   		movq	%rax, %rdi
  81 00a3 E8000000 		call	fclose@PLT
  81      00
  32:gameboy.c     ****         return -1;
  82              		.loc 1 32 16
  83 00a8 48C7C0FF 		movq	$-1, %rax
  83      FFFFFF
  84 00af E9CB0000 		jmp	.L3
  84      00
  85              	.L4:
  33:gameboy.c     ****     }
  34:gameboy.c     **** 
  35:gameboy.c     **** 
  36:gameboy.c     ****     if (buffer_size < file_size) {
  86              		.loc 1 36 8
  87 00b4 488B45C8 		movq	-56(%rbp), %rax
  88 00b8 483B45F0 		cmpq	-16(%rbp), %rax
  89 00bc 7D5A     		jge	.L5
  37:gameboy.c     ****         printf("Error: Buffer is too small\n");
  90              		.loc 1 37 9
  91 00be 488D0500 		leaq	.LC3(%rip), %rax
  91      000000
  92 00c5 4889C7   		movq	%rax, %rdi
GAS LISTING /tmp/ccAJyxY4.s 			page 4


  93 00c8 E8000000 		call	puts@PLT
  93      00
  38:gameboy.c     ****         printf("File size: %ld\n", file_size);
  94              		.loc 1 38 9
  95 00cd 488B45F0 		movq	-16(%rbp), %rax
  96 00d1 4889C6   		movq	%rax, %rsi
  97 00d4 488D0500 		leaq	.LC4(%rip), %rax
  97      000000
  98 00db 4889C7   		movq	%rax, %rdi
  99 00de B8000000 		movl	$0, %eax
  99      00
 100 00e3 E8000000 		call	printf@PLT
 100      00
  39:gameboy.c     ****         printf("Buffer size: %ld\n", buffer_size);
 101              		.loc 1 39 9
 102 00e8 488B45C8 		movq	-56(%rbp), %rax
 103 00ec 4889C6   		movq	%rax, %rsi
 104 00ef 488D0500 		leaq	.LC5(%rip), %rax
 104      000000
 105 00f6 4889C7   		movq	%rax, %rdi
 106 00f9 B8000000 		movl	$0, %eax
 106      00
 107 00fe E8000000 		call	printf@PLT
 107      00
  40:gameboy.c     ****         fclose(file);
 108              		.loc 1 40 9
 109 0103 488B45E8 		movq	-24(%rbp), %rax
 110 0107 4889C7   		movq	%rax, %rdi
 111 010a E8000000 		call	fclose@PLT
 111      00
  41:gameboy.c     ****         return -1;
 112              		.loc 1 41 16
 113 010f 48C7C0FF 		movq	$-1, %rax
 113      FFFFFF
 114 0116 EB67     		jmp	.L3
 115              	.L5:
  42:gameboy.c     ****     }
  43:gameboy.c     **** 
  44:gameboy.c     ****     // read the file into the buffer
  45:gameboy.c     ****     size_t result = fread(buffer, 1, file_size, file);
 116              		.loc 1 45 21
 117 0118 488B55F0 		movq	-16(%rbp), %rdx
 118 011c 488B4DE8 		movq	-24(%rbp), %rcx
 119 0120 488B45D0 		movq	-48(%rbp), %rax
 120 0124 BE010000 		movl	$1, %esi
 120      00
 121 0129 4889C7   		movq	%rax, %rdi
 122 012c E8000000 		call	fread@PLT
 122      00
 123 0131 488945F8 		movq	%rax, -8(%rbp)
  46:gameboy.c     ****     if (result != file_size){
 124              		.loc 1 46 16
 125 0135 488B45F0 		movq	-16(%rbp), %rax
 126              		.loc 1 46 8
 127 0139 483945F8 		cmpq	%rax, -8(%rbp)
 128 013d 7430     		je	.L6
  47:gameboy.c     ****         printf("Error: Reading file failed\n");
GAS LISTING /tmp/ccAJyxY4.s 			page 5


 129              		.loc 1 47 9
 130 013f 488D0500 		leaq	.LC6(%rip), %rax
 130      000000
 131 0146 4889C7   		movq	%rax, %rdi
 132 0149 E8000000 		call	puts@PLT
 132      00
  48:gameboy.c     ****         fclose(file);
 133              		.loc 1 48 9
 134 014e 488B45E8 		movq	-24(%rbp), %rax
 135 0152 4889C7   		movq	%rax, %rdi
 136 0155 E8000000 		call	fclose@PLT
 136      00
  49:gameboy.c     ****         free(buffer);
 137              		.loc 1 49 9
 138 015a 488B45D0 		movq	-48(%rbp), %rax
 139 015e 4889C7   		movq	%rax, %rdi
 140 0161 E8000000 		call	free@PLT
 140      00
  50:gameboy.c     ****         return -1;
 141              		.loc 1 50 16
 142 0166 48C7C0FF 		movq	$-1, %rax
 142      FFFFFF
 143 016d EB10     		jmp	.L3
 144              	.L6:
  51:gameboy.c     ****     }  
  52:gameboy.c     ****     fclose(file);
 145              		.loc 1 52 5
 146 016f 488B45E8 		movq	-24(%rbp), %rax
 147 0173 4889C7   		movq	%rax, %rdi
 148 0176 E8000000 		call	fclose@PLT
 148      00
  53:gameboy.c     ****     return file_size;
 149              		.loc 1 53 12
 150 017b 488B45F0 		movq	-16(%rbp), %rax
 151              	.L3:
  54:gameboy.c     **** }
 152              		.loc 1 54 1
 153 017f C9       		leave
 154              		.cfi_def_cfa 7, 8
 155 0180 C3       		ret
 156              		.cfi_endproc
 157              	.LFE6:
 158              		.size	read_file_into_buffer, .-read_file_into_buffer
 159              		.globl	create_buffer8
 160              		.type	create_buffer8, @function
 161              	create_buffer8:
 162              	.LFB7:
  55:gameboy.c     **** 
  56:gameboy.c     **** 
  57:gameboy.c     **** uint8_t* create_buffer8(uint32_t size, bool randomize) {
 163              		.loc 1 57 56
 164              		.cfi_startproc
 165 0181 F30F1EFA 		endbr64
 166 0185 55       		pushq	%rbp
 167              		.cfi_def_cfa_offset 16
 168              		.cfi_offset 6, -16
 169 0186 4889E5   		movq	%rsp, %rbp
GAS LISTING /tmp/ccAJyxY4.s 			page 6


 170              		.cfi_def_cfa_register 6
 171 0189 4883EC20 		subq	$32, %rsp
 172 018d 897DEC   		movl	%edi, -20(%rbp)
 173 0190 89F0     		movl	%esi, %eax
 174 0192 8845E8   		movb	%al, -24(%rbp)
  58:gameboy.c     ****     uint8_t *ptr = (uint8_t *)calloc(size, sizeof(uint8_t));
 175              		.loc 1 58 31
 176 0195 8B45EC   		movl	-20(%rbp), %eax
 177 0198 BE010000 		movl	$1, %esi
 177      00
 178 019d 4889C7   		movq	%rax, %rdi
 179 01a0 E8000000 		call	calloc@PLT
 179      00
 180 01a5 488945F8 		movq	%rax, -8(%rbp)
  59:gameboy.c     **** 
  60:gameboy.c     ****     if (randomize){
 181              		.loc 1 60 8
 182 01a9 807DE800 		cmpb	$0, -24(%rbp)
 183 01ad 7438     		je	.L8
 184              	.LBB2:
  61:gameboy.c     ****         // randomize the buffer
  62:gameboy.c     ****         for (int i = 0; i < size; i++) {
 185              		.loc 1 62 18
 186 01af C745F400 		movl	$0, -12(%rbp)
 186      000000
 187              		.loc 1 62 9
 188 01b6 EB27     		jmp	.L9
 189              	.L10:
  63:gameboy.c     ****             ptr[i] = (uint8_t)(rand() % 256);
 190              		.loc 1 63 32 discriminator 3
 191 01b8 E8000000 		call	rand@PLT
 191      00
 192              		.loc 1 63 39 discriminator 3
 193 01bd 99       		cltd
 194 01be C1EA18   		shrl	$24, %edx
 195 01c1 01D0     		addl	%edx, %eax
 196 01c3 0FB6C0   		movzbl	%al, %eax
 197 01c6 29D0     		subl	%edx, %eax
 198 01c8 89C1     		movl	%eax, %ecx
 199              		.loc 1 63 16 discriminator 3
 200 01ca 8B45F4   		movl	-12(%rbp), %eax
 201 01cd 4863D0   		movslq	%eax, %rdx
 202 01d0 488B45F8 		movq	-8(%rbp), %rax
 203 01d4 4801D0   		addq	%rdx, %rax
 204              		.loc 1 63 22 discriminator 3
 205 01d7 89CA     		movl	%ecx, %edx
 206              		.loc 1 63 20 discriminator 3
 207 01d9 8810     		movb	%dl, (%rax)
  62:gameboy.c     ****             ptr[i] = (uint8_t)(rand() % 256);
 208              		.loc 1 62 36 discriminator 3
 209 01db 8345F401 		addl	$1, -12(%rbp)
 210              	.L9:
  62:gameboy.c     ****             ptr[i] = (uint8_t)(rand() % 256);
 211              		.loc 1 62 27 discriminator 1
 212 01df 8B45F4   		movl	-12(%rbp), %eax
 213 01e2 3945EC   		cmpl	%eax, -20(%rbp)
 214 01e5 77D1     		ja	.L10
GAS LISTING /tmp/ccAJyxY4.s 			page 7


 215              	.L8:
 216              	.LBE2:
  64:gameboy.c     ****         }
  65:gameboy.c     ****     }
  66:gameboy.c     **** 
  67:gameboy.c     ****     return ptr;
 217              		.loc 1 67 12
 218 01e7 488B45F8 		movq	-8(%rbp), %rax
  68:gameboy.c     **** }
 219              		.loc 1 68 1
 220 01eb C9       		leave
 221              		.cfi_def_cfa 7, 8
 222 01ec C3       		ret
 223              		.cfi_endproc
 224              	.LFE7:
 225              		.size	create_buffer8, .-create_buffer8
 226              		.globl	gameboy_init
 227              		.type	gameboy_init, @function
 228              	gameboy_init:
 229              	.LFB8:
  69:gameboy.c     **** 
  70:gameboy.c     **** /*-------------------- GAMEBOY FUNCTIONS --------------------------- */
  71:gameboy.c     **** void gameboy_init(GAMEBOY *gb, const char *filename) {
 230              		.loc 1 71 54
 231              		.cfi_startproc
 232 01ed F30F1EFA 		endbr64
 233 01f1 55       		pushq	%rbp
 234              		.cfi_def_cfa_offset 16
 235              		.cfi_offset 6, -16
 236 01f2 4889E5   		movq	%rsp, %rbp
 237              		.cfi_def_cfa_register 6
 238 01f5 4883EC30 		subq	$48, %rsp
 239 01f9 48897DD8 		movq	%rdi, -40(%rbp)
 240 01fd 488975D0 		movq	%rsi, -48(%rbp)
  72:gameboy.c     ****     srand(time(NULL));
 241              		.loc 1 72 11
 242 0201 BF000000 		movl	$0, %edi
 242      00
 243 0206 E8000000 		call	time@PLT
 243      00
 244              		.loc 1 72 5
 245 020b 89C7     		movl	%eax, %edi
 246 020d E8000000 		call	srand@PLT
 246      00
  73:gameboy.c     **** 
  74:gameboy.c     ****     init_cpu(gb);
 247              		.loc 1 74 5
 248 0212 488B45D8 		movq	-40(%rbp), %rax
 249 0216 4889C7   		movq	%rax, %rdi
 250 0219 E8000000 		call	init_cpu
 250      00
  75:gameboy.c     **** 
  76:gameboy.c     **** 
  77:gameboy.c     ****     uint32_t total_banks = 200;
 251              		.loc 1 77 14
 252 021e C745ECC8 		movl	$200, -20(%rbp)
 252      000000
GAS LISTING /tmp/ccAJyxY4.s 			page 8


  78:gameboy.c     ****     uint32_t total_rom_size = GAMEBOY_ROM_BANK_SIZE * total_banks;
 253              		.loc 1 78 14
 254 0225 8B45EC   		movl	-20(%rbp), %eax
 255 0228 C1E00E   		sall	$14, %eax
 256 022b 8945F0   		movl	%eax, -16(%rbp)
  79:gameboy.c     ****     uint32_t total_sram_size = GAMEBOY_RAM_BANK_SIZE * total_banks;
 257              		.loc 1 79 14
 258 022e 8B45EC   		movl	-20(%rbp), %eax
 259 0231 C1E00D   		sall	$13, %eax
 260 0234 8945F4   		movl	%eax, -12(%rbp)
  80:gameboy.c     ****     uint32_t total_vram_size = GAMEBOY_VRAM_BANK_SIZE * 2; //max two banks
 261              		.loc 1 80 14
 262 0237 C745F800 		movl	$16384, -8(%rbp)
 262      400000
  81:gameboy.c     ****     uint32_t total_wram_size = GAMEBOY_RAM_BANK_SIZE * 8; // 8 kb
 263              		.loc 1 81 14
 264 023e C745FC00 		movl	$65536, -4(%rbp)
 264      000100
  82:gameboy.c     **** 
  83:gameboy.c     ****     gb->rom = create_buffer8(total_rom_size, false); // 200 banks of 16 kb
 265              		.loc 1 83 15
 266 0245 8B45F0   		movl	-16(%rbp), %eax
 267 0248 BE000000 		movl	$0, %esi
 267      00
 268 024d 89C7     		movl	%eax, %edi
 269 024f E8000000 		call	create_buffer8
 269      00
 270              		.loc 1 83 13
 271 0254 488B55D8 		movq	-40(%rbp), %rdx
 272 0258 48894210 		movq	%rax, 16(%rdx)
  84:gameboy.c     ****     gb->sram = create_buffer8(total_sram_size, true); // 200 banks of 8 kb
 273              		.loc 1 84 16
 274 025c 8B45F4   		movl	-12(%rbp), %eax
 275 025f BE010000 		movl	$1, %esi
 275      00
 276 0264 89C7     		movl	%eax, %edi
 277 0266 E8000000 		call	create_buffer8
 277      00
 278              		.loc 1 84 14
 279 026b 488B55D8 		movq	-40(%rbp), %rdx
 280 026f 48894220 		movq	%rax, 32(%rdx)
  85:gameboy.c     ****     gb->vram = create_buffer8(total_vram_size, true); // 8 kb
 281              		.loc 1 85 16
 282 0273 8B45F8   		movl	-8(%rbp), %eax
 283 0276 BE010000 		movl	$1, %esi
 283      00
 284 027b 89C7     		movl	%eax, %edi
 285 027d E8000000 		call	create_buffer8
 285      00
 286              		.loc 1 85 14
 287 0282 488B55D8 		movq	-40(%rbp), %rdx
 288 0286 48894218 		movq	%rax, 24(%rdx)
  86:gameboy.c     **** 
  87:gameboy.c     ****     gb->current_sram_bank = 0;
 289              		.loc 1 87 27
 290 028a 488B45D8 		movq	-40(%rbp), %rax
 291 028e C6405100 		movb	$0, 81(%rax)
GAS LISTING /tmp/ccAJyxY4.s 			page 9


  88:gameboy.c     ****     gb->current_wram_bank = 0;
 292              		.loc 1 88 27
 293 0292 488B45D8 		movq	-40(%rbp), %rax
 294 0296 C6405200 		movb	$0, 82(%rax)
  89:gameboy.c     ****     gb->current_rom_bank = 0;
 295              		.loc 1 89 26
 296 029a 488B45D8 		movq	-40(%rbp), %rax
 297 029e C6405000 		movb	$0, 80(%rax)
  90:gameboy.c     ****     gb->current_vram_bank = 0;
 298              		.loc 1 90 27
 299 02a2 488B45D8 		movq	-40(%rbp), %rax
 300 02a6 C6405300 		movb	$0, 83(%rax)
  91:gameboy.c     **** 
  92:gameboy.c     ****     read_file_into_buffer(filename, gb->rom, total_rom_size);
 301              		.loc 1 92 5
 302 02aa 8B55F0   		movl	-16(%rbp), %edx
 303 02ad 488B45D8 		movq	-40(%rbp), %rax
 304 02b1 488B4810 		movq	16(%rax), %rcx
 305 02b5 488B45D0 		movq	-48(%rbp), %rax
 306 02b9 4889CE   		movq	%rcx, %rsi
 307 02bc 4889C7   		movq	%rax, %rdi
 308 02bf E8000000 		call	read_file_into_buffer
 308      00
  93:gameboy.c     **** }
 309              		.loc 1 93 1
 310 02c4 90       		nop
 311 02c5 C9       		leave
 312              		.cfi_def_cfa 7, 8
 313 02c6 C3       		ret
 314              		.cfi_endproc
 315              	.LFE8:
 316              		.size	gameboy_init, .-gameboy_init
 317              		.section	.rodata
 318 0087 00       		.align 8
 319              	.LC7:
 320 0088 4572726F 		.string	"Error: Memory allocation failed"
 320      723A204D 
 320      656D6F72 
 320      7920616C 
 320      6C6F6361 
 321              		.align 8
 322              	.LC8:
 323 00a8 0A413A20 		.string	"\nA: %02X\t F:%02X (%s)\nB: %02X\t C:%02X\nD: %02X\t E:%02X\nH: %02X\t L:%02X\nSP: %04X\t
 323      25303258 
 323      0920463A 
 323      25303258 
 323      20282573 
 324              	.LC9:
 325 0101 25303258 		.string	"%02X "
 325      2000
 326              		.text
 327              		.globl	gameboy_status
 328              		.type	gameboy_status, @function
 329              	gameboy_status:
 330              	.LFB9:
  94:gameboy.c     **** 
  95:gameboy.c     **** void gameboy_status(GAMEBOY *gb) {
GAS LISTING /tmp/ccAJyxY4.s 			page 10


 331              		.loc 1 95 34
 332              		.cfi_startproc
 333 02c7 F30F1EFA 		endbr64
 334 02cb 55       		pushq	%rbp
 335              		.cfi_def_cfa_offset 16
 336              		.cfi_offset 6, -16
 337 02cc 4889E5   		movq	%rsp, %rbp
 338              		.cfi_def_cfa_register 6
 339 02cf 4155     		pushq	%r13
 340 02d1 4154     		pushq	%r12
 341 02d3 53       		pushq	%rbx
 342 02d4 4881EC48 		subq	$1096, %rsp
 342      040000
 343              		.cfi_offset 13, -24
 344              		.cfi_offset 12, -32
 345              		.cfi_offset 3, -40
 346 02db 4889BDA8 		movq	%rdi, -1112(%rbp)
 346      FBFFFF
 347              		.loc 1 95 34
 348 02e2 64488B04 		movq	%fs:40, %rax
 348      25280000 
 348      00
 349 02eb 488945D8 		movq	%rax, -40(%rbp)
 350 02ef 31C0     		xorl	%eax, %eax
  96:gameboy.c     ****     // get binary rep of F
  97:gameboy.c     ****     // create a buffer to store string representation of gameboy status 
  98:gameboy.c     ****     char buffer[1024];
  99:gameboy.c     ****     char *f_binary = display_register(gb->f);
 351              		.loc 1 99 41
 352 02f1 488B85A8 		movq	-1112(%rbp), %rax
 352      FBFFFF
 353 02f8 0FB64001 		movzbl	1(%rax), %eax
 354              		.loc 1 99 22
 355 02fc 0FB6C0   		movzbl	%al, %eax
 356 02ff 89C7     		movl	%eax, %edi
 357 0301 E8000000 		call	display_register
 357      00
 358 0306 488985C0 		movq	%rax, -1088(%rbp)
 358      FBFFFF
 100:gameboy.c     ****     if (f_binary == NULL) {
 359              		.loc 1 100 8
 360 030d 4883BDC0 		cmpq	$0, -1088(%rbp)
 360      FBFFFF00 
 361 0315 7514     		jne	.L14
 101:gameboy.c     ****         printf("Error: Memory allocation failed\n");
 362              		.loc 1 101 9
 363 0317 488D0500 		leaq	.LC7(%rip), %rax
 363      000000
 364 031e 4889C7   		movq	%rax, %rdi
 365 0321 E8000000 		call	puts@PLT
 365      00
 366 0326 E9740100 		jmp	.L13
 366      00
 367              	.L14:
 102:gameboy.c     ****         return;
 103:gameboy.c     ****     }
 104:gameboy.c     **** 
GAS LISTING /tmp/ccAJyxY4.s 			page 11


 105:gameboy.c     ****     sprintf(buffer,
 106:gameboy.c     ****             "\nA: %02X\t F:%02X (%s)\n"
 107:gameboy.c     ****             "B: %02X\t C:%02X\n"
 108:gameboy.c     ****             "D: %02X\t E:%02X\n"
 109:gameboy.c     ****             "H: %02X\t L:%02X\n"
 110:gameboy.c     ****             "SP: %04X\t PC:%04X\n",
 111:gameboy.c     ****             gb->a, gb->f, f_binary,
 112:gameboy.c     ****             gb->b, gb->c,
 113:gameboy.c     ****             gb->d, gb->e,
 114:gameboy.c     ****             gb->h, gb->l,
 115:gameboy.c     ****             gb->sp, gb->pc);
 368              		.loc 1 115 23
 369 032b 488B85A8 		movq	-1112(%rbp), %rax
 369      FBFFFF
 370 0332 0FB7400A 		movzwl	10(%rax), %eax
 105:gameboy.c     ****             "\nA: %02X\t F:%02X (%s)\n"
 371              		.loc 1 105 5
 372 0336 0FB7D8   		movzwl	%ax, %ebx
 373              		.loc 1 115 15
 374 0339 488B85A8 		movq	-1112(%rbp), %rax
 374      FBFFFF
 375 0340 0FB74008 		movzwl	8(%rax), %eax
 105:gameboy.c     ****             "\nA: %02X\t F:%02X (%s)\n"
 376              		.loc 1 105 5
 377 0344 440FB7D8 		movzwl	%ax, %r11d
 114:gameboy.c     ****             gb->sp, gb->pc);
 378              		.loc 1 114 22
 379 0348 488B85A8 		movq	-1112(%rbp), %rax
 379      FBFFFF
 380 034f 0FB64007 		movzbl	7(%rax), %eax
 105:gameboy.c     ****             "\nA: %02X\t F:%02X (%s)\n"
 381              		.loc 1 105 5
 382 0353 440FB6D0 		movzbl	%al, %r10d
 114:gameboy.c     ****             gb->sp, gb->pc);
 383              		.loc 1 114 15
 384 0357 488B85A8 		movq	-1112(%rbp), %rax
 384      FBFFFF
 385 035e 0FB64006 		movzbl	6(%rax), %eax
 105:gameboy.c     ****             "\nA: %02X\t F:%02X (%s)\n"
 386              		.loc 1 105 5
 387 0362 440FB6C8 		movzbl	%al, %r9d
 113:gameboy.c     ****             gb->h, gb->l,
 388              		.loc 1 113 22
 389 0366 488B85A8 		movq	-1112(%rbp), %rax
 389      FBFFFF
 390 036d 0FB64005 		movzbl	5(%rax), %eax
 105:gameboy.c     ****             "\nA: %02X\t F:%02X (%s)\n"
 391              		.loc 1 105 5
 392 0371 440FB6C0 		movzbl	%al, %r8d
 113:gameboy.c     ****             gb->h, gb->l,
 393              		.loc 1 113 15
 394 0375 488B85A8 		movq	-1112(%rbp), %rax
 394      FBFFFF
 395 037c 0FB64004 		movzbl	4(%rax), %eax
 105:gameboy.c     ****             "\nA: %02X\t F:%02X (%s)\n"
 396              		.loc 1 105 5
 397 0380 0FB6F8   		movzbl	%al, %edi
GAS LISTING /tmp/ccAJyxY4.s 			page 12


 112:gameboy.c     ****             gb->d, gb->e,
 398              		.loc 1 112 22
 399 0383 488B85A8 		movq	-1112(%rbp), %rax
 399      FBFFFF
 400 038a 0FB64003 		movzbl	3(%rax), %eax
 105:gameboy.c     ****             "\nA: %02X\t F:%02X (%s)\n"
 401              		.loc 1 105 5
 402 038e 0FB6F0   		movzbl	%al, %esi
 112:gameboy.c     ****             gb->d, gb->e,
 403              		.loc 1 112 15
 404 0391 488B85A8 		movq	-1112(%rbp), %rax
 404      FBFFFF
 405 0398 0FB64002 		movzbl	2(%rax), %eax
 105:gameboy.c     ****             "\nA: %02X\t F:%02X (%s)\n"
 406              		.loc 1 105 5
 407 039c 440FB6E8 		movzbl	%al, %r13d
 111:gameboy.c     ****             gb->b, gb->c,
 408              		.loc 1 111 22
 409 03a0 488B85A8 		movq	-1112(%rbp), %rax
 409      FBFFFF
 410 03a7 0FB64001 		movzbl	1(%rax), %eax
 105:gameboy.c     ****             "\nA: %02X\t F:%02X (%s)\n"
 411              		.loc 1 105 5
 412 03ab 0FB6C8   		movzbl	%al, %ecx
 111:gameboy.c     ****             gb->b, gb->c,
 413              		.loc 1 111 15
 414 03ae 488B85A8 		movq	-1112(%rbp), %rax
 414      FBFFFF
 415 03b5 0FB600   		movzbl	(%rax), %eax
 105:gameboy.c     ****             "\nA: %02X\t F:%02X (%s)\n"
 416              		.loc 1 105 5
 417 03b8 0FB6D0   		movzbl	%al, %edx
 418 03bb 4C8BA5C0 		movq	-1088(%rbp), %r12
 418      FBFFFF
 419 03c2 488D85D0 		leaq	-1072(%rbp), %rax
 419      FBFFFF
 420 03c9 4883EC08 		subq	$8, %rsp
 421 03cd 53       		pushq	%rbx
 422 03ce 4153     		pushq	%r11
 423 03d0 4152     		pushq	%r10
 424 03d2 4151     		pushq	%r9
 425 03d4 4150     		pushq	%r8
 426 03d6 57       		pushq	%rdi
 427 03d7 56       		pushq	%rsi
 428 03d8 4589E9   		movl	%r13d, %r9d
 429 03db 4D89E0   		movq	%r12, %r8
 430 03de 488D3500 		leaq	.LC8(%rip), %rsi
 430      000000
 431 03e5 4889C7   		movq	%rax, %rdi
 432 03e8 B8000000 		movl	$0, %eax
 432      00
 433 03ed E8000000 		call	sprintf@PLT
 433      00
 434 03f2 4883C440 		addq	$64, %rsp
 116:gameboy.c     **** 
 117:gameboy.c     ****     // get the next 10 elements in ROM based on current PC
 118:gameboy.c     ****     Register16 *pc = &gb->pc;
GAS LISTING /tmp/ccAJyxY4.s 			page 13


 435              		.loc 1 118 17
 436 03f6 488B85A8 		movq	-1112(%rbp), %rax
 436      FBFFFF
 437 03fd 4883C00A 		addq	$10, %rax
 438 0401 488985C8 		movq	%rax, -1080(%rbp)
 438      FBFFFF
 439              	.LBB3:
 119:gameboy.c     ****     for (int i = 0; i < 10; i++) {
 440              		.loc 1 119 14
 441 0408 C785BCFB 		movl	$0, -1092(%rbp)
 441      FFFF0000 
 441      0000
 442              		.loc 1 119 5
 443 0412 EB64     		jmp	.L16
 444              	.L17:
 120:gameboy.c     ****         sprintf(buffer + strlen(buffer), "%02X ", gb->rom[*pc + i]);
 445              		.loc 1 120 53 discriminator 3
 446 0414 488B85A8 		movq	-1112(%rbp), %rax
 446      FBFFFF
 447 041b 488B5010 		movq	16(%rax), %rdx
 448              		.loc 1 120 59 discriminator 3
 449 041f 488B85C8 		movq	-1080(%rbp), %rax
 449      FBFFFF
 450 0426 0FB700   		movzwl	(%rax), %eax
 451 0429 0FB7C8   		movzwl	%ax, %ecx
 452              		.loc 1 120 63 discriminator 3
 453 042c 8B85BCFB 		movl	-1092(%rbp), %eax
 453      FFFF
 454 0432 01C8     		addl	%ecx, %eax
 455 0434 4898     		cltq
 456              		.loc 1 120 58 discriminator 3
 457 0436 4801D0   		addq	%rdx, %rax
 458 0439 0FB600   		movzbl	(%rax), %eax
 459              		.loc 1 120 9 discriminator 3
 460 043c 0FB6D8   		movzbl	%al, %ebx
 461              		.loc 1 120 26 discriminator 3
 462 043f 488D85D0 		leaq	-1072(%rbp), %rax
 462      FBFFFF
 463 0446 4889C7   		movq	%rax, %rdi
 464 0449 E8000000 		call	strlen@PLT
 464      00
 465              		.loc 1 120 9 discriminator 3
 466 044e 488D95D0 		leaq	-1072(%rbp), %rdx
 466      FBFFFF
 467 0455 4801D0   		addq	%rdx, %rax
 468 0458 89DA     		movl	%ebx, %edx
 469 045a 488D0D00 		leaq	.LC9(%rip), %rcx
 469      000000
 470 0461 4889CE   		movq	%rcx, %rsi
 471 0464 4889C7   		movq	%rax, %rdi
 472 0467 B8000000 		movl	$0, %eax
 472      00
 473 046c E8000000 		call	sprintf@PLT
 473      00
 119:gameboy.c     ****     for (int i = 0; i < 10; i++) {
 474              		.loc 1 119 30 discriminator 3
 475 0471 8385BCFB 		addl	$1, -1092(%rbp)
GAS LISTING /tmp/ccAJyxY4.s 			page 14


 475      FFFF01
 476              	.L16:
 119:gameboy.c     ****     for (int i = 0; i < 10; i++) {
 477              		.loc 1 119 23 discriminator 1
 478 0478 83BDBCFB 		cmpl	$9, -1092(%rbp)
 478      FFFF09
 479 047f 7E93     		jle	.L17
 480              	.LBE3:
 121:gameboy.c     ****     }
 122:gameboy.c     **** 
 123:gameboy.c     ****     printf("%s\n", buffer);
 481              		.loc 1 123 5
 482 0481 488D85D0 		leaq	-1072(%rbp), %rax
 482      FBFFFF
 483 0488 4889C7   		movq	%rax, %rdi
 484 048b E8000000 		call	puts@PLT
 484      00
 124:gameboy.c     ****     free(f_binary);
 485              		.loc 1 124 5
 486 0490 488B85C0 		movq	-1088(%rbp), %rax
 486      FBFFFF
 487 0497 4889C7   		movq	%rax, %rdi
 488 049a E8000000 		call	free@PLT
 488      00
 489              	.L13:
 125:gameboy.c     **** }
 490              		.loc 1 125 1
 491 049f 488B45D8 		movq	-40(%rbp), %rax
 492 04a3 64482B04 		subq	%fs:40, %rax
 492      25280000 
 492      00
 493 04ac 7405     		je	.L18
 494 04ae E8000000 		call	__stack_chk_fail@PLT
 494      00
 495              	.L18:
 496 04b3 488D65E8 		leaq	-24(%rbp), %rsp
 497 04b7 5B       		popq	%rbx
 498 04b8 415C     		popq	%r12
 499 04ba 415D     		popq	%r13
 500 04bc 5D       		popq	%rbp
 501              		.cfi_def_cfa 7, 8
 502 04bd C3       		ret
 503              		.cfi_endproc
 504              	.LFE9:
 505              		.size	gameboy_status, .-gameboy_status
 506              		.globl	display_register
 507              		.type	display_register, @function
 508              	display_register:
 509              	.LFB10:
 126:gameboy.c     **** 
 127:gameboy.c     **** 
 128:gameboy.c     **** char* display_register(uint8_t reg) {
 510              		.loc 1 128 37
 511              		.cfi_startproc
 512 04be F30F1EFA 		endbr64
 513 04c2 55       		pushq	%rbp
 514              		.cfi_def_cfa_offset 16
GAS LISTING /tmp/ccAJyxY4.s 			page 15


 515              		.cfi_offset 6, -16
 516 04c3 4889E5   		movq	%rsp, %rbp
 517              		.cfi_def_cfa_register 6
 518 04c6 4883EC20 		subq	$32, %rsp
 519 04ca 89F8     		movl	%edi, %eax
 520 04cc 8845EC   		movb	%al, -20(%rbp)
 129:gameboy.c     ****     // create a buffer to store the binary representation of the register
 130:gameboy.c     ****     char *buffer = (char *)calloc(10, sizeof(char));
 521              		.loc 1 130 28
 522 04cf BE010000 		movl	$1, %esi
 522      00
 523 04d4 BF0A0000 		movl	$10, %edi
 523      00
 524 04d9 E8000000 		call	calloc@PLT
 524      00
 525 04de 488945F8 		movq	%rax, -8(%rbp)
 131:gameboy.c     **** 
 132:gameboy.c     ****     if (buffer == NULL) {
 526              		.loc 1 132 8
 527 04e2 48837DF8 		cmpq	$0, -8(%rbp)
 527      00
 528 04e7 7516     		jne	.L20
 133:gameboy.c     ****         printf("Error: Memory allocation failed\n");
 529              		.loc 1 133 9
 530 04e9 488D0500 		leaq	.LC7(%rip), %rax
 530      000000
 531 04f0 4889C7   		movq	%rax, %rdi
 532 04f3 E8000000 		call	puts@PLT
 532      00
 134:gameboy.c     ****         return NULL;
 533              		.loc 1 134 16
 534 04f8 B8000000 		movl	$0, %eax
 534      00
 535 04fd EB75     		jmp	.L21
 536              	.L20:
 537              	.LBB4:
 135:gameboy.c     ****     }
 136:gameboy.c     **** 
 137:gameboy.c     ****     for (int i = 7; i >= 0; i--) {
 538              		.loc 1 137 14
 539 04ff C745F407 		movl	$7, -12(%rbp)
 539      000000
 540              		.loc 1 137 5
 541 0506 EB57     		jmp	.L22
 542              	.L26:
 138:gameboy.c     **** 
 139:gameboy.c     ****         buffer[7 - i + (i < 4)] = (reg >> i) & 1 ? '1' : '0';
 543              		.loc 1 139 40
 544 0508 0FB655EC 		movzbl	-20(%rbp), %edx
 545 050c 8B45F4   		movl	-12(%rbp), %eax
 546 050f 89C1     		movl	%eax, %ecx
 547 0511 D3FA     		sarl	%cl, %edx
 548 0513 89D0     		movl	%edx, %eax
 549              		.loc 1 139 46
 550 0515 83E001   		andl	$1, %eax
 551              		.loc 1 139 33
 552 0518 85C0     		testl	%eax, %eax
GAS LISTING /tmp/ccAJyxY4.s 			page 16


 553 051a 7407     		je	.L23
 554              		.loc 1 139 33 is_stmt 0 discriminator 1
 555 051c BA310000 		movl	$49, %edx
 555      00
 556 0521 EB05     		jmp	.L24
 557              	.L23:
 558              		.loc 1 139 33 discriminator 2
 559 0523 BA300000 		movl	$48, %edx
 559      00
 560              	.L24:
 561              		.loc 1 139 18 is_stmt 1 discriminator 4
 562 0528 B8070000 		movl	$7, %eax
 562      00
 563 052d 2B45F4   		subl	-12(%rbp), %eax
 564 0530 89C1     		movl	%eax, %ecx
 565              		.loc 1 139 27 discriminator 4
 566 0532 837DF403 		cmpl	$3, -12(%rbp)
 567 0536 0F9EC0   		setle	%al
 568 0539 0FB6C0   		movzbl	%al, %eax
 569              		.loc 1 139 22 discriminator 4
 570 053c 01C8     		addl	%ecx, %eax
 571 053e 4863C8   		movslq	%eax, %rcx
 572              		.loc 1 139 15 discriminator 4
 573 0541 488B45F8 		movq	-8(%rbp), %rax
 574 0545 4801C8   		addq	%rcx, %rax
 575              		.loc 1 139 33 discriminator 4
 576 0548 8810     		movb	%dl, (%rax)
 140:gameboy.c     ****         if (i == 4) {
 577              		.loc 1 140 12 discriminator 4
 578 054a 837DF404 		cmpl	$4, -12(%rbp)
 579 054e 750B     		jne	.L25
 141:gameboy.c     ****             buffer[4] = '_';
 580              		.loc 1 141 19
 581 0550 488B45F8 		movq	-8(%rbp), %rax
 582 0554 4883C004 		addq	$4, %rax
 583              		.loc 1 141 23
 584 0558 C6005F   		movb	$95, (%rax)
 585              	.L25:
 137:gameboy.c     **** 
 586              		.loc 1 137 30 discriminator 2
 587 055b 836DF401 		subl	$1, -12(%rbp)
 588              	.L22:
 137:gameboy.c     **** 
 589              		.loc 1 137 23 discriminator 1
 590 055f 837DF400 		cmpl	$0, -12(%rbp)
 591 0563 79A3     		jns	.L26
 592              	.LBE4:
 142:gameboy.c     ****         }
 143:gameboy.c     ****     }
 144:gameboy.c     **** 
 145:gameboy.c     ****     buffer[9] = '\0';
 593              		.loc 1 145 11
 594 0565 488B45F8 		movq	-8(%rbp), %rax
 595 0569 4883C009 		addq	$9, %rax
 596              		.loc 1 145 15
 597 056d C60000   		movb	$0, (%rax)
 146:gameboy.c     ****     return buffer;
GAS LISTING /tmp/ccAJyxY4.s 			page 17


 598              		.loc 1 146 12
 599 0570 488B45F8 		movq	-8(%rbp), %rax
 600              	.L21:
 147:gameboy.c     **** }
 601              		.loc 1 147 1
 602 0574 C9       		leave
 603              		.cfi_def_cfa 7, 8
 604 0575 C3       		ret
 605              		.cfi_endproc
 606              	.LFE10:
 607              		.size	display_register, .-display_register
 608              		.section	.rodata
 609 0107 00       		.align 8
 610              	.LC10:
 611 0108 4572726F 		.string	"Error: Address %04X out of bounds\n"
 611      723A2041 
 611      64647265 
 611      73732025 
 611      30345820 
 612              		.text
 613              		.globl	gameboy_memset
 614              		.type	gameboy_memset, @function
 615              	gameboy_memset:
 616              	.LFB11:
 148:gameboy.c     **** 
 149:gameboy.c     **** 
 150:gameboy.c     **** void gameboy_memset(GAMEBOY *gb, uint16_t addr, uint8_t val) {
 617              		.loc 1 150 62
 618              		.cfi_startproc
 619 0576 F30F1EFA 		endbr64
 620 057a 55       		pushq	%rbp
 621              		.cfi_def_cfa_offset 16
 622              		.cfi_offset 6, -16
 623 057b 4889E5   		movq	%rsp, %rbp
 624              		.cfi_def_cfa_register 6
 625 057e 4883EC10 		subq	$16, %rsp
 626 0582 48897DF8 		movq	%rdi, -8(%rbp)
 627 0586 89F0     		movl	%esi, %eax
 628 0588 668945F4 		movw	%ax, -12(%rbp)
 629 058c 89D0     		movl	%edx, %eax
 630 058e 8845F0   		movb	%al, -16(%rbp)
 151:gameboy.c     ****     
 152:gameboy.c     ****     // ROM0
 153:gameboy.c     ****     if (0x0000 <= addr && addr < 0x4000) {
 631              		.loc 1 153 8
 632 0591 66817DF4 		cmpw	$16383, -12(%rbp)
 632      FF3F
 633 0597 0F862302 		jbe	.L40
 633      0000
 154:gameboy.c     ****         return;
 155:gameboy.c     ****     }
 156:gameboy.c     ****     // ROMX 
 157:gameboy.c     ****     if (0x4000 <= addr && addr < 0x8000) {
 634              		.loc 1 157 8
 635 059d 66817DF4 		cmpw	$16383, -12(%rbp)
 635      FF3F
 636 05a3 760D     		jbe	.L30
GAS LISTING /tmp/ccAJyxY4.s 			page 18


 637              		.loc 1 157 32 discriminator 1
 638 05a5 0FB745F4 		movzwl	-12(%rbp), %eax
 639              		.loc 1 157 24 discriminator 1
 640 05a9 6685C0   		testw	%ax, %ax
 641 05ac 0F891102 		jns	.L41
 641      0000
 642              	.L30:
 158:gameboy.c     ****         return;
 159:gameboy.c     ****     }
 160:gameboy.c     ****     
 161:gameboy.c     ****     // VRAM
 162:gameboy.c     ****     if (0x8000 <= addr && addr < 0xA000) {
 643              		.loc 1 162 16
 644 05b2 0FB745F4 		movzwl	-12(%rbp), %eax
 645              		.loc 1 162 8
 646 05b6 6685C0   		testw	%ax, %ax
 647 05b9 793A     		jns	.L31
 648              		.loc 1 162 24 discriminator 1
 649 05bb 66817DF4 		cmpw	$-24577, -12(%rbp)
 649      FF9F
 650 05c1 7732     		ja	.L31
 163:gameboy.c     ****         gb->vram[addr - 0x8000 + (gb->current_vram_bank * GAMEBOY_VRAM_BANK_SIZE)] = val;
 651              		.loc 1 163 11
 652 05c3 488B45F8 		movq	-8(%rbp), %rax
 653 05c7 488B5018 		movq	24(%rax), %rdx
 654              		.loc 1 163 23
 655 05cb 0FB745F4 		movzwl	-12(%rbp), %eax
 656 05cf 8D880080 		leal	-32768(%rax), %ecx
 656      FFFF
 657              		.loc 1 163 37
 658 05d5 488B45F8 		movq	-8(%rbp), %rax
 659 05d9 0FB64053 		movzbl	83(%rax), %eax
 660 05dd 0FB6C0   		movzbl	%al, %eax
 661              		.loc 1 163 57
 662 05e0 C1E00D   		sall	$13, %eax
 663              		.loc 1 163 32
 664 05e3 01C8     		addl	%ecx, %eax
 665 05e5 4898     		cltq
 666              		.loc 1 163 17
 667 05e7 4801C2   		addq	%rax, %rdx
 668              		.loc 1 163 84
 669 05ea 0FB645F0 		movzbl	-16(%rbp), %eax
 670 05ee 8802     		movb	%al, (%rdx)
 164:gameboy.c     ****         return;
 671              		.loc 1 164 9
 672 05f0 E9CF0100 		jmp	.L27
 672      00
 673              	.L31:
 165:gameboy.c     ****     } 
 166:gameboy.c     ****     // SRAMX
 167:gameboy.c     ****     if (0xA000 <= addr && addr < 0xC000) {
 674              		.loc 1 167 8
 675 05f5 66817DF4 		cmpw	$-24577, -12(%rbp)
 675      FF9F
 676 05fb 763A     		jbe	.L32
 677              		.loc 1 167 24 discriminator 1
 678 05fd 66817DF4 		cmpw	$-16385, -12(%rbp)
GAS LISTING /tmp/ccAJyxY4.s 			page 19


 678      FFBF
 679 0603 7732     		ja	.L32
 168:gameboy.c     ****         gb->sram[addr - 0xA000 + (gb->current_sram_bank * GAMEBOY_RAM_BANK_SIZE)] = val;
 680              		.loc 1 168 11
 681 0605 488B45F8 		movq	-8(%rbp), %rax
 682 0609 488B5020 		movq	32(%rax), %rdx
 683              		.loc 1 168 23
 684 060d 0FB745F4 		movzwl	-12(%rbp), %eax
 685 0611 8D880060 		leal	-40960(%rax), %ecx
 685      FFFF
 686              		.loc 1 168 37
 687 0617 488B45F8 		movq	-8(%rbp), %rax
 688 061b 0FB64051 		movzbl	81(%rax), %eax
 689 061f 0FB6C0   		movzbl	%al, %eax
 690              		.loc 1 168 57
 691 0622 C1E00D   		sall	$13, %eax
 692              		.loc 1 168 32
 693 0625 01C8     		addl	%ecx, %eax
 694 0627 4898     		cltq
 695              		.loc 1 168 17
 696 0629 4801C2   		addq	%rax, %rdx
 697              		.loc 1 168 83
 698 062c 0FB645F0 		movzbl	-16(%rbp), %eax
 699 0630 8802     		movb	%al, (%rdx)
 169:gameboy.c     ****         return;
 700              		.loc 1 169 9
 701 0632 E98D0100 		jmp	.L27
 701      00
 702              	.L32:
 170:gameboy.c     ****     }
 171:gameboy.c     **** 
 172:gameboy.c     ****     // WRAM0
 173:gameboy.c     ****     if (0xC000 <= addr && addr < 0xD000) {
 703              		.loc 1 173 8
 704 0637 66817DF4 		cmpw	$-16385, -12(%rbp)
 704      FFBF
 705 063d 7628     		jbe	.L33
 706              		.loc 1 173 24 discriminator 1
 707 063f 66817DF4 		cmpw	$-12289, -12(%rbp)
 707      FFCF
 708 0645 7720     		ja	.L33
 174:gameboy.c     ****         gb->wram[addr - 0xC000] = val;
 709              		.loc 1 174 11
 710 0647 488B45F8 		movq	-8(%rbp), %rax
 711 064b 488B5028 		movq	40(%rax), %rdx
 712              		.loc 1 174 17
 713 064f 0FB745F4 		movzwl	-12(%rbp), %eax
 714 0653 482D00C0 		subq	$49152, %rax
 714      0000
 715 0659 4801C2   		addq	%rax, %rdx
 716              		.loc 1 174 33
 717 065c 0FB645F0 		movzbl	-16(%rbp), %eax
 718 0660 8802     		movb	%al, (%rdx)
 175:gameboy.c     ****         return;
 719              		.loc 1 175 9
 720 0662 E95D0100 		jmp	.L27
 720      00
GAS LISTING /tmp/ccAJyxY4.s 			page 20


 721              	.L33:
 176:gameboy.c     ****     }
 177:gameboy.c     **** 
 178:gameboy.c     ****     // WRAMX
 179:gameboy.c     ****     if (0xD000 <= addr && addr < 0xE000) {
 722              		.loc 1 179 8
 723 0667 66817DF4 		cmpw	$-12289, -12(%rbp)
 723      FFCF
 724 066d 7644     		jbe	.L34
 725              		.loc 1 179 24 discriminator 1
 726 066f 66817DF4 		cmpw	$-8193, -12(%rbp)
 726      FFDF
 727 0675 773C     		ja	.L34
 180:gameboy.c     ****         gb->wram[addr - 0xD000 + (MAX(gb->current_wram_bank, 1) * GAMEBOY_RAM_BANK_SIZE)] = val;
 728              		.loc 1 180 11
 729 0677 488B45F8 		movq	-8(%rbp), %rax
 730 067b 488B5028 		movq	40(%rax), %rdx
 731              		.loc 1 180 23
 732 067f 0FB745F4 		movzwl	-12(%rbp), %eax
 733 0683 8DB00030 		leal	-53248(%rax), %esi
 733      FFFF
 734              		.loc 1 180 35
 735 0689 488B45F8 		movq	-8(%rbp), %rax
 736 068d 0FB64052 		movzbl	82(%rax), %eax
 737 0691 B9010000 		movl	$1, %ecx
 737      00
 738 0696 84C0     		testb	%al, %al
 739 0698 0F44C1   		cmove	%ecx, %eax
 740 069b 0FB6C0   		movzbl	%al, %eax
 741              		.loc 1 180 65
 742 069e C1E00D   		sall	$13, %eax
 743              		.loc 1 180 32
 744 06a1 01F0     		addl	%esi, %eax
 745 06a3 4898     		cltq
 746              		.loc 1 180 17
 747 06a5 4801C2   		addq	%rax, %rdx
 748              		.loc 1 180 91
 749 06a8 0FB645F0 		movzbl	-16(%rbp), %eax
 750 06ac 8802     		movb	%al, (%rdx)
 181:gameboy.c     ****         return;
 751              		.loc 1 181 9
 752 06ae E9110100 		jmp	.L27
 752      00
 753              	.L34:
 182:gameboy.c     ****     }
 183:gameboy.c     **** 
 184:gameboy.c     ****     // ECHO
 185:gameboy.c     ****     if (0xE000 <= addr && addr < 0xFE00) {
 754              		.loc 1 185 8
 755 06b3 66817DF4 		cmpw	$-8193, -12(%rbp)
 755      FFDF
 756 06b9 7644     		jbe	.L35
 757              		.loc 1 185 24 discriminator 1
 758 06bb 66817DF4 		cmpw	$-513, -12(%rbp)
 758      FFFD
 759 06c1 773C     		ja	.L35
 186:gameboy.c     ****         gb->wram[addr - 0xE000 - 0xC000 + (MAX(gb->current_wram_bank, 1) * GAMEBOY_RAM_BANK_SIZE)] 
GAS LISTING /tmp/ccAJyxY4.s 			page 21


 760              		.loc 1 186 11
 761 06c3 488B45F8 		movq	-8(%rbp), %rax
 762 06c7 488B5028 		movq	40(%rax), %rdx
 763              		.loc 1 186 32
 764 06cb 0FB745F4 		movzwl	-12(%rbp), %eax
 765 06cf 8DB00060 		leal	-106496(%rax), %esi
 765      FEFF
 766              		.loc 1 186 44
 767 06d5 488B45F8 		movq	-8(%rbp), %rax
 768 06d9 0FB64052 		movzbl	82(%rax), %eax
 769 06dd B9010000 		movl	$1, %ecx
 769      00
 770 06e2 84C0     		testb	%al, %al
 771 06e4 0F44C1   		cmove	%ecx, %eax
 772 06e7 0FB6C0   		movzbl	%al, %eax
 773              		.loc 1 186 74
 774 06ea C1E00D   		sall	$13, %eax
 775              		.loc 1 186 41
 776 06ed 01F0     		addl	%esi, %eax
 777 06ef 4898     		cltq
 778              		.loc 1 186 17
 779 06f1 4801C2   		addq	%rax, %rdx
 780              		.loc 1 186 100
 781 06f4 0FB645F0 		movzbl	-16(%rbp), %eax
 782 06f8 8802     		movb	%al, (%rdx)
 187:gameboy.c     ****         return; 
 783              		.loc 1 187 9
 784 06fa E9C50000 		jmp	.L27
 784      00
 785              	.L35:
 188:gameboy.c     ****     }
 189:gameboy.c     **** 
 190:gameboy.c     ****     // OAM
 191:gameboy.c     ****     if (0xFE00 <= addr && addr < 0xFEA0) {
 786              		.loc 1 191 8
 787 06ff 66817DF4 		cmpw	$-513, -12(%rbp)
 787      FFFD
 788 0705 7628     		jbe	.L36
 789              		.loc 1 191 24 discriminator 1
 790 0707 66817DF4 		cmpw	$-353, -12(%rbp)
 790      9FFE
 791 070d 7720     		ja	.L36
 192:gameboy.c     ****         gb->oam[addr - 0xFE00] = val;
 792              		.loc 1 192 11
 793 070f 488B45F8 		movq	-8(%rbp), %rax
 794 0713 488B5030 		movq	48(%rax), %rdx
 795              		.loc 1 192 16
 796 0717 0FB745F4 		movzwl	-12(%rbp), %eax
 797 071b 482D00FE 		subq	$65024, %rax
 797      0000
 798 0721 4801C2   		addq	%rax, %rdx
 799              		.loc 1 192 32
 800 0724 0FB645F0 		movzbl	-16(%rbp), %eax
 801 0728 8802     		movb	%al, (%rdx)
 193:gameboy.c     ****         return;
 802              		.loc 1 193 9
 803 072a E9950000 		jmp	.L27
GAS LISTING /tmp/ccAJyxY4.s 			page 22


 803      00
 804              	.L36:
 194:gameboy.c     ****     }
 195:gameboy.c     ****     // UNUSED
 196:gameboy.c     ****     // IO
 197:gameboy.c     **** 
 198:gameboy.c     ****     if (0xFF00 <= addr && addr < 0xFF80) {
 805              		.loc 1 198 8
 806 072f 66817DF4 		cmpw	$-257, -12(%rbp)
 806      FFFE
 807 0735 7625     		jbe	.L37
 808              		.loc 1 198 24 discriminator 1
 809 0737 66817DF4 		cmpw	$-129, -12(%rbp)
 809      7FFF
 810 073d 771D     		ja	.L37
 199:gameboy.c     ****         gb->io[addr - 0xFF00] = val;
 811              		.loc 1 199 11
 812 073f 488B45F8 		movq	-8(%rbp), %rax
 813 0743 488B5038 		movq	56(%rax), %rdx
 814              		.loc 1 199 15
 815 0747 0FB745F4 		movzwl	-12(%rbp), %eax
 816 074b 482D00FF 		subq	$65280, %rax
 816      0000
 817 0751 4801C2   		addq	%rax, %rdx
 818              		.loc 1 199 31
 819 0754 0FB645F0 		movzbl	-16(%rbp), %eax
 820 0758 8802     		movb	%al, (%rdx)
 200:gameboy.c     ****         return;
 821              		.loc 1 200 9
 822 075a EB68     		jmp	.L27
 823              	.L37:
 201:gameboy.c     ****     }
 202:gameboy.c     **** 
 203:gameboy.c     ****     // HRAM
 204:gameboy.c     ****     if (0xFF80 <= addr && addr < 0xFFFF) {
 824              		.loc 1 204 8
 825 075c 66817DF4 		cmpw	$-129, -12(%rbp)
 825      7FFF
 826 0762 7624     		jbe	.L38
 827              		.loc 1 204 24 discriminator 1
 828 0764 66837DF4 		cmpw	$-1, -12(%rbp)
 828      FF
 829 0769 741D     		je	.L38
 205:gameboy.c     ****         gb->hram[addr - 0xFF80] = val;
 830              		.loc 1 205 11
 831 076b 488B45F8 		movq	-8(%rbp), %rax
 832 076f 488B5040 		movq	64(%rax), %rdx
 833              		.loc 1 205 17
 834 0773 0FB745F4 		movzwl	-12(%rbp), %eax
 835 0777 482D80FF 		subq	$65408, %rax
 835      0000
 836 077d 4801C2   		addq	%rax, %rdx
 837              		.loc 1 205 33
 838 0780 0FB645F0 		movzbl	-16(%rbp), %eax
 839 0784 8802     		movb	%al, (%rdx)
 206:gameboy.c     ****         return;
 840              		.loc 1 206 9
GAS LISTING /tmp/ccAJyxY4.s 			page 23


 841 0786 EB3C     		jmp	.L27
 842              	.L38:
 207:gameboy.c     ****     }
 208:gameboy.c     **** 
 209:gameboy.c     ****     // IE
 210:gameboy.c     ****     if (addr == 0xFFFF) {
 843              		.loc 1 210 8
 844 0788 66837DF4 		cmpw	$-1, -12(%rbp)
 844      FF
 845 078d 750D     		jne	.L39
 211:gameboy.c     ****         gb->ime = val;
 846              		.loc 1 211 17
 847 078f 488B45F8 		movq	-8(%rbp), %rax
 848 0793 0FB655F0 		movzbl	-16(%rbp), %edx
 849 0797 88500C   		movb	%dl, 12(%rax)
 212:gameboy.c     ****         return;
 850              		.loc 1 212 9
 851 079a EB28     		jmp	.L27
 852              	.L39:
 213:gameboy.c     ****     }
 214:gameboy.c     **** 
 215:gameboy.c     ****     printf("Error: Address %04X out of bounds\n", addr);
 853              		.loc 1 215 5
 854 079c 0FB745F4 		movzwl	-12(%rbp), %eax
 855 07a0 89C6     		movl	%eax, %esi
 856 07a2 488D0500 		leaq	.LC10(%rip), %rax
 856      000000
 857 07a9 4889C7   		movq	%rax, %rdi
 858 07ac B8000000 		movl	$0, %eax
 858      00
 859 07b1 E8000000 		call	printf@PLT
 859      00
 216:gameboy.c     ****     exit(1);
 860              		.loc 1 216 5
 861 07b6 BF010000 		movl	$1, %edi
 861      00
 862 07bb E8000000 		call	exit@PLT
 862      00
 863              	.L40:
 154:gameboy.c     ****     }
 864              		.loc 1 154 9
 865 07c0 90       		nop
 866 07c1 EB01     		jmp	.L27
 867              	.L41:
 158:gameboy.c     ****     }
 868              		.loc 1 158 9
 869 07c3 90       		nop
 870              	.L27:
 217:gameboy.c     **** }
 871              		.loc 1 217 1
 872 07c4 C9       		leave
 873              		.cfi_def_cfa 7, 8
 874 07c5 C3       		ret
 875              		.cfi_endproc
 876              	.LFE11:
 877              		.size	gameboy_memset, .-gameboy_memset
 878              		.globl	gameboy_free
GAS LISTING /tmp/ccAJyxY4.s 			page 24


 879              		.type	gameboy_free, @function
 880              	gameboy_free:
 881              	.LFB12:
 218:gameboy.c     **** 
 219:gameboy.c     **** void gameboy_free(GAMEBOY *gb) {
 882              		.loc 1 219 32
 883              		.cfi_startproc
 884 07c6 F30F1EFA 		endbr64
 885 07ca 55       		pushq	%rbp
 886              		.cfi_def_cfa_offset 16
 887              		.cfi_offset 6, -16
 888 07cb 4889E5   		movq	%rsp, %rbp
 889              		.cfi_def_cfa_register 6
 890 07ce 4883EC10 		subq	$16, %rsp
 891 07d2 48897DF8 		movq	%rdi, -8(%rbp)
 220:gameboy.c     ****     if (gb -> rom != NULL) {
 892              		.loc 1 220 12
 893 07d6 488B45F8 		movq	-8(%rbp), %rax
 894 07da 488B4010 		movq	16(%rax), %rax
 895              		.loc 1 220 8
 896 07de 4885C0   		testq	%rax, %rax
 897 07e1 7410     		je	.L43
 221:gameboy.c     ****         free(gb->rom);
 898              		.loc 1 221 16
 899 07e3 488B45F8 		movq	-8(%rbp), %rax
 900 07e7 488B4010 		movq	16(%rax), %rax
 901              		.loc 1 221 9
 902 07eb 4889C7   		movq	%rax, %rdi
 903 07ee E8000000 		call	free@PLT
 903      00
 904              	.L43:
 222:gameboy.c     ****     }
 223:gameboy.c     ****     gb->rom = NULL;
 905              		.loc 1 223 13
 906 07f3 488B45F8 		movq	-8(%rbp), %rax
 907 07f7 48C74010 		movq	$0, 16(%rax)
 907      00000000 
 224:gameboy.c     **** 
 225:gameboy.c     ****     if (gb->vram != NULL) {
 908              		.loc 1 225 11
 909 07ff 488B45F8 		movq	-8(%rbp), %rax
 910 0803 488B4018 		movq	24(%rax), %rax
 911              		.loc 1 225 8
 912 0807 4885C0   		testq	%rax, %rax
 913 080a 7410     		je	.L44
 226:gameboy.c     ****         free(gb->vram);
 914              		.loc 1 226 16
 915 080c 488B45F8 		movq	-8(%rbp), %rax
 916 0810 488B4018 		movq	24(%rax), %rax
 917              		.loc 1 226 9
 918 0814 4889C7   		movq	%rax, %rdi
 919 0817 E8000000 		call	free@PLT
 919      00
 920              	.L44:
 227:gameboy.c     ****     }
 228:gameboy.c     **** 
 229:gameboy.c     ****     gb->vram = NULL;
GAS LISTING /tmp/ccAJyxY4.s 			page 25


 921              		.loc 1 229 14
 922 081c 488B45F8 		movq	-8(%rbp), %rax
 923 0820 48C74018 		movq	$0, 24(%rax)
 923      00000000 
 230:gameboy.c     **** 
 231:gameboy.c     ****     if (gb->sram != NULL) {
 924              		.loc 1 231 11
 925 0828 488B45F8 		movq	-8(%rbp), %rax
 926 082c 488B4020 		movq	32(%rax), %rax
 927              		.loc 1 231 8
 928 0830 4885C0   		testq	%rax, %rax
 929 0833 7410     		je	.L45
 232:gameboy.c     ****         free(gb->sram);
 930              		.loc 1 232 16
 931 0835 488B45F8 		movq	-8(%rbp), %rax
 932 0839 488B4020 		movq	32(%rax), %rax
 933              		.loc 1 232 9
 934 083d 4889C7   		movq	%rax, %rdi
 935 0840 E8000000 		call	free@PLT
 935      00
 936              	.L45:
 233:gameboy.c     ****     }
 234:gameboy.c     **** 
 235:gameboy.c     ****     gb->sram = NULL;
 937              		.loc 1 235 14
 938 0845 488B45F8 		movq	-8(%rbp), %rax
 939 0849 48C74020 		movq	$0, 32(%rax)
 939      00000000 
 236:gameboy.c     **** 
 237:gameboy.c     **** 
 238:gameboy.c     **** }
 940              		.loc 1 238 1
 941 0851 90       		nop
 942 0852 C9       		leave
 943              		.cfi_def_cfa 7, 8
 944 0853 C3       		ret
 945              		.cfi_endproc
 946              	.LFE12:
 947              		.size	gameboy_free, .-gameboy_free
 948              		.globl	init_cpu
 949              		.type	init_cpu, @function
 950              	init_cpu:
 951              	.LFB13:
 239:gameboy.c     **** 
 240:gameboy.c     **** 
 241:gameboy.c     **** /*-------------------- CPU FUNCTIONS --------------------------- */
 242:gameboy.c     **** void init_cpu(GAMEBOY *gb) {
 952              		.loc 1 242 28
 953              		.cfi_startproc
 954 0854 F30F1EFA 		endbr64
 955 0858 55       		pushq	%rbp
 956              		.cfi_def_cfa_offset 16
 957              		.cfi_offset 6, -16
 958 0859 4889E5   		movq	%rsp, %rbp
 959              		.cfi_def_cfa_register 6
 960 085c 48897DF8 		movq	%rdi, -8(%rbp)
 243:gameboy.c     ****     if (gb->cgb_mode) {
GAS LISTING /tmp/ccAJyxY4.s 			page 26


 961              		.loc 1 243 11
 962 0860 488B45F8 		movq	-8(%rbp), %rax
 963 0864 0FB64054 		movzbl	84(%rax), %eax
 964              		.loc 1 243 8
 965 0868 84C0     		testb	%al, %al
 966 086a 7455     		je	.L47
 244:gameboy.c     ****         gb->a = 0x11;
 967              		.loc 1 244 15
 968 086c 488B45F8 		movq	-8(%rbp), %rax
 969 0870 C60011   		movb	$17, (%rax)
 245:gameboy.c     ****         gb->f = 0b10000000;
 970              		.loc 1 245 15
 971 0873 488B45F8 		movq	-8(%rbp), %rax
 972 0877 C6400180 		movb	$-128, 1(%rax)
 246:gameboy.c     ****         gb->b = 0x00;
 973              		.loc 1 246 15
 974 087b 488B45F8 		movq	-8(%rbp), %rax
 975 087f C6400200 		movb	$0, 2(%rax)
 247:gameboy.c     ****         gb->c = 0x00;
 976              		.loc 1 247 15
 977 0883 488B45F8 		movq	-8(%rbp), %rax
 978 0887 C6400300 		movb	$0, 3(%rax)
 248:gameboy.c     ****         gb->d = 0xFF;
 979              		.loc 1 248 15
 980 088b 488B45F8 		movq	-8(%rbp), %rax
 981 088f C64004FF 		movb	$-1, 4(%rax)
 249:gameboy.c     ****         gb->e = 0x56;
 982              		.loc 1 249 15
 983 0893 488B45F8 		movq	-8(%rbp), %rax
 984 0897 C6400556 		movb	$86, 5(%rax)
 250:gameboy.c     ****         gb->h = 0x00;
 985              		.loc 1 250 15
 986 089b 488B45F8 		movq	-8(%rbp), %rax
 987 089f C6400600 		movb	$0, 6(%rax)
 251:gameboy.c     ****         gb->l = 0x0D;
 988              		.loc 1 251 15
 989 08a3 488B45F8 		movq	-8(%rbp), %rax
 990 08a7 C640070D 		movb	$13, 7(%rax)
 252:gameboy.c     ****         gb->sp = 0xFFFE;
 991              		.loc 1 252 16
 992 08ab 488B45F8 		movq	-8(%rbp), %rax
 993 08af 66C74008 		movw	$-2, 8(%rax)
 993      FEFF
 253:gameboy.c     ****         gb->pc = 0x0100;
 994              		.loc 1 253 16
 995 08b5 488B45F8 		movq	-8(%rbp), %rax
 996 08b9 66C7400A 		movw	$256, 10(%rax)
 996      0001
 254:gameboy.c     ****     } else {
 255:gameboy.c     ****         gb->a = 0x01;
 256:gameboy.c     ****         gb->f = 0b10000000;
 257:gameboy.c     ****         gb->b = 0x00;
 258:gameboy.c     ****         gb->c = 0x13;
 259:gameboy.c     ****         gb->d = 0x00;
 260:gameboy.c     ****         gb->e = 0xD8;
 261:gameboy.c     ****         gb->h = 0x01;
 262:gameboy.c     ****         gb->l = 0x4D;
GAS LISTING /tmp/ccAJyxY4.s 			page 27


 263:gameboy.c     ****         gb->sp = 0xFFFE;
 264:gameboy.c     ****         gb->pc = 0x0100;
 265:gameboy.c     ****     }
 266:gameboy.c     **** }
 997              		.loc 1 266 1
 998 08bf EB53     		jmp	.L49
 999              	.L47:
 255:gameboy.c     ****         gb->f = 0b10000000;
 1000              		.loc 1 255 15
 1001 08c1 488B45F8 		movq	-8(%rbp), %rax
 1002 08c5 C60001   		movb	$1, (%rax)
 256:gameboy.c     ****         gb->b = 0x00;
 1003              		.loc 1 256 15
 1004 08c8 488B45F8 		movq	-8(%rbp), %rax
 1005 08cc C6400180 		movb	$-128, 1(%rax)
 257:gameboy.c     ****         gb->c = 0x13;
 1006              		.loc 1 257 15
 1007 08d0 488B45F8 		movq	-8(%rbp), %rax
 1008 08d4 C6400200 		movb	$0, 2(%rax)
 258:gameboy.c     ****         gb->d = 0x00;
 1009              		.loc 1 258 15
 1010 08d8 488B45F8 		movq	-8(%rbp), %rax
 1011 08dc C6400313 		movb	$19, 3(%rax)
 259:gameboy.c     ****         gb->e = 0xD8;
 1012              		.loc 1 259 15
 1013 08e0 488B45F8 		movq	-8(%rbp), %rax
 1014 08e4 C6400400 		movb	$0, 4(%rax)
 260:gameboy.c     ****         gb->h = 0x01;
 1015              		.loc 1 260 15
 1016 08e8 488B45F8 		movq	-8(%rbp), %rax
 1017 08ec C64005D8 		movb	$-40, 5(%rax)
 261:gameboy.c     ****         gb->l = 0x4D;
 1018              		.loc 1 261 15
 1019 08f0 488B45F8 		movq	-8(%rbp), %rax
 1020 08f4 C6400601 		movb	$1, 6(%rax)
 262:gameboy.c     ****         gb->sp = 0xFFFE;
 1021              		.loc 1 262 15
 1022 08f8 488B45F8 		movq	-8(%rbp), %rax
 1023 08fc C640074D 		movb	$77, 7(%rax)
 263:gameboy.c     ****         gb->pc = 0x0100;
 1024              		.loc 1 263 16
 1025 0900 488B45F8 		movq	-8(%rbp), %rax
 1026 0904 66C74008 		movw	$-2, 8(%rax)
 1026      FEFF
 264:gameboy.c     ****     }
 1027              		.loc 1 264 16
 1028 090a 488B45F8 		movq	-8(%rbp), %rax
 1029 090e 66C7400A 		movw	$256, 10(%rax)
 1029      0001
 1030              	.L49:
 1031              		.loc 1 266 1
 1032 0914 90       		nop
 1033 0915 5D       		popq	%rbp
 1034              		.cfi_def_cfa 7, 8
 1035 0916 C3       		ret
 1036              		.cfi_endproc
 1037              	.LFE13:
GAS LISTING /tmp/ccAJyxY4.s 			page 28


 1038              		.size	init_cpu, .-init_cpu
 1039              	.Letext0:
 1040              		.file 2 "/usr/lib/gcc/x86_64-linux-gnu/11/include/stddef.h"
 1041              		.file 3 "/usr/include/x86_64-linux-gnu/bits/types.h"
 1042              		.file 4 "/usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h"
 1043              		.file 5 "/usr/include/x86_64-linux-gnu/bits/types/FILE.h"
 1044              		.file 6 "/usr/include/x86_64-linux-gnu/bits/stdint-uintn.h"
 1045              		.file 7 "/usr/include/x86_64-linux-gnu/bits/types/time_t.h"
 1046              		.file 8 "gameboy.h"
 1047              		.file 9 "/usr/include/stdlib.h"
 1048              		.file 10 "/usr/include/string.h"
 1049              		.file 11 "/usr/include/stdio.h"
 1050              		.file 12 "/usr/include/time.h"
 1051              		.section	.debug_info,"",@progbits
 1052              	.Ldebug_info0:
 1053 0000 C4080000 		.long	0x8c4
 1054 0004 0500     		.value	0x5
 1055 0006 01       		.byte	0x1
 1056 0007 08       		.byte	0x8
 1057 0008 00000000 		.long	.Ldebug_abbrev0
 1058 000c 17       		.uleb128 0x17
 1059 000d 00000000 		.long	.LASF111
 1060 0011 1D       		.byte	0x1d
 1061 0012 00000000 		.long	.LASF0
 1062 0016 00000000 		.long	.LASF1
 1063 001a 00000000 		.quad	.Ltext0
 1063      00000000 
 1064 0022 17090000 		.quad	.Letext0-.Ltext0
 1064      00000000 
 1065 002a 00000000 		.long	.Ldebug_line0
 1066 002e 03       		.uleb128 0x3
 1067 002f 00000000 		.long	.LASF7
 1068 0033 02       		.byte	0x2
 1069 0034 D1       		.byte	0xd1
 1070 0035 17       		.byte	0x17
 1071 0036 3A000000 		.long	0x3a
 1072 003a 06       		.uleb128 0x6
 1073 003b 08       		.byte	0x8
 1074 003c 07       		.byte	0x7
 1075 003d 00000000 		.long	.LASF2
 1076 0041 06       		.uleb128 0x6
 1077 0042 04       		.byte	0x4
 1078 0043 07       		.byte	0x7
 1079 0044 00000000 		.long	.LASF3
 1080 0048 18       		.uleb128 0x18
 1081 0049 08       		.byte	0x8
 1082 004a 0D       		.uleb128 0xd
 1083 004b 48000000 		.long	0x48
 1084 004f 06       		.uleb128 0x6
 1085 0050 01       		.byte	0x1
 1086 0051 08       		.byte	0x8
 1087 0052 00000000 		.long	.LASF4
 1088 0056 06       		.uleb128 0x6
 1089 0057 02       		.byte	0x2
 1090 0058 07       		.byte	0x7
 1091 0059 00000000 		.long	.LASF5
 1092 005d 06       		.uleb128 0x6
GAS LISTING /tmp/ccAJyxY4.s 			page 29


 1093 005e 01       		.byte	0x1
 1094 005f 06       		.byte	0x6
 1095 0060 00000000 		.long	.LASF6
 1096 0064 03       		.uleb128 0x3
 1097 0065 00000000 		.long	.LASF8
 1098 0069 03       		.byte	0x3
 1099 006a 26       		.byte	0x26
 1100 006b 17       		.byte	0x17
 1101 006c 4F000000 		.long	0x4f
 1102 0070 06       		.uleb128 0x6
 1103 0071 02       		.byte	0x2
 1104 0072 05       		.byte	0x5
 1105 0073 00000000 		.long	.LASF9
 1106 0077 03       		.uleb128 0x3
 1107 0078 00000000 		.long	.LASF10
 1108 007c 03       		.byte	0x3
 1109 007d 28       		.byte	0x28
 1110 007e 1C       		.byte	0x1c
 1111 007f 56000000 		.long	0x56
 1112 0083 19       		.uleb128 0x19
 1113 0084 04       		.byte	0x4
 1114 0085 05       		.byte	0x5
 1115 0086 696E7400 		.string	"int"
 1116 008a 03       		.uleb128 0x3
 1117 008b 00000000 		.long	.LASF11
 1118 008f 03       		.byte	0x3
 1119 0090 2A       		.byte	0x2a
 1120 0091 16       		.byte	0x16
 1121 0092 41000000 		.long	0x41
 1122 0096 06       		.uleb128 0x6
 1123 0097 08       		.byte	0x8
 1124 0098 05       		.byte	0x5
 1125 0099 00000000 		.long	.LASF12
 1126 009d 03       		.uleb128 0x3
 1127 009e 00000000 		.long	.LASF13
 1128 00a2 03       		.byte	0x3
 1129 00a3 98       		.byte	0x98
 1130 00a4 19       		.byte	0x19
 1131 00a5 96000000 		.long	0x96
 1132 00a9 03       		.uleb128 0x3
 1133 00aa 00000000 		.long	.LASF14
 1134 00ae 03       		.byte	0x3
 1135 00af 99       		.byte	0x99
 1136 00b0 1B       		.byte	0x1b
 1137 00b1 96000000 		.long	0x96
 1138 00b5 03       		.uleb128 0x3
 1139 00b6 00000000 		.long	.LASF15
 1140 00ba 03       		.byte	0x3
 1141 00bb A0       		.byte	0xa0
 1142 00bc 1A       		.byte	0x1a
 1143 00bd 96000000 		.long	0x96
 1144 00c1 05       		.uleb128 0x5
 1145 00c2 C6000000 		.long	0xc6
 1146 00c6 06       		.uleb128 0x6
 1147 00c7 01       		.byte	0x1
 1148 00c8 06       		.byte	0x6
 1149 00c9 00000000 		.long	.LASF16
GAS LISTING /tmp/ccAJyxY4.s 			page 30


 1150 00cd 1A       		.uleb128 0x1a
 1151 00ce C6000000 		.long	0xc6
 1152 00d2 12       		.uleb128 0x12
 1153 00d3 00000000 		.long	.LASF60
 1154 00d7 D8       		.byte	0xd8
 1155 00d8 04       		.byte	0x4
 1156 00d9 31       		.byte	0x31
 1157 00da 08       		.byte	0x8
 1158 00db 59020000 		.long	0x259
 1159 00df 01       		.uleb128 0x1
 1160 00e0 00000000 		.long	.LASF17
 1161 00e4 04       		.byte	0x4
 1162 00e5 33       		.byte	0x33
 1163 00e6 07       		.byte	0x7
 1164 00e7 83000000 		.long	0x83
 1165 00eb 00       		.byte	0
 1166 00ec 01       		.uleb128 0x1
 1167 00ed 00000000 		.long	.LASF18
 1168 00f1 04       		.byte	0x4
 1169 00f2 36       		.byte	0x36
 1170 00f3 09       		.byte	0x9
 1171 00f4 C1000000 		.long	0xc1
 1172 00f8 08       		.byte	0x8
 1173 00f9 01       		.uleb128 0x1
 1174 00fa 00000000 		.long	.LASF19
 1175 00fe 04       		.byte	0x4
 1176 00ff 37       		.byte	0x37
 1177 0100 09       		.byte	0x9
 1178 0101 C1000000 		.long	0xc1
 1179 0105 10       		.byte	0x10
 1180 0106 01       		.uleb128 0x1
 1181 0107 00000000 		.long	.LASF20
 1182 010b 04       		.byte	0x4
 1183 010c 38       		.byte	0x38
 1184 010d 09       		.byte	0x9
 1185 010e C1000000 		.long	0xc1
 1186 0112 18       		.byte	0x18
 1187 0113 01       		.uleb128 0x1
 1188 0114 00000000 		.long	.LASF21
 1189 0118 04       		.byte	0x4
 1190 0119 39       		.byte	0x39
 1191 011a 09       		.byte	0x9
 1192 011b C1000000 		.long	0xc1
 1193 011f 20       		.byte	0x20
 1194 0120 01       		.uleb128 0x1
 1195 0121 00000000 		.long	.LASF22
 1196 0125 04       		.byte	0x4
 1197 0126 3A       		.byte	0x3a
 1198 0127 09       		.byte	0x9
 1199 0128 C1000000 		.long	0xc1
 1200 012c 28       		.byte	0x28
 1201 012d 01       		.uleb128 0x1
 1202 012e 00000000 		.long	.LASF23
 1203 0132 04       		.byte	0x4
 1204 0133 3B       		.byte	0x3b
 1205 0134 09       		.byte	0x9
 1206 0135 C1000000 		.long	0xc1
GAS LISTING /tmp/ccAJyxY4.s 			page 31


 1207 0139 30       		.byte	0x30
 1208 013a 01       		.uleb128 0x1
 1209 013b 00000000 		.long	.LASF24
 1210 013f 04       		.byte	0x4
 1211 0140 3C       		.byte	0x3c
 1212 0141 09       		.byte	0x9
 1213 0142 C1000000 		.long	0xc1
 1214 0146 38       		.byte	0x38
 1215 0147 01       		.uleb128 0x1
 1216 0148 00000000 		.long	.LASF25
 1217 014c 04       		.byte	0x4
 1218 014d 3D       		.byte	0x3d
 1219 014e 09       		.byte	0x9
 1220 014f C1000000 		.long	0xc1
 1221 0153 40       		.byte	0x40
 1222 0154 01       		.uleb128 0x1
 1223 0155 00000000 		.long	.LASF26
 1224 0159 04       		.byte	0x4
 1225 015a 40       		.byte	0x40
 1226 015b 09       		.byte	0x9
 1227 015c C1000000 		.long	0xc1
 1228 0160 48       		.byte	0x48
 1229 0161 01       		.uleb128 0x1
 1230 0162 00000000 		.long	.LASF27
 1231 0166 04       		.byte	0x4
 1232 0167 41       		.byte	0x41
 1233 0168 09       		.byte	0x9
 1234 0169 C1000000 		.long	0xc1
 1235 016d 50       		.byte	0x50
 1236 016e 01       		.uleb128 0x1
 1237 016f 00000000 		.long	.LASF28
 1238 0173 04       		.byte	0x4
 1239 0174 42       		.byte	0x42
 1240 0175 09       		.byte	0x9
 1241 0176 C1000000 		.long	0xc1
 1242 017a 58       		.byte	0x58
 1243 017b 01       		.uleb128 0x1
 1244 017c 00000000 		.long	.LASF29
 1245 0180 04       		.byte	0x4
 1246 0181 44       		.byte	0x44
 1247 0182 16       		.byte	0x16
 1248 0183 72020000 		.long	0x272
 1249 0187 60       		.byte	0x60
 1250 0188 01       		.uleb128 0x1
 1251 0189 00000000 		.long	.LASF30
 1252 018d 04       		.byte	0x4
 1253 018e 46       		.byte	0x46
 1254 018f 14       		.byte	0x14
 1255 0190 77020000 		.long	0x277
 1256 0194 68       		.byte	0x68
 1257 0195 01       		.uleb128 0x1
 1258 0196 00000000 		.long	.LASF31
 1259 019a 04       		.byte	0x4
 1260 019b 48       		.byte	0x48
 1261 019c 07       		.byte	0x7
 1262 019d 83000000 		.long	0x83
 1263 01a1 70       		.byte	0x70
GAS LISTING /tmp/ccAJyxY4.s 			page 32


 1264 01a2 01       		.uleb128 0x1
 1265 01a3 00000000 		.long	.LASF32
 1266 01a7 04       		.byte	0x4
 1267 01a8 49       		.byte	0x49
 1268 01a9 07       		.byte	0x7
 1269 01aa 83000000 		.long	0x83
 1270 01ae 74       		.byte	0x74
 1271 01af 01       		.uleb128 0x1
 1272 01b0 00000000 		.long	.LASF33
 1273 01b4 04       		.byte	0x4
 1274 01b5 4A       		.byte	0x4a
 1275 01b6 0B       		.byte	0xb
 1276 01b7 9D000000 		.long	0x9d
 1277 01bb 78       		.byte	0x78
 1278 01bc 01       		.uleb128 0x1
 1279 01bd 00000000 		.long	.LASF34
 1280 01c1 04       		.byte	0x4
 1281 01c2 4D       		.byte	0x4d
 1282 01c3 12       		.byte	0x12
 1283 01c4 56000000 		.long	0x56
 1284 01c8 80       		.byte	0x80
 1285 01c9 01       		.uleb128 0x1
 1286 01ca 00000000 		.long	.LASF35
 1287 01ce 04       		.byte	0x4
 1288 01cf 4E       		.byte	0x4e
 1289 01d0 0F       		.byte	0xf
 1290 01d1 5D000000 		.long	0x5d
 1291 01d5 82       		.byte	0x82
 1292 01d6 01       		.uleb128 0x1
 1293 01d7 00000000 		.long	.LASF36
 1294 01db 04       		.byte	0x4
 1295 01dc 4F       		.byte	0x4f
 1296 01dd 08       		.byte	0x8
 1297 01de 7C020000 		.long	0x27c
 1298 01e2 83       		.byte	0x83
 1299 01e3 01       		.uleb128 0x1
 1300 01e4 00000000 		.long	.LASF37
 1301 01e8 04       		.byte	0x4
 1302 01e9 51       		.byte	0x51
 1303 01ea 0F       		.byte	0xf
 1304 01eb 8C020000 		.long	0x28c
 1305 01ef 88       		.byte	0x88
 1306 01f0 01       		.uleb128 0x1
 1307 01f1 00000000 		.long	.LASF38
 1308 01f5 04       		.byte	0x4
 1309 01f6 59       		.byte	0x59
 1310 01f7 0D       		.byte	0xd
 1311 01f8 A9000000 		.long	0xa9
 1312 01fc 90       		.byte	0x90
 1313 01fd 01       		.uleb128 0x1
 1314 01fe 00000000 		.long	.LASF39
 1315 0202 04       		.byte	0x4
 1316 0203 5B       		.byte	0x5b
 1317 0204 17       		.byte	0x17
 1318 0205 96020000 		.long	0x296
 1319 0209 98       		.byte	0x98
 1320 020a 01       		.uleb128 0x1
GAS LISTING /tmp/ccAJyxY4.s 			page 33


 1321 020b 00000000 		.long	.LASF40
 1322 020f 04       		.byte	0x4
 1323 0210 5C       		.byte	0x5c
 1324 0211 19       		.byte	0x19
 1325 0212 A0020000 		.long	0x2a0
 1326 0216 A0       		.byte	0xa0
 1327 0217 01       		.uleb128 0x1
 1328 0218 00000000 		.long	.LASF41
 1329 021c 04       		.byte	0x4
 1330 021d 5D       		.byte	0x5d
 1331 021e 14       		.byte	0x14
 1332 021f 77020000 		.long	0x277
 1333 0223 A8       		.byte	0xa8
 1334 0224 01       		.uleb128 0x1
 1335 0225 00000000 		.long	.LASF42
 1336 0229 04       		.byte	0x4
 1337 022a 5E       		.byte	0x5e
 1338 022b 09       		.byte	0x9
 1339 022c 48000000 		.long	0x48
 1340 0230 B0       		.byte	0xb0
 1341 0231 01       		.uleb128 0x1
 1342 0232 00000000 		.long	.LASF43
 1343 0236 04       		.byte	0x4
 1344 0237 5F       		.byte	0x5f
 1345 0238 0A       		.byte	0xa
 1346 0239 2E000000 		.long	0x2e
 1347 023d B8       		.byte	0xb8
 1348 023e 01       		.uleb128 0x1
 1349 023f 00000000 		.long	.LASF44
 1350 0243 04       		.byte	0x4
 1351 0244 60       		.byte	0x60
 1352 0245 07       		.byte	0x7
 1353 0246 83000000 		.long	0x83
 1354 024a C0       		.byte	0xc0
 1355 024b 01       		.uleb128 0x1
 1356 024c 00000000 		.long	.LASF45
 1357 0250 04       		.byte	0x4
 1358 0251 62       		.byte	0x62
 1359 0252 08       		.byte	0x8
 1360 0253 A5020000 		.long	0x2a5
 1361 0257 C4       		.byte	0xc4
 1362 0258 00       		.byte	0
 1363 0259 03       		.uleb128 0x3
 1364 025a 00000000 		.long	.LASF46
 1365 025e 05       		.byte	0x5
 1366 025f 07       		.byte	0x7
 1367 0260 19       		.byte	0x19
 1368 0261 D2000000 		.long	0xd2
 1369 0265 1B       		.uleb128 0x1b
 1370 0266 00000000 		.long	.LASF112
 1371 026a 04       		.byte	0x4
 1372 026b 2B       		.byte	0x2b
 1373 026c 0E       		.byte	0xe
 1374 026d 0E       		.uleb128 0xe
 1375 026e 00000000 		.long	.LASF47
 1376 0272 05       		.uleb128 0x5
 1377 0273 6D020000 		.long	0x26d
GAS LISTING /tmp/ccAJyxY4.s 			page 34


 1378 0277 05       		.uleb128 0x5
 1379 0278 D2000000 		.long	0xd2
 1380 027c 0F       		.uleb128 0xf
 1381 027d C6000000 		.long	0xc6
 1382 0281 8C020000 		.long	0x28c
 1383 0285 13       		.uleb128 0x13
 1384 0286 3A000000 		.long	0x3a
 1385 028a 00       		.byte	0
 1386 028b 00       		.byte	0
 1387 028c 05       		.uleb128 0x5
 1388 028d 65020000 		.long	0x265
 1389 0291 0E       		.uleb128 0xe
 1390 0292 00000000 		.long	.LASF48
 1391 0296 05       		.uleb128 0x5
 1392 0297 91020000 		.long	0x291
 1393 029b 0E       		.uleb128 0xe
 1394 029c 00000000 		.long	.LASF49
 1395 02a0 05       		.uleb128 0x5
 1396 02a1 9B020000 		.long	0x29b
 1397 02a5 0F       		.uleb128 0xf
 1398 02a6 C6000000 		.long	0xc6
 1399 02aa B5020000 		.long	0x2b5
 1400 02ae 13       		.uleb128 0x13
 1401 02af 3A000000 		.long	0x3a
 1402 02b3 13       		.byte	0x13
 1403 02b4 00       		.byte	0
 1404 02b5 05       		.uleb128 0x5
 1405 02b6 59020000 		.long	0x259
 1406 02ba 0D       		.uleb128 0xd
 1407 02bb B5020000 		.long	0x2b5
 1408 02bf 03       		.uleb128 0x3
 1409 02c0 00000000 		.long	.LASF50
 1410 02c4 06       		.byte	0x6
 1411 02c5 18       		.byte	0x18
 1412 02c6 13       		.byte	0x13
 1413 02c7 64000000 		.long	0x64
 1414 02cb 03       		.uleb128 0x3
 1415 02cc 00000000 		.long	.LASF51
 1416 02d0 06       		.byte	0x6
 1417 02d1 19       		.byte	0x19
 1418 02d2 14       		.byte	0x14
 1419 02d3 77000000 		.long	0x77
 1420 02d7 03       		.uleb128 0x3
 1421 02d8 00000000 		.long	.LASF52
 1422 02dc 06       		.byte	0x6
 1423 02dd 1A       		.byte	0x1a
 1424 02de 14       		.byte	0x14
 1425 02df 8A000000 		.long	0x8a
 1426 02e3 06       		.uleb128 0x6
 1427 02e4 08       		.byte	0x8
 1428 02e5 05       		.byte	0x5
 1429 02e6 00000000 		.long	.LASF53
 1430 02ea 03       		.uleb128 0x3
 1431 02eb 00000000 		.long	.LASF54
 1432 02ef 07       		.byte	0x7
 1433 02f0 0A       		.byte	0xa
 1434 02f1 12       		.byte	0x12
GAS LISTING /tmp/ccAJyxY4.s 			page 35


 1435 02f2 B5000000 		.long	0xb5
 1436 02f6 06       		.uleb128 0x6
 1437 02f7 08       		.byte	0x8
 1438 02f8 07       		.byte	0x7
 1439 02f9 00000000 		.long	.LASF55
 1440 02fd 05       		.uleb128 0x5
 1441 02fe CD000000 		.long	0xcd
 1442 0302 0D       		.uleb128 0xd
 1443 0303 FD020000 		.long	0x2fd
 1444 0307 03       		.uleb128 0x3
 1445 0308 00000000 		.long	.LASF56
 1446 030c 08       		.byte	0x8
 1447 030d 08       		.byte	0x8
 1448 030e 11       		.byte	0x11
 1449 030f BF020000 		.long	0x2bf
 1450 0313 03       		.uleb128 0x3
 1451 0314 00000000 		.long	.LASF57
 1452 0318 08       		.byte	0x8
 1453 0319 09       		.byte	0x9
 1454 031a 12       		.byte	0x12
 1455 031b CB020000 		.long	0x2cb
 1456 031f 03       		.uleb128 0x3
 1457 0320 00000000 		.long	.LASF58
 1458 0324 08       		.byte	0x8
 1459 0325 0A       		.byte	0xa
 1460 0326 12       		.byte	0x12
 1461 0327 2B030000 		.long	0x32b
 1462 032b 05       		.uleb128 0x5
 1463 032c BF020000 		.long	0x2bf
 1464 0330 03       		.uleb128 0x3
 1465 0331 00000000 		.long	.LASF59
 1466 0335 08       		.byte	0x8
 1467 0336 0C       		.byte	0xc
 1468 0337 11       		.byte	0x11
 1469 0338 BF020000 		.long	0x2bf
 1470 033c 12       		.uleb128 0x12
 1471 033d 00000000 		.long	.LASF61
 1472 0341 58       		.byte	0x58
 1473 0342 08       		.byte	0x8
 1474 0343 0E       		.byte	0xe
 1475 0344 10       		.byte	0x10
 1476 0345 61040000 		.long	0x461
 1477 0349 04       		.uleb128 0x4
 1478 034a 6100     		.string	"a"
 1479 034c 0F       		.byte	0xf
 1480 034d 0E       		.byte	0xe
 1481 034e 07030000 		.long	0x307
 1482 0352 00       		.byte	0
 1483 0353 04       		.uleb128 0x4
 1484 0354 6600     		.string	"f"
 1485 0356 10       		.byte	0x10
 1486 0357 0E       		.byte	0xe
 1487 0358 07030000 		.long	0x307
 1488 035c 01       		.byte	0x1
 1489 035d 04       		.uleb128 0x4
 1490 035e 6200     		.string	"b"
 1491 0360 11       		.byte	0x11
GAS LISTING /tmp/ccAJyxY4.s 			page 36


 1492 0361 0E       		.byte	0xe
 1493 0362 07030000 		.long	0x307
 1494 0366 02       		.byte	0x2
 1495 0367 04       		.uleb128 0x4
 1496 0368 6300     		.string	"c"
 1497 036a 12       		.byte	0x12
 1498 036b 0E       		.byte	0xe
 1499 036c 07030000 		.long	0x307
 1500 0370 03       		.byte	0x3
 1501 0371 04       		.uleb128 0x4
 1502 0372 6400     		.string	"d"
 1503 0374 13       		.byte	0x13
 1504 0375 0E       		.byte	0xe
 1505 0376 07030000 		.long	0x307
 1506 037a 04       		.byte	0x4
 1507 037b 04       		.uleb128 0x4
 1508 037c 6500     		.string	"e"
 1509 037e 14       		.byte	0x14
 1510 037f 0E       		.byte	0xe
 1511 0380 07030000 		.long	0x307
 1512 0384 05       		.byte	0x5
 1513 0385 04       		.uleb128 0x4
 1514 0386 6800     		.string	"h"
 1515 0388 15       		.byte	0x15
 1516 0389 0E       		.byte	0xe
 1517 038a 07030000 		.long	0x307
 1518 038e 06       		.byte	0x6
 1519 038f 04       		.uleb128 0x4
 1520 0390 6C00     		.string	"l"
 1521 0392 16       		.byte	0x16
 1522 0393 0E       		.byte	0xe
 1523 0394 07030000 		.long	0x307
 1524 0398 07       		.byte	0x7
 1525 0399 04       		.uleb128 0x4
 1526 039a 737000   		.string	"sp"
 1527 039d 17       		.byte	0x17
 1528 039e 10       		.byte	0x10
 1529 039f 13030000 		.long	0x313
 1530 03a3 08       		.byte	0x8
 1531 03a4 04       		.uleb128 0x4
 1532 03a5 706300   		.string	"pc"
 1533 03a8 18       		.byte	0x18
 1534 03a9 10       		.byte	0x10
 1535 03aa 13030000 		.long	0x313
 1536 03ae 0A       		.byte	0xa
 1537 03af 04       		.uleb128 0x4
 1538 03b0 696D6500 		.string	"ime"
 1539 03b4 1A       		.byte	0x1a
 1540 03b5 0E       		.byte	0xe
 1541 03b6 07030000 		.long	0x307
 1542 03ba 0C       		.byte	0xc
 1543 03bb 04       		.uleb128 0x4
 1544 03bc 726F6D00 		.string	"rom"
 1545 03c0 1C       		.byte	0x1c
 1546 03c1 0C       		.byte	0xc
 1547 03c2 1F030000 		.long	0x31f
 1548 03c6 10       		.byte	0x10
GAS LISTING /tmp/ccAJyxY4.s 			page 37


 1549 03c7 01       		.uleb128 0x1
 1550 03c8 00000000 		.long	.LASF62
 1551 03cc 08       		.byte	0x8
 1552 03cd 1D       		.byte	0x1d
 1553 03ce 0C       		.byte	0xc
 1554 03cf 1F030000 		.long	0x31f
 1555 03d3 18       		.byte	0x18
 1556 03d4 01       		.uleb128 0x1
 1557 03d5 00000000 		.long	.LASF63
 1558 03d9 08       		.byte	0x8
 1559 03da 1E       		.byte	0x1e
 1560 03db 0C       		.byte	0xc
 1561 03dc 1F030000 		.long	0x31f
 1562 03e0 20       		.byte	0x20
 1563 03e1 01       		.uleb128 0x1
 1564 03e2 00000000 		.long	.LASF64
 1565 03e6 08       		.byte	0x8
 1566 03e7 1F       		.byte	0x1f
 1567 03e8 0C       		.byte	0xc
 1568 03e9 1F030000 		.long	0x31f
 1569 03ed 28       		.byte	0x28
 1570 03ee 04       		.uleb128 0x4
 1571 03ef 6F616D00 		.string	"oam"
 1572 03f3 20       		.byte	0x20
 1573 03f4 0C       		.byte	0xc
 1574 03f5 1F030000 		.long	0x31f
 1575 03f9 30       		.byte	0x30
 1576 03fa 04       		.uleb128 0x4
 1577 03fb 696F00   		.string	"io"
 1578 03fe 21       		.byte	0x21
 1579 03ff 0C       		.byte	0xc
 1580 0400 1F030000 		.long	0x31f
 1581 0404 38       		.byte	0x38
 1582 0405 01       		.uleb128 0x1
 1583 0406 00000000 		.long	.LASF65
 1584 040a 08       		.byte	0x8
 1585 040b 22       		.byte	0x22
 1586 040c 0C       		.byte	0xc
 1587 040d 1F030000 		.long	0x31f
 1588 0411 40       		.byte	0x40
 1589 0412 01       		.uleb128 0x1
 1590 0413 00000000 		.long	.LASF66
 1591 0417 08       		.byte	0x8
 1592 0418 24       		.byte	0x24
 1593 0419 0D       		.byte	0xd
 1594 041a 61040000 		.long	0x461
 1595 041e 48       		.byte	0x48
 1596 041f 01       		.uleb128 0x1
 1597 0420 00000000 		.long	.LASF67
 1598 0424 08       		.byte	0x8
 1599 0425 26       		.byte	0x26
 1600 0426 0A       		.byte	0xa
 1601 0427 30030000 		.long	0x330
 1602 042b 50       		.byte	0x50
 1603 042c 01       		.uleb128 0x1
 1604 042d 00000000 		.long	.LASF68
 1605 0431 08       		.byte	0x8
GAS LISTING /tmp/ccAJyxY4.s 			page 38


 1606 0432 27       		.byte	0x27
 1607 0433 0A       		.byte	0xa
 1608 0434 30030000 		.long	0x330
 1609 0438 51       		.byte	0x51
 1610 0439 01       		.uleb128 0x1
 1611 043a 00000000 		.long	.LASF69
 1612 043e 08       		.byte	0x8
 1613 043f 28       		.byte	0x28
 1614 0440 0A       		.byte	0xa
 1615 0441 30030000 		.long	0x330
 1616 0445 52       		.byte	0x52
 1617 0446 01       		.uleb128 0x1
 1618 0447 00000000 		.long	.LASF70
 1619 044b 08       		.byte	0x8
 1620 044c 29       		.byte	0x29
 1621 044d 0A       		.byte	0xa
 1622 044e 30030000 		.long	0x330
 1623 0452 53       		.byte	0x53
 1624 0453 01       		.uleb128 0x1
 1625 0454 00000000 		.long	.LASF71
 1626 0458 08       		.byte	0x8
 1627 0459 2B       		.byte	0x2b
 1628 045a 0A       		.byte	0xa
 1629 045b 66040000 		.long	0x466
 1630 045f 54       		.byte	0x54
 1631 0460 00       		.byte	0
 1632 0461 05       		.uleb128 0x5
 1633 0462 1F030000 		.long	0x31f
 1634 0466 06       		.uleb128 0x6
 1635 0467 01       		.byte	0x1
 1636 0468 02       		.byte	0x2
 1637 0469 00000000 		.long	.LASF72
 1638 046d 03       		.uleb128 0x3
 1639 046e 00000000 		.long	.LASF73
 1640 0472 08       		.byte	0x8
 1641 0473 2C       		.byte	0x2c
 1642 0474 03       		.byte	0x3
 1643 0475 3C030000 		.long	0x33c
 1644 0479 05       		.uleb128 0x5
 1645 047a 6D040000 		.long	0x46d
 1646 047e 1C       		.uleb128 0x1c
 1647 047f 00000000 		.long	.LASF74
 1648 0483 09       		.byte	0x9
 1649 0484 7002     		.value	0x270
 1650 0486 0D       		.byte	0xd
 1651 0487 91040000 		.long	0x491
 1652 048b 02       		.uleb128 0x2
 1653 048c 83000000 		.long	0x83
 1654 0490 00       		.byte	0
 1655 0491 08       		.uleb128 0x8
 1656 0492 00000000 		.long	.LASF75
 1657 0496 0A       		.byte	0xa
 1658 0497 9701     		.value	0x197
 1659 0499 0F       		.byte	0xf
 1660 049a 2E000000 		.long	0x2e
 1661 049e A8040000 		.long	0x4a8
 1662 04a2 02       		.uleb128 0x2
GAS LISTING /tmp/ccAJyxY4.s 			page 39


 1663 04a3 FD020000 		.long	0x2fd
 1664 04a7 00       		.byte	0
 1665 04a8 08       		.uleb128 0x8
 1666 04a9 00000000 		.long	.LASF76
 1667 04ad 0B       		.byte	0xb
 1668 04ae 6601     		.value	0x166
 1669 04b0 0C       		.byte	0xc
 1670 04b1 83000000 		.long	0x83
 1671 04b5 C5040000 		.long	0x4c5
 1672 04b9 02       		.uleb128 0x2
 1673 04ba C1000000 		.long	0xc1
 1674 04be 02       		.uleb128 0x2
 1675 04bf FD020000 		.long	0x2fd
 1676 04c3 14       		.uleb128 0x14
 1677 04c4 00       		.byte	0
 1678 04c5 10       		.uleb128 0x10
 1679 04c6 00000000 		.long	.LASF79
 1680 04ca 09       		.byte	0x9
 1681 04cb C801     		.value	0x1c8
 1682 04cd D7040000 		.long	0x4d7
 1683 04d1 02       		.uleb128 0x2
 1684 04d2 41000000 		.long	0x41
 1685 04d6 00       		.byte	0
 1686 04d7 15       		.uleb128 0x15
 1687 04d8 00000000 		.long	.LASF77
 1688 04dc 0C       		.byte	0xc
 1689 04dd 4C       		.byte	0x4c
 1690 04de 0F       		.byte	0xf
 1691 04df EA020000 		.long	0x2ea
 1692 04e3 ED040000 		.long	0x4ed
 1693 04e7 02       		.uleb128 0x2
 1694 04e8 ED040000 		.long	0x4ed
 1695 04ec 00       		.byte	0
 1696 04ed 05       		.uleb128 0x5
 1697 04ee EA020000 		.long	0x2ea
 1698 04f2 1D       		.uleb128 0x1d
 1699 04f3 00000000 		.long	.LASF113
 1700 04f7 09       		.byte	0x9
 1701 04f8 C601     		.value	0x1c6
 1702 04fa 0C       		.byte	0xc
 1703 04fb 83000000 		.long	0x83
 1704 04ff 08       		.uleb128 0x8
 1705 0500 00000000 		.long	.LASF78
 1706 0504 09       		.byte	0x9
 1707 0505 1F02     		.value	0x21f
 1708 0507 0E       		.byte	0xe
 1709 0508 48000000 		.long	0x48
 1710 050c 1B050000 		.long	0x51b
 1711 0510 02       		.uleb128 0x2
 1712 0511 2E000000 		.long	0x2e
 1713 0515 02       		.uleb128 0x2
 1714 0516 2E000000 		.long	0x2e
 1715 051a 00       		.byte	0
 1716 051b 10       		.uleb128 0x10
 1717 051c 00000000 		.long	.LASF80
 1718 0520 09       		.byte	0x9
 1719 0521 2B02     		.value	0x22b
GAS LISTING /tmp/ccAJyxY4.s 			page 40


 1720 0523 2D050000 		.long	0x52d
 1721 0527 02       		.uleb128 0x2
 1722 0528 48000000 		.long	0x48
 1723 052c 00       		.byte	0
 1724 052d 08       		.uleb128 0x8
 1725 052e 00000000 		.long	.LASF81
 1726 0532 0B       		.byte	0xb
 1727 0533 A302     		.value	0x2a3
 1728 0535 0F       		.byte	0xf
 1729 0536 2E000000 		.long	0x2e
 1730 053a 53050000 		.long	0x553
 1731 053e 02       		.uleb128 0x2
 1732 053f 4A000000 		.long	0x4a
 1733 0543 02       		.uleb128 0x2
 1734 0544 2E000000 		.long	0x2e
 1735 0548 02       		.uleb128 0x2
 1736 0549 2E000000 		.long	0x2e
 1737 054d 02       		.uleb128 0x2
 1738 054e BA020000 		.long	0x2ba
 1739 0552 00       		.byte	0
 1740 0553 08       		.uleb128 0x8
 1741 0554 00000000 		.long	.LASF82
 1742 0558 0B       		.byte	0xb
 1743 0559 6401     		.value	0x164
 1744 055b 0C       		.byte	0xc
 1745 055c 83000000 		.long	0x83
 1746 0560 6B050000 		.long	0x56b
 1747 0564 02       		.uleb128 0x2
 1748 0565 FD020000 		.long	0x2fd
 1749 0569 14       		.uleb128 0x14
 1750 056a 00       		.byte	0
 1751 056b 15       		.uleb128 0x15
 1752 056c 00000000 		.long	.LASF83
 1753 0570 0B       		.byte	0xb
 1754 0571 B2       		.byte	0xb2
 1755 0572 0C       		.byte	0xc
 1756 0573 83000000 		.long	0x83
 1757 0577 81050000 		.long	0x581
 1758 057b 02       		.uleb128 0x2
 1759 057c B5020000 		.long	0x2b5
 1760 0580 00       		.byte	0
 1761 0581 10       		.uleb128 0x10
 1762 0582 00000000 		.long	.LASF84
 1763 0586 0B       		.byte	0xb
 1764 0587 D302     		.value	0x2d3
 1765 0589 93050000 		.long	0x593
 1766 058d 02       		.uleb128 0x2
 1767 058e B5020000 		.long	0x2b5
 1768 0592 00       		.byte	0
 1769 0593 08       		.uleb128 0x8
 1770 0594 00000000 		.long	.LASF85
 1771 0598 0B       		.byte	0xb
 1772 0599 CE02     		.value	0x2ce
 1773 059b 11       		.byte	0x11
 1774 059c 96000000 		.long	0x96
 1775 05a0 AA050000 		.long	0x5aa
 1776 05a4 02       		.uleb128 0x2
GAS LISTING /tmp/ccAJyxY4.s 			page 41


 1777 05a5 B5020000 		.long	0x2b5
 1778 05a9 00       		.byte	0
 1779 05aa 08       		.uleb128 0x8
 1780 05ab 00000000 		.long	.LASF86
 1781 05af 0B       		.byte	0xb
 1782 05b0 C902     		.value	0x2c9
 1783 05b2 0C       		.byte	0xc
 1784 05b3 83000000 		.long	0x83
 1785 05b7 CB050000 		.long	0x5cb
 1786 05bb 02       		.uleb128 0x2
 1787 05bc B5020000 		.long	0x2b5
 1788 05c0 02       		.uleb128 0x2
 1789 05c1 96000000 		.long	0x96
 1790 05c5 02       		.uleb128 0x2
 1791 05c6 83000000 		.long	0x83
 1792 05ca 00       		.byte	0
 1793 05cb 08       		.uleb128 0x8
 1794 05cc 00000000 		.long	.LASF87
 1795 05d0 0B       		.byte	0xb
 1796 05d1 0201     		.value	0x102
 1797 05d3 0E       		.byte	0xe
 1798 05d4 B5020000 		.long	0x2b5
 1799 05d8 E7050000 		.long	0x5e7
 1800 05dc 02       		.uleb128 0x2
 1801 05dd 02030000 		.long	0x302
 1802 05e1 02       		.uleb128 0x2
 1803 05e2 02030000 		.long	0x302
 1804 05e6 00       		.byte	0
 1805 05e7 1E       		.uleb128 0x1e
 1806 05e8 00000000 		.long	.LASF88
 1807 05ec 01       		.byte	0x1
 1808 05ed F2       		.byte	0xf2
 1809 05ee 06       		.byte	0x6
 1810 05ef 00000000 		.quad	.LFB13
 1810      00000000 
 1811 05f7 C3000000 		.quad	.LFE13-.LFB13
 1811      00000000 
 1812 05ff 01       		.uleb128 0x1
 1813 0600 9C       		.byte	0x9c
 1814 0601 13060000 		.long	0x613
 1815 0605 09       		.uleb128 0x9
 1816 0606 676200   		.string	"gb"
 1817 0609 F2       		.byte	0xf2
 1818 060a 18       		.byte	0x18
 1819 060b 79040000 		.long	0x479
 1820 060f 02       		.uleb128 0x2
 1821 0610 91       		.byte	0x91
 1822 0611 68       		.sleb128 -24
 1823 0612 00       		.byte	0
 1824 0613 0C       		.uleb128 0xc
 1825 0614 00000000 		.long	.LASF89
 1826 0618 DB       		.byte	0xdb
 1827 0619 00000000 		.quad	.LFB12
 1827      00000000 
 1828 0621 8E000000 		.quad	.LFE12-.LFB12
 1828      00000000 
 1829 0629 01       		.uleb128 0x1
GAS LISTING /tmp/ccAJyxY4.s 			page 42


 1830 062a 9C       		.byte	0x9c
 1831 062b 3D060000 		.long	0x63d
 1832 062f 09       		.uleb128 0x9
 1833 0630 676200   		.string	"gb"
 1834 0633 DB       		.byte	0xdb
 1835 0634 1C       		.byte	0x1c
 1836 0635 79040000 		.long	0x479
 1837 0639 02       		.uleb128 0x2
 1838 063a 91       		.byte	0x91
 1839 063b 68       		.sleb128 -24
 1840 063c 00       		.byte	0
 1841 063d 0C       		.uleb128 0xc
 1842 063e 00000000 		.long	.LASF90
 1843 0642 96       		.byte	0x96
 1844 0643 00000000 		.quad	.LFB11
 1844      00000000 
 1845 064b 50020000 		.quad	.LFE11-.LFB11
 1845      00000000 
 1846 0653 01       		.uleb128 0x1
 1847 0654 9C       		.byte	0x9c
 1848 0655 83060000 		.long	0x683
 1849 0659 09       		.uleb128 0x9
 1850 065a 676200   		.string	"gb"
 1851 065d 96       		.byte	0x96
 1852 065e 1E       		.byte	0x1e
 1853 065f 79040000 		.long	0x479
 1854 0663 02       		.uleb128 0x2
 1855 0664 91       		.byte	0x91
 1856 0665 68       		.sleb128 -24
 1857 0666 0A       		.uleb128 0xa
 1858 0667 00000000 		.long	.LASF91
 1859 066b 96       		.byte	0x96
 1860 066c 2B       		.byte	0x2b
 1861 066d CB020000 		.long	0x2cb
 1862 0671 02       		.uleb128 0x2
 1863 0672 91       		.byte	0x91
 1864 0673 64       		.sleb128 -28
 1865 0674 09       		.uleb128 0x9
 1866 0675 76616C00 		.string	"val"
 1867 0679 96       		.byte	0x96
 1868 067a 39       		.byte	0x39
 1869 067b BF020000 		.long	0x2bf
 1870 067f 02       		.uleb128 0x2
 1871 0680 91       		.byte	0x91
 1872 0681 60       		.sleb128 -32
 1873 0682 00       		.byte	0
 1874 0683 16       		.uleb128 0x16
 1875 0684 00000000 		.long	.LASF102
 1876 0688 80       		.byte	0x80
 1877 0689 07       		.byte	0x7
 1878 068a C1000000 		.long	0xc1
 1879 068e 00000000 		.quad	.LFB10
 1879      00000000 
 1880 0696 B8000000 		.quad	.LFE10-.LFB10
 1880      00000000 
 1881 069e 01       		.uleb128 0x1
 1882 069f 9C       		.byte	0x9c
GAS LISTING /tmp/ccAJyxY4.s 			page 43


 1883 06a0 DF060000 		.long	0x6df
 1884 06a4 09       		.uleb128 0x9
 1885 06a5 72656700 		.string	"reg"
 1886 06a9 80       		.byte	0x80
 1887 06aa 20       		.byte	0x20
 1888 06ab BF020000 		.long	0x2bf
 1889 06af 02       		.uleb128 0x2
 1890 06b0 91       		.byte	0x91
 1891 06b1 5C       		.sleb128 -36
 1892 06b2 07       		.uleb128 0x7
 1893 06b3 00000000 		.long	.LASF92
 1894 06b7 82       		.byte	0x82
 1895 06b8 0B       		.byte	0xb
 1896 06b9 C1000000 		.long	0xc1
 1897 06bd 02       		.uleb128 0x2
 1898 06be 91       		.byte	0x91
 1899 06bf 68       		.sleb128 -24
 1900 06c0 11       		.uleb128 0x11
 1901 06c1 00000000 		.quad	.LBB4
 1901      00000000 
 1902 06c9 66000000 		.quad	.LBE4-.LBB4
 1902      00000000 
 1903 06d1 0B       		.uleb128 0xb
 1904 06d2 6900     		.string	"i"
 1905 06d4 89       		.byte	0x89
 1906 06d5 0E       		.byte	0xe
 1907 06d6 83000000 		.long	0x83
 1908 06da 02       		.uleb128 0x2
 1909 06db 91       		.byte	0x91
 1910 06dc 64       		.sleb128 -28
 1911 06dd 00       		.byte	0
 1912 06de 00       		.byte	0
 1913 06df 0C       		.uleb128 0xc
 1914 06e0 00000000 		.long	.LASF93
 1915 06e4 5F       		.byte	0x5f
 1916 06e5 00000000 		.quad	.LFB9
 1916      00000000 
 1917 06ed F7010000 		.quad	.LFE9-.LFB9
 1917      00000000 
 1918 06f5 01       		.uleb128 0x1
 1919 06f6 9C       		.byte	0x9c
 1920 06f7 55070000 		.long	0x755
 1921 06fb 09       		.uleb128 0x9
 1922 06fc 676200   		.string	"gb"
 1923 06ff 5F       		.byte	0x5f
 1924 0700 1E       		.byte	0x1e
 1925 0701 79040000 		.long	0x479
 1926 0705 03       		.uleb128 0x3
 1927 0706 91       		.byte	0x91
 1928 0707 9877     		.sleb128 -1128
 1929 0709 07       		.uleb128 0x7
 1930 070a 00000000 		.long	.LASF92
 1931 070e 62       		.byte	0x62
 1932 070f 0A       		.byte	0xa
 1933 0710 55070000 		.long	0x755
 1934 0714 03       		.uleb128 0x3
 1935 0715 91       		.byte	0x91
GAS LISTING /tmp/ccAJyxY4.s 			page 44


 1936 0716 C077     		.sleb128 -1088
 1937 0718 07       		.uleb128 0x7
 1938 0719 00000000 		.long	.LASF94
 1939 071d 63       		.byte	0x63
 1940 071e 0B       		.byte	0xb
 1941 071f C1000000 		.long	0xc1
 1942 0723 03       		.uleb128 0x3
 1943 0724 91       		.byte	0x91
 1944 0725 B077     		.sleb128 -1104
 1945 0727 0B       		.uleb128 0xb
 1946 0728 706300   		.string	"pc"
 1947 072b 76       		.byte	0x76
 1948 072c 11       		.byte	0x11
 1949 072d 66070000 		.long	0x766
 1950 0731 03       		.uleb128 0x3
 1951 0732 91       		.byte	0x91
 1952 0733 B877     		.sleb128 -1096
 1953 0735 11       		.uleb128 0x11
 1954 0736 00000000 		.quad	.LBB3
 1954      00000000 
 1955 073e 79000000 		.quad	.LBE3-.LBB3
 1955      00000000 
 1956 0746 0B       		.uleb128 0xb
 1957 0747 6900     		.string	"i"
 1958 0749 77       		.byte	0x77
 1959 074a 0E       		.byte	0xe
 1960 074b 83000000 		.long	0x83
 1961 074f 03       		.uleb128 0x3
 1962 0750 91       		.byte	0x91
 1963 0751 AC77     		.sleb128 -1108
 1964 0753 00       		.byte	0
 1965 0754 00       		.byte	0
 1966 0755 0F       		.uleb128 0xf
 1967 0756 C6000000 		.long	0xc6
 1968 075a 66070000 		.long	0x766
 1969 075e 1F       		.uleb128 0x1f
 1970 075f 3A000000 		.long	0x3a
 1971 0763 FF03     		.value	0x3ff
 1972 0765 00       		.byte	0
 1973 0766 05       		.uleb128 0x5
 1974 0767 13030000 		.long	0x313
 1975 076b 0C       		.uleb128 0xc
 1976 076c 00000000 		.long	.LASF95
 1977 0770 47       		.byte	0x47
 1978 0771 00000000 		.quad	.LFB8
 1978      00000000 
 1979 0779 DA000000 		.quad	.LFE8-.LFB8
 1979      00000000 
 1980 0781 01       		.uleb128 0x1
 1981 0782 9C       		.byte	0x9c
 1982 0783 E9070000 		.long	0x7e9
 1983 0787 09       		.uleb128 0x9
 1984 0788 676200   		.string	"gb"
 1985 078b 47       		.byte	0x47
 1986 078c 1C       		.byte	0x1c
 1987 078d 79040000 		.long	0x479
 1988 0791 02       		.uleb128 0x2
GAS LISTING /tmp/ccAJyxY4.s 			page 45


 1989 0792 91       		.byte	0x91
 1990 0793 48       		.sleb128 -56
 1991 0794 0A       		.uleb128 0xa
 1992 0795 00000000 		.long	.LASF96
 1993 0799 47       		.byte	0x47
 1994 079a 2C       		.byte	0x2c
 1995 079b FD020000 		.long	0x2fd
 1996 079f 02       		.uleb128 0x2
 1997 07a0 91       		.byte	0x91
 1998 07a1 40       		.sleb128 -64
 1999 07a2 07       		.uleb128 0x7
 2000 07a3 00000000 		.long	.LASF97
 2001 07a7 4D       		.byte	0x4d
 2002 07a8 0E       		.byte	0xe
 2003 07a9 D7020000 		.long	0x2d7
 2004 07ad 02       		.uleb128 0x2
 2005 07ae 91       		.byte	0x91
 2006 07af 5C       		.sleb128 -36
 2007 07b0 07       		.uleb128 0x7
 2008 07b1 00000000 		.long	.LASF98
 2009 07b5 4E       		.byte	0x4e
 2010 07b6 0E       		.byte	0xe
 2011 07b7 D7020000 		.long	0x2d7
 2012 07bb 02       		.uleb128 0x2
 2013 07bc 91       		.byte	0x91
 2014 07bd 60       		.sleb128 -32
 2015 07be 07       		.uleb128 0x7
 2016 07bf 00000000 		.long	.LASF99
 2017 07c3 4F       		.byte	0x4f
 2018 07c4 0E       		.byte	0xe
 2019 07c5 D7020000 		.long	0x2d7
 2020 07c9 02       		.uleb128 0x2
 2021 07ca 91       		.byte	0x91
 2022 07cb 64       		.sleb128 -28
 2023 07cc 07       		.uleb128 0x7
 2024 07cd 00000000 		.long	.LASF100
 2025 07d1 50       		.byte	0x50
 2026 07d2 0E       		.byte	0xe
 2027 07d3 D7020000 		.long	0x2d7
 2028 07d7 02       		.uleb128 0x2
 2029 07d8 91       		.byte	0x91
 2030 07d9 68       		.sleb128 -24
 2031 07da 07       		.uleb128 0x7
 2032 07db 00000000 		.long	.LASF101
 2033 07df 51       		.byte	0x51
 2034 07e0 0E       		.byte	0xe
 2035 07e1 D7020000 		.long	0x2d7
 2036 07e5 02       		.uleb128 0x2
 2037 07e6 91       		.byte	0x91
 2038 07e7 6C       		.sleb128 -20
 2039 07e8 00       		.byte	0
 2040 07e9 16       		.uleb128 0x16
 2041 07ea 00000000 		.long	.LASF103
 2042 07ee 39       		.byte	0x39
 2043 07ef 0A       		.byte	0xa
 2044 07f0 2B030000 		.long	0x32b
 2045 07f4 00000000 		.quad	.LFB7
GAS LISTING /tmp/ccAJyxY4.s 			page 46


 2045      00000000 
 2046 07fc 6C000000 		.quad	.LFE7-.LFB7
 2046      00000000 
 2047 0804 01       		.uleb128 0x1
 2048 0805 9C       		.byte	0x9c
 2049 0806 53080000 		.long	0x853
 2050 080a 0A       		.uleb128 0xa
 2051 080b 00000000 		.long	.LASF104
 2052 080f 39       		.byte	0x39
 2053 0810 22       		.byte	0x22
 2054 0811 D7020000 		.long	0x2d7
 2055 0815 02       		.uleb128 0x2
 2056 0816 91       		.byte	0x91
 2057 0817 5C       		.sleb128 -36
 2058 0818 0A       		.uleb128 0xa
 2059 0819 00000000 		.long	.LASF105
 2060 081d 39       		.byte	0x39
 2061 081e 2D       		.byte	0x2d
 2062 081f 66040000 		.long	0x466
 2063 0823 02       		.uleb128 0x2
 2064 0824 91       		.byte	0x91
 2065 0825 58       		.sleb128 -40
 2066 0826 0B       		.uleb128 0xb
 2067 0827 70747200 		.string	"ptr"
 2068 082b 3A       		.byte	0x3a
 2069 082c 0E       		.byte	0xe
 2070 082d 2B030000 		.long	0x32b
 2071 0831 02       		.uleb128 0x2
 2072 0832 91       		.byte	0x91
 2073 0833 68       		.sleb128 -24
 2074 0834 11       		.uleb128 0x11
 2075 0835 00000000 		.quad	.LBB2
 2075      00000000 
 2076 083d 38000000 		.quad	.LBE2-.LBB2
 2076      00000000 
 2077 0845 0B       		.uleb128 0xb
 2078 0846 6900     		.string	"i"
 2079 0848 3E       		.byte	0x3e
 2080 0849 12       		.byte	0x12
 2081 084a 83000000 		.long	0x83
 2082 084e 02       		.uleb128 0x2
 2083 084f 91       		.byte	0x91
 2084 0850 64       		.sleb128 -28
 2085 0851 00       		.byte	0
 2086 0852 00       		.byte	0
 2087 0853 20       		.uleb128 0x20
 2088 0854 00000000 		.long	.LASF106
 2089 0858 01       		.byte	0x1
 2090 0859 0D       		.byte	0xd
 2091 085a 06       		.byte	0x6
 2092 085b 96000000 		.long	0x96
 2093 085f 00000000 		.quad	.LFB6
 2093      00000000 
 2094 0867 81010000 		.quad	.LFE6-.LFB6
 2094      00000000 
 2095 086f 01       		.uleb128 0x1
 2096 0870 9C       		.byte	0x9c
GAS LISTING /tmp/ccAJyxY4.s 			page 47


 2097 0871 0A       		.uleb128 0xa
 2098 0872 00000000 		.long	.LASF96
 2099 0876 0D       		.byte	0xd
 2100 0877 28       		.byte	0x28
 2101 0878 FD020000 		.long	0x2fd
 2102 087c 02       		.uleb128 0x2
 2103 087d 91       		.byte	0x91
 2104 087e 48       		.sleb128 -56
 2105 087f 0A       		.uleb128 0xa
 2106 0880 00000000 		.long	.LASF92
 2107 0884 0D       		.byte	0xd
 2108 0885 3B       		.byte	0x3b
 2109 0886 2B030000 		.long	0x32b
 2110 088a 02       		.uleb128 0x2
 2111 088b 91       		.byte	0x91
 2112 088c 40       		.sleb128 -64
 2113 088d 0A       		.uleb128 0xa
 2114 088e 00000000 		.long	.LASF107
 2115 0892 0D       		.byte	0xd
 2116 0893 48       		.byte	0x48
 2117 0894 96000000 		.long	0x96
 2118 0898 03       		.uleb128 0x3
 2119 0899 91       		.byte	0x91
 2120 089a B87F     		.sleb128 -72
 2121 089c 07       		.uleb128 0x7
 2122 089d 00000000 		.long	.LASF108
 2123 08a1 0E       		.byte	0xe
 2124 08a2 0A       		.byte	0xa
 2125 08a3 96000000 		.long	0x96
 2126 08a7 02       		.uleb128 0x2
 2127 08a8 91       		.byte	0x91
 2128 08a9 60       		.sleb128 -32
 2129 08aa 07       		.uleb128 0x7
 2130 08ab 00000000 		.long	.LASF109
 2131 08af 0F       		.byte	0xf
 2132 08b0 0B       		.byte	0xb
 2133 08b1 B5020000 		.long	0x2b5
 2134 08b5 02       		.uleb128 0x2
 2135 08b6 91       		.byte	0x91
 2136 08b7 58       		.sleb128 -40
 2137 08b8 07       		.uleb128 0x7
 2138 08b9 00000000 		.long	.LASF110
 2139 08bd 2D       		.byte	0x2d
 2140 08be 0C       		.byte	0xc
 2141 08bf 2E000000 		.long	0x2e
 2142 08c3 02       		.uleb128 0x2
 2143 08c4 91       		.byte	0x91
 2144 08c5 68       		.sleb128 -24
 2145 08c6 00       		.byte	0
 2146 08c7 00       		.byte	0
 2147              		.section	.debug_abbrev,"",@progbits
 2148              	.Ldebug_abbrev0:
 2149 0000 01       		.uleb128 0x1
 2150 0001 0D       		.uleb128 0xd
 2151 0002 00       		.byte	0
 2152 0003 03       		.uleb128 0x3
 2153 0004 0E       		.uleb128 0xe
GAS LISTING /tmp/ccAJyxY4.s 			page 48


 2154 0005 3A       		.uleb128 0x3a
 2155 0006 0B       		.uleb128 0xb
 2156 0007 3B       		.uleb128 0x3b
 2157 0008 0B       		.uleb128 0xb
 2158 0009 39       		.uleb128 0x39
 2159 000a 0B       		.uleb128 0xb
 2160 000b 49       		.uleb128 0x49
 2161 000c 13       		.uleb128 0x13
 2162 000d 38       		.uleb128 0x38
 2163 000e 0B       		.uleb128 0xb
 2164 000f 00       		.byte	0
 2165 0010 00       		.byte	0
 2166 0011 02       		.uleb128 0x2
 2167 0012 05       		.uleb128 0x5
 2168 0013 00       		.byte	0
 2169 0014 49       		.uleb128 0x49
 2170 0015 13       		.uleb128 0x13
 2171 0016 00       		.byte	0
 2172 0017 00       		.byte	0
 2173 0018 03       		.uleb128 0x3
 2174 0019 16       		.uleb128 0x16
 2175 001a 00       		.byte	0
 2176 001b 03       		.uleb128 0x3
 2177 001c 0E       		.uleb128 0xe
 2178 001d 3A       		.uleb128 0x3a
 2179 001e 0B       		.uleb128 0xb
 2180 001f 3B       		.uleb128 0x3b
 2181 0020 0B       		.uleb128 0xb
 2182 0021 39       		.uleb128 0x39
 2183 0022 0B       		.uleb128 0xb
 2184 0023 49       		.uleb128 0x49
 2185 0024 13       		.uleb128 0x13
 2186 0025 00       		.byte	0
 2187 0026 00       		.byte	0
 2188 0027 04       		.uleb128 0x4
 2189 0028 0D       		.uleb128 0xd
 2190 0029 00       		.byte	0
 2191 002a 03       		.uleb128 0x3
 2192 002b 08       		.uleb128 0x8
 2193 002c 3A       		.uleb128 0x3a
 2194 002d 21       		.uleb128 0x21
 2195 002e 08       		.sleb128 8
 2196 002f 3B       		.uleb128 0x3b
 2197 0030 0B       		.uleb128 0xb
 2198 0031 39       		.uleb128 0x39
 2199 0032 0B       		.uleb128 0xb
 2200 0033 49       		.uleb128 0x49
 2201 0034 13       		.uleb128 0x13
 2202 0035 38       		.uleb128 0x38
 2203 0036 0B       		.uleb128 0xb
 2204 0037 00       		.byte	0
 2205 0038 00       		.byte	0
 2206 0039 05       		.uleb128 0x5
 2207 003a 0F       		.uleb128 0xf
 2208 003b 00       		.byte	0
 2209 003c 0B       		.uleb128 0xb
 2210 003d 21       		.uleb128 0x21
GAS LISTING /tmp/ccAJyxY4.s 			page 49


 2211 003e 08       		.sleb128 8
 2212 003f 49       		.uleb128 0x49
 2213 0040 13       		.uleb128 0x13
 2214 0041 00       		.byte	0
 2215 0042 00       		.byte	0
 2216 0043 06       		.uleb128 0x6
 2217 0044 24       		.uleb128 0x24
 2218 0045 00       		.byte	0
 2219 0046 0B       		.uleb128 0xb
 2220 0047 0B       		.uleb128 0xb
 2221 0048 3E       		.uleb128 0x3e
 2222 0049 0B       		.uleb128 0xb
 2223 004a 03       		.uleb128 0x3
 2224 004b 0E       		.uleb128 0xe
 2225 004c 00       		.byte	0
 2226 004d 00       		.byte	0
 2227 004e 07       		.uleb128 0x7
 2228 004f 34       		.uleb128 0x34
 2229 0050 00       		.byte	0
 2230 0051 03       		.uleb128 0x3
 2231 0052 0E       		.uleb128 0xe
 2232 0053 3A       		.uleb128 0x3a
 2233 0054 21       		.uleb128 0x21
 2234 0055 01       		.sleb128 1
 2235 0056 3B       		.uleb128 0x3b
 2236 0057 0B       		.uleb128 0xb
 2237 0058 39       		.uleb128 0x39
 2238 0059 0B       		.uleb128 0xb
 2239 005a 49       		.uleb128 0x49
 2240 005b 13       		.uleb128 0x13
 2241 005c 02       		.uleb128 0x2
 2242 005d 18       		.uleb128 0x18
 2243 005e 00       		.byte	0
 2244 005f 00       		.byte	0
 2245 0060 08       		.uleb128 0x8
 2246 0061 2E       		.uleb128 0x2e
 2247 0062 01       		.byte	0x1
 2248 0063 3F       		.uleb128 0x3f
 2249 0064 19       		.uleb128 0x19
 2250 0065 03       		.uleb128 0x3
 2251 0066 0E       		.uleb128 0xe
 2252 0067 3A       		.uleb128 0x3a
 2253 0068 0B       		.uleb128 0xb
 2254 0069 3B       		.uleb128 0x3b
 2255 006a 05       		.uleb128 0x5
 2256 006b 39       		.uleb128 0x39
 2257 006c 0B       		.uleb128 0xb
 2258 006d 27       		.uleb128 0x27
 2259 006e 19       		.uleb128 0x19
 2260 006f 49       		.uleb128 0x49
 2261 0070 13       		.uleb128 0x13
 2262 0071 3C       		.uleb128 0x3c
 2263 0072 19       		.uleb128 0x19
 2264 0073 01       		.uleb128 0x1
 2265 0074 13       		.uleb128 0x13
 2266 0075 00       		.byte	0
 2267 0076 00       		.byte	0
GAS LISTING /tmp/ccAJyxY4.s 			page 50


 2268 0077 09       		.uleb128 0x9
 2269 0078 05       		.uleb128 0x5
 2270 0079 00       		.byte	0
 2271 007a 03       		.uleb128 0x3
 2272 007b 08       		.uleb128 0x8
 2273 007c 3A       		.uleb128 0x3a
 2274 007d 21       		.uleb128 0x21
 2275 007e 01       		.sleb128 1
 2276 007f 3B       		.uleb128 0x3b
 2277 0080 0B       		.uleb128 0xb
 2278 0081 39       		.uleb128 0x39
 2279 0082 0B       		.uleb128 0xb
 2280 0083 49       		.uleb128 0x49
 2281 0084 13       		.uleb128 0x13
 2282 0085 02       		.uleb128 0x2
 2283 0086 18       		.uleb128 0x18
 2284 0087 00       		.byte	0
 2285 0088 00       		.byte	0
 2286 0089 0A       		.uleb128 0xa
 2287 008a 05       		.uleb128 0x5
 2288 008b 00       		.byte	0
 2289 008c 03       		.uleb128 0x3
 2290 008d 0E       		.uleb128 0xe
 2291 008e 3A       		.uleb128 0x3a
 2292 008f 21       		.uleb128 0x21
 2293 0090 01       		.sleb128 1
 2294 0091 3B       		.uleb128 0x3b
 2295 0092 0B       		.uleb128 0xb
 2296 0093 39       		.uleb128 0x39
 2297 0094 0B       		.uleb128 0xb
 2298 0095 49       		.uleb128 0x49
 2299 0096 13       		.uleb128 0x13
 2300 0097 02       		.uleb128 0x2
 2301 0098 18       		.uleb128 0x18
 2302 0099 00       		.byte	0
 2303 009a 00       		.byte	0
 2304 009b 0B       		.uleb128 0xb
 2305 009c 34       		.uleb128 0x34
 2306 009d 00       		.byte	0
 2307 009e 03       		.uleb128 0x3
 2308 009f 08       		.uleb128 0x8
 2309 00a0 3A       		.uleb128 0x3a
 2310 00a1 21       		.uleb128 0x21
 2311 00a2 01       		.sleb128 1
 2312 00a3 3B       		.uleb128 0x3b
 2313 00a4 0B       		.uleb128 0xb
 2314 00a5 39       		.uleb128 0x39
 2315 00a6 0B       		.uleb128 0xb
 2316 00a7 49       		.uleb128 0x49
 2317 00a8 13       		.uleb128 0x13
 2318 00a9 02       		.uleb128 0x2
 2319 00aa 18       		.uleb128 0x18
 2320 00ab 00       		.byte	0
 2321 00ac 00       		.byte	0
 2322 00ad 0C       		.uleb128 0xc
 2323 00ae 2E       		.uleb128 0x2e
 2324 00af 01       		.byte	0x1
GAS LISTING /tmp/ccAJyxY4.s 			page 51


 2325 00b0 3F       		.uleb128 0x3f
 2326 00b1 19       		.uleb128 0x19
 2327 00b2 03       		.uleb128 0x3
 2328 00b3 0E       		.uleb128 0xe
 2329 00b4 3A       		.uleb128 0x3a
 2330 00b5 21       		.uleb128 0x21
 2331 00b6 01       		.sleb128 1
 2332 00b7 3B       		.uleb128 0x3b
 2333 00b8 0B       		.uleb128 0xb
 2334 00b9 39       		.uleb128 0x39
 2335 00ba 21       		.uleb128 0x21
 2336 00bb 06       		.sleb128 6
 2337 00bc 27       		.uleb128 0x27
 2338 00bd 19       		.uleb128 0x19
 2339 00be 11       		.uleb128 0x11
 2340 00bf 01       		.uleb128 0x1
 2341 00c0 12       		.uleb128 0x12
 2342 00c1 07       		.uleb128 0x7
 2343 00c2 40       		.uleb128 0x40
 2344 00c3 18       		.uleb128 0x18
 2345 00c4 7C       		.uleb128 0x7c
 2346 00c5 19       		.uleb128 0x19
 2347 00c6 01       		.uleb128 0x1
 2348 00c7 13       		.uleb128 0x13
 2349 00c8 00       		.byte	0
 2350 00c9 00       		.byte	0
 2351 00ca 0D       		.uleb128 0xd
 2352 00cb 37       		.uleb128 0x37
 2353 00cc 00       		.byte	0
 2354 00cd 49       		.uleb128 0x49
 2355 00ce 13       		.uleb128 0x13
 2356 00cf 00       		.byte	0
 2357 00d0 00       		.byte	0
 2358 00d1 0E       		.uleb128 0xe
 2359 00d2 13       		.uleb128 0x13
 2360 00d3 00       		.byte	0
 2361 00d4 03       		.uleb128 0x3
 2362 00d5 0E       		.uleb128 0xe
 2363 00d6 3C       		.uleb128 0x3c
 2364 00d7 19       		.uleb128 0x19
 2365 00d8 00       		.byte	0
 2366 00d9 00       		.byte	0
 2367 00da 0F       		.uleb128 0xf
 2368 00db 01       		.uleb128 0x1
 2369 00dc 01       		.byte	0x1
 2370 00dd 49       		.uleb128 0x49
 2371 00de 13       		.uleb128 0x13
 2372 00df 01       		.uleb128 0x1
 2373 00e0 13       		.uleb128 0x13
 2374 00e1 00       		.byte	0
 2375 00e2 00       		.byte	0
 2376 00e3 10       		.uleb128 0x10
 2377 00e4 2E       		.uleb128 0x2e
 2378 00e5 01       		.byte	0x1
 2379 00e6 3F       		.uleb128 0x3f
 2380 00e7 19       		.uleb128 0x19
 2381 00e8 03       		.uleb128 0x3
GAS LISTING /tmp/ccAJyxY4.s 			page 52


 2382 00e9 0E       		.uleb128 0xe
 2383 00ea 3A       		.uleb128 0x3a
 2384 00eb 0B       		.uleb128 0xb
 2385 00ec 3B       		.uleb128 0x3b
 2386 00ed 05       		.uleb128 0x5
 2387 00ee 39       		.uleb128 0x39
 2388 00ef 21       		.uleb128 0x21
 2389 00f0 0D       		.sleb128 13
 2390 00f1 27       		.uleb128 0x27
 2391 00f2 19       		.uleb128 0x19
 2392 00f3 3C       		.uleb128 0x3c
 2393 00f4 19       		.uleb128 0x19
 2394 00f5 01       		.uleb128 0x1
 2395 00f6 13       		.uleb128 0x13
 2396 00f7 00       		.byte	0
 2397 00f8 00       		.byte	0
 2398 00f9 11       		.uleb128 0x11
 2399 00fa 0B       		.uleb128 0xb
 2400 00fb 01       		.byte	0x1
 2401 00fc 11       		.uleb128 0x11
 2402 00fd 01       		.uleb128 0x1
 2403 00fe 12       		.uleb128 0x12
 2404 00ff 07       		.uleb128 0x7
 2405 0100 00       		.byte	0
 2406 0101 00       		.byte	0
 2407 0102 12       		.uleb128 0x12
 2408 0103 13       		.uleb128 0x13
 2409 0104 01       		.byte	0x1
 2410 0105 03       		.uleb128 0x3
 2411 0106 0E       		.uleb128 0xe
 2412 0107 0B       		.uleb128 0xb
 2413 0108 0B       		.uleb128 0xb
 2414 0109 3A       		.uleb128 0x3a
 2415 010a 0B       		.uleb128 0xb
 2416 010b 3B       		.uleb128 0x3b
 2417 010c 0B       		.uleb128 0xb
 2418 010d 39       		.uleb128 0x39
 2419 010e 0B       		.uleb128 0xb
 2420 010f 01       		.uleb128 0x1
 2421 0110 13       		.uleb128 0x13
 2422 0111 00       		.byte	0
 2423 0112 00       		.byte	0
 2424 0113 13       		.uleb128 0x13
 2425 0114 21       		.uleb128 0x21
 2426 0115 00       		.byte	0
 2427 0116 49       		.uleb128 0x49
 2428 0117 13       		.uleb128 0x13
 2429 0118 2F       		.uleb128 0x2f
 2430 0119 0B       		.uleb128 0xb
 2431 011a 00       		.byte	0
 2432 011b 00       		.byte	0
 2433 011c 14       		.uleb128 0x14
 2434 011d 18       		.uleb128 0x18
 2435 011e 00       		.byte	0
 2436 011f 00       		.byte	0
 2437 0120 00       		.byte	0
 2438 0121 15       		.uleb128 0x15
GAS LISTING /tmp/ccAJyxY4.s 			page 53


 2439 0122 2E       		.uleb128 0x2e
 2440 0123 01       		.byte	0x1
 2441 0124 3F       		.uleb128 0x3f
 2442 0125 19       		.uleb128 0x19
 2443 0126 03       		.uleb128 0x3
 2444 0127 0E       		.uleb128 0xe
 2445 0128 3A       		.uleb128 0x3a
 2446 0129 0B       		.uleb128 0xb
 2447 012a 3B       		.uleb128 0x3b
 2448 012b 0B       		.uleb128 0xb
 2449 012c 39       		.uleb128 0x39
 2450 012d 0B       		.uleb128 0xb
 2451 012e 27       		.uleb128 0x27
 2452 012f 19       		.uleb128 0x19
 2453 0130 49       		.uleb128 0x49
 2454 0131 13       		.uleb128 0x13
 2455 0132 3C       		.uleb128 0x3c
 2456 0133 19       		.uleb128 0x19
 2457 0134 01       		.uleb128 0x1
 2458 0135 13       		.uleb128 0x13
 2459 0136 00       		.byte	0
 2460 0137 00       		.byte	0
 2461 0138 16       		.uleb128 0x16
 2462 0139 2E       		.uleb128 0x2e
 2463 013a 01       		.byte	0x1
 2464 013b 3F       		.uleb128 0x3f
 2465 013c 19       		.uleb128 0x19
 2466 013d 03       		.uleb128 0x3
 2467 013e 0E       		.uleb128 0xe
 2468 013f 3A       		.uleb128 0x3a
 2469 0140 21       		.uleb128 0x21
 2470 0141 01       		.sleb128 1
 2471 0142 3B       		.uleb128 0x3b
 2472 0143 0B       		.uleb128 0xb
 2473 0144 39       		.uleb128 0x39
 2474 0145 0B       		.uleb128 0xb
 2475 0146 27       		.uleb128 0x27
 2476 0147 19       		.uleb128 0x19
 2477 0148 49       		.uleb128 0x49
 2478 0149 13       		.uleb128 0x13
 2479 014a 11       		.uleb128 0x11
 2480 014b 01       		.uleb128 0x1
 2481 014c 12       		.uleb128 0x12
 2482 014d 07       		.uleb128 0x7
 2483 014e 40       		.uleb128 0x40
 2484 014f 18       		.uleb128 0x18
 2485 0150 7C       		.uleb128 0x7c
 2486 0151 19       		.uleb128 0x19
 2487 0152 01       		.uleb128 0x1
 2488 0153 13       		.uleb128 0x13
 2489 0154 00       		.byte	0
 2490 0155 00       		.byte	0
 2491 0156 17       		.uleb128 0x17
 2492 0157 11       		.uleb128 0x11
 2493 0158 01       		.byte	0x1
 2494 0159 25       		.uleb128 0x25
 2495 015a 0E       		.uleb128 0xe
GAS LISTING /tmp/ccAJyxY4.s 			page 54


 2496 015b 13       		.uleb128 0x13
 2497 015c 0B       		.uleb128 0xb
 2498 015d 03       		.uleb128 0x3
 2499 015e 1F       		.uleb128 0x1f
 2500 015f 1B       		.uleb128 0x1b
 2501 0160 1F       		.uleb128 0x1f
 2502 0161 11       		.uleb128 0x11
 2503 0162 01       		.uleb128 0x1
 2504 0163 12       		.uleb128 0x12
 2505 0164 07       		.uleb128 0x7
 2506 0165 10       		.uleb128 0x10
 2507 0166 17       		.uleb128 0x17
 2508 0167 00       		.byte	0
 2509 0168 00       		.byte	0
 2510 0169 18       		.uleb128 0x18
 2511 016a 0F       		.uleb128 0xf
 2512 016b 00       		.byte	0
 2513 016c 0B       		.uleb128 0xb
 2514 016d 0B       		.uleb128 0xb
 2515 016e 00       		.byte	0
 2516 016f 00       		.byte	0
 2517 0170 19       		.uleb128 0x19
 2518 0171 24       		.uleb128 0x24
 2519 0172 00       		.byte	0
 2520 0173 0B       		.uleb128 0xb
 2521 0174 0B       		.uleb128 0xb
 2522 0175 3E       		.uleb128 0x3e
 2523 0176 0B       		.uleb128 0xb
 2524 0177 03       		.uleb128 0x3
 2525 0178 08       		.uleb128 0x8
 2526 0179 00       		.byte	0
 2527 017a 00       		.byte	0
 2528 017b 1A       		.uleb128 0x1a
 2529 017c 26       		.uleb128 0x26
 2530 017d 00       		.byte	0
 2531 017e 49       		.uleb128 0x49
 2532 017f 13       		.uleb128 0x13
 2533 0180 00       		.byte	0
 2534 0181 00       		.byte	0
 2535 0182 1B       		.uleb128 0x1b
 2536 0183 16       		.uleb128 0x16
 2537 0184 00       		.byte	0
 2538 0185 03       		.uleb128 0x3
 2539 0186 0E       		.uleb128 0xe
 2540 0187 3A       		.uleb128 0x3a
 2541 0188 0B       		.uleb128 0xb
 2542 0189 3B       		.uleb128 0x3b
 2543 018a 0B       		.uleb128 0xb
 2544 018b 39       		.uleb128 0x39
 2545 018c 0B       		.uleb128 0xb
 2546 018d 00       		.byte	0
 2547 018e 00       		.byte	0
 2548 018f 1C       		.uleb128 0x1c
 2549 0190 2E       		.uleb128 0x2e
 2550 0191 01       		.byte	0x1
 2551 0192 3F       		.uleb128 0x3f
 2552 0193 19       		.uleb128 0x19
GAS LISTING /tmp/ccAJyxY4.s 			page 55


 2553 0194 03       		.uleb128 0x3
 2554 0195 0E       		.uleb128 0xe
 2555 0196 3A       		.uleb128 0x3a
 2556 0197 0B       		.uleb128 0xb
 2557 0198 3B       		.uleb128 0x3b
 2558 0199 05       		.uleb128 0x5
 2559 019a 39       		.uleb128 0x39
 2560 019b 0B       		.uleb128 0xb
 2561 019c 27       		.uleb128 0x27
 2562 019d 19       		.uleb128 0x19
 2563 019e 8701     		.uleb128 0x87
 2564 01a0 19       		.uleb128 0x19
 2565 01a1 3C       		.uleb128 0x3c
 2566 01a2 19       		.uleb128 0x19
 2567 01a3 01       		.uleb128 0x1
 2568 01a4 13       		.uleb128 0x13
 2569 01a5 00       		.byte	0
 2570 01a6 00       		.byte	0
 2571 01a7 1D       		.uleb128 0x1d
 2572 01a8 2E       		.uleb128 0x2e
 2573 01a9 00       		.byte	0
 2574 01aa 3F       		.uleb128 0x3f
 2575 01ab 19       		.uleb128 0x19
 2576 01ac 03       		.uleb128 0x3
 2577 01ad 0E       		.uleb128 0xe
 2578 01ae 3A       		.uleb128 0x3a
 2579 01af 0B       		.uleb128 0xb
 2580 01b0 3B       		.uleb128 0x3b
 2581 01b1 05       		.uleb128 0x5
 2582 01b2 39       		.uleb128 0x39
 2583 01b3 0B       		.uleb128 0xb
 2584 01b4 27       		.uleb128 0x27
 2585 01b5 19       		.uleb128 0x19
 2586 01b6 49       		.uleb128 0x49
 2587 01b7 13       		.uleb128 0x13
 2588 01b8 3C       		.uleb128 0x3c
 2589 01b9 19       		.uleb128 0x19
 2590 01ba 00       		.byte	0
 2591 01bb 00       		.byte	0
 2592 01bc 1E       		.uleb128 0x1e
 2593 01bd 2E       		.uleb128 0x2e
 2594 01be 01       		.byte	0x1
 2595 01bf 3F       		.uleb128 0x3f
 2596 01c0 19       		.uleb128 0x19
 2597 01c1 03       		.uleb128 0x3
 2598 01c2 0E       		.uleb128 0xe
 2599 01c3 3A       		.uleb128 0x3a
 2600 01c4 0B       		.uleb128 0xb
 2601 01c5 3B       		.uleb128 0x3b
 2602 01c6 0B       		.uleb128 0xb
 2603 01c7 39       		.uleb128 0x39
 2604 01c8 0B       		.uleb128 0xb
 2605 01c9 27       		.uleb128 0x27
 2606 01ca 19       		.uleb128 0x19
 2607 01cb 11       		.uleb128 0x11
 2608 01cc 01       		.uleb128 0x1
 2609 01cd 12       		.uleb128 0x12
GAS LISTING /tmp/ccAJyxY4.s 			page 56


 2610 01ce 07       		.uleb128 0x7
 2611 01cf 40       		.uleb128 0x40
 2612 01d0 18       		.uleb128 0x18
 2613 01d1 7A       		.uleb128 0x7a
 2614 01d2 19       		.uleb128 0x19
 2615 01d3 01       		.uleb128 0x1
 2616 01d4 13       		.uleb128 0x13
 2617 01d5 00       		.byte	0
 2618 01d6 00       		.byte	0
 2619 01d7 1F       		.uleb128 0x1f
 2620 01d8 21       		.uleb128 0x21
 2621 01d9 00       		.byte	0
 2622 01da 49       		.uleb128 0x49
 2623 01db 13       		.uleb128 0x13
 2624 01dc 2F       		.uleb128 0x2f
 2625 01dd 05       		.uleb128 0x5
 2626 01de 00       		.byte	0
 2627 01df 00       		.byte	0
 2628 01e0 20       		.uleb128 0x20
 2629 01e1 2E       		.uleb128 0x2e
 2630 01e2 01       		.byte	0x1
 2631 01e3 3F       		.uleb128 0x3f
 2632 01e4 19       		.uleb128 0x19
 2633 01e5 03       		.uleb128 0x3
 2634 01e6 0E       		.uleb128 0xe
 2635 01e7 3A       		.uleb128 0x3a
 2636 01e8 0B       		.uleb128 0xb
 2637 01e9 3B       		.uleb128 0x3b
 2638 01ea 0B       		.uleb128 0xb
 2639 01eb 39       		.uleb128 0x39
 2640 01ec 0B       		.uleb128 0xb
 2641 01ed 27       		.uleb128 0x27
 2642 01ee 19       		.uleb128 0x19
 2643 01ef 49       		.uleb128 0x49
 2644 01f0 13       		.uleb128 0x13
 2645 01f1 11       		.uleb128 0x11
 2646 01f2 01       		.uleb128 0x1
 2647 01f3 12       		.uleb128 0x12
 2648 01f4 07       		.uleb128 0x7
 2649 01f5 40       		.uleb128 0x40
 2650 01f6 18       		.uleb128 0x18
 2651 01f7 7C       		.uleb128 0x7c
 2652 01f8 19       		.uleb128 0x19
 2653 01f9 00       		.byte	0
 2654 01fa 00       		.byte	0
 2655 01fb 00       		.byte	0
 2656              		.section	.debug_aranges,"",@progbits
 2657 0000 2C000000 		.long	0x2c
 2658 0004 0200     		.value	0x2
 2659 0006 00000000 		.long	.Ldebug_info0
 2660 000a 08       		.byte	0x8
 2661 000b 00       		.byte	0
 2662 000c 0000     		.value	0
 2663 000e 0000     		.value	0
 2664 0010 00000000 		.quad	.Ltext0
 2664      00000000 
 2665 0018 17090000 		.quad	.Letext0-.Ltext0
GAS LISTING /tmp/ccAJyxY4.s 			page 57


 2665      00000000 
 2666 0020 00000000 		.quad	0
 2666      00000000 
 2667 0028 00000000 		.quad	0
 2667      00000000 
 2668              		.section	.debug_line,"",@progbits
 2669              	.Ldebug_line0:
 2670 0000 04040000 		.section	.debug_str,"MS",@progbits,1
 2670      05000800 
 2670      71000000 
 2670      010101FB 
 2670      0E0D0001 
 2671              	.LASF101:
 2672 0000 746F7461 		.string	"total_wram_size"
 2672      6C5F7772 
 2672      616D5F73 
 2672      697A6500 
 2673              	.LASF36:
 2674 0010 5F73686F 		.string	"_shortbuf"
 2674      72746275 
 2674      6600
 2675              	.LASF113:
 2676 001a 72616E64 		.string	"rand"
 2676      00
 2677              	.LASF112:
 2678 001f 5F494F5F 		.string	"_IO_lock_t"
 2678      6C6F636B 
 2678      5F7400
 2679              	.LASF75:
 2680 002a 7374726C 		.string	"strlen"
 2680      656E00
 2681              	.LASF25:
 2682 0031 5F494F5F 		.string	"_IO_buf_end"
 2682      6275665F 
 2682      656E6400 
 2683              	.LASF92:
 2684 003d 62756666 		.string	"buffer"
 2684      657200
 2685              	.LASF85:
 2686 0044 6674656C 		.string	"ftell"
 2686      6C00
 2687              	.LASF23:
 2688 004a 5F494F5F 		.string	"_IO_write_end"
 2688      77726974 
 2688      655F656E 
 2688      6400
 2689              	.LASF3:
 2690 0058 756E7369 		.string	"unsigned int"
 2690      676E6564 
 2690      20696E74 
 2690      00
 2691              	.LASF41:
 2692 0065 5F667265 		.string	"_freeres_list"
 2692      65726573 
 2692      5F6C6973 
 2692      7400
 2693              	.LASF17:
GAS LISTING /tmp/ccAJyxY4.s 			page 58


 2694 0073 5F666C61 		.string	"_flags"
 2694      677300
 2695              	.LASF29:
 2696 007a 5F6D6172 		.string	"_markers"
 2696      6B657273 
 2696      00
 2697              	.LASF93:
 2698 0083 67616D65 		.string	"gameboy_status"
 2698      626F795F 
 2698      73746174 
 2698      757300
 2699              	.LASF98:
 2700 0092 746F7461 		.string	"total_rom_size"
 2700      6C5F726F 
 2700      6D5F7369 
 2700      7A6500
 2701              	.LASF78:
 2702 00a1 63616C6C 		.string	"calloc"
 2702      6F6300
 2703              	.LASF66:
 2704 00a8 6D6D6170 		.string	"mmap"
 2704      00
 2705              	.LASF111:
 2706 00ad 474E5520 		.string	"GNU C17 11.4.0 -mtune=generic -march=x86-64 -g -fasynchronous-unwind-tables -fstack-prote
 2706      43313720 
 2706      31312E34 
 2706      2E30202D 
 2706      6D74756E 
 2707              	.LASF61:
 2708 013b 67616D65 		.string	"gameboy"
 2708      626F7900 
 2709              	.LASF107:
 2710 0143 62756666 		.string	"buffer_size"
 2710      65725F73 
 2710      697A6500 
 2711              	.LASF52:
 2712 014f 75696E74 		.string	"uint32_t"
 2712      33325F74 
 2712      00
 2713              	.LASF90:
 2714 0158 67616D65 		.string	"gameboy_memset"
 2714      626F795F 
 2714      6D656D73 
 2714      657400
 2715              	.LASF28:
 2716 0167 5F494F5F 		.string	"_IO_save_end"
 2716      73617665 
 2716      5F656E64 
 2716      00
 2717              	.LASF48:
 2718 0174 5F494F5F 		.string	"_IO_codecvt"
 2718      636F6465 
 2718      63767400 
 2719              	.LASF62:
 2720 0180 7672616D 		.string	"vram"
 2720      00
 2721              	.LASF95:
GAS LISTING /tmp/ccAJyxY4.s 			page 59


 2722 0185 67616D65 		.string	"gameboy_init"
 2722      626F795F 
 2722      696E6974 
 2722      00
 2723              	.LASF110:
 2724 0192 72657375 		.string	"result"
 2724      6C7400
 2725              	.LASF55:
 2726 0199 6C6F6E67 		.string	"long long unsigned int"
 2726      206C6F6E 
 2726      6720756E 
 2726      7369676E 
 2726      65642069 
 2727              	.LASF109:
 2728 01b0 66696C65 		.string	"file"
 2728      00
 2729              	.LASF106:
 2730 01b5 72656164 		.string	"read_file_into_buffer"
 2730      5F66696C 
 2730      655F696E 
 2730      746F5F62 
 2730      75666665 
 2731              	.LASF65:
 2732 01cb 6872616D 		.string	"hram"
 2732      00
 2733              	.LASF10:
 2734 01d0 5F5F7569 		.string	"__uint16_t"
 2734      6E743136 
 2734      5F7400
 2735              	.LASF27:
 2736 01db 5F494F5F 		.string	"_IO_backup_base"
 2736      6261636B 
 2736      75705F62 
 2736      61736500 
 2737              	.LASF38:
 2738 01eb 5F6F6666 		.string	"_offset"
 2738      73657400 
 2739              	.LASF77:
 2740 01f3 74696D65 		.string	"time"
 2740      00
 2741              	.LASF70:
 2742 01f8 63757272 		.string	"current_vram_bank"
 2742      656E745F 
 2742      7672616D 
 2742      5F62616E 
 2742      6B00
 2743              	.LASF31:
 2744 020a 5F66696C 		.string	"_fileno"
 2744      656E6F00 
 2745              	.LASF7:
 2746 0212 73697A65 		.string	"size_t"
 2746      5F7400
 2747              	.LASF79:
 2748 0219 7372616E 		.string	"srand"
 2748      6400
 2749              	.LASF20:
 2750 021f 5F494F5F 		.string	"_IO_read_base"
GAS LISTING /tmp/ccAJyxY4.s 			page 60


 2750      72656164 
 2750      5F626173 
 2750      6500
 2751              	.LASF72:
 2752 022d 5F426F6F 		.string	"_Bool"
 2752      6C00
 2753              	.LASF80:
 2754 0233 66726565 		.string	"free"
 2754      00
 2755              	.LASF74:
 2756 0238 65786974 		.string	"exit"
 2756      00
 2757              	.LASF56:
 2758 023d 52656769 		.string	"Register"
 2758      73746572 
 2758      00
 2759              	.LASF16:
 2760 0246 63686172 		.string	"char"
 2760      00
 2761              	.LASF44:
 2762 024b 5F6D6F64 		.string	"_mode"
 2762      6500
 2763              	.LASF96:
 2764 0251 66696C65 		.string	"filename"
 2764      6E616D65 
 2764      00
 2765              	.LASF47:
 2766 025a 5F494F5F 		.string	"_IO_marker"
 2766      6D61726B 
 2766      657200
 2767              	.LASF68:
 2768 0265 63757272 		.string	"current_sram_bank"
 2768      656E745F 
 2768      7372616D 
 2768      5F62616E 
 2768      6B00
 2769              	.LASF18:
 2770 0277 5F494F5F 		.string	"_IO_read_ptr"
 2770      72656164 
 2770      5F707472 
 2770      00
 2771              	.LASF58:
 2772 0284 4D656D6F 		.string	"Memory"
 2772      727900
 2773              	.LASF59:
 2774 028b 42616E6B 		.string	"Bank"
 2774      00
 2775              	.LASF50:
 2776 0290 75696E74 		.string	"uint8_t"
 2776      385F7400 
 2777              	.LASF54:
 2778 0298 74696D65 		.string	"time_t"
 2778      5F7400
 2779              	.LASF21:
 2780 029f 5F494F5F 		.string	"_IO_write_base"
 2780      77726974 
 2780      655F6261 
GAS LISTING /tmp/ccAJyxY4.s 			page 61


 2780      736500
 2781              	.LASF53:
 2782 02ae 6C6F6E67 		.string	"long long int"
 2782      206C6F6E 
 2782      6720696E 
 2782      7400
 2783              	.LASF63:
 2784 02bc 7372616D 		.string	"sram"
 2784      00
 2785              	.LASF82:
 2786 02c1 7072696E 		.string	"printf"
 2786      746600
 2787              	.LASF26:
 2788 02c8 5F494F5F 		.string	"_IO_save_base"
 2788      73617665 
 2788      5F626173 
 2788      6500
 2789              	.LASF108:
 2790 02d6 66696C65 		.string	"file_size"
 2790      5F73697A 
 2790      6500
 2791              	.LASF86:
 2792 02e0 66736565 		.string	"fseek"
 2792      6B00
 2793              	.LASF102:
 2794 02e6 64697370 		.string	"display_register"
 2794      6C61795F 
 2794      72656769 
 2794      73746572 
 2794      00
 2795              	.LASF104:
 2796 02f7 73697A65 		.string	"size"
 2796      00
 2797              	.LASF42:
 2798 02fc 5F667265 		.string	"_freeres_buf"
 2798      65726573 
 2798      5F627566 
 2798      00
 2799              	.LASF88:
 2800 0309 696E6974 		.string	"init_cpu"
 2800      5F637075 
 2800      00
 2801              	.LASF43:
 2802 0312 5F5F7061 		.string	"__pad5"
 2802      643500
 2803              	.LASF57:
 2804 0319 52656769 		.string	"Register16"
 2804      73746572 
 2804      313600
 2805              	.LASF87:
 2806 0324 666F7065 		.string	"fopen"
 2806      6E00
 2807              	.LASF35:
 2808 032a 5F767461 		.string	"_vtable_offset"
 2808      626C655F 
 2808      6F666673 
 2808      657400
GAS LISTING /tmp/ccAJyxY4.s 			page 62


 2809              	.LASF76:
 2810 0339 73707269 		.string	"sprintf"
 2810      6E746600 
 2811              	.LASF51:
 2812 0341 75696E74 		.string	"uint16_t"
 2812      31365F74 
 2812      00
 2813              	.LASF67:
 2814 034a 63757272 		.string	"current_rom_bank"
 2814      656E745F 
 2814      726F6D5F 
 2814      62616E6B 
 2814      00
 2815              	.LASF19:
 2816 035b 5F494F5F 		.string	"_IO_read_end"
 2816      72656164 
 2816      5F656E64 
 2816      00
 2817              	.LASF9:
 2818 0368 73686F72 		.string	"short int"
 2818      7420696E 
 2818      7400
 2819              	.LASF12:
 2820 0372 6C6F6E67 		.string	"long int"
 2820      20696E74 
 2820      00
 2821              	.LASF71:
 2822 037b 6367625F 		.string	"cgb_mode"
 2822      6D6F6465 
 2822      00
 2823              	.LASF81:
 2824 0384 66726561 		.string	"fread"
 2824      6400
 2825              	.LASF49:
 2826 038a 5F494F5F 		.string	"_IO_wide_data"
 2826      77696465 
 2826      5F646174 
 2826      6100
 2827              	.LASF83:
 2828 0398 66636C6F 		.string	"fclose"
 2828      736500
 2829              	.LASF100:
 2830 039f 746F7461 		.string	"total_vram_size"
 2830      6C5F7672 
 2830      616D5F73 
 2830      697A6500 
 2831              	.LASF8:
 2832 03af 5F5F7569 		.string	"__uint8_t"
 2832      6E74385F 
 2832      7400
 2833              	.LASF84:
 2834 03b9 72657769 		.string	"rewind"
 2834      6E6400
 2835              	.LASF94:
 2836 03c0 665F6269 		.string	"f_binary"
 2836      6E617279 
 2836      00
GAS LISTING /tmp/ccAJyxY4.s 			page 63


 2837              	.LASF40:
 2838 03c9 5F776964 		.string	"_wide_data"
 2838      655F6461 
 2838      746100
 2839              	.LASF37:
 2840 03d4 5F6C6F63 		.string	"_lock"
 2840      6B00
 2841              	.LASF2:
 2842 03da 6C6F6E67 		.string	"long unsigned int"
 2842      20756E73 
 2842      69676E65 
 2842      6420696E 
 2842      7400
 2843              	.LASF33:
 2844 03ec 5F6F6C64 		.string	"_old_offset"
 2844      5F6F6666 
 2844      73657400 
 2845              	.LASF60:
 2846 03f8 5F494F5F 		.string	"_IO_FILE"
 2846      46494C45 
 2846      00
 2847              	.LASF99:
 2848 0401 746F7461 		.string	"total_sram_size"
 2848      6C5F7372 
 2848      616D5F73 
 2848      697A6500 
 2849              	.LASF64:
 2850 0411 7772616D 		.string	"wram"
 2850      00
 2851              	.LASF89:
 2852 0416 67616D65 		.string	"gameboy_free"
 2852      626F795F 
 2852      66726565 
 2852      00
 2853              	.LASF97:
 2854 0423 746F7461 		.string	"total_banks"
 2854      6C5F6261 
 2854      6E6B7300 
 2855              	.LASF105:
 2856 042f 72616E64 		.string	"randomize"
 2856      6F6D697A 
 2856      6500
 2857              	.LASF73:
 2858 0439 47414D45 		.string	"GAMEBOY"
 2858      424F5900 
 2859              	.LASF4:
 2860 0441 756E7369 		.string	"unsigned char"
 2860      676E6564 
 2860      20636861 
 2860      7200
 2861              	.LASF11:
 2862 044f 5F5F7569 		.string	"__uint32_t"
 2862      6E743332 
 2862      5F7400
 2863              	.LASF22:
 2864 045a 5F494F5F 		.string	"_IO_write_ptr"
 2864      77726974 
GAS LISTING /tmp/ccAJyxY4.s 			page 64


 2864      655F7074 
 2864      7200
 2865              	.LASF69:
 2866 0468 63757272 		.string	"current_wram_bank"
 2866      656E745F 
 2866      7772616D 
 2866      5F62616E 
 2866      6B00
 2867              	.LASF91:
 2868 047a 61646472 		.string	"addr"
 2868      00
 2869              	.LASF15:
 2870 047f 5F5F7469 		.string	"__time_t"
 2870      6D655F74 
 2870      00
 2871              	.LASF39:
 2872 0488 5F636F64 		.string	"_codecvt"
 2872      65637674 
 2872      00
 2873              	.LASF103:
 2874 0491 63726561 		.string	"create_buffer8"
 2874      74655F62 
 2874      75666665 
 2874      723800
 2875              	.LASF13:
 2876 04a0 5F5F6F66 		.string	"__off_t"
 2876      665F7400 
 2877              	.LASF6:
 2878 04a8 7369676E 		.string	"signed char"
 2878      65642063 
 2878      68617200 
 2879              	.LASF5:
 2880 04b4 73686F72 		.string	"short unsigned int"
 2880      7420756E 
 2880      7369676E 
 2880      65642069 
 2880      6E7400
 2881              	.LASF30:
 2882 04c7 5F636861 		.string	"_chain"
 2882      696E00
 2883              	.LASF46:
 2884 04ce 46494C45 		.string	"FILE"
 2884      00
 2885              	.LASF32:
 2886 04d3 5F666C61 		.string	"_flags2"
 2886      67733200 
 2887              	.LASF34:
 2888 04db 5F637572 		.string	"_cur_column"
 2888      5F636F6C 
 2888      756D6E00 
 2889              	.LASF14:
 2890 04e7 5F5F6F66 		.string	"__off64_t"
 2890      6636345F 
 2890      7400
 2891              	.LASF45:
 2892 04f1 5F756E75 		.string	"_unused2"
 2892      73656432 
GAS LISTING /tmp/ccAJyxY4.s 			page 65


 2892      00
 2893              	.LASF24:
 2894 04fa 5F494F5F 		.string	"_IO_buf_base"
 2894      6275665F 
 2894      62617365 
 2894      00
 2895              		.section	.debug_line_str,"MS",@progbits,1
 2896              	.LASF1:
 2897 0000 2F686F6D 		.string	"/home/duys/Documents/cbc"
 2897      652F6475 
 2897      79732F44 
 2897      6F63756D 
 2897      656E7473 
 2898              	.LASF0:
 2899 0019 67616D65 		.string	"gameboy.c"
 2899      626F792E 
 2899      6300
 2900              		.ident	"GCC: (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0"
 2901 0023 2F686F6D 		.section	.note.GNU-stack,"",@progbits
 2901      652F6475 
 2901      79732F44 
 2901      6F63756D 
 2901      656E7473 
 2902              		.section	.note.gnu.property,"a"
 2903              		.align 8
 2904 0000 04000000 		.long	1f - 0f
 2905 0004 10000000 		.long	4f - 1f
 2906 0008 05000000 		.long	5
 2907              	0:
 2908 000c 474E5500 		.string	"GNU"
 2909              	1:
 2910              		.align 8
 2911 0010 020000C0 		.long	0xc0000002
 2912 0014 04000000 		.long	3f - 2f
 2913              	2:
 2914 0018 03000000 		.long	0x3
 2915              	3:
 2916 001c 00000000 		.align 8
 2917              	4:
GAS LISTING /tmp/ccAJyxY4.s 			page 66


DEFINED SYMBOLS
                            *ABS*:0000000000000000 gameboy.c
     /tmp/ccAJyxY4.s:23     .text:0000000000000000 read_file_into_buffer
     /tmp/ccAJyxY4.s:161    .text:0000000000000181 create_buffer8
     /tmp/ccAJyxY4.s:228    .text:00000000000001ed gameboy_init
     /tmp/ccAJyxY4.s:950    .text:0000000000000854 init_cpu
     /tmp/ccAJyxY4.s:329    .text:00000000000002c7 gameboy_status
     /tmp/ccAJyxY4.s:508    .text:00000000000004be display_register
     /tmp/ccAJyxY4.s:615    .text:0000000000000576 gameboy_memset
     /tmp/ccAJyxY4.s:880    .text:00000000000007c6 gameboy_free

UNDEFINED SYMBOLS
fopen
puts
fseek
ftell
rewind
fclose
printf
fread
free
calloc
rand
time
srand
sprintf
strlen
__stack_chk_fail
exit
