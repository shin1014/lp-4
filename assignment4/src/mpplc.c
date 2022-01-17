#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "mpplc.h"
#include "token-list.h"

#define LABELSIZE 6



struct DC DC_root;
struct DC *DC_tail;
struct DC *DC_pretail;

void init_output(void){
	if ((outfp = fopen("test.mpl", "w")) == NULL) {/* open file "test.mpl" for write. */
		printf("cannot open\n");	/* if can't open file */
		exit(1);      	/* exit */
	}
}

void init_DC_list(void){
	DC_tail = &DC_root;
	DC_pretail = NULL;
}

void end_output(void){
	fclose(outfp);
}

void start_mpl(char* program_name){
	START(program_name);
	LAD(gr0,"0",NULL);
	CALL(newlabel(),NULL);
	CALL("FLUSH",NULL);
	SVC("0",NULL);
}

char* newlabel(void){
	char *label;
	label = (char*)malloc(sizeof(char)*LABELSIZE);
	sprintf(label, "L%04d", LNUM);
	LNUM++;
	return label;
}

void new_DC(void){
	char *value;
	struct DC *DC_node;
	value = (char*)malloc(sizeof(char)*(strlen(string_attr)+strlen("''")));
	DC_node = (struct DC*)malloc(sizeof(DC));
	if(token == TSTRING) sprintf(value, "'%s'", string_attr);
	else sprintf(value, "%s", string_attr);
	DC_node->label = newlabel();
	DC_node->value = value;
	DC_tail->nextp = DC_node;
	DC_pretail = DC_tail;
	DC_tail = DC_node;
}

void print_DCs(void){
	struct DC *p;
	p = DC_root.nextp;
	while(p!=NULL){
		Label_DC(p->label, p->value);
		p = p->nextp;
	}
}

char *get_latestlabel(void){
	return DC_tail->label;
}

void print_label(char *label){
	fprintf(outfp, "%s\n", label);
}

void LD(char *a, char *b){fprintf(outfp,"\tLD\t%s,\t%s\n",a,b);}
void LD_ra(char *a, char *b, char *x){if(x==NULL) fprintf(outfp,"\tLD\t%s,\t%s\n",a,b);else fprintf(outfp,"\tLD\t%s,\t%s,\t%s\n",a,b,x);}
void ST(char *a, char *b, char *x){if(x==NULL) fprintf(outfp,"\tST\t%s,\t%s\n",a,b);else fprintf(outfp,"\tST\t%s,%s,%s\n",a,b,x);}
void LAD(char *a, char *b, char *x){if(x==NULL) fprintf(outfp,"\tLAD\t%s,\t%s\n",a,b);else fprintf(outfp,"\tLAD\t%s,%s,%s\n",a,b,x);}

void ADDA(char *a, char *b){fprintf(outfp,"\tADDA\t%s,%s\n",a,b);}
void ADDL(char *a, char *b){fprintf(outfp,"\tADDL\t%s,%s\n",a,b);}
void SUBA(char *a, char *b){fprintf(outfp,"\tSUBA\t%s,%s\n",a,b);}
void SUBL(char *a, char *b){fprintf(outfp,"\tSUBL\t%s,%s\n",a,b);}
void MULA(char *a, char *b){fprintf(outfp,"\tMULA\t%s,%s\n",a,b);}
void MULL(char *a, char *b){fprintf(outfp,"\tMULL\t%s,%s\n",a,b);}
void DIVA(char *a, char *b){fprintf(outfp,"\tDIVA\t%s,%s\n",a,b);}
void DIVL(char *a, char *b){fprintf(outfp,"\tDIVL\t%s,%s\n",a,b);}

