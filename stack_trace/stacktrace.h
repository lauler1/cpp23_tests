// stacktrace.h (c) 2008, Timo Bingmann from http://idlebox.net/
// published under the WTFPL v2.0

#ifndef _STACKTRACE_H_
#define _STACKTRACE_H_

#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#include <cxxabi.h>

#include <string.h>
#include <ctype.h>
#include <math.h>

#if 1

static inline void my_stacktrace(void){
	
  void *array[63];
  char **strings;
  int size, i;

  size = backtrace (array, 63);
  strings = backtrace_symbols (array, size);
  if (strings != NULL)
  {

    printf ("\n\n\nObtained %d stack frames.\n", size);
    for (i = 0; i < size; i++){
      printf ("... %s\n", strings[i]);
	  puts("");
	}
  }

  free (strings);	
	
	
}







static inline char *demangle_symbols(char* pch);


static inline char *demangle_symbol(char* pch){
	if(pch){
		
//printf("isdigit = %d\n", isdigit(*pch));
		
		if(isdigit(*pch)){
			int len = atoi(pch);
//printf("len = %d\n", len);
			if(len <= 0) {
				return ++pch;
			}
			int offset = (int)log10(len) + 1;
//printf(">>>>>>     demangle_symbol %s %d %d\n", pch, len, offset);
			
			pch += offset;
			printf("%.*s", len, pch);
			return pch + len;
		}
		
		if(*pch != 0){		
//printf(">>>>>>     demangle_symbol %s %d %d\n", pch, 1, 0);
//puts(" sdfsdf 1 ");
			putchar(*pch);
//puts(" sdfsdf 2 ");
		}
		return ++pch;

	}
	return NULL;
}

static inline char *demangle_templates(char* pch){
	if(pch && *pch == 'I'){
		fputs("<", stdout);
//printf(">>>>>>     demangle_templates %s\n", pch);
		++pch;
		int cont = 0;
		do{
//puts(" t1 ");
			pch = demangle_symbols(pch);
//printf(" t2 %s\n", pch);
			cont = (*pch != 'E');
			if(cont)fputs(",", stdout);
		}while(cont);
		fputs(">", stdout);	

		return pch;		
	}
	
	return NULL;
}

static inline char *demangle_namespaces (char* pch){
	if(pch && *pch == 'N'){
//printf(">>>>>>     demangle_namespaces %s\n", pch);
		
		++pch;
		int cont = 0;
		do{
//puts(" n1 ");
			pch = demangle_symbols(pch);
//printf(" n2 %s\n", pch);
			cont = (*pch != 'E');
			if(cont)fputs("::", stdout);
		}while(cont);
		
		return pch;
	}

	return NULL;
}

static inline char *demangle_literal (char* pch){
	if(pch && *pch == 'L'){
//printf(">>>>>>     demangle_literal %s\n", pch);
		

		int cont = 0;
		do{
//puts(" n1 ");
			++pch;

//printf(" n2 %s\n", pch);
			cont = (*pch != 'E');
			if(cont){
				putchar(*pch);
				fputs(" ", stdout);
			}
		}while(cont);
		
		return pch;
	}

	return NULL;
}

static inline char *demangle_symbols(char* pch){

	if(pch){
		//printf(">>>>     demangle_symbols %s\n", pch);
		if(*pch == 'I'){
			return demangle_templates(pch);
		}else if(*pch == 'N'){
			return demangle_namespaces (pch);
		}else if(*pch == 'L'){
			return demangle_literal (pch);
		}else if(*pch == '+'){
			return NULL;
		}else{
			return demangle_symbol(pch);
		}
	}else{
		return NULL;
	}
	
}

static inline void demangle(char* func){
	if(func){
		char *pch = strstr(func, "_Z");
		if(pch){
	//printf(">>>>>> demangle %s\n", pch);
			try{
				demangle_symbols(pch+=2);
				puts("");
			}catch(...){
				puts("Exception");
				my_stacktrace();
			}
		}else{
			puts(func);
		}
	}
}


static inline void print_stacktrace(FILE *out = stderr, unsigned int max_frames = 63){
	
  void *array[max_frames];
  char **strings;
  int size, i;

  size = backtrace (array, max_frames);
  strings = backtrace_symbols (array, size);
  if (strings != NULL)
  {

    fprintf(out, "\n\nObtained %d stack frames.\n", size);
    for (i = 0; i < size; i++){
      fprintf(out, "... %s\n", strings[i]);
	  //demangle(strings[i]);

	}
  }

  free (strings);	

}


#else //with __cxa_demangle

/** Print a demangled stack backtrace of the caller function to FILE* out. */
static inline void print_stacktrace(FILE *out = stderr, unsigned int max_frames = 63)
{
    fprintf(out, "stack trace:\n");

    // storage array for stack trace address data
    void* addrlist[max_frames+1];

    // retrieve current stack addresses
    int addrlen = backtrace(addrlist, sizeof(addrlist) / sizeof(void*));

    if (addrlen == 0) {
		fprintf(out, "  <empty, possibly corrupt>\n");
		return;
    }

    // resolve addresses into strings containing "filename(function+address)",
    // this array must be free()-ed
    char** symbollist = backtrace_symbols(addrlist, addrlen);

    // allocate string which will be filled with the demangled function name
    size_t funcnamesize = 256;
    char* funcname = (char*)malloc(funcnamesize);

    // iterate over the returned symbol lines. skip the first, it is the
    // address of this function.
    for (int i = 1; i < addrlen; i++)
    {
		char *begin_name = 0, *begin_offset = 0, *end_offset = 0;
		// find parentheses and +address offset surrounding the mangled name:
		// ./module(function+0x15c) [0x8048a6d]
		int ii=1;
		for (char *p = symbollist[i]; *p; ++p)
		{
			if (*p == '(')
				begin_name = p;
			else if (*p == '+')
				begin_offset = p;
			else if (*p == ')' && begin_offset) {
				end_offset = p;
			break;
			}
		}

		if (begin_name && begin_offset && end_offset
			&& begin_name < begin_offset)
		{
			*begin_name++ = '\0';
			*begin_offset++ = '\0';
			*end_offset = '\0';

			// mangled name is now in [begin_name, begin_offset) and caller
			// offset in [begin_offset, end_offset). now apply
			// __cxa_demangle():

			int status;
			char* ret = abi::__cxa_demangle(begin_name,
							funcname, &funcnamesize, &status);
			if (status == 0) {
				funcname = ret; // use possibly realloc()-ed string
				fprintf(out, "  %s : %s+%s\n",
					symbollist[i], funcname, begin_offset);
			}
			else {
				// demangling failed. Output function name as a C function with
				// no arguments.
				fprintf(out, "  %s : %s()+%s\n",
					symbollist[i], begin_name, begin_offset);
			}
		}
		else
		{
			// couldn't parse the line? print the whole line.
			fprintf(out, "  %s\n", symbollist[i]);
		}
    }

    free(funcname);
    free(symbollist);
}
#endif //0
#endif // _STACKTRACE_H_
