#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <ctype.h>
#include <unistd.h>

/*
Morse Code translation

Morse code is a method used in telecommunications to encode text characters as standardized sequences of two different signal 
durations, called dots and dashes or dots (dot,.) And dash (dash, -). The Morse code is named after Samuel Morse, an inventor 
of the telegraph.

This program allows users to encode a message as Morse code and also to decode Morse code.

However, this program only translates basic ASCII characters [A-Z, 0-9]. So French characters are not included in the program.
*/

//Maxiumun size of table of characters that can be used (2¹⁴)
#define MAX_SIZE 16384

/*Morse code translation table
 letter = key
 *morse = Morsce code associates to the key
 */
static const struct
{
	char letter, *morse;
}Code[] = 
{
   { 'A', ".- "   },{ 'B', "-... " },{ 'C', "-.-. " },{ 'D', "-.. "  },
   { 'E', ". "    },{ 'F', "..-. " },{ 'G', "--. "  },{ 'H', ".... " },
   { 'I', ".. "   },{ 'J', ".--- " },{ 'K', ".-.- " },{ 'L', ".-.. " },
   { 'M', "-- "   },{ 'N', "-. "   },{ 'O', "--- "  },{ 'P', ".--. " },
   { 'Q', "--.- " },{ 'R', ".-. "  },{ 'S', "... "  },{ 'T', "- "    },
   { 'U', "..- "  },{ 'V', "...- " },{ 'W', ".-- "  },{ 'X', "-..- " },
   { 'Y', "-.-- " },{ 'Z', "--.. " },{ '0', "----- "},{ '1', ".---- "},
   { '2', "..--- "},{ '4', "....- "},{ '5', "..... "},{ '6', "-.... "},
   { '7', "--... "},{ '8', "---.. "},{ '9', "----. "},{ ' ', "  "    },	
};



/*
 Transform a text (string) into Morsce code.
 Every input will be turn into uppercase letter to simplify the translation

 string = the text that need to be translated into Morse code
*/
char* encode_string(const char *string)
{	
	size_t size = MAX_SIZE;
	char *encode = malloc(size);

	size_t i, j;
	for (i = 0; string[i]; ++i)
	{
		for( j = 0; j < (sizeof(Code) / sizeof(*Code)); ++j)
		{
			if(toupper(string[i]) == Code[j].letter)
			{
				strcat(encode, Code[j].morse);
				break;
			}
		}
	}

	return encode;

}

/*
 Transform a Morse code (a string of dot and dash) into string of text
 The size of each code will be compared with Morse code table to indentify the key

 morse = the string of Morse code that need to be translated into text
*/
char* decode_morse(const char *morse)
{
	size_t size = MAX_SIZE;
	char* decode = malloc(size);

	size_t i, j;

	for(i = 0; morse[i]; )
	{
		for(j = 0; j < sizeof(Code) / sizeof(*Code); ++j)
		{
			size_t size = strlen(Code[j].morse);
			if (memcmp(Code[j].morse, &morse[i], size) == 0)
			{
				strcat(decode, (char[2]){(char) Code[j].letter, '\0'});
				i += size;
				break;
			}
		}
	}

	return decode;
}

/*
 Display the translation table of the Morse code
*/
void print_morse_code_table()
{
	size_t i, j;
	char buff = 'A';

	putchar('\n');
	for(i = 0;i < sizeof(Code) / sizeof(*Code); ++i)
	{
		buff = (char) Code[i].letter;
		printf("	'%c' = \"%s\"\n", buff,Code[i].morse);
	}

	putchar('\n');

}

