#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <stdexcept>
#include <vector>
#include <string>
#include <cstddef>
#include <sstream>
#include <iomanip>
using namespace std;

enum class Format { R, I, S, B, U, J };

// Struct to define assembler components
struct InstructionInfo
{
	Format format;
	string opcode;
	string funct3;
	string funct7;

	InstructionInfo(Format f, string op, string f3, string f7)
		: format(f), opcode(op), funct3(f3), funct7(f7) {
	}
};

// Map of all RISC-V Base Integer Instructions
unordered_map<string, InstructionInfo> instructionMap =
{
	// R-Type
	{"add",  InstructionInfo(Format::R, "0110011", "000", "0000000")},
	{"sub",  InstructionInfo(Format::R, "0110011", "000", "0100000")},
	{"xor",  InstructionInfo(Format::R, "0110011", "100", "0000000")},
	{"or",   InstructionInfo(Format::R, "0110011", "110", "0000000")},
	{"and",  InstructionInfo(Format::R, "0110011", "111", "0000000")},
	{"sll",  InstructionInfo(Format::R, "0110011", "001", "0000000")},
	{"srl",  InstructionInfo(Format::R, "0110011", "101", "0000000")},
	{"sra",  InstructionInfo(Format::R, "0110011", "101", "0100000")},
	{"slt",  InstructionInfo(Format::R, "0110011", "010", "0000000")},
	{"sltu", InstructionInfo(Format::R, "0110011", "011", "0000000")},

	// I-Type
	{"addi",  InstructionInfo(Format::I, "0010011", "000", "")},
	{"xori",  InstructionInfo(Format::I, "0010011", "100", "")},
	{"ori",   InstructionInfo(Format::I, "0010011", "110", "")},
	{"andi",  InstructionInfo(Format::I, "0010011", "111", "")},
	{"slli",  InstructionInfo(Format::I, "0010011", "001", "0000000")},
	{"srli",  InstructionInfo(Format::I, "0010011", "101", "0000000")},
	{"srai",  InstructionInfo(Format::I, "0010011", "101", "0100000")},
	{"slti",  InstructionInfo(Format::I, "0010011", "010", "")},
	{"sltiu", InstructionInfo(Format::I, "0010011", "011", "")},

	// I-Load
	{"lb",  InstructionInfo(Format::I, "0000011", "000", "")},
	{"lh",  InstructionInfo(Format::I, "0000011", "001", "")},
	{"lw",  InstructionInfo(Format::I, "0000011", "010", "")},
	{"lbu", InstructionInfo(Format::I, "0000011", "100", "")},
	{"lhu", InstructionInfo(Format::I, "0000011", "101", "")},

	// S-Type
	{"sb", InstructionInfo(Format::S, "0100011", "000", "")},
	{"sh", InstructionInfo(Format::S, "0100011", "001", "")},
	{"sw", InstructionInfo(Format::S, "0100011", "010", "")},

	// B-Type
	{"beq",  InstructionInfo(Format::B, "1100011", "000", "")},
	{"bne",  InstructionInfo(Format::B, "1100011", "001", "")},
	{"blt",  InstructionInfo(Format::B, "1100011", "100", "")},
	{"bge",  InstructionInfo(Format::B, "1100011", "101", "")},
	{"bltu", InstructionInfo(Format::B, "1100011", "110", "")},
	{"bgeu", InstructionInfo(Format::B, "1100011", "111", "")},

	// J-Type
	{"jal",  InstructionInfo(Format::J, "1101111", "", "")},
	{"jalr", InstructionInfo(Format::I, "1100111", "000", "")},

	// U-Type
	{"lui",   InstructionInfo(Format::U, "0110111", "", "")},
	{"auipc", InstructionInfo(Format::U, "0010111", "", "")}
};


// Define register map from x0-x31
unordered_map<string, int> regMap =
{
	// X-Registers
	{"x0", 0}, {"x1", 1}, {"x2", 2}, {"x3", 3},
	{"x4", 4}, {"x5", 5}, {"x6", 6}, {"x7", 7},
	{"x8", 8}, {"x9", 9}, {"x10", 10}, {"x11", 11},
	{"x12", 12}, {"x13", 13}, {"x14", 14}, {"x15", 15},
	{"x16", 16}, {"x17", 17}, {"x18", 18}, {"x19", 19},
	{"x20", 20}, {"x21", 21} , {"x22", 22}, {"x23", 23},
	{"x24", 24}, {"x25", 25}, {"x26", 26}, {"x27", 27},
	{"x28", 28}, {"x29", 29}, {"x30", 30}, {"x31", 31},

};

// ===== FUNCTION PROTOTYPES =====

// Utility
string toLowercase(string s);
string toBinary(int value, int width);
string binaryToHex(const string& str);

