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

	printf("[----- [ Choi Yoolim ] [ 2020022001 ] -----]");

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
		head->left = newNode; // BST의 루트노드
		return 1;
	}

	/* head->left is the root */
	// BST가 비어 있지 않을 경우
	Node* ptr = head->left; // ptr이 BST의 루트노드가 되도록 설정

	Node* parentNode = NULL; // 부모 노드에 대한 포인터
	while(ptr != NULL) { // ptr이 비어있지 않을 경우

		/* if there is a node for the key, then just return */
		// 입력한 값과 동일한 키값을 가진 노드가 존재할 경우 함수 종료
		if(ptr->key == key) return 1;

		/* we have to move onto children nodes,
		 * keep tracking the parent using parentNode */
		parentNode = ptr; // 현재 노드를 부모노드로 설정

		/* key comparison, if current node's key is greater than input key
		 * then the new node has to be inserted into the right subtree;
		 * otherwise the left subtree.*/
		if(ptr->key < key) // 현재노드의 키값보다 입력값이 큰 경우
			ptr = ptr->right; // 오른쪽 자식노드로 이동
		else // 현재노드의 키값보다 입력값이 작은 경우
			ptr = ptr->left; // 왼쪽 자식노드로 이동
	}

	/* linking the new node to the parent */
	// 노드 삽입
	if(parentNode->key > key) // 부모노드의 키값이 입력된 키값보다 큰 경우
		parentNode->left = newNode; // 부모노드의 왼쪽 자식노드에 삽입
	else // 부모노드의 키값이 입력된 키값보다 작은 경우
		parentNode->right = newNode; // 부모노드의 오른쪽 자식노드에 삽입
    
	return 1;
}

// 입력한 키값을 가진 노드 삭제하는 함수
int deleteNode(Node* head, int key){

	if (head == NULL) { // 헤드노드에 대한 동적할당이 되지 않은 경우
		printf("\n Nothing to delete!!\n");
		return -1;
	}

	if (head->left == NULL) { // BST에 노드가 존재하지 않는 경우
		printf("\n Nothing to delete!!\n");
		return -1;
	}

	/* head->left is the root */
	Node* root = head->left; // 헤드노드의 왼쪽 자식노드를 루트노드로 설정

	Node* parent = NULL; // 부모노드는 NULL로 초기화
	Node* ptr = root; // 현재노드(삭제할 노드)를 루트노드로 설정

	while((ptr != NULL)&&(ptr->key != key)) { // 현재노드가 존재하고 현재노드의 키값과 입력된 키값이 일치하지 않는 경우
		if(ptr->key != key) { // 현재 노드의 키값이 입력된 키값과 일치하지 않을 경우

			parent = ptr;	/* save the parent */

			if(ptr->key > key) // 현재노드의 키값이 입력된 키값보다 큰 경우
				ptr = ptr->left; // 왼쪽 자식노드로 이동
			else // 현재노드의 키값이 입력된 키값보다 작은 경우
				ptr = ptr->right; // 오른쪽 자식노드로 이동
		}
	}

	/* there is no node for the key */
	if(ptr == NULL){ // 키값과 일치하는 노드를 찾지 못한 경우
		printf("No node for key [%d]\n ", key);
		return -1;
	}

	// case 1: the node which has to be removed is a leaf node
	// 삭제할 노드(현재노드)가 리프노드인 경우	
	if(ptr->left == NULL && ptr->right == NULL){
		// 부모노드가 존재하는 경우
		if(parent != NULL) { /* parent exists, parent's left and right links are adjusted */
			if(parent->left == ptr) // 부모노드의 왼쪽 자식노드가 현재노드(삭제할 노드)와 일치하는 경우
				parent->left = NULL; // 부모노드의 왼쪽 자식노드 삭제
			else // 부모노드의 오른쪽 자식노드가 현재노드(삭제할 노드)와 일치할 경우
				parent->right = NULL; // 부모노드의 오른쪽 자식노드 삭제
		} 
        else { // 부모 노드가 존재하지 않는 경우(삭제할 노드가 루트노드인 경우)
			/* parent is null, which means the node to be deleted is the root */
			head->left = NULL; // 루트노드 초기화
		}

		free(ptr); // 삭제할 노드에 대한 동적할당 해제
		return 1;
	}

	// case 2: if the node to be deleted has one child
	// 삭제할 노드가 하나의 자식을 갖는 경우
	if ((ptr->left == NULL || ptr->right == NULL)){

		Node* child; // 자식 노드를 저장할 변수
		if (ptr->left != NULL) // 삭제할 노드의 왼쪽 자식노드가 존재하는 경우
			child = ptr->left; // 왼쪽 자식노드를 child에 저장
		else // 삭제할 노드의 오른쪽 자식노드가 존재하는 경우
			child = ptr->right; // 오른쪽 자식노드를 child에 저장

		if(parent != NULL){ // 부모노드가 존재하는 경우
			if(parent->left == ptr) // 삭제할 노드가 부모노드의 왼쪽 자식노드인 경우
				parent->left = child; // 부모노드의 왼쪽 자식노드를 child으로 설정
			else // 삭제할 노드가 부모노드의 오른쪽 자식노드인 경우
				parent->right = child; // 부모노드의 오른쪽 자식노드를 child으로 설정
		}
        else { // 부모노드가 비어있는 경우(삭제할 노드가 루트노드인 경우)
			/* parent is null, which means the node to be deleted is the root
			 * and the root has one child. Therefore, the child should be the root
			 */
			root = child; // 루트노드를 child로 설정
		}

		free(ptr); // 삭제할 노드의 동적할당 해제
		return 1;
	}

	/*case 3: the node (ptr) has two children
	 *
	 * we have to find either the biggest descendant node in the left subtree of the ptr
	 * or the smallest descendant in the right subtree of the ptr.
	 *
	 * we will find the smallest descendant from the right subtree of the ptr.*/
	// 삭제할 노드가 양쪽 모두에 자식을 갖는 경우
	Node* candidate; // 후보노드
	parent = ptr; // 부모노드를 현재노드로 설정

	candidate = ptr->right; // 후보노드를 현재노드의 오른쪽 자식노드를 저장

	/* the smallest node is left deepest node in the right subtree of the ptr */
	while(candidate->left != NULL){ // 후보노드의 왼쪽 자식노드가 존재하는 경우
		parent = candidate; // 부모노드를 후보노드로 설정
		candidate = candidate->left; // 후보노드의 왼쪽 자식노드로 이동
	}

	/* the candidate node is the right node which has to be deleted.
	 * note that candidate's left is null */
	if (parent->right == candidate) // 후보노드가 부모노드의 오른쪽 자식노드인 경우
		parent->right = candidate->right; // 후보노드의 오른쪽 자식노드를 부모노드의 오른쪽 자식노드로 설정
	else // 후보노드가 부모노드의 왼쪽 자식노드인 경우
		parent->left = candidate->right; // 후보노드의 오른쪽 자식노드를 부모노드의 왼쪽 자식노드로 설정

	/* instead of removing ptr, we just change the key of ptr
	 * with the key of candidate node and remove the candidate node */

	ptr->key = candidate->key; // 후보노드의 키값을 현재노드의 키값으로 설정

	free(candidate); // 후보노드에 대한 동적할당 해제
	return 1;
}

