%{
    #include<stdio.h>
    int count = 0;
    char cmd[10];
    char* strlwr (char* s) {
    for (int i = 0; i < strlen(s); ++i)
        if (s[i] >= 'A' && s[i] <= 'Z')
            s[i] += 'a' - 'A';
    return s;
} 
    int match(char in[]){
        in = strlwr(in);
        if(strcmp(in,"insert") == 0 ||strcmp(in,"update") == 0 || strcmp(in,"delete") == 0 || strcmp(in,"call") == 0 || strcmp(in,"lock") == 0)
            return 1;
        else if (strcmp(in,"create") == 0 ||strcmp(in,"drop") == 0 || strcmp(in,"alter") == 0 || strcmp(in,"truncate") == 0)
            return 0;
        else
            return -1;
    }
%}


%%
[^;]*[;]{1} {
        for (int i =0; i< yyleng;i++){
            if(yytext[i] == ' '){
                cmd[i] = '\0';
                break;
            }
            cmd[i] = yytext[i];
        }
        count ++;
        if(match(cmd) == 1)
            printf("DML:");
        else if(match(cmd) == 0)
            printf("DDL:");
        else
            printf("Neither:");
        printf("%s\n",cmd);
    }
. ;

%%
int main(){
    extern FILE *yyin;
    yyin = fopen("in","r");
    yylex();
    return 0;
}
