/* test commit, check diff */

#include "token-list.h"
#include "id-list.h"

extern int IN_FP;
extern int IN_VAR;
extern int IS_PROCNAME;
extern char PROCEDURE_NAME[MAXSTRSIZE];
extern int ARRAYSIZE;
extern struct ID *head_of_variables;
extern struct ID *procedure;
extern struct FP *tail_of_formal_parameters;

int MAX_NAMESIZE = NAME_WIDTH;
int MAX_TYPESIZE = TYPE_WIDTH;
int ID_NUM;

char TYPES[8][10] = {"char", "integer", "bool", "array", "arrayint", "arraychar", "arraybool", "procedure"};
char type_string[MAXSTRSIZE];

void init_idtab() {		/* Initialise the table */
	if((globalidroot = (struct ID *)malloc(sizeof(struct ID))) == NULL) {	/* allocate for global_idroot */
		printf("can not malloc line in id_record\n");
		exit(0);
	}
	if((localidroot = (struct ID *)malloc(sizeof(struct ID))) == NULL) {	/* allocate for local_idroot */
		printf("can not malloc line in id_record\n");
		exit(0);
	}
	globalidroot->nextp = NULL;
	localidroot->nextp = NULL;

	globalidtail = globalidroot;
	localidtail = localidroot;
}

void init_globalid() {		/* Initialise the table */
	if((globalidroot = (struct ID *)malloc(sizeof(struct ID))) == NULL) {	/* allocate for global_idroot */
		printf("can not malloc line in id_record\n");
		exit(0);
	}
	globalidroot->nextp = NULL;
	globalidtail = globalidroot;
}

void init_localid() {		/* Initialise the table */
	if((localidroot = (struct ID *)malloc(sizeof(struct ID))) == NULL) {	/* allocate for global_idroot */
		printf("can not malloc line in id_record\n");
		exit(0);
	}
	localidroot->nextp = NULL;
	localidtail = localidroot;
}

struct ID *search_idtab(char *np) {	/* search the name pointed by np */
	struct ID *p;
	struct ID *returnp=NULL;

	for(p = globalidroot->nextp; p != NULL; p = p->nextp) {	/* search global */
		if(strcmp(np, p->name) == 0 && strcmp(p->procname, " ") == 0) returnp = p;
	}
	if(strcmp(PROCEDURE_NAME, " ")!=0){	/* IN LOCAL */
		for(p = localidroot->nextp; p != NULL; p = p->nextp) {	/* search local */
			if(strcmp(np, p->name) == 0 && strcmp(p->procname, PROCEDURE_NAME) == 0) return(p);
		}
	}
	return(returnp);
}

void record_types(int Type){
	if(IN_FP){
		tail_of_formal_parameters = procedure->fp;
	}
	while(head_of_variables != NULL){
		head_of_variables->itp->ttype = Type;
		head_of_variables->itp->arraysize = ARRAYSIZE;
		if(IN_FP){
			add_formal_parameter(Type, ARRAYSIZE);
		}
		head_of_variables = head_of_variables->nextp;
	}
}

void add_formal_parameter(int type, int arraysize){
	struct FP *p;
	if((p = (struct FP *)malloc(sizeof(struct FP))) == NULL) {	/* allocate for FP */
		printf("can not malloc FP in id_record\n");
		exit(0);
	}
	p->fptype = type;	/* save type */
	if(arraysize>0) p->fparraysize = arraysize; /* save array size */
	if(procedure->fpnum==0) procedure->fp = p;
	else tail_of_formal_parameters->nextp = p;
	procedure->fpnum++;	/* countup fpnum */
	tail_of_formal_parameters = p; /* go next */
	MAX_TYPESIZE = max(MAX_TYPESIZE, strlen("procedure") + (strlen(TYPES[TPINT])+strlen(", "))*(procedure->fpnum+1) + strlen("()"));
}

