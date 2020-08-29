#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define N 4
#define p 0.7
#define MAX 2000000

typedef struct coordinates{
	int x;
	int y;
}coor;

typedef struct ma{
	int value;
	int move;
}ma;

typedef struct node{
	int x;
	int y;
	int cost;
	int e_cost;
	int parent;
	int visited;
	int id;
}Node;

Node* queue_array[MAX];
int rear = 0;
int front = 0;
int counter=0;

Node tree[N*N*N*N];
Node tree2[N*N*N*N];
coor path[25],path2[25];
int path_length,path_length2;
int final_cost,final_cost2;
int expan;
int flag_Astar_G1=0,flag_Astar_G2=0;
int **maze;
coor S,G1,G2;
int elements_of_tree,final_states[2];


void insert(Node *add_item){
    queue_array[rear] = add_item;
	rear = rear + 1;
}


int is_empty(){
	return (front == rear);
}


Node* delete(){
	if(is_empty()){
		return NULL;
	}
    front = front + 1;
	return queue_array[front-1];
}


void clean_tree2(){
	int k;
	for(k=0; k<N*N*N*N; k++){
		tree2[k].visited = 0;
	}
}


void initiate_root(){
	tree[0].x = S.x;
	tree[0].y = S.y;
	tree[0].cost = 0;
	tree[0].parent = -1;
	tree[0].visited = 0;
	tree[0].id=0;
	elements_of_tree = 0;
}


void initiate_root2(coor fs){
	tree2[0].x = S.x;
	tree2[0].y = S.y;
	tree2[0].cost = 0;
	tree2[0].e_cost = 2*abs(S.x-fs.x) +  abs(S.y-fs.y);
	tree2[0].parent = -1;
	tree2[0].visited = 0;
	tree2[0].id=0;
	elements_of_tree = 0;
	clean_tree2();
}



void initiate_maze(){
	int i,j;
	time_t t;
	srand((unsigned) time(&t));
	maze = (int **)malloc(N*sizeof(int));

	if(maze == NULL){
		printf("No memory\n");
		exit(1);
	}

	for(i=0; i<N; i++){
		maze[i] = (int *)malloc(N*sizeof(int));
	}
	
	for(i=0; i<N; i++){
		for(j=0; j<N; j++){
			if((rand() % 10) < 10 * p){
				maze[i][j] = 1;
			}else{
				maze[i][j] = -1;
			}
		}
	}
}



void save_path(int id,int sign){
	int x = tree2[id].parent;
	int l = 0;
	if(sign==1){
		path_length = 0;
		path[l].x = tree2[id].x;
		path[l].y = tree2[id].y;
		path_length++;
		l++;
		while(x != -1){
			path[l].x = tree2[x].x;
			path[l].y = tree2[x].y;
			x = tree2[x].parent;
			l++;
			path_length++;
		}
	}else if(sign==2){
		path_length2 = 0;
		path2[l].x = tree2[id].x;
		path2[l].y = tree2[id].y;
		path_length2++;
		l++;
		while(x != -1){
			path2[l].x = tree2[x].x;
			path2[l].y = tree2[x].y;
			x = tree2[x].parent;
			l++;
			path_length2++;
		}
	}
}

