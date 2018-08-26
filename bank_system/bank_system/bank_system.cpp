#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct remark {
	char saving_clerk[20];
	float pay;
	float income;
	char date[20];
};

struct account {
	char acc_no[20];
	char name[20];
	float balance;
	char id[20];
	char phonenum[20];
	char passwd[6];
	//char saving_clerk[20];
	char home_site[100];
	char de_bank[20];
	char de_time[20];
	int rcd;
	struct remark target[100];
};

int display();
void addAcc();         //添加一个账户
void deposit();             //存款
void withdraw();
void accUpdate();
void search(int * n, struct account * buffe);     //查找账号,返回查找到的地址
void showAccount();     //显示所有账号的信息
void showInfo();
void count();

int main() {
	int sev;
	int is_exit = 0;
	while (is_exit == 0) {

		/*    printf("请选择服务:\n", );
			scanf("%d",&sev);*/
		sev = display();
		switch (sev) {
		case 1:
			addAcc();
			break;
		case 2:
			deposit();
			break;
		case 3:
			withdraw();
			break;
		case 4:
			accUpdate();
			break;
		case 5:
			count();
			break;
		case 6:
			showAccount();
			break;
		case 7:
			is_exit = 1;
		}
	}
	return 0;
}

/*用户主菜单*/
int display() {
	int ser;
	printf("\n====================WELCOME====================\n");
	printf("1:创建一个账号\n");
	printf("2:存款\n");
	printf("3:取款\n");
	printf("4:修改账号信息\n");
	printf("5:统计\n");
	printf("6:显示账号信息\n");
	printf("7:退出\n\n");
	printf("请选择服务:");
	scanf("%d", &ser);
	return ser;
}

//添加一个账号
void addAcc() {
	FILE * fp;
	//判断账号是否合法
	int is_leg = 1;
	//是否跳出
	int is_lev = 1;
	char accNo[20];
	char bank[20];
	//定义缓冲
	struct account buff =
	{
		"acc_no",
		"name",
		0.0,
		"id",
		"phontnum",
		"passw",
		//"saving_clerk",
		"home_site",
		"de_bank",
		"de_time",
	};
	if ((fp = fopen("bankcard_base.dat", "ab+")) == NULL) {
		printf("文件未能打开\n");
		exit(0);
	}
	while (is_lev) {
		printf("\n=======请输入以下信息========\n");
		printf("请输入开户行：");
		scanf("%s", bank);
		printf("请输入账号：");
		scanf("%s", accNo);
		rewind(fp);
		//遍历所有元素，查看账号是否与已知账号相同
		while ((fread(&buff, sizeof(struct account), 1, fp)) != (int)NULL) {
			if (strcmp(buff.de_bank, bank) == 0) {
				//开户银行相同的前提下，账号相同
				if (strcmp(buff.acc_no, accNo) == 0) {
					printf("账号已存在,请重新输入\n");
					scanf("%d", &accNo);
					is_leg = 0;
					break;
				}

			}
		}
		//账号判断合法后
		if (is_leg) {
			strcpy(buff.acc_no, accNo);
			strcpy(buff.de_bank, bank);
			printf("无相同账号，请输入名字:");
			scanf("%s", buff.name);
			printf("请输入身份证号：");
			scanf("%s", buff.id);
			printf("请输入手机号：");
			scanf("%s", buff.phonenum);
			printf("请输入余额:");
			scanf("%f", &buff.balance);
			printf("请输入密码(6位)：");
			scanf("%6s", buff.passwd);
			/*printf("请输入储蓄员：");
			scanf("%s", buff.saving_clerk);*/
			printf("请输入开户时间：(按照1988-8-8格式)");
			scanf("%s", buff.de_time);
			printf("请输入联系地址：");
			scanf("%s", buff.home_site);
			buff.rcd = 0;
			//定位到文件末尾
			fseek(fp, 0, SEEK_END);
			//文件末尾追加信息
			fwrite(&buff, sizeof(struct account), 1, fp);
			printf("账号创建成功\n");
		}
		printf("============是否继续创建账号？==============\n");
		printf("1.是\n");
		printf("0.否\n");
		printf("请选择服务：");
		scanf("%d", &is_lev);
	}
	fclose(fp);
}

