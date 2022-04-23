#include "spimcore.h"
/*
ALU                 Adam Whitlock
instruction_fetch   Nicole Zolnier
instruction_partition   Austin Brown
instruction_decode  Austin Brown
read_register       Austin Brown
sign_register       Nicole Zolnier
ALU_operations      Austin Brown
rw_memory           Jonas O'Donnell
write_register      Jonas O'Donnell
PC_update           Adam Whitlock


*/

/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
switch(ALUControl){
        case '0':
            // Case where *ALUresult is not B + A
            *ALUresult = B + A;
            break;

        case '1':
            // Case where *ALUresult is A - B
            *ALUresult = A - B;
            break;

        case '2':
            // Case where A and B are signed. If A < B *ALUresult = 1. If not then, = 0
            if((signed) A < (signed) B)
                *ALUresult = 1;
            else
                *ALUresult = 0;
            break;

        case '3':
            // Case where if A < B then ALUresult is 1 if not than equal to 0
            if(A < B)
                *ALUresult = 1;
            else
                *ALUresult = 0;
            break;

        case '4':
            // Case where *ALUresult is A and B
            *ALUresult = A & B;
            break;

        case '5':
            // Case where the result is either A or B
            *ALUresult = A | B;
            break;

        case '6':
            // Case where B is shifted over 16 places
            *ALUresult = B << 16;
            break;

        case '7':
            // Case where *ALUresult is not A
            *ALUresult = ~A;
            break;

    }

    // Sets the *Zero = 1 if ALUresult == 0
    // Sets the *Zero = 0 if ALUresult != 0
    if(*ALUresult == 0)
        *Zero = '1';
    else
        *Zero = '0';

}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
// PC is the index of the Mem[], where the address is, but to get it right we shift right by 2
unsigned index = PC >> 2;

// if PC isn't byte addressed (divisible by 4)
if(PC % 4 != 0)
	    return 1;

// Mem[index] is the decimal value of the instruction that was in Hex in the file.
// set instruction to the memory
*instruction = Mem[index];
return 0;

}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
//32 bit register size
//000000 <- 26|00 000 <- 21|00000 <- 16| 00000 <- 11|000 00|5 -> 000000
//| op (6 most significant bits) |
*op = instruction >> 26;
//| rs: 1st source register (next 5 MSBs) |
*r1 = instruction >> 21 & 0x1F;
//|rt: 2nd source register (next 5 MBSs) |
*r2 = instruction >> 16 & 0x1F;
//|rd: destination register number (next 5 MSBs) |
*r3 = instruction >> 11 & 0x1F;

//|shamt: shift amount

//|funct: function code (last 6 bits)|
//0x3F: 000000 0 -> |00 0000 1 -> |0000 00 2 -> |000000 3 -> | last 6 bits -> 00000000
*funct = instruction & 0x3F;

//jump to last 26 bits
//removes leftmost 4 bits i think
*jsec = instruction & 0x3FFFFFF;

}



