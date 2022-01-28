#include "token-list.h"
#include "id-list.h"
#include "mpplc.h"

/* keyword list */
struct KEY key[KEYWORDSIZE] = {
	{"and", 	TAND	},
	{"array",	TARRAY	},
	{"begin",	TBEGIN	},
	{"boolean",	TBOOLEAN},
	{"break",	TBREAK  },
	{"call",	TCALL	},
	{"char",	TCHAR	},
	{"div",		TDIV	},
	{"do",		TDO	},
	{"else",	TELSE	},
	{"end",		TEND	},
	{"false",	TFALSE	},
	{"if",		TIF	},
	{"integer",	TINTEGER},
	{"not",		TNOT	},
	{"of",		TOF	},
	{"or",		TOR	},
	{"procedure", TPROCEDURE},
	{"program",	TPROGRAM},
	{"read",	TREAD	},
	{"readln",	TREADLN },
	{"return", 	TRETURN },
	{"then",	TTHEN	},
	{"true",	TTRUE	},
	{"var",		TVAR	},
	{"while",	TWHILE	},
	{"write",	TWRITE  },
	{"writeln",	TWRITELN}
};

/* Token counter */
int numtoken[NUMOFTOKEN+1];

/* string of each token */
char *tokenstr[NUMOFTOKEN+1] = {
	"",
	"NAME", "program", "var", "array", "of", "begin", "end", "if", "then",
	 "else", "procedure", "return", "call", "while", "do", "not", "or", 
	"div", "and", "char", "integer", "boolean", "readln", "writeln", "true",
	 "false", "NUMBER", "STRING", "+", "-", "*", "=", "<>", "<", "<=", ">", 
	">=", "(", ")", "[", "]", ":=", ".", ",", ":", ";", "read","write", "break"
};

/* add */
FILE *fp;
extern char string_attr[MAXSTRSIZE];
int token;
int error_(char *mes);


FILE *outfp;

int main(int nc, char *np[]) {
    if(nc < 2) {
	printf("File name id not given.\n");
	return 0;
    }
   if(init_scan(np[1]) < 0) {
	printf("File %s can not open.\n", np[1]);
	return 0;
    }
    init_globalid();
    init_localid();

    init_DC_list();
    init_output(np[1]);

    token = scan();
    parse_program();
    printf("\n");

    end_output();
    return 0;
}


int error_(char *mes) {
	printf("\n Line:%d / ERROR: %s\n", get_linenum(), mes);
	end_scan();
	exit(-1);
}
