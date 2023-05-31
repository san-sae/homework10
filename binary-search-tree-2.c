/*
 * Binary Search Tree #2
 * Data Structures
 * School of Computer Science
 * at Chungbuk National University
 */

#include <stdio.h>
#include <stdlib.h>

// BST의 노드에 대한 구조체
typedef struct node {
	int key;
	struct node *left; // 왼쪽 자식 노드에 대한 포인터
	struct node *right; // 오른쪽 자식 노드에 대한 포인터
} Node;

/* for stack */
#define MAX_STACK_SIZE 20 // 스택의 최대 사이즈
Node* stack[MAX_STACK_SIZE]; // 노드 포인터를 저장하는 배열
int top = -1; // 스택의 최상단을 가리키는 인덱스

/* for queue */
#define MAX_QUEUE_SIZE	20 // 큐의 최대 사이즈
Node* queue[MAX_QUEUE_SIZE]; // 노드 포인터를 저장하는 배열
int front = -1; // 큐의 첫 번째 인덱스
int rear = -1; // 큐의 마지막 인덱스

int initializeBST(Node** h); // BST 초기화하는 함수
void recursiveInorder(Node* ptr);	  /* recursive inorder traversal */
int insert(Node* head, int key);      /* insert a node to the tree */
int freeBST(Node* head); /* free all memories allocated to the tree */

/* functions that you have to implement */
void iterativeInorder(Node* ptr);     /* iterative inorder traversal */
void levelOrder(Node* ptr);	          /* level order traversal */
int deleteNode(Node* head, int key);  /* delete the node for the key */
Node* pop(); // 스택에서 노드를 pop하는 함수
void push(Node* aNode); // 스택에 노드를 push하는 함수
Node* deQueue(); // 큐에서 노드를 삭제하는 함수
void enQueue(Node* aNode); // 큐에 노드를 추가하는 함수

/* you may add your own defined functions if necessary */

void printStack(); // 스택의 노드들에 대하여 출력하는 함수

