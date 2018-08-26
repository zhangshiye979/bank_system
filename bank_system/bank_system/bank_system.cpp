#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stddef.h>

const int max_log = 100;

typedef struct _log {
	char saving_clerk[20];
	//用于保存当前时间
	char trade_time[50];
	//char trade_site[100];
	float take_in;
	float pay_out;

}Log;

//节点,既是一个账号
typedef struct _node
{
	float balance;
	char name[20];
	char bankNo[20];
	char id[20];
	char passwd[7];
	//用于记录交易信息已保存的数量,只会保存最近100条记录
	int log_num;
	//用于保存交易信息
	Log log[100];
	struct _node *pre;
	struct _node *next;
} Node, *pNode;

//指向节点的结构体，用于记录链表的首尾，以及当前位置
typedef struct _list
{
	pNode head;
	pNode pN;
	pNode tail;
} List, *pList;

//创建账号
void creatNode(pList pli);
int display();
//用于查找账号,查找pli中是否有node存在，有则返回1，没有则返回0,并将哪个节点储存在pList->pN
int search(pList pli, Node node);
void showInfo(pList pli);
void updateNode(pList pli);
void delNode(pList pli);
void deposit(pList pli);
void withdraw(pList pli);
void get_time(char trade_time[50]);

int main()
{
	int sel;
	int is_break = 0;
	List list = {
		NULL,
		NULL,
		NULL,
	};
	//
	while (is_break == 0)
	{
		sel = display();
		switch (sel)
		{
		case 1:
			creatNode(&list);
			break;
		case 2:
			showInfo(&list);
			break;
		case 3:
			updateNode(&list);
			break;
		case 4:
			delNode(&list);
			break;
		case 5:
			deposit(&list);
			break;
		case 6:
			withdraw(&list);
			break;
		case 7:
			is_break = 1;
		default:
			is_break = 1;
		}
	}
	return 0;
}

void creatNode(pList pli)
{
	//判断账号是否已经存在
	int is_exist = 0;
	pNode pnode;
	pnode = (pNode)malloc(sizeof(Node));
	printf("请输入账号：");
	scanf("%s", pnode->bankNo);
	//寻找账号是否存在
	is_exist = search(pli, *pnode);
	//如果账号不存在
	if (is_exist == 0) {
		printf("请输入姓名：");
		scanf("%s", pnode->name);
		printf("请输入id：");
		scanf("%s", pnode->id);
		printf("请输入余额：");
		scanf("%f", &(pnode->balance));
		printf("请输入密码：");
		scanf("%s", pnode->passwd);
		//创建pnode时，将log_num置零
		pnode->log_num = 0;
		pnode->next = NULL;
		if (pli->head != NULL)
		{
			pli->tail->next = pnode;
			pnode->pre = pli->tail;
			pli->tail = pnode;
		}
		else
		{
			pli->head = pnode;
			pli->head->pre = NULL;
			pli->tail = pnode;
		}
	}
	else {
		printf("账号已存在！\n");
	}
}

int display()
{
	int sel;
	printf("===========请选择服务==========\n");
	printf("1.创建账号\n");
	printf("2.查询账号\n");
	printf("3.修改账号\n");
	printf("4.删除账号\n");
	printf("5.存款\n");
	printf("6.取款\n");
	printf("7.退出\n");
	printf("请选择服务:");
	scanf("%d", &sel);
	return sel;
}

int search(pList pli, Node node)
{
	int is_exist = 0;
	if (pli->head != NULL) {
		for (pNode pnode = pli->head; pnode != NULL; pnode = pnode->next) {
			if (strcmp(node.bankNo, pnode->bankNo) == 0) {
				pli->pN = pnode;
				is_exist = 1;
				break;
			}
		}
	}
	return is_exist;
}