void ADDA_ra(char *a, char *b, char *x){if(x==NULL) fprintf(outfp,"\tADDA\t%s,%s\n",a,b);else fprintf(outfp,"\tADDA\t%s,%s,%s\n",a,b,x);}
void ADDL_ra(char *a, char *b, char *x){if(x==NULL) fprintf(outfp,"\tADDL\t%s,%s\n",a,b);else fprintf(outfp,"\tADDL\t%s,%s,%s\n",a,b,x);}
void SUBA_ra(char *a, char *b, char *x){if(x==NULL) fprintf(outfp,"\tSUBA\t%s,%s\n",a,b);else fprintf(outfp,"\tSUBA\t%s,%s,%s\n",a,b,x);}
void SUBL_ra(char *a, char *b, char *x){if(x==NULL) fprintf(outfp,"\tSUBL\t%s,%s\n",a,b);else fprintf(outfp,"\tSUBL\t%s,%s,%s\n",a,b,x);}
void MULA_ra(char *a, char *b, char *x){if(x==NULL) fprintf(outfp,"\tMULA\t%s,%s\n",a,b);else fprintf(outfp,"\tMULA\t%s,%s,%s\n",a,b,x);}
void MULL_ra(char *a, char *b, char *x){if(x==NULL) fprintf(outfp,"\tMULL\t%s,%s\n",a,b);else fprintf(outfp,"\tMULL\t%s,%s,%s\n",a,b,x);}
void DIVA_ra(char *a, char *b, char *x){if(x==NULL) fprintf(outfp,"\tDIVA\t%s,%s\n",a,b);else fprintf(outfp,"\tDIVA\t%s,%s,%s\n",a,b,x);}
void DIVL_ra(char *a, char *b, char *x){if(x==NULL) fprintf(outfp,"\tDIVL\t%s,%s\n",a,b);else fprintf(outfp,"\tDIVL\t%s,%s,%s\n",a,b,x);}

void AND(char *a, char *b){fprintf(outfp,"\tAND\t%s,%s\n",a,b);}
void OR(char *a, char *b){fprintf(outfp,"\tOR\t%s,%s\n",a,b);}
void XOR(char *a, char *b){fprintf(outfp,"\tXOR\t%s,%s\n",a,b);}

void AND_ra(char *a, char *b, char *x){if(x==NULL) fprintf(outfp,"\tAND\t%s,%s\n",a,b);else fprintf(outfp,"\tAND\t%s,%s,%s\n",a,b,x);}
void OR_ra(char *a, char *b, char *x){if(x==NULL) fprintf(outfp,"\tOR\t%s,%s\n",a,b);else fprintf(outfp,"\tOR\t%s,%s,%s\n",a,b,x);}
void XOR_ra(char *a, char *b, char *x){if(x==NULL) fprintf(outfp,"\tXOR\t%s,%s\n",a,b);else fprintf(outfp,"\tXOR\t%s,%s,%s\n",a,b,x);}

void CPA(char *a, char *b){fprintf(outfp,"\tCPA\t%s,%s\n",a,b);}
void CPL(char *a, char *b){fprintf(outfp,"\tCPA\t%s,%s\n",a,b);}

void CPA_ra(char *a, char *b, char *x){if(x==NULL) fprintf(outfp,"\tCPA\t%s,%s\n",a,b);else fprintf(outfp,"\tCPA\t%s,%s,%s\n",a,b,x);}
void CPL_ra(char *a, char *b, char *x){if(x==NULL) fprintf(outfp,"\tCPL\t%s,%s\n",a,b);else fprintf(outfp,"\tCPL\t%s,%s,%s\n",a,b,x);}

void SLA(char *a, char *b, char *x){if(x==NULL) fprintf(outfp,"\tSLA\t%s,%s\n",a,b);else fprintf(outfp,"\tSLA\t%s,%s,%s\n",a,b,x);}
void SRA(char *a, char *b, char *x){if(x==NULL) fprintf(outfp,"\tSRA\t%s,%s\n",a,b);else fprintf(outfp,"\tSRA\t%s,%s,%s\n",a,b,x);}
void SLL(char *a, char *b, char *x){if(x==NULL) fprintf(outfp,"\tSLL\t%s,%s\n",a,b);else fprintf(outfp,"\tSLL\t%s,%s,%s\n",a,b,x);}
void SRL(char *a, char *b, char *x){if(x==NULL) fprintf(outfp,"\tSRL\t%s,%s\n",a,b);else fprintf(outfp,"\tSRL\t%s,%s,%s\n",a,b,x);}

void JPL(char *a, char *x){if(x==NULL) fprintf(outfp,"\tJPL\t%s\n",a);else fprintf(outfp,"\tJPL\t%s,%s\n",a,x);}
void JMI(char *a, char *x){if(x==NULL) fprintf(outfp,"\tJMI\t%s\n",a);else fprintf(outfp,"\tJMI\t%s,%s\n",a,x);}
void JNZ(char *a, char *x){if(x==NULL) fprintf(outfp,"\tJNZ\t%s\n",a);else fprintf(outfp,"\tJNZ\t%s,%s\n",a,x);}
void JZE(char *a, char *x){if(x==NULL) fprintf(outfp,"\tJZE\t%s\n",a);else fprintf(outfp,"\tJZE\t%s,%s\n",a,x);}
void JOV(char *a, char *x){if(x==NULL) fprintf(outfp,"\tJOV\t%s\n",a);else fprintf(outfp,"\tJOV\t%s,%s\n",a,x);}
void JUMP(char *a, char *x){if(x==NULL) fprintf(outfp,"\tJUMP\t%s\n",a);else fprintf(outfp,"\tJUMP\t%s,%s\n",a,x);}

