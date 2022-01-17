#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "token-list.h"
#include "id-list.h"
#include "mpplc.h"

#define PP 1

#define NORMAL 0
#define ERROR -1
extern int token;
extern char *tokenstr[NUMOFTOKEN+1];
extern char string_attr[MAXSTRSIZE];
extern int num_attr;

extern char PROCEDURE_NAME[MAXSTRSIZE];
struct ID *head_of_variables;
struct ID *procedure;
struct ID *tail_of_formal_parameters;
struct FP *root_fp;

#define NUMOFFP 100
char FP_NAMES[NUMOFFP][MAXSTRSIZE]; /* for ST */
int FPNUM;

int ARRAYSIZE;

int scan_pp(void);
void print_tab(int tab_num);

int parse_program(void);
int block(void);
int variable_declaration(void);
int variable_names(void);
int variable_name(void);
int type(void);
int standerd_type(void);
int array_type(void);
int subprogram_declaration(void);
int procedure_name(void);
int formal_parameter(void);
int compound_statement(void);
int statement(void);
int condition_statement(void);
int iteration_statement(void);
int exit_statement(void);
int call_statement(void);
struct FP *expressions(void);
int return_statement(void);
int assignment_statement(void);
int left_part(void);
int variable(void);
int expression(void);
int simple_expression(void);
int term(void);
int factor(void);
int constant(void);
int multiplicative_operator(void);
int additive_operator(void);
int relational_operator(void);
int input_statement(void);
int output_statement(void);
int output_format(void);
int empty_statement(void);

int TAB;
int SUBPRO_DEC = 0;
int IN_BEGIN = 0;
int IN_WHILE = 0;
int IN_IF = 0;
int IN_ELSE = 0;
int IN_FP = 0;
int IN_VAR = 0;
int IN_ASSIGN = 0;
int IN_LEFTPART = 0;

int IS_PROCNAME = 0;


int LNUM;
char LATESTLABEL[MAXSTRSIZE];



int scan_pp(void){
	if(!PP) return scan();
	else{
		int next_token;
		int end_of_line;
		end_of_line = 0;
		TAB = SUBPRO_DEC + IN_BEGIN + IN_IF + IN_ELSE;
		/*  BEFORE */
		if(token==TEND){
			printf("\n");
			print_tab(TAB);
		}
		else if(token==TVAR) print_tab(TAB+1);
		else if(token==TPROCEDURE) print_tab(TAB);
		else if(token==TBEGIN && SUBPRO_DEC && IN_BEGIN==1) print_tab(1);

		/* PRINT */
		if(token == TNAME) printf("%s", string_attr);
		else if(token == TNUMBER) printf("%s", string_attr);
		else if(token == TSTRING){printf("'"); printf("%s", string_attr); printf("'");}
		else printf("%s", tokenstr[token]);
		fflush(stdout);
		next_token = scan();

		/*  AFTER */
		if(token==TSEMI){
			if (IN_FP>0);
			else if(IN_VAR>0 && next_token!=TVAR && next_token!=TPROCEDURE && next_token!=TBEGIN){printf("\n"); print_tab(TAB+1+1);}
			else{
				printf("\n");
				end_of_line = 1;
			}
		}
		else if(token == TBEGIN){printf("\n"); end_of_line = 1;}
		else if(token == TDO){printf("\n"); end_of_line = 1;}
		else if(token == TTHEN){printf("\n"); end_of_line = 1;}
		else if(token == TELSE){printf("\n"); end_of_line = 1;}
		else if(next_token == TELSE){printf("\n"); end_of_line = 1;}
		else if(token == TEND); /* for remove blank */
		else if(next_token==TSEMI); /* for remove blank */
		else if(next_token==TCOMMA); /* for remove blank */
		else if(token == TLPAREN); /* for remove blank */
		else if(next_token == TRPAREN); /* for remove blank */
		else if(next_token == TLPAREN); /* for remove blank */
		else printf(" ");
		fflush(stdout);
		if(end_of_line){
			if(IN_BEGIN>0) print_tab(IN_BEGIN);
			if(IN_WHILE>0) print_tab(IN_WHILE);
			if(IN_IF>0) print_tab(IN_IF);
			if(IN_ELSE>0) print_tab(IN_ELSE);
			if(SUBPRO_DEC && IN_BEGIN) print_tab(1);
		}
		end_of_line=0;

		fflush(stdout);
		return next_token;
	}
}

