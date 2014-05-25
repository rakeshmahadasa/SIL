%{
#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>

//STRUCTURES

struct ArgStruct {
int TYPE;
char *NAME;
bool ref_var;
struct ArgStruct *NEXT;
};

struct Lsymbol {

char *NAME; // Name of the Identifier

int TYPE; // TYPE can be INTEGER or BOOLEAN



int BINDING; // Address of the Identifier in Memory


/***Argstruct must store the name and type of each argument ***/

struct Lsymbol *NEXT; // Pointer to next Symbol Table Entry */

};

struct Gsymbol {

bool isFUNCTION ;
bool isDefined;

char *NAME; // Name of the Identifier

int TYPE; // TYPE can be INTEGER or BOOLEAN

/***The TYPE field must be a TypeStruct if user defined types are allowed***/

int SIZE; // Size field for arrays

int BINDING; // Address of the Identifier in Memory

struct ArgStruct *ARGLIST; // Argument List for functions

/***Argstruct must store the name and type of each argument ***/

struct Gsymbol *NEXT; // Pointer to next Symbol Table Entry */

};


struct node{
			int TYPE;
			int NODETYPE;
			char *NAME;
			int index;
			int value;
			struct Gsymbol *Gentry;
			struct Lsymbol *Lentry;			
			struct ArgStruct *ArgList;
			struct node *left;
			struct node *middle;
			struct node *right;
		};	


// INITIALIZATIONS

void Construct_Tree(struct node *, struct node *);
void Print_Tree_Code(struct node *);
void Print_Node_Code(struct node *);
void Print_Expr_Code(struct node *);
void Print_Iter_Code(struct node *);
int yylex();

int sym[100];
int registers=1;
int labels = 0;
int memory = 0;

FILE *fp = NULL;
struct node *p;
struct Gsymbol *head=NULL;
struct Gsymbol *gp = NULL;
struct Lsymbol *lHead=NULL;
struct Lsymbol *lp = NULL;


//INSTALLERS

struct Gsymbol *Glookup(char *NAME){
	struct Gsymbol *np;
	np = head;
	while(np!=NULL){
		if(strcmp(np->NAME, NAME) == 0){
			break;
		}	
		np = np->NEXT;
	}
	return np;
}

	
void Ginstall(char *NAME, int TYPE, int SIZE, bool isFunc, struct ArgStruct *arg ){
	struct Gsymbol *np;
	np =  malloc(sizeof(struct Gsymbol) );
	np->isFUNCTION = isFunc;
	np->isDefined = false;
	np->ARGLIST = arg;
	np->NAME = malloc(sizeof(char) * 10);
	strcpy(np->NAME, NAME);
	np->TYPE = TYPE;
	np->SIZE = SIZE;
	np->BINDING = memory;
	memory += SIZE;
	np->NEXT = head;
	head = np; 
}					



void Linstall(char *NAME, int TYPE, int BINDING){
	struct Lsymbol *np;
	np =  malloc(sizeof(struct Lsymbol) );
	np->NAME = malloc(sizeof(char) * 10);
	strcpy(np->NAME, NAME);
	np->TYPE = TYPE;
	np->BINDING = BINDING;
	np->NEXT = lHead;
	lHead = np; 
}					

struct Lsymbol *Llookup(char *NAME){
	struct Lsymbol *np;
	np = lHead;	
	while(np!=NULL){
		if(strcmp(np->NAME, NAME) == 0){
			break;
		}
		np = np->NEXT;
	}
	return np;
}

//FUNCTION CARETAKERS

void isFunctionsDefined()
{
gp = head;
while(gp)
{
	if(gp->isFUNCTION && !(gp->isDefined))
	{      
		yyerror("undefined function");
		exit(0);
	}
	gp = gp->NEXT;
}
}

bool Is_FuncDefMatch( struct ArgStruct *list1, struct ArgStruct *list2)
{
while ((list1 != NULL) && (list2!=NULL)){
	if((list1->TYPE != list2->TYPE) || (strcmp(list1->NAME, list2->NAME)) || (list1->ref_var != list2->ref_var))
	   {
	   return false;
	   }
	list1 = list1->NEXT;
	list2 = list2->NEXT;
	}
if (( list1 == NULL) && (list2 == NULL)) 
{
	return true;
}
return false;
}

bool Is_FuncCallMatch( struct ArgStruct *list1, struct node *list2)
{
	while (( list1 != NULL ) && ( list2!=NULL )){
		if (list1->TYPE != list2->TYPE) 
		   {
		   return false;
		   }
		if ((list1->ref_var == true) && ((list2->right)->NODETYPE != 1)) 
		   {
		   return false;
		   }	  
		list1 = list1->NEXT;
		list2 = list2->left;
	}
	if (( list1 == NULL) && (list2 == NULL)) 
	{
		return true;
	}
	return false;
}

//TREE

void Construct_Tree(struct node *Tree, struct node *n)
{
while(Tree->right != NULL)
{
Tree = Tree->right;
}  
Tree->right = n;
}


//CODE GENERATORS 

void Print_Iter_Code(struct node *root){
			registers++;
			
			int i;
			for(i=0; i<(registers-1); ++i){
				fprintf(fp, "PUSH R%d\n", i);		
		  	} 
			struct node *A, *TEMP;
			TEMP = NULL;
			A = root->left;
			
			if(A) {
			fprintf(fp, "PUSH R%d\n",0);
			A->middle = TEMP;
			TEMP = A;
			A = A->left;
			}
			while(A){
					A->middle = TEMP;
					TEMP = A;
					Print_Expr_Code(A->right);
					fprintf(fp, "MOV R%d, R%d \n",registers-2,registers-1);
					registers--;
					fprintf(fp, "PUSH R%d\n",registers-1);
					A = A->left;
				}
			fprintf(fp, "PUSH R0 \n"); 
			fprintf(fp, "CALL %s\n", root->NAME);
			fprintf( fp, "POP R%d\n",registers-1);
			A = TEMP;
			while (A)
			  {
				fprintf(fp, "POP R%d\n", (registers + 2));
				if ( A->NODETYPE == 1 ){
					if( (A->right)->Lentry ){
					  	fprintf(fp, "MOV R%d, BP\n", registers);
					  	fprintf(fp, "MOV R%d, %d\n",registers + 1,  ((A->right)->Lentry)->BINDING );
					  	fprintf(fp, "ADD R%d, R%d\n",registers,registers + 1);
					  	fprintf(fp, "MOV [R%d], R%d\n" ,registers,registers + 2);
					  }
					else{
					  	if((A->right)->middle == NULL){	
					  		fprintf(fp, "MOV [%d], R%d\n" , ((A->right)->Gentry)->BINDING, registers + 2);
					  	  }
					  	else{
					  		Print_Expr_Code((A->right)->middle);
					  		fprintf(fp, "MOV R%d, %d\n", registers, ((A->right)->Gentry)->BINDING );
					  		fprintf(fp, "ADD R%d, R%d\n", registers,registers-1);
					  		registers--;
					  		fprintf(fp, "MOV [R%d], R%d \n",registers+1,registers + 2);
					  }}}
			A = A->middle;
		}        
			 for(i=( (registers-1) -1 )  ; i>=0 ; --i){
			 	fprintf( fp, "POP R%d\n", i);
			   }

}


void Print_Tree_Code(struct node *root){
if(root == NULL) { return; }
else{
Print_Node_Code(root->left);
Print_Tree_Code(root->right);
}}

void Print_Node_Code(struct node *root){
              
	switch(root->NODETYPE)
	  {    
		case 1:
			if((root->left)->TYPE == 1 )
			  {
				if(root->middle != NULL)
				  {	
				 	registers++;
				    fprintf(fp, "MOV R%d, %d\n",registers-1, ((root->left)->Gentry)->BINDING );				
					Print_Expr_Code(root->middle);
					fprintf(fp, "ADD R%d, R%d\n", registers-2, registers-1);
					registers--;
					Print_Expr_Code(root->right);
					fprintf(fp, "MOV [R%d], R%d\n",registers-2, registers-1);
					registers--;
					registers--;
				}
				else
				  {
					if( (root->left) -> Lentry )
					  {
						registers++;
						fprintf(fp, "MOV R%d, BP\n",registers-1);
						registers++;
						fprintf(fp, "MOV R%d, %d\n",registers-1, ((root->left) -> Lentry)->BINDING );
						fprintf(fp, "ADD R%d, R%d\n",registers-2,registers-1);
						registers--;
						Print_Expr_Code(root->right);
						fprintf(fp, "MOV [R%d], R%d\n",registers-2,registers-1); 
						registers--;
						registers--;
				     }
					else 
					  {
					  	Print_Expr_Code(root->right);
						fprintf(fp, "MOV [%d], R%d\n", ((root->left)->Gentry)->BINDING,registers-1); 
						registers--;
					  }
				}
			}
			else  {
			   
			       if(root->middle != NULL)
				 {
					registers++;
				    fprintf(fp, "MOV R%d, %d\n", registers-1, ((root->left)->Gentry)->BINDING); 				
					Print_Expr_Code(root->middle);		
					fprintf(fp, "ADD R%d, R%d\n",registers-2, registers-1);
					registers--;
					Print_Expr_Code(root->right);
					fprintf(fp, "MOV [R%d], R%d\n", registers-2, registers-1);
					registers--;
					registers--;
				}
				else
				  {
					if( (root->left) -> Lentry )
					  {
				        registers++;	
						fprintf(fp, "MOV R%d, BP\n", (registers-1));				
						registers++;
						fprintf(fp, "MOV R%d, %d\n", (registers-1), ((root->left) -> Lentry)->BINDING);				
						fprintf(fp, "ADD R%d, R%d\n", (registers-2),  (registers-1) );
						registers--;
						Print_Expr_Code(root->right);				
						fprintf(fp, "MOV [R%d], R%d\n", registers-2,registers-1); 
						registers--;
						registers--;
					}	
					else 
					{
					
						Print_Expr_Code(root->right);
						fprintf(fp, "MOV [%d], R%d\n", ((root->left)->Gentry)->BINDING,registers-1); 
						registers--;
					  
					  }
				}
			}
			break;
		case 2:
			registers++;				
			fprintf(fp, "IN R%d \n", registers-1);
		
			if(root->middle != NULL)
			  {
				registers++;
			    fprintf(fp, "MOV R%d, %d\n", registers-1, ((root->left)->Gentry)->BINDING );	
				Print_Expr_Code(root->middle);
				fprintf(fp, "ADD R%d, R%d\n", registers-2, registers-1);
				registers--; 
				fprintf(fp, "MOV [R%d], R%d\n", registers-1, registers-2);
				registers--;
				registers--;			
			  }
			else
			  {
				if( (root->left)->Lentry) 
			          {
					registers++;
					fprintf(fp, "MOV R%d, BP\n", registers-1);
					registers++;
					fprintf(fp, "MOV R%d, %d\n", registers-1, ((root->left) -> Lentry)->BINDING);
					fprintf(fp, "ADD R%d, R%d\n",registers-2, registers-1);
					registers--;
					fprintf(fp, "MOV [R%d], R%d\n", registers-1,registers-2); 
					registers--;
					registers--;
				  }
				else
				  {
					fprintf(fp,"MOV [%d], R%d\n", ((root->left)->Gentry)->BINDING,registers-1);
					registers--;			
				  }
			  }
			break;
		case 3:
			Print_Expr_Code(root->left);	 
			fprintf(fp, "OUT R%d \n", registers-1);
			registers--;
			break;
		case 4:
			labels+=2;
			int if_end = labels-1;
			int else_end=labels;
			Print_Expr_Code(root->left);
			fprintf(fp, "JZ R%d, L%d \n", registers-1, if_end);
			registers--;
			Print_Tree_Code(root->middle);
			fprintf(fp, "JMP L%d \n",  else_end);
         	fprintf(fp, "L%d: \n", if_end);
			Print_Tree_Code(root->right);
			fprintf(fp, "L%d: \n" , else_end); 
			break;
		case 5:
			labels += 2;
			int labelStart = labels-1;
			int labelEnd = labels;
			fprintf(fp, "L%d: \n", labelStart);
			Print_Expr_Code(root->left);
			fprintf(fp, "JZ R%d, L%d \n", registers-1, labelEnd);
			registers--;
			Print_Tree_Code(root->right);
			fprintf(fp, "JMP L%d \n", labelStart);
			fprintf(fp, "L%d: \n", labelEnd);
			break;
	}
}



void Print_Expr_Code(struct node *root)
	{
	switch(root->NODETYPE)
	  {
	 case 1: 
			if(root->middle == NULL)
			  {
				 if(root -> Lentry)
				 {
	   			    registers++;
					fprintf(fp, "MOV R%d, BP \n",registers-1 );
					registers++;
					fprintf(fp, "MOV R%d, %d\n",registers-1, (root -> Lentry)->BINDING ) ;
					fprintf(fp, "ADD R%d, R%d\n",registers-2, registers-1 );
				    registers--;
					fprintf(fp, "MOV R%d, [R%d]\n",registers-1,registers-1);
				 }
				 else {
					registers++;
					fprintf(fp, "MOV R%d, [%d] \n", registers-1, ((root)->Gentry)->BINDING);
					}
			}		
			else {
				registers++;
			    fprintf(fp, "MOV R%d, %d \n",registers-1, ((root)->Gentry)->BINDING);
			    Print_Expr_Code(root->middle);	
				fprintf(fp, "ADD R%d, R%d\n", registers-2,registers-1);
				registers--;
				fprintf(fp, "MOV R%d, [R%d]\n",registers-1,registers-1);
			     }
			break;
			
	case 2:
			registers++;
			fprintf(fp, "MOV R%d, %d \n",registers-1, root->value);
			break;
			
	case 3:
			Print_Expr_Code(root->left);				
			Print_Expr_Code(root->right);
			fprintf(fp, "ADD R%d, R%d \n",registers-2,registers-1);
			registers--;
			break;

	case 4:
			Print_Expr_Code(root->left);				
			Print_Expr_Code(root->right);
			fprintf(fp, "MUL R%d, R%d \n",registers-2,registers-1);
			registers--;
			break;
	
	case 5:
			Print_Expr_Code(root->left);				
			Print_Expr_Code(root->right);
			fprintf(fp, "SUB R%d, R%d \n",registers-2,registers-1);
			registers--;
			break;

	case 6:
			Print_Expr_Code(root->left);				
			Print_Expr_Code(root->right);
			fprintf(fp, "DIV R%d, R%d \n",registers-2,registers-1);
			registers--;
			break;
	
	case 7:
			Print_Expr_Code(root->left);				
			Print_Expr_Code(root->right);
			fprintf(fp, "GE R%d, R%d \n",registers-2,registers-1);
			registers--;
			break;

	case 8:
			Print_Expr_Code(root->left);				
			Print_Expr_Code(root->right);
			fprintf(fp, "LT R%d, R%d \n",registers-2,registers-1);
			registers--;
			break;		

	case 9:
			Print_Expr_Code(root->left);				
			Print_Expr_Code(root->right);
			fprintf(fp, "GT R%d, R%d \n",registers-2,registers-1);
			registers--;
			break;
			
	case 10: 
			Print_Expr_Code(root->left);
			labels+=2;
			fprintf(fp, "JZ R%d, L%d\n", registers-1, labels-1 );
			fprintf(fp, "MOV R%d, 0 \n", registers-1);
			fprintf(fp, "JMP L%d \n", labels);
			fprintf(fp, "L%d: \n", labels-1);
			fprintf(fp, "MOV R%d, 1 \n", registers-1);		
			fprintf(fp, "L%d: \n", labels);
			break;	
			
	case 11:
		Print_Expr_Code(root->left);				
		Print_Expr_Code(root->right);
		fprintf(fp, "LE R%d, R%d \n",registers-2,registers-1);
		registers--;
		break;
		
	case 12:
		Print_Expr_Code(root->left);				
		Print_Expr_Code(root->right);
		fprintf(fp, "EQ R%d, R%d \n",registers-2,registers-1);
		registers--;
		break;
		
	case 13:
		Print_Expr_Code(root->left);				
		Print_Expr_Code(root->right);
		fprintf(fp, "ADD R%d, R%d \n",registers-2,registers-1);
		registers--;
		break;
		
	case 14:
		Print_Expr_Code(root->left);				
		Print_Expr_Code(root->right);
		fprintf(fp, "MUL R%d, R%d \n",registers-2,registers-1);
		registers--;
		break;		
	
	case 15:  
			registers++;
			int i;
			for(i=0; i<(registers-1); ++i){
				fprintf(fp, "PUSH R%d\n", i);		
		  	} 
			struct node *A, *TEMP;
			TEMP = NULL;
			A = root->left;
			while(A){
					A->middle = TEMP;
					TEMP = A;
					Print_Expr_Code(A->right);
					fprintf(fp, "MOV R%d, R%d \n",registers-2,registers-1);
					registers--;
					fprintf(fp, "PUSH R%d\n",registers-1);
					A = A->left;
				}
			fprintf(fp, "PUSH R0 \n"); 
			fprintf(fp, "CALL %s\n", root->NAME);
			fprintf( fp, "POP R%d\n",registers-1);
			A = TEMP;
			while (A)
			  {
				fprintf(fp, "POP R%d\n", (registers + 2));
				if ( A->NODETYPE == 1 ){
					if( (A->right)->Lentry ){
					  	fprintf(fp, "MOV R%d, BP\n", registers);
					  	fprintf(fp, "MOV R%d, %d\n",registers + 1,  ((A->right)->Lentry)->BINDING );
					  	fprintf(fp, "ADD R%d, R%d\n",registers,registers + 1);
					  	fprintf(fp, "MOV [R%d], R%d\n" ,registers,registers + 2);
					  }
					else{
					  	if((A->right)->middle == NULL){	
					  		fprintf(fp, "MOV [%d], R%d\n" , ((A->right)->Gentry)->BINDING, registers + 2);
					  	  }
					  	else{
					  		Print_Expr_Code((A->right)->middle);
					  		fprintf(fp, "MOV R%d, %d\n", registers, ((A->right)->Gentry)->BINDING );
					  		fprintf(fp, "ADD R%d, R%d\n", registers,registers-1);
					  		registers--;
					  		fprintf(fp, "MOV [R%d], R%d \n",registers+1,registers + 2);
					  }}}
			A = A->middle;
		}        
			 for(i=( (registers-1) -1 )  ; i>=0 ; --i){
			 	fprintf( fp, "POP R%d\n", i);
			   }
			break;							
	case 16:
			Print_Expr_Code(root->right->left->right);
			fprintf(fp, "MOV R0, R%d\n",registers-1);
			labels += 3;
			int labelStart = labels-1;
			int labelEnd = labels;
			int labelIter = labels-2;
			fprintf(fp, "L%d: \n", labelStart);
			Print_Iter_Code(root->right);
			fprintf(fp,"NE R%d,R%d\n",0,registers-1);
			fprintf(fp, "JZ R%d, L%d \n", 0, labelEnd);
			fprintf(fp, "MOV R0, R%d\n",registers-1);
			registers--;
			fprintf(fp, "JMP L%d \n", labelStart);
			fprintf(fp, "L%d: \n", labelEnd);
			break;
  }
}



%}

