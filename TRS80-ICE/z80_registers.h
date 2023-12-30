// Z80 registers, flags
// --------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------
// Registers
// --------------------------------------------------------------------------------------------------

#define OPCODE_READ_M1             0x07
#define MEM_WRITE_BYTE             0x02
#define MEM_READ_BYTE              0x03
#define IO_WRITE_BYTE              0x04
#define IO_READ_BYTE               0x05
#define INTERRUPT_ACK              0x09


#define flag_s (  (register_f & 0x80)==0 ? 0 : 1  )
#define flag_z (  (register_f & 0x40)==0 ? 0 : 1  )
#define flag_h (  (register_f & 0x10)==0 ? 0 : 1  )
#define flag_p (  (register_f & 0x04)==0 ? 0 : 1  )
#define flag_n (  (register_f & 0x02)==0 ? 0 : 1  )
#define flag_c (  (register_f & 0x01)==0 ? 0 : 1  )


#define REGISTER_BC  ( (register_b<<8)   | register_c )
#define REGISTER_DE  ( (register_d<<8)   | register_e )
#define REGISTER_HL  ( (register_h<<8)   | register_l )
#define REGISTER_AF  ( (register_a<<8)   | register_f )
#define REGISTER_IX  ( (register_ixh<<8) | register_ixl )
#define REGISTER_IY  ( (register_iyh<<8) | register_iyl )

#define REG_BC 1
#define REG_DE 2
#define REG_HL 3
#define REG_AF 4
#define REG_IX 5
#define REG_IY 6

// --------------------------------------------------------------------------------------------------
// Registers, Flags and variables for clock, opcodes...
// --------------------------------------------------------------------------------------------------

uint8_t  clock_counter = 0;
uint8_t  pause_interrupts = 0;
uint8_t  temp8 = 0;
uint8_t  opcode_byte = 0;
uint8_t  opcode_second_byte = 0;
uint8_t  debounce_refresh = 0;
uint8_t  last_instruction_set_a_prefix = 0;
uint8_t  and_opcode = 0;
uint8_t  prefix_dd = 0;
uint8_t  prefix_fd = 0;
uint8_t  prefix_cb = 0;
uint8_t  inc_dec = 0;
uint8_t  with_carry = 0;
uint8_t  CB_opcode = 0;
uint8_t  cp_opcode = 0;
uint8_t  halt_in_progress = 0;
uint8_t  assert_iack_type0 = 0;
uint8_t  cb_prefix_offset = 0;
uint8_t  register_a    = 0;
uint8_t  register_b    = 0;
uint8_t  register_c    = 0;
uint8_t  register_d    = 0;
uint8_t  register_e    = 0;
uint8_t  register_f    = 0;
uint8_t  register_h    = 0;
uint8_t  register_l    = 0;
uint8_t  register_a2   = 0;
uint8_t  register_b2   = 0;
uint8_t  register_c2   = 0;
uint8_t  register_d2   = 0;
uint8_t  register_e2   = 0;
uint8_t  register_f2   = 0;
uint8_t  register_h2   = 0;
uint8_t  register_l2   = 0;
uint8_t  register_iff1 = 0;
uint8_t  register_iff2 = 0;
uint8_t  register_im   = 0;
uint8_t  register_i    = 0;
uint8_t  register_r    = 0;
uint8_t  register_ixh  = 0;
uint8_t  register_ixl  = 0;
uint8_t  register_iyh  = 0;
uint8_t  register_iyl  = 0;
uint8_t  special  = 0;
uint8_t  nmi_latched = 0;


uint16_t register_sp   = 0;
uint16_t register_pc   = 0;
uint16_t temp16 = 0;

uint32_t direct_nmi = 0;
uint32_t direct_nmi_d = 0;
uint32_t direct_wait = 0;
uint32_t direct_intr = 0;
uint32_t direct_reset = 0;
uint32_t GPIO6_raw_data = 0;

uint8_t fetched_byte = 0;
uint32_t resetButtonPressed = 0;


// --------------------------------------------------------------------------------------------------
// Arrays which hold the Z80 clock cycles for each opcode
// --------------------------------------------------------------------------------------------------