void print_tab(int tab_num){
	int i;
	for(i=0;i<tab_num;i++) printf("    ");
	fflush(stdout);
}

int parse_program(void){
	LNUM=1;
	if(token != TPROGRAM) return(error_("Keyword 'program' is not found"));
	token = scan_pp();
	if(token != TNAME) return(error_("Keyword 'name' is not found"));
	start_mpl(string_attr);
	token = scan_pp();
	if(token != TSEMI) return(error_("';' is not found"));
	token = scan_pp();
	strcpy(PROCEDURE_NAME, " "); /* it means in global */
	if(block() == ERROR) return (ERROR);
	if(token != TDOT) return(error_("'.'is not found at the end of program"));
	token = scan_pp();

	print_DCs();
	OTHER_CSL();

	print_cross_referencer();

	return(NORMAL);
}

int block(void){
	while(token == TVAR || token == TPROCEDURE){
		if(token == TVAR){
			if(variable_declaration()==ERROR) return(ERROR);
		}
		else if(token == TPROCEDURE){
			if(subprogram_declaration()==ERROR) return(ERROR);
		}
	}
	if(compound_statement() == ERROR) return(ERROR/*error_("compound statement is not found")*/);
	return(NORMAL);
}

int variable_declaration(void){
	int Type;
	if(token != TVAR) return(error_("Keyword 'var' is not found"));
	IN_VAR++;
	token = scan_pp();
	head_of_variables = NULL; /* for record variable_names */
	if(variable_names()==ERROR) return(ERROR);
	if(token != TCOLON) return(error_("Symbol ':' is not found"));
	token = scan_pp();

	if((Type = type())==ERROR) return(ERROR);
	record_types(Type);

	if(token != TSEMI) return(error_("Symbol ';' is not found"));
	token = scan_pp();
	while(token == TNAME){
		head_of_variables = NULL; /* for record variable_names */
		if(variable_names()==ERROR) return(error_("Variable names is not found"));
		if(token != TCOLON) return(error_("Symbol ':' is not found"));
		token = scan_pp();
		if((Type = type())==ERROR) return(ERROR);
		record_types(Type);
		if(token != TSEMI) return(error_("Symbol ';' is not found"));
		token = scan_pp();
	}
	IN_VAR--;
	return(NORMAL);
}

int variable_names(void){
	if(variable_name() == ERROR) return(error_("Variable name is not found"));
	while(token == TCOMMA){
		token = scan_pp();
		if(variable_name() == ERROR) return(ERROR);
	}
	return(NORMAL);
}

int variable_name(void){
	int Type;
	Type=0;
	struct ID *temp;
	if(token != TNAME) return(error_("Variable name is not found"));
	if(head_of_variables == NULL){ /* first variable */
		head_of_variables = id_record(string_attr);
		temp = head_of_variables;
	}else{	/* not first variable */
		temp = id_record(string_attr);
	}

	if(IN_FP){
		strcpy(FP_NAMES[FPNUM], string_attr);
		FPNUM++;
	}

	char label[MAXSTRSIZE];
	if(!SUBPRO_DEC) snprintf(label,MAXSTRSIZE,"$%s",string_attr); /* global */
	else snprintf(label,MAXSTRSIZE,"$%s%%%s",string_attr, PROCEDURE_NAME); /* local */
	struct ID *id;
	if(IN_FP || IN_VAR){ /*declear part*/
		DC(label, "0");
	}else{ /*refer part*/
		id = search_idtab(string_attr);
		if(!strcmp(id->procname, " ")) snprintf(label,MAXSTRSIZE,"$%s",id->name); /* global */
		else snprintf(label,MAXSTRSIZE,"$%s%%%s",id->name, id->procname); /* local */
		strcpy(LATESTLABEL, label);
	}
	token = scan_pp();
	if(temp->itp!=NULL){
		Type = temp->itp->ttype;
	}
	return(Type);
}

int type(void){	/* Irregular style */
	int Type;
	/*TYPE = 0;*/
	ARRAYSIZE = 0;
	if(token == TINTEGER || token == TBOOLEAN || token == TCHAR){
		Type = token - TYPEOFFSET;
		token = scan_pp();
	}else if(token == TARRAY){
		if((Type = array_type())==ERROR) return(ERROR);
	}
	return(Type);
}

