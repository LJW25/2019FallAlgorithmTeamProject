#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "stackqueue.h"
#define DAYS 31										//Day�Դϴ�
#define SPEED (500.0)								// ������� �ӷ��Դϴ� 500km/hour


typedef struct point {
	char name;
	int x, y;
	int distance, needTime, departureTime;			// ������ distance�̿ܿ� needtime�� departureTime, paths[26], pathsNum�� �߰��Ǿ����ϴ�.
	struct point* next;								//distance�� adjency list�� edge�κп����� Ȱ��ȭ�Ǹ�, default���� -1�̰�, addpath�� ��Ͻ� �� city�� �Ÿ��� ��ϵ˴ϴ�.
	char paths[26];									//needTime�� ���̽�Ʈ�� �˰����� �ش� city���� �ʿ��� cost�μ�, ��λ��� city�� departureTime�� distance�� ���� ����˴ϴ�.
	int pathsNum;									//paths[26]�� �ش� city���� �ִܰŸ��� ���� city�� ����Ʈ�Դϴ�. pathsNum�� paths[26]�� ���̸� ��Ÿ���ϴ�
}City;
typedef struct a {
	int num;
	City** heads;
}Graph;

typedef struct {
	char name[20],source, destination;
	int departureTime, departureDate, arrivalTime, arrivalDate, flightTime;
	char flightPath[30];
}output;

Graph* CreateGraph(int num);						// num���� city�� �ִ� �⺻ �׷����� �����մϴ�.
void DestroyGraph(Graph* pgraph);					// �׷��� �ϳ��� ���� �޸𸮿� ��ȯ�մϴ�.
void AddPath(Graph* pgraph, char src, char dest);	//pgraph���� 2�� city���� bidriectional edge�� 1�� �����մϴ�
void AddPaths(Graph* pgraph, int num);				//pgraph���� num����ŭ �����ϰ� AddPath�� �����մϴ�.
int PathExists(Graph* pgraph, char src, char dst);  // AddPath���� Ȱ��Ǵ� �Լ��Դϴ�. ���� �׷����� ���� path�� �����Ǿ��ִ��� Ȯ���մϴ�.
Graph** CreateTimeTable(Graph* pgraph);				//�⺻ �׷��� pgraph�� departure time�� �Ҵ��� 31���� �׷����� ����ϴ�.
void PrintGraph(Graph* pgraph);						//1�� graph�� ������ ����մϴ�.
City* ShortestPath(Graph* pgraph, int src, int dst);	// pgraph���� ������ ����� �� ���� �������ø� ��ȯ�մϴ�. 
			//���⼭ needTime�� �����ϸ� ���������ð�, paths�� �����ϸ� ��λ�city�� ���array, pathsNum�� pathsArray�� ���̸� ��Ÿ���ϴ�.
City* ExtractMin(City** Q, int* qNum);				// ShortestPath�������� ���̴� �Լ��μ� �����ڵ��� ExtractMin�� �����մϴ�.
void PrintArray(char* arr);			// char array�μ�.
void PrintResult(output *values);
output* module(char* name, char source, char destination, int date);

