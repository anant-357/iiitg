%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    struct tag{
        char tag_name[11];
        int count;
    };
    struct tag tags[100];
%}

%%

[<]{1}[a-z|"="|" "]+[>]{1} {
        yyleng++;
        strcat(yytext,"\0");
        int last = -1;
        char tag_name[11];
        for(int i=1;i<9;i++){
            if(yytext[i] == ' ' || yytext[i] == '>'){
                tag_name[i-1]='\0';
                break;
             }
            tag_name[i-1] = yytext[i];
        } 
        for(int i=0; i< 100; i++){
            if(strcmp(tag_name, tags[i].tag_name) == 0 || strcmp(tags[i].tag_name, "") == 0){
                last = i;
                break;
            }
        }
        strcpy(tags[last].tag_name, tag_name);
        if(!tags[last].count)
            tags[last].count = 1;
        else
            tags[last].count ++;
}

[<]{1}[ ]*[/][a-z|" "]+[>]{1} {
        yyleng++;
        strcat(yytext,"\0");
}

. ;
 
%%

int main(){
    yyin = fopen("input.html", "r");
    yylex();
    for(int i=0; i< 100; i++){
        if(strcmp(tags[i].tag_name, "")!=0)
            printf("tag: %s, count: %d\n",tags[i].tag_name, tags[i].count);
    }
 }