void showInfo(pList pli)
{
	pNode pnode = NULL;
	Node node;
	//默认全部查询
	int sel = 0;
	int search_style = 0;
	//n既是log信息数量
	int n;
	//用log临时变量储存得来的交易信息
	Log log;
	//用于储存输入信息
	Log log_search;
	printf("\n=================请选择查询内容===============\n");
	printf("1.查询账号信息\n");
	printf("2.查询交易信息\n");
	printf("请输入查询内容：");
	scanf("%d", &search_style);
	switch (search_style) {
	case 1:
		printf("\n=================请选择查询方式===============\n");
		printf("1.按账号查询\n");
		printf("2.按身份证号查询\n");
		printf("3.按姓名查询\n");
		printf("4.查询全部账号\n");
		printf("请输入查询方式：");
		scanf("%d", &sel);
		switch (sel) {
		case 1:
			printf("请输入账号：");
			scanf("%s", node.bankNo);
			printf("\n======账号信息如下=======\n");
			printf("%-20s%-20s%-20s%-10s\n", "账号", "身份证号", "姓名", "余额");
			if (pli->head != NULL) {
				for (pnode = pli->head; pnode != NULL; pnode = pnode->next) {
					if (strcmp(node.bankNo, pnode->bankNo) == 0) {
						printf("%-20s", pnode->bankNo);
						printf("%-20s", pnode->id);
						printf("%-20s", pnode->name);
						printf("%-10.2f\n", pnode->balance);
					}
				}
			}
			break;
		case 2:
			printf("请输入身份证号：");
			scanf("%s", node.id);
			printf("\n======账号信息如下=======\n");
			printf("%-20s%-20s%-20s%-10s\n", "账号", "身份证号", "姓名", "余额");
			if (pli->head != NULL) {
				for (pnode = pli->head; pnode != NULL; pnode = pnode->next) {
					if (strcmp(node.id, pnode->id) == 0) {
						printf("%-20s", pnode->bankNo);
						printf("%-20s", pnode->id);
						printf("%-20s", pnode->name);
						printf("%-10.2f\n", pnode->balance);
					}
				}
			}
			break;
		case 3:
			printf("请输入姓名：");
			scanf("%s", node.name);
			printf("\n======账号信息如下=======\n");
			printf("%-20s%-20s%-20s%-10s\n", "账号", "身份证号", "姓名", "余额");
			if (pli->head != NULL) {
				for (pnode = pli->head; pnode != NULL; pnode = pnode->next) {
					if (strcmp(node.name, pnode->name) == 0) {
						printf("%-20s", pnode->bankNo);
						printf("%-20s", pnode->id);
						printf("%-20s", pnode->name);
						printf("%-10.2f\n", pnode->balance);
					}
				}
			}
			break;
		case 4:
		default:
			printf("\n======账号信息如下=======\n");
			printf("%-20s%-20s%-20s%-10s\n", "账号", "身份证号", "姓名", "余额");
			if (pli->head != NULL) {
				for (pnode = pli->head; pnode != NULL; pnode = pnode->next) {
					printf("%-20s", pnode->bankNo);
					printf("%-20s", pnode->id);
					printf("%-20s", pnode->name);
					printf("%-10.2f\n", pnode->balance);
				}
			}
		}
		break;
	case 2:
		printf("\n=================请选择查询方式===============\n");
		printf("1.按账号查询\n");
		printf("2.储蓄员\n");
		printf("3.全部\n");
		printf("请输入查询方式：");
		scanf("%d", &sel);
		switch (sel) {
		case 1:
			printf("请输入账号：");
			scanf("%s", node.bankNo);
			printf("\n======交易信息如下=======\n");
			printf("%-20s%-20s%-20s%-20s%-10s%-10s%-50s\n", "账号", "身份证号", "姓名", "储蓄员","存款","取款","交易时间");
			if (pli->head != NULL) {
				//找到符合条件的节点的节点，可能不止一个(虽然现在只能有一个，设定账号唯一)
				for (pnode = pli->head; pnode != NULL; pnode = pnode->next) {
					if (strcmp(node.bankNo, pnode->bankNo) == 0) {
						//取得pnode里面log信息数量
						n = pnode->log_num;
						//取出pnode里面的信息
						for (int i = 0; i < n; i++) {
							log = pnode->log[i];
							printf("%-20s", pnode->bankNo);
							printf("%-20s", pnode->id);
							printf("%-20s", pnode->name);
							printf("%-20s", log.saving_clerk);
							printf("%-10.2f", log.take_in);
							printf("%-10.2f", log.pay_out);
							printf("%-50s\n", log.trade_time);
						}
					}
				}
			}
			break;
		case 2:
			printf("请输入储蓄员：");
			scanf("%s",log_search.saving_clerk);
			printf("\n======交易信息如下=======\n");
			printf("%-20s%-20s%-20s%-20s%-10s%-10s%-50s\n", "账号", "身份证号", "姓名", "储蓄员", "存款", "取款", "交易时间");
			if (pli->head != NULL) {
				//找到符合条件的节点的节点，可能不止一个(虽然现在只能有一个，设定账号唯一)
				for (pnode = pli->head; pnode != NULL; pnode = pnode->next) {
					//取得pnode里面log信息数量
					n = pnode->log_num;
					//取出每个pnode里面的所有交易信息
					for (int i = 0; i < n; i++) {
						log = pnode->log[i];
						if (strcmp(log_search.saving_clerk, log.saving_clerk) == 0) {
							printf("%-20s", pnode->bankNo);
							printf("%-20s", pnode->id);
							printf("%-20s", pnode->name);
							printf("%-20s", log.saving_clerk);
							printf("%-10.2f", log.take_in);
							printf("%-10.2f", log.pay_out);
							printf("%-50s\n", log.trade_time);
						}
					}
				}
			}
			break;
		default:
			printf("\n======交易信息如下=======\n");
			printf("%-20s%-20s%-20s%-20s%-10s%-10s%-50s\n", "账号", "身份证号", "姓名", "储蓄员", "存款", "取款", "交易时间");
			if (pli->head != NULL) {
				//找到符合条件的节点的节点，可能不止一个(虽然现在只能有一个，设定账号唯一)
				for (pnode = pli->head; pnode != NULL; pnode = pnode->next) {
					//取得pnode里面log信息数量
					n = pnode->log_num;
					//取出pnode里面的信息
					for (int i = 0; i < n; i++) {
						log = pnode->log[i];
						printf("%-20s", pnode->bankNo);
						printf("%-20s", pnode->id);
						printf("%-20s", pnode->name);
						printf("%-20s", log.saving_clerk);
						printf("%-10.2f", log.take_in);
						printf("%-10.2f", log.pay_out);
						printf("%-50s\n", log.trade_time);
					}
				}
			}
		}
	}
}

