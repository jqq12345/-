#define MaxNum 100
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<limits.h>
//1.����ҵ�����㷨
struct Process_struct {
	int  Number;                 //���̱��
	char Name[MaxNum];           //��������
	int  ArrivalTime;    //����ʱ��
	int  ServiceTime;    //��ʼ����ʱ��
	int  FinishTime;     //���н���ʱ��
	int  WholeTime;      //����ʱ��
	int run_flag;        //���ȱ�־
	int order;           //���д���
	double  WeightWholeTime;        //��תʱ��
	double AverageWT_FCFS, AverageWT_SJF;    //ƽ����תʱ��
	double AverageWWT_FCFS, AverageWWT_SJF;  //ƽ����Ȩ��תʱ��
}Process[MaxNum];

//�ҵ���ǰ���������ʱ����̵Ľ���
int N;    //ʵ�ʽ��̸���
int SJF() {       //����ҵ�����㷨
	int temp_time = 0;    //������ʱ��
	int i = 0, j;
	int number_schedul=0, temp_counter;      //���̱�ţ���ǰ��ִ�н��̸���
	float run_time= INT_MAX;
	temp_counter = 0;
	while (temp_counter < N)
	{
		int matime = INT_MAX;//���ø���ʱ������ֵΪ��������ڱȽ�Ѱ�����ȵ���Ľ���
		for (j = 0; j < N; j++)
		{
			//�������ʱ��С�ڵ�ǰʱ�䣬�ҽ���δ���
			if ((Process[j].ArrivalTime <= temp_time) && (!Process[j].run_flag))
			{//����ҵ�һ������ʱ��С�ڵ�ǰʱ��Ľ��̣���ѡ��ý��̣���һ�����У���Ϊ�п��ܴ���ͬʱ���ﵫ�����������ҵʱ��̵Ľ���
				run_time = Process[j].WholeTime;
				number_schedul = j;
				Process[number_schedul].ServiceTime = temp_time; //��ʼ����ʱ���ǵ�ǰʱ��
				break; //ֻ��Ҫ��һ�ξ���
			}
			else {
				//���û���ڵ�ǰʱ��֮ǰ����Ľ��̣����ھ����������ҵ���ʱ������Ľ���
				if (Process[j].ArrivalTime < matime && (!Process[j].run_flag))
				{
					matime = Process[j].ArrivalTime;
					run_time = Process[j].WholeTime;
					number_schedul = j;
					Process[number_schedul].ServiceTime = Process[j].ArrivalTime; //��ʼ����ʱ���ǵ���ʱ��
				}
			}
		}
		for (j = 0; j < N; j++)
		{
			//�������ʱ��С�ڵ�ǰʱ�䣬�ҽ���δ���
			if ((Process[j].ArrivalTime <= temp_time) && (!Process[j].run_flag))
				//�����б�׼�����еĽ�������ʱ����̵Ľ��̣���ѡ���������ʱ����̽�������
			{
				if (Process[j].WholeTime < run_time)
				{
					run_time = Process[j].WholeTime;
					number_schedul = j;
					Process[number_schedul].ServiceTime = temp_time; //��ʼ����ʱ���ǵ�ǰʱ��
				}
			}
			else {//��ͬʱ����Ľ������ҵ���ҵʱ����̵�һ��
				if (Process[j].ArrivalTime == matime && (!Process[j].run_flag)&& Process[j].WholeTime < run_time)
				{
					matime = Process[j].ArrivalTime;
					run_time = Process[j].WholeTime;
					number_schedul = j;
					Process[number_schedul].ServiceTime = Process[j].ArrivalTime; //��ʼ����ʱ���ǵ���ʱ��
				}

			}

		}
		
		//���ҵ�����һ�������ȵĽ�������Ӧ�Ĳ���
		
		Process[number_schedul].FinishTime = Process[number_schedul].ServiceTime + Process[number_schedul].WholeTime;
		Process[number_schedul].run_flag = 1;
		temp_time = Process[number_schedul].FinishTime;
		Process[number_schedul].order = temp_counter;
		temp_counter++;
	}return 0;
}
int Pinput()   //���̲�������
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
int Poutput()   //���Ƚ�����
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



//2.�����ȷ����㷨
typedef struct PCB      //������̿��ƿ�
{
	char name[10];   //������
	char state;   //����״̬
	int  arrivetime;  //����ʱ��
	int  starttime;  //���̿�ʼʱ��
	int  finishtime;  //���̽���ʱ��
	int  servicetime;   //����ʱ��
	float turnaroundtime;//��תʱ��
	float weightedturnaroundtime;//��Ȩ��תʱ��
	struct PCB* next;  //ָ���¸�����
}pcb;