int standerd_type(void){
	int Type;
	if(token == TINTEGER || token == TBOOLEAN || token == TCHAR){
		Type = token - TYPEOFFSET;
		token = scan_pp();
		return(Type);
	}else return(error_("Standerd type is not found"));
}

int array_type(void){
	int Type;
	if(token != TARRAY) return(error_("Keyword 'array' is not found"));
	token = scan_pp();
	if(token != TLSQPAREN) return(error_("Symbol '[' is not found"));
	token = scan_pp();
	if(token != TNUMBER) return(error_("NUMBER is not found"));
	/* ADD */
	if(num_attr<1) return(error_("ARRAYSIZE must be 1 or more."));

	token = scan_pp();
	if(token != TRSQPAREN) return(error_("Symbol ']' is not found"));
	token = scan_pp();
	if(token != TOF) return(error_("Keyword 'of' is not found"));
	token = scan_pp();
	if((Type = standerd_type()) == ERROR) return(Type + ARRAYTYPEOFFSET);
	return(NORMAL);
}

int subprogram_declaration(void){
	if(token != TPROCEDURE) return(error_("Keyword 'procedure' is not found"));
	SUBPRO_DEC++; /* for indent */
	token = scan_pp();
	IS_PROCNAME = 1;
	FPNUM=0;
	procedure = id_record(string_attr);
	strcpy(PROCEDURE_NAME, string_attr);
	if(procedure_name() == ERROR) return(ERROR);
	IS_PROCNAME = 0;
	procedure->itp->ttype = TPPROC;
	if(token == TLPAREN){ /* if head of formal_parameter is found */
		procedure->fpnum = 0;
		if(formal_parameter() == ERROR) return(ERROR);
	}
	procedure = NULL;
	if(token != TSEMI) return(error_("Symbol ';' is not found"));
	token = scan_pp();
	if(token == TVAR){	/* if head of variable_declaration is found */
		if(variable_declaration()==ERROR) return(ERROR);
	}
	if(compound_statement() == ERROR) return(ERROR);
	if(token != TSEMI) return(error_("Symbol ';' is not found"));
	SUBPRO_DEC--; /* for indent */

	strcpy(PROCEDURE_NAME, " ");	/* scope changed to global */
	if(localidtail->name != 0){
		globalidtail->nextp = localidroot->nextp;	/*connect GLOBAL<-LOCAL*/
		globalidtail = localidtail;	/* change tail */
	}

	init_localid();
	token = scan_pp();
	return(NORMAL);
}

int procedure_name(void){
	if(token != TNAME) return(error_("Procedure name is not found"));
	token = scan_pp();
	return(NORMAL);
}

int formal_parameter(void){
	int Type;
	if(token != TLPAREN) return(error_("Symbol '(' is not found"));
	IN_FP++;
	token = scan_pp();
	head_of_variables = NULL; /* for record variable_names */
	tail_of_formal_parameters = NULL; /* for record formal_parameters */
	if(variable_names() == ERROR) return(ERROR);
	if(token != TCOLON) return(error_("Symbol ':' is not found"));
	token = scan_pp();
	if( strcmp(string_attr, TYPES[TPCHAR]) && strcmp(string_attr, TYPES[TPINT]) && strcmp(string_attr, TYPES[TPBOOL])){	/* type isn't standard type. */
		return(error_("Type must be standard type."));
	}
	if((Type = type()) == ERROR) return(ERROR);
	record_types(Type);
	while(token == TSEMI){
		token = scan_pp();
		if(variable_names() == ERROR) return(ERROR);
		if(token != TCOLON) return(error_("Symbol ':' is not found"));
		token = scan_pp();
		if((Type = type()) == ERROR) return(ERROR);
	}
	record_types(Type);
	if(token != TRPAREN) return(error_("Symbol ')' is not found"));
	token = scan_pp();
	IN_FP--;
	return(NORMAL);
}