// Parsing
vector<string> tokenizer(const string& str);
int parseImmediate(const string& str);

// Lookup
int getRegister(const string& str);
InstructionInfo getInstruction(const string& str);

// Encoders
string encodeRType(const InstructionInfo& info, int rd, int rs1, int rs2);
string encodeIType(const InstructionInfo& info, int rd, int rs1, int imm);
string encodeSType(const InstructionInfo& info, int rs2, int rs1, int imm);
string encodeBType(const InstructionInfo& info, int rs2, int rs1, int imm);
string encodeUType(const InstructionInfo& info, int rd, int imm);
string encodeJType(const InstructionInfo& info, int rd, int imm);

// Assembler
string encodeInstruction(const string& line);

// Output
void printInstruction(const string& binary);

// Main
int main()
{
	cout << "=========================\n";
	cout << "   Mini RISC-V Assembler\n";
	cout << "=========================\n";
	cout << "Type 'help' for examples, list for pairings, 'exit' to quit.\n\n";

	try
	{
		string line;

		cout << "Type an instruction, or type exit to quit." << endl;

		while (true)
		{
			cout << "> ";
			getline(cin, line);

			if (line == "exit")
				break;

			if (line.empty())
				continue;

			if (line == "help")
			{
				cout << "Supported examples:\n";
				cout << "  add x1, x2, x3\n";
				cout << "  addi x1, x2, 10\n";
				cout << "  lw x1, 8(x2)\n";
				cout << "  sw x3, 12(x4)\n";
				cout << "  beq x1, x2, 16\n";
				cout << "  jal x1, 20\n";
				cout << "Type exit to quit.\n\n";
				continue;
			}

			if (line == "list")
			{
				for (const auto& pair : instructionMap)
					cout << " " << pair.first << endl;

				cout << endl;
				continue;
			}

			if (line.rfind("#", 0) == 0 || line.rfind("//", 0) == 0)
				continue;

			try
			{
				string binary = encodeInstruction(line);

				printInstruction(binary);
			}
			catch (const exception& e)
			{
				cout << "Error: " << e.what() << endl;
			}

			cout << endl;
		}
	}
	catch (const exception& e)
	{
		cout << "Fatal error: " << e.what() << endl;
	}

	return 0;
}

// Get the register
int getRegister(const string& str)
{
	auto it = regMap.find(str);

	if (it != regMap.end())
	{

		return it->second;
	}

	throw invalid_argument("Invalid Register:" + str);
}

// Convert to lowercase
string toLowercase(string s)
{
	for (char& c : s)
	{
		c = tolower(c);
	}
	return s;
}

// Basic print function for binary and hex
void printInstruction(const string& binary)
{

	cout << "Binary:\t\t " << binary << endl;
	cout << "Hex:\t\t0x" << binaryToHex(binary);
}

InstructionInfo getInstruction(const string& str)
{
	auto it = instructionMap.find(str);

	if (it != instructionMap.end())
	{
		return it->second;
	}

	throw invalid_argument("Invalid Instruction: " + str);
}

string toBinary(int value, int width)
{
	string result;

	for (int i = width - 1; i >= 0; i--)
	{
		result += ((value >> i) & 1) ? '1' : '0';
	}

	return result;
}

string binaryToHex(const string& str)
{
	if (str.length() != 32)
	{
		throw invalid_argument("Binary string must be 32 bits");
	}

	uint32_t value = stoul(str, nullptr, 2);

	stringstream ss;

	ss << uppercase << hex << setw(8) << setfill('0') << value;

	return ss.str();
}
string encodeRType(const InstructionInfo& info, int rd, int rs1, int rs2)
{
	return info.funct7 + toBinary(rs2, 5) + toBinary(rs1, 5) + info.funct3
		+ toBinary(rd, 5) + info.opcode;
}

string encodeIType(const InstructionInfo& info, int rd, int rs1, int imm)
{

	if (info.funct7 != "") // Validate to modify machine code instructions
	{
		return info.funct7 + toBinary(imm, 5) + toBinary(rs1, 5) + info.funct3 + toBinary(rd, 5) + info.opcode;
	}
	else
	{
		return toBinary(imm, 12) + toBinary(rs1, 5) + info.funct3 + toBinary(rd, 5) + info.opcode;
	}

}

string encodeSType(const InstructionInfo& info, int rs2, int rs1, int imm)
{
	string bin = toBinary(imm, 12);

	string imm_left = bin.substr(0, 7);  // imm[11:5]
	string imm_right = bin.substr(7, 5); // imm[4:0]

	return imm_left + toBinary(rs2, 5) + toBinary(rs1, 5) + info.funct3 + imm_right
		+ info.opcode;
}