struct LINE* get_tail_linep(struct LINE *irefp){
	if(irefp == NULL){	/* Not referred yet */
		return irefp;
	}
	struct LINE *tail;	/* pointer for line tail */
	tail = irefp;
	while(tail->nextp != NULL){
		tail = tail->nextp;
	}
	return tail;
}

int max(int a, int b) { return a > b ? a : b; }

struct ID *create_ID(struct ID *p){
	if((p = (struct ID *)malloc(sizeof(struct ID))) == NULL) {	/* allocate for ID */
		printf("can not malloc ID in id_record\n");
		exit(0);
	}

	if((p->name = (char *)malloc(strlen(string_attr)+1)) == NULL) {	/* allocate for name */
		printf("can not malloc namep in id_record\n");
		exit(0);
	}
	strcpy(p->name, string_attr);	/* save variable name */
	MAX_NAMESIZE = max(MAX_NAMESIZE, strlen(p->name));

	if((p->procname = (char *)malloc(strlen(PROCEDURE_NAME)+1)) == NULL) {	/* allocate for procedure_name */
		printf("can not malloc procedure_name in id_record\n");
		exit(0);
	}
	strcpy(p->procname, PROCEDURE_NAME);	/* save procedure name (scope) */

	if((p->itp = (struct TYPE *)malloc(sizeof(struct TYPE))) == NULL) {	/* allocate for type */
		printf("can not malloc tyoe in id_record\n");
		exit(0);
	}

	if(IN_FP) p->ispara = 1;	/*formal parameter*/
	else if(IN_VAR) p->ispara = 0; /* variable */
	else if(IS_PROCNAME) p->ispara = 0; /* procedure_name */
	else{	/* not IN_FP and not IN_VAR -> first appear without declaration. */
		printf("\nERROR: Undeclared variable.\n");
		free(p);
		exit(0);
	}

	p->deflinenum = get_linenum();
	p->irefp = NULL;
	p->nextp = NULL;

	if(strcmp(PROCEDURE_NAME, " ")==0){	/* in global*/
		globalidtail->nextp = p;
		globalidtail = p;
	}else{	/* in local */
		localidtail->nextp = p;
		localidtail = p;
	}
	return(p);
}

struct ID *add_refline(struct ID *p, struct LINE *linep){
	if((linep = (struct LINE *)malloc(sizeof(struct LINE))) == NULL) {	/* allocate for line */
		printf("can not malloc line in id_record\n");
		exit(0);
	}
	linep->reflinenum = get_linenum();	/* save linenum */
	if(p->irefp == NULL){	/* Not referred yet */
		p->irefp = linep;
	}else{	/* already referred */
		get_tail_linep(p->irefp)->nextp = linep; /* connect liner list */
	}
	return p;
}

struct ID *id_record(char *np) {
	struct ID *p=NULL;
	struct LINE *linep=NULL;

	if((p = search_idtab(np)) != NULL){	/* already declared*/
		if((IN_VAR || IN_FP) && (strcmp(p->procname, PROCEDURE_NAME) == 0) ){	/* In part of declaration and same scope */
			printf("Find duplicate declaration.\n");
			exit(0);
		}else if(IN_VAR || IN_FP){ /* In part of declaration and global variable*/
			return create_ID(p);
		}
		else{	/* Not in part of declaration */
			return add_refline(p, linep);
		}
	}
	else {	/* Not declared yet */
		return create_ID(p);
	}
	return(NULL);
}



void print_repeat(char *c, int x){
	int i;
	for(i=0;i<x;i++) printf("%s", c);
}

void print_wide(char *word, int width){
	printf("%s",word);print_repeat(" ", width-strlen(word));
}

int get_digit(int n){
    int digit = 1;
    while (n /= 10) ++digit;
    return digit;
}

char *print_formal_parameter_type(struct FP *p){
	if(p->fptype == TPARRAY){
		sprintf(type_string, "array [%d] of [type]", p->fparraysize);
	}else{
		sprintf(type_string, "%s", TYPES[p->fptype]);
	}
	return type_string;
}

