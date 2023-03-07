#define MaxNum 100
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<limits.h>
//1.短作业优先算法
struct Process_struct {
	int  Number;                 //进程编号
	char Name[MaxNum];           //进程名称
	int  ArrivalTime;    //到达时间
	int  ServiceTime;    //开始运行时间
	int  FinishTime;     //运行结束时间
	int  WholeTime;      //运行时间
	int run_flag;        //调度标志
	int order;           //运行次序
	double  WeightWholeTime;        //周转时间
	double AverageWT_FCFS, AverageWT_SJF;    //平均周转时间
	double AverageWWT_FCFS, AverageWWT_SJF;  //平均带权周转时间
}Process[MaxNum];

//找到当前到达的运行时间最短的进程
int N;    //实际进程个数
int SJF() {       //短作业优先算法
	int temp_time = 0;    //当期那时间
	int i = 0, j;
	int number_schedul=0, temp_counter;      //进程编号，当前已执行进程个数
	float run_time= INT_MAX;
	temp_counter = 0;
	while (temp_counter < N)
	{
		int matime = INT_MAX;//设置该临时变量的值为无穷大，用于比较寻找最先到达的进程
		for (j = 0; j < N; j++)
		{
			//如果到达时间小于当前时间，且进程未完成
			if ((Process[j].ArrivalTime <= temp_time) && (!Process[j].run_flag))
			{//如果找到一个到达时间小于当前时间的进程，则选择该进程，不一定运行，因为有可能存在同时到达但比这个进程作业时间短的进程
				run_time = Process[j].WholeTime;
				number_schedul = j;
				Process[number_schedul].ServiceTime = temp_time; //开始服务时间是当前时间
				break; //只需要找一次就行
			}
			else {
				//如果没有在当前时刻之前到达的进程，则在就绪队列中找到达时间最早的进程
				if (Process[j].ArrivalTime < matime && (!Process[j].run_flag))
				{
					matime = Process[j].ArrivalTime;
					run_time = Process[j].WholeTime;
					number_schedul = j;
					Process[number_schedul].ServiceTime = Process[j].ArrivalTime; //开始服务时间是到达时间
				}
			}
		}
		for (j = 0; j < N; j++)
		{
			//如果到达时间小于当前时间，且进程未完成
			if ((Process[j].ArrivalTime <= temp_time) && (!Process[j].run_flag))
				//若果有比准备运行的进程运行时间更短的进程，则选择这个运行时间更短进程运行
			{
				if (Process[j].WholeTime < run_time)
				{
					run_time = Process[j].WholeTime;
					number_schedul = j;
					Process[number_schedul].ServiceTime = temp_time; //开始服务时间是当前时间
				}
			}
			else {//在同时到达的进程中找到作业时间最短的一个
				if (Process[j].ArrivalTime == matime && (!Process[j].run_flag)&& Process[j].WholeTime < run_time)
				{
					matime = Process[j].ArrivalTime;
					run_time = Process[j].WholeTime;
					number_schedul = j;
					Process[number_schedul].ServiceTime = Process[j].ArrivalTime; //开始服务时间是到达时间
				}

			}

		}
		
		//对找到的下一个被调度的进程求相应的参数
		
		Process[number_schedul].FinishTime = Process[number_schedul].ServiceTime + Process[number_schedul].WholeTime;
		Process[number_schedul].run_flag = 1;
		temp_time = Process[number_schedul].FinishTime;
		Process[number_schedul].order = temp_counter;
		temp_counter++;
	}return 0;
}
int Pinput()   //进程参数输入
{
	int i;
	printf("Enter the number of processes:");
	scanf("%d", &N);
	for (i = 0; i < N; i++)
	{
		printf("----Enter the informations of process %d----\n", i + 1);
		printf("Enter the process name:");
		scanf("%s", Process[i].Name);
		printf("Enter the arrival time:");
		scanf("%d", &Process[i].ArrivalTime);
		printf("Enter the service time:");
		scanf("%d", &Process[i].WholeTime);
		Process[i].ServiceTime = 0;
		Process[i].FinishTime = 0;
		Process[i].WeightWholeTime = 0;
		Process[i].order = 0;
		Process[i].run_flag = 0;
	}return 0;
}
int Poutput()   //调度结果输出
{
	int i;
	float turn_round_time = 0, f1, w = 0;

	printf("      Name arrive_t run_t start_t finish_t execute_seq around_t weight_around_t\n");
	for (i = 0; i < N; i++)
	{
		Process[i].WeightWholeTime = Process[i].FinishTime - Process[i].ArrivalTime;
		f1 = Process[i].WeightWholeTime / Process[i].WholeTime;
		turn_round_time += Process[i].FinishTime-Process[i].ArrivalTime;
		w += f1;
		printf("time%2d:", Process[i].ServiceTime);
		printf("%-5s%-9d%-6d%-8d%-9d%-12d%-9.2f%-.2f\n", Process[i].Name, Process[i].ArrivalTime, Process[i].WholeTime, Process[i].ServiceTime, Process[i].FinishTime, Process[i].order, Process[i].WeightWholeTime, f1);
	}
	printf("average_turn_round_time=%.3f\n", turn_round_time / N);
	printf("weight_average_turn_round_time=%.3f\n", w / N);
	return 0;
}



