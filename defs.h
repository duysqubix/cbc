#define MAX(a, b) ((a) > (b) ? (a) : (b))


#define SET_BIT(byte, bit) ((byte) |= (1 << (bit)))
#define CLEAR_BIT(byte, bit) ((byte) &= ~(1 << (bit)))
#define TOGGLE_BIT(byte, bit) ((byte) ^= (1 << (bit)))
#define CHECK_BIT(byte, bit) ((byte) & (1 << (bit)))


#define GAMEBOY_ROM_BANK_SIZE 0x4000
#define GAMEBOY_RAM_BANK_SIZE 0x2000
#define GAMEBOY_VRAM_BANK_SIZE 0x2000


// IO Addresses
#define IO_START_ADDR   0xFF00  // Start of IO addresses
#define IO_END_ADDR     0xFF7F  // End of IO addresses
#define IO_P1_JOYP      0xFF00  // Joypad
#define IO_SB           0xFF01  // Serial transfer data
#define IO_SC           0xFF02  // Serial transfer control
#define IO_DIV          0xFF04  // Divider Register
#define IO_TIMA         0xFF05  // Timer counter
#define IO_TMA          0xFF06  // Timer Modulo
#define IO_TAC          0xFF07  // Timer Control
#define IO_IF           0xFF0F  // Interrupt Flag
#define IO_NR10         0xFF10  // Sound Mode 1 register, Sweep register
#define IO_NR11         0xFF11  // Sound Mode 1 register, Sound length/Wave pattern duty
#define IO_NR12         0xFF12  // Sound Mode 1 register, Envelope
#define IO_NR13         0xFF13  // Sound Mode 1 register, Frequency lo
#define IO_NR14         0xFF14  // Sound Mode 1 register, Frequency hi
#define IO_NR21         0xFF16  // Sound Mode 2 register, Sound length/Wave pattern duty
#define IO_NR22         0xFF17  // Sound Mode 2 register, Envelope
#define IO_NR23         0xFF18  // Sound Mode 2 register, Frequency lo
#define IO_NR24         0xFF19  // Sound Mode 2 register, Frequency hi
#define IO_NR30         0xFF1A  // Sound Mode 3 register, Sound on/off
#define IO_NR31         0xFF1B  // Sound Mode 3 register, Sound length
#define IO_NR32         0xFF1C  // Sound Mode 3 register, Select output level
#define IO_NR33         0xFF1D  // Sound Mode 3 register, Frequency lo
#define IO_NR34         0xFF1E  // Sound Mode 3 register, Frequency hi
#define IO_NR41         0xFF20  // Sound Mode 4 register, Sound length
#define IO_NR42         0xFF21  // Sound Mode 4 register, Envelope
#define IO_NR43         0xFF22  // Sound Mode 4 register, Polynomial counter
#define IO_NR44         0xFF23  // Sound Mode 4 register, Counter/consecutive; Initial
#define IO_NR50         0xFF24  // Channel control / ON-OFF / Volume
#define IO_NR51         0xFF25  // Selection of Sound output terminal
#define IO_NR52         0xFF26  // Sound on/off

// Wave RAM (for arbitrary sound data)
#define IO_WAVE_RAM1    0xFF30
#define IO_WAVE_RAM2    0xFF31
#define IO_WAVE_RAM3    0xFF32
#define IO_WAVE_RAM4    0xFF33
#define IO_WAVE_RAM5    0xFF34
#define IO_WAVE_RAM6    0xFF35
#define IO_WAVE_RAM7    0xFF36
#define IO_WAVE_RAM8    0xFF37
#define IO_WAVE_RAM9    0xFF38
#define IO_WAVE_RAMA    0xFF39
#define IO_WAVE_RAMB    0xFF3A
#define IO_WAVE_RAMC    0xFF3B
#define IO_WAVE_RAMD    0xFF3C
#define IO_WAVE_RAME    0xFF3D
#define IO_WAVE_RAMF    0xFF3E

// LCD and related registers
#define IO_LCDC         0xFF40  // LCD Control
#define IO_STAT         0xFF41  // LCD Status
#define IO_SCY          0xFF42  // Scroll Y
#define IO_SCX          0xFF43  // Scroll X
#define IO_LY           0xFF44  // LCDC Y-Coordinate
#define IO_LYC          0xFF45  // LY Compare
#define IO_DMA          0xFF46  // DMA Transfer and Start Address
#define IO_BGP          0xFF47  // BG Palette Data
#define IO_OBP0         0xFF48  // Object Palette 0 Data
#define IO_OBP1         0xFF49  // Object Palette 1 Data
#define IO_WY           0xFF4A  // Window Y Position
#define IO_WX           0xFF4B  // Window X Position

