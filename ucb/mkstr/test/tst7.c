#include <stdio.h>

foobar(char *str) {puts(str);}
bazfoo(char *str) {puts(str);}
error(char *str)  {puts(str);}

main()
        {
        foobar("abc"); error("def"); bazfoo("ghi");
        }
