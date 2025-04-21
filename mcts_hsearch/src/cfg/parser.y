
%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "config.h"

    #define INITIAL_INPUT_SIZE 256

    extern int yylex(void);
    extern char *yytext;
    extern Config config;

    void setScannerInput(char *input);
    void parseConfig(char *input);
    void finalizeScanner();

    void yyerror(const char *s);
%}

%union {
  int ival;     /* used for passing int values from lexer to parser */
  double dval;  /* used for passing double values from lexer to parser */
}

%token <ival> INTNUMBER
%token <dval> REALNUMBER

%token ARG_VALUE SEPARATOR N K MAXWEIGHT 
EXPLORE_COEFF DEPTH_COEFF
NUM_SEARCHES NUM_GAMES
EXPERIMENT_RUNS ENABLE_SHARED_MEMORY ENABLE_VERBATIM
NUM_THREADS
ENABLE_ACTION_SPACE_SPLITTING SPLITTING_COL LEFT_WEIGHT RIGHT_WEIGHT
HIGHSCORE
HEATING_RATE HEATING_COLUMN_LIMIT;



%start Settings

%%
Settings        : ArgumentList
                ;
ArgumentList    : N SEPARATOR INTNUMBER ArgumentList { config.N = $3; }
                | K SEPARATOR INTNUMBER ArgumentList { config.K = $3; }
                | MAXWEIGHT SEPARATOR INTNUMBER ArgumentList { config.max_weight = $3;}
                | EXPLORE_COEFF SEPARATOR REALNUMBER ArgumentList { config.explore_coeff = $3; }
                | DEPTH_COEFF SEPARATOR REALNUMBER ArgumentList { config.depth_coeff = $3; }
                | NUM_SEARCHES SEPARATOR INTNUMBER ArgumentList { config.num_searches = $3; }
                | NUM_GAMES SEPARATOR INTNUMBER ArgumentList { config.num_games = $3; }
                | EXPERIMENT_RUNS SEPARATOR INTNUMBER ArgumentList { config.experiment_runs = $3; }
                | ENABLE_SHARED_MEMORY SEPARATOR INTNUMBER ArgumentList { config.enable_shared_memory = $3; }
                | ENABLE_VERBATIM SEPARATOR INTNUMBER ArgumentList { config.enable_verbatim = $3; }
                | NUM_THREADS SEPARATOR INTNUMBER ArgumentList { config.num_threads = $3; }
                | ENABLE_ACTION_SPACE_SPLITTING SEPARATOR INTNUMBER ArgumentList { config.enable_action_space_splitting = $3; }
                | SPLITTING_COL SEPARATOR INTNUMBER ArgumentList { config.splitting_col = $3; }
                | LEFT_WEIGHT SEPARATOR INTNUMBER ArgumentList { config.left_weight = $3; }
                | RIGHT_WEIGHT SEPARATOR INTNUMBER ArgumentList { config.right_weight = $3; }
                | HIGHSCORE SEPARATOR INTNUMBER ArgumentList { config.enable_highscore = $3; }
                | HEATING_RATE SEPARATOR REALNUMBER ArgumentList { config.heating_rate = $3; }
                | HEATING_COLUMN_LIMIT SEPARATOR INTNUMBER ArgumentList { config.heating_column_limit = $3; }
                |
                ;

%%

/* the following code is copied verbatim
 * in the generated C parser file
 */

void printToken() {
/* prints yychar in human readable format */
    switch (yychar) {
        case N              : printf("N"); break;
        case K              : printf("K"); break;
        case MAXWEIGHT      : printf("MAXWEIGHT"); break;
        case EXPLORE_COEFF  : printf("EXPLORE_COEFF"); break;
        case DEPTH_COEFF    : printf("DEPTH_COEFF"); break;
        case NUM_SEARCHES   : printf("NUM_SEARCGES"); break;
        case NUM_GAMES      : printf("NUM_GAMES"); break;
        case ENABLE_SHARED_MEMORY : printf("ENABLE_SHARED_MEMORY"); break;
        case ENABLE_VERBATIM : printf("ENABLE_VERBATIM"); break;
        case NUM_THREADS    : printf("NUM_THREADS"); break;
        case SEPARATOR      : printf("="); break;
        case ARG_VALUE      : printf("argument value"); break;
        default             : printf("'%c'", yychar);
    }
}

void yyerror(const char *msg) {
    printf("Syntax error");
    printf("-------------");
    printToken();
    printf("\n");
    exit(EXIT_SUCCESS);
}

void parseConfig(char *input) {
    // Parse Configuration File
    setScannerInput(input);
    yyparse();
    finalizeScanner();
    free(input);

    // Update Implied Values
    config.rows = config.N - config.K;
    // config.depth_coeff = (double)(1) / config.K;
    config.depth_coeff = 0;
}