%union {
	struct node *np;
       }

%token <np> NUM  '+' '*' '(' ')' '\n' '=' '-' '/' '|' '&' '{' '}' '!' ';' ',' '<' '>' 
%token <np> ID READ WRITE EQL LE GE IF THEN ELSE ENDIF WHILE DO ENDWHILE DECL ENDDECL BEG END INT BOOL TRUE FALSE RETURN MAIN REF ITER
%type <np> expr stmt slist program dList decl Fdeflist Fdef TYPE G_Ids GDefBlock argList idList argList_notEmpty LDefblock ldList lDecl lVarList exprList exprListNotEmpty retStmt
%left '='
%left '|' '&'
%left '<' '>' EQL LE GE
%left '+' '-'
%left '*' '/'
%left '!'

%%

program:  GDefBlock  Fdeflist INT MAIN '(' ')' '{' LDefblock 
		           {   
			int lVars = 1;
			struct ArgStruct *AL;
			AL = $8->ArgList;	 
			while(AL){
				if(Llookup(AL->NAME ))
				{
					yyerror("Variable declared already");
					exit(0);
			  	}
			  	else {
					Linstall(AL->NAME, AL->TYPE, lVars);
					lVars++;
					AL = AL->NEXT;
					}		 
				  }
				lVars = lVars -1;
				fprintf(fp, "START\n");                                         	
                fprintf(fp, "MOV BP, %d\n", memory );
                fprintf(fp, "MOV SP, %d\n",  memory+lVars);
			}	                        
	BEG slist END '}' 
		{  
			isFunctionsDefined();
			Print_Tree_Code($11);
			fprintf(fp, "HALT\n");
			return 0;
        }
               
               
               