Graph *pgraph, ** pgraphs;
int main() {
	srand(time(NULL));
	pgraph = CreateGraph(26);			//26�� city�� �����մϴ�.
	AddPaths(pgraph, 100);						//�ش� 26�� city���� path�� �������� 100���� �����մϴ�.
	pgraphs = CreateTimeTable(pgraph);	//���� ������ path�� ������ departureTime�� �ο��� graph 31���� �����մϴ�.

	//from here//
	char name[20]="james",src,dst, day;
	printf("Graph project\n");
	/*printf("Enter the name: "); scanf("%s", &name); getchar();
	printf("Enter the day: "); scanf("%d", &day); getchar();
	printf("Enter the source city (a~z): "); scanf("%c", &src); getchar();
	printf("Enter the destination city (a~z): "); scanf("%c", &dst); getchar();*/
	day = 31; src = 'e'; dst = 'f';

	output *values = module(name, src, dst, day);
	PrintResult(values);
	//to here//

	DestroyGraph(pgraph);						//�޸�����
	for (int i = 0; i < DAYS; i++) DestroyGraph(pgraphs[i]);
	return 0;
}
void PrintResult(output* values) {
	if (values == NULL) {
		printf("no path.");
		return;
	}
	printf("name: %s source: %c destination: %c \n", values->name, values->source, values->destination);
	printf("departureTime: %3dh %3dm departureDate:%3d\n", values->departureTime / 60, values->departureTime % 60, values->departureDate);
	printf("arrivalTime  : %3dh %3dm arrivalDate:%3d\n", values->arrivalTime / 60, values->arrivalTime % 60, values->arrivalDate);
	printf("flightTime   : %3dh %3dm flightPath: ", values->flightTime / 60, values->flightTime % 60);
	PrintArray(values->flightPath);
}
output* module(char* name,char source, char destination, int date) {
	output* result = (output*)malloc(sizeof(output));
	char src = source, dst= destination;
	int day = date; 
	for (int i = 0; i < 20; i++) result->name[i] = name[i];
	

	
	//PrintGraph(pgraphs[day - 1]);						// 3+1=4������ depatureTime�� ��� �׷����� ����մϴ�.
	City* srcCity = pgraphs[day - 1]->heads[src - 'a'];
	//printf("%d���ڿ� %c���ÿ��� �̵������� ������ ������ �Ʒ��� �����ϴ�.(���� 0�ñ��� �����ýð�, ���ľ��ϴ� ���õ�)\n", day, src);
	//printf("departure time and date: %dh %dm %dday,", srcCity->departureTime / 60, srcCity->departureTime % 60, day);
	City* dstCity = ShortestPath(pgraphs[day - 1], src, dst);
	if (dstCity->needTime > 0) {				//����� ã�� ��
		//printf("%c to %c : %2dh %2dm ", src, dst, dstCity->needTime / 60, dstCity->needTime % 60);
		//PrintArray(dstCity->paths, dstCity->pathsNum);
		result->source = src;
		result->destination = dst;
		for (int i = 0; i < dstCity->pathsNum; i++) result->flightPath[i] = dstCity->paths[i];
		result->flightPath[dstCity->pathsNum] = '\0';
		City* adj = pgraphs[day - 1]->heads[src - 'a'];
		while (adj->next != NULL) {
			adj = adj->next;
			if (adj->name == dstCity->paths[1]) {
				result->departureTime = adj->departureTime;
				result->departureDate = day;
				break;
			}
		}
		result->arrivalTime = dstCity->needTime;
		result->arrivalDate = day;
		result->flightTime = (result->arrivalTime) - (result->departureTime);
		if (result->arrivalTime / 60 >= 24) {
			result->arrivalTime -= 24 * 60;
			result->arrivalDate++;
		}
	}
	else result = NULL;
	
	return result;
}
void PrintArray(char* arr) {
	printf("(");
	int i = 0;
	while(arr[i]!='\0'){
		printf("%c ", arr[i++]);
	}
	printf(")\n");
}
Graph* CreateGraph(int num) {
	Graph* pgraph = (Graph*)malloc(sizeof(Graph));
	City* head;
	if (NULL == pgraph) return NULL;
	pgraph->num = num;
	pgraph->heads = (City * *)malloc(sizeof(City*) * num * 2);
	if (NULL == pgraph->heads) return NULL;
	for (int i = 0; i < num; i++) {
		head = (City*)malloc(sizeof(City));
		if (NULL == pgraph->heads || NULL == head) return NULL;
		(pgraph->heads)[i] = head;
		head->name = 'a' + i;
		head->next = NULL;
		head->x = (rand() % 6001) - 3000;
		head->y = (rand() % 6001) - 3000;
		head->distance = -1;
		head->needTime = -1;
		head->departureTime = -1;
	}
	return pgraph;
}
void DestroyGraph(Graph* pgraph) {
	City* cur, * tmp;
	for (int i = 0; i < pgraph->num; i++) {
		cur = pgraph->heads[i];
		while (cur != NULL) {
			tmp = cur;
			cur = cur->next;
			free(tmp);
		}
	}
	free(pgraph->heads);
	free(pgraph);
	return;
}
void AddPath(Graph* pgraph, char src, char dest) {
	if (PathExists(pgraph, src, dest) == 1) {
		//printf("Path exists!\n");
		return;
	}
	City* newCity1, * newCity2, * head1, * head2, * cur;
	int distance;
	head1 = pgraph->heads[dest - 'a'];
	head2 = pgraph->heads[src - 'a'];
	distance = (head1->x - head2->x) * (head1->x - head2->x) +
		(head1->y - head2->y) * (head1->y - head2->y);//blabla
	distance = (int)(sqrt(distance));

	newCity1 = (City*)malloc(sizeof(City));
	newCity2 = (City*)malloc(sizeof(City));
	if (NULL == newCity1 || NULL == newCity2) return;
	*newCity1 = *head1;
	*newCity2 = *head2;
	newCity1->next = NULL;
	newCity2->next = NULL;
	newCity1->distance = distance;
	newCity2->distance = distance;
	newCity1->needTime = -1;
	newCity2->needTime = -1;

	cur = head1;
	while (cur->next != NULL) cur = cur->next;
	cur->next = newCity2;
	cur = head2;
	while (cur->next != NULL) cur = cur->next;
	cur->next = newCity1;
}
void AddPaths(Graph* pgraph, int num) {
	City* cur;
	int src, dst;
	for (int i = 0; i < num; i++) {
		src = rand() % 26 + 'a';
		dst = rand() % 26 + 'a';
		while (src == dst) {
			dst = rand() % 26 + 'a';
		}
		AddPath(pgraph, src, dst);
	}
}
int PathExists(Graph* pgraph, char src, char dst) {
	int result = 0;
	City** heads = pgraph->heads, * cur;
	cur = heads[src - 'a'];
	while (cur->next != NULL) {
		//printf("hi");
		cur = cur->next;
		//printf("%c", cur->name);
		if (cur->name == dst) {
			result = 1;
			break;
		}
	}
	//printf("\n");
	return result;
}
Graph** CreateTimeTable(Graph* pgraph) {
	Graph** graphs = (Graph * *)malloc(sizeof(Graph*) * DAYS);
	City** heads1, ** heads2, * cur1, * cur2, * new;
	//
	for (int i = 0; i < DAYS; i++) {
		heads1 = pgraph->heads;
		heads2 = (City * *)malloc(sizeof(City*) * (pgraph->num));
		if (NULL == graphs) return;
		graphs[i] = (Graph*)malloc(sizeof(Graph));
		graphs[i]->heads = heads2;
		graphs[i]->num = pgraph->num;
		for (int j = 0; j < pgraph->num; j++) {
			cur1 = heads1[j];
			cur2 = (City*)malloc(sizeof(City));
			heads2[j] = cur2;
			*(cur2) = *(cur1);
			cur2->next = NULL;
			while (cur1->next != NULL) {
				cur1 = cur1->next;
				cur2->next = (City*)malloc(sizeof(City));
				cur2 = cur2->next;
				*(cur2) = *(cur1);
				cur2->next = NULL;
				cur2->departureTime = rand() % (24 * 60);
			}
		}
	}
	return graphs;
}
void PrintGraph(Graph* pgraph) {
	printf("City|x-cor|y-cor|distance|departureTime|arriveTime\n");
	City* cur, ** heads = pgraph->heads;
	int arrive;
	for (int i = 0; i < 26; i++) {
		printf("==================================================\n");
		cur = heads[i];
		printf("%3c |%5d|%5d|\n\n", cur->name, cur->x, cur->y);
		while (cur->next != NULL) {
			cur = cur->next;
			arrive = cur->departureTime + (int)(cur->distance / SPEED * 60);
			printf("%3c |%5d|%5d|%8d| %4dh %4dm | %3dh %3dm\n", cur->name, cur->x, cur->y,
				cur->distance, cur->departureTime / 60, cur->departureTime % 60, arrive / 60, arrive % 60);
		}
	}
	printf("==================================================\n");
	return;
}
City* ExtractMin(City** Q, int* qNum) {
	City* result = NULL;
	int min = (1 << 31) - 1, mindex = -1;
	for (int i = 0; i < *qNum; i++) {
		if (Q[i]->needTime == -1) continue;
		if (min > (Q[i]->needTime)) {
			min = Q[i]->needTime;
			mindex = i;
		}
	}
	if (mindex == -1) return NULL;
	//deleting
	for (int i = 0; i < *qNum; i++) {
		if (i == mindex) {
			result = Q[i];
			for (int j = i; j < (*qNum) - 1; j++) {
				Q[j] = Q[j + 1];
			}
			break;
		}
	}
	//printf("%c ", result->name);
	(*qNum)--;
	return result;
}
City* ShortestPath(Graph* pgraph, int src, int dst) {
	if (src == dst) {
		printf("source can't equal destination.");
		return -1;
	}
	char oldPaths[20] = { 0, };
	City** heads = pgraph->heads, ** Q, * u = NULL, * adj, * v;
	Q = (City * *)malloc(sizeof(City*) * pgraph->num);
	int qNum = pgraph->num, pathTime, oldNeedTime, oldPathsNum, arrive;
	for (int i = 0; i < pgraph->num; i++) {
		if (i == src - 'a') {
			heads[i]->needTime = 0;
			heads[i]->pathsNum = 1;
			heads[i]->paths[0] = src;
		}
		else {
			heads[i]->needTime = -1;
			heads[i]->pathsNum = 0;
		}

	}
	Queue* S = CreateQueue();
	for (int i = 0; i < pgraph->num; i++) Q[i] = heads[i];
	while (qNum > 0) {
		u = ExtractMin(Q, &qNum);
		if (u == NULL) break;
		EnQueue(S, u);
		if (u->name == dst) break;
		adj = u;
		while (adj->next != NULL) {
			adj = adj->next;
			v = heads[adj->name - 'a'];
			arrive = adj->departureTime + (int)(adj->distance / SPEED * 60);
			//pathTime += (adj->departureTime - u->needTime);// waiting Time ��߽ð��� �ҿ�ð������������� ���� �Ʒ����ɷ���
			oldNeedTime = v->needTime;
			for (int i = 0; i < v->pathsNum; i++) {
				oldPaths[i] = v->paths[i];
			}
			oldPathsNum = v->pathsNum;
			if (v->needTime == -1 || v->needTime > arrive) {
				v->needTime = arrive;
				for (int i = 0; i < u->pathsNum; i++) v->paths[i] = u->paths[i];
				v->paths[u->pathsNum] = v->name;
				v->pathsNum = u->pathsNum + 1;
			}
			if (u->needTime > adj->departureTime) {//���⿡�� �ɷ���
				v->needTime = oldNeedTime;
				for (int i = 0; i < v->pathsNum; i++) v->paths[i] = oldPaths[i];
				v->pathsNum = oldPathsNum;
			}
		}
	}
	free(S);
	//if (u != NULL) printf("\n");
	//for (int i = 0; i < heads[dst - 'a']->pathsNum; i++) printf("%c ", heads[dst - 'a']->paths[i]);
	//result = heads[dst - 'a']->needTime;
	heads[dst - 'a']->paths[heads[dst - 'a']->pathsNum] = '\0';
	return heads[dst - 'a'];
}