/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdio.h>
#include <stdlib.h>
#include "fileHandlers.h"
#include "clientHandlers.h"

void insert(client** a, char* username, char* password){
    if(*a==NULL){
    *a= (struct client*) malloc( sizeof( client) );
    (*a)->username= username;
    (*a)->password=password;
    (*a)->used=0;
    (*a)->ipAddr =NULL;
    (*a)->left= NULL;
    (*a)->right= NULL;
    (*a)->lastSession= NULL;
   // (*a)->socket=NULL;
    (*a)->portAddress = NULL;
    (*a)->socketID=0;
    }
    else if(strcmp((*a)->username,username)>0)
    {
        insert(&(*a)->left ,  username, password);
    }
    else if(strcmp((*a)->username,username)<0)
    {
         insert(&(*a)->right ,  username, password);
    }
}

char*  getActiveClients(client* tree){
    char* list="";
   if(tree!=NULL){
    
      if(!(tree->left==NULL)){
         list= combine_chars(list,getActiveClients(tree->left));
       }
      if(tree->used){
      list= combine_chars(list, " ");
      list= combine_chars(list, tree->username);
      } 
     if(!(tree->right==NULL)){
         list= combine_chars(list,getActiveClients(tree->right));
       }
   }
    return list;
}

void printAllClients(client* tree){

   if(tree!=NULL){
    
      if(!(tree->left==NULL)){
        printAllClients(tree->left);
       }
        printf( "Username is %s, password is %s",tree->username,tree->password);
        
     if(!(tree->right==NULL)){
        printAllClients(tree->right);
       }
   }
}

void printActiveClients(client* tree){

   if(tree!=NULL){
    
      if(!(tree->left==NULL)){
        printActiveClients(tree->left);
       }
      if(tree->used==1)
        printf( "\nUsername is %s \n",tree->username);
        
     if(!(tree->right==NULL)){
        printActiveClients(tree->right);
       }
   }



}


int checkLogin(client* a,char * name,char * pass){
    if(a!=NULL){
   
            if(strcmp(a->password,pass)==0){
                
                if(a->used){
                    return LOGINALREADY;
                }
                a->used=1;
                return LOGINSUCCESS;
            }
            return BADPASS;
    }
    
    return NOUSER;
}

int logOut(client* tree,char * name){
  client* a=search(tree,name);
  
  if(a==NULL){
      printf(" \n client does not exist \n");
      return 0;
  }
  // do stuff to cleint info
  
  
  
  a->used=0;
  return 1;
}






client* search(client* tree,char * name){

    if(tree!=NULL){
    
        if(strcmp(tree->username,name)==0){
            return tree;
        }
          if(strcmp(tree->username,name)>0){
            return search( tree->left,name);
        }
        
          if(strcmp(tree->username,name)<0){
             return search( tree->right,name);
        }
    }
    
    return NULL;
}



void processClientList(client**  a,char* filePath){

    int exists= checkFileExist(filePath);
    if(exists){
    char line[256];
     FILE *fileptr= fopen(filePath, "rb");
     
      while (fgets(line, sizeof(line), fileptr)) {
          int start=0, end=0;
          findBounds(&start, & end, ':', line);
          char* username= getSubString(start, end-1, line);
          findBounds(&start, & end, '\n', line);
          char* password= getSubString(start, end-1, line);
          insert(a, username,  password);
    }
    }
    else{
        printf(" fail to open");
    }
}




void changeClientList(client**  a, FILE *fileptr){
    if((*a)!=NULL&&fileptr!=NULL){
    if((*a)->left!=NULL){
    changeClientList(&(*a)->left,fileptr);
    }
  if((*a)->right!=NULL){
     changeClientList(&(*a)->right,fileptr);
    }
  
    char* info= (*a)->username;
    printf(info);
    info= combine_chars(info, ":");
    info= combine_chars(info, (*a)->password);
    info= combine_chars(info, "\n");
    fputs(info,fileptr);
    }
}