void ucs(){
	int i;
	Node* current_node;
	insert(&(tree[0]));
	while(!(is_empty())){
		current_node = delete();
		if(current_node == NULL){
			break;
		}
		if(tree[current_node->id].visited==1){
			continue;
		}
		counter ++;
		current_node->visited = 1;
		if(current_node->x == G1.x && current_node->y == G1.y){
			final_states[0] = current_node->id;
			if(final_states[1] != 0) break;
		}
		
		if(current_node->x == G2.x && current_node->y == G2.y){
			final_states[1] = current_node->id;
			if(final_states[0] != 0) break;
		}
		
		if((current_node->y)+1 < N && maze[current_node->x][(current_node->y)+1]==1 &&(current_node->parent == -1 || (current_node->x != tree[current_node->parent].x || (current_node->y)+1 != tree[current_node->parent].y))){
			elements_of_tree++;
			tree[elements_of_tree].x = current_node->x;
			tree[elements_of_tree].y = (current_node->y)+1;
			tree[elements_of_tree].cost = (current_node->cost)+1;
			tree[elements_of_tree].id = elements_of_tree;
			tree[elements_of_tree].parent = current_node->id;
			for(i = 0; i<elements_of_tree-1; i++){
				if(tree[elements_of_tree].x==tree[i].x && tree[elements_of_tree].y==tree[i].y && tree[i].visited==1){
					tree[elements_of_tree].visited = 1;
					break;
				}
			}
			insert(&(tree[elements_of_tree]));
		}
		if((current_node->y)-1 >= 0 && maze[current_node->x][(current_node->y)-1]==1 && (current_node->parent == -1 || (current_node->x != tree[current_node->parent].x || (current_node->y)-1 != tree[current_node->parent].y))){
			elements_of_tree++;
			tree[elements_of_tree].x = current_node->x;
			tree[elements_of_tree].y = (current_node->y)-1;
			tree[elements_of_tree].cost = (current_node->cost)+1;
			tree[elements_of_tree].id = elements_of_tree;
			tree[elements_of_tree].parent = current_node->id;
			for(i = 0; i<elements_of_tree-1; i++){
				if(tree[elements_of_tree].x==tree[i].x && tree[elements_of_tree].y==tree[i].y && tree[i].visited==1){
					tree[elements_of_tree].visited = 1;
					break;
				}
			}
			insert(&(tree[elements_of_tree]));
		}
		if((current_node->x)+1 < N && maze[(current_node->x)+1][current_node->y]==1 && (current_node->parent == -1 || ((current_node->x)+1 != tree[current_node->parent].x || current_node->y != tree[current_node->parent].y))){
			elements_of_tree++;
			tree[elements_of_tree].x = (current_node->x)+1;
			tree[elements_of_tree].y = current_node->y;
			tree[elements_of_tree].cost = (current_node->cost)+2;
			tree[elements_of_tree].id = elements_of_tree;
			tree[elements_of_tree].parent = current_node->id;
			for(i = 0; i<elements_of_tree-1; i++){
				if(tree[elements_of_tree].x==tree[i].x && tree[elements_of_tree].y==tree[i].y && tree[i].visited==1){
					tree[elements_of_tree].visited = 1;
					break;
				}
			}
			insert(&(tree[elements_of_tree]));
		}
		if((current_node->x)-1 >= 0 && maze[(current_node->x)-1][current_node->y]==1 &&(current_node->parent == -1 ||  ((current_node->x)-1 != tree[current_node->parent].x || current_node->y != tree[current_node->parent].y))){
			elements_of_tree++;
			tree[elements_of_tree].x = (current_node->x)-1;
			tree[elements_of_tree].y = current_node->y;
			tree[elements_of_tree].cost = (current_node->cost)+2;
			tree[elements_of_tree].id = elements_of_tree;
			tree[elements_of_tree].parent = current_node->id;
			for(i = 0; i<elements_of_tree-1; i++){
				if(tree[elements_of_tree].x==tree[i].x && tree[elements_of_tree].y==tree[i].y && tree[i].visited==1){
					tree[elements_of_tree].visited = 1;
					break;
				}
			}
			insert(&(tree[elements_of_tree]));
		}
	}
}