// CGB Mode Only - Prepare Speed Switch and VRAM Bank
#define IO_KEY1         0xFF4D  // Prepare Speed Switch
#define IO_VBK          0xFF4F  // VRAM Bank
#define IO_HDMA1        0xFF51  // New DMA Source, High
#define IO_HDMA2        0xFF52  // New DMA Source, Low
#define IO_HDMA3        0xFF53  // New DMA Destination, High
#define IO_HDMA4        0xFF54  // New DMA Destination, Low
#define IO_HDMA5        0xFF55  // New DMA Length/Mode/Start
#define IO_RP           0xFF56  // Infrared Communications Port
#define IO_BCPS         0xFF68  // Background Color Palette Specification
#define IO_BCPD         0xFF69  // Background Color Palette Data
#define IO_OCPS         0xFF6A  // Object Color Palette Specification
#define IO_OCPD         0xFF6B  // Object Color Palette Data
#define IO_OPRI         0xFF6C  // Object Priority
#define IO_SVBK         0xFF70  // WRAM Bank
#define IO_PCM12        0xFF76  // PCM Channel 1 & 2 Control
#define IO_PCM34        0xFF77  // PCM Channel 3 & 4 Control

// Interrupt Enable Register
#define IE              0xFFFF

// Flags
#define FLAG_CARRY      0x04  // Carry flag
#define FLAG_HALF_CARRY 0x05  // Half carry flag
#define FLAG_SUBTRACT   0x06  // Subtract flag
#define FLAG_ZERO       0x07  // Zero flag

// Interrupts
#define INTR_VBLANK     0x00  // VBlank interrupt
#define INTR_LCDSTAT    0x01  // LCD status interrupt
#define INTR_TIMER      0x02  // Timer interrupt
#define INTR_SERIAL     0x03  // Serial interrupt
#define INTR_JOYPAD     0x04  // Joypad interrupt

#define INTR_VBLANK_ADDR    0x0040  // VBlank interrupt Memory address
#define INTR_LCDSTAT_ADDR   0x0048  // LCD status interrupt Memory address
#define INTR_TIMER_ADDR     0x0050  // Timer interrupt Memory address
#define INTR_SERIAL_ADDR    0x0058  // Serial interrupt Memory address
#define INTR_JOYPAD_ADDR    0x0060  // Joypad interrupt Memory address

// LCD Control Bits
#define LCDC_ENABLE    7  // LCD Display Enable
#define LCDC_WINMAP    6  // Window Tile Map Display Select
#define LCDC_WINEN     5  // Window Display Enable
#define LCDC_BGMAP     4  // BG & Window Tile Data Select
#define LCDC_BGWIN     3  // BG Tile Map Display Select
#define LCDC_OBJSZ     2  // OBJ (Sprite) Size
#define LCDC_OBJEN     1  // OBJ (Sprite) Display Enable
#define LCDC_BGEN      0  // BG Display Enable

// LCD Status Flags
#define STAT_LYCINT    6  // LYC=LY Coincidence Interrupt
#define STAT_OAMINT    5  // Mode 2 OAM Interrupt
#define STAT_VBLINT    4  // Mode 1 V-Blank Interrupt
#define STAT_HBLINT    3  // Mode 0 H-Blank Interrupt
#define STAT_LYC       2  // LYC=LY Coincidence Flag
#define STAT_MODE1     1  // Mode Flag (Mode 0-3)
#define STAT_MODE0     0  // Mode Flag (Mode 0-3)

#define STAT_MODE_HBLANK  0x00  // Mode 0: During H-Blank
#define STAT_MODE_VBLANK  0x01  // Mode 1: During V-Blank
#define STAT_MODE_OAM     0x02  // Mode 2: During Searching OAM-RAM
#define STAT_MODE_TRANS   0x03  // Mode 3: During Transferring Data to LCD Driver

// Timer Control (TAC) Flags
#define TAC_ENABLE        0x04  // Timer enable
#define TAC_SPEED_1024    1024  // CPU_CLOCK / 1024
#define TAC_SPEED_16      16    // CPU_CLOCK / 16
#define TAC_SPEED_64      64    // CPU_CLOCK / 64
#define TAC_SPEED_256     256   // CPU_CLOCK / 256

// Timer Divider Frequency
#define TIMER_DIV_HZ 16384  // 16384 Hz

// Boot ROM and ROM Start Addresses
#define BOOTROM_START_ADDR 0x0000  // Start of CPU addresses
#define ROM_START_ADDR     0x0100  // Start of ROM addresses
