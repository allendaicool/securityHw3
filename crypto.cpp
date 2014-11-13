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

//int inputlenghhh = 0;
using namespace std;
int size_of_file (string & );
int writeIntoFile(string &);
unsigned char * write_Into_file_and_out (string &,int &);
void handleErrors(void)
{
  ERR_print_errors_fp(stderr);
  abort();
}


void rand_gen (unsigned char *bytes)
{
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
  //    		printf("bytes[%d] %2.2x\n", i, bytes[i]);
		//unsigned char hhh = (unsigned char)bytes[i];
		//printf("our unsigned byte is %2.2x\n", hhh);
      		i++;
	}
//	printf("the size of byte is %d\n", sizeof(bytes[0]));
	fclose(f);
}



static void hex_print(const void* pv, size_t len)
{
    const unsigned char * p = (const unsigned char*)pv;
    if (NULL == pv)
        printf("NULL");
    else
    {
        size_t i = 0;
        for (; i<len;++i){

           	 printf("hexdecimal is%02X ", *p++);

	}	

	}
    printf("\n");
} 

char * MD5Sequence() {
 
	string input;	
        cout<<"please enter the password";
        getline(cin, input);
        unsigned char *data = (unsigned char *)input.c_str();
        unsigned char digest[16];
        int i;
        char tmp[3]={'\0'};
	char * buf= (char *)malloc(sizeof(char)*33);
	buf[32] = '\0';
        MD5(data,strlen((const char *)data),digest);
        for (i = 0; i < 16; i++){
                snprintf(tmp,3,"%2.2x",digest[i]);
                strcat(buf,tmp);
        }
        printf("hashed value is %s\n",buf);
        return buf;
}








int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
  unsigned char *iv, unsigned char *ciphertext)
{
  EVP_CIPHER_CTX *ctx;

  int len;

  int ciphertext_len;

  /* Create and initialise the context */
  if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

  /* Initialise the encryption operation. IMPORTANT - ensure you use a key
   * and IV size appropriate for your cipher
   * In this example we are using 256 bit AES (i.e. a 256 bit key). The
   * IV size for *most* modes is the same as the block size. For AES this
   * is 128 bits */
  if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv))
    handleErrors();

  /* Provide the message to be encrypted, and obtain the encrypted output.
   * EVP_EncryptUpdate can be called multiple times if necessary
   */
  if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
    handleErrors();
  ciphertext_len = len;

  /* Finalise the encryption. Further ciphertext bytes may be written at
   * this stage.
   */
  if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
  ciphertext_len += len;

  /* Clean up */
  EVP_CIPHER_CTX_free(ctx);

  return ciphertext_len;
}





int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
  unsigned char *iv, unsigned char *plaintext)
{
  EVP_CIPHER_CTX *ctx;

  int len;

  int plaintext_len;

  /* Create and initialise the context */
  if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

  /* Initialise the decryption operation. IMPORTANT - ensure you use a key
   * and IV size appropriate for your cipher
   * In this example we are using 256 bit AES (i.e. a 256 bit key). The
   * IV size for *most* modes is the same as the block size. For AES this
   * is 128 bits */
  if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv))
    handleErrors();

  /* Provide the message to be decrypted, and obtain the plaintext output.
   * EVP_DecryptUpdate can be called multiple times if necessary
   */
  if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
    handleErrors();
  plaintext_len = len;

  /* Finalise the decryption. Further plaintext bytes may be written at
   * this stage.
   */
  if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) handleErrors();
  plaintext_len += len;

  /* Clean up */
  EVP_CIPHER_CTX_free(ctx);

  return plaintext_len;
}

int main(int argc, const char * argv[])
{
	
	char *sequenceReturned = MD5Sequence();
	cout<<sequenceReturned<<endl;

	unsigned char iv_enc[AES_BLOCK_SIZE], iv_dec[AES_BLOCK_SIZE];
 	unsigned char aes_Key[16];
	memset(aes_Key, 0, 16);
	rand_gen(aes_Key);
	rand_gen(iv_enc);
	memcpy(iv_dec, iv_enc, AES_BLOCK_SIZE);
	

	size_t cipher_length = 0;


	string relativePath("");
	relativePath.append("testcase");
	int  size_file = 0;
	//string ptr("yihongdai");
	//const char * abc = "xxxx\0abcd\0";
	//ptr.append((const char *)"\0");
	//ptr.append((const char *)"x");
	//ptr.append((const char *)"x");
	//ptr.append(abc);	
	//unsigned char *plaintext = (unsigned char *)ptr.c_str();

	unsigned char *plaintext = write_Into_file_and_out(relativePath, size_file);

	int plaintext_len = size_file;

	printf("the string length is %d  string is %s \n", plaintext_len,plaintext );


	int encslength = ((plaintext_len + AES_BLOCK_SIZE) /AES_BLOCK_SIZE) * AES_BLOCK_SIZE;

	unsigned char ciphertext[encslength];

	//unsigned char * enc_out =  AesEncryption(aes_Key, iv_enc, iv_dec,cipher_length);
   	cipher_length = encrypt(plaintext,plaintext_len, aes_Key,iv_enc,ciphertext);
	
	write_Into_file_cipher(relativePath,ciphertext, cipher_length);
	

	
	// calculate the size of file
	



	unsigned char *plaintext2 = (unsigned char*)malloc(sizeof(char)*encslength);
	hex_print(ciphertext, cipher_length);
	
	int decslength = decrypt(ciphertext, cipher_length, aes_Key,
 	 iv_dec, plaintext2);
	
 	printf("encrypt:\t\n");
	for(int k = 0; k < strlen((const char *)ciphertext); k++){
		printf(" %c ", ciphertext[k]);
	}

	printf("decrypt:\t \n");
	printf("begining");
	for(int k = 0; k < strlen((const char *)plaintext); k++){
		printf("%c", plaintext[k]);
	}
	printf("Ending");
	
    return 0;
}

void write_Into_file_cipher(string &relativePath, unsigned char *ciphertext, int cipher_length )
{
	int fd = open((const char *)relativePath.c_str(),O_WRONLY);
	if(write(fd,(void *)ciphertext,cipher_length) == -1){
		fprintf(stderr,"error happens\n");
		exit(1);
	}
	close(fd);
	return;
}
unsigned char * write_Into_file_and_out (string &relativePath,int & size_file)
{
	writeIntoFile(relativePath);

	size_file = size_of_file(relativePath);

	printf("the size of file is%d \n" ,  size_file);

	void *buffer = (void *)malloc(sizeof(char)*size_file +1 );

	memset(buffer,0,sizeof(char)*size_file +1);

	int fd;

	fd = open((const char *)relativePath.c_str(),O_RDONLY);
	if(read(fd,buffer,size_file) == -1){
		fprintf(stderr,"error happens\n");
		exit(1);
	}
	close(fd);
	unsigned char * plaintext = (unsigned char *)buffer;
	printf("our buffer is %s \n", plaintext);
	return plaintext;
}


int size_of_file (string & relativePath)
{
	struct stat st;
	int size;
	stat(relativePath.c_str(), &st);
	size = (int)st.st_size;
	return size;
}
int writeIntoFile(string &relativePath)
{
	string bufferLine;
	ofstream newFile;
  	newFile.open(relativePath.c_str(),std::ofstream::out);	
	/* read file contents from the input file into the usr file
	 */
	int flag = 0;
	while(getline(cin,bufferLine)){
		if(flag !=0){
		 	newFile<<"\n";
		}
		flag = 1;
		newFile<<bufferLine;
	}
	newFile.close();
}