//取款
void withdraw() {

	//int accNo;
	int n = 0;
	FILE * fp;
	int draw_balance = 0;
	int err_times = 0;
	char passwd[20];
	struct remark record = {
		"clerk",
			0,
			0,
			"date",
	};
	//输入账号
	struct account buff = {
		"acc_no",
		"name",
		0.0,
		"id",
		"phonenum",
		"passw"
	};
	if ((fp = fopen("bankcard_base.dat", "rb+")) == NULL) {
		printf("文件未能打开\n");
	}
	/*printf("请输入账号：");
	scanf("%d", &accNo);
	//定位指针到文件首
	rewind(fp);
	//遍历所有元素，直到所要查找的那个账号
	while ((fread(&buff, sizeof(struct account), 1, fp))!= (int)NULL) {
		if (buff.acc_no == accNo) {
			//定位指针到当前位置
			fseek(fp, i * sizeof(struct account), SEEK_SET);
			//fseek(fp,0,SEEK_CUR)
			printf("您现有余额：%.2f\n", buff.balance);
			while (1) {
				printf("请输入取款金额：");
				scanf("%d", &draw_balance);
				if (draw_balance <= buff.balance) {
					break;
				}
				printf("您没有这么多存款\n");
			}
			buff.balance -= draw_balance;
			//写入账号数据
			fwrite(&buff, sizeof(struct account), 1, fp);
			printf("您取款后余额：%.2f", buff.balance);
			break;
		}
	}*/
	search(&n, &buff);
	fseek(fp, n * sizeof(struct account), SEEK_SET);
	//fseek(fp,0,SEEK_CUR)
	while (1) {
		printf("请输入密码：");
		scanf("%s", passwd);
		if (strcmp(passwd, buff.passwd) == 0) {
			break;
		}
		else {
			if (err_times < 2) {
				printf("第%d密码输入错误，您一共有三次机会，", err_times + 1);
			}
			else {
				exit(0);
			}
		}
		err_times++;
	}
	printf("您现有余额：%.2f\n", buff.balance);
	while (1) {
		printf("请输入取款金额：");
		scanf("%d", &draw_balance);
		if (draw_balance <= buff.balance) {
			break;
		}
		printf("您没有这么多存款\n");
	}
	buff.balance -= draw_balance;
	if (buff.rcd < 100) {
		printf("请输入储蓄员：");
		scanf("%s", record.saving_clerk);
		//(buff.target)[buff.rcd]->income = depo_balance;
		record.income = 0;
		record.pay = draw_balance;
		printf("请输入日期（按照1998-8-8格式）：");
		scanf("%s", record.date);
		buff.target[buff.rcd] = record;
		buff.rcd++;
	}
	else {
		for (int i = 1; i < 100; i++) {
			buff.target[i - 1] = buff.target[i];
		}
		printf("请输入储蓄员：");
		scanf("%s", record.saving_clerk);
		//(buff.target)[buff.rcd]->income = depo_balance;
		record.income = 0;
		record.pay = draw_balance;
		printf("请输入日期（按照1998-8-8格式）：");
		scanf("%s", record.date);
		buff.target[buff.rcd] = record;
	}
	//写入账号数据
	fwrite(&buff, sizeof(struct account), 1, fp);
	printf("您取款后余额：%.2f", buff.balance);
	fclose(fp);
}