int compound_statement(void){
	if(token != TBEGIN) return(error_("Keyword 'begin' is not found"));
	IN_BEGIN++; /* for indent */
	if(SUBPRO_DEC && IN_BEGIN==1){
		Label_procedure(PROCEDURE_NAME);
		if(FPNUM) POP(gr2);
		int i;
		char fp_name[MAXSTRSIZE];
		for(i=FPNUM-1;i>=0;i--){ /* if procedure, assign formal parameters. */
			POP(gr1);
			sprintf(fp_name, "$%s%%%s", FP_NAMES[i], PROCEDURE_NAME);
			ST(gr1, fp_name, NULL);
		}
		if(FPNUM) PUSH("0",gr2);
	}

	token = scan_pp();
	if(statement() == ERROR) return(ERROR);
	while(token == TSEMI){
		token = scan_pp();
		if(statement() == ERROR) return(ERROR);
	}
	if(token != TEND) return(error_("Keyword 'end' is not found"));
	IN_BEGIN--; /* for indent */
	if(SUBPRO_DEC && !IN_BEGIN) RET();
	token = scan_pp();
	return(NORMAL);
}

int statement(void){
	switch(token){
	case TNAME:
		if(assignment_statement() == ERROR) return(ERROR);
		break;
	case TIF:
		if(condition_statement() == ERROR) return(ERROR);
		break;
	case TWHILE:
		if(iteration_statement() == ERROR) return(ERROR);
		break;
	case TBREAK:
		if(exit_statement() == ERROR) return(ERROR);
		break;
	case TCALL:
		if(call_statement() == ERROR) return(ERROR);
		break;
	case TRETURN:
		if(return_statement() == ERROR) return(ERROR);
		break;
	case TREAD:
		if(input_statement() == ERROR) return(ERROR);
		break;
	case TREADLN:
		if(input_statement() == ERROR) return(ERROR);
		break;
	case TWRITE:
		if(output_statement() == ERROR) return(ERROR);
		break;
	case TWRITELN:
		if(output_statement() == ERROR) return(ERROR);
		break;
	case TBEGIN:
		if(compound_statement() == ERROR) return(ERROR);
		break;
	default:
		if(empty_statement() == ERROR) return(ERROR);
		break;
	}
	return(NORMAL);
}

int condition_statement(void){
	int Type;
	if(token != TIF) return(error_("Keyword 'if' is not found"));
	IN_IF++; /* for indent */
	token = scan_pp();
	if((Type = expression()) == ERROR) return(ERROR);
	if(Type != TPBOOL) return(error_("if statement must be boolean."));
	if(token != TTHEN) return(error_("Keyword 'then' is not found"));
	token = scan_pp();
	IN_IF--; /* for indent */
	if(statement() == ERROR) return(ERROR);
	if(token == TELSE){
		IN_ELSE++; /* for indent */
		token = scan_pp();
		if(statement() == ERROR) return(ERROR);
		IN_ELSE--; /* for indent */
	}
	return(NORMAL);
}

int iteration_statement(void){
	int Type;
	if(token != TWHILE) return(error_("Keyword 'while' is not found"));
	print_label(newlabel());
	token = scan_pp();
	if((Type = expression()) == ERROR) return(ERROR);
	if(Type != TPBOOL) return(error_("while statement must be boolean."));
	if(token != TDO) return(error_("Keyword 'do' is not found"));
	token = scan_pp();
	if(statement() == ERROR) return(ERROR);
	return(NORMAL);
}

int exit_statement(void){
	if(token != TBREAK) return(error_("Keyword 'break' is not found"));
	token = scan_pp();
	return(NORMAL);
}

int chaek_formal_parameters_and_expressions(){
	struct FP *fp, *ex;
	fp = procedure->fp;
	ex = expressions();
	for(fp = procedure->fp;fp!=NULL;fp = fp->nextp){
		if(ex == NULL) {printf("\n Line:%d / ERROR: Lack of arguments. %s needs %d arguments.", get_linenum(), procedure->name, procedure->fpnum); return(ERROR);}
		else if(fp->fptype != ex->fptype) return(error_("Types between formal_parameters and expressions are different."));
		else if(fp->fparraysize != ex->fparraysize) return(error_("ArrayTypes between formal_parameters and expressions are different."));
		else ex = ex->nextp;
	}
	if(ex != NULL) {printf("\n Line:%d / ERROR: Too many argument is given. %s needs %d arguments.", get_linenum(), procedure->name, procedure->fpnum); return(ERROR);}
	return(NORMAL);
}

