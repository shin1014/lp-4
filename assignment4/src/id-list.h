#define TPCHAR 0
#define TPINT 1
#define TPBOOL 2
#define TPARRAY 3
#define ARRAYTYPECHAR 4
#define TPARRAYINT 5
#define TPARRAYBOOL 6
#define TPPROC 7
#define TYPEOFFSET 20
#define ARRAYTYPEOFFSET 4

#define NAME_WIDTH 20
#define TYPE_WIDTH 40
#define DEF_WIDTH 4
#define REF_WIDTH 30

extern int linenum;
extern char TYPES[8][10];


struct TYPE {
	int ttype;
	int arraysize; /* size of array, if TPARRAY */
};

struct LINE {
	int reflinenum;
	struct LINE *nextp;
};

struct FP{
	int fptype;
	int fparraysize;
	struct FP *nextp;
};

struct ID {
	char *name;
	char *procname; /* procedure name within which this name is defined */ /* NULL if global name */
	struct TYPE *itp;
	int ispara; /* 1:formal parameter, 0:else(variable) */
	int deflinenum;
	struct LINE *irefp;
	struct ID *nextp;
	int fpnum;
	struct FP *fp;

} *globalidroot, *localidroot, *globalidtail, *localidtail; /* Pointers to root of global & local symbol tables */


extern struct ID *id_record(char *np);

void record_types(int Type);

extern void init_idtab();
extern void init_globalid();
extern void init_localid();
extern struct ID *search_idtab(char *np);
extern void id_countup(char *np);
extern void print_idtab();
extern void release_idtab(struct ID *root);
extern struct ID *create_ID(struct ID *p);
extern void print_cross_referencer();
extern int max(int a, int b);
extern void add_formal_parameter(int type, int arraysize);
