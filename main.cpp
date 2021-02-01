#include <stdio.h>   // this will provide stdin for when iostream/cout get removed
#include <string.h>  // for all those lovely string manipulation functions with weird names
#include <unistd.h>  // for execvp

const int MAXLEN = 80;

struct Command {  // funct not allowed to return char* [], that's why there's this
    char input[MAXLEN];
    char* args[MAXLEN];
    bool background;
    bool nonempty;  // execvp doesn't like it when you give empty args, easy way to check for it
};

//function declarations
Command get_command();      //read command from consoles & check for special characters
Command executeFromHistory(int lineNumber); //execute previously used command
void logHandler(char appndLn[]);
void appendLine(char putLine[]);
int readNumberOfLines();

int main()
{
    Command c = get_command();
    if (c.nonempty) {
        execvp(c.args[0], c.args);
    }
}

Command get_command(){
    Command c;
    printf("osh>");
    fgets(c.input, MAXLEN, stdin);
    fflush(stdin);
    
    if (strlen(c.input) == 1) {  // pressed enter without any text any text
        c.args[0] = NULL;
        c.background = false;
        c.nonempty = false;
        return c;
    }
    
    //check for ! & !N
    //uitzonderingstoestanden voor ! en !10
    if(c.input[0] == '!' && c.input[1] == '\n'){
        c = executeFromHistory(1);
        printf("%s\n", c.input);        //print used command
        fflush(stdin);                //delete comment in case SegFault
    } else if(c.input[0] == '!' && (48<c.input[1]<=57) && c.input[2] == '\n'){
        c = executeFromHistory(c.input[1]-48);
    } else if(c.input[0] == '!' && c.input[1]=='1' && c.input[2] == '0'){
        c = executeFromHistory(10);
    } else{
        ;
    }
    c.nonempty = true;
    logHandler(c.input);
    //log_history(c.input);         //todo: delete
    c.input[strlen(c.input) - 1] = '\0';  // newline is captured too, need to remove

    // snips char arr into arguments
    char* token;
    char* rest = c.input;
    int i = 0;
    while ((token = strtok_r(rest, " ", &rest))) {
        c.args[i] = token;
        i++;
    }
    c.args[i] = NULL;
    
    // check for '&' which indicates we want it to run in the bg
    char* last = c.args[i - 1];
    if (last[strlen(last) - 1] == '&') {
        c.background = true;
        c.args[i - 1][strlen(last) - 1] = '\0';
    }
    return c;
}

Command executeFromHistory(int lineNumber){
    int indexNum[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    FILE *fp;
    int count = 1;
    lineNumber = indexNum[lineNumber-1];            //telling omkeren van asc naar desc

    Command command;
    fp = fopen("./osshell_history", "r");
    
    while(fgets(command.input, 100, (FILE*)fp)){    //read line from file
        
        if (count == lineNumber){
            fclose(fp);
            return command;
        } else {
            count++;
            if(count > 10){
                break;
            }
        }
    }
    fclose(fp);
    return command;
}

void logHandler(char appndLn[]){
    char buffer;
    FILE *fp, *fp1;
    int lines = 0;
    int lineCounter = 0;
    
    fp = fopen("./osshell_history", "r");
        
    fp1 = fopen("./TEMP", "w");
    //buffer = getc(fp);
    lines = readNumberOfLines();        //check file length to keep at 10 lines
    
        while(lines < 10 && buffer != EOF){ //EOF is endcriteria
            buffer = getc(fp);
            if(buffer != EOF){          //voorkomen dat EOF geprint wordt
                putc(buffer, fp1);
            }
        }
        
        while(lines >= 10 && buffer != EOF){
            buffer = getc(fp);
            if (lineCounter != 0 && buffer != EOF){
                putc(buffer, fp1);
            }

            if (buffer == '\n'){
                lineCounter++;
            }
        }
    fclose(fp);
    fclose(fp1);
    
    rename("./TEMP", "osshell_history");
            
    appendLine(appndLn);
}

int readNumberOfLines(){
    char buffer;
    FILE *fp;
    int lines = 0;
    
    fp = fopen("./osshell_history", "r");
    buffer = getc(fp);

    while (buffer != EOF){
        buffer = getc(fp);
        if (buffer == '\n')
            lines++;
    }
    return lines;
}

void appendLine(char appndLn[]){
    FILE* fp = fopen("./osshell_history", "a");
    auto b = fputs(appndLn, fp);
    fclose(fp);
}
