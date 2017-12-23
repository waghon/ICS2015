#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, EQ,DEMICAL_NUMBER,HEXADEMICAL_NUMBER,REG_NAME,NEQ,AND,OR,
	REVERSE,DERE,NAGETIVE_NUMBER,CHANGE_FACTOR

	/* TODO: Add more token types */

};

bool find_match(char *str);

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/*  TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
 	 */

	{" +",	NOTYPE},				// spaces
	{"\\+", '+'},					// plus
	{"-",'-'},
	{"\\*",'*'},
	{"/",'/'},
	{"\\(",'('},
	{"\\)",')'},
	{"!=",NEQ},
	{"&&",AND},
	{"\\|\\|",OR},
	{"0x[0-9a-fA-F]+",HEXADEMICAL_NUMBER},
	{"[0-9]+",DEMICAL_NUMBER},
	{"\\$e[acdb]x",REG_NAME},
	{"\\$e[bs]p",REG_NAME},
	{"\\$e[sd]i",REG_NAME},
	{"\\$[acdb][lh]",REG_NAME},
	{"\\$[acdb]x",REG_NAME},
	{"\\$[bs]p",REG_NAME}, 
	{"\\$[sd]i",REG_NAME},
	{"\\$eip",REG_NAME},
	{"!",REVERSE},
	{"==", EQ},						// equal
	{"[0-9a-zA-Z_]+",CHANGE_FACTOR}
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

 	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
 		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret == 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}	

typedef struct token {
	int type;
	char str[32];
	int prior;
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

 	while(e[position] != '\0') {
		/* Try all rules one by one. */
 		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

			//	Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array ``tokens''. For certain 
				 * types of tokens, some extra actions should be performed.
				 */
				
 				switch(rules[i].token_type) {
					case NOTYPE:break;
					case '+':tokens[nr_token].type='+';
							 tokens[nr_token].prior=4;
							 nr_token++;break;
					case '-':
							 if(nr_token!=0&&(tokens[nr_token-1].type==DEMICAL_NUMBER||tokens[nr_token-1].type==')')) {
							 tokens[nr_token].type='-';
							  tokens[nr_token].prior=4; }
							 else
							 {			tokens[nr_token].type=NAGETIVE_NUMBER;
							  tokens[nr_token].prior=2; } 
							 nr_token++;break;
					case '*':
	 if(nr_token!=0&&(tokens[nr_token-1].type==DEMICAL_NUMBER||tokens[nr_token-1].type==')'))
	 { tokens[nr_token].prior=3; 
							 tokens[nr_token].type='*'; }
	 else
	 { tokens[nr_token].prior=2; 
		  tokens[nr_token].type=DERE; } 
							 nr_token++;break; 
				    case '/':tokens[nr_token].type='/';
							  tokens[nr_token].prior=3; 
							 nr_token++;break;
				    case '(':tokens[nr_token].type='(';nr_token++;break;
					case ')':tokens[nr_token].type=')';nr_token++;break;
					case EQ:
							 tokens[nr_token].type=EQ;
							  tokens[nr_token].prior=7;
							 nr_token++; break;
				    case NEQ:
							 tokens[nr_token].type=NEQ;
							  tokens[nr_token].prior=7;
							 nr_token++; break;
                    case AND:
							 tokens[nr_token].type=AND;
							  tokens[nr_token].prior=11;
							 nr_token++; break;
					case OR:
							  tokens[nr_token].type=OR;
							 tokens[nr_token].prior=12;
							nr_token++; break;  
					case REVERSE:
							 tokens[nr_token].type=REVERSE;
							  tokens[nr_token].prior=2;
							 nr_token++; break;
					case HEXADEMICAL_NUMBER:
							 tokens[nr_token].type=DEMICAL_NUMBER;
							 char q=e[position];
							 e[position]='\0';
							 strcpy(tokens[nr_token].str,substr_start);
							 e[position]=q;
							 int n;
							 sscanf(tokens[nr_token].str,"0x%x",&n);
							 sprintf(tokens[nr_token].str,"%d",n);
							 nr_token++;
							 break;
					case DEMICAL_NUMBER:
							 tokens[nr_token].type=DEMICAL_NUMBER;
								char p=e[position];
									e[position]='\0';
									strcpy(tokens[nr_token].str,substr_start);
									e[position]=p;
					 Assert(strlen(tokens[nr_token].str)<=31,"data overflow");
									 nr_token++;
									 break;
									//nr_token++;
								/*int i;
									for(i=0;i<substr_len;i++)
									{
									tokens[nr_token].str[i]=substr_start[i];
								 	}
									tokens[nr_token].str[i]='\0';*/
	              case REG_NAME:
							tokens[nr_token].type=DEMICAL_NUMBER;
							 char w=e[position]; 
							  e[position]='\0';  
							   strcpy(tokens[nr_token].str,substr_start+1);
							    e[position]=w;
						int i=R_EAX;
								for(;i<=R_EDI;i++)
							{if(strcmp(tokens[nr_token].str,regsl[i])==0)
							sprintf(tokens[nr_token].str,"%d",reg_l(i));
							}
						      for(i=R_EAX;i<=R_EDI;i++)
							 {if(strcmp(tokens[nr_token].str,regsw[i])==0) 
							  sprintf(tokens[nr_token].str,"%d",reg_w(i));}
							   for(i=R_EAX;i<=R_EDI;i++)
							 {if(strcmp(tokens[nr_token].str,regsb[i])==0)
							 sprintf(tokens[nr_token].str,"%d",reg_b(i));}
							   if(strcmp(tokens[nr_token].str,"eip")==0)
							sprintf(tokens[nr_token].str,"%d",cpu.eip);
					Assert(strlen(tokens[nr_token].str)<=31,"data overflow");
								nr_token++;		  
								break;
					case CHANGE_FACTOR:
							// tokens[nr_token].type=DEMICAL_NUMBER;
							 q=e[position];
							 e[position]='\0';
							 strcpy(tokens[nr_token].str,substr_start);
							 e[position]=q;
							 if(find_match(tokens[nr_token].str)==true)
							 {
								 tokens[nr_token].type=DEMICAL_NUMBER;
								 nr_token++;
							 }
							 else
								 return false;
							// int n;
							// sscanf(tokens[nr_token].str,"0x%x",&n);
							// sprintf(tokens[nr_token].str,"%d",n);
							// nr_token++;
							 break;

					default: panic("please implement me");
				}

				break;
 			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
 		}
	}

	return true; 
}
bool judge;
bool check_parentheses(int p,int q)
{
	int count=0;
	bool success=true;
//	if (tokens[p].type!='(') success=false;
//	else
	int i=p;
		for(;i<=q;i++)
		{
			if(tokens[i].type=='(')
				count++;
			if(tokens[i].type==')')
				count--;
			if(count<0){
			   	judge=false;return false;}
			if(count==0&&i!=q) success=false;}
	 return success;}