//存款
void deposit() {
	//int accNo;
	int n = 0;
	FILE * fp;
	float depo_balance = 0;
	//输入账号
	struct account buff = {
		"acc_no",
			"name",
			0,
			"id",
			"phonenum",
			"pass",
	};
	struct remark record = {
		"clerk",
			0,
			0,
			"date",
	};
	if ((fp = fopen("bankcard_base.dat", "rb+")) == NULL) {
		printf("文件未能打开\n");
	}
	/*printf("请输入账号：");
	scanf("%d", &accNo);
	//定位指针到文件首
	rewind(fp);*/
	//遍历所有元素，直到所要查找的那个账号
	/*while ((fread(&buff, sizeof(struct account), 1, fp))!=(int)NULL) {
		if (buff.acc_no == accNo) {
			//定位指针到当前位置
			fseek(fp, i * sizeof(struct account), SEEK_SET);
			//fseek(fp, 0, SEEK_CUR);
			printf("您现有余额为:%.2f\n", buff.balance);
			printf("请输入存款金额：");
			scanf("%f", &depo_balance);
			buff.balance += depo_balance;
			//写入账号数据
			fwrite(&buff, sizeof(struct account), 1, fp);
			printf("您的现有余额为:%.2f\n", buff.balance);
			printf("存款成功！\n");
			break;
		}
		i++;
	}*/

	//查找账号,并得到返回的地址和账号
	search(&n, &buff);
	fseek(fp, n * sizeof(struct account), SEEK_SET);
	//fseek(fp, 0, SEEK_CUR);
	printf("您现有余额为:%.2f\n", buff.balance);
	printf("请输入存款金额：");
	scanf("%f", &depo_balance);
	buff.balance += depo_balance;
	//写入账号数据

	//交易记录
	if (buff.rcd < 100) {
		printf("请输入储蓄员：");
		scanf("%s", record.saving_clerk);
		//(buff.target)[buff.rcd]->income = depo_balance;
		record.income = depo_balance;
		record.pay = 0;
		printf("请输入日期（按照1998-8-8格式）：");
		scanf("%s", record.date);
		buff.target[buff.rcd] = record;
		buff.rcd++;
	}
	else {
		for (int i = 1; i < 100; i++) {
			buff.target[i - 1] = buff.target[i];
		}
		printf("请输入储蓄员：");
		scanf("%s", record.saving_clerk);
		//(buff.target)[buff.rcd]->income = depo_balance;
		record.income = depo_balance;
		record.pay = 0;
		printf("请输入日期（按照1998-8-8格式）：");
		scanf("%s", record.date);
		buff.target[buff.rcd] = record;
	}
	fwrite(&buff, sizeof(struct account), 1, fp);
	printf("您的现有余额为:%.2f\n", buff.balance);
	printf("存款成功！\n");
	fclose(fp);
}

