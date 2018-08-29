// bank_perfect_unstable.cpp : 定义控制台应用程序的入口点。
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <time.h>
#include <stddef.h>

const int max_log = 100;

typedef struct _log
{
	char saving_clerk[20];
	//用于保存交易时间
	char trade_time[14];
	char trade_site[20];
	float take_in;
	float pay_out;
	float balance;
} Log;

//节点,既是一个账号
typedef struct _node
{
	float balance;
	char bank[20];
	char acc_date[14];
	char home[20];
	char phonenum[20];
	char name[20];
	char bankNo[20];
	char id[20];
	char passwd[7];
	//用于记录交易信息已保存的数量,只会保存最近120条记录
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
//void get_time(char trade_time[50]);
//把从文件中读取到的数据添加到链表
void add_node(pList pli, Node node);
void cal(pList pli);

int main()
{
	int sel;
	int is_break = 0;
	FILE *fp;
	Node account;
	pNode pnode;
	List list = {
		NULL,
		NULL,
		NULL,
	};
	//
	//从文件读取数据
	if ((fp = fopen("bankcard_deal.dat", "rb+")) != NULL)
	{
		while (fread(&account, sizeof(Node), 1, fp) != 0)
		{
			add_node(&list, account);
		}
		//先关闭该文件，以后重新创建
		fclose(fp);
	}
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
			cal(&list);
			break;
		case 8:
			is_break = 1;
		default:
			is_break = 1;
		}
	}
	if ((fp = fopen("bankcard_deal.dat", "wb+")) != NULL)
	{
		for (pnode = list.head; pnode != NULL; pnode = pnode->next)
		{
			fwrite(pnode, sizeof(Node), 1, fp);
		}
	}
	fclose(fp);
	return 0;
}

