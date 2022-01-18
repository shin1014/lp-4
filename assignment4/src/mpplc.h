#define gr0 "gr0"
#define gr1 "gr1"
#define gr2 "gr2"
#define gr3 "gr3"
#define gr4 "gr4"
#define gr5 "gr5"
#define gr6 "gr6"
#define gr7 "gr7"

extern FILE *outfp;
extern int LNUM;

extern struct DC DC_root;
extern struct DC *DC_tail;

struct DC {
	char *label;
	char *value;
	struct DC *nextp;
};

void init_output(void);
void init_DC_list(void);
void end_output(void);
void start_mpl(char* program_name);
char* newlabel(void);
void new_DC(void);
char *get_latestlabel(void);
void print_DCs(void);
void print_label(char *label);
char *make_procedure_label(char *procedure_name);

void LD(char *a, char *b);
void LD_ra(char *a, char *b, char *x);
void ST(char *a, char *b, char *x);
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

void START(char* program_name);
void END(void);
void DS(char *label, char *value);
void DC(char *label, char *value);
void IN(char *a, char *b);
void OUT(char *a,char *b);
void RPOP(void);
void RPUSH(void);

void Label(char *label);
void Label_procedure(char *label);
void Label_DS(char *label, char *value);
void Label_DC(char *label, char *value);

void WRITECHAR(void);
void WRITESTR(void);
void WRITEINT(void);
void WRITEBOOL(void);
void WRITELINE(void);
void FLUSH(void);

void READCHAR(void);
void READINT(void);
void READLINE(void);
void CONSTANTS(void);

void OTHER_CSL(void);



