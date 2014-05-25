%{
#include<ctype.h>
#include "y.tab.h"

#include<string.h>
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
struct node *p;
void yyerror(char*);
%}

%%


main { 
		yylval.np = NULL;
		return MAIN;
	}

return { 
	yylval.np = NULL;
	return RETURN;
	}
	
decl { 
	yylval.np = NULL;
	return DECL;
	}
	
enddecl  { 
	yylval.np = NULL;
	return ENDDECL;
	}

begin { 
	yylval.np = NULL;
	return BEG;
	}	     

end { 
	yylval.np = NULL;
	return END;
	}	     

integer { 
		p = malloc(sizeof( struct node ));
        p->left = NULL;
        p->middle = NULL;
		p->right = NULL;
		p->TYPE = 1;
		p->NODETYPE = 0;  
		yylval.np = p;
		return INT;
	}

boolean { 
		p = malloc(sizeof( struct node ));
    	p->left = NULL;
    	p->middle = NULL;
		p->right = NULL;
		p->TYPE = 2;
		p->NODETYPE = 0;
		yylval.np = p;
		return BOOL;
	}

if  {
		p = malloc(sizeof( struct node ));
        p->left = NULL;
        p->middle = NULL;
		p->right = NULL;
		p->TYPE = 3;
		p->NODETYPE = 4;
		yylval.np = p;
		return IF;
    }    

then  {
		yylval.np = NULL;
		return THEN;
      }    

else  {
		yylval.np = NULL;
		return ELSE;
      }    

endif  {
        yylval.np = NULL;
        return ENDIF;
       }    

read   {
		p = malloc(sizeof( struct node ));
		p->left = NULL;
        p->middle = NULL;
        p->right = NULL;          
        p->TYPE = 3;
        p->NODETYPE = 2;
        yylval.np = p;
        return READ;
       }  

write  {
		p = malloc(sizeof( struct node ));
        p->left = NULL;
        p->middle = NULL;
        p->right = NULL;
        p->TYPE = 3;
        p->NODETYPE = 3;
        yylval.np = p;
        return WRITE;
       }  

while  {
		p = malloc(sizeof( struct node ));
        p->left = NULL;
        p->middle = NULL;
        p->right = NULL;       
        p->TYPE = 3;
        p->NODETYPE = 5;
        yylval.np = p;
        return WHILE;
       }  

do   {
		yylval.np = NULL;
        return DO;
     }  

endwhile  {
		yylval.np = NULL;
        return ENDWHILE;
          }  
ITER {
		p = malloc(sizeof( struct node ));
		p->left = NULL;
        p->middle = NULL;
        p->right = NULL;          
        p->TYPE = 1;
        p->NODETYPE = 16;
        yylval.np = p;
        return ITER;
	
	
	}               
true   {
		p = malloc( sizeof(struct node) ) ;
        p->value = 1;
        p->left = NULL;
        p->middle = NULL;
        p->right = NULL;
        p->TYPE = 2;
        p->NODETYPE = 2;
        yylval.np = p;
        return TRUE;
        }

false   {
		p = malloc( sizeof(struct node) ) ;
        p->value = 0;
        p->left = NULL;
        p->middle = NULL;
        p->right = NULL;
        p->TYPE = 2;
        p->NODETYPE = 2;
        yylval.np = p;
        return FALSE;
        }
        
[a-zA-Z]+  {
		
		p = malloc( sizeof(struct node) ) ;
        p->index = *yytext-'a';        
        p->NAME = malloc(sizeof(char)*10);
        strcpy(p->NAME, yytext);
		p->Gentry = NULL;
		p->Lentry = NULL;       
        p->NODETYPE = 1;        
        p->ArgList = NULL;
        p->left = NULL;
        p->middle=NULL;
        p->right = NULL;
        yylval.np = p;
        return ID;
        
        }