//2.先来先服务算法
typedef struct PCB      //定义进程控制块
{
	char name[10];   //进程名
	char state;   //运行状态
	int  arrivetime;  //到达时间
	int  starttime;  //进程开始时间
	int  finishtime;  //进程结束时间
	int  servicetime;   //服务时间
	float turnaroundtime;//周转时间
	float weightedturnaroundtime;//带权周转时间
	struct PCB* next;  //指向下个进程
}pcb;

int time=0;   //计时器
int n;    //进程个数
float avg_around_time = 0;  //平均周转时间
pcb* head1 = NULL, * p, * q,*min; //进程链表指针
void run_fcfs(pcb* p1) //运行未完成的进程
{
	time =( p1->arrivetime > time ? p1->arrivetime : time);
	p1->starttime = time;
	printf("\nNow the time is %d,Start running job %s\n", time, p1->name);
	time += p1->servicetime;   //计时器
	p1->state = 'F';
	p1->finishtime = time;
	p1->turnaroundtime = p1->finishtime - p1->arrivetime;
	p1->weightedturnaroundtime = p1->turnaroundtime / p1->servicetime;
	avg_around_time += p1->turnaroundtime;
	printf("Name Arrive_t start_t served_t finish_t around_t weight_around_t\n");
	printf("%-5s %-9d%-8d%-9d%-9d%-9.1f%.2f\n",
		p1->name,p1->arrivetime, p1->starttime, p1->servicetime, p1->finishtime,
		p1->turnaroundtime, p1->weightedturnaroundtime);
}
void fcfs() //找先到先服务
{
	int i, j;
	for (i = 0; i < n; i++) //遍历链表找到服务顺序
	{
		p = head1;
		q = p; //标记当前的进程
		for (j = 0; j < n; j++)
		{
			if (p && p->state == 'W')
			{min = p; break;}
			p = p->next;
		}
		for (j=0;j < n;j++)
		{
			if(p->next)	p=p->next;
			if ((min->arrivetime >= p->arrivetime)&& min->state == 'W'&& p->state == 'W')
				//F表示已经完成Finish状态，W-wait,R-run
			{
				min = p; //找到最先到达的进程
			}
	    }
		run_fcfs(min);
	}
	printf("average_turn_round_time=%f\n", avg_around_time / n);
}
void getInfo()          //获得进程信息并创建进程
{
	int num;
	printf("\nNumber of jobs:");
	scanf("%d", &n);

	for (num = 0; num < n; num++)
	{
		if ((p = (pcb*)malloc(sizeof(pcb))) == NULL)
		{
			perror("malloc");
			exit(1);
		}
		printf("----Enter the informations of process %d----\n", num + 1);
		printf("Enter the process name:");
		scanf("%s", &p->name);
		printf("Enter the arrival time:");
		scanf("%d", &p->arrivetime);
		printf("Enter the service time:");
		scanf("%d", &p->servicetime);
		if (head1 == NULL) { head1 = p; q = p; time = p->arrivetime; }
		if (p->arrivetime < time) time = p->arrivetime;
		q->next = p;
		p->starttime = 0;
		p->finishtime = 0;
		p->turnaroundtime = 0;
		p->weightedturnaroundtime = 0;
		p->next = NULL;
		p->state = 'W';
		q = p;
	}
}


