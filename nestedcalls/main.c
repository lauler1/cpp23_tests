//This simple program just print the nested function calls

int myfunccallcount = 0;
#ifdef USE_CPP
 #include <iostream>
 #include <string>
 #define AA std::cout << std::string(++myfunccallcount, ' ') << ">>> "<< __FUNCTION__ <<"\n";
 #define BB std::cout << std::string(myfunccallcount--, ' ') << "<<<\n";
 #define return std::cout << std::string(myfunccallcount--, ' ') << "<<<\n"; return
#else// else use pure C
 #include <stdio.h>
 #define PRINT_SPACES(num_spaces) for(int i = 0; i < num_spaces; i++){putchar(' ');}
 #define AA PRINT_SPACES(myfunccallcount);myfunccallcount++;printf(">>>: %s{\n", __FUNCTION__);
 #define BB ;myfunccallcount--;PRINT_SPACES(myfunccallcount); puts("<<<}");
 #define return myfunccallcount--;PRINT_SPACES(myfunccallcount); puts("<<<}"); return
#endif



int foo2 (){AA

    return 1;
BB}


void foo1 (){AA

    foo2();
BB}

// Type your code here, or load an example.
int main() {AA
    //std::cout << "Hello!\n";
    foo1();
    return 0;
BB}