GDefBlock : DECL dList ENDDECL 
		{
			fp=fopen("output.txt", "w");						
			$$=NULL;
		}
		
		
dList: dList decl {}
		| {}
		;


decl: TYPE G_Ids ';' {}
	   ;	

TYPE : INT 
      | BOOL
      ;		

		
G_Ids :  
	ID
		{
			gp = Glookup($1->NAME);
			if(gp != NULL)
	      		  {
				yyerror("Variable declared already!");
				exit(0);
			  }                                       	
			Ginstall($1->NAME, $<np>0->TYPE, 1, false, NULL);
             	}
                                      	
	| G_Ids ',' ID  
		{
			gp = Glookup($3->NAME);
			if(gp != NULL)
			  {
				yyerror("Variable declared already!");
				exit(0);
                          }                                       	
			Ginstall($3->NAME, $<np>0->TYPE, 1, false, NULL);
      		}

	| ID '[' NUM ']'	
		{
			gp = Glookup($1->NAME);	
                   
			if(gp != NULL)
			  {
				yyerror("Variable declared already!");
				exit(0);
			  }                                     
                                     	
      			Ginstall($1->NAME, $<np>0->TYPE, $3->value, false, NULL);
		}
					
	| G_Ids ',' ID '[' NUM ']'  
		{
			gp = Glookup($3->NAME);
						
			if(gp != NULL)
			  {
				yyerror("Variable declared already!");
				exit(0);
			  }                                     
                                     	
			Ginstall($3->NAME, $<np>0->TYPE, $5->value, false, NULL);
		}
                                     		
	|  ID '(' argList ')'	
		{
			gp = Glookup($1->NAME);
						
			if(gp != NULL)
			  {
				yyerror("Variable declared already!");
				exit(0);
			  }                                     
                                     	
              		Ginstall($1->NAME, $<np>0->TYPE, 0, true, $1->ArgList);
              	}
                                      	
	| G_Ids ',' ID '(' argList ')'	
		{
			gp = Glookup($3->NAME);
			if(gp != NULL)
			  {
				yyerror("Variable declared already!");
				exit(0);
      			  }                                     
			Ginstall($3->NAME, $<np>0->TYPE, 0, true, $3->ArgList);
              	}                 	                 	             		
                    ;    
                    