void updateNode(pList pli)
{
	int sel = 1;
	float balance;
	char name[20];
	Node node;
	printf("请输入要修改的账号：");
	scanf("%s", node.bankNo);
	//如果账号存在
	if (search(pli, node)) {
		printf("\n===========请选择需要修改的数据=============\n");
		printf("1.姓名\n");
		printf("2.余额\n");
		printf("请选择需要修改的数据：");
		scanf("%d", &sel);
		switch (sel) {
		case 1:
			printf("请输入新的名字:");
			scanf("%s", name);
			strcpy(pli->pN->name, name);
			break;
		case 2:
			printf("请输入新余额:");
			scanf("%f", &balance);
			pli->pN->balance = balance;
			break;
		default:
			printf("您的输入有误！");
		}
	}
	else {
		printf("账号不存在！\n");
	}
}

void delNode(pList pli)
{
	Node node;
	printf("请输入要删除的账号：");
	scanf("%s", node.bankNo);
	//如果账号存在
	if (search(pli, node)) {
		//如果pli->pN->next不为空
		if (pli->pN->next != NULL) {
			//如果pli->pN->next不为空而且pli->pN->pre存在
			if (pli->pN->pre != NULL) {
				pli->pN->pre->next = pli->pN->next;
				pli->pN->next->pre = pli->pN->pre;
				free(pli->pN);
				pli->pN = NULL;
			}
			//如果pli->pN->next不为空，但是pli->pN->pre不存在
			//就是删除链表head
			else {
				pli->pN->next->pre = NULL;
				//改变list->head
				pli->head = pli->pN->next;
				free(pli->pN);
				pli->pN = NULL;
			}
		}
		//如果pli->pN->pre不为空,pli->pN->next为空
		else if (pli->pN->pre != NULL) {
			pli->pN->pre->next = NULL;
			//改变list->tail
			pli->tail = pli->pN->pre;
			free(pli->pN);
			pli->pN = NULL;
		}
		//pli->pN->pre和pli->pN->next都为空
		else {
			free(pli->pN);
			pli->head = NULL;
			pli->tail = NULL;
			pli->pN = NULL;
		}
	}
	else {
		printf("账号不存在！\n");
	}

}

