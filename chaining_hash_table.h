#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#define TRUE 1
#define FALSE 0

typedef struct Linked_entri
{
    char* key;
    char* data;
    struct Linked_entri* next;
}Linked_entri;

typedef struct
{
    unsigned int table_fullness;
    unsigned int current_table_size;
    unsigned int key_len;
    unsigned int data_len;
    Linked_entri** entries;
}Chaining_hashtable;

unsigned int hash_function(Chaining_hashtable*);
Linked_entri* CH_entri_create(unsigned int, unsigned int);
void CH_entri_delete(Linked_entri*);
Chaining_hashtable* CH_create(unsigned int, unsigned int, unsigned int);
void CH_delete(Chaining_hashtable*); //Delete from hashtable
void CH_resize(Chaining_hashtable*); //If size >= 0.5 - resize table x2
int CH_add(Chaining_hashtable*, char*, char*); //add element to table
char* CH_find(Chaining_hashtable*, char*); //search element
void CH_delete_elem(Chaining_hashtable* , char*); //delete elem from table


unsigned int hash_function(Chaining_hashtable* table)
{
    return (unsigned int)(table->table_fullness % table->current_table_size);
}

Linked_entri* CH_entri_create(unsigned int key_len,unsigned int data_len)
{
  Linked_entri* new_entri = (Linked_entri*)malloc(sizeof(Linked_entri*));
  new_entri->key = (char*)calloc(key_len,sizeof(char*));
  new_entri->data = (char*)malloc(sizeof(char*)*data_len);
  new_entri->next = NULL;
  return new_entri;
}

void CH_entri_delete(Linked_entri* entri)
{
    if(entri->next==NULL)
    {
        free(entri->key);
        free(entri->data);
        free(entri);
    }
    else
    {
        while(entri->next!=NULL)
        {
        Linked_entri* tmp = entri;
        while(tmp->next!=NULL)
            tmp = tmp->next;
        free(tmp->key);
        free(tmp->data);
        free(tmp);
        tmp=NULL;
        }
    }
    
}

Chaining_hashtable* CH_create(unsigned int default_table_size,unsigned int key_len,unsigned int data_len)
{
    Chaining_hashtable* new_table = (Chaining_hashtable*)malloc(sizeof(Chaining_hashtable*));
    new_table->key_len = key_len;
    new_table->data_len = data_len;
    new_table->current_table_size = default_table_size;
    new_table->table_fullness = 0;
    new_table->entries = (Linked_entri**)malloc(sizeof(Linked_entri*)*new_table->current_table_size);
    for (size_t i = 0; i < new_table->current_table_size; i++)
        new_table->entries[i] = CH_entri_create(new_table->key_len,new_table->data_len);
    return new_table;
}

void CH_delete(Chaining_hashtable* table)
{
    for (size_t i = 0; i < table->current_table_size; i++)
        CH_entri_delete(table->entries[i]);
    free(table->entries);
    free(table);
}

void CH_resize(Chaining_hashtable* table)
{
    table->current_table_size*=2;
    table->entries = (Linked_entri**)realloc(table->entries,sizeof(Linked_entri*) * table->current_table_size);
    for (size_t i = table->current_table_size/2; i < table->current_table_size; i++)
        table->entries[i] = CH_entri_create(table->key_len,table->data_len);
}

int CH_add(Chaining_hashtable* table, char* input_key,char* input_data)
{
    Linked_entri* tmp;
    unsigned int hash = hash_function(table);
    for (size_t i = 0; i < table->table_fullness; i++)
    {
        tmp = table->entries[i];
        if(tmp->next==NULL)
            if(strcmp(tmp->key,input_key)==0)
                return FALSE;
        else
            while(tmp->next!=NULL)
            {
                if(strcmp(tmp->key,input_key)==0)
                    return FALSE;
                tmp = tmp->next;
            }
    }

    if((double)table->table_fullness / (double)table->current_table_size >= 0.5)
        CH_resize(table);
    
    tmp = table->entries[hash];
    while(tmp->next!=NULL)
        tmp = tmp->next;
    if(tmp->key[0]!=0)
    {
        tmp->next=CH_entri_create(table->key_len,table->data_len);
        tmp = tmp->next;
    }
    for (size_t i = 0; i < table->key_len && i < strlen(input_key); i++)
        tmp->key[i] = input_key[i];
    for (size_t i = 0; i < table->data_len && i < strlen(input_data); i++)
        tmp->data[i] = input_data[i];
    table->table_fullness++;
    return TRUE;
}

char* CH_find(Chaining_hashtable* table,char* search_key)
{
    for (size_t i = 0; i < table->table_fullness; i++)
    {
        Linked_entri* tmp = table->entries[i];
        if(tmp->next == NULL)
            if(strcmp(search_key,tmp->key)==0)
                return tmp->data;
        while(tmp->next!=NULL)
        {
            if(tmp->next == NULL)
            {
                if(strcmp(search_key,tmp->key)==0)
                    return tmp->data;
            }
            else
                tmp = tmp->next;
        }
    }
    return "[CAN'T FIND KEY]";
}

void CH_delete_elem(Chaining_hashtable* table, char* search_key)
{
    for (size_t i = 0; i < table->table_fullness; i++)
    {
        Linked_entri* tmp = table->entries[i];
        if(strcmp(tmp->key,search_key)==0)
        {
                CH_entri_delete(tmp);
                table->entries[i] = CH_entri_create(table->key_len,table->data_len);
                table->table_fullness--;
        }
        else
            {
                while(tmp->next!=NULL)
                {
                    if(strcmp(tmp->next->key,search_key)==0)
                    {
                        Linked_entri* next_tmp = tmp->next->next;
                        CH_entri_delete(tmp->next);
                        tmp->next = next_tmp;
                        table->table_fullness--;
                    }
                    else
                        tmp = tmp->next;
                }
            }
    }
    
}