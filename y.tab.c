/* A Bison parser, made by GNU Bison 2.7.12-4996.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.7.12-4996"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
/* Line 371 of yacc.c  */
#line 1 "tree.y"

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




/* Line 371 of yacc.c  */
#line 702 "y.tab.c"

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NUM = 258,
     ID = 259,
     READ = 260,
     WRITE = 261,
     EQL = 262,
     LE = 263,
     GE = 264,
     IF = 265,
     THEN = 266,
     ELSE = 267,
     ENDIF = 268,
     WHILE = 269,
     DO = 270,
     ENDWHILE = 271,
     DECL = 272,
     ENDDECL = 273,
     BEG = 274,
     END = 275,
     INT = 276,
     BOOL = 277,
     TRUE = 278,
     FALSE = 279,
     RETURN = 280,
     MAIN = 281,
     REF = 282,
     ITER = 283
   };
#endif
/* Tokens.  */
#define NUM 258
#define ID 259
#define READ 260
#define WRITE 261
#define EQL 262
#define LE 263
#define GE 264
#define IF 265
#define THEN 266
#define ELSE 267
#define ENDIF 268
#define WHILE 269
#define DO 270
#define ENDWHILE 271
#define DECL 272
#define ENDDECL 273
#define BEG 274
#define END 275
#define INT 276
#define BOOL 277
#define TRUE 278
#define FALSE 279
#define RETURN 280
#define MAIN 281
#define REF 282
#define ITER 283



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 387 of yacc.c  */
#line 635 "tree.y"

	struct node *np;
       

/* Line 387 of yacc.c  */
#line 806 "y.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 834 "y.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef __attribute__
/* This feature is available in gcc versions 2.5 and later.  */
# if (! defined __GNUC__ || __GNUC__ < 2 \
      || (__GNUC__ == 2 && __GNUC_MINOR__ < 5))
#  define __attribute__(Spec) /* empty */
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif


/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(N) (N)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   343

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  48
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  27
/* YYNRULES -- Number of rules.  */
#define YYNRULES  70
/* YYNRULES -- Number of states.  */
#define YYNSTATES  173

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   283

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       8,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    16,     2,     2,     2,     2,    13,     2,
       6,     7,     5,     4,    18,    10,     2,    11,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    17,
      19,     9,    20,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    46,     2,    47,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    14,    12,    15,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,    18,    22,    25,    26,    30,    32,
      34,    36,    40,    45,    52,    57,    64,    66,    67,    70,
      75,    79,    81,    86,    89,    92,    93,    94,    95,   108,
     112,   115,   116,   120,   122,   126,   130,   133,   134,   139,
     147,   153,   162,   168,   179,   188,   190,   191,   195,   197,
     199,   201,   203,   205,   210,   214,   218,   222,   226,   230,
     234,   238,   242,   246,   250,   254,   258,   261,   262,   268,
     269
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      49,     0,    -1,    -1,    51,    59,    38,    43,     6,     7,
      14,    63,    50,    36,    68,    37,    15,    -1,    34,    52,
      35,    -1,    52,    53,    -1,    -1,    54,    55,    17,    -1,
      38,    -1,    39,    -1,    21,    -1,    55,    18,    21,    -1,
      21,    46,     3,    47,    -1,    55,    18,    21,    46,     3,
      47,    -1,    21,     6,    56,     7,    -1,    55,    18,    21,
       6,    56,     7,    -1,    57,    -1,    -1,    54,    58,    -1,
      57,    17,    54,    58,    -1,    58,    18,    21,    -1,    21,
      -1,    58,    18,    44,    21,    -1,    44,    21,    -1,    59,
      60,    -1,    -1,    -1,    -1,    54,    21,     6,    56,     7,
      61,    14,    63,    62,    68,    67,    15,    -1,    34,    64,
      35,    -1,    64,    65,    -1,    -1,    54,    66,    17,    -1,
      21,    -1,    66,    18,    21,    -1,    42,    72,    17,    -1,
      68,    69,    -1,    -1,    21,     9,    72,    17,    -1,    21,
      46,    72,    47,     9,    72,    17,    -1,    22,     6,    21,
       7,    17,    -1,    22,     6,    21,    46,    72,    47,     7,
      17,    -1,    23,     6,    72,     7,    17,    -1,    27,     6,
      72,     7,    28,    68,    29,    68,    30,    17,    -1,    31,
       6,    72,     7,    32,    68,    33,    17,    -1,    71,    -1,
      -1,    71,    18,    72,    -1,    72,    -1,     3,    -1,    40,
      -1,    41,    -1,    21,    -1,    21,    46,    72,    47,    -1,
      72,     4,    72,    -1,    72,     5,    72,    -1,    72,    10,
      72,    -1,    72,    11,    72,    -1,    72,    12,    72,    -1,
      72,    13,    72,    -1,     6,    72,     7,    -1,    72,    19,
      72,    -1,    72,    20,    72,    -1,    72,    24,    72,    -1,
      72,    25,    72,    -1,    72,    26,    72,    -1,    16,    72,
      -1,    -1,    21,    73,     6,    70,     7,    -1,    -1,    45,
       6,    21,    74,     6,    70,     7,     7,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   652,   652,   651,   684,   691,   692,   696,   699,   700,
     705,   716,   727,   740,   753,   766,   780,   781,   786,   787,
     791,   831,   874,   916,   966,   967,   972,   994,   971,  1090,
    1095,  1110,  1118,  1124,  1139,  1160,  1165,  1191,  1197,  1225,
    1245,  1271,  1300,  1312,  1326,  1342,  1345,  1349,  1365,  1378,
    1380,  1382,  1384,  1402,  1414,  1426,  1437,  1449,  1459,  1471,
    1482,  1485,  1496,  1508,  1520,  1532,  1546,  1557,  1556,  1585,
    1584
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NUM", "'+'", "'*'", "'('", "')'",
  "'\\n'", "'='", "'-'", "'/'", "'|'", "'&'", "'{'", "'}'", "'!'", "';'",
  "','", "'<'", "'>'", "ID", "READ", "WRITE", "EQL", "LE", "GE", "IF",
  "THEN", "ELSE", "ENDIF", "WHILE", "DO", "ENDWHILE", "DECL", "ENDDECL",
  "BEG", "END", "INT", "BOOL", "TRUE", "FALSE", "RETURN", "MAIN", "REF",
  "ITER", "'['", "']'", "$accept", "program", "$@1", "GDefBlock", "dList",
  "decl", "TYPE", "G_Ids", "argList", "argList_notEmpty", "idList",
  "Fdeflist", "Fdef", "$@2", "$@3", "LDefblock", "ldList", "lDecl",
  "lVarList", "retStmt", "slist", "stmt", "exprList", "exprListNotEmpty",
  "expr", "$@4", "$@5", YY_NULL
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,    43,    42,    40,    41,    10,    61,
      45,    47,   124,    38,   123,   125,    33,    59,    44,    60,
      62,   259,   260,   261,   262,   263,   264,   265,   266,   267,
     268,   269,   270,   271,   272,   273,   274,   275,   276,   277,
     278,   279,   280,   281,   282,   283,    91,    93
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    48,    50,    49,    51,    52,    52,    53,    54,    54,
      55,    55,    55,    55,    55,    55,    56,    56,    57,    57,
      58,    58,    58,    58,    59,    59,    61,    62,    60,    63,
      64,    64,    65,    66,    66,    67,    68,    68,    69,    69,
      69,    69,    69,    69,    69,    70,    70,    71,    71,    72,
      72,    72,    72,    72,    72,    72,    72,    72,    72,    72,
      72,    72,    72,    72,    72,    72,    72,    73,    72,    74,
      72
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,    13,     3,     2,     0,     3,     1,     1,
       1,     3,     4,     6,     4,     6,     1,     0,     2,     4,
       3,     1,     4,     2,     2,     0,     0,     0,    12,     3,
       2,     0,     3,     1,     3,     3,     2,     0,     4,     7,
       5,     8,     5,    10,     8,     1,     0,     3,     1,     1,
       1,     1,     1,     4,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     0,     5,     0,
       8
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     6,     0,    25,     0,     1,     0,     4,     8,     9,
       5,     0,     8,     0,    24,    10,     0,     0,     0,    17,
       0,     7,     0,     0,    17,     0,     0,    16,     0,    11,
       0,     0,    21,     0,    18,    14,     0,    12,    17,     0,
       0,    26,    23,     0,     0,     0,     0,    31,     2,     0,
      20,     0,    19,    15,    13,     0,     0,     0,    22,    29,
       0,    30,    37,    27,    33,     0,     0,    37,    32,     0,
       0,     0,     0,     0,     0,     0,    36,     0,    34,     0,
       0,     0,     0,     0,     0,     3,     0,     0,    49,     0,
       0,    52,    50,    51,     0,     0,     0,     0,     0,     0,
       0,     0,    28,     0,    66,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    38,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    35,    60,     0,    46,
      69,    54,    55,    56,    57,    58,    59,    61,    62,    63,
      64,    65,     0,    40,     0,    42,    37,    37,    53,     0,
      45,    48,     0,     0,     0,     0,     0,    68,     0,    46,
      39,     0,    37,     0,    47,     0,    41,     0,    44,     0,
       0,    70,    43
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,    56,     3,     4,    10,    25,    16,    26,    27,
      34,     6,    14,    49,    67,    48,    55,    61,    65,    87,
      66,    76,   149,   150,   151,   106,   152
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -78
static const yytype_int16 yypact[] =
{
     -18,   -78,    18,   -78,   -12,   -78,    -9,   -78,   -78,   -78,
     -78,    16,     3,    31,   -78,    -2,    42,    61,    70,    23,
      79,   -78,    67,    82,    23,   -20,    87,    86,    49,     4,
      90,    99,   -78,    93,   110,   -78,    23,   -78,    23,   112,
      77,   -78,   -78,    -1,   -20,   123,    89,   -78,   -78,   117,
     -78,   116,   110,   -78,   -78,    36,   102,    77,   -78,   -78,
     119,   -78,   -78,   -78,   -78,    55,   285,   -78,   -78,   124,
      12,   137,   147,   148,   153,   136,   -78,   282,   -78,    84,
      84,   139,    84,    84,    84,   -78,    84,   146,   -78,    84,
      84,     5,   -78,   -78,   156,   122,    44,     7,   145,   168,
     191,   208,   -78,   231,   -78,    84,   157,   155,    84,    84,
      84,    84,    84,    84,   -78,    84,    84,    84,    84,    84,
     158,   149,    84,   151,   154,   142,   -78,   -78,    73,    84,
     -78,    14,   -78,    14,   -78,   276,   276,   108,   108,   108,
     108,   108,    84,   -78,    97,   -78,   -78,   -78,   -78,   170,
     165,   265,   178,   248,   179,   309,   296,   -78,    84,    84,
     -78,   172,   -78,   173,   265,   184,   -78,   312,   -78,   190,
     182,   -78,   -78
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -78,   -78,   -78,   -78,   -78,   -78,    11,   -78,   -16,   -78,
     141,   -78,   -78,   -78,   -78,   143,   -78,   -78,   -78,   -78,
     -67,   -78,    46,   -78,   -77,   -78,   -78
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -68
static const yytype_int16 yytable[] =
{
      77,    32,    95,    96,    19,    98,    99,   100,    31,   101,
      38,   -67,   103,   104,   121,    11,     1,    13,     5,   109,
      50,    79,    45,     7,    33,   111,     8,     9,   128,    12,
       9,   131,   132,   133,   134,   135,   136,    15,   137,   138,
     139,   140,   141,    51,    20,   144,    17,    44,   108,   109,
      39,   105,    18,   122,   110,   111,   112,   113,    80,    21,
      22,     8,     9,   115,   116,   153,    60,    23,   117,   118,
     119,    59,    68,    69,     8,     9,    24,   108,   109,   155,
     156,   164,    28,   110,   111,   112,   113,    88,    29,    30,
      89,   120,   115,   116,    35,   167,    37,   117,   118,   119,
      90,   108,   109,    36,    40,    91,    41,   110,   111,   112,
     113,    47,   108,   109,    42,    46,   115,   116,   110,   111,
     148,   117,   118,   119,    92,    93,   108,   109,    43,    94,
      53,    57,   110,   111,   112,   113,    54,    58,    62,   114,
      64,   115,   116,    81,   154,    78,   117,   118,   119,   108,
     109,    85,   123,    82,    83,   110,   111,   112,   113,    84,
      97,   102,   107,   129,   115,   116,   143,   142,   145,   117,
     118,   119,   108,   109,   147,   124,   130,   157,   110,   111,
     112,   113,   146,   158,   159,    52,   161,   115,   116,   166,
     168,   169,   117,   118,   119,   108,   109,   171,   125,   172,
      63,   110,   111,   112,   113,   165,     0,     0,     0,     0,
     115,   116,   108,   109,     0,   117,   118,   119,   110,   111,
     112,   113,     0,     0,     0,   126,     0,   115,   116,     0,
       0,     0,   117,   118,   119,   108,   109,     0,   127,     0,
       0,   110,   111,   112,   113,     0,     0,     0,     0,     0,
     115,   116,   108,   109,     0,   117,   118,   119,   110,   111,
     112,   113,     0,     0,     0,   160,     0,   115,   116,   108,
     109,     0,   117,   118,   119,   110,   111,   112,   113,     0,
     108,   109,     0,     0,   115,   116,   110,   111,     0,   117,
     118,   119,     0,     0,     0,   115,   116,     0,     0,     0,
     117,   118,   119,    70,    71,    72,    70,    71,    72,    73,
       0,     0,    73,    74,     0,     0,    74,    70,    71,    72,
       0,     0,    75,    73,    86,     0,     0,    74,     0,   163,
      70,    71,    72,    70,    71,    72,    73,     0,   162,    73,
      74,     0,   170,    74
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-78)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      67,    21,    79,    80,     6,    82,    83,    84,    24,    86,
       6,     6,    89,    90,     7,     4,    34,     6,     0,     5,
      21,     9,    38,    35,    44,    11,    38,    39,   105,    38,
      39,   108,   109,   110,   111,   112,   113,    21,   115,   116,
     117,   118,   119,    44,    46,   122,    43,    36,     4,     5,
      46,    46,    21,    46,    10,    11,    12,    13,    46,    17,
      18,    38,    39,    19,    20,   142,    55,     6,    24,    25,
      26,    35,    17,    18,    38,    39,     6,     4,     5,   146,
     147,   158,     3,    10,    11,    12,    13,     3,    21,     7,
       6,    47,    19,    20,     7,   162,    47,    24,    25,    26,
      16,     4,     5,    17,    14,    21,     7,    10,    11,    12,
      13,    34,     4,     5,    21,     3,    19,    20,    10,    11,
      47,    24,    25,    26,    40,    41,     4,     5,    18,    45,
       7,    14,    10,    11,    12,    13,    47,    21,    36,    17,
      21,    19,    20,     6,    47,    21,    24,    25,    26,     4,
       5,    15,     7,     6,     6,    10,    11,    12,    13,     6,
      21,    15,     6,     6,    19,    20,    17,     9,    17,    24,
      25,    26,     4,     5,    32,     7,    21,     7,    10,    11,
      12,    13,    28,    18,     6,    44,     7,    19,    20,    17,
      17,     7,    24,    25,    26,     4,     5,     7,     7,    17,
      57,    10,    11,    12,    13,   159,    -1,    -1,    -1,    -1,
      19,    20,     4,     5,    -1,    24,    25,    26,    10,    11,
      12,    13,    -1,    -1,    -1,    17,    -1,    19,    20,    -1,
      -1,    -1,    24,    25,    26,     4,     5,    -1,     7,    -1,
      -1,    10,    11,    12,    13,    -1,    -1,    -1,    -1,    -1,
      19,    20,     4,     5,    -1,    24,    25,    26,    10,    11,
      12,    13,    -1,    -1,    -1,    17,    -1,    19,    20,     4,
       5,    -1,    24,    25,    26,    10,    11,    12,    13,    -1,
       4,     5,    -1,    -1,    19,    20,    10,    11,    -1,    24,
      25,    26,    -1,    -1,    -1,    19,    20,    -1,    -1,    -1,
      24,    25,    26,    21,    22,    23,    21,    22,    23,    27,
      -1,    -1,    27,    31,    -1,    -1,    31,    21,    22,    23,
      -1,    -1,    37,    27,    42,    -1,    -1,    31,    -1,    33,
      21,    22,    23,    21,    22,    23,    27,    -1,    29,    27,
      31,    -1,    30,    31
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    34,    49,    51,    52,     0,    59,    35,    38,    39,
      53,    54,    38,    54,    60,    21,    55,    43,    21,     6,
      46,    17,    18,     6,     6,    54,    56,    57,     3,    21,
       7,    56,    21,    44,    58,     7,    17,    47,     6,    46,
      14,     7,    21,    18,    54,    56,     3,    34,    63,    61,
      21,    44,    58,     7,    47,    64,    50,    14,    21,    35,
      54,    65,    36,    63,    21,    66,    68,    62,    17,    18,
      21,    22,    23,    27,    31,    37,    69,    68,    21,     9,
      46,     6,     6,     6,     6,    15,    42,    67,     3,     6,
      16,    21,    40,    41,    45,    72,    72,    21,    72,    72,
      72,    72,    15,    72,    72,    46,    73,     6,     4,     5,
      10,    11,    12,    13,    17,    19,    20,    24,    25,    26,
      47,     7,    46,     7,     7,     7,    17,     7,    72,     6,
      21,    72,    72,    72,    72,    72,    72,    72,    72,    72,
      72,    72,     9,    17,    72,    17,    28,    32,    47,    70,
      71,    72,    74,    72,    47,    68,    68,     7,    18,     6,
      17,     7,    29,    33,    72,    70,    17,    68,    17,     7,
      30,     7,    17
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))