int call_statement(void){
	if(token != TCALL) return(error_("Keyword 'call' is not found"));
	token = scan_pp();
	procedure = id_record(string_attr);
	if(strcmp(procedure->name,PROCEDURE_NAME)==0){	/* if Recursive call */
		return(error_("Cannot Recursive call in MPPL."));
	}
	if(procedure_name() == ERROR) return(ERROR);
	if(token == TLPAREN){
		token = scan_pp();
		if(chaek_formal_parameters_and_expressions()==ERROR) return(ERROR);
		if(token != TRPAREN) return(error_("Keyword ')' is not found"));
		token = scan_pp();
	}
	return(NORMAL);
}


struct FP *expressions(void){
	if((root_fp = (struct FP *)malloc(sizeof(struct FP))) == NULL) {	/* allocate for root_fp */
		printf("can not malloc FP in expressions\n");
		exit(0);
	}
	if((root_fp->fptype = expression()) == ERROR) error_("Head of expressions does't have type.");
	struct FP *p, *tail;
	tail = root_fp;
	while(token == TCOMMA){
		if((p = (struct FP *)malloc(sizeof(struct FP))) == NULL) {	/* allocate for FP */
			printf("can not malloc FP in expressions\n");
			exit(0);
		}
		token = scan_pp();
		if((p->fptype = expression()) == ERROR) error_("One of expressions does't have type.");
		tail->nextp = p;
		tail = tail->nextp;
	}
	return(root_fp);
}

int return_statement(void){
	if(token != TRETURN) return(error_("Keyword 'return' is not found"));
	token = scan_pp();
	return(NORMAL);
}

int assignment_statement(void){
	int LType, RType;
	IN_ASSIGN++;
	if((LType = left_part()) == ERROR) return(ERROR);
	if(token != TASSIGN) return(error_("Symbol ':=' is not found"));
	token = scan_pp();
	if((RType = expression()) == ERROR) return(ERROR);
	if(LType != RType) return(error_("Left part and Right part must be same types in assignment_statement."));
	POP(gr2); /* POP left_part from STACK.(by left_part()) */
	ST(gr1, "0", gr2); /* gr1 -> gr2 */
	IN_ASSIGN--;
	return(NORMAL);
}

int left_part(void){
	int Type;
	IN_LEFTPART++;
	Type = variable();
	PUSH("0", gr1); /* gr1 -> STACK */
	/* left_part is top of stack. */
	IN_LEFTPART--;
	return Type;
}

int variable(void){
	int Type;
	int ArrayType;
	if((Type = variable_name()) == ERROR) return(ERROR);
	if(token == TLSQPAREN){
		ArrayType = TPARRAY + Type;
		if(Type != TPARRAY) return(error_("[arraysize] comes only after array type."));
		token = scan_pp();
		if((Type = expression()) == ERROR) return(ERROR);
		if(Type != TPINT) return(error_("[arraysize] must be integer."));
		if(token != TRSQPAREN) return(error_("Symbol ']' is not found"));
		token = scan_pp();
		Type = ArrayType;
	}else if(IN_LEFTPART) LD(gr1,LATESTLABEL);
	return(Type);
}

int expression(void){
	int Type;
	if((Type = simple_expression()) == ERROR) return(ERROR);
	while(token == TEQUAL || token == TNOTEQ || token == TLE || token == TLEEQ || token == TGR || token == TGREQ){
		Type = relational_operator();
		token = scan_pp();
		if(simple_expression() == ERROR) return(ERROR);
	}
	return(Type);
}

int simple_expression(void){
	int Type;
	int RType;
	int Mode;	/* TPINT:int , TPBOOL:bool*/
	int flag;
	int ope;
	flag=0;

	if(token == TPLUS){/* Constraint rule */
		token = scan_pp();
		flag = 1;
	}
	else if(token == TMINUS){/* Constraint rule */
		token = scan_pp();
		flag = -1;
	}

	if((Type = term()) == ERROR) return(ERROR);
	if(flag != 1 && Type != TPINT){/* Constraint rule */
		return(error_("the term after + or - must be integer."));
	}
	while(token == TPLUS || token == TMINUS || token == TOR){
		if(token == TPLUS || token == TMINUS) Mode = TPINT;
		else if(token == TOR) Mode = TPBOOL;
		ope = token;
		token = scan_pp();
		if((RType = term()) == ERROR) return(ERROR);
		if(Type != Mode || Mode != RType) return(error_("operator error."));
		POP(gr2);
		POP(gr1);
		if(flag == -1) MULA(gr1, "-1");
		if(ope == TPLUS) ADDA(gr1, gr2);
		else if(ope == TMINUS) SUBA(gr1, gr2);
		else if(ope == TOR) ADDL(gr1, gr2);
		JOV("EOVF", NULL);
		PUSH("0", gr1);
	}
	return(Type);
}

