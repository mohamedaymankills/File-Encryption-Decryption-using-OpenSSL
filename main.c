#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rand.h>

void handleErrors(void);

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
  unsigned char *iv, unsigned char *ciphertext);

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
  unsigned char *iv, unsigned char *plaintext);

int main(int argc, char *argv[]) {
  // Initialize OpenSSL
  OpenSSL_add_all_algorithms();
  ERR_load_crypto_strings();

  // Generate a random key and IV
  unsigned char key[EVP_MAX_KEY_LENGTH];
  unsigned char iv[EVP_MAX_IV_LENGTH];

  if(!RAND_bytes(key, 16) || !RAND_bytes(iv, 16)) {
    handleErrors();
  }

  // Open the input file
  FILE* infile = fopen(argv[1], "rb");
  if (!infile) {
    printf("Failed to open input file %s\n", argv[1]);
    return 1;
  }

  // Calculate the input file size
  fseek(infile, 0, SEEK_END);
  int filesize = ftell(infile);
  fseek(infile, 0, SEEK_SET);

  // Allocate a buffer for the input file data
  unsigned char* plaintext = (unsigned char*) malloc(filesize + 1);

  // Read the input file data
  fread(plaintext, 1, filesize, infile);

  // Encrypt the plaintext
  int ciphertext_len = encrypt(plaintext, filesize, key, iv, plaintext);

  // Open the output file
  FILE* outfile = fopen(argv[2], "wb");
  if (!outfile) {
    printf("Failed to open output file %s\n", argv[2]);
    return 1;
  }

  // Write the ciphertext to the output file
  fwrite(plaintext, 1, ciphertext_len, outfile);

  // Free memory
  fclose(infile);
  fclose(outfile);
  free(plaintext);
  EVP_cleanup();
  ERR_free_strings();

  return 0;
}

void handleErrors(void) {
  fprintf(stderr, "Error encrypting/decrypting data\n");
  ERR_print_errors_fp(stderr);
  abort();
}

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
  unsigned char *iv, unsigned char *ciphertext) {
  EVP_CIPHER_CTX *ctx;

  int len;
  int ciphertext_len;

  if(!(ctx = EVP_CIPHER_CTX_new())) {
    handleErrors();
  }

  // ... rest of the function

  return ciphertext_len;
}
