#include <shellfunctions.h>
#include <shell.h>
#include <stdlib.h>
#include <syscall.h>
#include <snake.h>
#include <stdint.h>

#define BUFFER_SPACE 1000
#define MAX_ECHO 1000
#define MAX_USERNAME_LENGTH 16
#define PROMPT "%s$>"
#define CANT_INSTRUCTIONS 10
uint64_t curr = 0;

extern void div_zero();
extern void invalid_opcode();

char * help = "Para ayuda relacionada a un comando en especifico, ingrese\nel comando \"man\" seguido del comando a consultar.\n"
                "Lista de comandos disponibles en la Shell:\n"        
                "exit\n"
                "help\n"
                "snake SPEED (numero de 1 a 5)\n"
                "time\n"
                "registers\n"
                "echo\n"
                "man\n"
                "test_div_0\n"
                "test_invalid_opcode\n"
                "clear\n";

typedef enum {
    EXIT = 0,
    HELP,
    SNAKE,
    TIME,
    REGISTERS,
    ECHO, 
    MAN,
    TEST_DIV_0,
    TEST_INVALID_OPCODE,
    CLEAR
} instructions;

static char * inst_list[CANT_INSTRUCTIONS] = {"exit", 
                                            "help", 
                                            "snake", 
                                            "time", 
                                            "registers", 
                                            "echo", 
                                            "man", 
                                            "test_div_0", 
                                            "test_invalid_opcode", 
                                            "clear"};

char * man_list[CANT_INSTRUCTIONS] = {
    "exit - Sale de la terminal.\n"
    "    Uso: exit\n"
    "    Este comando cierra la sesion de la terminal actual y termina cualquier proceso que se este ejecutando. Es util cuando deseas finalizar una sesion o apagar el sistema de manera controlada.\n",

    "help - Muestra la lista de comandos disponibles en la Shell.\n"
    "    Uso: help\n"
    "    Este comando despliega una lista completa de los comandos que se pueden ejecutar desde la terminal, junto con una breve descripcion de cada uno. Es util para los usuarios que no estan familiarizados con todos los comandos disponibles.\n",

    "snake - Inicia un juego de Snake.\n"
    "    Uso: snake SPEEDN\n\n"
    "               SPEEDN: un numero entero que representa la velocidad del juego (1-5).\n"
    "    Este comando ejecuta el popular juego Snakes en la terminal.\n ",

    "time - Muestra en pantalla el horario actual.\n"
    "    Uso: time\n"
    "    Este comando muestra la hora actual del sistema, basada en el reloj interno.\n",

    "registers - Imprime el valor de los registros en tiempo real.\n"
    "    Uso: registers\n"
    "    Este comando muestra los valores actuales de los registros de la CPU. Es ideal para desarrolladores o ingenieros que necesitan monitorear el estado del hardware o realizar depuracion en tiempo real.\n",

    "echo - Imprime en pantalla lo que el usuario ingrese por linea de comando"
    "   Uso: echo \"Mensaje\"\n"
    "   Este comando es util para probar el funcionamiento de la teminal",

    "man - Da informacion sobre el comando que el usuario consulta\n"
    "   Uso: man \"comando\"\n"
    "   Este comando es util para obtener informacion sobre todos los comandos de la termina, y aprender a usarlos\n",


    "test_div_0 - Prueba la excepcion que se produce al intentar dividir por cero.\n"
    "    Uso: divzero\n"
    "    Este comando genera una excepcion controlada al intentar realizar una division por cero, lo cual es un error en la mayoria de los entornos de programacion. El comando esta pensado para para pruebas y depuracion de excepciones del sistema.\n",

    "test_invalid_opcode - Prueba la excepcion que se genera al ingresar un codigo de operacion invalido.\n"
    "    Uso: invopcode\n"
    "    Este comando provoca una excepcion al ejecutar una instruccion con un codigo de operacion invalido.\nSu funcionalidad principal es ver si funciona la excepcion programada en el Kernel\n",

    "clear - Limpia la pantalla de la terminal.\n"
    "    Uso: clear\n"
    "    Este comando borra todo el contenido de la terminal, dejando la pantalla vacia. \nEs util para mejorar la visibilidad cuando la terminal esta llena de texto y quieres comenzar de nuevo con una vista limpia.\n"
};



void man(char * instruction){
    for(int j = 0; j < CANT_INSTRUCTIONS ; j++){
        if(!strcmp(instruction, inst_list[j])){
            printf("%s" , man_list[j]);
            return;
        }
    }
    if(instruction[0] == 0){
        printf("Por favor, ingrese el comando que desea consultar\n");
    } else {
        printf("No existe el comando: %s\n", instruction);
    }
}

int verify_instruction(char * instruction){
    for(int i = 0; i < CANT_INSTRUCTIONS; i++){
        if(strcmp(inst_list[i], instruction) == 0){
            return i;
        }
    }
    return -1;
}

int getInstruction(char * arguments){
    char shell_buffer[BUFFER_SPACE] = {0};
    readLine(shell_buffer, BUFFER_SPACE);
    int i = 0;
    int j = 0;
    char instruction[BUFFER_SPACE] = {0};
    
    for(; i < BUFFER_SPACE; i++){
        if(shell_buffer[i] == ' ' || shell_buffer[i] == '\0'){
            instruction[j] = 0;
            i++;
            break;
        }
        else {
            instruction[j++] = shell_buffer[i];
        }
    }

    int arg_index = 0;

    while (shell_buffer[i] != '\0' && shell_buffer[i] != '\n') {
        arguments[arg_index++] = shell_buffer[i++];
    }
    arguments[arg_index] = '\0';


    int iNum = 0;
    if((iNum = verify_instruction(instruction)) == -1 && instruction[0] != 0){ // borrar esto
        printf("Comando no reconocido: %s\n", instruction); // Usando nuestro printf 
        return -1;
    }
    return iNum;
}

void shell() {
    clear();
    // pedir username
    printf("Ingrese su nombre de usuario: ");
    char username[MAX_USERNAME_LENGTH];
    readLine(username, MAX_USERNAME_LENGTH);
    unsigned int exit = EXIT; // 0
    int instruction;
    clear();
    while(!exit){
        printf(PROMPT, username);
        char arg[BUFFER_SPACE] = {0};
        instruction = getInstruction(arg); // Lee el comando que ingresa el usuario en la shell
        switch(instruction) {
            case HELP: {
                printf("\n");
                printf(help); // Usando nuestro printf
                break;
            }
            case SNAKE: {
                runSnake();
                break;
            }
            case TIME: {
                showTime();
                break;
            }
            case REGISTERS: {
                showRegisters();
                break;
            }
            case TEST_DIV_0: {
                div_zero(); // Funcion de asm
                break;
            }
            case ECHO: {
                printf("%s\n", arg);
                break;
            }
            case MAN: {
                man(arg);
                break;
            }
            case EXIT : {
                exit = 1;
                break;
            }
            case TEST_INVALID_OPCODE: {
                invalid_opcode(); // Funcion de asm
                break;
            }
            case CLEAR : {
                clear();
                break;
            }
            default: {
                break;
            }

            // Salto al proximo renglon si la instruccion no es Snake : 

            if(instruction != SNAKE){
                printf("\n");
            }
        }
        /*
        AL FINAL DE CADA CICLO SE RESETEA EL SHELL BUFFER COMENZANDO DESDE 0, LO HACE getInstruction()? 
        */
    }
    printf("Saliendo de la terminal...\n");
    syscall(12,10,0,0);
    return;
}