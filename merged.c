#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "stackqueue.h"
#define DAYS 31                              //Day입니다
#define SPEED (500.0)                        // 비행기의 속력입니다 500km/hour


typedef struct point {
	char name;
	int x, y;
	int distance, needTime, departureTime;        // 기존의 distance이외에 needtime과 departureTime, paths[26], pathsNum이 추가되었습니다.
	struct point* next;                        //distance는 adjency list의 edge부분에서만 활성화되며, default값은 -1이고, addpath로 등록시 두 city간 거리가 기록됩니다.
	char paths[26];                           //needTime은 다이스트라 알고리즘의 해당 city까지 필요한 cost로서, 경로상의 city와 departureTime과 distance에 의해 산출됩니다.
	int pathsNum;                           //paths[26]은 해당 city까지 최단거리로 오는 city의 리스트입니다. pathsNum은 paths[26]의 길이를 나타냅니다
}City;

typedef struct a {
	int num;
	City** heads;

}Graph;

typedef struct {
	char name[20], source, destination;
	int departureTime, departureDate, arrivalTime, arrivalDate, flightTime;
	int level;
	char flightPath[30];

}Passenger;

typedef struct Node {
	int key;//= reservation number
	int color;
	struct Node * parent;
	struct Node * leftc;
	struct Node * rightc;
	Passenger * passenger;
}node;


//RBT==============================================
node* makenode(int key);
node* isexist(node* root, int key);
void Lrotate(node* root, node* cur);
void Rrotate(node* root, node* cur);
void insert_fixup(node* root, node* cur);
node* insert(node* root, node* nod);
node* minnode(node* cur);
node* successor(node* cur);
void delete_fixup(node* root, node* cur);
node* delet(node* root, node* nod);

void printreserve(node* root);

int height(node* root);

//Graph============================================

Graph* CreateGraph(int num);                  // num개의 city가 있는 기본 그래프를 생성합니다.
void DestroyGraph(Graph* pgraph);               // 그래프 하나를 전부 메모리에 반환합니다.
void AddPath(Graph* pgraph, char src, char dest);   //pgraph내의 2개 city간에 bidriectional edge를 1개 생성합니다
void AddPaths(Graph* pgraph, int num);            //pgraph내에 num개만큼 랜덤하게 AddPath를 실행합니다.
int PathExists(Graph* pgraph, char src, char dst);  // AddPath에만 활용되는 함수입니다. 기존 그래프에 동일 path가 생성되어있는지 확인합니다.
Graph** CreateTimeTable(Graph* pgraph);            //기본 그래프 pgraph에 departure time을 할당한 31가지 그래프를 만듭니다.
void PrintGraph(Graph* pgraph);                  //1개 graph의 내용을 출력합니다.
City* ShortestPath(Graph* pgraph, int src, int dst);   // pgraph내에 내용을 기록한 후 최종 목적도시를 반환합니다. 
													   //여기서 needTime을 참조하면 도착예정시간, paths를 참조하면 경로상city가 담긴array, pathsNum은 pathsArray의 길이를 나타냅니다.
City* ExtractMin(City** Q, int* qNum);            // ShortestPath내에서만 쓰이는 함수로서 수도코드의 ExtractMin과 동일합니다.
void PrintArray(char* arr);         // char array인쇄.
void PrintResult(Passenger *values);
Passenger* module(char* name, char source, char destination, int date, int level);

Graph *pgraph, ** pgraphs;


