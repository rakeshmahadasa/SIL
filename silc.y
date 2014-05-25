%{

#include<stdlib.h>
#include<stdio.h>
#include<math.h>

int usedreg = -1, label = 0, prevusedreg,itervalue;
FILE *f;
FILE *r;
int SP = 0;
int sizecount = 0,BP_decrement=0,BP_increment=1;
int t = 0;
char *currFnName;
struct Tnode
{
	int TYPE;
	int NODETYPE;
	int VALUE;
	char *NAME;
	struct Tnode *nodeArgList;
	struct Tnode *ptr1, *ptr2, *ptr3;
}currArgTree;

struct Lsymbol
{
	char *NAME;
	int TYPE;
	int BINDING;
	int SIZE;
	struct Lsymbol *NEXT;
	struct Argstruct *ArgList;
	struct Fsymbol *localPtr;
}start;

struct Fsymbol
{
	char *NAME;
	int TYPE;
	int BINDING;
	int SIZE;
	struct Fsymbol *NEXT;
}*currLTable;


struct Argstruct
{
	char *NAME;
	int TYPE;
	struct Argstruct* NEXT;
}*st;


struct Tnode* create(struct Tnode *c1, struct Tnode *c2, struct Tnode *c3)
{
	struct Tnode *new = malloc(sizeof(struct Tnode));
	new->ptr1 = c1;
	new->ptr2 = c2;
	new->ptr3 = c3;
	return new;
}

void assign(struct Tnode *c, struct Tnode *c1, struct Tnode *c2, struct Tnode *c3)
{
	
	c->ptr1 = c1;
	c->ptr2 = c2;
	c->ptr3 = c3;
	
}

struct Lsymbol* Llookup(char *NAME)
{
	struct Lsymbol *temp = start.NEXT;
	
	while (temp != NULL && temp->NAME != NULL && strcmp(temp->NAME, NAME))
		{temp = temp->NEXT;
		
		}
	if (temp == NULL)
	{
		printf("undeclared variable %s\n", NAME);
		exit(0);
	}
	
	return temp;
}

struct Fsymbol* Flookup(struct Fsymbol *localTable, char *NAME)
{
	struct Fsymbol *temp = localTable;
	if (temp == NULL)
	{
		struct Lsymbol *temp1 = Llookup(NAME);
		if(temp1 != NULL)
		{	temp =malloc(sizeof(struct Fsymbol));
			temp->NAME = temp1->NAME;
	temp->TYPE =temp1->TYPE;
	temp->BINDING =temp1->BINDING;
	temp->SIZE = temp1->SIZE;
	return temp;	
		}
	}
	temp = (localTable->NEXT)->NEXT;
	
	while (temp != NULL && temp->NAME != NULL && strcmp(temp->NAME, NAME))
		temp = temp->NEXT;
	if (temp == NULL)
	{
		struct Lsymbol *temp1 = Llookup(NAME);
		if(temp1 != NULL)
		{	temp =malloc(sizeof(struct Fsymbol));
			temp->NAME = temp1->NAME;
	temp->TYPE =temp1->TYPE;
	temp->BINDING =temp1->BINDING;
	temp->SIZE = temp1->SIZE;	
		}
	}
	
	return temp;
}



void declargvar(struct Tnode *A,int TYPE, struct Lsymbol *newnode, struct Argstruct *tem, struct Fsymbol *newlocalptr)
{
	if (A == NULL) return;
	declargvar(A->ptr1, TYPE, newnode, tem, newlocalptr);
		//struct Argstruct *stem;
		
			if (newnode->ArgList == NULL)
			{
				tem = malloc(sizeof(struct Argstruct));
				
				
				tem->NAME = A->ptr2->NAME;
				tem->TYPE = TYPE;
				//stem = tem;
				newnode->ArgList = tem;
				
			}
			else
			{	
				tem = newnode->ArgList;
				while(tem->NEXT != NULL)
					tem = tem->NEXT;
				struct Argstruct *newtem = malloc(sizeof(struct Argstruct));
				newtem->NAME = A->ptr2->NAME;
			
				newtem->TYPE = TYPE;
				tem->NEXT = newtem;
				
				
			}


			struct Fsymbol *nlocalptr = malloc(sizeof(struct Fsymbol));
			struct Fsymbol *temp;
			temp = newlocalptr;
			sizecount++;
			
			
			while (temp->NEXT != NULL && strcmp(temp->NEXT->NAME, A->ptr2->NAME))
				temp = temp->NEXT;
			if (temp->NEXT != NULL && !strcmp(temp->NEXT->NAME, A->ptr2->NAME))
			{	
				printf("Variable %s already declared\n", A->ptr2->NAME);
				exit(0);
			}
			
			nlocalptr->NAME = A->ptr2->NAME;
			
			
			nlocalptr->TYPE = TYPE;
			nlocalptr->BINDING = BP_decrement;
			BP_decrement--;
			nlocalptr->SIZE = 1;
			nlocalptr->NEXT = NULL;
			temp->NEXT = nlocalptr;
			
			
			
		
}

void declarg(struct Tnode *A, struct Lsymbol *newnode, struct Argstruct *tem, struct Fsymbol *newlocalptr)
{
	if(A == NULL) return;
	declarg(A->ptr1, newnode, tem, newlocalptr);
	declargvar(A->ptr2,A->ptr2->TYPE, newnode, tem, newlocalptr);


}

void simpledecl(char *NAME, int TYPE, struct Lsymbol *globalTable, struct Fsymbol *localTable)
{	


	if(globalTable)
	{	
		struct Lsymbol *newnode = malloc(sizeof(struct Lsymbol));
		struct Lsymbol *temp;
		temp = &start;
		while (temp->NEXT != NULL && strcmp(temp->NEXT->NAME, NAME))
			temp = temp->NEXT;
		if (temp->NEXT != NULL && !strcmp(temp->NEXT->NAME, NAME))
		{	
			printf("Variable %s already declared\n", NAME);
			exit(0);
		}
	
		newnode->NAME = NAME;
		newnode->TYPE = TYPE;
		newnode->BINDING = ++SP;
		newnode->SIZE = 1;
		newnode->NEXT = NULL;
		temp->NEXT = newnode;	
	}
	else
	{
		struct Fsymbol *newnode = malloc(sizeof(struct Fsymbol));
		struct Fsymbol *temp;
		temp = localTable;
		sizecount++;
	
	
		while (temp->NEXT != NULL && strcmp(temp->NEXT->NAME, NAME))
			temp = temp->NEXT;
		if (temp->NEXT != NULL && !strcmp(temp->NEXT->NAME, NAME))
		{	
			printf("Variable %s already declared\n", NAME);
			exit(0);
		}

		newnode->NAME = NAME;
		newnode->TYPE = TYPE;
		newnode->BINDING = BP_increment;
		BP_increment++;
		newnode->SIZE = 1;
		newnode->NEXT = NULL;
		temp->NEXT = newnode;	
	}
}

void declvar(struct Tnode *p, int type, struct Lsymbol *globalTable, struct Fsymbol *localTable)
{	
	if (p == NULL) return;
	
	declvar(p->ptr1, type, globalTable, localTable);
	
	if (p->ptr2->TYPE == 1)
	{	printf("dddd\n");
		simpledecl(p->ptr2->NAME, type, globalTable, localTable);
		p->ptr2->TYPE = type;
		
	}
	
	else if(p->ptr2->TYPE == 4)
	{	
		struct Lsymbol *newnode = malloc(sizeof(struct Lsymbol));
		struct Lsymbol *temp;
		temp = &start;
		
		while (temp->NEXT != NULL && strcmp(temp->NEXT->NAME, p->ptr2->NAME))
			temp = temp->NEXT;
		if (temp->NEXT != NULL && !strcmp(temp->NEXT->NAME, p->ptr2->NAME))
		{	
			printf("Variable %s already declared\n", p->ptr2->NAME);
			exit(0);
		}
		newnode->NAME = p->ptr2->NAME;
		newnode->TYPE = type;
		newnode->BINDING = ++SP;
		newnode->SIZE = 1;
		newnode->NEXT = NULL;
		struct Fsymbol *ptr = malloc(sizeof(struct Fsymbol));
		
		temp->NEXT = newnode;
		
		struct Argstruct *tem = NULL;
	
		struct Fsymbol *sizeHead = malloc(sizeof(struct Fsymbol)); 
	        sizeHead->SIZE = 0;
		
		newnode->ArgList = tem;		
		newnode->localPtr = sizeHead;
		sizeHead->NEXT = ptr;
		sizecount = 0;	

		BP_decrement = -3;
		declarg(p->ptr2->nodeArgList, newnode, tem, ptr);
		BP_decrement = -1;
		//sizeHead->SIZE = sizecount;
		
		
		
					
		
	}
	else
	{
		if(globalTable)
		{
			struct Lsymbol *newnode = malloc(sizeof(struct Lsymbol));
			struct Lsymbol *temp;
			temp = &start;


			while (temp->NEXT != NULL && strcmp(temp->NEXT->NAME, p->ptr2->NAME))
				temp = temp->NEXT;
			if (temp->NEXT != NULL && !strcmp(temp->NEXT->NAME, p->ptr2->NAME))
			{	
				printf("Variable %s already declared\n", p->ptr2->NAME);
				exit(0);
			}
			newnode->NAME = p->ptr2->NAME;
			newnode->TYPE = type;
			newnode->SIZE = p->ptr2->ptr1->VALUE;
			newnode->BINDING = ++SP;
			SP += newnode->SIZE;
			newnode->NEXT = NULL;
			p->ptr2->TYPE = type;
			temp->NEXT = newnode;
		}
		else
		{
			struct Fsymbol *newnode = malloc(sizeof(struct Fsymbol));
			struct Fsymbol *temp;
			temp = localTable;
			sizecount += p->ptr2->ptr1->VALUE;
		
			while (temp->NEXT != NULL && strcmp(temp->NEXT->NAME, p->ptr2->NAME))
				temp = temp->NEXT;
			if (temp->NEXT != NULL && !strcmp(temp->NEXT->NAME, p->ptr2->NAME))
			{	
				printf("Variable %s already declared\n", p->ptr2->NAME);
				exit(0);
			}
			newnode->NAME = p->ptr2->NAME;
			newnode->TYPE = type;
			newnode->SIZE = p->ptr2->ptr1->VALUE;
			newnode->BINDING = BP_increment++;
			BP_increment += newnode->SIZE;
			newnode->NEXT = NULL;
			p->ptr2->TYPE = type;
			temp->NEXT = newnode;
		}	
	}
	
	
}

int compare(struct Argstruct* a, struct Tnode* b)
{
	
	while (b != NULL && a != NULL)
	{

		if (a->TYPE == b->ptr2->TYPE)
		{
			a = a->NEXT;
			b = b->ptr1;
		}
		else
		{			

			return 0;
		}
	}
	
	if(a != NULL || b != NULL)
	{ 
		return 0;

	}

	else
		return 1;
}


void treeprint(struct Tnode *x);
void execute(struct Tnode *);
void pstart();
void pstop();
void setRegisters();
void retrieveRegisters();
void filecreate();
int getregister();
void freeregister();
void typecheck(struct Tnode *,struct Tnode *,struct Tnode *);
void fnTypeCheck1(struct Tnode *, int);
void fnTypeCheck(struct Tnode *);
int argnamecheck(struct Tnode*);
void printFnName(char *Name);
void codegenFnCall(struct Tnode *);
void calleeAction1(char *Name);
void calleeAction2(char *Name,struct Tnode *);
void initialize();
struct Tnode* extractfirst(struct Tnode *);
int implementIter(struct Tnode *,int);
void pushexpnew(struct Tnode *);
int codegenFnCall1(struct Tnode *,int);
extern FILE * yyin;
%}