void PUSH(char *a, char *x){if(x==NULL) fprintf(outfp,"\tPUSH\t%s\n",a);else fprintf(outfp,"\tPUSH\t%s,%s\n",a,x);}
void POP(char *a){fprintf(outfp,"\tPOP\t%s\n",a);}

void CALL(char *a, char *x){if(x==NULL) fprintf(outfp,"\tCALL\t%s\n",a);else fprintf(outfp,"\tCALL\t%s,%s\n",a,x);}
void RET(void){fprintf(outfp,"\tRET\n");}

void SVC(char *a, char *x){if(x==NULL) fprintf(outfp,"\tSVC\t%s\n",a);else fprintf(outfp,"\tSVC\t%s,%s\n",a,x);}
void NOP(void){fprintf(outfp,"\tNOP\n");}

void START(char* program_name){fprintf(outfp, "$$%s\tSTART\n", program_name);}
void END(void){fprintf(outfp, "\tEND\n");}
void DS(char *label, char *value){fprintf(outfp, "%s\tDS\t%s\n",label,value);}
void DC(char *label, char *value){fprintf(outfp, "%s\tDC\t%s\n",label,value);}
void IN(char *a, char *b){fprintf(outfp,"\tIN\t%s,%s\n", a, b);}
void OUT(char *a,char *b){fprintf(outfp,"\tOUT\t%s,%s\n",a,b);}
void RPOP(void){fprintf(outfp,"\tRPOP\n");}
void RPUSH(void){fprintf(outfp,"\tRPUSH\n");}

void Label(char *label){fprintf(outfp, "%s\n", label);}
void Label_procedure(char *label){fprintf(outfp, "$%s\n", label);}
void Label_DS(char *label, char *value){fprintf(outfp, "%s\tDS\t%s\n", label, value);}
void Label_DC(char *label, char *value){fprintf(outfp, "%s\tDC\t%s\n", label, value);}

void EOVF(void){
  Label("EOVF");
  CALL("WRITELINE",NULL);
  LAD(gr1,"EOVF1",NULL);
  LD(gr2,gr0);
  CALL("WRITESTR",NULL);
  CALL("WRITELINE",NULL);
  SVC("1",NULL);
  Label_DC("EOVF1", "\'***** Run-Time Error : Overflow *****\'");
}
void E0DIV(void){
  Label("E0DIV");
  JNZ("EOVF",NULL);
  CALL("WRITELINE",NULL);
  LAD(gr1, "E0DIV1",NULL);
  LD(gr2, gr0);
  CALL("WRITESTR",NULL);
  CALL("WRITELINE",NULL);
  SVC("2",NULL);
  Label_DC("E0DIV1","\'***** Run-Time Error : Zero-Divide *****\'");
}
void EROV(void){
  Label("EROV");
  CALL("WRITELINE",NULL);
  LAD(gr1, "EROV1",NULL);
  LD(gr2, gr0);
  CALL("WRITESTR",NULL);
  CALL("WRITELINE",NULL);
  SVC("3",NULL);
  Label_DC("EROV1", "\'***** Run-Time Error : Range-Over in Array Index *****\'");
}

void WRITECHAR(void){
  Label("WRITECHAR");
  RPUSH();
  LD_ra(gr6, "SPACE",NULL);
  LD_ra(gr7, "OBUFSIZE",NULL);
  Label("WC1");
  SUBA_ra(gr2, "ONE",NULL);
  JZE("WC2",NULL);
  JMI("WC2",NULL);
  ST(gr6, "OBUF",gr7);
  CALL("BOVFCHECK",NULL);
  JUMP("WC1",NULL);
  Label("WC2");
  ST(gr1, "OBUF",gr7);
  CALL("BOVFCHECK",NULL);
  ST(gr7, "OBUFSIZE",NULL);
  RPOP();
  RET();
}