void print_procedure_type(struct ID *p, int width){
	struct FP *fp;
	char procedure_type[MAXSTRSIZE];
	strcpy(procedure_type, TYPES[p->itp->ttype]);
	if(p->fpnum>0){
		strcat(procedure_type, "(");
		strcat(procedure_type, TYPES[p->fp->fptype]);
		for(fp = p->fp;fp!=NULL;fp = fp->nextp){
			strcat(procedure_type, ", ");
			strcat(procedure_type, print_formal_parameter_type(fp));
		}
		strcat(procedure_type, ")");
	}
	print_wide(procedure_type, width);
}

int cmpname(const void * n1, const void * n2){
	return(strcmp((*(struct ID **)n1)->name, (*(struct ID **)n2)->name));
}

struct ID **creare_id_array(){
	struct ID *p, *q;
	struct ID **id_array;
	int i;
	ID_NUM=0;
	if(globalidroot->nextp==NULL){
		error_("NO VARIABLES.");
	}else{
		for(p = globalidroot->nextp;p!=NULL;p = p->nextp){
			if(strcmp(p->procname, " ")!=0){	/* local */
				strcat(p->name, ":");
				strcat(p->name, p->procname);
			}
			ID_NUM++;
		}
		id_array = (struct ID**)malloc(sizeof(struct ID*)*ID_NUM);
		q = globalidroot->nextp;
		for(i=0;i<ID_NUM;i++){
			id_array[i] = q;
			q = q->nextp;
		}
	}
	return id_array;
}

struct ID **sort_id(){
	struct ID **id_array;
	id_array = creare_id_array();
	qsort(id_array, ID_NUM, sizeof(struct ID*), cmpname);
	return id_array;
}

void print_cross_referencer() {
	struct ID *p;
	struct LINE *refp;
	struct ID **sorted_id_array;
	int i;
	if(globalidroot->nextp == NULL) printf("NOTHING\n");
	else{
		print_repeat("-", MAX_NAMESIZE+MAX_TYPESIZE+DEF_WIDTH+REF_WIDTH);printf("\n");
		print_wide("Name", MAX_NAMESIZE);
		print_wide("Type", MAX_TYPESIZE);
		print_repeat(" ", DEF_WIDTH-strlen("Def."));printf("Def.");
		printf("  ");
		print_wide("Ref.", REF_WIDTH);
		printf("\n");

		sorted_id_array = sort_id();
		for(i=0;i<ID_NUM;i++){
			p = sorted_id_array[i];
			print_wide(p->name, MAX_NAMESIZE);
			if(p->itp->ttype == TPPROC){
				print_procedure_type(p, MAX_TYPESIZE);
			}else{
				print_wide(TYPES[p->itp->ttype], MAX_TYPESIZE);
			}
			print_repeat(" ", DEF_WIDTH-get_digit(p->deflinenum));
			printf("%d", p->deflinenum);
			printf("|");
			refp = p->irefp;
			if(refp!=NULL){
				printf("%d",refp->reflinenum);
				refp = refp->nextp;
			}
			while(refp!=NULL){
				printf(", %d",refp->reflinenum);
				refp = refp->nextp;
			}
			printf("\n");
		}
	}
	print_repeat("-", MAX_NAMESIZE+MAX_TYPESIZE+DEF_WIDTH+REF_WIDTH);printf("\n");
	release_idtab(globalidroot);
}

void release_ref_list(struct LINE *root){
	struct LINE *p,*q;
	if(root == NULL) return;
	for(p = root->nextp;p != NULL;p = q){
		q = p->nextp;
		free(p);
	}
}
void release_fp_list(struct FP *root){
	struct FP *p,*q;
	if(root == NULL) return;
	for(p = root->nextp;p != NULL;p = q){
		q = p->nextp;
		free(p);
	}
}


void release_idtab(struct ID *root) {
	struct ID *p, *q;
	for(p = root->nextp; p != NULL; p = q) {
		free(p->name);
		free(p->procname);
		free(p->itp);
		release_ref_list(p->irefp);
		release_fp_list(p->fp);
		q = p->nextp;
		free(p);
	}
	return;
}


