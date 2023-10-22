#include<stdio.h>
#include<stdlib.h>
using Byte = unsigned char;
using Word = unsigned short;
using u32 = unsigned int;
struct Mem {
	static constexpr u32 MAX_MEM = 1024 * 64;//64 kb of memory;
	Byte Data[MAX_MEM];

	void Initialise() {
		for (u32 i = 0; i < MAX_MEM; i++) {
			Data[i] = 0;
		}
	}
	// byte reader
	Byte operator[](u32 Address) const 
	{
		return Data[Address];
	}
	// byte writer
	Byte &operator[](u32 Address) 
	{
		return Data[Address];
	}
};

struct CPU {
	

	Word PC;
	Word SP; // stack pointer

	Byte A, X, Y; // memory registers

	Byte C:1; // status flags 
	Byte Z:1;
	Byte I:1;
	Byte D:1;
	Byte B:1;
	Byte V:1;
	Byte N:1;

	void Reset(Mem& memory) {
		PC = 0xFFFC;
		SP = 0X0100;
		D = 0;
		Z = 0;
		I = 0;
		B = 0;
		V = 0;
		N = 0;
		A = X = Y = 0;
		// reset and clear all pointers
		memory.Initialise();
	}

	Byte Fetchbyte(u32& Ticks, Mem& memory) {
		Byte Data = memory[PC];
		PC++;
		Ticks--;
		return Data;
	}
	static constexpr Byte
		INS_LDA_IM = 0xA9;
	void Execute(u32 Ticks,Mem& memory) {
		while (Ticks > 0) {
			
			Byte Ins = Fetchbyte(Ticks, memory);
			switch (Ins) 
			{
			case INS_LDA_IM:
			{
				Byte Value = Fetchbyte(Ticks, memory);
				A = Value;
				Z = (A == 0);
				N = (A & 0b10000000) > 0;
			}
			break;
			default:
			{
				printf("NOt handled id %d", Ins);
				break;
			}
				
			}
			//Ticks--;
		}
	}
};

int main() {
	Mem mem;
	CPU cpu;
	cpu.Reset(mem);

	mem[0xFFFC] = CPU:: INS_LDA_IM;
	mem[0xFFFD] = 0x42;
	cpu.Execute(2,mem);
	return 0;
}