%union
{
struct Tnode *t;
}


%token <t> BEG END INTEGER MAIN DECL ENDDECL READ TYP WRITE ITER NUM VAR BOOL IF THEN ENDIF ELSE WHILE DO ENDWHILE EQ  GE LE NE SQ SQRT AND OR NOT RETURN '('')'','  '+' '*' '/' '-' '=' '<' '>' ';' '[' ']' '\n'

%type <t> SLIST STMT EXPR Varlist Prog GDECL GDefblock Mainblock ArgList BODY ExprList FdefList Fdef LDefblock VARJumper RetStmt

%nonassoc '<' '>' EQ GE LE NE
%right '=' SQ SQRT
%left AND OR NOT  
%left '+' '-'
%left '/'

%left '*'



%%

Prog: GDefblock FdefList Mainblock
;

FdefList : FdefList Fdef  {$$ = create($1,$2,NULL);} 
	 | Fdef  {$$ = create(NULL,$1,NULL);}
	;

Fdef : TYP VARJumper '(' ArgList ')' '{' LDefblock BODY '}' {
			$2->nodeArgList = $4; 
			$7->nodeArgList = $4; 
			st = Llookup(currFnName)->ArgList; 
			fnTypeCheck($4); 
			if($1->TYPE == ((Llookup($2->NAME))->TYPE))
			{
				assign($2,$4,$7,$8);$$ = $2; 
			} 
			else 
			{
				printf("Functin Decl and Def Type Mismatch\n"); 
				exit(0);
			}  
			setRegisters();  
			execute($8->ptr2); 
			calleeAction2(currFnName,$8->ptr3); 
			currLTable = NULL; 
			retrieveRegisters();
		}
	;