int time=0;   //��ʱ��
int n;    //���̸���
float avg_around_time = 0;  //ƽ����תʱ��
pcb* head1 = NULL, * p, * q,*min; //��������ָ��
void run_fcfs(pcb* p1) //����δ��ɵĽ���
{
	time =( p1->arrivetime > time ? p1->arrivetime : time);
	p1->starttime = time;
	printf("\nNow the time is %d,Start running job %s\n", time, p1->name);
	time += p1->servicetime;   //��ʱ��
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
void fcfs() //���ȵ��ȷ���
{
	int i, j;
	for (i = 0; i < n; i++) //���������ҵ�����˳��
	{
		p = head1;
		q = p; //��ǵ�ǰ�Ľ���
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
				//F��ʾ�Ѿ����Finish״̬��W-wait,R-run
			{
				min = p; //�ҵ����ȵ���Ľ���
			}
	    }
		run_fcfs(min);
	}
	printf("average_turn_round_time=%f\n", avg_around_time / n);
}
void getInfo()          //��ý�����Ϣ����������
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


//3.ʱ��Ƭ��ת�㷨
typedef struct node
{
	char name[20];
	int round;
	int cputime;   //�����ܹ�ʹ��cpu��ʱ�䣬�����̵ķ���ʱ��
	int starttime;  //���̿�ʼ����ʱ��
	int arrivetime; //���̵����ڴ��ʱ��
	int finishtime; //�������ʱ��
	int needtime;  //���̵�����ʱ��
	char state;  //����״̬
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


void GetFirst()  /*ȡ�õ�һ���������нڵ�*/
{
	run1 = ready;

	if (ready != NULL)
	{
		run1->state = 'R'; //R--run
		ready = ready->next;
		run1->next = NULL;
	}
}
void Output()    /*���������Ϣ*/
{
	PCB* p;
	p = ready;  //����������Ϣ
	printf("Time  Name cpu_t Need_Time status Counter\n");
	while (p != NULL)
	{
		printf("%2d-%-2d %-5s%-6d%-10d%-7c%d\n",p->starttime,p->finishtime, p->name, p->cputime, p->needtime, p->state, p->count);
		p = p->next;
	}
	p = finish; //��ɶ�����Ϣ
	while (p != NULL)
	{
		printf("%2d-%-2d %-5s%-6d%-10d%-7c%d\n", p->starttime, p->finishtime, p->name, p->cputime, p->needtime, p->state, p->count);
		p = p->next;
	}
	p = run1;  //���ж�����Ϣ
	while (p != NULL)
	{
		printf("%2d-%-2d %-5s%-6d%-10d%-7c%d\n", p->starttime, p->finishtime, p->name, p->cputime, p->needtime, p->state, p->count);
		p = p->next;
	}
}

void InsertTime(PCB* in)  /*�����̲��뵽��������β��*/
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
void InsertFinish(PCB* in)  /*�����̲��뵽��ɶ���β��*/
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
void TimeCreate() /*ʱ��Ƭ���뺯��*/
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
void RoundRun()    /*ʱ��Ƭ��ת�����㷨*/
{
	int flag = 1,flag2=1;
	int time = 0; //��ǰʱ��
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
				run1->finishtime = run1->starttime + run1->count;//��¼ÿ�����̵�ʵ�ʽ���ʱ��
				InsertFinish(run1);  //�ڷ����ʱ��Ƭ���������
				flag = 0;
			}
			else if (run1->count == run1->round) //ʱ��Ƭ����
			{
				run1->state = 'W';
				run1->finishtime = run1->starttime + run1->round;
				run1->count = 0;
				InsertTime(run1); //�����ٴλص���������
				flag = 0;
			}
		}
		if(run1->state=='F')
		{
			avg_around_time += run1->finishtime - run1->arrivetime;
		}
		time = run1->finishtime;
		flag = 1; //�����ж�һ�������Ƿ����
		GetFirst();
	}
	Output();
	printf("average_turn_round_time=%f\n", avg_around_time / num);
}






//4.��̬����Ȩ�����㷨

int SequenceNumber = 1;	//���̱��
int ProcessAmount;		//��������
int StartCount = 0;		//�����ڴ���̼���

struct PCB1 {				//���̿��ƿ�
	int No;				//���̺�
	char name[16];		//������
	int enterMemoryTime;//�����ڴ�ʱ��
	int serviceTime;	//����ʱ��
	int priority;		//���ȼ�
	struct PCB1* next;
};
struct PCB1* head, * thisP, * newP;		//��̬���ս��̼���ϸ

struct PCB1* readyHead;				//��̬�洢��������

//��������
int timeOfData(FILE*);
void ListAllPCB(struct PCB1* h);
void release(struct PCB1*);
void releaseR(struct PCB1*);
void setNewPCB(char*, int, int, int);
struct PCB1* runTheProcess(int i);