argList : 
	argList_notEmpty {$$=NULL;}
	|	{$$=NULL;}
	;


argList_notEmpty :  
	TYPE idList   { $$ = NULL; }
	| argList_notEmpty  ';' TYPE idList { $$=NULL; }
    ;
            
idList : 
	 idList ',' ID 
	 	{
			struct ArgStruct *A, *T;
			struct node *id;

			A = malloc(sizeof(struct ArgStruct) );
			A->TYPE = $<np>0->TYPE;
			A->NAME = strdup($3->NAME );
			A->ref_var = false;
			A->NEXT = NULL;	
	        if(($<np>-2)!=NULL) { 
			  	id = ($<np>-2);	
			  } 
 			else {
 			 	id = ($<np>-4); 
 			  }
 				 
			if( (id->ArgList) == NULL){
				(id->ArgList) = A;
  			  }
			else{
				T = (id->ArgList);

				while(T->NEXT != NULL ){
					if(!strcmp(T->NAME, A->NAME) )
					  {
					  	yyerror("Argument names doesn't match");
					  	exit(0);
					  }	
					T = T->NEXT;
 				}
 					
 				T->NEXT = A;	
 						
 			  }
					
			$$ = NULL;
		}


	| ID  
		{                                          
			struct ArgStruct *A, *T;
			struct node *id;
			A = malloc(sizeof(struct ArgStruct) );
			A->TYPE = $<np>0->TYPE;
			A -> NAME = strdup($1->NAME );
			A->ref_var = false;
			A->NEXT = NULL;	
			if(($<np>-2)!=NULL) 
			  {
				id = ($<np>-2);	 
			  } 
			else 
			  {
				id = ($<np>-4); 
			  }
 				
			if( id->ArgList == NULL)
			  {
				(id->ArgList) = A;
			  }
 			else
 			  {
				T = id->ArgList;
				while(T->NEXT != NULL ){
				  
					if(!strcmp(T->NAME, A->NAME) )
					  {
					  	yyerror("Argument names doesn't match");
					  	exit(0);
					  }	

 						T = T->NEXT;
				}
 					
 				T->NEXT = A;	
 						
 			  }
					
			$$ = NULL;
		}

	| idList ',' REF ID 
	 	{
			struct ArgStruct *A, *T;
			struct node *id;
			A = malloc(sizeof(struct ArgStruct) );
			A->TYPE = $<np>0->TYPE;
			A -> NAME = strdup($4->NAME );
			A->ref_var = true;
			A->NEXT = NULL;
		        if(($<np>-2)!=NULL) 
			  { 
			  	id = ($<np>-2);	
			  } 
 			else 
 			  {
 			 	id = ($<np>-4); 
 			  }
			if( (id->ArgList) == NULL)
			  {
				(id->ArgList) = A;
  			  }
			else
			  {
				T = (id->ArgList);

				while(T->NEXT != NULL ){
					if(!strcmp(T->NAME, A->NAME) )
					  {
					  	yyerror("Argument names doesn't match");
					  	exit(0);
					  }	
					T = T->NEXT;
 				}
 					
 				T->NEXT = A;	
 						
 			  }
					
			$$ = NULL;
		}


	| REF ID  
		{                                          
			struct ArgStruct *A, *T;
			struct node *id;
			A = malloc(sizeof(struct ArgStruct) );
			A->TYPE = $<np>0->TYPE;
			A -> NAME = strdup($2->NAME );
			
			A->ref_var = true;
			
			A->NEXT = NULL;
 					
			if(($<np>-2)!=NULL) 
			  {
				id = ($<np>-2);	 
			  } 
			else 
			  {
				id = ($<np>-4); 
			  }
 				
			if( id->ArgList == NULL)
			  {
				(id->ArgList) = A;
			  }
 			else
 			  {
				T = id->ArgList;
				while(T->NEXT != NULL ){
				  
					if(!strcmp(T->NAME, A->NAME) )
					  {
					  	yyerror("Argument names doesn't match");
					  	exit(0);
					  }	

 						T = T->NEXT;
				}
 					
 				T->NEXT = A;	
 						
 			  }
					
			$$ = NULL;
		}
	;
            
                                 		

