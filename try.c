#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* string compare function */
int compare_strings (const void *a, const void *b) {
    return strcmp(*(char **)(a)+15, *(char **)(b)+15);
    //const char **ia=(const char **)a+24;
    //const char **ib=(const char **)b+24;
    //return strcmp(*ia,*ib);
}

int cmp (char **str1, char **str2)
{
  return strcmp(*(str1)+16,*(str2)+16);
}


int main (void) {
	char **array = malloc(sizeof *array * 3);
	//for(int i=0;i<3;i++){
		//array[i] = (char*)malloc(10);
	//}
	//char *array

	//char **array=["s |sec:1485117662, nsec:577283680","i |sec:1485117660, nsec:577304348","a |sec:1485117668, nsec:577386549"];
//    char *strings[] = {"Baba", "Cece" , "Caca" , "Zaab" , "Laaa"};
    size_t i;
    
    array[0]="s |sec:1485117662, nsec:577283683";
    array[1]="i |sec:1485117660, nsec:577304348";
    array[2]="a |sec:1485117668, nsec:577386549";
    
    qsort (array,3,sizeof(char *),
                    (int (*)(const void *, const void *)) cmp);
                    
  // now display the sorted array
  for (int i=0; i < 3; i++) 
    printf("%s \n",array[i]);
    
    /*
     a |sec:1485117659, nsec:577269247
Server received s |sec:1485117662, nsec:577283683
Server received i |sec:1485117665, nsec:577304348
Server received a |sec:1485117668, nsec:577386549

     
     */
    
    //qsort (array, sizeof array/sizeof **array, 
      //     sizeof **array, compare_strings);

    /* sort strings */
    //qsort (strings, sizeof strings/sizeof *strings, 
      //     sizeof *strings, compare_strings);

    /* output sorted arrray of strings */
    //for (i = 0; i < (sizeof array/sizeof *array); i++)
    //for (i = 0; i < 3; i++)
      //  printf (" strings[%2zu] : %s\n", i, array[i]);

    return 0;
}
