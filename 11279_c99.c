#include <stdio.h>
#include <stdlib.h>

int currentSize;

typedef struct Node {
    int data;
    struct Node *left;
    struct Node *right;
    struct Node *parent;
} Node;

typedef struct Heap {
    Node *root;
} Heap;

// 노드 생성 함수
Node* createNode(int data, Node* parent) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->left = newNode->right = NULL;
    newNode->parent = parent;
    currentSize++;
    return newNode;
}

// Heap 초기화
Heap* createHeap() {
    Heap* heap = (Heap*)malloc(sizeof(Heap));
    heap->root = NULL;
    return heap;
}

// Swap 함수
void swap(Node* a, Node* b) {
    int temp = a->data;
    a->data = b->data;
    b->data = temp;
}

// Heapify Up (삽입 후 위로 이동)
void heapifyUp(Node* node) {
    while (node->parent && node->parent->data < node->data) {
        swap(node, node->parent);
        node = node->parent;
    }
}

// 완전 이진 트리에 삽입할 위치를 찾는 함수 (BFS)
Node* findInsertionPoint(Node* root) {
    if (!root) return NULL;
    
    Node* queue[100];
    int front = 0, rear = 0;
    queue[rear++] = root;
    
    while (front < rear) {
        Node* current = queue[front++];
        
        if (!current->left || !current->right)
            return current;
        
        queue[rear++] = current->left;
        queue[rear++] = current->right;
    }
    return NULL;
}

// 삽입 함수
void insert(Heap* heap, int data) {
    if (!heap->root) {
        heap->root = createNode(data, NULL);
        return;
    }

    Node* parent = findInsertionPoint(heap->root);
    Node* newNode = createNode(data, parent);
    
    if (!parent->left)
        parent->left = newNode;
    else
        parent->right = newNode;

    heapifyUp(newNode);
}

// Heapify Down (삭제 후 아래로 이동)
void heapifyDown(Node* node) {
    while (node->left) {
        Node* largest = node->left;
        if (node->right && node->right->data > node->left->data)
            largest = node->right;
        
        if (node->data > largest->data)
            break;

        swap(node, largest);
        node = largest;
    }
}

// 마지막 노드 찾기 (BFS)
Node* findLastNode(Node* root) {
    Node* queue[100];
    int front = 0, rear = 0;
    queue[rear++] = root;
    Node* lastNode = NULL;
    
    while (front < rear) {
        lastNode = queue[front++];
        if (lastNode->left) queue[rear++] = lastNode->left;
        if (lastNode->right) queue[rear++] = lastNode->right;
    }
    
    return lastNode;
}

// 최대값 추출
int extractMax(Heap* heap) {
    if (!heap->root) {
        return -1;
    }

    int maxVal = heap->root->data;
    Node* lastNode = findLastNode(heap->root);
    
    // 마지막 노드가 루트만 남은 경우
    if (heap->root == lastNode) {
        free(heap->root);
        heap->root = NULL;
        currentSize = 0;
        return maxVal;
    }

    // 루트 값 교체
    heap->root->data = lastNode->data;
    
    if (lastNode->parent->left == lastNode){
        lastNode->parent->left = NULL;
        --currentSize;
    }
    else{
        lastNode->parent->right = NULL;
        --currentSize;
    }
    
    free(lastNode);
    
    heapifyDown(heap->root);
    
    return maxVal;
}

int main() {
    int n, x;
    Heap* heap = createHeap();
    currentSize = 0;
    
    scanf("%d", &n);
    while(n--){
        scanf("%d", &x);
        if(x == 0)
            printf("%d\n", currentSize == 0 ? 0 : extractMax(heap));
        else
            insert(heap, x);
    }
    return 0;
}
