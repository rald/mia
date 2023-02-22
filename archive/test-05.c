#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RE_IMPLEMENTATION
#include "re.h"

#define STRING_MAX 65536

typedef struct Find Find;

struct Find {
	int idx;
	int len;
	char *txt;
};

Find **finds=NULL;
size_t nfinds=0;

char *mid(char *s,size_t i,size_t n) {
	char *r=malloc(sizeof(*r)*(n+1));
	strncpy(r,s+i,n);
	r[n]='\0';
	return r;
}

void find(Find ***finds,size_t *nfinds,char **pats,size_t npats,char *str) {
	size_t pos=0;
	for(size_t i=0;i<npats;i++) {
		Find *f=malloc(sizeof(*f));
		f->idx=re_match(pats[i],str+pos,&f->len);
		f->txt=NULL;
		
		if(f->idx!=-1) {
		 	size_t spos=pos;
			f->txt=mid(str+pos,f->idx,f->len);
			pos+=f->idx+f->len;
			f->idx+=spos;
		}

		*finds=realloc(*finds,sizeof(**finds)*(*nfinds+1));
		(*finds)[(*nfinds)++]=f;
	}
}

void Find_Print(Find *find) {
	printf("idx: %3d len: %3d txt: \"%s\"\n",find->idx,find->len,find->txt);
}

void Finds_Print(Find **finds,size_t nfinds) {
	for(size_t i=0;i<nfinds;i++) {
		Find_Print(finds[i]);
	}
}

void Find_Free(Find **find) {
	(*find)->idx=0;
	(*find)->len=0;
	free((*find)->txt);
	(*find)->txt=NULL;
	
	free(*find);
	(*find)=NULL;
}

void Finds_Free(Find ***finds,size_t *nfinds) {
	for(size_t i=0;i<(*nfinds);i++) {
		Find_Free(&(*finds)[i]);
	}
	free(*finds);
	(*finds)=NULL;
	(*nfinds)=0;
}




int main(void) {

	#define pnum "[+-]?[0-9]*[.]?[0-9]+"
	#define pvar "\\$[A-Za-z_]+[A-Za-z0-9_]*"

	char *pats[]={"ang presyo ng pagibig ay " pnum " pesos"};
	size_t npats=1;
	char *str="ang presyo ng pagibig ay 143.44 pesos";

	printf("%s\n",str);

	find(&finds,&nfinds,pats,npats,str);

	Finds_Print(finds,nfinds);

	Finds_Free(&finds,&nfinds);

	return 0;
}