//3.时间片轮转算法
typedef struct node
{
	char name[20];
	int round;
	int cputime;   //进程总共使用cpu的时间，即进程的服务时间
	int starttime;  //进程开始运行时间
	int arrivetime; //进程到达内存的时间
	int finishtime; //进程完成时间
	int needtime;  //进程的运行时间
	char state;  //进程状态
	int count;
	struct node* next;
}PCB;
PCB* ready = NULL, * run1 = NULL, * finish = NULL;
int num, i;

void GetFirst();
void Output();
void InsertTime(PCB* in);
void InsertFinish(PCB* in);
void TimeCreate();
void RoundRun();


void GetFirst()  /*取得第一个就绪队列节点*/
{
	run1 = ready;

	if (ready != NULL)
	{
		run1->state = 'R'; //R--run
		ready = ready->next;
		run1->next = NULL;
	}
}
void Output()    /*输出队列信息*/
{
	PCB* p;
	p = ready;  //就绪队列信息
	printf("Time  Name cpu_t Need_Time status Counter\n");
	while (p != NULL)
	{
		printf("%2d-%-2d %-5s%-6d%-10d%-7c%d\n",p->starttime,p->finishtime, p->name, p->cputime, p->needtime, p->state, p->count);
		p = p->next;
	}
	p = finish; //完成队列信息
	while (p != NULL)
	{
		printf("%2d-%-2d %-5s%-6d%-10d%-7c%d\n", p->starttime, p->finishtime, p->name, p->cputime, p->needtime, p->state, p->count);
		p = p->next;
	}
	p = run1;  //运行队列信息
	while (p != NULL)
	{
		printf("%2d-%-2d %-5s%-6d%-10d%-7c%d\n", p->starttime, p->finishtime, p->name, p->cputime, p->needtime, p->state, p->count);
		p = p->next;
	}
}

void InsertTime(PCB* in)  /*将进程插入到就绪队列尾部*/
{
	PCB* fst;
	fst = ready;

	if (ready == NULL)
	{
		in->next = ready;
		ready = in;
	}
	else
	{
		while (fst->next != NULL)
		{
			fst = fst->next;
		}
		in->next = fst->next;
		fst->next = in;
	}
}
void InsertFinish(PCB* in)  /*将进程插入到完成队列尾部*/
{
	PCB* fst;
	fst = finish;

	if (finish == NULL)
	{
		in->next = finish;
		finish = in;
	}
	else
	{
		while (fst->next != NULL)
		{
			fst = fst->next;
		}
		in->next = fst->next;
		fst->next = in;
	}
}
void TimeCreate() /*时间片输入函数*/
{
	PCB* tmp;
	int i;


	for (i = 0; i < num; i++)
	{
		printf("----Enter the informations of process %d----\n", i+ 1);
		if ((tmp = (PCB*)malloc(sizeof(PCB))) == NULL)
		{
			perror("malloc");
			exit(1);
		}
		printf("Enter the process name:");
		scanf("%s", tmp->name);
		printf("Enter the arrival time:");
		scanf("%d", &(tmp->arrivetime));
		printf("Enter the service time:");
		scanf("%d", &(tmp->needtime));
		printf("Enter the time slice:");
		scanf("%d", &(tmp->round));
		getchar();
		tmp->cputime = 0;
		tmp->state = 'W';
		//	tmp->prio=0;
		tmp->count = 0;
		InsertTime(tmp);
	}
}
void RoundRun()    /*时间片轮转调度算法*/
{
	int flag = 1,flag2=1;
	int time = 0; //当前时间
	float avf_around_t = 0;
	GetFirst();
	while (run1 != NULL)
	{
		Output();
		while (flag)
		{
			if (time < run1->arrivetime)
				run1->starttime = run1->arrivetime;
			else
				run1->starttime = time;
			run1->cputime++;
			run1->needtime--;
			run1->count++;
			if (run1->needtime == 0)
			{
				run1->state = 'F';
				run1->finishtime = run1->starttime + run1->count;//记录每个进程的实际结束时间
				InsertFinish(run1);  //在分配的时间片内运行完成
				flag = 0;
			}
			else if (run1->count == run1->round) //时间片用完
			{
				run1->state = 'W';
				run1->finishtime = run1->starttime + run1->round;
				run1->count = 0;
				InsertTime(run1); //进程再次回到就绪队列
				flag = 0;
			}
		}
		if(run1->state=='F')
		{
			avg_around_time += run1->finishtime - run1->arrivetime;
		}
		time = run1->finishtime;
		flag = 1; //用于判断一个进程是否完成
		GetFirst();
	}
	Output();
	printf("average_turn_round_time=%f\n", avg_around_time / num);
}






