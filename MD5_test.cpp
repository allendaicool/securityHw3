#include<stdio.h>
#include<openssl/md5.h>
#include<string.h>
#include<iostream>

using namespace std;

int main( int argc, char **argv )
{
        string input;
        cout<<"Please, enter the value you want to hash:";
        getline(cin, input);
        unsigned char *data = (unsigned char *)input.c_str();
        unsigned char md[16];
        int i;
        char tmp[3]={'\0'},buf[33]={'\0'};
        MD5(data,strlen((const char *)data),md);
        for (i = 0; i < 16; i++){
                sprintf(tmp,"%2.2x",md[i]);
                strcat(buf,tmp);
        }
        printf("%s\n",buf);
        return 0;
}