Fdeflist : 
	Fdeflist Fdef { $$ = NULL; }		
	  |  { $$ = NULL; }	
	  ;

Fdef : 
	TYPE ID '(' argList ')' 
		{
			int argCount, i;
			argCount = 0;
			lHead = NULL;			  
			struct ArgStruct *AL;
			AL = $2->ArgList;  
			while(AL)
			{
				argCount++;
			  	AL = AL->NEXT;
			}
			$2->value = argCount;	  
			AL = $2->ArgList;	 
			i=0;
			while(AL){
				Linstall(AL->NAME, AL->TYPE, -2 -argCount + i  );
				i++;
				AL = AL->NEXT;		 
			}
		}
		
	'{' LDefblock 
		{ 
			int lVars = 1;
			struct ArgStruct *AL;
			AL = $8->ArgList;	 
			while(AL){	
				if(Llookup(AL->NAME ))
			  	  {
					yyerror("Variable declared already!");
				  	exit(0);
			  	  }
			  	else 
			  	  {
				  	Linstall(AL->NAME, AL->TYPE, lVars  );
					lVars++;
					AL = AL->NEXT;
				  }		 
			}
			$8->value = lVars -1;
		}

	slist retStmt '}'	
		{
			fprintf(fp, "%s:\n", $2->NAME);
			
			struct Gsymbol *G;
			
			G = Glookup($2->NAME);
			if(  G == NULL )
			  {
			 	yyerror("Undefined function"); 
				exit(0);			 
			  }

  			if(G->TYPE != $11->TYPE)
			  {
				yyerror("Return type not matching");
				exit(0);
			  } 
			  
			G->isDefined = true;
			  
			if(G->TYPE != $1->TYPE)
			  {
				yyerror("Function type not matching");
			  	exit(0);
			  }	
		  

			if( !( Is_FuncDefMatch ( $2->ArgList, (Glookup($2->NAME))->ARGLIST ) ))
			  {
				yyerror("Argument name not matching"); 
			   	exit(0);
			  }
			fprintf(fp, "PUSH BP\n"); 
			fprintf(fp, "MOV BP, SP\n");
			int i;
			for(  i=0; i<$8->value;++i)
			  {
				fprintf(fp, "PUSH R0\n");
			  }			
			Print_Tree_Code($10);			 
			if($11->TYPE == 1)
			  { 
				registers++;
				fprintf(fp, "MOV R%d, BP\n", (registers-1));
				registers++;
				fprintf(fp, "MOV R%d, 2 \n", (registers-1));  
				fprintf(fp, "SUB R%d, R%d\n", (registers-2),  (registers-1));
				registers--;
			     
			Print_Expr_Code($11);
			fprintf(fp, "MOV [R%d], R%d \n", (registers-2), (registers-1));
			registers--;
			registers--;
 			  }
			else 
			  {
				registers++;
				fprintf(fp, "MOV R%d, BP\n", (registers-1));
				registers++;
				fprintf(fp, "MOV R%d, 2 \n", (registers-1));  
				fprintf(fp, "SUB R%d, R%d\n", (registers-2),  (registers-1));
				registers--;     
			  Print_Expr_Code($11);
			  fprintf(fp, "MOV [R%d], R%d \n", (registers-2), registers-1);
			  registers--;
			  registers--;
			  }
			 for(  i=0; i<$8->value;++i)
			 {
			 fprintf(fp, "POP R0\n");            
			 }
			 fprintf(fp, "POP BP\n"); 
			 fprintf(fp, "RET\n");
			 lHead = NULL;
			}