void creatNode(pList pli)
{
	//判断账号是否已经存在
	int is_exist = 0;
	pNode pnode;
	pnode = (pNode)malloc(sizeof(Node));
	Log log;
	printf("\n===================================================请输入相关信息===================================================\n");
	printf("请输入开户行：");
	scanf("%s", pnode->bank);
	printf("请输入账号：");
	scanf("%s", pnode->bankNo);
	//寻找账号是否存在
	is_exist = search(pli, *pnode);
	//如果账号不存在
	if (is_exist == 0)
	{
		printf("请输入姓名：");
		scanf("%s", pnode->name);
		printf("请输入身份证号：");
		scanf("%s", pnode->id);
		printf("请输入开户日期(例1998-8-8)：");
		scanf("%s", pnode->acc_date);
		printf("请输入联系地址：");
		scanf("%s", pnode->home);
		printf("请输入交易地址：");
		scanf("%s", log.trade_site);
		printf("请输入储蓄员：");
		scanf("%s", log.saving_clerk);
		printf("请输入手机号：");
		scanf("%s", pnode->phonenum);
		printf("请输入余额：");
		scanf("%f", &(pnode->balance));
		printf("请输入密码：");
		scanf("%s", pnode->passwd);
		printf("--------------------------------------------------------------------------------------------------------------------\n");
		system("cls");
		//创建pnode时，将log_num置零
		pnode->log_num = 0;
		log.pay_out = 0;
		log.take_in = pnode->balance;
		strcpy(log.trade_time, pnode->acc_date);
		log.balance = pnode->balance;
		pnode->log[pnode->log_num] = log;
		pnode->log_num++;
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
	else
	{
		printf("--------------------------------------------------------------------------------------------------------------------\n");
		printf("账号已存在！\n");
		printf("--------------------------------------------------------------------------------------------------------------------\n");
	}
}

int display()
{
	int sel;
	printf("\n=====================================================请选择服务=====================================================\n");
	printf("1.创建账号\n");
	printf("2.查询信息\n");
	printf("3.修改账号\n");
	printf("4.删除账号\n");
	printf("5.存款\n");
	printf("6.取款\n");
	printf("7.统计信息\n");
	printf("8.退出\n");
	printf("--------------------------------------------------------------------------------------------------------------------\n");
	printf("请选择服务:");
	scanf("%d", &sel);
	printf("--------------------------------------------------------------------------------------------------------------------\n");
	system("cls");
	return sel;
}

int search(pList pli, Node node)
{
	int is_exist = 0;
	if (pli->head != NULL)
	{
		for (pNode pnode = pli->head; pnode != NULL; pnode = pnode->next)
		{
			if (strcmp(node.bank, pnode->bank) == 0)
			{
				if (strcmp(node.bankNo, pnode->bankNo) == 0)
				{
					pli->pN = pnode;
					is_exist = 1;
					break;
				}
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
	printf("\n====================================================请选择查询内容====================================================\n");
	printf("1.查询账号信息\n");
	printf("2.查询交易信息\n");
	printf("--------------------------------------------------------------------------------------------------------------------\n");
	printf("请输入查询内容：");
	scanf("%d", &search_style);
	printf("--------------------------------------------------------------------------------------------------------------------\n");
	system("cls");
	switch (search_style)
	{
	case 1:
		printf("\n====================================================请选择查询方式====================================================\n");
		printf("1.按账号查询\n");
		printf("2.按身份证号查询\n");
		printf("3.按姓名查询\n");
		printf("4.查询全部账号\n");
		printf("--------------------------------------------------------------------------------------------------------------------\n");
		printf("请输入查询方式：");
		scanf("%d", &sel);
		printf("--------------------------------------------------------------------------------------------------------------------\n");
		switch (sel)
		{
		case 1:
			printf("请输入账号：");
			scanf("%s", node.bankNo);
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			system("cls");
			printf("\n====================================================账号信息如下====================================================\n");
			printf("%-12s%-12s%-20s%-20s%-12s%-12s%-12s%-8s\n", "开户行", "开户时间", "账号", "身份证号", "姓名", "手机", "联系地址", "余额");
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			if (pli->head != NULL)
			{
				for (pnode = pli->head; pnode != NULL; pnode = pnode->next)
				{
					if (strcmp(node.bankNo, pnode->bankNo) == 0)
					{
						printf("%-12s", pnode->bank);
						printf("%-12s", pnode->acc_date);
						printf("%-20s", pnode->bankNo);
						printf("%-20s", pnode->id);
						printf("%-12s", pnode->name);
						printf("%-12s", pnode->phonenum);
						printf("%-12s", pnode->home);
						printf("%-8.2f\n", pnode->balance);
					}
				}
			}
			break;
		case 2:
			printf("请输入身份证号：");
			scanf("%s", node.id);
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			system("cls");
			printf("\n====================================================账号信息如下====================================================\n");
			printf("%-12s%-12s%-20s%-20s%-12s%-12s%-12s%-8s\n", "开户行", "开户时间", "账号", "身份证号", "姓名", "手机", "联系地址", "余额");
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			if (pli->head != NULL)
			{
				for (pnode = pli->head; pnode != NULL; pnode = pnode->next)
				{
					if (strcmp(node.id, pnode->id) == 0)
					{
						printf("%-12s", pnode->bank);
						printf("%-12s", pnode->acc_date);
						printf("%-20s", pnode->bankNo);
						printf("%-20s", pnode->id);
						printf("%-12s", pnode->name);
						printf("%-12s", pnode->phonenum);
						printf("%-12s", pnode->home);
						printf("%-8.2f\n", pnode->balance);
					}
				}
			}
			break;
		case 3:
			printf("请输入姓名：");
			scanf("%s", node.name);
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			system("cls");
			printf("\n====================================================账号信息如下====================================================\n");
			printf("%-12s%-12s%-20s%-20s%-12s%-12s%-12s%-8s\n", "开户行", "开户时间", "账号", "身份证号", "姓名", "手机", "联系地址", "余额");
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			if (pli->head != NULL)
			{
				for (pnode = pli->head; pnode != NULL; pnode = pnode->next)
				{
					if (strcmp(node.name, pnode->name) == 0)
					{
						printf("%-12s", pnode->bank);
						printf("%-12s", pnode->acc_date);
						printf("%-20s", pnode->bankNo);
						printf("%-20s", pnode->id);
						printf("%-12s", pnode->name);
						printf("%-12s", pnode->phonenum);
						printf("%-12s", pnode->home);
						printf("%-8.2f\n", pnode->balance);
					}
				}
			}
			break;
		case 4:
		default:
			system("cls");
			printf("\n====================================================账号信息如下====================================================\n");
			printf("%-12s%-12s%-20s%-20s%-12s%-12s%-12s%-8s\n", "开户行", "开户时间", "账号", "身份证号", "姓名", "手机", "联系地址", "余额");
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			if (pli->head != NULL)
			{
				for (pnode = pli->head; pnode != NULL; pnode = pnode->next)
				{
					printf("%-12s", pnode->bank);
					printf("%-12s", pnode->acc_date);
					printf("%-20s", pnode->bankNo);
					printf("%-20s", pnode->id);
					printf("%-12s", pnode->name);
					printf("%-12s", pnode->phonenum);
					printf("%-12s", pnode->home);
					printf("%-8.2f\n", pnode->balance);
				}
			}
		}
		printf("--------------------------------------------------------------------------------------------------------------------\n");
		system("pause");
		system("cls");
		break;
	case 2:
		printf("\n===================================================请选择查询方式===================================================\n");
		printf("1.按账号查询\n");
		printf("2.储蓄员\n");
		printf("3.全部\n");
		printf("--------------------------------------------------------------------------------------------------------------------\n");
		printf("请输入查询方式：");
		scanf("%d", &sel);
		printf("--------------------------------------------------------------------------------------------------------------------\n");
		switch (sel)
		{
		case 1:
			printf("请输入账号：");
			scanf("%s", node.bankNo);
			system("cls");
			printf("\n====================================================交易信息如下====================================================\n");
			printf("%-12s%-20s%-20s%-8s%-8s%-8s%-8s%-8s%-12s%-12s\n", "开户行", "账号", "身份证号", "姓名", "储蓄员", "存款", "取款", "余额", "交易时间", "交易地点");
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			if (pli->head != NULL)
			{
				//找到符合条件的节点的节点，可能不止一个(虽然现在只能有一个，设定账号唯一)
				for (pnode = pli->head; pnode != NULL; pnode = pnode->next)
				{
					if (strcmp(node.bankNo, pnode->bankNo) == 0)
					{
						//取得pnode里面log信息数量
						n = pnode->log_num;
						//取出pnode里面的信息
						for (int i = 0; i < n; i++)
						{
							log = pnode->log[i];
							printf("%-12s", pnode->bank);
							printf("%-20s", pnode->bankNo);
							printf("%-20s", pnode->id);
							printf("%-12s", pnode->name);
							printf("%-12s", log.saving_clerk);
							printf("%-8.2f", log.take_in);
							printf("%-8.2f", log.pay_out);
							printf("%-8.2f", log.balance);
							printf("%-12s", log.trade_time);
							printf("%-12s\n", log.trade_site);
						}
					}
				}
			}
			break;
		case 2:
			printf("请输入储蓄员：");
			scanf("%s", log_search.saving_clerk);
			system("cls");
			printf("\n====================================================交易信息如下====================================================\n");
			printf("%-12s%-20s%-20s%-8s%-8s%-8s%-8s%-8s%-12s%-12s\n", "开户行", "账号", "身份证号", "姓名", "储蓄员", "存款", "取款", "余额", "交易时间", "交易地点");
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			if (pli->head != NULL)
			{
				//找到符合条件的节点的节点，可能不止一个(虽然现在只能有一个，设定账号唯一)
				for (pnode = pli->head; pnode != NULL; pnode = pnode->next)
				{
					//取得pnode里面log信息数量
					n = pnode->log_num;
					//取出每个pnode里面的所有交易信息
					for (int i = 0; i < n; i++)
					{
						if (strcmp(log_search.saving_clerk, log.saving_clerk) == 0)
						{
							log = pnode->log[i];
							printf("%-12s", pnode->bank);
							printf("%-20s", pnode->bankNo);
							printf("%-20s", pnode->id);
							printf("%-8s", pnode->name);
							printf("%-8s", log.saving_clerk);
							printf("%-8.2f", log.take_in);
							printf("%-8.2f", log.pay_out);
							printf("%-8.2f", log.balance);
							printf("%-12s", log.trade_time);
							printf("%-12s\n", log.trade_site);
						}
					}
				}
			}
			break;
		default:
			system("cls");
			printf("\n====================================================交易信息如下====================================================\n");
			printf("%-12s%-20s%-20s%-8s%-8s%-8s%-8s%-8s%-12s%-12s\n", "开户行", "账号", "身份证号", "姓名", "储蓄员", "存款", "取款", "余额", "交易时间", "交易地点");
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			if (pli->head != NULL)
			{
				//找到符合条件的节点的节点，可能不止一个(虽然现在只能有一个，设定账号唯一)
				for (pnode = pli->head; pnode != NULL; pnode = pnode->next)
				{
					//取得pnode里面log信息数量
					n = pnode->log_num;
					//取出pnode里面的信息
					for (int i = 0; i < n; i++)
					{
						log = pnode->log[i];
						printf("%-12s", pnode->bank);
						printf("%-20s", pnode->bankNo);
						printf("%-20s", pnode->id);
						printf("%-8s", pnode->name);
						printf("%-8s", log.saving_clerk);
						printf("%-8.2f", log.take_in);
						printf("%-8.2f", log.pay_out);
						printf("%-8.2f", log.balance);
						printf("%-12s", log.trade_time);
						printf("%-12s\n", log.trade_site);
					}
				}
			}
		}
		printf("--------------------------------------------------------------------------------------------------------------------\n");
		system("pause");
		system("cls");
	}
}

void updateNode(pList pli)
{
	int sel = 1;
	//float balance;
	//char name[12];
	Node node;
	printf("\n===================================================请输入相关信息===================================================\n");
	printf("请输入开户行：");
	scanf("%s", node.bank);
	printf("请输入账号：");
	scanf("%s", node.bankNo);
	printf("--------------------------------------------------------------------------------------------------------------------\n");
	//如果账号存在
	if (search(pli, node))
	{
		//输入密码才可以继续修改信息
		printf("--------------------------------------------------------------------------------------------------------------------\n");
		printf("请输入密码：");
		scanf("%s", node.passwd);
		printf("--------------------------------------------------------------------------------------------------------------------\n");
		system("cls");
		if (strcmp(node.passwd, pli->pN->passwd) == 0)
		{
			printf("\n=================================================请选择需要修改的数据=================================================\n");
			printf("1.姓名\n");
			printf("2.地址\n");
			printf("3.手机号\n");
			printf("4.密码\n");
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			printf("请选择需要修改的数据：");
			scanf("%d", &sel);
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			switch (sel)
			{
			case 1:
				printf("请输入新的名字:");
				scanf("%s", node.name);
				strcpy(pli->pN->name, node.name);
				printf("--------------------------------------------------------------------------------------------------------------------\n");
				printf("信息更新成功!\n");
				printf("--------------------------------------------------------------------------------------------------------------------\n");
				break;
			case 2:
				printf("请输入新地址:");
				scanf("%s", node.home);
				strcpy(pli->pN->home, node.home);
				printf("--------------------------------------------------------------------------------------------------------------------\n");
				printf("信息更新成功!\n");
				printf("--------------------------------------------------------------------------------------------------------------------\n");
				break;
			case 3:
				printf("请输入新手机号:");
				scanf("%s", node.phonenum);
				strcpy(pli->pN->phonenum, node.phonenum);
				printf("--------------------------------------------------------------------------------------------------------------------\n");
				printf("信息更新成功!\n");
				printf("--------------------------------------------------------------------------------------------------------------------\n");
				break;
			case 4:
				printf("请输入新密码:");
				scanf("%s", node.passwd);
				strcpy(pli->pN->passwd, node.passwd);
				printf("--------------------------------------------------------------------------------------------------------------------\n");
				printf("信息更新成功!\n");
				printf("--------------------------------------------------------------------------------------------------------------------\n");
				break;
			default:
				printf("您的输入有误！\n");
				printf("--------------------------------------------------------------------------------------------------------------------\n");
			}
			system("pause");
			system("cls");
		}
		else
		{
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			printf("密码错误!\n");
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			system("pause");
			system("cls");
		}
	}
	else
	{
		printf("--------------------------------------------------------------------------------------------------------------------\n");
		printf("账号不存在！\n");
		printf("--------------------------------------------------------------------------------------------------------------------\n");
		system("pause");
		system("cls");
	}
}

void delNode(pList pli)
{
	Node node;
	printf("\n=================================================请选择需要修改的数据=================================================\n");
	printf("请输入开户行：");
	scanf("%s", node.bank);
	printf("请输入要删除的账号：");
	scanf("%s", node.bankNo);
	printf("--------------------------------------------------------------------------------------------------------------------\n");
	//如果账号存在
	if (search(pli, node))
	{
		//如果pli->pN->next不为空
		if (pli->pN->next != NULL)
		{
			//如果pli->pN->next不为空而且pli->pN->pre存在
			if (pli->pN->pre != NULL)
			{
				pli->pN->pre->next = pli->pN->next;
				pli->pN->next->pre = pli->pN->pre;
				free(pli->pN);
				pli->pN = NULL;
			}
			//如果pli->pN->next不为空，但是pli->pN->pre不存在
			//就是删除链表head
			else
			{
				pli->pN->next->pre = NULL;
				//改变list->head
				pli->head = pli->pN->next;
				free(pli->pN);
				pli->pN = NULL;
			}
		}
		//如果pli->pN->pre不为空,pli->pN->next为空
		else if (pli->pN->pre != NULL)
		{
			pli->pN->pre->next = NULL;
			//改变list->tail
			pli->tail = pli->pN->pre;
			free(pli->pN);
			pli->pN = NULL;
		}
		//pli->pN->pre和pli->pN->next都为空
		else
		{
			free(pli->pN);
			pli->head = NULL;
			pli->tail = NULL;
			pli->pN = NULL;
		}
		printf("--------------------------------------------------------------------------------------------------------------------\n");
		printf("成功删除账号！\n");
		printf("--------------------------------------------------------------------------------------------------------------------\n");
		system("pause");
		system("cls");
	}
	else
	{
		printf("--------------------------------------------------------------------------------------------------------------------\n");
		printf("账号不存在！\n");
		printf("--------------------------------------------------------------------------------------------------------------------\n");
		system("pause");
		system("cls");
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
	printf("\n===================================================请输入相关信息===================================================\n");
	printf("请输入开户行：");
	scanf("%s", node.bank);
	printf("请输入账号：");
	scanf("%s", node.bankNo);
	printf("--------------------------------------------------------------------------------------------------------------------\n");
	if (search(pli, node))
	{
		printf("请输入存款金额：");
		scanf("%f", &balance);
		pli->pN->balance += balance;
		//录入交易信息
		//如果交易信息数量<100
		n = pli->pN->log_num;
		if (n < max_log)
		{
			printf("请输入储蓄员：");
			scanf("%s", log.saving_clerk);
			log.take_in = balance;
			log.pay_out = 0;
			printf("请输入交易时间(例1998-08-08):");
			scanf("%s", log.trade_time);
			printf("请输入交易地点:");
			scanf("%s", log.trade_site);
			log.balance = pli->pN->balance;
			//get_time(log.trade_time);
			//将log临时变量储存的信息写入pli->pN->log[n]
			pli->pN->log[n] = log;
			pli->pN->log_num++;
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			printf("取款成功!");
			system("pause");
			system("cls");
		}
		else
		{
			//用for循环将记录的100信息向前推移一步
			for (int i = 1; i < max_log; i++)
			{
				pli->pN->log[i - 1] = pli->pN->log[i];
			}
			printf("请输入储蓄员：");
			scanf("%s", log.saving_clerk);
			log.take_in = balance;
			log.pay_out = 0;
			printf("请输入交易时间(例1998-08-08):");
			scanf("%s", log.trade_time);
			printf("请输入交易时间：");
			scanf("%s", log.trade_site);
			log.balance = pli->pN->balance;
			//get_time(log.trade_time);
			//将log临时变量储存的信息写入pli->pN->log[99]
			//n=99
			//n = 99;
			pli->pN->log_num = max_log - 1;
			pli->pN->log[max_log - 1] = log;
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			system("cls");
		}
	}
	else
	{
		printf("--------------------------------------------------------------------------------------------------------------------\n");
		printf("账号不存在！");
		printf("--------------------------------------------------------------------------------------------------------------------\n");
		system("pause");
		system("cls");
	}
}

void withdraw(pList pli)
{
	Node node;
	float balance = 0;
	int n;
	//用log临时变量储存得来的交易信息
	Log log;
	printf("\n===================================================请输入相关信息===================================================\n");
	printf("请输入开户行：");
	scanf("%s", node.bank);
	printf("请输入账号：");
	scanf("%s", node.bankNo);
	printf("--------------------------------------------------------------------------------------------------------------------\n");
	//账号存在则继续
	if (search(pli, node))
	{
		//密码正确继续
		printf("请输入密码：");
		scanf("%s", node.passwd);
		printf("--------------------------------------------------------------------------------------------------------------------\n");
		if (strcmp(node.passwd, pli->pN->passwd) == 0)
		{
			printf("请输入取款金额：");
			scanf("%f", &balance);
			//如果取款金额小于账号余额
			if (balance < pli->pN->balance)
			{
				pli->pN->balance -= balance;
				//录入交易信息
				//如果交易信息数量<100
				n = pli->pN->log_num;
				if (n < max_log)
				{
					printf("请输入储蓄员：");
					scanf("%s", log.saving_clerk);
					log.take_in = 0;
					log.pay_out = balance;
					printf("请输入交易时间（例1998-08-08）：");
					scanf("%s", log.trade_time);
					printf("请输入交易地点：");
					scanf("%s", log.trade_site);
					log.balance = pli->pN->balance;
					//get_time(log.trade_time);
					//将log临时变量储存的信息写入pli->pN->log[n]
					pli->pN->log[n] = log;
					pli->pN->log_num++;
					printf("--------------------------------------------------------------------------------------------------------------------\n");
					system("cls");
				}
				else
				{
					//用for循环将记录的100信息向前推移一步
					for (int i = 1; i < max_log; i++)
					{
						pli->pN->log[i - 1] = pli->pN->log[i];
					}
					printf("请输入储蓄员：");
					scanf("%s", log.saving_clerk);
					log.take_in = 0;
					log.pay_out = balance;
					printf("请输入交易时间(例1198-08-08)：");
					scanf("%s", log.trade_time);
					printf("请输入交易交易地点：");
					scanf("%s", log.trade_site);
					log.balance = pli->pN->balance;
					//get_time(log.trade_time);
					//将log临时变量储存的信息写入pli->pN->log[99]
					//n = 99;
					pli->pN->log_num = max_log - 1;
					pli->pN->log[max_log - 1] = log;
					printf("--------------------------------------------------------------------------------------------------------------------\n");
					printf("取款成功！");
					system("pause");
					system("cls");
				}
			}
			else
			{
				printf("--------------------------------------------------------------------------------------------------------------------\n");
				printf("余额不足！\n");
				printf("--------------------------------------------------------------------------------------------------------------------\n");
				system("pause");
				system("cls");
			}
		}
		else
		{
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			printf("密码错误！\n");
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			system("pause");
			system("cls");
		}
	}
	else
	{
		printf("--------------------------------------------------------------------------------------------------------------------\n");
		printf("账号不存在！\n");
		printf("--------------------------------------------------------------------------------------------------------------------\n");
		system("pause");
		system("cls");
	}
}

/*void get_time(char trade_time[50])
{
	time_t timer;	//time_t就是long int 类型
	struct tm *tblock;
	timer = time(NULL);
	tblock = localtime(&timer);
	strcpy(trade_time, asctime(tblock));
}*/

void add_node(pList pli, Node node)
{
	//判断账号是否已经存在
	pNode pnode;
	pnode = (pNode)malloc(sizeof(Node));
	*pnode = node;
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

void cal(pList pli)
{
	pNode pnode = NULL;
	Node node;
	//默认全部查询
	int sel = 0;
	int search_style = 0;
	//n既是log信息数量
	int n;
	float total_balance = 0;
	float total_pay = 0;
	float total_takein = 0;
	//用log临时变量储存得来的交易信息
	Log log;
	//用于储存输入信息
	Log log_search;
	printf("\n===================================================请选择统计内容===================================================\n");
	printf("1.统计账号信息\n");
	printf("2.统计交易信息\n");
	printf("--------------------------------------------------------------------------------------------------------------------\n");
	printf("请输入统计内容：");
	scanf("%d", &search_style);
	printf("--------------------------------------------------------------------------------------------------------------------\n");
	system("cls");
	switch (search_style)
	{
	case 1:
		printf("\n===================================================请选择统计内容===================================================\n");
		printf("2.按账号统计\n");
		printf("3.按身份证号统计\n");
		printf("4.按姓名统计\n");
		printf("5.统计全部\n");
		printf("--------------------------------------------------------------------------------------------------------------------\n");
		printf("请输入统计内容：");
		scanf("%d", &sel);
		printf("--------------------------------------------------------------------------------------------------------------------\n");
		switch (sel)
		{
		case 1:
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			printf("请输入银行：");
			scanf("%s", node.bank);
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			system("cls");
			printf("\n====================================================账号信息如下====================================================\n");
			printf("%-12s%-12s%-12s%-12s%-8s\n", "开户行", "账号", "身份证号", "姓名", "余额");
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			if (pli->head != NULL)
			{
				//按开户行统计
				for (pnode = pli->head; pnode != NULL; pnode = pnode->next)
				{
					if (strcmp(node.bank, pnode->bank) == 0)
					{
						printf("%-12s", pnode->bank);
						printf("%-12s", pnode->bankNo);
						printf("%-12s", pnode->id);
						printf("%-12s", pnode->name);
						printf("%-8.2f\n", pnode->balance);
						total_balance += pnode->balance;
					}
				}
				printf("--------------------------------------------------------------------------------------------------------------------\n");
				printf("%-8s\t\t\t\t\t%.2f\n", "总计", total_balance);
				printf("--------------------------------------------------------------------------------------------------------------------\n");
			}
			system("pause");
			system("cls");
			break;
		case 2:
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			printf("请输入账号：");
			scanf("%s", node.bankNo);
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			system("cls");
			printf("\n====================================================账号信息如下====================================================\n");
			printf("%-12s%-12s%-12s%-12s%-8s\n", "开户行", "账号", "身份证号", "姓名", "余额");
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			if (pli->head != NULL)
			{
				//按开户行统计
				for (pnode = pli->head; pnode != NULL; pnode = pnode->next)
				{
					if (strcmp(node.bankNo, pnode->bankNo) == 0)
					{
						printf("%-12s", pnode->bank);
						printf("%-12s", pnode->bankNo);
						printf("%-12s", pnode->id);
						printf("%-12s", pnode->name);
						printf("%-8.2f\n", pnode->balance);
						total_balance += pnode->balance;
					}
				}
				printf("--------------------------------------------------------------------------------------------------------------------\n");
				printf("%-8s\t\t\t\t\t%.2f\n", "总计", total_balance);
				printf("--------------------------------------------------------------------------------------------------------------------\n");
			}
			system("pause");
			system("cls");
			break;
		case 3:
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			printf("请输入身份证号：");
			scanf("%s", node.id);
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			system("cls");
			printf("\n====================================================账号信息如下====================================================\n");
			printf("%-12s%-12s%-12s%-12s%-8s\n", "开户行", "账号", "身份证号", "姓名", "余额");
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			if (pli->head != NULL)
			{
				//按开户行统计
				for (pnode = pli->head; pnode != NULL; pnode = pnode->next)
				{
					if (strcmp(node.id, pnode->id) == 0)
					{
						printf("%-12s", pnode->bank);
						printf("%-12s", pnode->bankNo);
						printf("%-12s", pnode->id);
						printf("%-12s", pnode->name);
						printf("%-8.2f\n", pnode->balance);
						total_balance += pnode->balance;
					}
				}
				printf("--------------------------------------------------------------------------------------------------------------------\n");
				printf("%-8s\t\t\t\t\t%.2f\n", "总计", total_balance);
				printf("--------------------------------------------------------------------------------------------------------------------\n");
			}
			system("pause");
			system("cls");
			break;
		case 4:
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			printf("请输入姓名：");
			scanf("%s", node.name);
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			system("cls");
			printf("\n====================================================账号信息如下====================================================\n");
			printf("%-12s%-12s%-12s%-12s%-8s\n", "开户行", "账号", "身份证号", "姓名", "余额");
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			if (pli->head != NULL)
			{
				//按开户行统计
				for (pnode = pli->head; pnode != NULL; pnode = pnode->next)
				{
					if (strcmp(node.name, pnode->name) == 0)
					{
						printf("%-12s", pnode->bank);
						printf("%-12s", pnode->bankNo);
						printf("%-12s", pnode->id);
						printf("%-12s", pnode->name);
						printf("%-8.2f\n", pnode->balance);
						total_balance += pnode->balance;
					}
				}
				printf("--------------------------------------------------------------------------------------------------------------------\n");
				printf("%-8s\t\t\t\t\t%.2f\n", "总计", total_balance);
				printf("--------------------------------------------------------------------------------------------------------------------\n");
			}
			system("pause");
			system("cls");
			break;
			//默认统计全部
		case 5:
		default:
			system("cls");
			printf("\n====================================================账号信息如下====================================================\n");
			printf("%-12s%-12s%-12s%-12s%-8s\n", "开户行", "账号", "身份证号", "姓名", "余额");
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			if (pli->head != NULL)
			{
				for (pnode = pli->head; pnode != NULL; pnode = pnode->next)
				{
					printf("%-12s", pnode->bank);
					printf("%-12s", pnode->bankNo);
					printf("%-12s", pnode->id);
					printf("%-12s", pnode->name);
					printf("%-8.2f\n", pnode->balance);
					total_balance += pnode->balance;
				}
				printf("--------------------------------------------------------------------------------------------------------------------\n");
				printf("%-8s\t\t\t\t\t%.2f\n", "总计", total_balance);
				printf("--------------------------------------------------------------------------------------------------------------------\n");
			}
			system("pause");
			system("cls");
			break;
		}
		break;
	case 2:
		printf("\n===================================================请选择统计方式===================================================\n");
		printf("1.按账号统计\n");
		printf("2.按交易时间统计\n");
		printf("3.按账号交易地点统计\n");
		printf("4.按储蓄员统计\n");
		printf("5.统计全部\n");
		printf("请输入统计方式(默认5)：");
		scanf("%d", &sel);
		switch (sel)
		{
		case 1:
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			printf("请输入账号：");
			scanf("%s", node.bankNo);
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			system("cls");
			printf("\n====================================================交易信息如下====================================================\n");
			printf("%-12s%-20s%-20s%-8s%-8s%-12s%-12s%-8s%-8s\n", "开户行", "账号", "身份证号", "姓名", "储蓄员", "交易时间", "交易地点", "存款", "取款");
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			if (pli->head != NULL)
			{
				//找到符合条件的节点的节点，可能不止一个
				for (pnode = pli->head; pnode != NULL; pnode = pnode->next)
				{
					if (strcmp(node.bankNo, pnode->bankNo) == 0)
					{
						//取得pnode里面log信息数量
						n = pnode->log_num;
						//取出pnode里面的信息
						for (int i = 0; i < n; i++)
						{
							log = pnode->log[i];
							printf("%-12s", pnode->bank);
							printf("%-20s", pnode->bankNo);
							printf("%-20s", pnode->id);
							printf("%-8s", pnode->name);
							printf("%-8s", log.saving_clerk);
							printf("%-12s", log.trade_time);
							printf("%-12s", log.trade_site);
							printf("%-8.2f", log.take_in);
							printf("%-8.2f", log.pay_out);
							printf("%-8.2f\n", pnode->balance);
							total_pay += log.pay_out;
							total_takein += log.take_in;
						}
					}
				}
				printf("--------------------------------------------------------------------------------------------------------------------\n");
				printf("%-92s%-8.2f%-8.2f\n", "总计", total_takein, total_pay);
				printf("--------------------------------------------------------------------------------------------------------------------\n");
			}
			break;
		case 2:
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			printf("请输入储蓄员：");
			scanf("%s", node.log->saving_clerk);
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			system("cls");
			printf("\n====================================================交易信息如下====================================================\n");
			printf("%-12s%-20s%-20s%-8s%-8s%-12s%-12s%-8s%-8s\n", "开户行", "账号", "身份证号", "姓名", "储蓄员", "交易时间", "交易地点", "存款", "取款");
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			if (pli->head != NULL)
			{
				//找到符合条件的节点的节点，可能不止一个
				for (pnode = pli->head; pnode != NULL; pnode = pnode->next)
				{
					if (strcmp(node.log->saving_clerk, pnode->log->saving_clerk) == 0)
					{
						//取得pnode里面log信息数量
						n = pnode->log_num;
						//取出pnode里面的信息
						for (int i = 0; i < n; i++)
						{
							log = pnode->log[i];
							printf("%-12s", pnode->bank);
							printf("%-20s", pnode->bankNo);
							printf("%-20s", pnode->id);
							printf("%-8s", pnode->name);
							printf("%-8s", log.saving_clerk);
							printf("%-12s", log.trade_time);
							printf("%-12s", log.trade_site);
							printf("%-8.2f", log.take_in);
							printf("%-8.2f", log.pay_out);
							printf("%-8.2f\n", pnode->balance);
							total_pay += log.pay_out;
							total_takein += log.take_in;
						}
					}
				}
				printf("--------------------------------------------------------------------------------------------------------------------\n");
				printf("%-92s%-8.2f%-8.2f\n", "总计", total_takein, total_pay);
				printf("--------------------------------------------------------------------------------------------------------------------\n");
			}
			break;
		case 3:
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			printf("请输入交易地点：");
			scanf("%s", node.log->trade_site);
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			system("cls");
			printf("\n====================================================交易信息如下====================================================\n");
			printf("%-12s%-20s%-20s%-8s%-8s%-12s%-12s%-8s%-8s\n", "开户行", "账号", "身份证号", "姓名", "储蓄员", "交易时间", "交易地点", "存款", "取款");
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			if (pli->head != NULL)
			{
				//找到符合条件的节点的节点，可能不止一个
				for (pnode = pli->head; pnode != NULL; pnode = pnode->next)
				{
					if (strcmp(node.log->trade_site, pnode->log->trade_site) == 0)
					{
						//取得pnode里面log信息数量
						n = pnode->log_num;
						//取出pnode里面的信息
						for (int i = 0; i < n; i++)
						{
							log = pnode->log[i];
							printf("%-12s", pnode->bank);
							printf("%-20s", pnode->bankNo);
							printf("%-20s", pnode->id);
							printf("%-8s", pnode->name);
							printf("%-8s", log.saving_clerk);
							printf("%-12s", log.trade_time);
							printf("%-12s", log.trade_site);
							printf("%-8.2f", log.take_in);
							printf("%-8.2f", log.pay_out);
							printf("%-8.2f\n", pnode->balance);
							total_pay += log.pay_out;
							total_takein += log.take_in;
						}
					}
				}
				printf("--------------------------------------------------------------------------------------------------------------------\n");
				printf("%-92s%-8.2f%-8.2f\n", "总计", total_takein, total_pay);
				printf("--------------------------------------------------------------------------------------------------------------------\n");
			}
			break;
		case 4:
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			printf("请输入交易时间：");
			scanf("%s", node.log->trade_time);
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			system("cls");
			printf("\n====================================================交易信息如下====================================================\n");
			printf("%-12s%-20s%-20s%-8s%-8s%-12s%-12s%-8s%-8s\n", "开户行", "账号", "身份证号", "姓名", "储蓄员", "交易时间", "交易地点", "存款", "取款");
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			if (pli->head != NULL)
			{
				//找到符合条件的节点的节点，可能不止一个
				for (pnode = pli->head; pnode != NULL; pnode = pnode->next)
				{
					if (strcmp(node.log->trade_time, pnode->log->saving_clerk) == 0)
					{
						//取得pnode里面log信息数量
						n = pnode->log_num;
						//取出pnode里面的信息
						for (int i = 0; i < n; i++)
						{
							log = pnode->log[i];
							printf("%-12s", pnode->bank);
							printf("%-20s", pnode->bankNo);
							printf("%-20s", pnode->id);
							printf("%-8s", pnode->name);
							printf("%-8s", log.saving_clerk);
							printf("%-12s", log.trade_time);
							printf("%-12s", log.trade_site);
							printf("%-8.2f", log.take_in);
							printf("%-8.2f", log.pay_out);
							printf("%-8.2f\n", pnode->balance);
							total_pay += log.pay_out;
							total_takein += log.take_in;
						}
					}
				}
				printf("--------------------------------------------------------------------------------------------------------------------\n");
				printf("%-92s%-8.2f%-8.2f\n", "总计", total_takein, total_pay);
				printf("--------------------------------------------------------------------------------------------------------------------\n");
			}
			break;
		case 5:
		default:
			printf("\n====================================================交易信息如下====================================================\n");
			printf("%-12s%-20s%-20s%-8s%-8s%-12s%-12s%-8s%-8s%\n", "开户行", "账号", "身份证号", "姓名", "储蓄员", "交易时间", "交易地点", "存款", "取款");
			printf("--------------------------------------------------------------------------------------------------------------------\n");
			if (pli->head != NULL)
			{
				//找到符合条件的节点的节点，可能不止一个
				for (pnode = pli->head; pnode != NULL; pnode = pnode->next)
				{
					//取得pnode里面log信息数量
					n = pnode->log_num;
					//取出pnode里面的信息
					for (int i = 0; i < n; i++)
					{
						log = pnode->log[i];
						printf("%-12s", pnode->bank);
						printf("%-20s", pnode->bankNo);
						printf("%-20s", pnode->id);
						printf("%-8s", pnode->name);
						printf("%-8s", log.saving_clerk);
						printf("%-12s", log.trade_time);
						printf("%-12s", log.trade_site);
						printf("%-8.2f", log.take_in);
						printf("%-8.2f", log.pay_out);
						printf("%-8.2f\n", pnode->balance);
						total_pay += log.pay_out;
						total_takein += log.take_in;
					}
				}
				printf("--------------------------------------------------------------------------------------------------------------------\n");
				printf("%-92s%-8.2f%-8.2f\n", "总计", total_takein, total_pay);
				printf("--------------------------------------------------------------------------------------------------------------------\n");
			}
			break;
		}
		system("pause");
		system("cls");
	}
}
