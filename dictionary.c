#include <stdbool.h>
#include "dictionary.h"
#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
//represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 1000000; 
unsigned int hash_value = 0; // initialise positive hash value
unsigned int word_count = 0; // initialise positive hash value word count
node *table[N]; // Hash table

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    int hashed = hash(word);
    
    if (table[hashed] == NULL)
    {
        return false;
    }

    node *pointer = table[hashed];

    while (strcasecmp(pointer->word, word) != 0)
    {
        if (pointer->next == NULL)
        {
            return false;
        }
        pointer = pointer->next;
    }
    return true;
}
// Hashes word to a number
unsigned int hash(const char *word) // This hash function was first reported by Dan Bernstein of York University in Toronto, Ontario. The hash function shown was later customized for use in Problem Set 5 of 2020.
{
    unsigned int hash = 5381;
    int c;

    while ((c = *word++))        // *str++ is going to the next address in memory, where the next char in the string is stored
    {
        if (isupper(c))
        {
            c = c + 32;
        }

        hash = ((hash << 5) + hash) + c; // hash * 33 + c   // hash << 5 = hash * 2^5
    }

    return hash % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r"); // Open dict 
    if (file == NULL) // If file is not opened, return false
    {
        return false;
    }
    
    char word[LENGTH + 1]; // storage space for word
    
    while (fscanf(file, "%s", word) != EOF) // Scan dict for strings that are not the end of the file
    {
        node *n = malloc(sizeof(node)); // Allocate memory for new node
        
        if (n == NULL) // If malloc returns NULL, return false
        {
            return false;
        }
        
        strcpy(n->word, word); // Pointer to next node and word itself
        hash_value = hash(word); // Hash the word to get hash value
        n->next = table[hash_value]; // Set new pointer
        table[hash_value] = n; // Set head to new pointer
        word_count++; // Increment word count
    }
    
    fclose(file); // Close file
    
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
   if (word_count > 0)
    {
        return word_count;
    }
    return 0;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // Iterate through buckets
    for (int i = 0; i < N; i++)
    {
        // Set pointer to this each bucket location  
        node *pointer = table[i];
        
        while (pointer != NULL) // If pointer is not NULL, free
        {
            node *tmp = pointer; // Create apointer called temp 
            pointer = pointer->next; // Move pointer to next node
            free(tmp);  // Free up temp
        }
        if (i == N - 1 && pointer == NULL) // If pointer is NULL
        {
            return true; //sus
        }
    }
    return false;
}