//修改个人账户信息或者删除信息
void accUpdate() {
	//删除账户还是修改账号信息
	int select = 1;
	char accNo[20];
	int n = 0;
	FILE * fp;
	FILE * fpt;
	int rev = 1;	//用户所要修改的信息
	rename("bankcard_base.dat", "tmp.dat");
	struct account buff = {
		"acno",
			"name",
			0,
	};

	printf("1.修改个人账户信息\n");
	printf("2.删除个人账户信息\n");
	printf("请选择服务:");
	scanf("%d", &select);

	switch (select) {
	case 1:
		if ((fp = fopen("bankcard_base.dat", "rb+")) == NULL) {
			printf("文件未能打开\n");
			exit(0);
		}
		//输入账号
		/*printf("请输入账号：");
		scanf("%d", &accNo);
		//定位指针到文件首
		rewind(fp);
		//遍历所有元素，直到所要查找的那个账号
		while ((fread(&buff, sizeof(struct account), 1, fp))) {
			if (buff.acc_no == accNo) {
				//定位指针到当前位置
				fseek(fp, i * sizeof(struct account), SEEK_SET);
				//fseek(fp,0,SEEK_CUR)
				printf("\n===============选择你想要修改的内容============\n");
				printf("1.名字\n");
				printf("2.余额\n");
				printf("3.手机号\n");
				printf("4.联系地址\n");
				printf("5.密码\n");
				printf("请选择服务");
				scanf("%d", &rev);
				switch (rev) {
				case 1:
					printf("请输入新名：");
					scanf("%s", buff.name);
					break;
				case 2:
					printf("请输入余额：");
					scanf("%f", &buff.balance);
					break;
				case 3:
					printf("请输入新号码：");
					scanf("%s", buff.phonenum);
					break;
				case 4:
					printf("请输入地址：");
					scanf("%s", buff.home_site);
					break;
				case 5:
					printf("请输入新密码：");
					scanf("%s", &buff.passwd);
				}
				//写入账号数据
				fwrite(&buff, sizeof(struct account), 1, fp);
				printf("账号信息修改成功！");
				break;
			}
			i++;
		}*/
		//查找账号
		search(&n, &buff);
		fseek(fp, n * sizeof(struct account), SEEK_SET);
		//fseek(fp,0,SEEK_CUR)
		printf("\n===============选择你想要修改的内容============\n");
		printf("1.名字\n");
		printf("2.余额\n");
		printf("3.手机号\n");
		printf("4.联系地址\n");
		printf("5.密码\n");
		printf("请选择服务");
		scanf("%d", &rev);
		switch (rev) {
		case 1:
			printf("请输入新名：");
			scanf("%s", buff.name);
			break;
		case 2:
			printf("请输入余额：");
			scanf("%f", &buff.balance);
			break;
		case 3:
			printf("请输入新号码：");
			scanf("%s", buff.phonenum);
			break;
		case 4:
			printf("请输入地址：");
			scanf("%s", buff.home_site);
			break;
		case 5:
			printf("请输入新密码：");
			scanf("%s", &buff.passwd);
		}
		//写入账号数据
		fwrite(&buff, sizeof(struct account), 1, fp);
		printf("账号信息修改成功！");
		fclose(fp);
		break;
	case 2:
		if ((fp = fopen("bankcard_base.dat", "wb")) == NULL) {
			printf("文件未能打开\n");
			exit(0);
		}
		if ((fpt = fopen("tmp.dat", "rb")) == NULL) {
			printf("w文件未能打开\n");
			exit(0);
		}
		printf("请输入账号:");
		scanf("%s", accNo);
		while ((fread(&buff, sizeof(struct account), 1, fpt)) != (int)NULL) {
			//只要不是要删除的数据就把他写入文件
			if (strcmp(buff.acc_no, accNo)) {
				fwrite(&buff, sizeof(struct account), 1, fp);
			}
		}
		fclose(fp);
		fclose(fpt);
		remove("tmp.dat");
	}

}

