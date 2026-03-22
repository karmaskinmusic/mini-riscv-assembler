# mini-riscv-assembler
MINI RISC-V ASSEMBLER
A lightweight, interactive command-line assembler that parses and encodes a subset of RISC-V instructions into 32-bit machine code (binary and hexadecimal).

Features
- Supports R, I, S, B, U, and J Instruction formats
- Interactive CLI (read-eval-print)
- Converts assembly -> 32-bit binary and hex
- Customized tokenizer for parsing instruction strings
- Handles memory operands
- Case Insensitive input
- Instruction listing and help commands

Supported Instructions

R-Type
add, sub, xor, or, and, sll, srl, sra, slt, slt, sltu

I-Type
addi, xori, ori, andi, slli, srli, srai, slti, sltiu

Loads
lb, lh, lw, lbu, lhu

S-Type
sb, sh, sw

B-Type
beq, bne, blt, bge, bltu, bgeu

U-Type
lui, auipc

J-Type
jal, jalr

Citation:https://www.cs.sfu.ca/~ashriram/Courses/CS295/assets/notebooks/RISCV/RISCV_CARD.pdf

Example
=========================
   Mini RISC-V Assembler
=========================
Type 'help' for examples, list for pairings, 'exit' to quit.

Type an instruction, or type exit to quit.
> add x1, x2, x3
Binary:          00000000001100010000000010110011
Hex:             0x003100B3

> lw x1, 8(x2)
Binary:          00000000100000010010000010000011
Hex:             0x00812083

> addi x1, x2, x10
Error: invalid stoi argument

> addi x1, x2, 10
Binary:          00000000101000010000000010010011
Hex:            0x00A10093
> list
 sltiu
 add
 srl
 lhu
 or
 sub
 and
 sh
 srai
 xor
 sll
 auipc
 sra
 slt
 andi
 sltu
 srli
 addi
 xori
 ori
 slli
 lui
 slti
 lb
 lh
 lw
 lbu
 sb
 sw
 beq
 bne
 blt
 bge
 jal
 bltu
 bgeu
 jalr

> help
Supported examples:
  add x1, x2, x3
  addi x1, x2, 10
  lw x1, 8(x2)
  sw x3, 12(x4)
  beq x1, x2, 16
  jal x1, 20
Type exit to quit.
