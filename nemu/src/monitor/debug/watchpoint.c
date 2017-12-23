#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_list[NR_WP];
static WP *head, *free_;

void init_wp_list() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_list[i].NO = i;
		wp_list[i].next = &wp_list[i + 1];
	}
	wp_list[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_list;
}

/* TODO: Implement the functionality of watchpoint */

void printf_wp()
{
	WP *p=head;
	for(;p!=NULL;p=p->next){
		printf("%d  %s  %d\n",p->NO,p->expr,p->initial);
	}
}

WP* new_wp(){
	if (free_==NULL) assert(0);
	WP *q=free_,*p=NULL;
	while(q->next->next!=NULL)
		q=q->next;
	p=q->next;
	q->next=NULL;//delete free_'last,p=last 
	if(head==NULL) head=p;
	else{
	WP *new=head;
	while(new->next!=NULL)
		new=new->next;
	new->next=p;//add p to the head
}
return p;
}

void free_wp(WP *wp){
	WP *new=NULL;
	if(wp==head){
		new=head;
		head=head->next;}
	else {
		WP *p=head;
		while(p->next!=wp) p=p->next;
		new=wp;
		p->next=wp->next;}
	new->next=NULL;
	WP *p=free_;
	if(p==NULL)
	   	free_=new;
	else{
	while(p->next!=NULL) p=p->next;
	p->next=new;
	}
}

WP *find_wp(int no){
	WP *p=head;
	while(p!=NULL){
		if(p->NO==no) return p;
		else p=p->next;}
		printf("can't find the point\n");
		return NULL;
}

bool check_wp()
{
	WP *p=head;
	bool judge=true;
	for(;p!=NULL;p=p->next)
	{
		bool suc=true;
		int n=expr(p->expr,&suc);
		if (suc==false){
			printf("The expression is failing\n");
			return false;}
		else
			if(p->initial!=n){
printf("%s the initial answer is %d,change to %d\n",p->expr,p->initial,n); 
p->initial=n;
judge=false;}
	}
		return judge;
	}
