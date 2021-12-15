#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "token-list.h"
#include "id-list.h"

#define SINGLEQUOTE 39

extern FILE *fp;
char cbuf;
int num_attr;
char string_attr[MAXSTRSIZE];
int linenum;

char PROCEDURE_NAME[MAXSTRSIZE];

int isAlphabet(void);
int isNumber(void);
int isBlank(void);
int isEndOfLine(void);
void skipComment(void);

/* Error list */
char error_string_over[]="String size over.";
char error_number_over[]="Number size over";

int init_scan(char *filename){
	if ((fp = fopen(filename, "r")) == NULL) {
		printf("file open error!!\n");
		exit(EXIT_FAILURE);
	}
	if((cbuf = fgetc(fp)) != EOF) {
		linenum=1;
		/*printf("%c", cbuf);*/	/*for debug*/
		return 0;
	}else{
		return -1;
	}
}


int scan(void){
	int p;
	num_attr = 0;
	memset(string_attr, '\0', sizeof(string_attr));	/* initialize string_attr */
	while(1){
		switch(cbuf){
				case '+': getcbuf(); return TPLUS; break;
				case '-': getcbuf(); return TMINUS; break;
				case '*': getcbuf(); return TSTAR; break;
				case '<':
					getcbuf();
					if(cbuf=='>'){getcbuf(); return TNOTEQ;}
					else if(cbuf=='='){getcbuf(); return TLEEQ;}
					else return TLE;
					break;
				case '>':
					getcbuf();
					if(cbuf=='='){getcbuf(); return TGREQ;}
					else return TGR;
					break;
				case '(': getcbuf(); return TLPAREN; break;
				case ')': getcbuf(); return TRPAREN; break;
				case '[': getcbuf(); return TLSQPAREN; break;
				case ']': getcbuf(); return TRSQPAREN; break;
				case '.': getcbuf(); return TDOT; break;
				case ',': getcbuf(); return TCOMMA; break;
				case ':':
					getcbuf();
					if(cbuf=='='){getcbuf(); return TASSIGN;}
					return TCOLON;
					break;
				case ';': getcbuf(); return TSEMI; break;
				case '=': getcbuf(); return TEQUAL; break;	/* '=' must be last, because it comes after some symbols, '<', '>' and ':' . */
				case '/': skipComment(); break;
				case '{': skipComment(); break;
				default:
					p=0;
					if(isBlank()) continue;	/* if blank, skip in the function. */
					else if(isEndOfLine()) continue;	/* if end-of-line, skip and count in the function. */
					else if(isAlphabet()){	/* Word ("word" means keyword or name) */
						while(isAlphabet() || isNumber()){	/* read until word-end */
							string_attr[p] = cbuf;
							p++;
							if(p>=MAXSTRSIZE-1){/* string size over. */
								error_(error_string_over);
								exit(EXIT_FAILURE);
							}
							getcbuf();

						}
						int k=0;
						while(k<KEYWORDSIZE){
							if( strcmp(string_attr, key[k].keyword) == 0){	/* match -> return token number. */
								return key[k].keytoken;
							}else{
								k++;
								continue;
							}
						}
						return TNAME; /* if reach here, it is not keyword */
					}
					else if(isNumber()){/* Number */
						while(cbuf != ' ' || cbuf != '\t'){	/* read until number-end */
							if(isNumber()){ /* if number comes after number */
								string_attr[p] = cbuf;
								p++;
								getcbuf();
							}else{	/* non-number comes after number */
								if(atoi(string_attr)>MAXNUMBERSIZE) {/* number size over. */
									error_(error_number_over);
									exit(EXIT_FAILURE);
								}
								else{
									num_attr = atoi(string_attr);
									return TNUMBER;
								}
							}
						}
						/*only number*/
						num_attr = atoi(string_attr);
						return TNUMBER;
					}else if(cbuf==SINGLEQUOTE){	/* if String starts */
						getcbuf();
						while(1){	/* read until String-end */
							if(cbuf==SINGLEQUOTE){
								getcbuf();
								if(cbuf!=SINGLEQUOTE){
									ungetc(cbuf, fp);
									break;
								}
								else{
									string_attr[p] = SINGLEQUOTE;
									p++;
								}
							}
							string_attr[p] = cbuf;
							p++;
							if(p>MAXSTRSIZE-1){/* string size over. */
								error_(error_string_over);
								exit(EXIT_FAILURE);
							}
							getcbuf();
						}
						getcbuf();	/* dump " of end*/
						return TSTRING;
					}else if(cbuf==EOF){	/* EOF */
						/* printf("\nEOF\n"); */ /*for debug*/
						return -1;
					}else{	/* other */
						printf("\nDetect Undefined character '%c'\n", cbuf);
						exit(EXIT_FAILURE);
					}
					break;
			}
	}

	return 0;
}

int get_linenum(void){
	/*
	if(cbuf=='\r' || cbuf=='\n') return linenum-1;
	else return linenum;
	*/
	return linenum;
}

void end_scan(void){
	fclose(fp);
}

int getcbuf(void){
	if((cbuf = fgetc(fp)) != EOF) {
			/* printf("%c", cbuf); */ /*for debug*/
			/* fflush(stdout); */
			return 0;
	}else{
		return -1;
	}
}

int isAlphabet(void){
	if(('a'<=cbuf && cbuf<='z') || ('A'<=cbuf && cbuf<='Z')) return 1;
	else return 0;
}

int isNumber(void){
	if('0'<=cbuf && cbuf<='9') return 1;
	else return 0;
}

int isBlank(void){
	if(cbuf!=' ' && cbuf!='\t') return 0;
	else{
		while(cbuf==' ' || cbuf=='\t') getcbuf();
		return 1;
	}
}

int isEndOfLine(void){
	if(cbuf=='\r'){
		getcbuf();
		if(cbuf=='\n') getcbuf();	/* for '\r\n' */
		linenum++;
		return 1;
	}else if(cbuf=='\n'){
		getcbuf();
		if(cbuf=='\r') getcbuf();	/* for '\n\r */
		linenum++;
		return 1;
	}
	else return 0;
}

void skipComment(void){
	if(cbuf=='/'){
		getcbuf();
		if(cbuf=='*'){
			getcbuf();
			while(1){
				if(cbuf!='*'){
					getcbuf();
					continue;
				}
				else{
					getcbuf();
					if(cbuf=='/'){
						getcbuf();
						return;	/* find comment end*/
					}
					else continue;	/* only '*' */
				}
			}

			return;	/* if reach here, the code has syntax error. it does't have end of comment.*/

		}else{	/* only '/' */
			getcbuf();
			return;
		}
	}
	else if(cbuf=='{'){
		while(cbuf!='}') getcbuf();
		getcbuf();
		return;
	}
}