int term(void){
	int Type;
	int Mode;
	int RType;
	int ope;
	if((Type = factor()) == ERROR) return(ERROR);
	while(token == TSTAR || token == TDIV || token == TAND){
		Mode = multiplicative_operator();
		ope = token;
		token = scan_pp();
		if((RType = factor()) == ERROR) return(ERROR);
		if(Type != Mode || Mode != RType) return(error_("operator error."));

		POP(gr2);
		POP(gr1);
		if(ope == TSTAR) MULA(gr1, gr2);
		else if(ope == TDIV) DIVA(gr1, gr2);
		else if(ope == TAND) MULL(gr1, gr2);
		JOV("EOVF", NULL);
		PUSH("0", gr1);
	}
	return(Type);
}

int factor(void){	/* Irregular style*/
	int Type;
	int temp;
	if(token == TNAME){	/* variable */
		return variable();
	}
	else if(token == TNUMBER || token == TTRUE || token == TFALSE || token ==TSTRING){/* constant */
		return constant();
	}
	else if(token == TLPAREN){
		token = scan_pp();
		if((Type = expression()) == ERROR) return(ERROR);
		if(token != TRPAREN) return(error_("Symbol ')' is not found"));
		token = scan_pp();
		return(Type);
	}
	else if(token == TNOT){
		token = scan_pp();
		if((Type = factor()) == ERROR) return(ERROR);
		if(Type != TPBOOL) return(error_("not must have boolean value."));

		POP(gr1);
		XOR(gr1, "1");
		return(Type);
	}
	else if(token == TINTEGER || token == TBOOLEAN || token == TCHAR){
		Type = token - TYPEOFFSET;
		if(standerd_type() == ERROR) return(ERROR);
		if(token != TLPAREN) return(error_("Symbol '(' is not found"));
		token = scan_pp();
		if((temp = expression()) == ERROR) return(ERROR);
		if(temp!=TPINT && temp!=TPCHAR && temp!=TPBOOL) return(error_("standerd_type and expression_type must be same."));
		if(token != TRPAREN) return(error_("Symbol ')' is not found"));

		POP(gr1);
		if(Type == TPINT){
			if(temp == TPINT){

			}else if(temp == TPBOOL){
				CPA(gr1, gr0);
				JNZ(newlabel(), NULL);
				print_label(get_latestlabel());
				ST("1", gr1, NULL);
			}else if(temp == TPCHAR){
				XOR(gr1, "#FFFF");
				ADDA(gr1, "1");
			}
		}else if(Type == TPBOOL){
			if(temp == TPINT){
				CPA(gr1,gr0);
				JNZ(newlabel(), NULL);
				print_label(get_latestlabel());
				ST("1", gr1, NULL);
			}else if(Type == TPBOOL){

			}else if(Type == TPCHAR){
				CPA(gr1, gr0);
				JNZ(newlabel(), NULL);
				print_label(get_latestlabel());
				ST("1", gr1, NULL);
			}
		}else if(Type == TPCHAR){
			if(temp == TPINT){

			}else if(Type == TPBOOL){
				CPA(gr1,gr0);
				JNZ(newlabel(), NULL);
				print_label(get_latestlabel());
				ST("1", gr1, NULL);
			}else if(Type == TPCHAR){
			}
		}
		PUSH("0", gr1);

		token = scan_pp();
		return(Type);
	}
	else return(ERROR);
}

int constant(void){
	if(token == TNUMBER){
		if(num_attr == 0) LAD(gr1, "0", NULL);
		else LAD(gr1, LATESTLABEL, NULL);
		token = scan_pp();
		return(TPINT);
	}
	else if(token == TFALSE) {LD(gr1, "0"); token = scan_pp(); return(TPBOOL);}
	else if(token == TTRUE) { LD(gr1, "1"); token = scan_pp(); return(TPBOOL);}
	else if(token == TSTRING) {
		if(strlen(string_attr)!=1) return(error_("Constant char's length must be 1."));
		new_DC();
		token = scan_pp();
		return(TPCHAR);
	}
	else return(ERROR);
}