string encodeBType(const InstructionInfo& info, int rs2, int rs1, int imm)
{
	string bin = toBinary(imm >> 1, 13);

	string imm_12 = bin.substr(0, 1);
	string imm_11 = bin.substr(1, 1);
	string imm_10_5 = bin.substr(2, 6);
	string imm_4_1 = bin.substr(8, 4);

	return imm_12 + imm_10_5 + toBinary(rs2, 5) + toBinary(rs1, 5)
		+ info.funct3 + imm_4_1 + imm_11 + info.opcode;

}

string encodeJType(const InstructionInfo& info, int rd, int imm)
{
	string bin = toBinary(imm >> 1, 21);

	string imm_20 = bin.substr(0, 1);
	string imm_19_12 = bin.substr(1, 8);
	string imm_11 = bin.substr(9, 1);
	string imm_10_1 = bin.substr(10, 10);

	return imm_20 + imm_10_1 + imm_11 + imm_19_12 + toBinary(rd, 5) + info.opcode;
}

string encodeUType(const InstructionInfo& info, int rd, int imm)
{
	return toBinary(imm, 20) + toBinary(rd, 5) + info.opcode;
}

string encodeInstruction(const string& line)
{
	vector<string> tokens = tokenizer(toLowercase(line));

	if (tokens.empty())
		throw invalid_argument("Empty Instruction");

	InstructionInfo info = getInstruction(tokens[0]);

	switch (info.format)
	{
	case Format::R:
	{
		if (tokens.size() != 4)
			throw invalid_argument("R-Type requires: instr rd, rs1, rs2");

		int rd = getRegister(tokens[1]);
		int rs1 = getRegister(tokens[2]);
		int rs2 = getRegister(tokens[3]);

		return encodeRType(info, rd, rs1, rs2);
	}

	case Format::I:
	{
		// LOADS + JALR
		if (tokens[0] == "lb" || tokens[0] == "lh" || tokens[0] == "lw" ||
			tokens[0] == "lbu" || tokens[0] == "lhu" || tokens[0] == "jalr")
		{
			if (tokens.size() != 4)
				throw invalid_argument("Load/jalr requires: instr rd, imm(rs1)");

			int rd = getRegister(tokens[1]);
			int imm = parseImmediate(tokens[2]);
			int rs1 = getRegister(tokens[3]);

return encodeIType(info, rd, rs1, imm);
		}
		else
		{
			if (tokens.size() != 4)
				throw invalid_argument("I-type requires: instr rd, rs1, imm");

			int rd = getRegister(tokens[1]);
			int rs1 = getRegister(tokens[2]);
			int imm = parseImmediate(tokens[3]);

			return encodeIType(info, rd, rs1, imm);
		}
	}

	case Format::S:
	{
		if (tokens.size() != 4)
			throw invalid_argument("S-type requires: instr rs2, imm(rs1)");

		int rs2 = getRegister(tokens[1]);
		int imm = parseImmediate(tokens[2]);
		int rs1 = getRegister(tokens[3]);

		return encodeSType(info, rs2, rs1, imm);
	}

	case Format::B:
	{
		if (tokens.size() != 4)
			throw invalid_argument("B-type requires: instr rs1, rs2, imm");

		int rs1 = getRegister(tokens[1]);
		int rs2 = getRegister(tokens[2]);
		int imm = parseImmediate(tokens[3]);

		return encodeBType(info, rs2, rs1, imm);
	}

	case Format::U:
	{
		if (tokens.size() != 3)
			throw invalid_argument("U-type requires: instr rd, imm");

		int rd = getRegister(tokens[1]);
		int imm = parseImmediate(tokens[2]);

		return encodeUType(info, rd, imm);
	}

	case Format::J:
	{
		if (tokens.size() != 3)
			throw invalid_argument("J-type requires: instr rd, imm");

		int rd = getRegister(tokens[1]);
		int imm = parseImmediate(tokens[2]);

		return encodeJType(info, rd, imm);
	}

	default:
		throw invalid_argument("Unsupported instruction");
	}
}
vector<string> tokenizer(const string& str)
{
	vector<string> tokens;
	string curr;

	for (char c : str)
	{
		if (c == ' ' || c == ',' || c == '(' || c == ')')
		{
			if (!curr.empty())
			{
				tokens.push_back(curr);
				curr.clear();
			}
		}
		else
		{
			curr += c;
		}
	}

	if (!curr.empty())
	{
		tokens.push_back(curr);
	}

	return tokens;
}


int parseImmediate(const string& str)
{
	if (str.size() > 2 && str[0] == '0' &&
		(str[1] == 'x' || str[1] == 'X'))
		return stoi(str, nullptr, 16);

	return stoi(str);
}