// 재귀방식으로 BST의 모든 노드에 대한 메모리 해제하는 함수
void freeNode(Node* ptr){

	if(ptr) { // 현재노드가 비어져 있지 않는 경우
		freeNode(ptr->left); // 재귀방식으로 왼쪽 서브트리에 대한 동적할당 해제
		freeNode(ptr->right); // 재귀방식으로 오른쪽 서브트리에 대한 동적할당 해제
		free(ptr); // 현재노드에 대한 동적할당 해제
	}
}

// BST의 메모리 해제하는 함수
int freeBST(Node* head){

	if(head->left == head){ // BST가 노드를 가지고 있지 않는 경우
		free(head); // 헤드노드에 대한 동적할당 해제
		return 1;
	}

	// BST가 노드를 가지고 있는 경우
	Node* p = head->left; // 현재노드를 루트노드로 설정

	freeNode(p); // 트리의 모든 노드 해제하는 함수 호출

	free(head); // 헤드노드에 대한 동적할당 해제
	return 1;
}

// 스택의 top을 pop하는 함수
Node* pop(){
	if (top < 0) return NULL; // 스택이 비어있을 경우, NULL 반환
	
	// 스택이 비어있지 않을 경우 
	return stack[top--]; // 스택의 탑을 pop, pop한 결과를 top에 적용
}

// 스택에 push하는 함수
void push(Node* aNode){
	stack[++top] = aNode; // push하기 위해 top을 조정한 뒤, 스택에 노드를 push
}

// 스택의 원소를 출력하는 함수
void printStack(){
	int i = 0;
	printf("--- stack ---\n");
	while(i <= top){ // 스택의 노드에 대한 반복
		printf("stack[%d] = %d\n", i, stack[i]->key);
	}
}

// 큐에서 데이터를 삭제하는 함수
Node* deQueue(){
	if (front == rear) { // 큐가 비어있는 경우
		// printf("\n....Now Queue is empty!!\n" );
		return NULL; // NULL 반환
	}
	// 큐가 비어있지 않을 경우
	front = (front + 1) % MAX_QUEUE_SIZE; // 모듈러 연산을 통한 front 계산
	return queue[front]; // 첫번째 원소 pop하여 반환
}

// 큐에 데이터를 삽입하는 함수
void enQueue(Node* aNode){ 
    
	rear = (rear + 1) % MAX_QUEUE_SIZE; // 모듈러 연산을 통한 rear 계산
	if (front == rear) { // 큐가 가득 차 있는 경우
		// printf("\n....Now Queue is full!!\n");
		return; // 함수 종료
	}
	// 큐가 가득 차 있지 않는 경우
	queue[rear] = aNode; // 큐에 노드를 push
}