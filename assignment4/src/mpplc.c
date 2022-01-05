#include <stdio.h>
#include <stdlib.h>
#include "mpplc.h"

void init_output(void){
	if ((outfp = fopen("test.mpl", "w")) == NULL) {/* open file "test.mpl" for write. */
		printf("cannot open\n");	/* if can't open file */
		exit(1);      	/* exit */
	}
	fprintf(outfp, "Test output.\n");
}

void end_output(void){
	fclose(outfp);
}

void LD(char *a, char *b){fprintf(outfp,"\tLD\t%s, %s\n",a,b);}
void LD_ra(char *a, char *b, char *x){if(x==NULL) fprintf(outfp,"\tLD\t%s, %s\n",a,b);else fprintf(outfp,"\tLD\t%s, %s, %s\n",a,b,x);}
void ST(char *a, char *b){fprintf(outfp,"\tST\t%s, %s\n",a,b);}
void LAD(char *a, char *b){fprintf(outfp,"\tLAD\t%s, %s\n",a,b);}

void ADDA(char *a, char *b){fprintf(outfp,"\tADDA\t%s, %s\n",a,b);}
void ADDL(char *a, char *b){fprintf(outfp,"\tADDL\t%s, %s\n",a,b);}
void SUBA(char *a, char *b){fprintf(outfp,"\tSUBA\t%s, %s\n",a,b);}
void SUBL(char *a, char *b){fprintf(outfp,"\tSUBL\t%s, %s\n",a,b);}
void MULA(char *a, char *b){fprintf(outfp,"\tMULA\t%s, %s\n",a,b);}
void MULL(char *a, char *b){fprintf(outfp,"\tMULL\t%s, %s\n",a,b);}
void DIVA(char *a, char *b){fprintf(outfp,"\tDIVA\t%s, %s\n",a,b);}
void DIVL(char *a, char *b){fprintf(outfp,"\tDIVL\t%s, %s\n",a,b);}

void ADDA_ra(char *a, char *b, char *x){if(x==NULL) fprintf(outfp,"\tADDA\t%s, %s\n",a,b);else fprintf(outfp,"\tADDA\t%s, %s, %s\n",a,b,x);}
void ADDL_ra(char *a, char *b, char *x){if(x==NULL) fprintf(outfp,"\tADDL\t%s, %s\n",a,b);else fprintf(outfp,"\tADDL\t%s, %s, %s\n",a,b,x);}
void SUBA_ra(char *a, char *b, char *x){if(x==NULL) fprintf(outfp,"\tSUBA\t%s, %s\n",a,b);else fprintf(outfp,"\tSUBA\t%s, %s, %s\n",a,b,x);}
void SUBL_ra(char *a, char *b, char *x){if(x==NULL) fprintf(outfp,"\tSUBL\t%s, %s\n",a,b);else fprintf(outfp,"\tSUBL\t%s, %s, %s\n",a,b,x);}
void MULA_ra(char *a, char *b, char *x){if(x==NULL) fprintf(outfp,"\tMULA\t%s, %s\n",a,b);else fprintf(outfp,"\tMULA\t%s, %s, %s\n",a,b,x);}
void MULL_ra(char *a, char *b, char *x){if(x==NULL) fprintf(outfp,"\tMULL\t%s, %s\n",a,b);else fprintf(outfp,"\tMULL\t%s, %s, %s\n",a,b,x);}
void DIVA_ra(char *a, char *b, char *x){if(x==NULL) fprintf(outfp,"\tDIVA\t%s, %s\n",a,b);else fprintf(outfp,"\tDIVA\t%s, %s, %s\n",a,b,x);}
void DIVL_ra(char *a, char *b, char *x){if(x==NULL) fprintf(outfp,"\tDIVL\t%s, %s\n",a,b);else fprintf(outfp,"\tDIVL\t%s, %s, %s\n",a,b,x);}