LDefblock : LDefblock TYP Varlist';'{ 
			$$ = create($1,$3,NULL);  
			declvar($3,$2->TYPE,NULL,(Llookup(currFnName)->localPtr)->NEXT);
		}
		    
	|   TYP Varlist';' {  
			$$ = create(NULL,$2,NULL);
			      
			struct Fsymbol *localTable = Llookup(currFnName)->localPtr;
			      
			sizecount = 0;
			declvar($2,$1->TYPE,NULL,localTable->NEXT);
			localTable->SIZE += sizecount;
			currLTable = Llookup(currFnName)->localPtr;
			calleeAction1(currFnName);

		} 
	;
VARJumper:VAR {
			$$ = $1;currFnName = $1->NAME;
		}
;



Mainblock: INTEGER MAIN '('')''{' BODY '}'  { 
			fprintf(f,"MAIN:\n");
			execute($6->ptr2);  
			$$ = NULL; pstop();
		}
	;
 
GDefblock: DECL GDECL ENDDECL {$$ = $2; filecreate(); pstart();initialize();}
	;
BODY:  BEG SLIST RetStmt END {$$ = create(NULL,$2,$3); }
	;
RetStmt: RETURN EXPR ';' {$$=$2; }
	;
GDECL:GDECL TYP Varlist';'{assign($2,$3,NULL,NULL); $$ = create($1,$2,NULL); declvar($3,$2->TYPE,&start,NULL);}	
	|TYP Varlist ';' {assign($1,$2,NULL,NULL); $2->TYPE = $1->TYPE; declvar($2,$1->TYPE,&start,NULL); $$ = create(NULL,$1,NULL);}
	;

SLIST :  SLIST STMT ';'  {$$ = create($1,$2,NULL);}
     |STMT ';'{$$ = create(NULL,$1,NULL);}
    ; 

