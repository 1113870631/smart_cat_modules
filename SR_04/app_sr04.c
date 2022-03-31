#include <stdio.h>
void main(int argc,char ** argv){
       FILE *file;
       file= fopen("/dev/SR04_cdev1", "r");
	   fclose(file);

}






