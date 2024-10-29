#include <stdlib.h>
#include <stdarg.h>
#include <syscall.h>
#include <stdint.h>

#define MAX_LENGTH 1000
#define MAX_INT_LENGTH 10
#define STDOUT 1
#define STDERR 2

// extracted from naiveConsole.c
static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}

uint64_t strlen(char * s){
    uint64_t length = 0;
    while(*(s++)) length++;
    return length;
}

void intToStr(int n, char * buff){
    uintToBase(n, buff, 10);
    return;
}

void intToHex(int n, char * buff){
    uintToBase(n, buff, 16);
    return;
}

int strcmp(const char *s1, const char *s2){
    while(*s1 && *s2 && *s1 == *s2){
        s1++;
        s2++;
    }
    return *s1 - *s2;
}


uint64_t printf(const char *format, ...){
    va_list args;
    va_start(args, format);
    char output[MAX_LENGTH] = {0};
    int i = 0, k = 0;
    while(format[i] != 0){
        if(format[i] == '%' && format[i+1] != 0){
            i++;
            switch(format[i]){
                case 'd':{
                    int num = va_arg(args, int);
                    char str[MAX_INT_LENGTH];
                    intToStr(num, str);
                    for(int j = 0; str[j] != 0; j++, k++){
                        output[k] = str[j];
                    }
                    break;
                }
                case 's':{
                    char *str = va_arg(args, char *);
                    for(int j = 0; str[j] != 0; j++, k++){
                        output[k] = str[j];
                    }
                    break;
                }
                case 'c':{
                    uint8_t c = va_arg(args, int);
                    output[k++] = c;
                    break;
                }
                case 'x':{
                    int num = va_arg(args, int);
                    char str[MAX_INT_LENGTH]; 
                    intToHex(num, str);
                    for(int j = 0; str[j] != 0; j++, k++){
                        output[k] = str[j];
                    }
                    break;
                }
                default:
                    break;
            }
        } else {
            output[k++] = format[i];
        }
        i++;
    }
    va_end(args);
    
    return syscall(2, STDOUT, output, strlen(output));
}

int printferror(){
    // como printf pero al hacer syscall ponemos STDERR 
    // TODO modularizar
    return 0; // borrar... cuando se implemente esto
}

char getChar(){
    char c;
    syscall(1,&c,1,0);
    return c;
}

uint64_t readLine(char *buff, uint64_t length) {
    char c;
    int i = 0;
    while ((c = getChar()) != '\n' && i < length - 1) { // -1 para dejar espacio para el null
        if (c == '\b') {
            if (i > 0) {
                i--; // Decrementa el índice para "borrar" el último carácter
                printf("%c", c);
            }
        } else if (c != 0) {
            buff[i++] = c;
            printf("%c", c);
        }
    }
    buff[i] = 0;
    printf("\n");
    return i;
}