void AND(char *a, char *b){fprintf(outfp,"\tAND\t%s, %s\n",a,b);}
void OR(char *a, char *b){fprintf(outfp,"\tOR\t%s, %s\n",a,b);}
void XOR(char *a, char *b){fprintf(outfp,"\tXOR\t%s, %s\n",a,b);}

void AND_ra(char *a, char *b, char *x){if(x==NULL) fprintf(outfp,"\tAND\t%s, %s\n",a,b);else fprintf(outfp,"\tAND\t%s, %s, %s\n",a,b,x);}
void OR_ra(char *a, char *b, char *x){if(x==NULL) fprintf(outfp,"\tOR\t%s, %s\n",a,b);else fprintf(outfp,"\tOR\t%s, %s, %s\n",a,b,x);}
void XOR_ra(char *a, char *b, char *x){if(x==NULL) fprintf(outfp,"\tXOR\t%s, %s\n",a,b);else fprintf(outfp,"\tXOR\t%s, %s, %s\n",a,b,x);}

void CPA(char *a, char *b){fprintf(outfp,"\tCPA\t%s, %s\n",a,b);}
void CPL(char *a, char *b){fprintf(outfp,"\tCPA\t%s, %s\n",a,b);}

void CPA_ra(char *a, char *b, char *x){if(x==NULL) fprintf(outfp,"\tCPA\t%s, %s\n",a,b);else fprintf(outfp,"\tCPA\t%s, %s, %s\n",a,b,x);}
void CPL_ra(char *a, char *b, char *x){if(x==NULL) fprintf(outfp,"\tCPL\t%s, %s\n",a,b);else fprintf(outfp,"\tCPL\t%s, %s, %s\n",a,b,x);}

void SLA(char *a, char *b, char *x){if(x==NULL) fprintf(outfp,"\tSLA\t%s, %s\n",a,b);else fprintf(outfp,"\tSLA\t%s, %s, %s\n",a,b,x);}
void SRA(char *a, char *b, char *x){if(x==NULL) fprintf(outfp,"\tSRA\t%s, %s\n",a,b);else fprintf(outfp,"\tSRA\t%s, %s, %s\n",a,b,x);}
void SLL(char *a, char *b, char *x){if(x==NULL) fprintf(outfp,"\tSLL\t%s, %s\n",a,b);else fprintf(outfp,"\tSLL\t%s, %s, %s\n",a,b,x);}
void SRL(char *a, char *b, char *x){if(x==NULL) fprintf(outfp,"\tSRL\t%s, %s\n",a,b);else fprintf(outfp,"\tSRL\t%s, %s, %s\n",a,b,x);}

void JPL(char *a, char *x){if(x==NULL) fprintf(outfp,"\tJPL\t%s\n",a);else fprintf(outfp,"\tJPL\t%s, %s\n",a,x);}
void JMI(char *a, char *x){if(x==NULL) fprintf(outfp,"\tJMI\t%s\n",a);else fprintf(outfp,"\tJMI\t%s, %s\n",a,x);}
void JNZ(char *a, char *x){if(x==NULL) fprintf(outfp,"\tJNZ\t%s\n",a);else fprintf(outfp,"\tJNZ\t%s, %s\n",a,x);}
void JZE(char *a, char *x){if(x==NULL) fprintf(outfp,"\tJZE\t%s\n",a);else fprintf(outfp,"\tJZE\t%s, %s\n",a,x);}
void JOV(char *a, char *x){if(x==NULL) fprintf(outfp,"\tJOV\t%s\n",a);else fprintf(outfp,"\tJOV\t%s, %s\n",a,x);}
void JUMP(char *a, char *x){if(x==NULL) fprintf(outfp,"\tJUMP\t%s\n",a);else fprintf(outfp,"\tJUMP\t%s, %s\n",a,x);}

void PUSH(char *a, char *x){if(x==NULL) fprintf(outfp,"\tPUSH\t%s\n",a);else fprintf(outfp,"\tPUSH\t%s, %s\n",a,x);}
void POP(char *a){fprintf(outfp,"\tPUSH\t%s\n",a);}