//4.静态优先权优先算法

int SequenceNumber = 1;	//进程编号
int ProcessAmount;		//进程数量
int StartCount = 0;		//调入内存进程计数

struct PCB1 {				//进程控制块
	int No;				//进程号
	char name[16];		//进程名
	int enterMemoryTime;//进入内存时间
	int serviceTime;	//服务时间
	int priority;		//优先级
	struct PCB1* next;
};
struct PCB1* head, * thisP, * newP;		//动态接收进程及详细

struct PCB1* readyHead;				//动态存储就绪队列

//函数声明
int timeOfData(FILE*);
void ListAllPCB(struct PCB1* h);
void release(struct PCB1*);
void releaseR(struct PCB1*);
void setNewPCB(char*, int, int, int);
struct PCB1* runTheProcess(int i);

void newPCB() {			//建立PCB
	newP = (struct PCB1*)malloc(sizeof(struct PCB1));

	if (head == NULL) {		//判断头节点是否为空
		head = newP;		//为空，头节点指向新开辟的内存
	}
	else {
		thisP = head;
		while (thisP->next != NULL) {
			thisP = thisP->next;
		}
		thisP->next = newP;		//遍历单链表，找到最后一个元素
	}

	thisP = newP;
	//插入新节点
	thisP->No = SequenceNumber;
	SequenceNumber++;
	//printf("ID %d\n", thisP->No);
	printf("----Enter the informations of process %d----\n", thisP->No);
	printf("Enter the process name:");
	scanf("%s", thisP->name);
	printf("Enter priority:");
	scanf("%d", &thisP->priority);
	printf("Enter arrive time:");
	scanf("%d", &thisP->enterMemoryTime);
	printf("Enter service time:");
	scanf("%d", &thisP->serviceTime);

	thisP->next = NULL;
}
void buildProcess() {		//创建进程
	int i = 0;
	printf("Enter the number of processes:");
	scanf("%d", &ProcessAmount);
	while (i < ProcessAmount) {
		newPCB();
		i++;
	}
}
void readyList(struct PCB1* pcb) {		//单链表的形式创建就绪队列
	newP = (struct PCB1*)malloc(sizeof(struct PCB1));

	if (readyHead == NULL) {
		readyHead = newP;
	}
	else {
		thisP = readyHead;
		while (thisP->next != NULL) {
			thisP = thisP->next;
		}
		thisP->next = newP;
	}
	thisP = newP;
	strcpy(thisP->name, pcb->name);
	thisP->No = pcb->No;
	thisP->priority = pcb->priority;
	thisP->enterMemoryTime = pcb->enterMemoryTime;
	thisP->serviceTime = pcb->serviceTime;
	thisP->next = NULL;
}
struct PCB1* callMemory() {		//调入内存，返回调入内存的链表节点
	int at;		//到达时间
	struct PCB1* markP;
	if (head == NULL) {
		printf("Program not found.\n");
	}
	else {
		markP = thisP = head;		//标记指向头节点
		at = thisP->enterMemoryTime;	//到达时间为头节点到达时间
		while (thisP->next != NULL) {	//当下一节点不为空
			if (at > thisP->next->enterMemoryTime) {	//判断当前时间是否大于下一节点时间
				markP = thisP->next;			//是，标记此节点
				at = markP->enterMemoryTime;	//到达时间更改为标记时间
			}
			thisP = thisP->next;	//向后遍历
		}
	}
	return markP;
}
int timesetout(int at)  //判断就绪队列中是否有在当前时间之前到达的进程
{
	struct PCB1* markThis;
	if (readyHead == NULL) {
		printf("There are no runnable processes.\n");
	}
	else {
		markThis = thisP = readyHead;
		time = thisP->enterMemoryTime;//当前到达时间
		while (thisP->next != NULL) {
			if (thisP->enterMemoryTime < at)
				return 1; //表示有在当前时间之前到达的进程
			thisP = thisP->next;
		}
		return 0;
	}
}
void run() {		//静态优先级算法执行。
	struct PCB1* temp;	//临时节点用来存储调入内存节点
	struct PCB1* runPro;	//用来接收执行的节点
	int i;		//循环初始条件
	int at=0, srt, finishtime = 0;	//当前时间，开始执行时间，运行时间。
	int turnOverTime;		//周转时间
	float avg_around_time = 0;
	double ptot;			//带权周转时间
	if (head == NULL) {
		printf("No processes found.\n");
	}
	else {
		while (head != NULL) { //将所有进程都放到就绪队列中去
			temp = callMemory(); //返回当前最先进入内存的进程
			readyList(temp);	//初始纳入就绪列表
			release(temp);		//释放进程
		}//循环完成之后，就绪队列中的进程按照进入内存的时间由小到大排序


		for (i = 0; i < ProcessAmount; i++) {	//循环进程的数量次
			runPro = runTheProcess(timesetout(at));  //1.返回就绪队列中当前最先进入内存且在同时间到达的进程中优先权最高的进程
			//2.返回就绪队列中优先级最高的进程
			if (runPro->enterMemoryTime > at) {
				//初始情况下求开始执行时间，完成时间，周转时间，带权周转时间
				srt = runPro->enterMemoryTime;  //开始时间等于到达时间
			}
			else {		//初始之后，计算各时间及周转
				srt = at;//开始时间等于当前时间
			}
			finishtime = srt + runPro->serviceTime;
			turnOverTime = finishtime - runPro->enterMemoryTime;
			avg_around_time += turnOverTime;
			ptot = turnOverTime * 1.0 / runPro->serviceTime;
			at = finishtime;
			printf("Currently executing process:");
			printf("\n");
			printf("Name ID   priority arrive_t serve_t start_t finish_t around_t weight_around_t\n");

			if (runPro != NULL) {
				printf("%-5s%-5d%-9d%-9d%-8d", runPro->name, runPro->No, runPro->priority, runPro->enterMemoryTime, runPro->serviceTime);
				printf("%-8d%-9d%-9d%-8.2lf", srt, finishtime, turnOverTime, ptot);
			}
			else
				printf("There are no processes.\n");
			releaseR(runPro);
			printf("\nReady process list: ");
			ListAllPCB(readyHead);
		}
	}
	printf("average_turn_round_time=%f\n", avg_around_time / ProcessAmount);
}
struct PCB1* runTheProcess(int i) {	//执行的进程，返回要执行进程的节点
	int time, pri;
	struct PCB1* markThis;	//标记要返回的节点
	if (readyHead == NULL) {
		printf("There are no runnable processes.\n");
	}
	else {
		markThis = thisP = readyHead;
		time = thisP->enterMemoryTime;//当前到达时间
		pri = thisP->priority;	//当前进程的优先级
		while (thisP->next != NULL) {
			if (i == 0)
			{
				if (time == thisP->next->enterMemoryTime) {//判断之后节点的到达时间是否与当前一致
					if (pri < thisP->next->priority) {//一致，则判断优先级决定
						pri = thisP->next->priority;//更改优先级判断条件为下一优先级
						markThis = thisP->next;	//标记当前节点的下一节点
					}
					else {
						break;
					}
				}
			}
			else {
				if (pri < thisP->next->priority) {//判断优先级决定
					pri = thisP->next->priority;//更改优先级判断条件为下一优先级
					markThis = thisP->next;	//标记当前节点的下一节点
				}
				else {
					break;
				}
			}
			thisP = thisP->next;
		}
	}
	return markThis;
}
void release(struct PCB1* pcb) {		//删除原单链表的进程节点
	struct PCB1* markP, * f;
	markP = head;
	if (pcb != NULL) {
		if (pcb == head) {
			f = head;
			head = head->next;
			free(f);
		}
		else {
			thisP = head->next;
			while (thisP != NULL) {
				if (pcb == thisP) {
					f = thisP;
					markP->next = thisP->next;
					free(f);
					break;
				}
				else {
					markP = thisP;
				}
				thisP = thisP->next;
			}
		}
	}
}
void releaseR(struct PCB1* pcb) {		//删除就绪列表中进程节点
	struct PCB1* markP, * f;
	markP = readyHead;
	if (pcb != NULL) {
		if (pcb == readyHead) {
			f = readyHead;
			readyHead = readyHead->next;
			free(f);
		}
		else {
			thisP = readyHead->next;
			while (thisP != NULL) {
				if (pcb == thisP) {
					f = thisP;
					markP->next = thisP->next;
					free(f);
					break;
				}
				else {
					markP = thisP;
				}
				thisP = thisP->next;
			}
		}
	}
}
void ListAllPCB(struct PCB1* h) {		//打印所有进程
	if (h == NULL) {
		printf("\nThere are no processes.\n\n");
	}
	else {
		printf("\n");
		printf("Name ID   priority arrie_t served_t\n");
		thisP = h;
		while (thisP != NULL) {
			printf("%-5s%-5d%-9d%-8d%-8d\n", thisP->name, thisP->No, thisP->priority, thisP->enterMemoryTime, thisP->serviceTime);
			thisP = thisP->next;
		}
		printf("\n");
	}
}