void deposit(pList pli)
{
	Node node;
	float balance = 0;
	//n既是log信息数量
	int n;
	//用log临时变量储存得来的交易信息
	Log log;
	printf("请输入账号：");
	scanf("%s", node.bankNo);
	if (search(pli, node)) {
		printf("请输入存款金额：");
		scanf("%f", &balance);
		pli->pN->balance += balance;
		//录入交易信息
		//如果交易信息数量<100
		n = pli->pN->log_num;
		if (n < max_log) {
			printf("请输入储蓄员：");
			scanf("%s", log.saving_clerk);
			log.take_in = balance;
			log.pay_out = 0;
			get_time(log.trade_time);
			//将log临时变量储存的信息写入pli->pN->log[n]
			pli->pN->log[n] = log;
			pli->pN->log_num++;
		}
		else {
			//用for循环将记录的100信息向前推移一步
			for (int i = 1; i < 100; i++) {
				pli->pN->log[i - 1] = pli->pN->log[i];
			}
			printf("请输入储蓄员：");
			scanf("%s", log.saving_clerk);
			log.take_in = balance;
			log.pay_out = 0;
			get_time(log.trade_time);
			//将log临时变量储存的信息写入pli->pN->log[99]
			//n=99
			//n = 99;
			pli->pN->log_num = 99;
			pli->pN->log[99] = log;
		}
	}
	else {
		printf("账号不存在！");
	}
}

void withdraw(pList pli)
{
	Node node;
	float balance = 0;
	int n;
	//用log临时变量储存得来的交易信息
	Log log;
	printf("请输入账号：");
	scanf("%s", node.bankNo);
	//账号存在则继续
	if (search(pli, node)) {
		//密码正确继续
		printf("请输入密码：");
		scanf("%s", node.passwd);
		if (strcmp(node.passwd, pli->pN->passwd) == 0) {
			printf("请输入取款金额：");
			scanf("%f", &balance);
			pli->pN->balance -= balance;
			//录入交易信息
			//如果交易信息数量<100
			n = pli->pN->log_num;
			if (n < max_log) {
				printf("请输入储蓄员：");
				scanf("%s", log.saving_clerk);
				log.take_in = 0;
				log.pay_out = balance;
				get_time(log.trade_time);
				//将log临时变量储存的信息写入pli->pN->log[n]
				pli->pN->log[n] = log;
				pli->pN->log_num++;
			}
			else {
				//用for循环将记录的100信息向前推移一步
				for (int i = 1; i < 100; i++) {
					pli->pN->log[i - 1] = pli->pN->log[i];
				}
				printf("请输入储蓄员：");
				scanf("%s", log.saving_clerk);
				log.take_in = 0;
				log.pay_out = balance;
				get_time(log.trade_time);
				//将log临时变量储存的信息写入pli->pN->log[99]
				//n = 99;
				pli->pN->log_num = 99;
				pli->pN->log[99] = log;
			}
		}
		else {
			printf("密码错误！\n");
		}
	}
	else {
		printf("账号不存在！\n");
	}
}

void get_time(char trade_time[50])
{
	time_t timer;	//time_t就是long int 类型
	struct tm *tblock;
	timer = time(NULL);
	tblock = localtime(&timer);
	strcpy(trade_time, asctime(tblock));
}