int main() {
	srand(time(NULL));

	int num, day;
	char name[20] = { '\0' };
	char src, dst;
	int reservationNumber;
	int seat, level;
	int h = 0;

	node* temp = NULL;
	node* root = NULL;
	Passenger *values = NULL;
	node** latest = (node* *)malloc(sizeof(node*));

	pgraph = CreateGraph(26);         //26개 city를 생성합니다.
	AddPaths(pgraph, 100);                  //해당 26개 city간에 path를 무작위로 100개를 생성합니다.
	pgraphs = CreateTimeTable(pgraph);   //위에 생성된 path에 무작위 departureTime을 부여한 graph 31개를 생성합니다.

										 //from here//

										 //printf("Graph project\n");
										 /*printf("Enter the name: "); scanf("%s", &name); getchar();
										 printf("Enter the day: "); scanf("%d", &day); getchar();
										 printf("Enter the source city (a~z): "); scanf("%c", &src); getchar();
										 printf("Enter the destination city (a~z): "); scanf("%c", &dst); getchar();*/
										 //day = 31; src = 'e'; dst = 'f';

	int length;

	printf("Added 500 reservations\n");
	printf("Latest 10 Reservations------------------------\n");

	for (num = 0; num < 500; num++) {

		length = rand() % 4 + 3;		//length of name
		name[0] = rand() % 26 + 65;		//first character of name(Cap)
		for (int i = 1; i < length; i++) {
			name[i] = rand() % 26 + 97;		//orthrt characters of name
		}

		day = rand() % 31 + 1;
		src = rand() % 26 + 97;
		dst = rand() % 26 + 97;
		while (dst == src) dst = rand() % 26 + 97;

		//확률에 따라 grade 배정
		seat = rand() % 100;
		if (seat < 4) level = 1;
		else if (seat < 11) level = 2;
		else level = 3;

		//예약번호 - 등급/예약수/날짜/출발지/도착지 10자리 수
		//reservationNumber = level * 1000000000 + num * 1000000 + day * 10000 + src * 100 + dst;

		values = module(name, src, dst, day, level);         //여기에 name, source, destination, date를 입력하면!!
															 //PrintResult(values);  //name, source, destination, departure time, departure date, arrival time, arrival date, flight path, flight time을 반환합니다.

		//temp = makenode(reservationNumber);
		temp = makenode(num);
		temp->passenger = values;
		root = insert(root, temp);

		if (num >= 490) printreserve(temp);
	}


	//input
	int rnum;
	int choice = 0;
	char yn;
	while (1) {

		printf("\nWhat do you want to do?\n");
		printf("1. Reservation\n");
		printf("2. Cancel\n");

		scanf("%d", &choice);

		switch (choice) {
		case 1:
			printf("==================== Reservation ======================= \n");
			printf("Enter the name: "); scanf("%s", &name); getchar();
			printf("Enter the day: "); scanf("%d", &day); getchar();
			printf("Enter the source city (a~z): "); scanf("%c", &src); getchar();
			printf("Enter the destination city (a~z): "); scanf("%c", &dst); getchar();
			printf("Enter the level of seat(1~3): "); scanf("%d", &day); getchar();
			num++;

			//reservationNumber = level * 1000000000 + num * 1000000 + day * 10000 + src * 100 + dst;

			values = module(name, src, dst, day, level);

			//temp = makenode(reservationNumber);
			temp = makenode(num);
			temp->passenger = values;
			root = insert(root, temp);


			printf("Reservated--------------------------------------------");
			printreserve(temp);

			h = height(root);
			printf("Number of reservation: %d\n", num);
			printf("Height is %d\n", h);
			break;

		case 2:
			printf("====================== Cancel ========================== \n");
			printf("Enter the reservation number for cancel");
			scanf("%d", &rnum);

			while (isexist(root, rnum) == NULL) {
				printf("There are no reservation!\n");
				printf("Pleasse enter the number correctly.\n");
				scanf("%d", &rnum);
			}

			temp = isexist(root, rnum);
			printreserve(temp);			

			root = delet(root, isexist(root, rnum));
			num--;

			printf("The reservation is canceled.\n");

			h = height(root);
			printf("Number of reservation: %d\n", num);
			printf("Height is %d\n", h);

			break;

		default:
			printf("Wrong number.\nPlease input number correctly\n");
			break;
		}
	}                                          //to here//

	DestroyGraph(pgraph);                  //메모리해제
	for (int i = 0; i < DAYS; i++) DestroyGraph(pgraphs[i]);
	return 0;
}