LDefblock : DECL ldList ENDDECL   
		{   
			$$=$2;
		};

ldList :	ldList lDecl 
		{
			struct ArgStruct *A;
			A = $1->ArgList;
			if(A == NULL)
			{
				$1->ArgList = $2->ArgList;
			}
			else {
				while(A->NEXT)
				{A = A->NEXT;}
				A->NEXT = $2->ArgList;}						
				$$ = $1;
		}
	|        
        { 
			$$=malloc(sizeof(struct node));
			$$->ArgList = NULL;
		}
 		;
		
		
lDecl : 
	TYPE lVarList ';' 
	{ 
			$$=$2;
	 }


lVarList : ID 
	{
			struct ArgStruct *A;
			A = malloc(sizeof(struct ArgStruct));
			
			A->ref_var = false;
			
			A->NEXT = NULL;
			$1->ArgList = A;
		
			A->NAME = strdup( $1->NAME);
			A->TYPE = $<np>0 -> TYPE;
			$$=$1; 
	}	
	
 	|  lVarList ',' ID
		{
			struct ArgStruct *A, *B;
			A = malloc(sizeof(struct ArgStruct));
			A->NEXT = NULL;
			A->ref_var = false;		
			A->NAME = strdup( $3->NAME);
			A->TYPE = $<np>0 -> TYPE;
 
			B = $1->ArgList;
	       
		       while( B->NEXT)
		       {
			       B = B->NEXT;
		       }
	   
		      B->NEXT = A;
	      
		      $$=$1;    
		}

retStmt : RETURN expr ';' 
		{
			$$ = $2;
		}

slist:   slist stmt
		{                                          
			if($1==NULL)
					{
						p = malloc(sizeof( struct node ));
				        p->left = $2;   
						p->middle = NULL; 
						p->right = NULL;
						p->TYPE = 3;
						p->NODETYPE = 0;
						$$=p;
					}
			else
					{
						p = malloc(sizeof( struct node ));
                        p->left = $2;  
						p-> middle = NULL;  
						p->right = NULL;
						p->TYPE = 3;
						p->NODETYPE = 0;
						Construct_Tree( $1, p);
						$$=$1;
					}
                                     }	 
                    
           
        |                            {  
					$$=NULL;
                                     }  
        ;


