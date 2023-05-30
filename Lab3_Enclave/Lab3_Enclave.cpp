#include "Lab3_Enclave_t.h"
#include "sgx_trts.h"
#include <string.h>


const char table[10][33] = {
	"8fb45a90ff009603f938f057f7428cc8",
	"974251e2862eead80a5b6e327c019f92",
	"da5cbab382623be30a8de89d60c46f1d",
	"02363bf4aca89dad5c04e5dc7c7f15ae",
	"7dc0b49c99d000f1c0a6d6e9b5b1080a",
	"3a35cb591843a3ad22c15d97c0a1e939",
	"5598c784534e1cb473d515d8ff40f99c",
	"3f1de303abcf4a6fb4f997db53d8b017",
	"ffce08d8ed19eec21b29ecbcac7c82f4",
	"f9059229d95c485432c71fb821f4dc9b"
};

void foo(char* buf, size_t len, size_t idx) {
    if (idx < 10) {
        const char* data_ptr = table[idx]; // �������� ��������� �� ������ �� ������� �� �������
        size_t data_len = strlen(data_ptr); // ��������� ����� ������
        if (data_len < len) { // ���������, ���������� �� ������ � �����
            memcpy(buf, data_ptr, data_len + 1); // �������� ��� ������ � �����
        }
        else {
            memcpy(buf, data_ptr, len - 1); // �������� len - 1 �������� ������ � �����
            buf[len - 1] = '\0'; // ������������� ������� ������ � ����� ������
        }
    }
    else {
        buf[0] = '\0'; // ���������� ������ ������, ���

    }