void PrintResult(Passenger* values) {
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

Passenger* module(char* name, char source, char destination, int date, int level) {
	Passenger* result = (Passenger*)malloc(sizeof(Passenger));
	char src = source, dst = destination;
	int day = date;
	int lev = level;
	for (int i = 0; i < 20; i++) result->name[i] = name[i];



	//PrintGraph(pgraphs[day - 1]);                  // 3+1=4일자의 depatureTime이 담긴 그래프를 출력합니다.
	City* srcCity = pgraphs[day - 1]->heads[src - 'a'];
	//printf("%d일자에 %c도시에서 이동가능한 도시의 정보는 아래와 같습니다.(금일 0시기준 도착시시간, 거쳐야하는 도시들)\n", day, src);
	//printf("departure time and date: %dh %dm %dday,", srcCity->departureTime / 60, srcCity->departureTime % 60, day);
	City* dstCity = ShortestPath(pgraphs[day - 1], src, dst);
	if (dstCity->needTime > 0) {            //제대로 찾음 ㅎ
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
	while (arr[i] != '\0') {
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
	City* cur, *tmp;
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
	City* newCity1, *newCity2, *head1, *head2, *cur;
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
	City** heads = pgraph->heads, *cur;
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
	City** heads1, ** heads2, *cur1, *cur2, *new;
	//
	for (int i = 0; i < DAYS; i++) {
		heads1 = pgraph->heads;
		heads2 = (City * *)malloc(sizeof(City*) * (pgraph->num));
		if (NULL == graphs) return 0;
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
		if (min >(Q[i]->needTime)) {
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
	City** heads = pgraph->heads, ** Q, *u = NULL, *adj, *v;
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
			//pathTime += (adj->departureTime - u->needTime);// waiting Time 출발시간이 소요시간보다적은경우는 없다 아래서걸러짐
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
			if (u->needTime > adj->departureTime) {//여기에서 걸러짐
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



//=============================RBT======================================

node* makenode(int key) {
	node* temp = (node*)malloc(sizeof(node));
	temp->parent = NULL;
	temp->leftc = NULL;
	temp->rightc = NULL;
	temp->passenger = NULL;
	temp->key = key;
	temp->color = 0;

	return temp;
}

void printreserve(node * root) {

	int hour, min;

	//printf("Reservated---------------------------------------\n");
	printf("Reservation Number: %d\n", root->key);
	printf("Name of passenger : %s\n", root->passenger->name);
	if (root->passenger->level == 1) printf("Grade of seat : First class\n");
	else if (root->passenger->level == 2) printf("Grade of seat : Business class\n");
	else if (root->passenger->level == 3) printf("Grade of seat : Economy class\n");
	printf("From : %c\n", root->passenger->source);
	hour = root->passenger->departureTime / 60;
	min = root->passenger->departureTime % 60;
	printf("Date & time : %d / %d : %d\n", root->passenger->departureDate, hour, min);
	printf("To : %c\n", root->passenger->destination);
	hour = root->passenger->arrivalTime / 60;
	min = root->passenger->arrivalTime % 60;
	printf("Date & time : %d / %d : %d\n", root->passenger->arrivalDate, hour, min);
	hour = root->passenger->flightTime / 60;
	min = root->passenger->flightTime % 60;
	printf("Flight time : %d : %d\n", hour, min);


}

int height(node* root) {
	int h;
	if (root == NULL)
		return 0;
	else {
		int left = height(root->leftc);
		int right = height(root->rightc);
		h = (left > right ? left : right) + 1;
		return h;
	}
}





//이 아래로는 기본적인 RBT의 기능 구현을 위한 함수들입니다.

node* isexist(node* root, int key) {
	if (root == NULL || root->key == key) return root;
	else if (root->key > key)
		return isexist(root->leftc, key);
	else
		return isexist(root->rightc, key);
}
void Lrotate(node* root, node* cur) {
	node* par = cur->rightc;

	cur->rightc = par->leftc;
	if (par->leftc != NULL)
		par->leftc->parent = cur;
	par->parent = cur->parent;
	if (cur->parent == NULL)
		root = par;
	else if (cur == cur->parent->leftc) {
		cur->parent->leftc = par;
	}
	else cur->parent->rightc = par;
	par->leftc = cur;
	cur->parent = par;
}
void Rrotate(node* root, node* cur) {
	node* par = cur->leftc;

	cur->leftc = par->rightc;
	if (par->rightc != NULL)
		par->rightc->parent = cur;
	par->parent = cur->parent;
	if (cur->parent == NULL)
		root = par;
	else if (cur == cur->parent->rightc) {
		cur->parent->rightc = par;
	}
	else cur->parent->leftc = par;
	par->rightc = cur;
	cur->parent = par;
}
void insert_fixup(node* root, node* cur) {
	node* y = NULL;

	while (cur != root && cur->parent->color == 1) {
		if (cur->parent == cur->parent->parent->leftc) {
			y = cur->parent->parent->rightc;
			if (y != NULL && y->color == 1) {
				cur->parent->color = 0;
				y->color = 0;
				cur->parent->parent->color = 1;
				cur = cur->parent->parent;
			}
			else {
				if (cur == cur->parent->rightc) {
					cur = cur->parent;
					Lrotate(root, cur);
				}
				cur->parent->color = 0;
				cur->parent->parent->color = 1;
				Rrotate(root, cur->parent->parent);
			}
		}
		else {
			y = cur->parent->parent->leftc;
			if (y != NULL && y->color == 1) {
				cur->parent->color = 0;
				y->color = 0;
				cur->parent->parent->color = 1;
				cur = cur->parent->parent;
			}
			else {
				if (cur == cur->parent->leftc) {
					cur = cur->parent;
					Rrotate(root, cur);
				}
				cur->parent->color = 0;
				cur->parent->parent->color = 1;
				Lrotate(root, cur->parent->parent);
			}
		}
	}
	root->color = 0;
}
node* insert(node* root, node* nod) {
	node* par = NULL;
	node* cur = root;

	while (cur != NULL) {
		par = cur;
		if (nod->key < cur->key) {
			cur = cur->leftc;
		}
		else cur = cur->rightc;
	}
	nod->parent = par;
	if (par == NULL) {
		root = nod;
	}
	else if (nod->key < par->key) {
		par->leftc = nod;
	}
	else par->rightc = nod;

	nod->leftc = NULL;
	nod->rightc = NULL;
	nod->color = 1;
	insert_fixup(root, nod);

	while (root->parent != NULL)
		root = root->parent;

	return root;
}
node* minnode(node* cur) {
	node* temp = cur;

	while (temp->leftc != NULL) {
		temp = temp->leftc;
	}
	return temp;
}
node* successor(node* cur) {
	node* par = cur->parent;

	if (cur->rightc != NULL)
		return minnode(cur->rightc);
	while (par != NULL && cur == par->rightc) {
		cur = par;
		par = par->parent;
	}
	return par;
}
void delete_fixup(node* root, node* cur) {

	node* temp = NULL;

	while (cur != NULL && cur->color == 0) {
		if (cur == cur->parent->leftc) {
			temp = cur->parent->rightc;
			if (temp->color == 1) {
				temp->color = 0;
				cur->parent->color = 1;
				Lrotate(root, cur->parent);
				temp = cur->parent->rightc;
			}
			if (temp->leftc->color == 0 && temp->rightc->color == 0) {
				temp->color = 1;
				cur = cur->parent;
			}
			else {
				if (temp->rightc->color == 0) {
					temp->leftc->color = 0;
					temp->color = 1;
					Rrotate(root, temp);
					temp = cur->parent->rightc;
				}
				temp->color = cur->parent->color;
				cur->parent->color = 0;
				temp->rightc->color = 0;
				Lrotate(root, cur->parent);
				cur = root;
			}
		}
		else {
			temp = cur->parent->leftc;
			if (temp->color == 1) {
				temp->color = 0;
				cur->parent->color = 1;
				Rrotate(root, cur->parent);
				temp = cur->parent->leftc;
			}
			if (temp->rightc->color == 0 && temp->leftc->color == 0) {
				temp->color = 1;
				cur = cur->parent;
			}
			else {
				if (temp->leftc->color == 0) {
					temp->rightc->color = 0;
					temp->color = 1;
					Lrotate(root, temp);
					temp = cur->parent->leftc;
				}
				temp->color = cur->parent->color;
				cur->parent->color = 0;
				temp->leftc->color = 0;
				Rrotate(root, cur->parent);
				cur = root;
			}
		}

	}
	if (cur != NULL)
		cur->color = 0;
}
node* delet(node* root, node* nod) {
	node* par = NULL;
	node* cur = NULL;

	if (nod->leftc == NULL || nod->rightc == NULL) {
		par = nod;
	}
	else par = successor(nod);

	if (par->leftc != NULL) {
		cur = par->leftc;
	}
	else cur = par->rightc;
	if (cur != NULL)
		cur->parent = par->parent;

	if (par->parent == NULL) {
		root = cur;
	}
	else {
		if (par == par->parent->leftc) {
			par->parent->leftc = cur;
		}
		else par->parent->rightc = cur;
	}

	if (par != nod) {
		nod->key = par->key;
	}
	if (par->color == 0) {
		delete_fixup(root, cur);
	}

	return root;
}