uint8_t Opcode_Timing_Main[256] = {0x0, 0x6, 0x3, 0x2, 0x0, 0x0, 0x3, 0x0, 0x0, 0x7, 0x3, 0x2, 0x0, 0x0, 0x3, 0x0, 0x4, 0x6, 0x3, 0x2, 0x0, 0x0, 0x3, 0x0, 0x8, 0x7, 0x3, 0x2, 0x0, 0x0, 0x3, 0x0, 0x3, 0x6, 0xc, 0x2, 0x0, 0x0, 0x3, 0x0, 0x3, 0x7, 0xc, 0x2, 0x0, 0x0, 0x3, 0x0, 0x3, 0x6, 0x9, 0x2, 0x7, 0x7, 0x6, 0x0, 0x3, 0x7, 0x9, 0x2, 0x0, 0x0, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0x0, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x0, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0x0, 0x1, 0x6, 0x6, 0x6, 0x6, 0x7, 0x3, 0x7, 0x1, 0x6, 0x6, 0x0, 0x6, 0xd, 0x3, 0x7, 0x1, 0x6, 0x6, 0x7, 0x6, 0x7, 0x3, 0x7, 0x1, 0x0, 0x6, 0x7, 0x6, 0x0, 0x3, 0x7, 0x1, 0x6, 0x6, 0xf, 0x6, 0x7, 0x3, 0x7, 0x1, 0x0, 0x6, 0x0, 0x6, 0x0, 0x3, 0x7, 0x1, 0x6, 0x6, 0x0, 0x6, 0x7, 0x3, 0x7, 0x1, 0x2, 0x6, 0x0, 0x6, 0x0, 0x3, 0x7 };
uint8_t Opcode_Timing_ED[256]   = {0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x8, 0x8, 0xb, 0x10, 0x4, 0xa, 0x4, 0x5, 0x8, 0x8, 0xb, 0x10, 0x4, 0xa, 0x4, 0x5, 0x8, 0x8, 0xb, 0x10, 0x4, 0xa, 0x4, 0x5, 0x8, 0x8, 0xb, 0x10, 0x4, 0xa, 0x4, 0x5, 0x8, 0x8, 0xb, 0x10, 0x4, 0xa, 0x4, 0xe, 0x8, 0x8, 0xb, 0x10, 0x4, 0xa, 0x4, 0xe, 0x8, 0x8, 0xb, 0x10, 0x4, 0xa, 0x4, 0x4, 0x8, 0x8, 0xb, 0x10, 0x4, 0xa, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0xc, 0xc, 0xc, 0xc, 0x4, 0x4, 0x4, 0x4, 0xc, 0xc, 0xc, 0xc, 0x4, 0x4, 0x4, 0x4, 0xc, 0xc, 0xc, 0xc, 0x4, 0x4, 0x4, 0x4, 0xc, 0xc, 0xc, 0xc, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4 };
uint8_t Opcode_Timing_DDFD[256] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xb, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xb, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa, 0x10, 0x6, 0x4, 0x4, 0x7, 0x0, 0x0, 0xb, 0x10, 0x6, 0x4, 0x4, 0x7, 0x0, 0x0, 0x0, 0x0, 0x0, 0x13, 0x13, 0xf, 0x0, 0x0, 0xb, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x4, 0xf, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x4, 0xf, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x4, 0xf, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x4, 0xf, 0x0, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0xf, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0xf, 0x4, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0x0, 0xf, 0x0, 0x0, 0x0, 0x0, 0x4, 0x4, 0xf, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x4, 0xf, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x4, 0xf, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x4, 0xf, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x4, 0xf, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x4, 0xf, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x4, 0xf, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x4, 0xf, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x4, 0xf, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xa, 0x0, 0x13, 0x0, 0xb, 0x0, 0x0, 0x0, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x6, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };


// --------------------------------------------------------------------------------------------------
// Pre-calculated 8-bit parity
// --------------------------------------------------------------------------------------------------

uint8_t Parity_Array[256] = {4, 0, 0, 4, 0, 4, 4, 0, 0, 4, 4, 0, 4, 0, 0, 4, 0, 4, 4, 0, 4, 0, 0, 4, 4, 0, 0, 4, 0, 4, 4, 0, 0, 4, 4, 0, 4, 0, 0, 4, 4, 0, 0, 4, 0, 4, 4, 0, 4, 0, 0, 4, 0, 4, 4, 0, 0, 4, 4, 0, 4, 0, 0, 4, 0, 4, 4, 0, 4, 0, 0, 4, 4, 0, 0, 4, 0, 4, 4, 0, 4, 0, 0, 4, 0, 4, 4, 0, 0, 4, 4, 0, 4, 0, 0, 4, 4, 0, 0, 4, 0, 4, 4, 0, 0, 4, 4, 0, 4, 0, 0, 4, 0, 4, 4, 0, 4, 0, 0, 4, 4, 0, 0, 4, 0, 4, 4, 0, 0, 4, 4, 0, 4, 0, 0, 4, 4, 0, 0, 4, 0, 4, 4, 0, 4, 0, 0, 4, 0, 4, 4, 0, 0, 4, 4, 0, 4, 0, 0, 4, 4, 0, 0, 4, 0, 4, 4, 0, 0, 4, 4, 0, 4, 0, 0, 4, 0, 4, 4, 0, 4, 0, 0, 4, 4, 0, 0, 4, 0, 4, 4, 0, 4, 0, 0, 4, 0, 4, 4, 0, 0, 4, 4, 0, 4, 0, 0, 4, 0, 4, 4, 0, 4, 0, 0, 4, 4, 0, 0, 4, 0, 4, 4, 0, 0, 4, 4, 0, 4, 0, 0, 4, 4, 0, 0, 4, 0, 4, 4, 0, 4, 0, 0, 4, 0, 4, 4, 0, 0, 4, 4, 0, 4, 0, 0, 4 };