/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{

//https://inst.eecs.berkeley.edu/~cs61c/resources/MIPS_help.html
//http://class.ece.iastate.edu/arun/Cpre305/lectures/week08.pdf
//RegDst | ALUSrc | MemtoReg | RegWrite | MemRead | MemWrite | Branch
switch(op){
    //R-type instructions: 00-09, 12, 16-19, 24-27, 32-43
    //J-type instructions: 02 and 03
    //I-type instructions: 04-15, 32-34, 36-37, 40, 41, 43
case 0:
    //R-type are of opcode 0
    controls -> RegDst = '1';
    controls -> ALUSrc = '0';
    controls -> MemtoReg = '0';
    controls -> RegWrite = '1';
    controls -> MemRead = '0';
    controls -> MemWrite = '0';
    controls -> Branch = '0';
    controls -> ALUOp = '7';
    controls -> Jump = '0';
    break;

case 2:
    //J-type
    controls -> RegDst = 'x';
    controls -> ALUSrc = '1';
    controls -> MemtoReg = 'x';
    controls -> RegWrite = '1';
    controls -> MemRead = '0';
    controls -> MemWrite = '0';
    controls -> Branch = '0';
    controls -> ALUOp = '0';
    controls -> Jump = '1';
    break;

case 3:
    //J-type (jump)
    controls -> RegDst = 'x';
    controls -> ALUSrc = '1';
    controls -> MemtoReg = 'x';
    controls -> RegWrite = '1';
    controls -> MemRead = '0';
    controls -> MemWrite = '0';
    controls -> Branch = '0';
    controls -> ALUOp = '0';
    controls -> Jump = '1';
    break;

case 4:
    //I-type
    controls -> RegDst = 'x';
    controls -> ALUSrc = '0';
    controls -> MemtoReg = 'x';
    controls -> RegWrite = '1';
    controls -> MemRead = '0';
    controls -> MemWrite = '0';
    controls -> Branch = '0';
    controls -> ALUOp = '1';
    controls -> Jump = '0';
    break;

case 8:
    //addi
    controls -> RegDst = '0';
    controls -> ALUSrc = '1';
    controls -> MemtoReg = '0';
    controls -> RegWrite = '1';
    controls -> MemRead = '0';
    controls -> MemWrite = '0';
    controls -> Branch = '0';
    controls -> ALUOp = '0';
    controls -> Jump = '0';
    break;

case 10:
    //slti
    controls -> RegDst = '0';
    controls -> ALUSrc = '1';
    controls -> MemtoReg = '0';
    controls -> RegWrite = '1';
    controls -> MemRead = '0';
    controls -> MemWrite = '0';
    controls -> Branch = '0';
    controls -> ALUOp = '2';
    controls -> Jump = '0';
    break;

case 11:
    //sltui
    controls -> RegDst = '0';
    controls -> ALUSrc = '1';
    controls -> MemtoReg = '0';
    controls -> RegWrite = '1';
    controls -> MemRead = '0';
    controls -> MemWrite = '0';
    controls -> Branch = '0';
    controls -> ALUOp = '3';
    controls -> Jump = '0';
    break;

case 15:
    //lui
    controls -> RegDst = '0';
    controls -> ALUSrc = '1';
    controls -> MemtoReg = '0';
    controls -> RegWrite = '1';
    controls -> MemRead = '0';
    controls -> MemWrite = '0';
    controls -> Branch = '0';
    controls -> ALUOp = '6';
    controls -> Jump = '0';
    break;

case 35:
    //lw
    controls -> RegDst = '0';
    controls -> ALUSrc = '1';
    controls -> MemtoReg = '1';
    controls -> RegWrite = '1';
    controls -> MemRead = '1';
    controls -> MemWrite = '0';
    controls -> Branch = '0';
    controls -> ALUOp = '1';
    controls -> Jump = '0';
    break;

case 43:
    //sw
    controls -> RegDst = 'x';
    controls -> ALUSrc = '1';
    controls -> MemtoReg = '1';
    controls -> RegWrite = '1';
    controls -> MemRead = '1';
    controls -> MemWrite = '0';
    controls -> Branch = '0';
    controls -> ALUOp = '1';
    controls -> Jump = '0';
    break;
default:
    return 1;
}
return 0;
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
    //context from ProjectDetails.pptx: Reg is an array containing the register file
data1 += Reg[r1];
data2 += Reg[r2];
}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{
unsigned msb = offset >> 15;
// assign the sign-extended value of offset to extended_value.
// if the MSB is negative, fill with 1s
if(msb == 1)
*extended_value = offset | 0xFFFF0000;
// otherwise fill with 0's
else
*extended_value = offset & 0x0000FFFF;

}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
    //if ALUSrc == '1' is true, then extended_value, otherwise data2
data2 = ALUSrc == '1' ? extended_value : data2;

//if ALUOp is of r-type
if(ALUOp == '7'){
        //then look at funct
    switch(funct){
    case 32: //32: add
        ALUOp = '0';
        break;

    case 34: //34: sub
        ALUOp = '1';
        break;

    case 36: //36: and
        ALUOp = '4';
        break;

    case 37: //37: or
        ALUOp = '5';
        break;

    case 42: //42: slt
        ALUOp = '2';
        break;

    case 43: //43: sltu
        ALUOp = '3';
        break;

    default:
        return 1;
    }

    ALU(data1, data2, ALUOp, ALUresult, Zero);
    return 0;
} else {
switch(ALUOp) {
case '0':
    ALU(data1, data2, ALUOp, ALUresult, Zero);
    return 0;
case '1':
    ALU(data1, data2, ALUOp, ALUresult, Zero);
    return 0;
case '2':
    ALU(data1, data2, ALUOp, ALUresult, Zero);
    return 0;
case '3':
    ALU(data1, data2, ALUOp, ALUresult, Zero);
    return 0;
case '6':
    ALU(data1, data2, ALUOp, ALUresult, Zero);
    return 0;
default:
    return 1;
}

}
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
if(MemWrite=='1'){
            //check to see if aligned, if not then halt condition is found
        if(ALUresult%4 != 0){
            return 1;
        }else{
            //must shift right twice to reference the proper address
        Mem[ALUresult>>2]=data2;
        }
    }
    else if(MemRead=='1'){
            //check to see if aligned, if not then halts
        if(ALUresult%4 != 0){
            return 1;
        }else{
            //must shift right twice to reference the proper address
        *memdata = Mem[ALUresult>>2];
        }
    }
return 0;

}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
if(RegWrite=='1'){
    if(MemtoReg=='1'){
        Reg[r2]=memdata;
    }else{
        if(RegDst=='1'){
        Reg[r3]=ALUresult;
        }else if(RegDst=='0'){
        Reg[r2]=ALUresult;
    }
  }
  }

}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
*PC += 4;

    // Increment pc by the offset of both zero and branch
    if(Zero == '1' && Branch == '1')
    {
        *PC += extended_value << 2;
    }

    // Takes bits of jsec and shifts them left by two. Jump signal uses first 4 bits of pc and sets it
    if(Jump == '1')
{
        *PC = (*PC & 0xf000000) | (jsec << 2);
}

}