STMT :VAR '=' EXPR {
			$$ = $2; 
			if (currLTable && Flookup(currLTable,$1->NAME)) 
			{
				$1->TYPE = Flookup(currLTable,$1->NAME)->TYPE;
			} 
			else 
			{
				$1->TYPE = Llookup($1->NAME)->TYPE; 
			} 
			typecheck($1,$3,$$); 
			assign($$,$1,$3,NULL);
		} 
  |VAR '[' EXPR ']' '=' EXPR {
			if ($3->TYPE==1)
			{ $1->TYPE = Llookup($1->NAME)->TYPE;
				typecheck($1,$6,$5);
				assign($1,$3,NULL,NULL);
				assign($5,$1,$6,NULL);
				$$=$5;
																																										
			} 
			else 
			{
				printf("array index not integer\n"); 
				exit(0);}
			} 
  |READ '(' VAR '[' EXPR ']' ')' {
				if ($5->TYPE==1)
				{
					$$=$1; 
					if (currLTable && Flookup(currLTable,$3->NAME))
					{
						$$->NAME=$3->NAME; $$->ptr1=$5;
					} 
					else if(Llookup($3->NAME))
					{
						$$->NAME=$3->NAME; 
						$$->ptr1=$5;
					}
				} 
				else 
				{
					printf("array index not integer\n"); 
					exit(0);
				}
			}
  |IF '(' EXPR ')' THEN SLIST ELSE SLIST ENDIF {
				$$=$1;
				typecheck($3,NULL,$$);
				$$->ptr1=$3;
				$$->ptr2=$6;
				$$->ptr3=$8;
			}
  |IF '(' EXPR ')' THEN SLIST ENDIF {
				$$=$1;
				typecheck($3,NULL,$$);
				$$->ptr1=$3;
				$$->ptr2=$6;
			}
  |WHILE '(' EXPR ')' DO SLIST ENDWHILE {
				$$=$1;typecheck($3,NULL,$$);
				$$->ptr1=$3;
				$$->ptr2=$6;
			}
  |READ '(' VAR ')'{
  				$$=$1; 
  				if (currLTable && Flookup(currLTable,$3->NAME))
  				{
  					$$->NAME=$3->NAME;
  				} 
  				else if(Llookup($3->NAME)){
  				$$->NAME=$3->NAME;
  				}
  			}
  |WRITE '(' EXPR ')' {$$=$1;$$->ptr1=$3;}
  
 ;
Varlist:Varlist ',' VAR '[' NUM ']'  {assign($3,$5,NULL,NULL);$3->TYPE=3;$$=create($1,$3,NULL);$$->TYPE=1;} 
	|Varlist ',' VAR {$$=create($1,$3,NULL);$$->TYPE=1;$3->TYPE=1;}
	|VAR '[' NUM ']' {if($3->VALUE<=0){printf("negative size given for an array\n"); exit(0);}assign($1,$3,NULL,NULL);$1->TYPE=3;$$=create(NULL,$1,NULL);$$->TYPE=1;}
	|VAR  {$$=create(NULL,$1,NULL);$1->TYPE=1;}
	|VAR '(' ArgList ')' {$1->nodeArgList=$3;$$=create(NULL,$1,NULL);$1->TYPE=4;}
	|Varlist ',' VAR '(' ArgList ')'  {$3->nodeArgList=$5;$3->TYPE=4;$$=create($1,$3,NULL);$$->TYPE=1;}
	;

ArgList : ArgList ';' TYP Varlist {$4->TYPE=$3->TYPE; $$=create($1,$4,NULL); }
	| TYP Varlist {$2->TYPE=$1->TYPE;$$=create(NULL,$2,NULL);}
	;

EXPR : NUM {$$=$1;$$->TYPE=1;}
  | VAR {$$=$1;if(currLTable && Flookup(currLTable,$1->NAME)){$$->TYPE=Flookup(currLTable,$1->NAME)->TYPE;} else{$$->TYPE=Llookup($1->NAME)->TYPE;}}
  |VAR '(' ExprList ')'{$$=$1;$$->NAME=$1->NAME; $$->nodeArgList=$3;typecheck(NULL,NULL,$$); if(currLTable && Flookup(currLTable,$1->NAME)){$$->TYPE=Flookup(currLTable,$1->NAME)->TYPE;} else{$$->TYPE=Llookup($1->NAME)->TYPE;}  }
  | EXPR '+' EXPR {$$=$2;typecheck($1,$3,$$);$$->ptr1=$1;$$->ptr2=$3;}
  | EXPR '-' EXPR {$$=$2;typecheck($1,$3,$$);$$->ptr1=$1;$$->ptr2=$3;}
  | EXPR '/' EXPR {$$=$2;typecheck($1,$3,$$);$$->ptr1=$1;$$->ptr2=$3;}
  | EXPR '*' EXPR {$$=$2;typecheck($1,$3,$$);$$->ptr1=$1;$$->ptr2=$3;}
  | EXPR AND EXPR {$$=$2;typecheck($1,$3,$$);$$->ptr1=$1;$$->ptr2=$3;}
  | EXPR OR EXPR {$$=$2;typecheck($1,$3,$$);$$->ptr1=$1;$$->ptr2=$3;}
  | NOT EXPR {$$=$1;typecheck($2,NULL,$$);$$->ptr1=$2;$$->ptr2=NULL;}
  | '(' EXPR ')' {$$=$2;}
  | EXPR EQ EXPR {$$=$2;typecheck($1,$3,$$);$$->ptr1=$1;$$->ptr2=$3;}
  | EXPR GE EXPR {$$=$2;typecheck($1,$3,$$);$$->ptr1=$1;$$->ptr2=$3;}
  | EXPR LE EXPR {$$=$2;typecheck($1,$3,$$);$$->ptr1=$1;$$->ptr2=$3;}
  | EXPR NE EXPR {$$=$2;typecheck($1,$3,$$);$$->ptr1=$1;$$->ptr2=$3;}
  | EXPR '>' EXPR {$$=$2;typecheck($1,$3,$$);$$->ptr1=$1;$$->ptr2=$3;}
  | EXPR '<' EXPR {$$=$2;typecheck($1,$3,$$);$$->ptr1=$1;$$->ptr2=$3;}
  |VAR'['EXPR']'{assign($1,$3,NULL,NULL);$1->TYPE=3;$$=$1;$$->TYPE=1;}
  |SQ'('EXPR')'{assign($1,NULL,$3,NULL);$$=$1;$$->TYPE=1;}
  |SQRT'('EXPR')'{assign($1,NULL,$3,NULL);$$=$1;$$->TYPE=1;} 
  |BOOL{$$=$1;$$->TYPE=2;}
  |ITER '(' VAR '('ExprList')'')' {$3->nodeArgList = $5; typecheck(NULL,NULL,$3); $1->ptr1=$3;$1->TYPE = Flookup(currLTable,$3->NAME)->TYPE;$3->nodeArgList = $5;$$=$1; }	 