stmt:   ID '=' expr ';'  
			{					
					lp = Llookup($1->NAME);
					if(lp){
					$1->Lentry = lp;
                    $1->TYPE = lp->TYPE;
					}
					else {
					gp = Glookup($1->NAME);	
                   	if(gp == NULL)
					{
					yyerror("Undeclared Variable");
					exit(0);
					}                                     
                    $1->Gentry = gp;
                 	$1->TYPE = gp->TYPE;
					}					
					if ( !( ( ($1->TYPE == 1) && ($3->TYPE == 1) )  || ( ($1->TYPE == 2) && ($3->TYPE == 2) ) ) )
					{ 
						yyerror("Assignment type not Matching"); 
						exit(0);  
					} 
					
					$2->left = $1;
					$2->right = $3;
					$$ = $2;
          }

    |  ID '[' expr ']' '=' expr ';'        
    		{  			
					gp = Glookup($1->NAME);	
  					if(gp == NULL)
							{
								yyerror("Undeclared  variable");
								exit(0);
							}                                     
			$1->Gentry = gp;
			$1->TYPE = gp->TYPE;					
			if (!((($1->TYPE == 1) && ($3->TYPE == 1) && ($6->TYPE == 1))  || (($1->TYPE == 2) && ($3->TYPE == 1)&& ($6->TYPE == 2))))
					{ 
						yyerror("Assignment type not Matching"); 
						exit(0);  
					} 				
					$5->left = $1;
					$5->middle = $3;
					$5->right = $6;
					$$ = $5;
			}
    | READ '(' ID ')' ';'    
    		{ 
					$1->left = $3;
					$1->right = NULL;
					lp = Llookup($3->NAME);
					if(lp){					
					$3->Lentry = lp;
                   	$3->TYPE = lp->TYPE;			
					}
					else {
					gp = Glookup($3->NAME);	
                   	if(gp == NULL)
					{
						yyerror("Undeclared variable");
						exit(0);
					}
					$3->Gentry = gp;
                	$3->TYPE = gp->TYPE;
					}	
					if ( $3->TYPE != 1)
					{ 
						yyerror("Arguments must be of type integer"); 
						exit(0);  
					}
					$$=$1;
            }         
	| READ '(' ID '[' expr ']' ')' ';'    
			{ 
					if ( ($5->TYPE) != 1 ) 
					{ 
						yyerror("Arguments must be of type integer"); 
						exit(0);  
					}
					$1->left = $3;
					$1->middle=$5;
					$1->right = NULL;
					gp = Glookup($3->NAME);	                   
                   	if(gp == NULL)
							{
								yyerror("Undeclared variable");
								exit(0);
							}                                     
                                     	$3->Gentry = gp;
					$3->TYPE = gp->TYPE;

					
					if ( $3->TYPE != 1)
					{ 
						yyerror("Arguments must be of type integer"); 
						exit(0);  
					} 
					
        
					$$=$1;
                }
      |  WRITE '(' expr ')' ';'   
      			{      
					if ($3->TYPE != 1 )
						{ 
							yyerror("Arguments must be of type integer");
							 exit(0);  
						}
					$1->left = $3;
					$1->right = NULL;
					$$=$1;				
				} 

     | IF '(' expr ')' THEN slist ELSE slist ENDIF ';' {   
					if ($3->TYPE != 2 ) 
					{
						yyerror("Arguments msut be of type boolean"); 
						exit(0); 
					}
                                        
					$1->left = $3;
					$1->middle=$6;
					$1->right = $8;
					$$=$1;

					}	

     | WHILE '(' expr ')' DO slist ENDWHILE ';' {  
					 if ($3->TYPE != 2 ) 
					 {
						yyerror("Arguments msut be of type boolean");
						exit(0);  
					 }

                     $1->left = $3;
                     $1->middle=NULL;
					 $1->right = $6;
          			 $$=$1;
                         		} 
     ;


exprList : 
	exprListNotEmpty 
		{ $$ = $1; } 	
	|     
		{ $$=NULL; } 
	;
	
exprListNotEmpty : 
	exprListNotEmpty  ',' expr 
		{
			struct node *n, *A;
			n = malloc(sizeof(struct node));
			n->TYPE = $3->TYPE;
			n->right = $3; 
			n->left = NULL; 
			$1->value += 1;
			A = $1;
			while(A->left)
			  {
				A = A->left;
			  }
			A->left = n;	
			$$=$1;
		}
	| expr   
		{ 
			struct node *n;
			n = malloc(sizeof(struct node));
			n->TYPE = $1->TYPE;
			n->right = $1; 
			n->left = NULL;
			n->value = 1;		
			$$ = n;
		}		
	;

