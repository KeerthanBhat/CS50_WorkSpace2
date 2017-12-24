/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

#include "dictionary.h"

// prototype of the function for hashing 
int hash ( const char *word );

//structure for linked list
typedef struct node
{
    char word[ LENGTH + 1 ];
    struct node *next;
}
node;

//hashtable declaration
node *hashtable[27];

//dictionary size variable
unsigned int dic_size = 0;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    
    //temporary string
    char temp[LENGTH + 1];
    int len = strlen(word);
    
    //which holds the word, all alphabets in lower case
    for( int i = 0; i < len; i++ )
        temp[i] = tolower( word[i] );
    temp[len] = '\0';
    
    //call the hashing function to get the index
    int index = hash(temp);

    //if table is empty at that index
    if ( hashtable[index] == NULL )
    {
        return false;
    }
    
    //temporary node just for comparison
    node* cursor = hashtable[index];
    
    while ( cursor != NULL )
    {
        
        //compare
        if ( strcmp( temp, cursor->word ) == 0)
        {
            return true;
        }
        
        //increment
        cursor = cursor->next;
    }
    
    return false;
}

int hash ( const char *word )
{
    int n;
    for ( int i = 0; word[i] != '\0'; i++ )
    {
        if( isalpha( word[i] ) )
        n = word [i] - 'a';
        else
        n = 26;
    }
    
    
    //returns the index ( a=0, b=1, c=2 and so on )
    return n;    
}


/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    
    //open the dictionary
    FILE *dic = fopen( dictionary, "r" );
    if( dic == NULL )
    {
        return 0;
    }
    
    char dword[ LENGTH + 1 ];
    
    //copy words from the dictionary
    while( fscanf( dic, "%s\n", dword ) != EOF )
    {
        dic_size++;
        
        //allocate memory
        node *new = malloc( sizeof(node) );
        
        if( new == NULL )
        {
            unload();
            return 0;
        }
        
        strcpy( new->word, dword );
        int index = hash(dword);
        
        //for the first word in some index
        if ( hashtable[index] == NULL )
        {
            hashtable[index] = new;
            new->next = NULL;
        }
        
        //for consecutive words in an index
        else
        {
            new->next = hashtable[index];
            hashtable[index] = new;
        }      
    }
     
    fclose( dic );    
    return 1;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    
    //previously determined dictionary size
    if( dic_size > 0 )
    return dic_size;
    
    return 0;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    int index = 0;
    
    while ( index < 27 )
    {
        
        //if table is empty at some index just goto the next index
        if ( hashtable[index] == NULL )
        {
            index++;
        }
        else
        {
            
            //deallocate memory of all the linked lists in an index
            while( hashtable[index] != NULL )
            {
                node* cursor = hashtable[index];
                hashtable[index] = cursor->next;
                free( cursor );
            }
        
            index++;
        }
    }
    
    return true;
}
