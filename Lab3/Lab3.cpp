#include <stdio.h>
#include <tchar.h>
#include <string.h>

#include "sgx_urts.h"
#include "sgx_tseal.h"
#include "Lab3_Enclave_u.h"
#define ENCLAVE_FILE _T ("Lab3_Enclave.signed.dll")
#define BUF_LEN 100 // Длина буфера обмена между анклавом и небезопасным приложением


int main() {

    char buffer[BUF_LEN] = { 0 }; // Создание буфера buffer размером BUF_LEN и инициализация его нулевыми символами

    sgx_enclave_id_t eid;
    sgx_status_t ret;
    sgx_launch_token_t token = { 0 }; // Объявление переменной token типа sgx_launch_token_t и инициализация ее нулевыми символами
    int updated = 0;

    ret = sgx_create_enclave(ENCLAVE_FILE, SGX_DEBUG_FLAG, &token, &updated, &eid, NULL); 
    if (ret != SGX_SUCCESS) { 
        printf("Application error", ret); 
        return -1; 
    }

    while (true) {
        printf("Enter a number: \t");
        int idx = -1; 
        scanf_s("%d", &idx);
        if (idx < 0) {
            return 0;
        }
        foo(eid, buffer, BUF_LEN, idx); // Вызов функции foo() для обработки данных с использованием анклава и сохранения результатов в буфере buffer
        printf("%s \n******************************************\n\n", buffer);
    }

    if (SGX_SUCCESS != sgx_destroy_enclave(eid)) // Проверка, удалось ли успешно выгрузить анклав
        return -1; 

    return 0; 

}