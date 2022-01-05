#define gr0 "gr0"
#define gr1 "gr1"
#define gr2 "gr2"
#define gr3 "gr3"
#define gr4 "gr4"
#define gr5 "gr5"
#define gr6 "gr6"
#define gr7 "gr7"

extern FILE *outfp;

void init_output(void);
void end_output(void);
void start_mpl(char* program_name);

void LD(char *a, char *b);
void LD_ra(char *a, char *b, char *x);
void ST(char *a, char *b);
void LAD(char *a, char *b, char *x);

void ADDA(char *a, char *b);
void ADDL(char *a, char *b);
void SUBA(char *a, char *b);
void SUBL(char *a, char *b);
void MULA(char *a, char *b);
void MULL(char *a, char *b);
void DIVA(char *a, char *b);
void DIVL(char *a, char *b);

void ADDA_ra(char *a, char *b, char *x);
void ADDL_ra(char *a, char *b, char *x);
void SUBA_ra(char *a, char *b, char *x);
void SUBL_ra(char *a, char *b, char *x);
void MULA_ra(char *a, char *b, char *x);
void MULL_ra(char *a, char *b, char *x);
void DIVA_ra(char *a, char *b, char *x);
void DIVL_ra(char *a, char *b, char *x);

void AND(char *a, char *b);
void OR(char *a, char *b);
void XOR(char *a, char *b);

void AND_ra(char *a, char *b, char *x);
void OR_ra(char *a, char *b, char *x);
void XOR_ra(char *a, char *b, char *x);

void CPA(char *a, char *b);
void CPL(char *a, char *b);

void CPA_ra(char *a, char *b, char *x);
void CPL_ra(char *a, char *b, char *x);

void SLA(char *a, char *b, char *x);
void SRA(char *a, char *b, char *x);
void SLL(char *a, char *b, char *x);
void SRL(char *a, char *b, char *x);

void JPL(char *a, char *x);
void JMI(char *a, char *x);
void JNZ(char *a, char *x);
void JZE(char *a, char *x);
void JOV(char *a, char *x);
void JUMP(char *a, char *x);

void PUSH(char *a, char *x);
void POP(char *a);

void CALL(char *a, char *x);
void RET(void);

void SVC(char *a, char *x);
void NOP(void);

