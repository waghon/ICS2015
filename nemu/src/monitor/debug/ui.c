#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint32_t);
void print_stack_line();
/* We use the ``readline'' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}

static int cmd_q(char *args) {
	return -1;
}

static int cmd_help(char *args);
static int cmd_exec(char *args){
	char *arg = strtok(NULL," ");
	int n;
	if(arg==NULL)
		n=1;
	else
		sscanf(arg,"%d",&n);
	cpu_exec(n);
	return 0;
}	
//const char *l[] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"};
// const char *w[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"}; 
// const char *b[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"}; 
static int cmd_print(char *args){
	char *arg = strtok(NULL," ");
	if(strcmp(arg,"r")==0)
	{
		int i;
	for(i=R_EAX;i<=R_EDI;i++)
		printf("%s  %d  %x\n",regsl[i],reg_l(i),reg_l(i));
	
	 for(i=R_AX;i<=R_DI;i++)
		  printf("%s  %d  %x\n",regsw[i],reg_w(i),reg_w(i));
	  for(i=R_AL;i<=R_BH;i++)
		   printf("%s  %d  %x\n",regsb[i],reg_b(i),reg_b(i));
		printf("eip  %d  %x\n",cpu.eip,cpu.eip);
		printf("CF %d  %x\n",cpu.CF,cpu.CF);
		printf("OF %d  %x\n",cpu.OF,cpu.OF);
		printf("PF %d  %x\n",cpu.PF,cpu.PF);
		printf("SF %d  %x\n",cpu.SF,cpu.SF);
		printf("ZF %d  %x\n",cpu.ZF,cpu.ZF);
		printf("DF %d  %x\n",cpu.DF,cpu.DF);
		printf("IF %d  %x\n",cpu.IF,cpu.IF);
		printf("gdtr %d  %x\n",cpu.gdtr.base,cpu.gdtr.base);
		printf("CS base %d  %x\n",cpu.segr[CS].hidden_cache.base,cpu.segr[CS].hidden_cache.base);
		printf("CS limit %d  %x\n",cpu.segr[CS].hidden_cache.limit,cpu.segr[CS].hidden_cache.limit);
		printf("CR3 %x\n",cpu.CR3.val);
		printf("IDTR %x\n",cpu.idtr.base);
	}
	else if(strcmp(arg,"w")==0){
		printf_wp();}
return 0;
}
static int cmd_memory(char *args){
     char *arg = strtok(NULL," ");
	  int n;
	  sscanf(arg,"%d",&n);
      int a;
	  arg=strtok(NULL," ");
	  sscanf(arg,"0x%x",&a);
	  int i;
	  for(i=0;i<n;i++){
	uint32_t p=swaddr_read(a,4,DS);
	printf("0x%08x ",p);
	a+=4;}
	  printf("\n");
return 0;
}

static int cmd_wp(char *args){
	bool suc=true;
	int n=expr(args,&suc);
	if(suc==false) {
		printf("The expression is failing\n");
		return 0;}
		else{
			WP *p=new_wp();
		    p->initial=n;
			strcpy(p->expr,args);
		}
return 0;
}

static int cmd_delete(char *args){
 char *arg=strtok(NULL," ");
 int n;
 sscanf(arg,"%d",&n);
 WP *q=find_wp(n);
 if (q!=NULL)
 free_wp(q);
 return 0;
}
 
static int cmd_calc(char *args){
bool suc=true;	
//printf("sda");
uint32_t p=expr(args,&suc);
//uint32_t p=1;
if(suc==true)
printf("%d  %x\n",p,p);
else 
	printf("The expression is failing\n");
return 0;
}

static int cmd_bt(char *args)
{
	print_stack_line();
	return 0;
}
static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },
	{"si","Execute singly",cmd_exec},
	{"info","Print",cmd_print},
	{"x","Print memory",cmd_memory},
	{"p","Calculate the expression",cmd_calc},
	{"w","Set the watchpoint",cmd_wp},
	{"d","Delete the watchpoint",cmd_delete},
	{"bt","Print the stack line",cmd_bt}
	/* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if (arg == NULL) {
		/* no argument given */
	 	for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

void ui_mainloop() {
	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}