/*
 Display the help menu which specify all the functionalities of the program
 Inspired by python menu
*/
void print_commands()
{
	printf("\n%s\n", "Usage: ");
		printf("%s\n", "	./MonPG4 <commands> <input file> [optional]\n");
		printf("%s\n", "Commands: ");
		printf("%s\n", "	encode 						Convert a text (only characters A-Z and space are allowed) into Morse code");
		printf("%s\n", " 	decode 						Convert morse code into text (A-Z)");
		printf("%s\n", "	display 					Display Morse code table");
		printf("%s\n", " 	help 						Display help menu");
		printf("%s\n", "\n");
		printf("%s\n", "Optional: ");
		printf("%s\n", "	filename 					The result for the encode and the decode will be saved in the file named [filename]");
		printf("\n%s\n", "Examples: ");
		printf("%s\n", "	./MorseCode display 				Display the Morse code table");
		printf("%s\n", "	./MorseCode encode test.txt result_test 	Encode text from file test.txt into result_test file");
		printf("%s\n", "\n");
		printf("%s\n", "\n");
}

/*
 Replace all the occurences of a character into another character
*/
char* replace_char(char* str, const char find, const char replace){
    char *current_pos = strchr(str,find);
    while (current_pos) {
        *current_pos = replace;
        current_pos = strchr(current_pos,find);
    }
    return str;
}

/*
 Program main function, Morse Code translation
*/
int main(int argc, char const *argv[])
{
	/*
	 In case of not enough arguments are entered by the user
	*/
	if(argc < 3)
	{
		/*
		  The only 1 stand-alone allowed is "display" which it displays the Morse code table
		  else : display help menu
		*/
		if (argc == 2 && (strcmp(argv[1], "display") == 0))
		{
			print_morse_code_table();

		}
		else{
			print_commands();
		}
	}else{


		if (argc >= 3)
		{
			/*
			 Only 2 operations are allowed
			 encode : transform text into Morse code
			 decode : transform Morse code into text
			*/
			if ((strcmp(argv[1], "encode") == 0) || (strcmp(argv[1], "decode") == 0))
			{
				//Commands valid, begin operation

				//check if input file exists
				if (access(argv[2], F_OK) == 0)
				{
					//File exist

					//Input file
					FILE *file_read;
					//Output file
					FILE *file_write;
					char * content = 0;
					file_read = fopen(argv[2], "r");
					if (file_read)
					{
						//Get the size of string in the input file for memory allocation
						fseek(file_read, 0, SEEK_END);
						long length = ftell(file_read);
						fseek(file_read, 0, SEEK_SET);
						content = malloc(length + 1);
						//Read the file into string
						if (content)
						{
							fread(content, 1, length, file_read);

							//Remove breakline character since there is no Morsce code for breakline, in this program
							content = replace_char(content, '\n', ' ');

							//The lenght of the text/Morse code
							const int len = strlen(content);

							//Verify if the Morse code ends with a whitespace
							//If not, add one
							if ((strcmp(&content[len -1], " ") != 0))
							{
								strcat(content, " ");
							}

							//Local variable to store the result
							char* result_translated;

							//Text to Morse code
							if ((strcmp(argv[1], "encode") == 0))
							{
								 result_translated = encode_string(content);
							}//Morse code to text
							else{
								result_translated = decode_morse(content);
							}
							
							//Display the result onn terminal
							printf("%s\n", result_translated);

							//Write and open the file if output file is given
							if (argc == 4)
							{
								file_write = fopen(argv[3],"w+");
								//If file is opened/created succesfully, write into it
								if (file_write)
								{
									fputs(result_translated, file_write);
									fclose(file_write);
									
									//Open the file created using text editor
									const char* gedit = "gedit ";
									const char* option = " &";
									char* command = malloc(256);
									strcpy(command, "");

									strcat(command, gedit);
									strcat(command, argv[3]);
									strcat(command, option);
									system(command);

									free(command);
								}
							}

							free(result_translated);
						}
					}

					free(content);
					fclose(file_read);


				}
				//In case the input file cannot be found
				else{
					printf("File '%s' cannot be found in current directory\n", argv[2]);
				}

			}else{
				//Commands invalid
				print_commands();
			}
			
		}else{
			print_commands();
		}

	}

	return 0;

	
}