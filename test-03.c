#include <stdio.h>



#include "common.h"

#define STRUTIL_IMPLEMENTATION
#include "strutil.h"



typedef enum ParserState {
  PARSERSTATE_DEFAULT=0,
  PARSERSTATE_STRING,
  PARSERSTATE_MAX  
} ParserState;



void csvaddline(char ***lines,size_t *nlines,char *line) {
  (*lines)=realloc(*lines,sizeof(**lines)*(*nlines+1));  
  (*lines)[(*nlines)++]=strdup(line);
}
 
void csvparse(char ***lines,size_t *nlines,char *line) {
  ParserState parserState=PARSERSTATE_DEFAULT;
  char *p=line;
  char text[STRING_MAX];
  text[0]='\0';
  while(*p) {
    switch(parserState) {
      case PARSERSTATE_DEFAULT: 
        if(*p=='\"') {
          parserState=PARSERSTATE_STRING;
        } 
      break;
      case PARSERSTATE_STRING: 
        while(*p!='\"') {
          if(*p=='\\') {
            p++;
            switch(*p) {
              case 'a': strcat(text,(char[2]){'\a','\0'}); break;
              case 'b': strcat(text,(char[2]){'\b','\0'}); break;
              case 'f': strcat(text,(char[2]){'\f','\0'}); break;
              case 'n': strcat(text,(char[2]){'\n','\0'}); break;
              case 'r': strcat(text,(char[2]){'\r','\0'}); break;
              case 't': strcat(text,(char[2]){'\t','\0'}); break;
              case 'v': strcat(text,(char[2]){'\v','\0'}); break;
              case '\'': strcat(text,(char[2]){'\'','\0'}); break;
              case '\"': strcat(text,(char[2]){'\"','\0'}); break;
              case '\?': strcat(text,(char[2]){'\?','\0'}); break;
              case '\\': strcat(text,(char[2]){'\\','\0'}); break;
              default: strcat(text,(char[2]){*p,'\0'});
            }
          } else strcat(text,(char[2]){*p,'\0'});
          p++;
        }
        csvaddline(lines,nlines,text);
        text[0]='\0';
        parserState=PARSERSTATE_DEFAULT;
      break;
      default: break;
    }
    p++;
  }
}

void csvprint(char **lines,size_t nlines) {
  for(size_t i=0;i<nlines;i++) {
    printf("%s\n",lines[i]);
  }
  printf("\n");
}

void csvfree(char ***lines,size_t *nlines) {
  for(size_t i=0;i<(*nlines);i++) {
    free((*lines)[i]);
    (*lines)[i]=NULL;
  }
  free(*lines);
  (*lines)=NULL;
  (*nlines)=0;
}



typedef struct Brain Brain;

struct Brain {
  char **lines;
  size_t nlines;
};

Brain **brains=NULL;
size_t nbrains=0;

Brain *Brain_New(char **lines,size_t nlines) {
  Brain *brain=malloc(sizeof(*brain));
  if(brain) {
    brain->lines=lines;
    brain->nlines=nlines;
  }
  return brain;
}

void Brain_Add(Brain ***brains,size_t *nbrains,Brain *brain) {
  (*brains)=realloc(*brains,sizeof(**brains)*((*nbrains)+1));
  (*brains)[(*nbrains)++]=brain;
}

void Brain_Print(Brain *brain) {
  csvprint(brain->lines,brain->nlines);
}

void Brains_Print(Brain **brains,size_t nbrains) {
  for(size_t i=0;i<nbrains;i++) {
    Brain_Print(brains[i]);
  }
}

void Brain_Free(Brain **brain) {
  csvfree(&(*brain)->lines,&(*brain)->nlines);
  free(*brain);
  (*brain)=NULL;
}

void Brains_Free(Brain ***brains,size_t *nbrains) {
  for(size_t i=0;i<(*nbrains);i++) {
    Brain_Free(&(*brains)[i]);
  } 
  free(*brains);
  (*brains)=NULL;
  (*nbrains)=0;
}



int main(void) {

  char *line=NULL;
  size_t llen=0;
  ssize_t rlen=0;

  char **lines=NULL;
  size_t nlines=0;
  
  while((rlen=getline(&line,&llen,stdin))>0) {

    csvparse(&lines,&nlines,line);

    Brain_Add(&brains,&nbrains,Brain_New(lines,nlines));

    lines=NULL;
    nlines=0;

    free(line);
    line=NULL;
    llen=0;
    rlen=0;
  }

  free(line);
  line=NULL;
  llen=0;
  rlen=0;

  Brains_Print(brains,nbrains);

  Brains_Free(&brains,&nbrains);

  return 0;
}