[0-9]+  {
		p = malloc( sizeof(struct node) ) ;
        p->value = atoi(yytext);
        p->left = NULL;
        p->middle = NULL;
        p->right = NULL;
        p->TYPE = 1;
        p->NODETYPE = 2;        
        yylval.np = p;
        return NUM;
        }

[-*+=/]   { 
	p = malloc( sizeof(struct node) ) ;
        switch( *yytext)
         {
		case '+':  	p->TYPE = 1;
	                p->NODETYPE = 3;
                    break;     
        case '*':   p->TYPE = 1;
                    p->NODETYPE = 4;
                    break;     
		case '=':   p->TYPE = 3;
        			p->NODETYPE = 1;
		            break;
		case '-':   p->TYPE = 1;
		            p->NODETYPE = 5;
		            break;         
		case '/':   p->TYPE = 1;
		            p->NODETYPE = 6;
     		        break;                                         
         }
	    p->left = NULL;
        p->middle = NULL;
        p->right = NULL;
        yylval.np = p;
        return *yytext; 
         }

"!"  { 
		p = malloc( sizeof(struct node) ) ;		
        p->TYPE = 2;
        p->NODETYPE = 10;
		p->left = NULL;
        p->middle = NULL;
        p->right = NULL;
        yylval.np = p;
        return *yytext; 
         }            

"||"  { 
		p = malloc( sizeof(struct node) ) ;
        p->TYPE = 2;
        p->NODETYPE = 13;
		p->left = NULL;
        p->middle = NULL;
        p->right = NULL;
        yylval.np = p;
        return *yytext; 
         }            


"&&"  { 
		p = malloc( sizeof(struct node) ) ; 	
        p->TYPE = 2;
        p->NODETYPE = 14;                           
		p->left = NULL;
        p->middle = NULL;
        p->right = NULL;
        yylval.np = p;
        return *yytext; 
         }            

& { 
	yylval.np = NULL;
	return REF;
	}                  

[(){};,]  {
	yylval.np = NULL; 
        return *yytext;
      }
      
"["     {
	yylval.np = NULL; 
        return *yytext;
      }
               
"]"     {
	yylval.np = NULL; 
        return *yytext;
      }  

\n    {   
	yylineno++;
      }

[ \t]    ;

"<="     {    
		p = malloc( sizeof(struct node) ) ;
        p->TYPE = 2;
        p->NODETYPE = 11;
        p->left = NULL;
        p->middle=NULL;
        p->right = NULL; 
        yylval.np = p;
        return LE;	
       }

">="     {    
		p = malloc( sizeof(struct node) ) ;
        p->TYPE = 2;
        p->NODETYPE = 7;
        p->left = NULL;
        p->middle=NULL;
        p->right = NULL; 
        yylval.np = p;
        return GE;	
       }

[<]    {   
		p = malloc(sizeof(struct node));
        p->TYPE = 2;
        p->NODETYPE = 8;
        p->left = NULL;
        p->middle=NULL;
        p->right = NULL; 
        yylval.np = p;
        return *yytext;
       }

[>]    {    
		p = malloc( sizeof(struct node) ) ;
		p->TYPE = 2;
        p->NODETYPE = 9;
        p->left = NULL;
        p->middle=NULL;
        p->right = NULL; 
        yylval.np = p;
        return *yytext;
       }

==     {    
		p = malloc( sizeof(struct node) ) ;
        p->TYPE = 2;
        p->NODETYPE = 12;
        p->left = NULL;
        p->middle=NULL;
        p->right = NULL; 
        yylval.np = p;
        return EQL;	
     }


.        yyerror("Unknown Character");

%%

void yyerror(char *s) 
{
	fprintf(stderr,"line %d:-  %s\n",yylineno, s);
}

int yywrap()
{
	return 1;
}

int main() 
{	
	yyin = fopen("sin.txt","r");
	yyparse();
	fclose(yyin);
	return 0;
}