;

ExprList: EXPR ',' ExprList {$$=create($3,$1,NULL);}
  |EXPR {$$=create(NULL,$1,NULL);}
;

%%
void main()
{	yyin=fopen("input.txt" , "r");
	yyparse();
	return ;
}

void pstart()
{
	fprintf(f,"START\n");
	
}

void printFnName(char *Name)
{
	fprintf(f,"%s:\n",Name);

}

void pstop()
{
	fprintf(f,"HALT\n");
	fclose(f);
}
void filecreate()
{

	
	f = fopen("MC.txt", "w");
}

void pushexp(struct Tnode *t)
{
	if(t == NULL) return;
	pushexp(t->ptr1);
	int a = evaluate(t->ptr2);
	
	fprintf(f,"PUSH R%d\n",a);
	freeregister();
	sizecount++;

}

struct Tnode* extractfirst(struct Tnode *t)
{	printf("fffffkkk\n");
	return t->ptr2;
	


}

void codegenFnCall(struct Tnode *fn)
{
	int rvalue,a,b;
	int i = usedreg;
	
	int j = i;
	while(i >= 0)
	{	
		fprintf(f,"PUSH R%d\n",i);
		i--;
	}
	
	struct Tnode *t;
	t = fn->nodeArgList;
	sizecount = 0;

	pushexp(t);
	
	if(getregister())
	{
	
	rvalue = usedreg;
		fprintf(f,"PUSH R%d\n",usedreg);
	}
	fprintf(f,"CALL %s\n",fn->NAME);

	fprintf(f,"POP R%d\n",rvalue);
	int size = sizecount;
	if(getregister())
	{	a = usedreg;
		fprintf(f,"MOV R%d,%d\n",usedreg,size);
	}
	
	if(getregister())
	{	
		b = usedreg;
		fprintf(f,"MOV R%d,SP\n",usedreg);

	}
	fprintf(f,"SUB R%d,R%d\n",b,a);
	
	fprintf(f,"MOV SP,R%d\n",usedreg);
	freeregister();
	freeregister();
	

	i = 0;
	while(i <= j)
	{	
		fprintf(f,"POP R%d\n",i);
		i++;
	}
} 

void calleeAction1(char *fnName)
{
	printFnName(currFnName);
	fprintf(f,"PUSH BP\n");
	fprintf(f,"MOV BP,SP\n");
	struct Fsymbol *temp = Llookup(fnName)->localPtr;

	int a = temp->SIZE;
	while(a>0)
	{
		fprintf(f,"PUSH R0\n");
		a--;
	}


		
}
void calleeAction2(char *fnName, struct Tnode *t)
{
	int a = evaluate(t);
	if(getregister())
	{
		fprintf(f,"MOV R%d,BP\n",usedreg);
	}
	if(getregister())
	{
		fprintf(f,"MOV R%d,2\n",usedreg);
	}
	fprintf(f,"SUB R%d,R%d\n",usedreg-1,usedreg);
	freeregister();

	fprintf(f,"MOV [R%d],R%d\n",usedreg,a);
	
	freeregister();
	freeregister();
	struct Fsymbol *temp = Llookup(fnName)->localPtr;

	a = temp->SIZE;
	while(a>0)
	{
		fprintf(f,"POP R0\n");
		a--;
	}
	
	fprintf(f,"POP BP\n");
	fprintf(f,"RET\n");	

}

void initialize()
{
	fprintf(f,"MOV SP,%d\n",SP);
	fprintf(f,"JMP MAIN\n");


}

void typecheck(struct Tnode *a, struct Tnode *b, struct Tnode *c)
{	
	switch(c->NODETYPE)
	{
			case '-':
			case '*':
			case '/':
			case SQ:
			case SQRT:			
			case '+': if(a->TYPE==1 &&b->TYPE==1)
					c->TYPE=1;
				else
				{	printf("Type missmatch");
					exit(0);
				}
				 break;
			case '<':
			case '>':
			case GE:
			case LE:	
			case EQ:
			case NE: if(a->TYPE==1 &&b->TYPE==1)
					c->TYPE=2;
				else
				{	printf("Type missmatch");
					exit(0);
				}
				 break;
			case AND:
			case OR: if(a->TYPE==2 && b->TYPE==2)
					c->TYPE=2;
				else
				{	printf("Type missmatch");
					exit(0);
				}
				 break;
			case NOT:
			case WHILE:
			case IF: if(a->TYPE==2)
					c->TYPE=2;
				else
				{	printf("Type missmatch");
					exit(0);
				}
				 break;
			case VAR: if(!(Llookup(c->NAME)) )
				  {
					printf("Variable not declared as a function\n");
				   }
				 else
				  {	
					int a = compare(Llookup(c->NAME)->ArgList,c->nodeArgList);
					if(a==1)
					{
						c->TYPE=Llookup(c->NAME)->TYPE;
					}
					else
					{
						printf("Function Call Type missmatch\n");
						exit(0);
					}
				}
				 	break;



			case '=': if(a->TYPE != b->TYPE)
				   {	
					printf("Type missmatch\n");
						exit(0);
				   }
				 break;
	}

	
}



