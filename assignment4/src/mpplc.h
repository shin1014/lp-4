extern FILE *outfp;

void init_output(void);
void end_output(void);

void LD(char *a, char *b);
void ST(char *a, char *b);
void LAD(char *a, char *b);

void ADDA(char *a, char *b);
void ADDL(char *a, char *b);
void SUBA(char *a, char *b);
void SUBL(char *a, char *b);
void MULA(char *a, char *b);
void MULL(char *a, char *b);
void DIVA(char *a, char *b);
void DIVL(char *a, char *b);
