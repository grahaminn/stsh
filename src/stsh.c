#include <stdio.h>
#include <stdlib.h>

#include <editline/readline.h>
#ifdef __APPLE__
#include <histedit.h>
#else
#include <editline/history.h>
#endif

int main(int argc, char **argv) 
{
    puts("stsh version 0.0.1"); 
    
    while (1)
    {
        char *input = readline("stsh> ");
    
        add_history(input);

        free(input);
    }
    return 0;
}
