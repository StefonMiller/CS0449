//Stefon Miller
//Cs449 Recitation 11579

#include <stdio.h>
#pragma pack(1)
#include <math.h>

//First header in bitmap file
//magic - magic number, "BM" if valid file
//size - size of file
//reserved - reserved value
//reserved2 - second reserved value
//offset - offset to start of pixels
struct Header
{
	unsigned short int magic;
	unsigned int size;
	unsigned short int reserved;
	unsigned short int reserved2;
	unsigned int offset;

};

//Second header in bitmap file
//magic - size of dib header, 40 if valid
//width - width of image in pixels
//height - height of image in pixels
//panes - number of color panes
//bits - bits per pixel, 24 if valid
//scheme - compression scheme
//size - size of image
//xRes - horizontal resolution
//yRes - vertical resolution
//colors - colors in palette
//impColors - important colors in palette
struct DIBHeader
{
	unsigned int magic;
	int width;
	int height;
	unsigned short int planes;
	unsigned short int bits;
	unsigned int scheme;
	unsigned int size;
	int xRes;
	int yRes;
	unsigned int colors;
	unsigned int impColors;
};

//Pixel information
//b - blue
//g - green
//r - red
struct Pixel
{
	unsigned char b;
	unsigned char g;
	unsigned char r;
};

int main(int argc, char **argv)
{
	char *op;		//Operation user wants
	char *img;		//Image user wants to grayscale/invert
	struct Header head;	//First bmp header
	struct DIBHeader head2;	//DIB Header
	struct Pixel pixel;	//Pixel to be read in
	op = argv[1];		//Assign operation selection to op
	img = argv[2];		//Assign image file to img
	

	if(strcmp(op, "-grayscale") == 0)				//Branch here if user wants grayscale
	{	
		FILE *rfile = fopen(img, "r+b");			//Open file specified by user
		if(rfile == NULL)
		{
			printf("File not found");			//Tell user if file is not found
			return 1;
		}
		fread(&head, sizeof(head), 1, rfile);			//Read in first header
		if(head.magic == 19778)					//Make sure header is "BM"
		{
			fread(&head2, sizeof(head2), 1, rfile);		//Read in second header
			if(head2.magic == 40)				//Check DIB size
			{
				if(head2.bits == 24)			//Check pixel size
				{	
					int r;				//size of rows
					int c;				//size of columns
					fseek(rfile, head.offset, SEEK_SET);			//Move file pointer to offset in head
					for(r = 0; r < head2.height; r++)			//Iterate through rows
					{
						for(c = 0; c < head2.width; c++)		//Iterate through columns
						{
							fread(&pixel, sizeof(pixel), 1, rfile);	//Read in pixel info
							fseek(rfile, -3, SEEK_CUR);		//Reset file for write operation
							float nR = (float)pixel.r / 255;	//Normalize RGB values
							float nG = (float)pixel.g / 255;
							float nB = (float)pixel.b / 255;
							float y = (.2126 * nR) + (.7152 * nG) + (.0722 * nB);	//Get y value
							if( y <= .0031308)					//Branch depending on y value
							{
								nR = 12.92 * y;					//If y <= .0031308, set all values to 12.92, * by 255 and caste back in
								nG = nR;
								nB = nR;
								pixel.r = (unsigned char) (nR * 255);
								pixel.g = (unsigned char) (nG * 255);
								pixel.b = (unsigned char) (nB * 255);
							}
							else							
							{
								nR = 1.055 * (pow(y, (1/2.4))) - .055;		//If y > .0031308, use equation given, multiply by 255 and case back in
								nG = nR;
								nB = nR;
								pixel.r = (unsigned char)(nR * 255);
								pixel.g = (unsigned char)(nG * 255);
								pixel.b = (unsigned char)(nB * 255);

							}
							fwrite(&pixel, sizeof(pixel), 1, rfile);		//Write grayscaled pixel back in
						}
						if(((head2.width*3)%4)!=0)					//Check if the file needs padding
						{
							int pack;						//if it needs padding, pad number of bytes needed
							pack = 4-((head2.width*3)%4);
							fseek(rfile, pack, SEEK_CUR);
						}	
						
					}
				}
				else										//If pixel size is not 24, print error
				{
					printf("File not supported(Pixel data not supported), please try again\n");
					return 1;
				}
			}
			else											//If DIB header size is not 40, print error
			{
				printf("File not supported(DIB Header size not 40), please try again\n");
				return 1;
			}
			
		}
		else												//If header is not BM, print error
		{
			printf("File not supported(Header is not \"BM\", please try again\n");
			return 1;
		}
		printf("Grayscaling complete, please check your file!\n");					//Confirmation that grayscaling is done
		fclose(rfile);											//Close file once done

	}
	else if(strcmp(op, "-invert") == 0)									//Branch here if user wants to invert
	{
                FILE *rfile = fopen(img, "r+b");        							//Open file specified by user
                if(rfile == NULL)
                {
                        printf("File not found");       							//Tell user if file is not found
                        return 1;
                }
                fread(&head, sizeof(head), 1, rfile);   							//Read in first header
                if(head.magic == 19778)                 							//Make sure header is "BM"
                {
                        fread(&head2, sizeof(head2), 1, rfile);							//Read in second header
                        if(head2.magic == 40)									//Make sure DIB header size is 40
                        {
                                if(head2.bits == 24)								//Make sure pixel size is 24
                                {
                                        int r;									//Number of rows
                                        int c;									//Number of columns
                                        fseek(rfile, head.offset, SEEK_SET);    				//Move file pointer to offset in head
                                        for(r = 0; r < head2.height; r++)					//Iterate through rows
                                        {
                                                for(c = 0; c < head2.width; c++)				//Iterate through columns
                                                {
                                                        fread(&pixel, sizeof(pixel), 1, rfile); 		//Read in pixel info
                                                        fseek(rfile, -3, SEEK_CUR);            			//Reset file for write operation
							pixel.r = ~pixel.r;					//Bitwise not r, g, and b values
							pixel.g = ~pixel.g;
							pixel.b = ~pixel.b;
                                                        fwrite(&pixel, sizeof(pixel), 1, rfile);		//Write inverted rgb values back into bitmap
                                                }
                                                if(((head2.width*3)%4)!=0)					//Pad if needed
                                                {
                                                        int pack;				
                                                        pack = 4-((head2.width*3)%4);
                                                        fseek(rfile, pack, SEEK_CUR);
                                                }

                                        }
                                }
                                else										//Display eror if pixel size is not 24
                                {
                                        printf("File not supported(Pixel data not supported), please try again\n");
					return 1;
                                }
                        }
                        else											//Display error if DIB header size is not 40
                        {
                                printf("File not supported(DIB Header size not 40), please try again\n");
                                return 1;
                        }

                }
                else                                    							//Display error if header is not BM
                {
                        printf("File not supported(Header is not \"BM\", please try again\n");
                        return 1;
                }
		printf("Inversion complete, please check your file!\n");					//Confirmation that inversion is complete
                fclose(rfile);                         								 //Close file once done




        }
	else													//Print error if op isn't either -grayscale of -invert
	{
		printf("Invalid operation, please try again\n");
		return 1;	
	}

	return 0;



}