void A_star(coor fs,int *flag){
	counter = 0;
	elements_of_tree = 0;
	while(!(is_empty())){
		delete();
	}
	int i;
	Node* current_node;
	insert(&(tree2[0]));
	while(!(is_empty())){
		int ma_size=0;
		ma ma_sort[4];
		current_node = delete();
		if(current_node == NULL){
			break;
		}
		if(tree2[current_node->id].visited==1){
			continue;
		}
		counter ++;
		current_node->visited = 1;
		if(current_node->x == fs.x && current_node->y == fs.y){
			if(current_node->cost < final_cost || final_cost == 0){
				expan = counter;
				final_cost = current_node->cost;
				save_path(current_node->id,1);
			}else if(current_node->cost == final_cost){
				final_cost2 = current_node->cost;
				save_path(current_node->id,2);
			}
			*flag = 1;
			break;
		}
		
		if((current_node->y)+1 < N && maze[current_node->x][(current_node->y)+1]==1 &&(current_node->parent == -1 || (current_node->x != tree2[current_node->parent].x || (current_node->y)+1 != tree2[current_node->parent].y))){
			int e = ((current_node->e_cost) + 1) + 2*abs((current_node->x)-fs.x) +  (abs(((current_node->y)+1)-fs.y));
			if(ma_size==0){
				ma_sort[ma_size].value = e;
				ma_sort[ma_size++].move = 1;
			}else{
				for(i=0; i<ma_size; i++){
					if(ma_sort[i].value < e){
						ma_size++;
						int temp = ma_sort[i].value;
						int temp3 = ma_sort[i].move;
						ma_sort[i].value = e;
						ma_sort[i].move = 1;
						int j;
						for(j=i+1; j<ma_size; j++){
							int temp2 = ma_sort[j].value;
							int temp4 = ma_sort[j].move;
							ma_sort[j].value = temp;
							ma_sort[j].move = temp3;
							temp = temp2;
							temp3 = temp4;
						}
						break;
					}else if(ma_sort[i].value >= e && i == ma_size-1){
						ma_sort[ma_size].value=e;
						ma_sort[ma_size++].move=1;
						break;
					}
				}
			}
		}
		
		if((current_node->y)-1 >= 0 && maze[current_node->x][(current_node->y)-1]==1 && (current_node->parent == -1 || (current_node->x != tree2[current_node->parent].x || (current_node->y)-1 != tree2[current_node->parent].y))){
			int e = ((current_node->e_cost) + 1) + 2*abs((current_node->x)-fs.x) +  (abs(((current_node->y)-1)-fs.y));
			if(ma_size==0){
				ma_sort[ma_size].value = e;
				ma_sort[ma_size++].move = 2;
			}else{
				for(i=0; i<ma_size; i++){
					if(ma_sort[i].value < e){
						ma_size++;
						int temp = ma_sort[i].value;
						int temp3 = ma_sort[i].move;
						ma_sort[i].value = e;
						ma_sort[i].move = 2;
						int j;
						for(j=i+1; j<ma_size; j++){
							int temp2 = ma_sort[j].value;
							int temp4 = ma_sort[j].move;
							ma_sort[j].value = temp;
							ma_sort[j].move = temp3;
							temp = temp2;
							temp3 = temp4;
						}
						break;
					}else if(ma_sort[i].value >= e && i == ma_size-1){
						ma_sort[ma_size].value=e;
						ma_sort[ma_size++].move=2;
						break;
					}
				}
			}
		}
		
		if((current_node->x)+1 < N && maze[(current_node->x)+1][current_node->y]==1 && (current_node->parent == -1 || ((current_node->x)+1 != tree2[current_node->parent].x || current_node->y != tree2[current_node->parent].y))){
			int e = ((current_node->e_cost) + 2) + 2*abs(((current_node->x)+1)-fs.x) +  (abs((current_node->y)-fs.y));
			if(ma_size==0){
				ma_sort[ma_size].value = e;
				ma_sort[ma_size++].move = 3;
			}else{
				for(i=0; i<ma_size; i++){
					if(ma_sort[i].value < e){
						ma_size++;
						int temp = ma_sort[i].value;
						int temp3 = ma_sort[i].move;
						ma_sort[i].value = e;
						ma_sort[i].move = 3;
						int j;
						for(j=i+1; j<ma_size; j++){
							int temp2 = ma_sort[j].value;
							int temp4 = ma_sort[j].move;
							ma_sort[j].value = temp;
							ma_sort[j].move = temp3;
							temp = temp2;
							temp3 = temp4;
						}
						break;
					}else if(ma_sort[i].value >= e && i == ma_size-1){
						ma_sort[ma_size].value=e;
						ma_sort[ma_size++].move=3;
						break;
					}
				}
			}
		}
		
		if((current_node->x)-1 >= 0 && maze[(current_node->x)-1][current_node->y]==1 &&(current_node->parent == -1 ||  ((current_node->x)-1 != tree2[current_node->parent].x || current_node->y != tree2[current_node->parent].y))){
			int e = ((current_node->e_cost) + 2) + 2*abs(((current_node->x)-1)-fs.x) +  (abs((current_node->y)-fs.y));
			if(ma_size==0){
				ma_sort[ma_size].value = e;
				ma_sort[ma_size++].move = 4;
			}else{
				for(i=0; i<ma_size; i++){
					if(ma_sort[i].value < e){
						ma_size++;
						int temp = ma_sort[i].value;
						int temp3 = ma_sort[i].move;
						ma_sort[i].value = e;
						ma_sort[i].move = 4;
						int j;
						for(j=i+1; j<ma_size; j++){
							int temp2 = ma_sort[j].value;
							int temp4 = ma_sort[j].move;
							ma_sort[j].value = temp;
							ma_sort[j].move = temp3;
							temp = temp2;
							temp3 = temp4;
						}
						break;
					}else if(ma_sort[i].value >= e && i == ma_size-1){
						ma_sort[ma_size].value=e;
						ma_sort[ma_size++].move=4;
						break;
					}
				}
			}
		}
		int k;
		for(k=0;k<ma_size; k++){
			if(ma_sort[k].move==1){
				elements_of_tree++;
				tree2[elements_of_tree].x = current_node->x;
				tree2[elements_of_tree].y = (current_node->y)+1;
				tree2[elements_of_tree].cost = (current_node->cost) + 1;
				tree2[elements_of_tree].id = elements_of_tree;
				tree2[elements_of_tree].parent = current_node->id;
				for(i = 0; i<elements_of_tree-1; i++){
					if(tree2[elements_of_tree].x==tree2[i].x && tree2[elements_of_tree].y==tree2[i].y && tree2[i].visited==1){
						tree2[elements_of_tree].visited = 1;
						break;
					}
				}
				insert(&(tree2[elements_of_tree]));
			}else if(ma_sort[k].move==2){
				elements_of_tree++;
				tree2[elements_of_tree].x = current_node->x;
				tree2[elements_of_tree].y = (current_node->y)-1;
				tree2[elements_of_tree].cost = (current_node->cost) + 1;
				tree2[elements_of_tree].id = elements_of_tree;
				tree2[elements_of_tree].parent = current_node->id;
				for(i = 0; i<elements_of_tree-1; i++){
					if(tree2[elements_of_tree].x==tree2[i].x && tree2[elements_of_tree].y==tree2[i].y && tree2[i].visited==1){
						tree2[elements_of_tree].visited = 1;
						break;
					}
				}
				insert(&(tree2[elements_of_tree]));
			}else if(ma_sort[k].move==3){
				elements_of_tree++;
				tree2[elements_of_tree].x = (current_node->x)+1;
				tree2[elements_of_tree].y = current_node->y;
				tree2[elements_of_tree].cost = (current_node->cost) + 2;
				tree2[elements_of_tree].id = elements_of_tree;
				tree2[elements_of_tree].parent = current_node->id;
				for(i = 0; i<elements_of_tree-1; i++){
					if(tree2[elements_of_tree].x==tree2[i].x && tree2[elements_of_tree].y==tree2[i].y && tree2[i].visited==1){
						tree2[elements_of_tree].visited = 1;
						break;
					}
				}
				insert(&(tree2[elements_of_tree]));
			}else{
				elements_of_tree++;
				tree2[elements_of_tree].x = (current_node->x)-1;
				tree2[elements_of_tree].y = current_node->y;
				tree2[elements_of_tree].cost = (current_node->cost) + 2;
				tree2[elements_of_tree].id = elements_of_tree;
				tree2[elements_of_tree].parent = current_node->id;
				for(i = 0; i<elements_of_tree-1; i++){
					if(tree2[elements_of_tree].x==tree2[i].x && tree2[elements_of_tree].y==tree2[i].y && tree2[i].visited==1){
						tree2[elements_of_tree].visited = 1;
						break;
					}
				}
				insert(&(tree2[elements_of_tree]));
			}
		}
	}
}