void pushexpnew(struct Tnode *t)
{
	if(t == NULL) return;
	pushexp(t->ptr1);
	int a = evaluate(t->ptr2);
	
	fprintf(f,"PUSH R%d\n",a);
	freeregister();
	sizecount++;

}

int codegenFncall1(struct Tnode *fn,int freg)
{
	int rvalue,a,b;
	int i = usedreg;
	
	int j = i;
	while(i >= 0)
	{	
		fprintf(f,"PUSH R%d\n",i);
		i--;
	}
	
	struct Tnode *t;
	t = fn->nodeArgList;
	sizecount = 0;

	pushexpnew(t->ptr1);
	fprintf(f,"PUSH R%d\n",freg);
	if(getregister())
	{
	
	rvalue = usedreg;
		fprintf(f,"PUSH R%d\n",usedreg);
	}
	fprintf(f,"CALL %s\n",fn->NAME);

	fprintf(f,"POP R%d\n",rvalue);
	int size = sizecount+1;
	if(getregister())
	{	a = usedreg;
		fprintf(f,"MOV R%d,%d\n",usedreg,size);
	}
	
	if(getregister())
	{	
		b = usedreg;
		fprintf(f,"MOV R%d,SP\n",usedreg);

	}
	fprintf(f,"SUB R%d,R%d\n",b,a);
	
	fprintf(f,"MOV SP,R%d\n",usedreg);
	freeregister();
	freeregister();
	

	i = 0;
	while(i <= j)
	{	
		fprintf(f,"POP R%d\n",i);
		i++;
	}
	return usedreg;
} 


int implementIter(struct Tnode *A,int fvalue)
{	
	
	
	int rvalue = codegenFncall1(A,fvalue);
	int l1 = label;
	fprintf(f,"L%d: \n",label++);
	if(getregister()){
	fprintf(f,"MOV R%d,R%d\n",usedreg,itervalue);
	}
	
	fprintf(f,"NE R%d,R%d\n",usedreg,rvalue);
	fprintf(f,"JZ R%d,L%d\n",usedreg,label++);
	freeregister();
	fprintf(f,"MOV R%d,R%d\n",itervalue,rvalue);
	freeregister();
	fvalue = itervalue;
	rvalue = codegenFncall1(A,fvalue);
	fprintf(f,"JMP L%d \n",l1);
	fprintf(f,"L%d: \n",l1+1);	 
		
	return itervalue;
		 
		
}	

void fnTypeCheck(struct Tnode *A)
{	
	if(A == NULL) return;
	fnTypeCheck(A->ptr1);
	fnTypeCheck1(A->ptr2, A->ptr2->TYPE);


}

void fnTypeCheck1(struct Tnode *A, int TYPE)
{
	if (A == NULL) return;
	fnTypeCheck1(A->ptr1,TYPE);
			
		if (!(strcmp(st->NAME, A->ptr2->NAME)) && (st->TYPE == TYPE))
		{
			st = st->NEXT;
			
		}
		else
		{	printf("Fn Defn Type missmatch\n");
			exit(0);
		}
			
			
		
		
		
		
}


int argnamecheck(struct Tnode* c)
{
		
	struct Argstruct *a = (Llookup(c->NAME))->ArgList; 
	struct Tnode *b = c->nodeArgList;
 
	while (a != NULL && b != NULL)
	{
		
		if (!(strcmp(a->NAME, b->ptr2->NAME)))
		{
			a = a->NEXT;
			b = b->ptr1;
		}
		else
		{			

			return 0;
		}
	}
	
	if(a != NULL || b != NULL)
	{ 
		return 0;

	}

	else
	{ 
		return 1;
	}
}

yyerror (const char *str) {printf("yerror%s\n",str);}

void setRegisters()
{
	prevusedreg = usedreg;	
	usedreg = -1;
}

void retrieveRegisters()
{
	usedreg = prevusedreg;
}


int getregister()
{
	if(usedreg<7)
	{
		usedreg++;
		return 1;
	}
	else
		return 0;	
}

void freeregister()
{	
	usedreg--;
}