//显示账号信息
void showAccount() {
	char accNo[20];
	char date[20];
	char name[20];
	char id[20];
	int search_style;
	FILE * fp;
	//输入账号
	struct account buff = {
		"acc_no",
			"name",
			0,
			"id",
			"phonenum",
			"passw",
			//"saving_clerk",
			"home_site",
			"de_bank",
			"de_time",
	};
	if ((fp = fopen("bankcard_base.dat", "rb")) == NULL) {
		printf("文件未能打开\n");
		exit(0);
	}
	printf("===========请选择查询方式：===========\n");
	printf("1.按账号查询\n");
	printf("2.按开户日期查询\n");
	printf("3.按姓名查询\n");
	printf("4.按身份证号查询\n");
	printf("5.全部\n");
	printf("请选择查询方式：");
	scanf("%d", &search_style);
	rewind(fp);
	switch (search_style) {
	case 1:
		printf("\n请输入账号：");
		scanf("%s", accNo);
		printf("\n==========所有账号信息=========\n");
		printf("%-8s\t%-8s\t%-8s\t%-8s\t%-8s\t\n", "银行", "账号", "名字", "身份证号", "余额");
		while ((fread(&buff, sizeof(struct account), 1, fp)) != (int)NULL) {
			if (strcmp(accNo, buff.acc_no) == 0) {

				//显示该账户基础信息
				printf("%-8s\t", buff.de_bank);
				printf("%-8s\t", buff.acc_no);
				printf("%-8s\t", buff.name);
				printf("%-8s\t", buff.id);
				printf("%-8.2f\t", buff.balance);
				printf("\n");


			}
		}
		break;
	case 2:
		printf("\n请输入开户日期：");
		scanf("%s", date);
		printf("\n==========所有账号信息=========\n");
		printf("%-8s\t%-8s\t%-8s\t%-8s\t%-8s\t\n", "银行", "账号", "名字", "身份证号", "余额");
		while ((fread(&buff, sizeof(struct account), 1, fp))) {
			if (strcmp(date, buff.de_time) == 0) {

				//显示该账户基础信息
				printf("%-8s\t", buff.de_bank);
				printf("%-8s\t", buff.acc_no);
				printf("%-8s\t", buff.name);
				printf("%-8s\t", buff.id);
				printf("%-8.2f\t", buff.balance);
				printf("\n");

			}
		}
		break;
	case 3:
		printf("\n请输入姓名：");
		scanf("%s", name);
		printf("\n==========所有账号信息=========\n");
		printf("%-8s\t%-8s\t%-8s\t%-8s\t%-8s\t\n", "银行", "账号", "名字", "身份证号", "余额");
		while ((fread(&buff, sizeof(struct account), 1, fp)) != (int)NULL) {
			if (strcmp(name, buff.name) == 0) {

				//显示该账户基础信息
				printf("%-8s\t", buff.de_bank);
				printf("%-8s\t", buff.acc_no);
				printf("%-8s\t", buff.name);
				printf("%-8s\t", buff.id);
				printf("%-8.2f\t", buff.balance);
				printf("\n");

			}
		}
		break;
	case 4:
		printf("\n请输入身份证号：");
		scanf("%s", id);
		printf("\n==========所有账号信息=========\n");
		printf("%-8s\t%-8s\t%-8s\t%-8s\t%-8s\t\n", "银行", "账号", "名字", "身份证号", "余额");
		while ((fread(&buff, sizeof(struct account), 1, fp))) {
			if (strcmp(id, buff.id) == 0) {

				//显示该账户基础信息
				printf("%-8s\t", buff.de_bank);
				printf("%-8s\t", buff.acc_no);
				printf("%-8s\t", buff.name);
				printf("%-8s\t", buff.id);
				printf("%-8.2f\t", buff.balance);
				printf("\n");

			}
		}
		break;
	case 5:
		printf("\n==========所有账号信息=========\n");
		printf("%-8s\t%-8s\t%-8s\t%-8s\t%-8s\t\n", "银行", "账号", "名字", "身份证号", "余额");
		while ((fread(&buff, sizeof(struct account), 1, fp)) != (int)NULL) {

			//显示该账户基础信息
			printf("%-8s\t", buff.de_bank);
			printf("%-8s\t", buff.acc_no);
			printf("%-8s\t", buff.name);
			printf("%-8s\t", buff.id);
			printf("%-8.2f\t", buff.balance);
			printf("\n");

		}
	}
	fclose(fp);

}

void search(int *n, struct account *buffe) {
	char accNo[20];
	char bank[20];
	FILE * fp;
	int i = 0;
	//输入账号
	struct account buff = {
		"acc_no",
			"name",
			0,
			"id",
			"phonenum",
			"passd",
			//"saving_clerk",
			"home_site",
			"de_bank",
			"de_time",
	};
	if ((fp = fopen("bankcard_base.dat", "rb")) == NULL) {
		printf("文件未能打开\n");
	}
	printf("请输入开户银行");
	scanf("%s", bank);
	printf("请输入账号：");
	scanf("%s", accNo);
	//定位指针到文件首
	rewind(fp);
	//遍历所有元素，直到所要查找的那个账号
	while ((fread(&buff, sizeof(struct account), 1, fp))) {
		if (strcmp(bank, buff.de_bank) == 0) {
			if (strcmp(accNo, buff.acc_no) == 0) {
				break;
			}
		}
		i++;
	}
	*n = i;
	*buffe = buff;
	fclose(fp);
}