void display(){
	int i,j;
	for(i=0; i<N; i++){
		for(j=0; j<N; j++){
			if(j<N-1){
				if(maze[i][j]<0){
					printf("%d ",maze[i][j]);
				}else{
					printf(" %d ",maze[i][j]);
				}
			}else{
				if(maze[i][j]<0){
					printf("%d\n",maze[i][j]);
				}else{
					printf(" %d\n",maze[i][j]);
				}
			}
		}
	}
}

void show_info(){
	printf("\n---------------ucs---------------\n");
	display();
	printf("S: (%d,%d), G1: (%d,%d), G2: (%d,%d)\n",S.x,S.y,G1.x,G1.y,G2.x,G2.y);
	if(tree[final_states[0]].cost < tree[final_states[1]].cost && tree[final_states[0]].cost!=0 && tree[final_states[1]].cost!=0){
		printf("Both final states accessible\n");
		printf("Cost: %d\n",tree[final_states[0]].cost);
		printf("Path: (%d,%d) <- ",G1.x,G1.y);
		int x = tree[final_states[0]].parent;
		while(x != -1){
			printf("(%d,%d) <- ",tree[x].x,tree[x].y);
			x = tree[x].parent;
		}
		printf("\n");
	}else if(tree[final_states[0]].cost > tree[final_states[1]].cost && tree[final_states[0]].cost!=0 && tree[final_states[1]].cost!=0){
		printf("Both final states accessible\n");
		printf("Cost: %d\n",tree[final_states[1]].cost);
		printf("Path: (%d,%d) <- ",G2.x,G2.y);
		int x = tree[final_states[1]].parent;
		while(x != -1){
			printf("(%d,%d) <- ",tree[x].x,tree[x].y);
			x = tree[x].parent;
		}
		printf("\n");
	}else if(tree[final_states[0]].cost==tree[final_states[1]].cost && tree[final_states[0]].cost!=0){
		printf("Both final states accessible\n");
		printf("Both final states has the same cost: %d \n",tree[final_states[0]].cost);
		printf("Path of G1: (%d,%d) <- ",G1.x,G1.y);
		int x = tree[final_states[0]].parent;
		while(x != -1){
			printf("(%d,%d) <- ",tree[x].x,tree[x].y);
			x = tree[x].parent;
		}
		printf("\n");
		printf("Path of G2: (%d,%d) <- ",G2.x,G2.y);
		int y = tree[final_states[1]].parent;
		while(y != -1){
			printf("(%d,%d) <- ",tree[y].x,tree[y].y);
			y = tree[y].parent;
		}
		printf("\n");
	}else if(tree[final_states[0]].cost==0 && tree[final_states[1]].cost!=0){
		printf("G1 is not accessible\n");
		printf("Cost: %d\n",tree[final_states[1]].cost);
		printf("Path: (%d,%d) <- ",G2.x,G2.y);
		int x = tree[final_states[1]].parent;
		while(x != -1){
			printf("(%d,%d) <- ",tree[x].x,tree[x].y);
			x = tree[x].parent;
		}
		printf("\n");
	}else if(tree[final_states[0]].cost!=0 && tree[final_states[1]].cost==0){
		printf("G2 is not accessible\n");
		printf("Cost: %d\n",tree[final_states[0]].cost);
		printf("Path: (%d,%d) <- ",G1.x,G1.y);
		int x = tree[final_states[0]].parent;
		while(x != -1){
			printf("(%d,%d) <- ",tree[x].x,tree[x].y);
			x = tree[x].parent;
		}
		printf("\n");
	}else{
		printf("Both final states are not accessible\n");
	}
	printf("Number of expansions is: %d\n",counter);
	printf("\n--------------------------------\n");
}