/* Error token number */
#define YYTERROR	1
#define YYERRCODE	256


/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */
#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YYUSE (yytype);
}




/* The lookahead symbol.  */
int yychar;


#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval YY_INITIAL_VALUE(yyval_default);

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
/* Line 1787 of yacc.c  */
#line 652 "tree.y"
    {   
			int lVars = 1;
			struct ArgStruct *AL;
			AL = (yyvsp[(8) - (8)].np)->ArgList;	 
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
    break;

  case 3:
/* Line 1787 of yacc.c  */
#line 674 "tree.y"
    {  
			isFunctionsDefined();
			Print_Tree_Code((yyvsp[(11) - (13)].np));
			fprintf(fp, "HALT\n");
			return 0;
        }
    break;

  case 4:
/* Line 1787 of yacc.c  */
#line 685 "tree.y"
    {
			fp=fopen("output.txt", "w");						
			(yyval.np)=NULL;
		}
    break;

  case 5:
/* Line 1787 of yacc.c  */
#line 691 "tree.y"
    {}
    break;

  case 6:
/* Line 1787 of yacc.c  */
#line 692 "tree.y"
    {}
    break;

  case 7:
/* Line 1787 of yacc.c  */
#line 696 "tree.y"
    {}
    break;

  case 10:
/* Line 1787 of yacc.c  */
#line 706 "tree.y"
    {
			gp = Glookup((yyvsp[(1) - (1)].np)->NAME);
			if(gp != NULL)
	      		  {
				yyerror("Variable declared already!");
				exit(0);
			  }                                       	
			Ginstall((yyvsp[(1) - (1)].np)->NAME, (yyvsp[(0) - (1)].np)->TYPE, 1, false, NULL);
             	}
    break;

  case 11:
/* Line 1787 of yacc.c  */
#line 717 "tree.y"
    {
			gp = Glookup((yyvsp[(3) - (3)].np)->NAME);
			if(gp != NULL)
			  {
				yyerror("Variable declared already!");
				exit(0);
                          }                                       	
			Ginstall((yyvsp[(3) - (3)].np)->NAME, (yyvsp[(0) - (3)].np)->TYPE, 1, false, NULL);
      		}
    break;

  case 12:
/* Line 1787 of yacc.c  */
#line 728 "tree.y"
    {
			gp = Glookup((yyvsp[(1) - (4)].np)->NAME);	
                   
			if(gp != NULL)
			  {
				yyerror("Variable declared already!");
				exit(0);
			  }                                     
                                     	
      			Ginstall((yyvsp[(1) - (4)].np)->NAME, (yyvsp[(0) - (4)].np)->TYPE, (yyvsp[(3) - (4)].np)->value, false, NULL);
		}
    break;

  case 13:
/* Line 1787 of yacc.c  */
#line 741 "tree.y"
    {
			gp = Glookup((yyvsp[(3) - (6)].np)->NAME);
						
			if(gp != NULL)
			  {
				yyerror("Variable declared already!");
				exit(0);
			  }                                     
                                     	
			Ginstall((yyvsp[(3) - (6)].np)->NAME, (yyvsp[(0) - (6)].np)->TYPE, (yyvsp[(5) - (6)].np)->value, false, NULL);
		}
    break;

  case 14:
/* Line 1787 of yacc.c  */
#line 754 "tree.y"
    {
			gp = Glookup((yyvsp[(1) - (4)].np)->NAME);
						
			if(gp != NULL)
			  {
				yyerror("Variable declared already!");
				exit(0);
			  }                                     
                                     	
              		Ginstall((yyvsp[(1) - (4)].np)->NAME, (yyvsp[(0) - (4)].np)->TYPE, 0, true, (yyvsp[(1) - (4)].np)->ArgList);
              	}
    break;

  case 15:
/* Line 1787 of yacc.c  */
#line 767 "tree.y"
    {
			gp = Glookup((yyvsp[(3) - (6)].np)->NAME);
			if(gp != NULL)
			  {
				yyerror("Variable declared already!");
				exit(0);
      			  }                                     
			Ginstall((yyvsp[(3) - (6)].np)->NAME, (yyvsp[(0) - (6)].np)->TYPE, 0, true, (yyvsp[(3) - (6)].np)->ArgList);
              	}
    break;

  case 16:
/* Line 1787 of yacc.c  */
#line 780 "tree.y"
    {(yyval.np)=NULL;}
    break;

  case 17:
/* Line 1787 of yacc.c  */
#line 781 "tree.y"
    {(yyval.np)=NULL;}
    break;

  case 18:
/* Line 1787 of yacc.c  */
#line 786 "tree.y"
    { (yyval.np) = NULL; }
    break;

  case 19:
/* Line 1787 of yacc.c  */
#line 787 "tree.y"
    { (yyval.np)=NULL; }
    break;

  case 20:
/* Line 1787 of yacc.c  */
#line 792 "tree.y"
    {
			struct ArgStruct *A, *T;
			struct node *id;

			A = malloc(sizeof(struct ArgStruct) );
			A->TYPE = (yyvsp[(0) - (3)].np)->TYPE;
			A->NAME = strdup((yyvsp[(3) - (3)].np)->NAME );
			A->ref_var = false;
			A->NEXT = NULL;	
	        if(((yyvsp[(-2) - (3)].np))!=NULL) { 
			  	id = ((yyvsp[(-2) - (3)].np));	
			  } 
 			else {
 			 	id = ((yyvsp[(-4) - (3)].np)); 
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
					
			(yyval.np) = NULL;
		}
    break;

  case 21:
/* Line 1787 of yacc.c  */
#line 832 "tree.y"
    {                                          
			struct ArgStruct *A, *T;
			struct node *id;
			A = malloc(sizeof(struct ArgStruct) );
			A->TYPE = (yyvsp[(0) - (1)].np)->TYPE;
			A -> NAME = strdup((yyvsp[(1) - (1)].np)->NAME );
			A->ref_var = false;
			A->NEXT = NULL;	
			if(((yyvsp[(-2) - (1)].np))!=NULL) 
			  {
				id = ((yyvsp[(-2) - (1)].np));	 
			  } 
			else 
			  {
				id = ((yyvsp[(-4) - (1)].np)); 
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
					
			(yyval.np) = NULL;
		}
    break;

  case 22:
/* Line 1787 of yacc.c  */
#line 875 "tree.y"
    {
			struct ArgStruct *A, *T;
			struct node *id;
			A = malloc(sizeof(struct ArgStruct) );
			A->TYPE = (yyvsp[(0) - (4)].np)->TYPE;
			A -> NAME = strdup((yyvsp[(4) - (4)].np)->NAME );
			A->ref_var = true;
			A->NEXT = NULL;
		        if(((yyvsp[(-2) - (4)].np))!=NULL) 
			  { 
			  	id = ((yyvsp[(-2) - (4)].np));	
			  } 
 			else 
 			  {
 			 	id = ((yyvsp[(-4) - (4)].np)); 
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
					
			(yyval.np) = NULL;
		}
    break;

  case 23:
/* Line 1787 of yacc.c  */
#line 917 "tree.y"
    {                                          
			struct ArgStruct *A, *T;
			struct node *id;
			A = malloc(sizeof(struct ArgStruct) );
			A->TYPE = (yyvsp[(0) - (2)].np)->TYPE;
			A -> NAME = strdup((yyvsp[(2) - (2)].np)->NAME );
			
			A->ref_var = true;
			
			A->NEXT = NULL;
 					
			if(((yyvsp[(-2) - (2)].np))!=NULL) 
			  {
				id = ((yyvsp[(-2) - (2)].np));	 
			  } 
			else 
			  {
				id = ((yyvsp[(-4) - (2)].np)); 
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
					
			(yyval.np) = NULL;
		}
    break;

  case 24:
/* Line 1787 of yacc.c  */
#line 966 "tree.y"
    { (yyval.np) = NULL; }
    break;

  case 25:
/* Line 1787 of yacc.c  */
#line 967 "tree.y"
    { (yyval.np) = NULL; }
    break;

  case 26:
/* Line 1787 of yacc.c  */
#line 972 "tree.y"
    {
			int argCount, i;
			argCount = 0;
			lHead = NULL;			  
			struct ArgStruct *AL;
			AL = (yyvsp[(2) - (5)].np)->ArgList;  
			while(AL)
			{
				argCount++;
			  	AL = AL->NEXT;
			}
			(yyvsp[(2) - (5)].np)->value = argCount;	  
			AL = (yyvsp[(2) - (5)].np)->ArgList;	 
			i=0;
			while(AL){
				Linstall(AL->NAME, AL->TYPE, -2 -argCount + i  );
				i++;
				AL = AL->NEXT;		 
			}
		}
    break;

  case 27:
/* Line 1787 of yacc.c  */
#line 994 "tree.y"
    { 
			int lVars = 1;
			struct ArgStruct *AL;
			AL = (yyvsp[(8) - (8)].np)->ArgList;	 
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
			(yyvsp[(8) - (8)].np)->value = lVars -1;
		}
    break;

  case 28:
/* Line 1787 of yacc.c  */
#line 1015 "tree.y"
    {
			fprintf(fp, "%s:\n", (yyvsp[(2) - (12)].np)->NAME);
			
			struct Gsymbol *G;
			
			G = Glookup((yyvsp[(2) - (12)].np)->NAME);
			if(  G == NULL )
			  {
			 	yyerror("Undefined function"); 
				exit(0);			 
			  }

  			if(G->TYPE != (yyvsp[(11) - (12)].np)->TYPE)
			  {
				yyerror("Return type not matching");
				exit(0);
			  } 
			  
			G->isDefined = true;
			  
			if(G->TYPE != (yyvsp[(1) - (12)].np)->TYPE)
			  {
				yyerror("Function type not matching");
			  	exit(0);
			  }	
		  

			if( !( Is_FuncDefMatch ( (yyvsp[(2) - (12)].np)->ArgList, (Glookup((yyvsp[(2) - (12)].np)->NAME))->ARGLIST ) ))
			  {
				yyerror("Argument name not matching"); 
			   	exit(0);
			  }
			fprintf(fp, "PUSH BP\n"); 
			fprintf(fp, "MOV BP, SP\n");
			int i;
			for(  i=0; i<(yyvsp[(8) - (12)].np)->value;++i)
			  {
				fprintf(fp, "PUSH R0\n");
			  }			
			Print_Tree_Code((yyvsp[(10) - (12)].np));			 
			if((yyvsp[(11) - (12)].np)->TYPE == 1)
			  { 
				registers++;
				fprintf(fp, "MOV R%d, BP\n", (registers-1));
				registers++;
				fprintf(fp, "MOV R%d, 2 \n", (registers-1));  
				fprintf(fp, "SUB R%d, R%d\n", (registers-2),  (registers-1));
				registers--;
			     
			Print_Expr_Code((yyvsp[(11) - (12)].np));
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
			  Print_Expr_Code((yyvsp[(11) - (12)].np));
			  fprintf(fp, "MOV [R%d], R%d \n", (registers-2), registers-1);
			  registers--;
			  registers--;
			  }
			 for(  i=0; i<(yyvsp[(8) - (12)].np)->value;++i)
			 {
			 fprintf(fp, "POP R0\n");            
			 }
			 fprintf(fp, "POP BP\n"); 
			 fprintf(fp, "RET\n");
			 lHead = NULL;
			}
    break;

  case 29:
/* Line 1787 of yacc.c  */
#line 1091 "tree.y"
    {   
			(yyval.np)=(yyvsp[(2) - (3)].np);
		}
    break;

  case 30:
/* Line 1787 of yacc.c  */
#line 1096 "tree.y"
    {
			struct ArgStruct *A;
			A = (yyvsp[(1) - (2)].np)->ArgList;
			if(A == NULL)
			{
				(yyvsp[(1) - (2)].np)->ArgList = (yyvsp[(2) - (2)].np)->ArgList;
			}
			else {
				while(A->NEXT)
				{A = A->NEXT;}
				A->NEXT = (yyvsp[(2) - (2)].np)->ArgList;}						
				(yyval.np) = (yyvsp[(1) - (2)].np);
		}
    break;

  case 31:
/* Line 1787 of yacc.c  */
#line 1110 "tree.y"
    { 
			(yyval.np)=malloc(sizeof(struct node));
			(yyval.np)->ArgList = NULL;
		}
    break;

  case 32:
/* Line 1787 of yacc.c  */
#line 1119 "tree.y"
    { 
			(yyval.np)=(yyvsp[(2) - (3)].np);
	 }
    break;

  case 33:
/* Line 1787 of yacc.c  */
#line 1125 "tree.y"
    {
			struct ArgStruct *A;
			A = malloc(sizeof(struct ArgStruct));
			
			A->ref_var = false;
			
			A->NEXT = NULL;
			(yyvsp[(1) - (1)].np)->ArgList = A;
		
			A->NAME = strdup( (yyvsp[(1) - (1)].np)->NAME);
			A->TYPE = (yyvsp[(0) - (1)].np) -> TYPE;
			(yyval.np)=(yyvsp[(1) - (1)].np); 
	}
    break;

  case 34:
/* Line 1787 of yacc.c  */
#line 1140 "tree.y"
    {
			struct ArgStruct *A, *B;
			A = malloc(sizeof(struct ArgStruct));
			A->NEXT = NULL;
			A->ref_var = false;		
			A->NAME = strdup( (yyvsp[(3) - (3)].np)->NAME);
			A->TYPE = (yyvsp[(0) - (3)].np) -> TYPE;
 
			B = (yyvsp[(1) - (3)].np)->ArgList;
	       
		       while( B->NEXT)
		       {
			       B = B->NEXT;
		       }
	   
		      B->NEXT = A;
	      
		      (yyval.np)=(yyvsp[(1) - (3)].np);    
		}
    break;

  case 35:
/* Line 1787 of yacc.c  */
#line 1161 "tree.y"
    {
			(yyval.np) = (yyvsp[(2) - (3)].np);
		}
    break;

  case 36:
/* Line 1787 of yacc.c  */
#line 1166 "tree.y"
    {                                          
			if((yyvsp[(1) - (2)].np)==NULL)
					{
						p = malloc(sizeof( struct node ));
				        p->left = (yyvsp[(2) - (2)].np);   
						p->middle = NULL; 
						p->right = NULL;
						p->TYPE = 3;
						p->NODETYPE = 0;
						(yyval.np)=p;
					}
			else
					{
						p = malloc(sizeof( struct node ));
                        p->left = (yyvsp[(2) - (2)].np);  
						p-> middle = NULL;  
						p->right = NULL;
						p->TYPE = 3;
						p->NODETYPE = 0;
						Construct_Tree( (yyvsp[(1) - (2)].np), p);
						(yyval.np)=(yyvsp[(1) - (2)].np);
					}
                                     }
    break;

  case 37:
/* Line 1787 of yacc.c  */
#line 1191 "tree.y"
    {  
					(yyval.np)=NULL;
                                     }
    break;

  case 38:
/* Line 1787 of yacc.c  */
#line 1198 "tree.y"
    {					
					lp = Llookup((yyvsp[(1) - (4)].np)->NAME);
					if(lp){
					(yyvsp[(1) - (4)].np)->Lentry = lp;
                    (yyvsp[(1) - (4)].np)->TYPE = lp->TYPE;
					}
					else {
					gp = Glookup((yyvsp[(1) - (4)].np)->NAME);	
                   	if(gp == NULL)
					{
					yyerror("Undeclared Variable");
					exit(0);
					}                                     
                    (yyvsp[(1) - (4)].np)->Gentry = gp;
                 	(yyvsp[(1) - (4)].np)->TYPE = gp->TYPE;
					}					
					if ( !( ( ((yyvsp[(1) - (4)].np)->TYPE == 1) && ((yyvsp[(3) - (4)].np)->TYPE == 1) )  || ( ((yyvsp[(1) - (4)].np)->TYPE == 2) && ((yyvsp[(3) - (4)].np)->TYPE == 2) ) ) )
					{ 
						yyerror("Assignment type not Matching"); 
						exit(0);  
					} 
					
					(yyvsp[(2) - (4)].np)->left = (yyvsp[(1) - (4)].np);
					(yyvsp[(2) - (4)].np)->right = (yyvsp[(3) - (4)].np);
					(yyval.np) = (yyvsp[(2) - (4)].np);
          }
    break;

  case 39:
/* Line 1787 of yacc.c  */
#line 1226 "tree.y"
    {  			
					gp = Glookup((yyvsp[(1) - (7)].np)->NAME);	
  					if(gp == NULL)
							{
								yyerror("Undeclared  variable");
								exit(0);
							}                                     
			(yyvsp[(1) - (7)].np)->Gentry = gp;
			(yyvsp[(1) - (7)].np)->TYPE = gp->TYPE;					
			if (!((((yyvsp[(1) - (7)].np)->TYPE == 1) && ((yyvsp[(3) - (7)].np)->TYPE == 1) && ((yyvsp[(6) - (7)].np)->TYPE == 1))  || (((yyvsp[(1) - (7)].np)->TYPE == 2) && ((yyvsp[(3) - (7)].np)->TYPE == 1)&& ((yyvsp[(6) - (7)].np)->TYPE == 2))))
					{ 
						yyerror("Assignment type not Matching"); 
						exit(0);  
					} 				
					(yyvsp[(5) - (7)].np)->left = (yyvsp[(1) - (7)].np);
					(yyvsp[(5) - (7)].np)->middle = (yyvsp[(3) - (7)].np);
					(yyvsp[(5) - (7)].np)->right = (yyvsp[(6) - (7)].np);
					(yyval.np) = (yyvsp[(5) - (7)].np);
			}
    break;

  case 40:
/* Line 1787 of yacc.c  */
#line 1246 "tree.y"
    { 
					(yyvsp[(1) - (5)].np)->left = (yyvsp[(3) - (5)].np);
					(yyvsp[(1) - (5)].np)->right = NULL;
					lp = Llookup((yyvsp[(3) - (5)].np)->NAME);
					if(lp){					
					(yyvsp[(3) - (5)].np)->Lentry = lp;
                   	(yyvsp[(3) - (5)].np)->TYPE = lp->TYPE;			
					}
					else {
					gp = Glookup((yyvsp[(3) - (5)].np)->NAME);	
                   	if(gp == NULL)
					{
						yyerror("Undeclared variable");
						exit(0);
					}
					(yyvsp[(3) - (5)].np)->Gentry = gp;
                	(yyvsp[(3) - (5)].np)->TYPE = gp->TYPE;
					}	
					if ( (yyvsp[(3) - (5)].np)->TYPE != 1)
					{ 
						yyerror("Arguments must be of type integer"); 
						exit(0);  
					}
					(yyval.np)=(yyvsp[(1) - (5)].np);
            }
    break;

  case 41:
/* Line 1787 of yacc.c  */
#line 1272 "tree.y"
    { 
					if ( ((yyvsp[(5) - (8)].np)->TYPE) != 1 ) 
					{ 
						yyerror("Arguments must be of type integer"); 
						exit(0);  
					}
					(yyvsp[(1) - (8)].np)->left = (yyvsp[(3) - (8)].np);
					(yyvsp[(1) - (8)].np)->middle=(yyvsp[(5) - (8)].np);
					(yyvsp[(1) - (8)].np)->right = NULL;
					gp = Glookup((yyvsp[(3) - (8)].np)->NAME);	                   
                   	if(gp == NULL)
							{
								yyerror("Undeclared variable");
								exit(0);
							}                                     
                                     	(yyvsp[(3) - (8)].np)->Gentry = gp;
					(yyvsp[(3) - (8)].np)->TYPE = gp->TYPE;

					
					if ( (yyvsp[(3) - (8)].np)->TYPE != 1)
					{ 
						yyerror("Arguments must be of type integer"); 
						exit(0);  
					} 
					
        
					(yyval.np)=(yyvsp[(1) - (8)].np);
                }
    break;

  case 42:
/* Line 1787 of yacc.c  */
#line 1301 "tree.y"
    {      
					if ((yyvsp[(3) - (5)].np)->TYPE != 1 )
						{ 
							yyerror("Arguments must be of type integer");
							 exit(0);  
						}
					(yyvsp[(1) - (5)].np)->left = (yyvsp[(3) - (5)].np);
					(yyvsp[(1) - (5)].np)->right = NULL;
					(yyval.np)=(yyvsp[(1) - (5)].np);				
				}
    break;

  case 43:
/* Line 1787 of yacc.c  */
#line 1312 "tree.y"
    {   
					if ((yyvsp[(3) - (10)].np)->TYPE != 2 ) 
					{
						yyerror("Arguments msut be of type boolean"); 
						exit(0); 
					}
                                        
					(yyvsp[(1) - (10)].np)->left = (yyvsp[(3) - (10)].np);
					(yyvsp[(1) - (10)].np)->middle=(yyvsp[(6) - (10)].np);
					(yyvsp[(1) - (10)].np)->right = (yyvsp[(8) - (10)].np);
					(yyval.np)=(yyvsp[(1) - (10)].np);

					}
    break;

  case 44:
/* Line 1787 of yacc.c  */
#line 1326 "tree.y"
    {  
					 if ((yyvsp[(3) - (8)].np)->TYPE != 2 ) 
					 {
						yyerror("Arguments msut be of type boolean");
						exit(0);  
					 }

                     (yyvsp[(1) - (8)].np)->left = (yyvsp[(3) - (8)].np);
                     (yyvsp[(1) - (8)].np)->middle=NULL;
					 (yyvsp[(1) - (8)].np)->right = (yyvsp[(6) - (8)].np);
          			 (yyval.np)=(yyvsp[(1) - (8)].np);
                         		}
    break;

  case 45:
/* Line 1787 of yacc.c  */
#line 1343 "tree.y"
    { (yyval.np) = (yyvsp[(1) - (1)].np); }
    break;

  case 46:
/* Line 1787 of yacc.c  */
#line 1345 "tree.y"
    { (yyval.np)=NULL; }
    break;

  case 47:
/* Line 1787 of yacc.c  */
#line 1350 "tree.y"
    {
			struct node *n, *A;
			n = malloc(sizeof(struct node));
			n->TYPE = (yyvsp[(3) - (3)].np)->TYPE;
			n->right = (yyvsp[(3) - (3)].np); 
			n->left = NULL; 
			(yyvsp[(1) - (3)].np)->value += 1;
			A = (yyvsp[(1) - (3)].np);
			while(A->left)
			  {
				A = A->left;
			  }
			A->left = n;	
			(yyval.np)=(yyvsp[(1) - (3)].np);
		}
    break;

  case 48:
/* Line 1787 of yacc.c  */
#line 1366 "tree.y"
    { 
			struct node *n;
			n = malloc(sizeof(struct node));
			n->TYPE = (yyvsp[(1) - (1)].np)->TYPE;
			n->right = (yyvsp[(1) - (1)].np); 
			n->left = NULL;
			n->value = 1;		
			(yyval.np) = n;
		}
    break;

  case 49:
/* Line 1787 of yacc.c  */
#line 1378 "tree.y"
    {	(yyval.np) = (yyvsp[(1) - (1)].np);  }
    break;

  case 50:
/* Line 1787 of yacc.c  */
#line 1380 "tree.y"
    {	(yyval.np) = (yyvsp[(1) - (1)].np);  }
    break;

  case 51:
/* Line 1787 of yacc.c  */
#line 1382 "tree.y"
    {	(yyval.np) = (yyvsp[(1) - (1)].np); }
    break;

  case 52:
/* Line 1787 of yacc.c  */
#line 1384 "tree.y"
    {
        			lp = Llookup((yyvsp[(1) - (1)].np)->NAME);		
        			if(lp){
        			(yyvsp[(1) - (1)].np)->Lentry = lp;
					(yyvsp[(1) - (1)].np)->TYPE = lp->TYPE;
					}				
       				else {
       				gp = Glookup((yyvsp[(1) - (1)].np)->NAME);	
               		if(gp == NULL)
					{
						yyerror("Undeclared variable");
						exit(0);
					}                                     
                 	(yyvsp[(1) - (1)].np)->Gentry = gp;
                    (yyvsp[(1) - (1)].np)->TYPE = gp->TYPE;
					}			
					(yyval.np) = (yyvsp[(1) - (1)].np); 
		}
    break;

  case 53:
/* Line 1787 of yacc.c  */
#line 1402 "tree.y"
    { 
        				gp = Glookup((yyvsp[(1) - (4)].np)->NAME);	
						if(gp == NULL)
							{
								yyerror("Undeclared variable");
								exit(0);
							}                                     
                                     	(yyvsp[(1) - (4)].np)->TYPE = gp->TYPE;
                                     	(yyvsp[(1) - (4)].np)->Gentry = gp;             	
                                     	(yyvsp[(1) - (4)].np)->middle = (yyvsp[(3) - (4)].np);
										(yyval.np) = (yyvsp[(1) - (4)].np); 
				     }
    break;

  case 54:
/* Line 1787 of yacc.c  */
#line 1414 "tree.y"
    { 
					if( ((yyvsp[(1) - (3)].np)->TYPE != 1) || ((yyvsp[(3) - (3)].np)->TYPE != 1) )
                                        {
						yyerror("Arguments must be of type integer");
						 exit(0);  
					}
					 
					(yyvsp[(2) - (3)].np) -> left = (yyvsp[(1) - (3)].np);
		            (yyvsp[(2) - (3)].np)->right = (yyvsp[(3) - (3)].np);
					(yyval.np)=(yyvsp[(2) - (3)].np); 
				     }
    break;

  case 55:
/* Line 1787 of yacc.c  */
#line 1426 "tree.y"
    {  
					if( ((yyvsp[(1) - (3)].np)->TYPE != 1) || ((yyvsp[(3) - (3)].np)->TYPE != 1) )
                                        {
						yyerror("Arguments must be of type integer");
						exit(0);
					}
					 
					(yyvsp[(2) - (3)].np) -> left = (yyvsp[(1) - (3)].np); 
					(yyvsp[(2) - (3)].np)->right = (yyvsp[(3) - (3)].np);  
         			(yyval.np)=(yyvsp[(2) - (3)].np); 
				     }
    break;

  case 56:
/* Line 1787 of yacc.c  */
#line 1437 "tree.y"
    {  
					if( ((yyvsp[(1) - (3)].np)->TYPE != 1) || ((yyvsp[(3) - (3)].np)->TYPE != 1) )
                                        {
						yyerror("Arguments must be of type integer");
						exit(0);
					}
					 
					(yyvsp[(2) - (3)].np) -> left = (yyvsp[(1) - (3)].np); 
					(yyvsp[(2) - (3)].np)->right = (yyvsp[(3) - (3)].np);  
         			(yyval.np)=(yyvsp[(2) - (3)].np); 
				     }
    break;

  case 57:
/* Line 1787 of yacc.c  */
#line 1449 "tree.y"
    {  
					if( ((yyvsp[(1) - (3)].np)->TYPE != 1) || ((yyvsp[(3) - (3)].np)->TYPE != 1) )
                                        {
						yyerror("Arguments must be of type integer");
						exit(0);
					} 
					(yyvsp[(2) - (3)].np) -> left = (yyvsp[(1) - (3)].np); 
					(yyvsp[(2) - (3)].np)->right = (yyvsp[(3) - (3)].np);  
         			(yyval.np)=(yyvsp[(2) - (3)].np); 
				     }
    break;

  case 58:
/* Line 1787 of yacc.c  */
#line 1459 "tree.y"
    {  
					if( ((yyvsp[(1) - (3)].np)->TYPE != 2) || ((yyvsp[(3) - (3)].np)->TYPE != 2) )
                                        {
						yyerror("Arguments must be of type boolean");
						exit(0);
					}
					 
					(yyvsp[(2) - (3)].np) -> left = (yyvsp[(1) - (3)].np); 
					(yyvsp[(2) - (3)].np)->right = (yyvsp[(3) - (3)].np);  
         			(yyval.np)=(yyvsp[(2) - (3)].np); 
				     }
    break;

  case 59:
/* Line 1787 of yacc.c  */
#line 1471 "tree.y"
    {  
					if( ((yyvsp[(1) - (3)].np)->TYPE != 2) || ((yyvsp[(3) - (3)].np)->TYPE != 2) )
                                        {
						yyerror("Arguments must be of type boolean");
						exit(0);
					}
					 
					(yyvsp[(2) - (3)].np) -> left = (yyvsp[(1) - (3)].np); 
					(yyvsp[(2) - (3)].np)->right = (yyvsp[(3) - (3)].np);  
         			(yyval.np)=(yyvsp[(2) - (3)].np); 
				     }
    break;

  case 60:
/* Line 1787 of yacc.c  */
#line 1482 "tree.y"
    { 
					(yyval.np) = (yyvsp[(2) - (3)].np); 
				    }
    break;

  case 61:
/* Line 1787 of yacc.c  */
#line 1485 "tree.y"
    { 
					if( ((yyvsp[(1) - (3)].np)->TYPE != 1) || ((yyvsp[(3) - (3)].np)->TYPE != 1) )
                   {
						yyerror("Arguments must be of type integer");
						exit(0); 
					}					 
					(yyvsp[(2) - (3)].np) -> left = (yyvsp[(1) - (3)].np);
         			(yyvsp[(2) - (3)].np)->right = (yyvsp[(3) - (3)].np); 
					(yyval.np)=(yyvsp[(2) - (3)].np); 
        			      }
    break;

  case 62:
/* Line 1787 of yacc.c  */
#line 1496 "tree.y"
    { 
					if( ((yyvsp[(1) - (3)].np)->TYPE != 1) || ((yyvsp[(3) - (3)].np)->TYPE != 1) )
                                        { 
						yyerror("Arguments must be of type integer");
						exit(0);  
					}
					         
           			(yyvsp[(2) - (3)].np) -> left = (yyvsp[(1) - (3)].np); 
					(yyvsp[(2) - (3)].np)->right = (yyvsp[(3) - (3)].np);  	
					(yyval.np)=(yyvsp[(2) - (3)].np);  
				    }
    break;

  case 63:
/* Line 1787 of yacc.c  */
#line 1508 "tree.y"
    {   
					if( ((yyvsp[(1) - (3)].np)->TYPE != 1) || ((yyvsp[(3) - (3)].np)->TYPE != 1) )
                                        { 
						yyerror("Arguments must be of type integer");
						exit(0);  
					}

					 (yyvsp[(2) - (3)].np) -> left = (yyvsp[(1) - (3)].np);
					 (yyvsp[(2) - (3)].np)->right = (yyvsp[(3) - (3)].np);   
       				 (yyval.np)=(yyvsp[(2) - (3)].np);  
				    }
    break;

  case 64:
/* Line 1787 of yacc.c  */
#line 1520 "tree.y"
    {   
					if( ((yyvsp[(1) - (3)].np)->TYPE != 1) || ((yyvsp[(3) - (3)].np)->TYPE != 1) )
                                        { 
						yyerror("Arguments must be of type integer");
						exit(0);  
					}

					 (yyvsp[(2) - (3)].np) -> left = (yyvsp[(1) - (3)].np);
					 (yyvsp[(2) - (3)].np)->right = (yyvsp[(3) - (3)].np);   
       				 (yyval.np)=(yyvsp[(2) - (3)].np);  
				    }
    break;

  case 65:
/* Line 1787 of yacc.c  */
#line 1532 "tree.y"
    {   
					if( ((yyvsp[(1) - (3)].np)->TYPE != 1) || ((yyvsp[(3) - (3)].np)->TYPE != 1) )
                                        { 
						yyerror("Arguments must be of type integer");
						exit(0);  
					}

					 (yyvsp[(2) - (3)].np) -> left = (yyvsp[(1) - (3)].np);
					 (yyvsp[(2) - (3)].np)->right = (yyvsp[(3) - (3)].np);   
       				 (yyval.np)=(yyvsp[(2) - (3)].np);  
				    }
    break;

  case 66:
/* Line 1787 of yacc.c  */
#line 1546 "tree.y"
    {   
					if((yyvsp[(2) - (2)].np)->TYPE != 2) 
                                        { 
						yyerror("Arguments must be of type boolean");
						exit(0);  
					}
					 (yyvsp[(1) - (2)].np) -> left = (yyvsp[(2) - (2)].np);					    
       				 (yyval.np)=(yyvsp[(1) - (2)].np);  
				    }
    break;

  case 67:
/* Line 1787 of yacc.c  */
#line 1557 "tree.y"
    {  
			(yyvsp[(1) - (1)].np)->Gentry = Glookup((yyvsp[(1) - (1)].np)->NAME);
			if((yyvsp[(1) - (1)].np)->Gentry==NULL) 
			  {
				yyerror("Function definition not found"); 
				exit(0); 
			  }			   
 			if(!(((yyvsp[(1) - (1)].np)->Gentry)->isFUNCTION))    
 			  {
 				yyerror("Variable cannot be a function"); 
 				exit(0); 
 			  }                        
                        (yyvsp[(1) - (1)].np)->TYPE = ((yyvsp[(1) - (1)].np)->Gentry)->TYPE;       
		}
    break;

  case 68:
/* Line 1787 of yacc.c  */
#line 1573 "tree.y"
    {                             
			if( ! (Is_FuncCallMatch((Glookup((yyvsp[(1) - (5)].np)->NAME))->ARGLIST, (yyvsp[(4) - (5)].np) ) ) )        
     			  {
				yyerror("Function call not matching");
     			   	exit(0);
     			  }     			   
			(yyvsp[(1) - (5)].np)->left = (yyvsp[(4) - (5)].np);    
			(yyvsp[(1) - (5)].np)->value =  ((yyvsp[(4) - (5)].np))? (yyvsp[(4) - (5)].np)->value : 0;     
			(yyvsp[(1) - (5)].np)->NODETYPE = 15;     		
			(yyval.np) = (yyvsp[(1) - (5)].np); 
		}
    break;

  case 69:
/* Line 1787 of yacc.c  */
#line 1585 "tree.y"
    {  
			(yyvsp[(3) - (3)].np)->Gentry = Glookup((yyvsp[(3) - (3)].np)->NAME);
			if((yyvsp[(3) - (3)].np)->Gentry==NULL) 
			  {
				yyerror("Function definitionin ITER not found"); 
				exit(0); 
			  }			   
 			if(!(((yyvsp[(3) - (3)].np)->Gentry)->isFUNCTION))    
 			  {
 				yyerror("ITER needs a function a argument"); 
 				exit(0); 
 			  }                        
                        (yyvsp[(3) - (3)].np)->TYPE = ((yyvsp[(3) - (3)].np)->Gentry)->TYPE;    
                        if((yyvsp[(3) - (3)].np)->TYPE != 1){
                        yyerror("Return type of argument in ITER must be int");
     			   	exit(0);
                        }   
		}
    break;

  case 70:
/* Line 1787 of yacc.c  */
#line 1604 "tree.y"
    {                             
			if( ! (Is_FuncCallMatch((Glookup((yyvsp[(3) - (8)].np)->NAME))->ARGLIST, (yyvsp[(6) - (8)].np) ) ) )        
     			  {
				yyerror("Function call in ITER not matching");
     			   	exit(0);
     			  }     
     		if ( (yyvsp[(6) - (8)].np)->TYPE != 1 ){
     		yyerror("First argument in Function call in ITER must be INT");
     			   	exit(0);
     		}
			(yyvsp[(3) - (8)].np)->left = (yyvsp[(6) - (8)].np);    
			(yyvsp[(3) - (8)].np)->value =  ((yyvsp[(6) - (8)].np))? (yyvsp[(6) - (8)].np)->value : 0;     
			(yyvsp[(3) - (8)].np)->NODETYPE = 16;  
			(yyvsp[(1) - (8)].np)->right = (yyvsp[(3) - (8)].np);   		
			(yyval.np) = (yyvsp[(1) - (8)].np); 
		}
    break;


/* Line 1787 of yacc.c  */
#line 3378 "y.tab.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


/* Line 2050 of yacc.c  */
#line 1622 "tree.y"