int main(){
    
	char command; // 명령어
	int key; // 입력받을 키값
	Node* head = NULL; // 헤드노드

	do{
		printf("\n\n");
		printf("----------------------------------------------------------------\n");
		printf("                   Binary Search Tree #2                        \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize BST       = z                                       \n");
		printf(" Insert Node          = i      Delete Node                  = d \n");
		printf(" Recursive Inorder    = r      Iterative Inorder (Stack)    = t \n");
		printf(" Level Order (Queue)  = l      Quit                         = q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command); // 사용자로부터 수행할 명령어 입력받음

		switch(command) {
		case 'z': case 'Z':
			initializeBST(&head); // BST 초기화
			break;
		case 'q': case 'Q':
			freeBST(head); // 동적할당 해제
			break;
		case 'i': case 'I':
			printf("Your Key = ");
			scanf("%d", &key); // 사용자로부터 키값 입력받음
			insert(head, key); // 입력받은 키값으로 구성된 노드를 BST에 삽입
			break;
		case 'd': case 'D':
			printf("Your Key = ");
			scanf("%d", &key); // 사용자로부터 키값 입력받음
			deleteNode(head, key); // 입력받은 키값을 갖는 노드를 BST에서 삭제
			break;

		case 'r': case 'R':
			recursiveInorder(head->left); // 재귀 방식으로 중위순회하여 트리에 대하여 출력
			break;
		case 't': case 'T':
			iterativeInorder(head->left); // 비재귀 방식으로 중위순회하여 트리에 대하여 출력
			break;

		case 'l': case 'L':
			levelOrder(head->left); // 레벨순서순회하여 트리에 대하여 출력
			break;

		case 'p': case 'P':
			printStack(); // 스택에 있는 노드에 대한 출력
			break;

		default: // 그 이외의 명령어는 유효하지 않음
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

// BST 초기화하는 함수
int initializeBST(Node** h) {

	/* if the tree is not empty, then remove all allocated nodes from the tree*/
	if(*h != NULL)
		freeBST(*h);

	/* create a head node */
	*h = (Node*)malloc(sizeof(Node)); // 헤드노드에 대한 동적 할당
	(*h)->left = NULL;	/* root */
	(*h)->right = *h; // 오른쪽 자식노드는 헤더노드 자신을 가리키도록 설정
	(*h)->key = -9999; // 무의미한 값 저장

	top = -1; // 스택에 대한 인덱스의 초기 상태

	front = rear = -1; // 원형 큐에 대한 인덱스의 초기 상태

	return 0;
}

// 재귀방식을 통해 중위순회하는 함수
void recursiveInorder(Node* ptr){
	if(ptr) { // ptr이 NULL이 아닐 경우(비워져 있지 않을 경우)
		recursiveInorder(ptr->left); // 재귀방식으로 왼쪽 자식 노드 방문
		printf(" [%d] ", ptr->key); // 현재 노드의 값 출력
		recursiveInorder(ptr->right); // 재귀방식으로 오른쪽 자식 노드 방문
	}
}

// textbook: p 224
// 반복문을 이용하여 중위 순회하는 함수
void iterativeInorder(Node* node){
	for(;;){ // infinite loop
        // node가 NULL이 아닐 경우 반복문 수행
        // 반복문 수행 후 왼쪽 자식 노드로 이동, 왼쪽 자식이 비어 있지 않을 경우 계속 반복
		for(; node; node = node->left)
			push(node); // 스택에 노드 push
		node = pop(); // 스택에서 노드 pop

		if(!node) // node(스택에서 팝한 노드)가 NULL일 경우
            break; // 함수 종료
        
        // node가 NULL이 아닐 경우
		printf(" [%d] ", node->key); // 현재 노드의 키 값 출력

		node = node->right; // 오른쪽 자식 노드로 이동
	}
}

// textbook: p 225
// 노드 ptr 기준으로 BST를 레벨 순서로 순회하는 함수
void levelOrder(Node* ptr){
	// int front = rear = -1;

	if(!ptr) return; /* If BST is empty, the function exits */

	enQueue(ptr); // 노드 ptr을 큐에 푸쉬(루트노드)

	for(;;){ // infinite loop
	 	ptr = deQueue(); // 큐의 첫 번째 원소 삭제
		if(ptr) { // ptr이 NULL이 아닐 경우
			printf(" [%d] ", ptr->key); // 현재 노드의 키값 출력

			if(ptr->left) // 현재 노드의 왼쪽 자식 노드가 존재할 경우
				enQueue(ptr->left); // 왼쪽 자식 노드를 큐에 추가
			if(ptr->right) // 현재 노드의 오른쪽 자식 노드가 존재할 경우
				enQueue(ptr->right); // 오른쪽 자식 노드를 큐에서 삭제
		}
		else // ptr이 NULL일 경우
			break; // 무한 루프 종료
	}
}

// 입력받은 키값을 갖는 노드 생성 및 삽입하는 함수
int insert(Node* head, int key){

	Node* newNode = (Node*)malloc(sizeof(Node)); // 삽입할 노드에 대한 동적할당
	newNode->key = key; // 노드의 키값
	newNode->left = NULL; // 왼쪽 자식 노드
	newNode->right = NULL; // 오른쪽 자식 노드

	if (head->left == NULL) { // BST가 비어있을 경우
		head->left = newNode; // 
		return 1;
	}

	/* head->left is the root */
	Node* ptr = head->left;

	Node* parentNode = NULL;
	while(ptr != NULL) {

		/* if there is a node for the key, then just return */
		if(ptr->key == key) return 1;

		/* we have to move onto children nodes,
		 * keep tracking the parent using parentNode */
		parentNode = ptr;

		/* key comparison, if current node's key is greater than input key
		 * then the new node has to be inserted into the right subtree;
		 * otherwise the left subtree.
		 */
		if(ptr->key < key)
			ptr = ptr->right;
		else
			ptr = ptr->left;
	}

	/* linking the new node to the parent */
	if(parentNode->key > key)
		parentNode->left = newNode;
	else
		parentNode->right = newNode;
    
	return 1;
}


int deleteNode(Node* head, int key){

	if (head == NULL) {
		printf("\n Nothing to delete!!\n");
		return -1;
	}

	if (head->left == NULL) {
		printf("\n Nothing to delete!!\n");
		return -1;
	}

	/* head->left is the root */
	Node* root = head->left;

	Node* parent = NULL;
	Node* ptr = root;

	while((ptr != NULL)&&(ptr->key != key)) {
		if(ptr->key != key) {

			parent = ptr;	/* save the parent */

			if(ptr->key > key)
				ptr = ptr->left;
			else
				ptr = ptr->right;
		}
	}

	/* there is no node for the key */
	if(ptr == NULL){
		printf("No node for key [%d]\n ", key);
		return -1;
	}

	/*
	 * case 1: the node which has to be removed is a leaf node
	 */
	if(ptr->left == NULL && ptr->right == NULL){

		if(parent != NULL) { /* parent exists, parent's left and right links are adjusted */
			if(parent->left == ptr)
				parent->left = NULL;
			else
				parent->right = NULL;
		} 
        else {
			/* parent is null, which means the node to be deleted is the root */
			head->left = NULL;

		}

		free(ptr);
		return 1;
	}

	/**
	 * case 2: if the node to be deleted has one child
	 */
	if ((ptr->left == NULL || ptr->right == NULL)){

		Node* child;
		if (ptr->left != NULL)
			child = ptr->left;
		else
			child = ptr->right;

		if(parent != NULL){
			if(parent->left == ptr)
				parent->left = child;
			else
				parent->right = child;
		}
        else {
			/* parent is null, which means the node to be deleted is the root
			 * and the root has one child. Therefore, the child should be the root
			 */
			root = child;
		}

		free(ptr);
		return 1;
	}

	/**
	 * case 3: the node (ptr) has two children
	 *
	 * we have to find either the biggest descendant node in the left subtree of the ptr
	 * or the smallest descendant in the right subtree of the ptr.
	 *
	 * we will find the smallest descendant from the right subtree of the ptr.
	 *
	 */

	Node* candidate;
	parent = ptr;

	candidate = ptr->right;

	/* the smallest node is left deepest node in the right subtree of the ptr */
	while(candidate->left != NULL){
		parent = candidate;
		candidate = candidate->left;
	}

	/* the candidate node is the right node which has to be deleted.
	 * note that candidate's left is null
	 */
	if (parent->right == candidate)
		parent->right = candidate->right;
	else
		parent->left = candidate->right;

	/* instead of removing ptr, we just change the key of ptr
	 * with the key of candidate node and remove the candidate node
	 */

	ptr->key = candidate->key;

	free(candidate);
	return 1;
}


void freeNode(Node* ptr){

	if(ptr) {
		freeNode(ptr->left);
		freeNode(ptr->right);
		free(ptr);
	}
}

int freeBST(Node* head){

	if(head->left == head){
		free(head);
		return 1;
	}

	Node* p = head->left;

	freeNode(p);

	free(head);
	return 1;
}

Node* pop(){
	if (top < 0) return NULL;
	return stack[top--];
}

void push(Node* aNode){
	stack[++top] = aNode;
}

void printStack(){
	int i = 0;
	printf("--- stack ---\n");
	while(i <= top){
		printf("stack[%d] = %d\n", i, stack[i]->key);
	}
}

Node* deQueue(){
	if (front == rear) {
		// printf("\n....Now Queue is empty!!\n" );
		return NULL;
	}

	front = (front + 1) % MAX_QUEUE_SIZE;
	return queue[front];
}

void enQueue(Node* aNode){
    
	rear = (rear + 1) % MAX_QUEUE_SIZE;
	if (front == rear) {
		// printf("\n....Now Queue is full!!\n");
		return;
	}

	queue[rear] = aNode;
}