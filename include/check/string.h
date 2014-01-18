#ifndef CHECK_STRING_H
#define CHECK_STRING_H

/*

find tslibc/src/string -name 'check_*.c' | cut -d'_' -f2 | xargs -I% basename % .c

strrev
strrot
strrtok
strshift
strstrip
*/

TCase * tcase_strrev(void);
TCase * tcase_strrot(void);
TCase * tcase_strrtok(void);
TCase * tcase_strshift(void);
TCase * tcase_strstrip(void);

#endif /* CHECK_STRING_H */