//统计管理
void count()
{
	char accNo[20];
	char date[20];
	char name[20];
	char clerk[20];
	char id[20];
	int search_style;
	int ss = 1;
	int relay = 1;
	float total_balance = 0;
	float total_income = 0;
	float total_pay = 0;
	struct remark tmp;
	FILE * fp;
	//输入账号
	struct account buff = {
		"acc_no",
			"name",
			0,
			"id",
			"phonenum",
			"passd",
			//"saving_clerk",
			"home_site",
			"de_bank",
			"de_time",
	};
	if ((fp = fopen("bankcard_base.dat", "rb")) == NULL) {
		printf("文件未能打开\n");
		exit(0);
	}

	//依据身份证号统计余额
	printf("===========请选择统计方式===========\n");
	printf("1.依据账号基本信息统计余额\n");
	printf("2.依据储蓄交易信息统计\n");
	scanf("%d", &ss);
	printf("请输入选项：");
	rewind(fp);
	switch (ss) {
	case 1:
		printf("==============请选择统计依据============");
		printf("1.姓名");
		printf("2.身份证号");
		printf("请选择统计依据：");
		scanf("%d", &relay);
		switch (relay) {
		case 1:
			printf("请输入姓名：\n");
			scanf("%s", name);
			printf("\n==========所有账号信息=========\n");
			printf("%-8s\t%-8s\t%-8s\t\n", "身份证号", "姓名", "余额");
			while ((fread(&buff, sizeof(struct account), 1, fp))) {
				if (strcmp(buff.name, name) == 0) {
					//total_balance += buff.balance;
					//显示该账户基础信息
					printf("%-8s\t", buff.id);
					printf("%-8s\t", buff.name);
					printf("%-8.2f\t", buff.balance);
					printf("\n");
				}
			}
			break;
		case 2:
			printf("请输入身份证号：\n");
			scanf("%s", id);
			printf("\n==========所有账号信息=========\n");
			printf("%-8s\t%-8s\t%-8s\t\n", "身份证号", "姓名", "余额");
			while ((fread(&buff, sizeof(struct account), 1, fp))) {
				if (strcmp(buff.id, id) == 0) {
					//total_balance += buff.balance;
					//显示该账户基础信息
					printf("%-8s\t", buff.id);
					printf("%-8s\t", buff.name);
					printf("%-8.2f\t", buff.balance);
					printf("\n");
				}
			}
			break;
		}
		//printf("您的总余额为%.2f", total_balance);
		break;

	case 2:
		printf("==============请选择统计依据============");
		printf("1.储蓄员");
		printf("2.时间");
		printf("请选择统计依据：");
		scanf("%d", &relay);
		switch (relay) {
		case 1:
			//依据储蓄员统计
			printf("请输入储蓄员：");
			scanf("%s", clerk);
			printf("\n==========所有账号信息=========\n");
			printf("%-8s\t%-8s\t%-8s\t%-8s\t%-8s\t%-8s\t\n", "账号", "用户", "储蓄员", "收入金额", "支出金额", "时间");
			while ((fread(&buff, sizeof(struct account), 1, fp)) != (int)NULL) {
				for (int i = 0; i < 100; i++) {
					tmp = buff.target[i];
					if (strcmp(tmp.saving_clerk, clerk) == 0) {
						//显示该账户基础信息
						printf("%-8s\t", buff.acc_no);
						printf("%-8s\t", buff.name);
						printf("%-8s\t", tmp.saving_clerk);
						printf("%-8.2f\t", tmp.income);
						printf("%-8.2f\t", tmp.pay);
						printf("%-8s\t", tmp.date);
						printf("\n");
					}
				}
			}
			break;
		case 2:
			printf("请输入时间：(按照1998-8-8输入)");
			scanf("%s", date);
			printf("\n==========所有账号信息=========\n");
			printf("%-8s\t%-8s\t%-8s\t%-8s\t%-8s\t%-8s\t\n", "账号", "用户", "储蓄员", "收入金额", "支出金额", "时间");
			while ((fread(&buff, sizeof(struct account), 1, fp)) != (int)NULL) {
				for (int i = 0; i < 100; i++) {
					tmp = buff.target[i];
					if (strcmp(tmp.date, date) == 0) {
						//显示该账户基础信息
						printf("%-8s\t", buff.acc_no);
						printf("%-8s\t", buff.name);
						printf("%-8s\t", tmp.saving_clerk);
						printf("%-8.2f\t", tmp.income);
						printf("%-8.2f\t", tmp.pay);
						printf("%-8s\t", tmp.date);
						printf("\n");
					}
				}
			}
			break;
		}
	}
}
