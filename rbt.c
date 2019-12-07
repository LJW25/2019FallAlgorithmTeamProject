#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
//black = 0, red = 1

typedef struct city {
	char name; //a~z
	int x;
	int y;
	int distance;
	struct city* next;
}City;

typedef struct passenger {
	char name[20];
	City* source;
	City* destination;
	int de_date;
	int de_time;
	int arr_date;
	int arr_time;
	int flighttime;
	int reservationNumber;

}Passenger;


typedef struct Node {
	int key;//= reservation number
	int color;
	struct Node * parent;
	struct Node * leftc;
	struct Node * rightc;
	Passenger * passenger;
}node;

//find shortest path
void reserve(node* temp) {
	int de_date;
	int de_time;
	int arr_date;
	int arr_time;
	int flighttime;

	//determine date and time

	temp->passenger->de_date = de_date;
	temp->passenger->de_time = de_time;
	temp->passenger->arr_date = arr_date;
	temp->passenger->arr_time = arr_time;
	temp->passenger->reservationNumber = temp->key;
	

}
node* input(int num, char name[20], City* source, City* destination, int date) {
	node* temp = (node*)malloc(sizeof(node));
	temp->parent = NULL;
	temp->leftc = NULL;
	temp->rightc = NULL;
	temp->key = num;
	temp->color = 0;

	temp->passenger->name = name;
	temp->passenger->source = source;
	temp->passenger->destination = destination;
	temp->passenger->arr_date = date;
	temp->passenger->reservationNumber = num;

	reserve(temp);

	return temp;
}
//-------------------------------------------------------------------------------------------------------RBT_start

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
/*
void printRBT(node *root, int level) {

	if (root == NULL) {
		for (int i = 0; i < level; i++)
			putchar('\t');
		puts("~");
	}
	else {
		printRBT(root->rightc, level + 1);
		for (int i = 0; i < level; i++)
			putchar('\t');
		if (root->color) {
			printf("%d[R]\n", root->key);
		}
		else printf("%d[B]\n", root->key);
		printRBT(root->leftc, level + 1);
	}
}
*/
//--------------------------------------------------------------------------------------------------RBT_end
void printreserve(node * root) {

	int hour, min;

	printf("Reservation---------------------------------------\n");
	printf("Reservation Number: %d\n", root->passenger->reservationNumber);
	printf("Name of passemger : %s\n", root->passenger->name);
	printf("From : %s", root->passenger->source->name);
	hour = root->passenger->de_time / 60;
	min = root->passenger->de_time % 60;

	printf("Date & time : %d / %d : %d", root->passenger->de_date, hour, min);
	printf("To : %s", root->passenger->destination->name);
	printf("Date & time : %d, %d", root->passenger->de_date, hout, min);
	printf("Flight time : %d", root->passenger->flighttime);

	//node num change
	//height change print


}

void printinfo() {

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

int main() {

	srand(time(NULL));

	node* root = NULL;
	node* temp = NULL;
	int num = 0;//reservationNumber
	char name[20];
	char so, de;
	City* source = NULL;
	City* destination = NULL;
	int date;
	int height = 0;

	for(int i = 0; i < 26; i++)
		//city[0] : acity 넣어주고 생성

	//insert
	for (num = 0; num < 500; num++) {

		date = rand % 31 + 1;
		numa = % 26 + 65; // asci맞나 확인

		for (int i = 0; i < 26; i++) {
			if (city[i].name == numa)
				source = &city[i]
		}


		temp = input(num, name, source, destination, date);
		root = insert(root, temp);
	}

	int renum = 0;
	int choice = 0;
	while (1) {

		printf("What do you want to do?\n");
		printf("1. Reservation\n");
		printf("2. Cancle\n");

		scanf("%d", &choice);

		switch (choice) {
		case 1: printf("\n insert\n");
			printf("Name: ");
			scanf("%s", &name);
			printf("Source: ");
			scanf("%c", &so);
			printf("Destionation: ");
			scanf("%c", &de);
			printf("Date: ");
			scanf("%d", &date);
			num++;
			temp = input(num, name, source, destination, date);
			h = height(root);
			printf("Number of reservation: %d\n", num);
			printf("Height is %d\n", height);
			break;
		case 2: printf("\n Cancel the reservation \n");
			printf("Please input your reservation Number");
			scanf("%d", &renum);

			root = delet(root, isexist(root, renum));

			num--;
			h = height(root);
			printf("Number of reservation: %d\n", num);
			printf("Height is %d\n", height);
			break;
		}
		}
	}
}