int evaluate(struct Tnode *t)
{	
	if(t==NULL) return 0;
	else if (t->ptr1==NULL&&t->ptr2==NULL)
	{	
		if(t != NULL)
		{
		if((t->NAME==NULL || t->NODETYPE== BOOL) && getregister())
		{	fprintf(f,"MOV R%d, %d\n",usedreg,t->VALUE);
			return usedreg;
		}
		else if(t->NODETYPE==VAR &&t->nodeArgList != NULL&&((Llookup(t->NAME))->SIZE)==1 )
		{
		codegenFnCall(t);
		
			
		}

		else if(t->NODETYPE==VAR && currLTable && ((Flookup(currLTable,t->NAME))->SIZE)==1)
		{	if(getregister()){

				fprintf(f, "MOV R%d, BP\n",usedreg);
				if(getregister()){
				fprintf(f, "MOV R%d, %d\n",usedreg,(Flookup(currLTable,t->NAME)->BINDING));
				fprintf(f, "ADD R%d, R%d\n",(usedreg),usedreg-1);
				
				fprintf(f, "MOV R%d, [R%d]\n",usedreg-1,usedreg);
				freeregister();	
				}
				return usedreg;
			}
			exit(0);
		}	
		
		else if(t->NODETYPE==VAR&&((Llookup(t->NAME))->SIZE)==1)
		{	if(getregister()){
				fprintf(f, "MOV R%d, [%d]\n",usedreg,(Llookup(t->NAME)->BINDING));
				return usedreg;
			}
			exit(0);
		}			
	}}

	else if(t->NODETYPE==VAR && currLTable && ((Flookup(currLTable,t->NAME))->SIZE!=1))
		{
		
			int b=evaluate(t->ptr1);
				if(getregister()){
											
							fprintf(f, "MOV R%d, BP\n",usedreg);
							if(getregister()){
								fprintf(f, "MOV R%d, %d\n",usedreg,(Flookup(currLTable,t->NAME)->BINDING));
								fprintf(f, "ADD R%d, R%d\n",(usedreg),usedreg-1);
							fprintf(f, "ADD R%d, R%d\n",b,usedreg);
							freeregister();
							freeregister();
						}}
				else
					exit(0);
				if(getregister())
					fprintf(f, "MOV R%d, [R%d]\n",usedreg,b);
				else
					exit(0);
				b = usedreg;
				freeregister();
				return b;
		}

	else if(t->NODETYPE==VAR&&(Llookup(t->NAME))->SIZE!=1)
		{
			int b=evaluate(t->ptr1);
				if(getregister()){
							fprintf(f, "MOV R%d, %d\n",usedreg,(Llookup(t->NAME)->BINDING));
							fprintf(f, "ADD R%d, R%d\n",b,usedreg);
							freeregister();
						}
				else
					exit(0);
				if(getregister())
					fprintf(f, "MOV R%d, [R%d]\n",usedreg,b);
				else
					exit(0);
				b = usedreg;
				freeregister();
				return b;
		}
	else if(t->NODETYPE==ITER)
	{	printf("fffffkkk%s\n",(t->ptr1->NAME));
		struct Tnode *temp = extractfirst((t->ptr1)->nodeArgList);
		printf("fffffkkkwww\n");
		int a = evaluate(temp);
		itervalue = a;
		implementIter(t->ptr1,a);
	}

	else
			{
				int x=evaluate(t->ptr1);  int y=evaluate(t->ptr2);
				switch(t->NODETYPE)
				{
					case '+':fprintf(f,"ADD R%d, R%d\n",x,y);
						freeregister();
						return x;
						 break;
					case '-':fprintf(f,"SUB R%d, R%d\n",x,y);
						freeregister();
						return x;
						break;
					case '*':fprintf(f,"MUL R%d, R%d\n",x,y);
						freeregister();
						return x;						 
						break;
					case '/':fprintf(f,"DIV R%d, R%d\n",x,y);
						freeregister();
						return x;						 
						break;
					case '>':fprintf(f,"GT R%d, R%d\n",x,y);
						freeregister();
						return x;						 
						break;
					case '<':fprintf(f,"LT R%d, R%d\n",x,y);
						freeregister();
						return x;						 
						break;
					case EQ :fprintf(f,"EQ R%d, R%d\n",x,y);
						freeregister();
						return x;						 
						break;
					case GE :fprintf(f,"GE R%d, R%d\n",x,y);
						freeregister();
						return x;						 
						break;
					case LE :fprintf(f,"LE R%d, R%d\n",x,y);
						freeregister();
						return x;						 
						break;
					case NE :fprintf(f,"NE R%d, R%d\n",x,y);
						freeregister();
						return x;						 
						break;
					case AND:fprintf(f,"AND R%d, R%d\n",x,y);
						freeregister();
						return x;						 
						break;
					case OR :fprintf(f,"OR R%d, R%d\n",x,y);
						freeregister();
						return x;						 
						break;
					case NOT :fprintf(f,"NOT R%d\n",x);
						
						return x;						 
						break;
					}
		}	

}
	