uint32_t eval(int p,int q){
if(p>q)
{	printf("The expression is failing\n");
judge=false;
return 0;}
else if(p==q){
	if(tokens[p].type!=DEMICAL_NUMBER)
	 {   printf("The expression is failing\n"); 
 		 judge=false;
	 return 0;} 
	else{
		int n;
		sscanf(tokens[p].str,"%d",&n);
return n;
 	}
} 
else if(check_parentheses(p,q)==true)
	return eval(p+1,q-1);
	else{
		if(judge==false)
			return 0;
		int i=p;
		int op=0;
		int max=0;
		int count=0;
		for(;i<=q;i++)
		{if(tokens[i].type==DEMICAL_NUMBER)continue;
			if(tokens[i].type=='(') {count++;continue;}
			if(tokens[i].type==')') {count--;continue;}
			if(count>0) continue;
			if(tokens[i].prior>=max)
			{
				if(tokens[i].prior>max||tokens[i].prior!=2)
					op=i;
			max=tokens[i].prior;
			}
		}
		int val1=1;
	if(tokens[op].type!=REVERSE&&tokens[op].type!=DERE&&tokens[op].type!=NAGETIVE_NUMBER)
		val1=eval(p,op-1);
		int val2=eval(op+1,q);
		if(judge==false)
			return 0;
		switch(tokens[op].type){
			case '+':return val1+val2;
			case '-':return val1-val2;
	   	 case '*':return val1*val2;
		 case '/':return val1/val2;
		case EQ: return val1==val2;
		case NEQ: return val1!=val2;
		case AND: return val1&&val2;
		case OR: return val1||val2;
		case REVERSE: return !val2;
		case DERE: return swaddr_read(val2,4,DS);
		case NAGETIVE_NUMBER: return -val2;
		 default:assert(0);}
	}
}
uint32_t expr(char *e, bool *success) {
//	printf("sda");
	if(!make_token(e)) {
		*success = false;}
	else
		{
//		int i;
	//	for(i=0;i<nr_token;i++)
	//		printf("%d %s\n",tokens[i].type,tokens[i].str);
	//	return 0;
		judge=true;
		int ans=eval(0,nr_token-1);
		if(judge==true)
			return ans;
		else *success=false;

	}

	/* TODO: Insert codes to evaluate the expression. */
	//panic("please implement me");
	return 0;
}

