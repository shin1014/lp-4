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

void LD(char *a, char *b){
	fprintf(outfp,"\tLD\t%s, %s\n",a,b);
}

void ST(char *a, char *b){
	fprintf(outfp,"\tST\t%s, %s\n",a,b);
}

void LAD(char *a, char *b){
	fprintf(outfp,"\tLAD\t%s, %s\n",a,b);
}
