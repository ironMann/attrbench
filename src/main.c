#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>                
//#include <attr/xattr.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

const int FILE_NAME_LEN = 16;

void printPercents(float n)
{
	printf("\b\b\b\b\b\b\b\b               \b\b\b\b\b\b\b\b\b\b\b\b\b\b\b%.2f %%", n);
}

inline long getus()
{
	struct timeval t;
	gettimeofday(&t, NULL);
	return t.tv_sec*1000000+t.tv_usec;
}
 inline long getusinterval(long start)
 {
 	struct timeval t;
	gettimeofday(&t, NULL);
	return (long) (t.tv_sec*1000000+t.tv_usec - start);
}

inline char randomChar()
{		
   char a;
   do {
   	a = rand() % ('z'+1);
   }
   while(a<'0' || (a>'9' && a<'A') || (a>'Z' && a<'a') || a>'z');
   return a;
}

inline char* randomStr(int len)
{
	static char str[4096] = {0};
	int i;
	for(i=0;i<len && i<4096; i++)
		str[i] = randomChar();
	
	str[4095]=0;
	str[len-1]=0;
	
	return str;
}


int main(int argc, char  **argv)
{
	int i;
	
	srand ( time(NULL) );
	
	if(argc < 4) {
		printf("usage: mdtest dir num_of_files bytes_of_extattr_per_file\n");
		return -1;
	}
	
	const char *dir = argv[1];
	
	struct stat st;
	if(stat(dir,&st) != 0) {
		printf("Directory %s is not present\n", dir);
		return -2;
	}
	
	if(chdir(dir) != 0) {
		printf("Can not set %s as a working directory\n", dir);
		return -3;
	}
	
	printf("Dir OK!\n");
	
	int num_files = atoi(argv[2]);
	
	if(num_files <=0) {
		printf("Invalid number of files %d\n", num_files);
		return -3;
	}
		
	int attr_size = atoi(argv[3]);
	
	if(attr_size < 0) {
		printf("Invalid size of ext. attributes %d\n", attr_size);
		return -3;
	}
	
	printf("Using dir %s. Creating %d files. Writing %d bytes of extended attributes per file.\n", dir, num_files,attr_size);
	
	char *file_names = (char*) calloc((FILE_NAME_LEN+1)*sizeof(char), num_files);
	for(i=0; i<num_files; i++) {
		memcpy(file_names+(i*(FILE_NAME_LEN+1)), randomStr(FILE_NAME_LEN), FILE_NAME_LEN+1);
		(file_names+i*(FILE_NAME_LEN+1))[FILE_NAME_LEN-1] = '7';
	}
	
	char *attrval = NULL;
	if (attr_size > 0 ) {
		attrval = calloc(1, attr_size+1);
		for(i=0; i<attr_size; i++)
			attrval[i]=randomChar();
	}
	
	printf("Creating files: ");
	
	long start = getus();

	int report_every = num_files/1000>0?num_files/1000:1;
	
	for(i=0; i<num_files; i++) {
		char *fname=file_names+(i*(FILE_NAME_LEN+1));
		
		if(mknod(fname, S_IFREG|0666, 0) ==0 ) {
			if(attr_size>0) {

				if(lsetxattr(fname, "user.mdtest.test", attrval, attr_size, 0) != 0) {
				
					if(ENOTSUP == errno)
						printf("Extended attributes not supported.\n");
					else if(ENOSPC == errno)
						printf("No space for extended attributes.\n");
					else if(EEXIST == errno)
						printf("Extended attributes exist.\n");
					else if(EACCES == errno)
						printf("EACCES.\n");
					else if(EBADF == errno)
						printf("EBADF.\n");

					printf("Error setting extended attributes! Ignoring option.\n");
					attr_size = 0;
				}
			}
		}
		
		if(i%report_every==0)
			printPercents((float)i/(float)num_files*100.);
		
	}
	
	long duration = getusinterval(start);
	
	printPercents(100.); printf("\n\n");

	printf("Created %d files in %ld us. %f files/s\n", num_files, duration, (float)num_files/((float)duration/1000000.));
	
	//for(i=0; i<num_files; i++) {
		//unlink(file_names+(i*(FILE_NAME_LEN+1)));
	//}
	
	if(file_names) free(file_names);
	if(attrval)    free(attrval);
	
	
	return 0;
}