int multiplicative_operator(void){
	if(token == TSTAR) return(TPINT);
	else if(token == TDIV) return(TPINT);
	else if(token == TAND) return(TPBOOL);
	else return(ERROR);
}

int additive_operator(void){
	if(token == TPLUS) return(TPINT);
	else if(token == TMINUS) return(TPINT);
	else if(token == TOR) return(TPBOOL);
	else return(ERROR);
}

int relational_operator(void){
	if(token == TEQUAL) return(TPBOOL);
	else if(token == TNOTEQ) return(TPBOOL);
	else if(token == TLE) return(TPBOOL);
	else if(token == TLEEQ) return(TPBOOL);
	else if(token == TGR) return(TPBOOL);
	else if(token == TGREQ) return(TPBOOL);
	else return(ERROR);
}

int input_statement(void){
	int Type, isln;
	if(token == TREAD){token = scan_pp(); isln = 0;}
	else if(token == TREADLN){token = scan_pp(); isln = 1;}
	else return(error_("Keyword 'read' or 'readln' is not found"));
	if(token == TLPAREN){
		token = scan_pp();
		if((Type = variable()) == ERROR) return(ERROR);
		LD(gr1, LATESTLABEL);
		if(Type == TPINT) CALL("READINT", NULL);
		else if(Type == TPCHAR) CALL("READCHAR", NULL);
		else return(error_("variable in input_statement must be integer or char"));
		while(token == TCOMMA){
			token = scan_pp();
			if((Type = variable()) == ERROR) return(ERROR);
			LD(gr1, LATESTLABEL);
			if(Type == TPINT) CALL("READINT", NULL);
			else if(Type == TPCHAR) CALL("READCHAR", NULL);
			else return(error_("variable in input_statement must be integer or char"));
		}
		if(token != TRPAREN) return(error_("Symbol ')' is not found"));
		token = scan_pp();
	}
	if(isln) CALL("HEADLINE",NULL);
	return(NORMAL);
}

int output_statement(void){/* Irregular style*/
	int isln;
	if(token == TWRITE){
		token = scan_pp();
		isln = 0;
	}
	else if(token == TWRITELN){
		token = scan_pp();
		isln = 1;
	}
	else return(error_("Keyword 'write' or 'writeln' is not found"));
	if(token == TLPAREN){
		token = scan_pp();
		if(output_format() == ERROR) return(ERROR);
		while(token == TCOMMA){
			token = scan_pp();
			if(output_format() == ERROR) return(ERROR);
		}
		if(token != TRPAREN) return(error_("Symbol ')' is not found"));
		token = scan_pp();
	}
	if(isln) CALL("WRITELINE",NULL);
	return(NORMAL);
}

int output_format(void){
	int Type;
	if(token == TPLUS || token == TMINUS || token == TNAME ||
			token == TNUMBER || token == TFALSE || token == TTRUE || (token == TSTRING && strlen(string_attr)==1) ||
			token == TLPAREN || token == TNOT || token == TINTEGER || token == TBOOLEAN || token == TCHAR){
		Type = expression();

		if(token/* expression is only variable */) LD(gr1, LATESTLABEL);
		else POP(gr1);

		if(Type != TPINT && Type != TPCHAR && Type != TPBOOL) return(error_("expression in output_format must be standerd_type."));
		if(token == TCOLON){
			token = scan_pp();
			if(token != TNUMBER) return(error_("NUMBER is not found"));
			ST(string_attr,gr2, NULL);
			token = scan_pp();
		}else{
			/* ST(gr0, gr2, NULL); from where? */
			LD(gr2, gr0);
		}

		if(Type == TPINT) CALL("WRITEINT", NULL);
		else if(Type == TPCHAR) CALL("WRITECHAR", NULL);
		else if(Type == TPBOOL) CALL("WRITEBOOL", NULL);

		return(NORMAL);
	}
	else if(token == TSTRING){
		if(strlen(string_attr) == 1) return(error_("String length in output_format must be 0, or above 2"));
		new_DC();
		LAD(gr1, get_latestlabel(), NULL);
		LD(gr2, gr0);
		CALL("WRITESTR", NULL);
		token = scan_pp();
		return(NORMAL);
	}
	else return(ERROR);
}

int empty_statement(void){
	return(NORMAL);
}


