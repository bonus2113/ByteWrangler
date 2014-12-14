#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef char byte;
#define global_persist static;
#define local_persist static;

global_persist const char* USAGE = "ByteWrangler <-c|-r|-t> [<input file>]";

enum Commands : byte {
	NOP = 0x00,
	JMP = 0x01,
	ADD = 0x02,
	SUB = 0x03,
	MUL = 0x04,
	AND = 0x05,
	OR	= 0x06,
	XOR = 0x07,
	PSH = 0x08,
	POP = 0x09,
	LD	= 0x0A,
	PRT = 0x0B,
};

global_persist bool
strcmp(const char* str1, const char* str2) {
	while (*str1 != 0 || *str2 != 0) {
		if (*str1 != *str2) {
			return false;
		}

		str1++;
		str2++;
	}
	return true;
}

global_persist int
ReadFile(const char* fileName, byte** data, long* len) {
	if (!fileName) {
		return 1;
	}

	FILE* codeFile;
	if (fopen_s(&codeFile, fileName, "r") != 0) {
		return 1;
	}

	fseek(codeFile, 0, SEEK_END);
	*len = ftell(codeFile);
	rewind(codeFile);

	*data = (byte *)malloc((*len + 1) * sizeof(byte));
	long gotLen = fread_s(*data, *len, 1, *len, codeFile);
	fclose(codeFile);
	assert(gotLen == *len);
	return 0;
}

int compile(int argc, char** argv) {


	return 0;
}

int run(int argc, char** argv) {
	byte* code;
	long length;
	if (ReadFile(argv[2], &code, &length) != 0) {
		printf("Failed to read file %s.", argv[2]);
		return 1;
	}

	byte* end = code + length;

	unsigned int r[8] = {};
	unsigned int* stack = (unsigned int*)malloc(2048 * sizeof(unsigned int));

	while (code < end) {
		switch (*code) {
		case NOP:
		{	
			code++;
			break;
		}
		case JMP:
		{
			code += *(code + 1);
			break;
		}
		case ADD:
		{
			byte ri0 = *(++code);
			byte ri1 = *(++code);
			r[ri0] += r[ri1];
			code++;
			break;
		}
		case MUL:
		{
			byte ri0 = *(++code);
			byte ri1 = *(++code);
			r[ri0] *= r[ri1];
			code++;
			break;
		}
		case LD:
		{
			byte ri0 = *(++code);
			byte val = *(++code);
			r[ri0] = val;
			code++;
			break;
		}
		case PRT:
		{
			byte ri0 = *(++code);
			printf("Value of reqister %d is %x\n", ri0, r[ri0]);
			code++;
			break;
		}
		}
	}

	return 0;
}

int test() {
	assert(strcmp("", ""));
	assert(!strcmp("a", "b"));
	char a[] = "Hello World";
	char* b = "Hello World";
	assert(strcmp(a, b));

	printf("All tests passed!");
	return 0;
}

int main(int argc, char** argv) {
	if (argc < 2) {
		return test();
	}

	if (strcmp(argv[1], "-t")) {
		return test();
	}

	if (argc < 3) {
		printf(USAGE);
		return 0;
	}

	if (strcmp(argv[1], "-c")) {
		return compile(argc, argv);
	}

	if (strcmp(argv[1], "-r")) {
		return run(argc, argv);
	}

	

	printf("Unknow flag.", argv[1]);
	printf(USAGE);
}