expr:
      NUM {	$$ = $1;  }
        
	| TRUE {	$$ = $1;  }
        
	| FALSE	{	$$ = $1; }	   

	| ID  {
        			lp = Llookup($1->NAME);		
        			if(lp){
        			$1->Lentry = lp;
					$1->TYPE = lp->TYPE;
					}				
       				else {
       				gp = Glookup($1->NAME);	
               		if(gp == NULL)
					{
						yyerror("Undeclared variable");
						exit(0);
					}                                     
                 	$1->Gentry = gp;
                    $1->TYPE = gp->TYPE;
					}			
					$$ = $1; 
		}
	| ID '[' expr ']'   { 
        				gp = Glookup($1->NAME);	
						if(gp == NULL)
							{
								yyerror("Undeclared variable");
								exit(0);
							}                                     
                                     	$1->TYPE = gp->TYPE;
                                     	$1->Gentry = gp;             	
                                     	$1->middle = $3;
										$$ = $1; 
				     }
	| expr '+' expr              { 
					if( ($1->TYPE != 1) || ($3->TYPE != 1) )
                                        {
						yyerror("Arguments must be of type integer");
						 exit(0);  
					}
					 
					$2 -> left = $1;
		            $2->right = $3;
					$$=$2; 
				     }

    | expr '*' expr              {  
					if( ($1->TYPE != 1) || ($3->TYPE != 1) )
                                        {
						yyerror("Arguments must be of type integer");
						exit(0);
					}
					 
					$2 -> left = $1; 
					$2->right = $3;  
         			$$=$2; 
				     }
	| expr '-' expr              {  
					if( ($1->TYPE != 1) || ($3->TYPE != 1) )
                                        {
						yyerror("Arguments must be of type integer");
						exit(0);
					}
					 
					$2 -> left = $1; 
					$2->right = $3;  
         			$$=$2; 
				     }

    | expr '/' expr              {  
					if( ($1->TYPE != 1) || ($3->TYPE != 1) )
                                        {
						yyerror("Arguments must be of type integer");
						exit(0);
					} 
					$2 -> left = $1; 
					$2->right = $3;  
         			$$=$2; 
				     }
	| expr '|' expr              {  
					if( ($1->TYPE != 2) || ($3->TYPE != 2) )
                                        {
						yyerror("Arguments must be of type boolean");
						exit(0);
					}
					 
					$2 -> left = $1; 
					$2->right = $3;  
         			$$=$2; 
				     }			     
				     
	| expr '&' expr              {  
					if( ($1->TYPE != 2) || ($3->TYPE != 2) )
                                        {
						yyerror("Arguments must be of type boolean");
						exit(0);
					}
					 
					$2 -> left = $1; 
					$2->right = $3;  
         			$$=$2; 
				     }			  
    |'(' expr ')' { 
					$$ = $2; 
				    }
    | expr '<' expr   	      { 
					if( ($1->TYPE != 1) || ($3->TYPE != 1) )
                   {
						yyerror("Arguments must be of type integer");
						exit(0); 
					}					 
					$2 -> left = $1;
         			$2->right = $3; 
					$$=$2; 
        			      }

    | expr '>' expr                 { 
					if( ($1->TYPE != 1) || ($3->TYPE != 1) )
                                        { 
						yyerror("Arguments must be of type integer");
						exit(0);  
					}
					         
           			$2 -> left = $1; 
					$2->right = $3;  	
					$$=$2;  
				    }

    | expr EQL expr               {   
					if( ($1->TYPE != 1) || ($3->TYPE != 1) )
                                        { 
						yyerror("Arguments must be of type integer");
						exit(0);  
					}

					 $2 -> left = $1;
					 $2->right = $3;   
       				 $$=$2;  
				    }

    | expr LE expr               {   
					if( ($1->TYPE != 1) || ($3->TYPE != 1) )
                                        { 
						yyerror("Arguments must be of type integer");
						exit(0);  
					}

					 $2 -> left = $1;
					 $2->right = $3;   
       				 $$=$2;  
				    }

    | expr GE expr               {   
					if( ($1->TYPE != 1) || ($3->TYPE != 1) )
                                        { 
						yyerror("Arguments must be of type integer");
						exit(0);  
					}

					 $2 -> left = $1;
					 $2->right = $3;   
       				 $$=$2;  
				    }
				    
				    
				    
	| '!' expr 		{   
					if($2->TYPE != 2) 
                                        { 
						yyerror("Arguments must be of type boolean");
						exit(0);  
					}
					 $1 -> left = $2;					    
       				 $$=$1;  
				    }
				    
	| ID   
		{  
			$1->Gentry = Glookup($1->NAME);
			if($1->Gentry==NULL) 
			  {
				yyerror("Function definition not found"); 
				exit(0); 
			  }			   
 			if(!(($1->Gentry)->isFUNCTION))    
 			  {
 				yyerror("Variable cannot be a function"); 
 				exit(0); 
 			  }                        
                        $1->TYPE = ($1->Gentry)->TYPE;       
		}   
		
	'(' exprList ')' 
		{                             
			if( ! (Is_FuncCallMatch((Glookup($1->NAME))->ARGLIST, $4 ) ) )        
     			  {
				yyerror("Function call not matching");
     			   	exit(0);
     			  }     			   
			$1->left = $4;    
			$1->value =  ($4)? $4->value : 0;     
			$1->NODETYPE = 15;     		
			$$ = $1; 
		}
	| ITER '(' ID
					{  
			$3->Gentry = Glookup($3->NAME);
			if($3->Gentry==NULL) 
			  {
				yyerror("Function definitionin ITER not found"); 
				exit(0); 
			  }			   
 			if(!(($3->Gentry)->isFUNCTION))    
 			  {
 				yyerror("ITER needs a function a argument"); 
 				exit(0); 
 			  }                        
                        $3->TYPE = ($3->Gentry)->TYPE;    
                        if($3->TYPE != 1){
                        yyerror("Return type of argument in ITER must be int");
     			   	exit(0);
                        }   
		} 
	 '(' exprList ')' ')'
	 				{                             
			if( ! (Is_FuncCallMatch((Glookup($3->NAME))->ARGLIST, $6 ) ) )        
     			  {
				yyerror("Function call in ITER not matching");
     			   	exit(0);
     			  }     
     		if ( $6->TYPE != 1 ){
     		yyerror("First argument in Function call in ITER must be INT");
     			   	exit(0);
     		}
			$3->left = $6;    
			$3->value =  ($6)? $6->value : 0;     
			$3->NODETYPE = 16;  
			$1->right = $3;   		
			$$ = $1; 
		}
     	;		  

%%