void WRITESTR(void){
  Label("WRITESTR");
  RPUSH();
  LD(gr6, gr1);
  Label("WS1");
  LD_ra(gr4, "0",gr6);
  JZE("WS2",NULL);
  ADDA_ra(gr6, "ONE",NULL);
  SUBA_ra(gr2, "ONE",NULL);
  JUMP("WS1",NULL);
  Label("WS2");
  LD_ra(gr7, "OBUFSIZE",NULL);
  LD_ra(gr5, "SPACE",NULL);
  Label("WS3");
  SUBA_ra(gr2, "ONE",NULL);
  JMI("WS4",NULL);
  ST(gr5, "OBUF",gr7);
  CALL("BOVFCHECK",NULL);
  JUMP("WS3",NULL);
  Label("WS4");
  LD_ra(gr4, "0",gr1);
  JZE("WS5",NULL);
  ST(gr4, "OBUF",gr7);
  ADDA_ra(gr1, "ONE",NULL);
  CALL("BOVFCHECK",NULL);
  JUMP("WS4",NULL);
  Label("WS5");
  ST(gr7, "OBUFSIZE",NULL);
  RPOP();
  RET();
  Label("BOVFCHECK");
  ADDA_ra(gr7, "ONE",NULL);
  CPA_ra(gr7, "BOVFLEVEL",NULL);
  JMI("BOVF1",NULL);
  CALL("WRITELINE",NULL);
  LD_ra(gr7, "OBUFSIZE",NULL);
  Label("BOVF1");
  RET();
  Label_DC("BOVFLEVEL","256");
}

void WRITEINT(void){
  Label("WRITEINT");
  RPUSH();
  LD(gr7, gr0);
  CPA(gr1, gr0);
  JPL("WI1", NULL);
  JZE("WI1", NULL);
  LD(gr4, gr0 );
  SUBA(gr4, gr1);
  CPA(gr4, gr1);
  JZE("WI6",NULL);
  LD(gr1, gr4);
  LD_ra(gr7, "ONE",NULL);
  Label("WI1");
  LD_ra(gr6, "SIX",NULL);
  ST(gr0, "INTBUF",gr6);
  SUBA_ra(gr6, "ONE",NULL);
  CPA(gr1, gr0);
  JNZ("WI2",NULL);
  LD_ra(gr4, "ZERO",NULL);
  ST(gr4, "INTBUF",gr6);
  JUMP("WI5",NULL);
  Label("WI2");
  CPA(gr1, gr0);
  JZE("WI3",NULL);
  LD(gr5, gr1);
  DIVA_ra(gr1, "TEN",NULL);
  LD(gr4, gr1);
  MULA_ra(gr4, "TEN",NULL);
  SUBA(gr5, gr4);
  ADDA_ra(gr5, "ZERO",NULL);
  ST(gr5, "INTBUF",gr6);
  SUBA_ra(gr6, "ONE",NULL);
  JUMP("WI2",NULL);
  Label("WI3");
  CPA(gr7, gr0);
  JZE("WI4",NULL);
  LD_ra(gr4, "MINUS",NULL);
  ST(gr4, "INTBUF",gr6);
  JUMP("WI5",NULL);
  Label("WI4");
  ADDA_ra(gr6, "ONE",NULL);
  Label("WI5");
  LAD(gr1, "INTBUF",gr6);
  CALL("WRITESTR",NULL);
  RPOP();
  RET();
  Label("WI6");
  LAD(gr1, "MMINT",NULL);
  CALL("WRITESTR",NULL);
  RPOP();
  RET();
  Label_DC("MMINT","-32768");
}

void WRITEBOOL(void){
  Label("WRITEBOOL");
  RPUSH();
  CPA(gr1, gr0);
  JZE("WB1",NULL);
  LAD(gr1, "WBTRUE",NULL);
  JUMP("WB2",NULL);
  Label("WB1");
  LAD(gr1, "WBFALSE",NULL);
  Label("WB2");
  CALL("WRITESTR",NULL);
  RPOP();
  RET();
  Label_DC("WBTRUE","'TRUE'");
  Label_DC("WBFALSE","'FALSE'");
}

void WRITELINE(void){
  Label("WRITELINE");
  RPUSH();
  LD_ra(gr7, "OBUFSIZE",NULL);
  LD_ra(gr6, "NEWLINE",NULL);
  ST(gr6, "OBUF",gr7);
  ADDA_ra(gr7, "ONE",NULL);
  ST(gr7, "OBUFSIZE",NULL);
  OUT("OBUF", "OBUFSIZE");
  ST(gr0, "OBUFSIZE",NULL);
  RPOP();
  RET();
}