void show_info2(){
	int r;
	printf("\n---------------A*---------------\n");
	display();
	printf("S: (%d,%d), G1: (%d,%d), G2: (%d,%d)\n",S.x,S.y,G1.x,G1.y,G2.x,G2.y);
	if(final_cost2==0 && final_cost==0){
		printf("Both final states are not accessible\n");
	}else if(final_cost2==0){
		if(flag_Astar_G1 == 1 && flag_Astar_G2 == 1){
			printf("Both final states accessible\n");
			printf("Cost: %d\n",final_cost);
			printf("Path: ");
			for(r=0; r<path_length; r++){
				printf("(%d,%d) <- ",path[r].x,path[r].y);
			}
			printf("\n");
		}else if(flag_Astar_G1 == 0){
			printf("G1 is not accessible\n");
			printf("Cost: %d\n",final_cost);
			printf("Path: ");
			for(r=0; r<path_length; r++){
				printf("(%d,%d) <- ",path[r].x,path[r].y);
			}
			printf("\n");
		}else{
			printf("G2 is not accessible\n");
			printf("Cost: %d\n",final_cost);
			printf("Path: ");
			for(r=0; r<path_length; r++){
				printf("(%d,%d) <- ",path[r].x,path[r].y);
			}
			printf("\n");
		}
	}else{
		printf("Both final states accessible\n");
		printf("Both final states has the same cost: %d \n",final_cost);
		printf("Path of G1: ");
		for(r=0; r<path_length; r++){
			printf("(%d,%d) <- ",path[r].x,path[r].y);
		}
		printf("\n");
		printf("Path of G2: ");
		for(r=0; r<path_length2; r++){
			printf("(%d,%d) <- ",path2[r].x,path2[r].y);
		}
		printf("\n");
		
	}
	printf("Number of expansions is: %d\n",expan);
	printf("\n--------------------------------\n");
}




int main(){
	initiate_maze();
	display();
	while(1){
		printf("Give start coordinates: \n");
		scanf("%d",&(S.x));
		scanf("%d",&(S.y));
		if(maze[S.x][S.y] == 1){
			break;
		}
		printf("Wrong coordinates!\n");
	}
	while(1){
		printf("Give first final state coordinates: \n");
		scanf("%d",&(G1.x));
		scanf("%d",&(G1.y));
		if(maze[G1.x][G1.y] == 1){
			break;
		}
		printf("Wrong coordinates!\n");
	}
	while(1){
		printf("Give second final state coordinates: \n");
		scanf("%d",&(G2.x));
		scanf("%d",&(G2.y));
		if(maze[G2.x][G2.y] == 1){
			break;
		}
		printf("Wrong coordinates!\n");
	}
	initiate_root();
	ucs();
	show_info();
	initiate_root2(G1);
	A_star(G1,&flag_Astar_G1);
	initiate_root2(G2);
	A_star(G2,&flag_Astar_G2);
	show_info2();
	return 0;
}
