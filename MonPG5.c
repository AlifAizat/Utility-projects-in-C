#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**

XOR_Encryption

The XOR operator is extremely common as a component in more complex ciphers. By itself, by using a constant repeating key, 
a simple XOR cipher can be trivially broken using the brute force algorithm.

This program allows users to encrypt a message, a text or even a password with a key (positive integer> 0). 
The message cannot be decrypted without knowing the key value used during its encryption, but since this 
is a basic program, there will be a possibility to find the key value to use (eg: brute force algorithm).
**/

//Maxiumun size of table of characters that can be used (2¹³)
#define MAX_SIZE 8192

/*
 Encryption algorithm that is used to encode and decode the string
 The encryption output can be used again using the same key value to obtain the original text, hence, the key is the most
 important value

 inputFile  = the file that contain the text that needs to be encrypted
 outputFile = the file where the encrypted/decrypted value will be saved
 key        = a string of key (digit) that is used for the XOR operation 
*/
void xor_encryption(const char* inputFile, const char* outputFile, const char* key)
{
   FILE *input  = fopen(inputFile, "rb");
   FILE *output = fopen(outputFile, "wb");
   if ( input != NULL && output != NULL )
   {
      unsigned char buffer[MAX_SIZE];
      size_t count, i, j = 0;
      do {
         count = fread(buffer, sizeof *buffer, sizeof buffer, input);
         for ( i = 0; i < count; ++i )
         {  
            //XOR logic between i'th postition of the text and j'th postion of the key (ex: 3 of 1234) 
            buffer[i] ^= key[j++];
            if ( key[j] == '\0' )
            {
               //restart at the beginning of the key
               j = 0; 
            }
         }
         fwrite(buffer, sizeof *buffer, count, output);
      } while ( count == sizeof buffer );
      fclose(input);
      fclose(output);
   }
}

/*
 Display the help menu which specify all the functionalities of the program
 Inspired by python menu
*/
void print_commands()
{

   printf("\n%s\n", "This is an encrpytion program using XOR logic. A file encrypted must be decypted using the same key value");
   printf("%s\n", "in order to obtain the original result");

   printf("\n%s\n", "Usage: ");
      printf("%s\n", "  ./MonPG5 <input file> <output file> [key]\n");
      printf("%s\n", "Key: ");
      printf("%s\n", "  Any numbers             the key use for the encrpytion");
      printf("\n%s\n", "Examples: ");
      printf("%s\n", "  ./XOR buffer file.bin 57891         Encrypt the content in buffer file into file.bin with 57891 as the key");
      printf("%s\n", "  ./XOR file.bin file.txt 57891       Decrypt the content in file.bin file into file.txt with 57891 as the key");
      printf("%s\n", "\n");
      printf("%s\n", "\n");
}

/*
 A function to verify if the key entered by a user is a string of integer
*/
int keyIsNumber(const char* key)
{
   int isNumber = 1;

   const int len = strlen(key);

   for (int i = 0; i < len; ++i)
   {
      if(!(key[i] >= '0' && key[i] <= '9'))
      {
         isNumber = 0;
      }
   }

   return isNumber;
}

/*
 Main function, XOR encryption
*/
int main(int argc, char const *argv[])
{
   if ( argc == 4 )
   {
      const char* input = argv[1];
      const char* output = argv[2];
      const char* key = argv[3];

      //Verify if the key entered by a user is a string of integer
      if (keyIsNumber(key))
      {
         xor_encryption(input, output, key);
      }else{
         printf("\n%s\n", "!!!!  The key must be a digit number !!!! \n\n");
         print_commands();
         }

   }else{
      print_commands();
   }

   return 0;
}