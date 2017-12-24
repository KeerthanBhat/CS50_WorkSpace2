#include<stdio.h>

int main( int argc, char *argv[] )
{
    //improper usage
    if( argc != 2 )
    {
        fprintf( stderr, "Usage: ./recover image" );
        return 1;
    }
    
    //opens the raw file
    FILE *f = fopen( argv[1], "r" );
    
    //incase the raw file doesn't exist
    if( f == NULL )
    {
        fprintf( stderr, "Cannot open the file!" );
        return 2;
    }
    
    //declarations
    int i = 0, count = 0;
    unsigned char buffer[512];
    char filename[8];
    FILE *img;
    
    
    //iterate over the 512 byte blocks until EOF
    while( fread( &buffer, 512, 1, f ) == 1 )
    {
        
        //when a new JPEG is encountered
        if( buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0 )
        {
           //when first JPEG is encountered
           if( count == 0 )
           {
               count = 1;
           }
           //else close the previous JPEG
           else
           {
               fclose(img);
           }
            
            //create a new JPEG file
            
            //to print something in a string
            sprintf( filename, "%03i.jpg", i++ );
            img = fopen( filename, "w" );
        }
       
       //write the contents into the JPEG file
       if( count == 1 )
       {
           fwrite( &buffer, 1, 512, img ); 
       }
    }
    
    //close all file pionters
    fclose(f);
    fclose(img);
    
    //success
    return 0;
}