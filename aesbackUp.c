unsigned char * AesEncryption(unsigned char *aes_key, unsigned char *iv_enc, unsigned char *iv_dec, size_t &cipher_length){
 	 /*initilize dont care*/
	 ERR_load_crypto_strings();
 	 OpenSSL_add_all_algorithms();
 	 OPENSSL_config(NULL);
//////////////////////////////////////////////////////////////

	AES_KEY encryptionkey;
	int keylength = 128;
    	memset(aes_key, 0, keylength/8);
	

	rand_gen(aes_key);
    	//if (!RAND_bytes(aes_key, keylength/8))
        //	exit(-1);
        /*  inputslength needs attention here!!!!!!!!!!!!
	*
	*   we need user input from the shell!!!!!!!!!!!!
	*/
	size_t inputslength = 0;
	string replace("Iamyihongdai123456789abcdefghijklmnopqrstuvwxyz");
	
	for (int ji = 0 ; ji < 10; ji++)
		replace.append("haha");
	inputslength = replace.length();
	printf("the original text length is %d\n", replace.length()); 

	/* copy the to aesinput
	*
	*/
	unsigned char aesInput[inputslength];
	memcpy((void *)aesInput,(void *)replace.c_str(),inputslength);


   	 /* init vector */
   	//RAND_bytes(iv_enc, AES_BLOCK_SIZE);
	rand_gen(iv_enc);
	memcpy(iv_dec, iv_enc, AES_BLOCK_SIZE);


///////////////////////////////////////////////////////////////////
	size_t encslength = ((inputslength + AES_BLOCK_SIZE) /AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
	cipher_length = encslength;

	printf("the encslength is %d \n",encslength);
    	unsigned char *enc_out = (unsigned char *)malloc(sizeof(char)*encslength+1);

    	memset(enc_out, 0, encslength+1);
    	AES_set_encrypt_key(aes_key, keylength, &encryptionkey);
	AES_cbc_encrypt(aesInput, enc_out, inputslength, &encryptionkey, iv_enc,AES_ENCRYPT);
   	
	printf("the encrypted message length is %d \n", strlen((const char *)enc_out));
///////////////////////////////////////////////////////////////////////

	hex_print(enc_out,encslength);


	EVP_cleanup();

  /* if you omit the next, a small leak may be left when you make use of the BIO (low level API) for e.g. base64 transformations */
  	CRYPTO_cleanup_all_ex_data();

  /* Remove error strings */
  	ERR_free_strings();
	return enc_out;
}


unsigned char *AesDecryption(unsigned char * aes_key, unsigned char *iv_dec, unsigned char * cipher_text, size_t fileSize)
{

 	ERR_load_crypto_strings();

 	OpenSSL_add_all_algorithms();

 	OPENSSL_config(NULL);
	int keylength = 128;
	// watch out for the last byte to be '\0';
	size_t decslength = fileSize;
	AES_KEY decryptionkey;

	unsigned char *dec_out = (unsigned char *)malloc(decslength*sizeof(char)+1);
	memset(dec_out,0,decslength*sizeof(char)+1);
	AES_set_decrypt_key(aes_key, keylength, &decryptionkey);
    	AES_cbc_encrypt(cipher_text, dec_out, decslength, &decryptionkey, iv_dec, AES_DECRYPT);
	printf("the hexdeximal for cipher text is \n");	
	hex_print(dec_out, decslength);
	printf("--------------------ending \n");	
	dec_out = (unsigned char *) realloc((void*)dec_out,strlen((const char *)dec_out)+1);
	

	printf("the cipher text length is %d ", strlen((const char *)cipher_text));
	printf("the strlen is %d ", (int)strlen((const char *)dec_out));
	EVP_cleanup();

  /* if you omit the next, a small leak may be left when you make use of the BIO (low level API) for e.g. base64 transformations */
  	CRYPTO_cleanup_all_ex_data();

  /* Remove error strings */
  	ERR_free_strings();
	return dec_out;
}


