/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
# include "session.h"
# include "fileHandlers.h"
// command, person, 

int insertSession(session** a,  char* sessionName, char* username, int socketID,  struct sockaddr_in socket){
    if(*a==NULL){
    *a= (struct session*) malloc( sizeof( session) );
    (*a)->sessionName= sessionName;
    (*a)->left= NULL;
    (*a)->right= NULL;
     (*a)->person=NULL;
     return joinMember(&(*a)->person,username,socketID, socket);
   
    }
    else if(strcmp((*a)->sessionName, sessionName)>0)
    {
      return   insertSession(&(*a)->left ,   sessionName,username,socketID, socket);
       
    }
    else if(strcmp((*a)->sessionName, sessionName)<0)
    {
        return  insertSession(&(*a)->right ,  sessionName, username,socketID, socket);
        
    }
    else{
    return 0;
    
    }
}


int joinMember(member** a,char* username, int socketID,  struct sockaddr_in  socket){

    while(*a!=NULL&& strcmp((*a)->client,username)!=0){
    a= &(*a)->next;
    }
    if(*a==NULL){
       *a= ( member*) malloc( sizeof( member) );
    (*a)->next=NULL;
    (*a)->client=username;
    (*a)->socketID=socketID;
    (*a)->socket=socket;
    return 1;
    }
    return 0;
}



void printMemberList(member** a){

while(*a!=NULL){
    
    //printf("%s\n",(*a)->client);
    puts((*a)->client);
    a= &(*a)->next;
    }
}


session* searchSession(session* tree,char * name){

    if(tree!=NULL){
    
        if(strcmp(tree->sessionName,name)==0){
            return tree;
        }
          if(strcmp(tree->sessionName,name)>0){
            return searchSession( tree->left,name);
        }
        
          if(strcmp(tree->sessionName,name)<0){
             return searchSession( tree->right,name);
        }
    }
    
    return NULL;
}

int inMemberList(member** a,char* userName){
  while(*a!=NULL&& strcmp((*a)->client,userName)!=0){
    a= &(*a)->next;
    }
    if(*a==NULL){
    return 0;
    }
    return 1;
}

void printSessions(session* tree){

   if(tree!=NULL){
    
      if(!(tree->left==NULL)){
  printSessions(tree->left);
       }
      
      printf("\nSession ID is %s \n", tree->sessionName);
    printMemberList(&tree->person);                   
        
     if(!(tree->right==NULL)){
     printSessions(tree->right);
       }
   }
}

int sessionCheck(session*  tree,char* sessionName,char* userName){
session* a= searchSession( tree, sessionName);

if(a!=NULL)
return inMemberList(&tree->person,userName);

return 0;
}



int leaveSession(session* b ,char* username){

   if(b!=NULL){
   member** tree=&b->person;

    if(strcmp((*tree)->client,username)==0){       
        member* a= *tree;
        *tree=((*tree)->next);
    
         a->next=NULL;
        free(a);  
        
        if(*tree==NULL){
        // DELETE SESSION FROM TREE SINCE NO ONE IS A PART OF IT
        }
        
        return 1 ;
    }
    

    while(*tree!=NULL&&strcmp((*tree)->client,username)!=0){
    
        if((*tree)->next!=NULL&&strcmp((*tree)->next->client,username)==0){
         member* a= (*tree)->next;
         (*tree)->next=a->next;
         a->next=NULL;
         free(a); 
        
            return 1 ;
        }
        tree= &(*tree)->next;

    
    }
   }
    return 0;
    
}


int userLeave(member** group ,char* username){


    if(*group!=NULL){
    if(strcmp((*group)->client,username)==0){       
        member* a= *group;
        *group=((*group)->next);
    
         a->next=NULL;
        free(a);  
        
        if(*group==NULL){
        // DELETE SESSION FROM TREE SINCE NO ONE IS A PART OF IT
        }
        
        return 1 ;
    }
    

    while(*group!=NULL&&strcmp((*group)->client,username)!=0){
    
        if((*group)->next!=NULL&&strcmp((*group)->next->client,username)==0){
         member* a= (*group)->next;
         (*group)->next=a->next;
         a->next=NULL;
         free(a); 
        
            return 1 ;
        }
        group= &(*group)->next;

    
    }
    }
    return 0;
}



void removeUser(session* tree, char* username){

 if(tree!=NULL){
    
      if(!(tree->left==NULL)){
 removeUser(tree->left, username);
       }
      
  leaveSession(tree , username);                  
        
     if(!(tree->right==NULL)){
    removeUser(tree->right,username);
       }

}
 }

char*  getActiveSessions(session* tree){
    char* list="";
   if(tree!=NULL){
    
      if(tree->left!=NULL){
         list= combine_chars(list, getActiveSessions(tree->left));
       }
      list= combine_chars(list, " ");
      list= combine_chars(list, tree->sessionName);
     if(tree->right!=NULL){
         list= combine_chars(list, getActiveSessions(tree->right));
       }
   }
    return list;
}


 member* getMembers(session*  tree, char* sessionName){
 
 session* a= searchSession(tree,sessionName);
 if(a!=NULL)
 return a->person;
 return NULL;
 }
 

 void deleteSession(session**  tree, char* sessionName, char* username){
 }
 