void FLUSH(void){
  Label("FLUSH");
  RPUSH();
  LD_ra(gr7, "OBUFSIZE",NULL);
  JZE("FL1",NULL);
  CALL("WRITELINE",NULL);
  Label("FL1");
  RPOP();
  RET();
}

void READCHAR(void){
  Label("READCHAR");
  RPUSH();
  LD_ra(gr5, "RPBBUF",NULL);
  JZE("RC0",NULL);
  ST(gr5, "0",gr1);
  ST(gr0, "RPBBUF",NULL);
  JUMP("RC3",NULL);
  Label("RC0");
  LD_ra(gr7, "INP",NULL);
  LD_ra(gr6, "IBUFSIZE",NULL);
  JNZ("RC1",NULL);
  IN("IBUF", "IBUFSIZE");
  LD(gr7, gr0);
  Label("RC1");
  CPA_ra(gr7, "IBUFSIZE",NULL);
  JNZ("RC2",NULL);
  LD_ra(gr5, "NEWLINE",NULL);
  ST(gr5, "0",gr1);
  ST(gr0, "IBUFSIZE",NULL);
  ST(gr0, "INP",NULL);
  JUMP("RC3",NULL);
  Label("RC2");
  LD_ra(gr5, "IBUF",gr7);
  ADDA_ra(gr7, "ONE",NULL);
  ST(gr5, "0",gr1);
  ST(gr7, "INP",NULL);
  Label("RC3");
  RPOP();
  RET();
}

void READINT(void){
  Label("READINT");
  RPUSH();
  Label("RI1");
  CALL("READCHAR",NULL);
  LD_ra(gr7, "0",gr1);
  CPA_ra(gr7, "SPACE",NULL);
  JZE("RI1",NULL);
  CPA_ra(gr7, "TAB",NULL);
  JZE("RI1",NULL);
  CPA_ra(gr7, "NEWLINE",NULL);
  JZE("RI1",NULL);
  LD_ra(gr5, "ONE",NULL);
  CPA_ra(gr7, "MINUS",NULL);
  JNZ("RI4",NULL);
  LD_ra(gr5, gr0,NULL);
  CALL("READCHAR",NULL);
  LD_ra(gr7, "0",gr1);
  Label("RI4");
  LD_ra(gr6, gr0 ,NULL);
  Label("RI2");
  CPA_ra(gr7, "ZERO",NULL);
  JMI("RI3",NULL);
  CPA_ra(gr7, "NINE",NULL);
  JPL("RI3",NULL);
  MULA_ra(gr6, "TEN",NULL);
  ADDA(gr6, gr7);
  SUBA_ra(gr6, "ZERO",NULL);
  CALL("READCHAR",NULL);
  LD_ra(gr7, "0",gr1);
  JUMP("RI2",NULL);
  Label("RI3");
  ST(gr7, "RPBBUF",NULL);
  ST(gr6, "0",gr1);
  CPA(gr5, gr0);
  JNZ("RI5",NULL);
  SUBA(gr5, gr6);
  ST(gr5, "0",gr1);
  Label("RI5");
  RPOP();
  RET();
}

void READLINE(void){
  Label("READLINE");
  ST(gr0, "IBUFSIZE",NULL);
  ST(gr0, "INP",NULL);
  ST(gr0, "RPBBUF",NULL);
  RET();
}

void CONSTANTS(void){
  Label_DC("ONE","1");
  Label_DC("SIX","6");
  Label_DC("TEN","10");
  Label_DC("SPACE","#0020");
  Label_DC("MINUS","#002D");
  Label_DC("TAB","#0009");
  Label_DC("ZERO","#0030");
  Label_DC("NINE","#0039");
  Label_DC("NEWLINE","#000A");
  Label_DS("INTBUF","8");
  Label_DC("OBUFSIZE","0");
  Label_DC("IBUFSIZE","0");
  Label_DC("INP", "0");
  Label_DS("OBUF","257");
  Label_DS("IBUF","257");
  Label_DC("RPBBUF","0");
  fprintf(outfp,"\tEND\n");
}

void OTHER_CSL(void){
  EOVF();
  E0DIV();
  EROV();
  WRITECHAR();
  WRITESTR();
  WRITEINT();
  WRITEBOOL();
  WRITELINE();
  FLUSH();
  READCHAR();
  READINT();
  READLINE();
  CONSTANTS();
}