void newPCB() {			//����PCB
	newP = (struct PCB1*)malloc(sizeof(struct PCB1));

	if (head == NULL) {		//�ж�ͷ�ڵ��Ƿ�Ϊ��
		head = newP;		//Ϊ�գ�ͷ�ڵ�ָ���¿��ٵ��ڴ�
	}
	else {
		thisP = head;
		while (thisP->next != NULL) {
			thisP = thisP->next;
		}
		thisP->next = newP;		//�����������ҵ����һ��Ԫ��
	}

	thisP = newP;
	//�����½ڵ�
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
void buildProcess() {		//��������
	int i = 0;
	printf("Enter the number of processes:");
	scanf("%d", &ProcessAmount);
	while (i < ProcessAmount) {
		newPCB();
		i++;
	}
}
void readyList(struct PCB1* pcb) {		//���������ʽ������������
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
struct PCB1* callMemory() {		//�����ڴ棬���ص����ڴ������ڵ�
	int at;		//����ʱ��
	struct PCB1* markP;
	if (head == NULL) {
		printf("Program not found.\n");
	}
	else {
		markP = thisP = head;		//���ָ��ͷ�ڵ�
		at = thisP->enterMemoryTime;	//����ʱ��Ϊͷ�ڵ㵽��ʱ��
		while (thisP->next != NULL) {	//����һ�ڵ㲻Ϊ��
			if (at > thisP->next->enterMemoryTime) {	//�жϵ�ǰʱ���Ƿ������һ�ڵ�ʱ��
				markP = thisP->next;			//�ǣ���Ǵ˽ڵ�
				at = markP->enterMemoryTime;	//����ʱ�����Ϊ���ʱ��
			}
			thisP = thisP->next;	//������
		}
	}
	return markP;
}
int timesetout(int at)  //�жϾ����������Ƿ����ڵ�ǰʱ��֮ǰ����Ľ���
{
	struct PCB1* markThis;
	if (readyHead == NULL) {
		printf("There are no runnable processes.\n");
	}
	else {
		markThis = thisP = readyHead;
		time = thisP->enterMemoryTime;//��ǰ����ʱ��
		while (thisP->next != NULL) {
			if (thisP->enterMemoryTime < at)
				return 1; //��ʾ���ڵ�ǰʱ��֮ǰ����Ľ���
			thisP = thisP->next;
		}
		return 0;
	}
}
void run() {		//��̬���ȼ��㷨ִ�С�
	struct PCB1* temp;	//��ʱ�ڵ������洢�����ڴ�ڵ�
	struct PCB1* runPro;	//��������ִ�еĽڵ�
	int i;		//ѭ����ʼ����
	int at=0, srt, finishtime = 0;	//��ǰʱ�䣬��ʼִ��ʱ�䣬����ʱ�䡣
	int turnOverTime;		//��תʱ��
	float avg_around_time = 0;
	double ptot;			//��Ȩ��תʱ��
	if (head == NULL) {
		printf("No processes found.\n");
	}
	else {
		while (head != NULL) { //�����н��̶��ŵ�����������ȥ
			temp = callMemory(); //���ص�ǰ���Ƚ����ڴ�Ľ���
			readyList(temp);	//��ʼ��������б�
			release(temp);		//�ͷŽ���
		}//ѭ�����֮�󣬾��������еĽ��̰��ս����ڴ��ʱ����С��������


		for (i = 0; i < ProcessAmount; i++) {	//ѭ�����̵�������
			runPro = runTheProcess(timesetout(at));  //1.���ؾ��������е�ǰ���Ƚ����ڴ�����ͬʱ�䵽��Ľ���������Ȩ��ߵĽ���
			//2.���ؾ������������ȼ���ߵĽ���
			if (runPro->enterMemoryTime > at) {
				//��ʼ�������ʼִ��ʱ�䣬���ʱ�䣬��תʱ�䣬��Ȩ��תʱ��
				srt = runPro->enterMemoryTime;  //��ʼʱ����ڵ���ʱ��
			}
			else {		//��ʼ֮�󣬼����ʱ�估��ת
				srt = at;//��ʼʱ����ڵ�ǰʱ��
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
struct PCB1* runTheProcess(int i) {	//ִ�еĽ��̣�����Ҫִ�н��̵Ľڵ�
	int time, pri;
	struct PCB1* markThis;	//���Ҫ���صĽڵ�
	if (readyHead == NULL) {
		printf("There are no runnable processes.\n");
	}
	else {
		markThis = thisP = readyHead;
		time = thisP->enterMemoryTime;//��ǰ����ʱ��
		pri = thisP->priority;	//��ǰ���̵����ȼ�
		while (thisP->next != NULL) {
			if (i == 0)
			{
				if (time == thisP->next->enterMemoryTime) {//�ж�֮��ڵ�ĵ���ʱ���Ƿ��뵱ǰһ��
					if (pri < thisP->next->priority) {//һ�£����ж����ȼ�����
						pri = thisP->next->priority;//�������ȼ��ж�����Ϊ��һ���ȼ�
						markThis = thisP->next;	//��ǵ�ǰ�ڵ����һ�ڵ�
					}
					else {
						break;
					}
				}
			}
			else {
				if (pri < thisP->next->priority) {//�ж����ȼ�����
					pri = thisP->next->priority;//�������ȼ��ж�����Ϊ��һ���ȼ�
					markThis = thisP->next;	//��ǵ�ǰ�ڵ����һ�ڵ�
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
void release(struct PCB1* pcb) {		//ɾ��ԭ������Ľ��̽ڵ�
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
void releaseR(struct PCB1* pcb) {		//ɾ�������б��н��̽ڵ�
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
void ListAllPCB(struct PCB1* h) {		//��ӡ���н���
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





//5.����Ӧ�ȵ����㷨
int N;
float timecounter5 = 0; //��ʱ��
struct zgxyb* c;
struct zgxyb {
	char name[10];
	float arrivetime;
	float servicetime;
	float starttime;
	float finishtime;
	float around_time; //��תʱ��
	float zztime;//�ȴ�ʱ��
	float dqzztime; //���̵���Ӧ��
	char stats; //����״̬
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
//������ʱ������
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
	for (int n = 1; n < N; n++) //����������̵���Ӧ��
	{
		if (p[n].stats == 'W')
		{
			p[n].zztime = timecounter5 - p[n].arrivetime; //���̵ĵȴ�ʱ��=��ǰʱ��-����ʱ��
			p[n].dqzztime = 1 + p[n].zztime / p[n].servicetime;
		}
	}
}
void ZGXYB(struct zgxyb* p, int N)
{
	int m, i, n, follow, k;
	int schedul=0; //��ǰʱ��
	float max=0,avg_around_time=0;
	struct zgxyb temp;
	sort(p,N);  //������ʱ��Խ��̽�������
	for(i=0;i<N;i++) //����N������
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
			
			calculate(p); //�����������Ӧ��
			for (m = 1; m < N; m++)  //ѡ��һ��δ��ɽ���
			{
				if (p[m].stats == 'W')
				{
					max = p[m].dqzztime;
					schedul = m;
					break;
				}
			}
			for (int k = 1; k < N; k++)  //ѡ����Ӧ����ߵĽ���
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

//���ļ���ȡ����
/*���ݸ�ʽ����

������  ����ʱ�� ����ʱ�� ���ȼ� ʱ��Ƭ
p1 0 7 1 5
p2 2 4 2 3
p3 5 4 2 3
*/


//1.����ҵ����
void SJFsetfin(char* name, int entime, int sertime,int i)   //���̲�������
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
//2.�����ȷ���
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
//3.ʱ��Ƭ��ѯ
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
//4.�����Ӧ��
void ZGXYBsetin(struct zgxyb* p,char* name, float entime, float sertime,int i)
{
	strcpy(p[i].name, name);
	p[i].arrivetime = entime;
	p[i].servicetime = sertime;
	p[i].stats = 'W';
}
//5.��̬���ȼ�
void setNewPCB(char* name, int pri, int entime, int sertime) {//��ֵ����PCB�����ڴ��ı��л�ȡ����

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
	ProcessAmount =N=n=num=times - 2;  //���̸���
	while (i < times - 1) {  //ѭ������=�ı�����
		if (i == 0) {	//�������������ı���ʽ��һ�еĺ��֡�
			fgets(temp, sizeof(temp), fp); //�Ѷ�ȡ�ĵ�һ�е����ݷ���temp������
		}
		else {		//�����������ս�������
			fscanf(fp, "%s", name);
			fscanf(fp, "%f", &emt);
			fscanf(fp, "%f", &st);
			fscanf(fp, "%d", &pri);
			fscanf(fp, "%f", &slice);
			switch (option)
			{
			case 1:
				SJFsetfin(name, emt, st, i-1);  //���һ�����̵�����������
				break;
			case 2:
				RRsetfin(name, emt, st, slice);//���һ������
				break;
			case 3:
				FCFSsetfin(name, emt, st);//���һ������
				break;
			case 4:
				ZGXYBsetin(a, name, emt, st, i-1);//���һ������
				break;
			case 5:
				setNewPCB(name, pri, emt, st);//���һ������
				break;
			default:
				break;
			}
		}
		i++;
	}
	fclose(fp);
}
int timeOfData(FILE* f) {	//��ȡ���ı��л�ȡ���ݵĽ�������
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
//������
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

