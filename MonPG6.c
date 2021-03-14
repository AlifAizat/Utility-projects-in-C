#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*
 An image filter is a technique by which the size, colors, shading, and other characteristics of an image are changed. 
 An image filter is used to transform the image using different graphic editing techniques. Image filters are typically 
 done through graphic design and editing software.

 This program allows users to filter a .jpg image in black-white (grayscale) and sepia (sepia) format. There are also 
 other filters, but they are not implemented in this program.

 To carry out this program, I use 3 libraries created by Mr. Sean BARRETT for the manipulation of images. I use them 
 in order to load a .jpg file to a pixel array where it will do the modification of its RGB values and then convert it 
 and save as new image.

 https://github.com/nothings/stb
*/


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image/stb_image_resize.h"


/*
 A function to concat 2 strings since C is crazy language
*/ 
char* concat(const char *s1, const char *s2)
{
    const size_t len1 = strlen(s1);
    const size_t len2 = strlen(s2);
    char *result = malloc(len1 + len2 + 1);
    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1);

    return result;
}

/*
 A grayscale filter is applied to the to the pixel or the image
 A for loop will goes through the pixel array, according the dimension and the channel of the image
 Grayscale filter : Every pixel values are divided by 3

 input: the name/path of the .jpg image that need to be applied with grayscale filter
*/
void gray_filter(const char* file)
{
	//Image file
	int width, height, channels;
	//Load the pixel array using the function from stb_image
	unsigned char* img = stbi_load(file, &width, &height, &channels, 0);

	//The file cannot be loaded
	if (img == NULL)
	{
		printf("%s\n", "Error in loading the image");
		exit(1);
	}
	printf("Loading image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels);

	size_t  img_size = width*height*channels;

	//Determine the channel of the new file
	// 4? 		= 2 new channels for the image with 4 channels (ex: CMYK)
	// else, 3? = Only 1 channel for the image with 3 channels (ex: RGB)
	int new_channels = channels == 4? 2 : 1;
	size_t new_image_size = width*height*new_channels;

	//Allocate the memory for the new image
	unsigned char* gray_img = malloc(new_image_size);

	//Apply the filter to the pixels of the image
	//A pixel is derrived from 3 value
	// *p 		: R
	// *(p+1)	: G
	// *(p+2)	: B
	for(unsigned char *p = img, *pg = gray_img; p != img + img_size; p += channels, pg += new_channels)
	{
		*pg = (uint8_t)((*p + *(p+1) + *(p+2))/3.0);
		if (channels == 4)
		{
			*(pg +1) = *(p + 3);
		}
	}

	//define the new namefile for the new image
	char* gray_img_file = concat("gray_", (char*)file);

	//write the the new image file using stb function
	stbi_write_jpg(gray_img_file, width, height, new_channels, gray_img, 100);


	//Liberate the allocated memories
	stbi_image_free(img);
	free(gray_img);
	free(gray_img_file);

}

/*
 A sepia filter is applied to the to the pixel or the image
 A for loop will goes through the pixel array, according the dimension and the channel of the image
 Sepia filter : https://stackoverflow.com/questions/1061093/how-is-a-sepia-tone-created

 input: the name/path of the .jpg image that need to be applied with sepia filter
*/
void sepia_filter(const char* file)
{
	//Image file
	int width, height, channels;
	//Load the pixel array using the function from stb_image
    unsigned char *img = stbi_load(file, &width, &height, &channels, 0);

    //The file cannot be loaded
    if(img == NULL) {
        printf("Error in loading the image\n");
        exit(1);
    }
    printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels);


    size_t img_size = width * height * channels;
    //Determine the channel of the new file
	// 4? 		= 2 new channels for the image with 4 channels (ex: CMYK)
	// else, 3? = Only 1 channel for the image with 3 channels (ex: RGB)
    int gray_channels = channels == 4 ? 2 : 1;
    size_t gray_img_size = width * height * gray_channels;


    //Allocate the memory for the new image
    unsigned char *sepia_img = malloc(img_size);
    uint8_t R,G,B;

    //Apply the filter to the pixels of the image
    //For more information about the filter: https://stackoverflow.com/questions/1061093/how-is-a-sepia-tone-created
	//A pixel is derrived from 3 value
	// *p 		: R
	// *(p+1)	: G
	// *(p+2)	: B
    for(unsigned char *p = img, *pg = sepia_img; p != img + img_size; p += channels, pg += channels) 
    {
    	R = *p;
    	G = *(p + 1);
    	B = *(p + 2);

    	//R
        *pg       = (uint8_t)fmin(0.393 * R + 0.769 * G + 0.189 * B, 255.0); 

        //G
        *(pg + 1) = (uint8_t)fmin(0.349 * R + 0.686 * G + 0.168 * B, 255.0);

        //B
        *(pg + 2) = (uint8_t)fmin(0.272 * R + 0.534 * G + 0.131 * B, 255.0);   

        if(channels == 4) {
            *(pg + 3) = *(p + 3);
        }
    }

    //define the new namefile for the new image
    char* sepia_img_file = concat("sepia_", (char*)file);

    //write the the new image file using stb function
    stbi_write_jpg(sepia_img_file, width, height, channels, sepia_img, width * channels);

    //Liberate the allocated memories
    stbi_image_free(img);
    free(sepia_img);
    free(sepia_img_file);
}

/*
 Display the help menu which specify all the functionalities of the program
 Inspired by python menu
*/
void print_commands()
{

   printf("\n%s\n", "This is an image filtering program and for this version, only .jpg file is allowed");
   printf("%s\n", "and the number of filter is limited (only grayscale and sepia)");

   printf("\n%s\n", "Usage: \n");
      printf("%s\n", "  ./MonPG6 <filter> <input file> \n");
      printf("%s\n", "filter: \n");
      printf("%s\n", "  gray             A grayscale (or graylevel) image is simply one in which the only colors are shades of gray.");
      printf("%s\n", "                   The reason for differentiating such images from any other sort of color image is that less");
      printf("%s\n", "                   information needs to be provided for each pixel.");
      printf("%s\n", "  sepia            Sepia toning is a specialized treatment to give a black-and-white photographic print");
      printf("%s\n", "                   a warmer tone and to enhance its archival qualities.");
      printf("\n%s\n", "Examples: \n");
      printf("%s\n", "  ./MonPG6 sepia Holiday.jpg");
      printf("%s\n", "\n");
      printf("%s\n", "\n");
}

/*
 Get the extension of a file
 Ex: jpg, png, txt
*/
const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

/*
 Main function, image filtering
*/
int main(int argc, char const *argv[])
{
	
	/*
	 In case of not enough arguments are entered by the user
	*/
	if (argc < 3)
	{
		print_commands();
	}
	else{

		//Only jpg images are allowed
		if ((strcmp(get_filename_ext(argv[2]), "jpg") == 0) || (strcmp(get_filename_ext(argv[2]), "jpeg") == 0) || (strcmp(get_filename_ext(argv[2]), "JPEG") == 0) || (strcmp(get_filename_ext(argv[2]), "JPG") == 0))
		{
			//In case of gray filter
			if (strcmp(argv[1], "gray") == 0)
			{
				gray_filter(argv[2]);
			}
			//In case of sepia filter
			else if (strcmp(argv[1], "sepia") == 0)
			{
				sepia_filter(argv[2]);
			}
			else{
				printf("%s\n", "Only the filter  { gray, sepia } is allowed! ");
			}
		}else{
			printf("%s\n", "Only the file with { .jpg, .JPG, .jpeg, . JPEG } extension is allowed! ");
		}
	}

	return 0;
}
