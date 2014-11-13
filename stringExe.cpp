#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <getopt.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <vector>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/conf.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/md5.h>

using namespace std;
void print (unsigned char *);
void changeArray(char array[8], int length)
{


	array[2] = 'c';

}

int main(int argc, const char * argv[])
{
	
	unsigned char *ciphertext = (unsigned char *)"saaa\0fd";
	
	int fd = open((const char *)"hahaha",O_CREAT|O_WRONLY|O_TRUNC);
	cout<<"fd is "<<fd<<endl;
	if(write(fd,(void *)ciphertext,7) == -1){
		fprintf(stderr,"error happens\n");
		exit(1);
	}
	close(fd);

	string bufferLine;
	ofstream newFile;
  	newFile.open((const char *)"testFile",std::ofstream::out);	
	/* read file contents from the input file into the usr file
	 */
	int flag = 0;

	ifstream ifs ("hahaha", std::ifstream::in);

	while(getline(ifs,bufferLine)){
		if(flag !=0){
		 	newFile<<"\n";
		}
		flag = 1;
		newFile<<bufferLine;
		cout << "bufferLine is "<< bufferLine<<endl;	
		if(bufferLine.at(4)==NULL)
			cout<<"it is null"<<endl;
		cout<<bufferLine.at(5)<<endl;
	}
	string ttmmp("ooo");
	ttmmp.append("6\0 7");
	ttmmp.append("ttt");
	ttmmp.insert(4,1,'\0');
	if(ttmmp.at(4)==NULL)
		cout<<"it is also null"<<endl;
	cout <<"another bufferline is " << ttmmp<<endl;
	cout<<"strlen is "<< ttmmp.length()<<endl;
	newFile.close();
	







	unsigned char *seq = (unsigned char *)"abcdefg\0aaaa";
	printf("the strlen is %d \n ", strlen((const char *)seq));
	char array[8] = {'x'};
	changeArray(array,2);
	char *hhh = "123456789999999";
	printf("the character here is %c \n", array[3]);
	printf("the size of arrays is %d  " , sizeof(hhh));
	unsigned char *bytes;
	bytes = ( unsigned char*)malloc(sizeof(char)*16);
	//print(bytes);		
	for (int i =0 ; i < 16 ; i ++){
		printf("bytes[%d] %2.2x\n", i, bytes[i]);
	}
	return 0;
}

unsigned char *rand_gen ()
{
	unsigned char *bytes;
	bytes = ( unsigned char*)malloc(sizeof(char)*16);
	int i = 0;
	FILE* f;
	/*open file */
	if((f = fopen("/dev/urandom", "r")) == NULL) {
  	   exit(1);
	}	
	while(i < 16)  {
     		if((bytes[i]  = fgetc(f)) == EOF) {
       		   	 fprintf(stderr,
				"reading failure\n");
				exit(1);
     		}
      		printf("bytes[%d] %2.2x\n", i, bytes[i]);
		//unsigned char hhh = (unsigned char)bytes[i];
		//printf("our unsigned byte is %2.2x\n", hhh);
      		i++;
	}
	printf("the size of byte is %d\n", sizeof(bytes[0]));
	fclose(f);
}