void execute1(struct Tnode* t)
{
	if(t->NODETYPE=='=')
	{
		
		if(t->ptr1->NODETYPE==VAR && currLTable && (Flookup(currLTable,t->ptr1->NAME))->SIZE!=1)
		{
			int b=evaluate(t->ptr2);
			int a=evaluate(t->ptr1->ptr1);
			if(getregister()){

				fprintf(f, "MOV R%d, BP\n",usedreg);
				if(getregister()){
					fprintf(f, "MOV R%d, %d\n",usedreg,(Flookup(currLTable,t->ptr1->NAME)->BINDING));
					fprintf(f, "ADD R%d, R%d\n",(usedreg-1),usedreg);

				freeregister();
				fprintf(f, "ADD R%d, R%d\n",a,usedreg);
				freeregister();
				}}
			else
			exit(0);
			fprintf(f, "MOV [R%d], R%d\n",a,b);
			freeregister();
			freeregister();
		}
		else if(t->ptr1->NODETYPE==VAR && currLTable && Flookup(currLTable,t->ptr1->NAME)->SIZE==1){
			
				int b=evaluate(t->ptr2);
				
				if(getregister()){
					fprintf(f, "MOV R%d, BP\n",usedreg);
					if(getregister()){
				fprintf(f, "MOV R%d, %d\n",usedreg,(Flookup(currLTable,t->ptr1->NAME)->BINDING));	
					      
						fprintf(f, "ADD R%d, R%d\n",(usedreg-1),usedreg);						
						freeregister();				
						fprintf(f, "MOV [R%d], R%d\n",usedreg,b);
						freeregister();
						freeregister();}
					}}		
		
		else if(t->ptr1->NODETYPE==VAR && Llookup(t->ptr1->NAME)->SIZE!=1 )
		{
			int b=evaluate(t->ptr2);
			int a=evaluate(t->ptr1->ptr1);

			if(getregister()){
				fprintf(f, "MOV R%d, %d\n",usedreg,(Llookup(t->ptr1->NAME)->BINDING));
				fprintf(f, "ADD R%d, R%d\n",a,usedreg);
				freeregister();
				}
			else
			exit(0);
			fprintf(f, "MOV [R%d], R%d\n",a,b);
			freeregister();
			freeregister();
		}
		else if(t->ptr1->NODETYPE==VAR && Llookup(t->ptr1->NAME)->SIZE==1){
				
				int b=evaluate(t->ptr2);
				
				
				fprintf(f, "MOV [%d], R%d\n",Llookup(t->ptr1->NAME)->BINDING,usedreg);
				freeregister();}
	}
	else if(t->NODETYPE==READ)
	{	
		
		int y;
		if(getregister())
		{		
				y=usedreg;					
				fprintf(f, "IN R%d\n",usedreg);
		}
		else
				exit(0);

		if(currLTable && (Flookup(currLTable,t->NAME))->SIZE!=1)
		{	
					int a=evaluate(t->ptr1);
						if(getregister())
							fprintf(f, "MOV R%d, %d\n",usedreg,((Flookup(currLTable,t->NAME))->BINDING));
						else
							exit(0);
						fprintf(f, "ADD R%d, R%d\n",a,usedreg);
						freeregister();
						fprintf(f, "MOV [R%d], R%d\n",a,y);
						freeregister();
		}
		else if(currLTable &&(Flookup(currLTable,t->NAME))->SIZE==1)
			{		fprintf(f, "MOV [%d], R%d\n",(Flookup(currLTable,t->NAME))->BINDING,y);
			
			}
	
		else if((Llookup(t->NAME))->SIZE!=1)
		{	
					int a=evaluate(t->ptr1);
						if(getregister())
							fprintf(f, "MOV R%d, %d\n",usedreg,((Llookup(t->NAME))->BINDING));
						else
							exit(0);
						fprintf(f, "ADD R%d, R%d\n",a,usedreg);
						freeregister();
						fprintf(f, "MOV [R%d], R%d\n",a,y);
						freeregister();
		}
		else if((Llookup(t->NAME))->SIZE==1)
			{		
			fprintf(f, "MOV [%d], R%d\n",(Llookup(t->NAME))->BINDING,y);
			
			}
		freeregister();
	}
	else if(t->NODETYPE==WRITE)
	{	
		int x=evaluate(t->ptr1);
		fprintf(f,"OUT R%d \n",x);
		freeregister();
	}
		
	else if(t->NODETYPE==WHILE)
	{
		int l =label;
		fprintf(f,"L%d: \n",label++); 
		
		int x=evaluate(t->ptr1);
		fprintf(f,"JZ R%d, L%d \n",x,label++); 
		freeregister();
		execute(t->ptr2);

		fprintf(f,"JMP L%d \n",l);
		fprintf(f,"L%d: \n",l+1); 
 

	 }

	else if(t->NODETYPE==IF)
	{
		int l= label;
	
		int x=evaluate(t->ptr1);
		fprintf(f, "JZ R%d, L%d\n",x,label++);
		freeregister();

		execute(t->ptr2);

		if(t->ptr3!=NULL)
		{
			fprintf(f, "JMP L%d\n",label++);
		}

		fprintf(f, "L%d:\n", l);

		if(t->ptr3!=NULL)
		{
			execute(t->ptr3);
			fprintf(f, "L%d:\n", l+1);
		}
		

	}
}

void execute(struct Tnode *t)
{
	if(t==NULL)
	return;
	execute(t->ptr1);
	execute1(t->ptr2);
}


 void treeprint(struct Tnode *x)
{
	if(x==NULL) return;
	printf(" ( ");
	switch(x->NODETYPE)
	{
		case '+':printf(" + ");
			 break;
		case '-':printf(" - ");
			 break;
		case '>':printf(" > ");
			 break;
		case '<':printf(" < ");
			 break;
		case EQ :printf(" == ");
			 break;
		case IF :printf("IF");
			 break;
		case READ:printf("READ");
			 break;
		case WRITE:printf("WRITE");
			 break;
		case WHILE:printf("WHILE(");
			 break;
		case NUM:printf("%d",x->VALUE);
			 break;
		case VAR:printf("%c",(char)x->VALUE);
			 break;
		case '=':printf("=");
			 break;
	}
	
	treeprint(x->ptr1);
	treeprint(x->ptr2);
	treeprint(x->ptr3);	
	printf(" ) ");
	//printf(" %d ",x->NODETYPE);
}


