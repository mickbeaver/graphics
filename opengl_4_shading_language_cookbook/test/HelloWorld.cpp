#include <cstdio>
#include <cstring>

void printHello()
{
    printf("Hello\n");
}

int printGoodbye()
{
    return printf("Goodbye\n");
}

typedef void (*funcptr)();

funcptr returnFunction(const char* procName)
{
    if (strcmp(procName, "printHello") == 0) {
        return (funcptr)printHello;
    }

    return (funcptr)printGoodbye;
}


#ifdef __cplusplus
extern "C" 
#endif
int main(int argc, char** argv) {

    void (*printh)() = (void (*)())returnFunction("printHello");
    printh();
    int (*printg)() = (int (*)())returnFunction("printGoodbye");
    printg();

    return 0;
}
