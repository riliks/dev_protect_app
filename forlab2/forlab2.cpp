#include <iostream>

int main()
{
    FILE* file;
    fopen_s(&file, "./test.shchipunov", "r+");
    char buf[256] = { 0 };
    fread_s(buf, 256, sizeof(char), 256, file); // Читаем данные из файла в буфер
    printf_s(buf);
    time_t rawtime;
    struct tm timeinfo;
    char buffer[256];
    time(&rawtime);
    localtime_s(&timeinfo, &rawtime);
    memset(buffer, 0, strlen(buffer)); // Очищаем буфер
    strftime(buffer, sizeof(buffer), "%m-%d   %H:%M:%S", &timeinfo);
    fseek(file, 0, 0); // Перемещаем указатель в начало файла
    fwrite(buffer, sizeof(char), strlen(buffer), file);
    fflush(file);
    fclose(file);
    getchar();

    return 0;
}