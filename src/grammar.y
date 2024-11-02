%union {
    int intval;
    char* strval;
}

%token <intval> NUM
%token <strval> CALL
%token PLUS

%%

exp:
   NUM {}
   | exp PLUS exp {}
   ;

%%