//5.高响应比调度算法
int N;
float timecounter5 = 0; //计时器
struct zgxyb* c;
struct zgxyb {
	char name[10];
	float arrivetime;
	float servicetime;
	float starttime;
	float finishtime;
	float around_time; //周转时间
	float zztime;//等待时间
	float dqzztime; //进程的响应比
	char stats; //进程状态
};
struct zgxyb a[100];
void input(struct zgxyb* p, int N)
{
	int i;
	for (i = 0; i <= N - 1; i++)
	{
		printf("----Enter the informations of process %d----\n",i + 1);
		//printf("Name, arrive_t need_t:\n");
		//scanf("%s%f%f", &p[i].name, &p[i].arrivetime, &p[i].servicetime);
		printf("Enter the process name:");
		scanf("%s", &p[i].name);
		printf("Enter the arrival time:");
		scanf("%f", &p[i].arrivetime);
		printf("Enter the service time:");
		scanf("%f", &p[i].servicetime);
		p[i].stats = 'W';
	}
}
void Print(struct zgxyb* p)
{
	int k;
	printf("\nrun order:");
	printf("%s", p[0].name);
	for (k = 1; k < N; k++)
	{
		printf("-->%s", p[k].name);
	}
	printf("\nCurrent executing_process information:\n");
	printf("Name arrive_t served_t start_t finish_t around_t RRatio\n");
	for (k = 0; k <= N - 1; k++)
	{
		printf(" %-5s%-9.2f%-9.2f%-8.2f%-9.2f%-9.2f%-.2f\n", p[k].name, p[k].arrivetime, p[k].servicetime, p[k].starttime, p[k].finishtime, p[k].around_time, p[k].dqzztime);
	}

}
//按到达时间排序
void sort(struct zgxyb* p,int N)
{
	int i, j;
	for (i = 0; i <= N - 1; i++)
		for (j = 0; j <= i; j++)
			if (p[i].arrivetime < p[j].arrivetime)
			{
				struct zgxyb temp;
				temp = p[i];
				p[i] = p[j];
				p[j] = temp;
			}
}
void calculate(struct zgxyb* p)
{
	for (int n = 1; n < N; n++) //计算各个进程的相应比
	{
		if (p[n].stats == 'W')
		{
			p[n].zztime = timecounter5 - p[n].arrivetime; //进程的等待时间=当前时间-到达时间
			p[n].dqzztime = 1 + p[n].zztime / p[n].servicetime;
		}
	}
}
void ZGXYB(struct zgxyb* p, int N)
{
	int m, i, n, follow, k;
	int schedul=0; //当前时间
	float max=0,avg_around_time=0;
	struct zgxyb temp;
	sort(p,N);  //按到达时间对进程进行排序
	for(i=0;i<N;i++) //运行N个进程
	{ 
			if (timecounter5 < p[schedul].arrivetime)
				p[schedul].starttime = p[schedul].arrivetime;
			else
				p[schedul].starttime = timecounter5;
			p[schedul].finishtime = p[schedul].starttime + p[schedul].servicetime;
			p[schedul].around_time = p[schedul].finishtime - p[schedul].arrivetime;
			avg_around_time+= p[schedul].around_time;
			p[schedul].stats = 'F';
			timecounter5 = p[schedul].finishtime;
			
			calculate(p); //计算各进程相应比
			for (m = 1; m < N; m++)  //选择一个未完成进程
			{
				if (p[m].stats == 'W')
				{
					max = p[m].dqzztime;
					schedul = m;
					break;
				}
			}
			for (int k = 1; k < N; k++)  //选出响应比最高的进程
			{
				if (max <= p[k].dqzztime && p[k].stats == 'W')
				{
					max = p[k].dqzztime;
					schedul = k;
				}
			}
	}
	Print(p);
	printf("average_turn_round_time=%f\n", avg_around_time / N);
}

//从文件读取数据
/*数据格式如下

进程名  到达时间 服务时间 优先级 时间片
p1 0 7 1 5
p2 2 4 2 3
p3 5 4 2 3
*/


//1.短作业优先
void SJFsetfin(char* name, int entime, int sertime,int i)   //进程参数输入
{
	strcpy(Process[i].Name,name);
	Process[i].ArrivalTime=entime;
	Process[i].WholeTime=sertime;
	Process[i].ServiceTime = 0;
	Process[i].FinishTime = 0;
	Process[i].WeightWholeTime = 0;
	Process[i].order = 0;
	Process[i].run_flag = 0;
}
//2.先来先服务
void FCFSsetfin(char* name, int entime, int sertime)
{
		p = (pcb*)malloc(sizeof(pcb));
		strcpy(p->name, name);
		p->arrivetime = entime;
		p->servicetime = sertime;
		if (head1 == NULL) { head1 = p; q = p; time = p->arrivetime; }
		if (p->arrivetime < time) time = p->arrivetime;
		q->next = p;
		p->starttime = 0;
		p->finishtime = 0;
		p->turnaroundtime = 0;
		p->weightedturnaroundtime = 0;
		p->next = NULL;
		p->state = 'W';
		q = p;
}
//3.时间片轮询
PCB* tmp;
void RRsetfin(char* name, int entime, int sertime,int slice)
{
	if ((tmp = (PCB*)malloc(sizeof(PCB))) == NULL)
	{
		perror("malloc");
		exit(1);
	}
	strcpy(tmp->name, name);
	tmp->arrivetime = entime;
	tmp->needtime = sertime;
	tmp->round = slice;
	tmp->cputime = 0;
	tmp->state = 'W';
	tmp->count = 0;
	InsertTime(tmp);
}
//4.最高响应比
void ZGXYBsetin(struct zgxyb* p,char* name, float entime, float sertime,int i)
{
	strcpy(p[i].name, name);
	p[i].arrivetime = entime;
	p[i].servicetime = sertime;
	p[i].stats = 'W';
}
//5.静态优先级
void setNewPCB(char* name, int pri, int entime, int sertime) {//赋值建立PCB，用于从文本中获取数据

	newP = (struct PCB1*)malloc(sizeof(struct PCB1));

	if (head == NULL) {
		head = newP;
	}
	else {
		thisP = head;
		while (thisP->next != NULL) {
			thisP = thisP->next;
		}
		thisP->next = newP;
	}

	thisP = newP;
	thisP->No = SequenceNumber;
	SequenceNumber++;
	strcpy(thisP->name, name);
	thisP->priority = pri;
	thisP->enterMemoryTime = entime;
	thisP->serviceTime = sertime;

	thisP->next = NULL;
}
void readFromFile(int option) {
	FILE* fp;
	int times;
	int i = 0,pri;
	char filePath[20], temp[1024];
	char name[16];
	float emt, st, slice;
	printf("Enter file path:");
	scanf("%s", filePath);
	fp = fopen(filePath, "r");
	times = timeOfData(fp);
	ProcessAmount =N=n=num=times - 2;  //进程个数
	while (i < times - 1) {  //循环次数=文本行数
		if (i == 0) {	//这是用来接收文本格式第一行的汉字。
			fgets(temp, sizeof(temp), fp); //把读取的第一行的内容放在temp数组中
		}
		else {		//以下用来接收进程数据
			fscanf(fp, "%s", name);
			fscanf(fp, "%f", &emt);
			fscanf(fp, "%f", &st);
			fscanf(fp, "%d", &pri);
			fscanf(fp, "%f", &slice);
			switch (option)
			{
			case 1:
				SJFsetfin(name, emt, st, i-1);  //添加一个进程到就绪队列中
				break;
			case 2:
				RRsetfin(name, emt, st, slice);//添加一个进程
				break;
			case 3:
				FCFSsetfin(name, emt, st);//添加一个进程
				break;
			case 4:
				ZGXYBsetin(a, name, emt, st, i-1);//添加一个进程
				break;
			case 5:
				setNewPCB(name, pri, emt, st);//添加一个进程
				break;
			default:
				break;
			}
		}
		i++;
	}
	fclose(fp);
}
int timeOfData(FILE* f) {	//求取从文本中获取数据的进程数量
	char temp[20];
	int i = 0;
	int flag = 0;
	if (f == NULL) {
		printf("File not found.\n");
		exit(0);
	}
	else {
		while (!feof(f))
		{
			flag = fgetc(f);
			if (flag == '\n')
				i++;
		}
	}
	rewind(f);
	return i;
}
//主函数
void choose(char option, int select)
{
	switch (option)
	{
	case '0':
		printf("End of operation.\n");
		return;
	case '1':
		printf("\n             -----Short job first scheduling algorithm------\n\n");
		if (select != 'F')
			Pinput();
		else
			readFromFile(1);

		SJF();	sh:
		Poutput();
		return;
	case '2':
		printf("\n             --------------Round-Robin algorithm------------\n\n");
		if (select != 'F')
		{
			printf("Enter the number of processes to create :");
			scanf("%d", &num);
			TimeCreate();
		}
		else
			readFromFile(2);

		RoundRun();
		return;

	case '3':
		printf("\n             ------First come first serve algorithm-------\n");
		if (select != 'F')
			getInfo();
		else
			readFromFile(3);
		p = head1;
		fcfs();
		return;

	case '4':
		printf("\n             -----Highest Response Ratio Next algorithm----\n\n");
		if (select != 'F')
		{
			printf("Enter the number of processes:");
			scanf("%d", &N);
			input(a, N);

		}
		else
			readFromFile(4);

		ZGXYB(a, N);

		return;

	case '5':
		printf("\n            -----Static priorit algorithm process scheduling---\n\n");
		if (select != 'F')
		{
			buildProcess();
		}
		else
			readFromFile(5);

		printf("Process list: ");
		ListAllPCB(head);
		run();
		return;
	default:
		break;
	}
}
void main()
{
	char select,option[5];


	printf("           --------------Welcome to use this system------------\n");
	printf("           ----------------------------------------------------\n");
	printf("           --         1    SJF,Shortest Job First            --\n");
	printf("           --         2    RR,Round-Robin                    --\n");
	printf("           --         3    FCFS,first come first served      --\n");
	printf("           --         4    HRRN,Highest Response Ratio Next  --\n");
	printf("           --         5    static priority algorithm         --\n");
	printf("           --         0    exit                              --\n");
	printf("           ----------------------------------------------------\n");

	printf("Enter 'F'or'f' to input information from file.\n\
If you want to enter it manually enter any key to continue:");
	scanf("%c", &select);
	printf("\nPlease enter a num to select algorithm:");
	scanf("%s", &option);
	int len=strlen(option);
	for (int i = 0; i < len; i++)
	{
		choose(option[i], select);
	}
}

