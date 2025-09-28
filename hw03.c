#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// 노드 구조체
typedef struct Node {
    char data;
    struct Node *left, *right;
} Node;

// 스택
typedef struct {
    Node* arr[2000];
    int top;
} Stack;

void push(Stack *s, Node* n) { s->arr[++s->top] = n; }
Node* pop(Stack *s) { return s->arr[s->top--]; }
Node* peek(Stack *s) { return s->arr[s->top]; }
int empty(Stack *s) { return s->top < 0; }

// ---------------- 트리 파서 ----------------
const char *input;
int pos = 0;

void skip() {
    while (input[pos] == ' ' || input[pos] == '\n' || input[pos] == '\t')
        pos++;
}

Node* newNode(char c) {
    Node* n = (Node*)malloc(sizeof(Node));
    n->data = c;
    n->left = n->right = NULL;
    return n;
}

Node* buildTree() {
    Stack s = {.top=-1};
    Node* root = NULL;
    Node* last = NULL;

    while (input[pos] != '\0') {
        skip();
        char c = input[pos];
        if (c == '\0') break;

        if (c == '(') {
            pos++;
            // 서브트리 시작 → 스택에 최근 노드 push
            if (last) push(&s, last);
        }
        else if (c == ')') {
            pos++;
            // 서브트리 끝 → 스택 pop
            if (!empty(&s)) pop(&s);
        }
        else if (isalpha(c)) {
            Node* n = newNode(c);
            if (!root) root = n; // 첫 노드가 root
            if (!empty(&s)) {
                Node* parent = peek(&s);
                if (!parent->left) parent->left = n;
                else parent->right = n;
            }
            last = n;
            pos++;
        }
        else {
            pos++; // 기타 공백 등 무시
        }
    }
    return root;
}

// ---------------- 반복 순회 ----------------

// 전위 (Root-Left-Right)
void preorder(Node* root) {
    Stack s = {.top=-1};
    if (!root) return;
    push(&s, root);
    while (!empty(&s)) {
        Node* n = pop(&s);
        printf("%c ", n->data);
        if (n->right) push(&s, n->right);
        if (n->left) push(&s, n->left);
    }
}

// 중위 (Left-Root-Right)
void inorder(Node* root) {
    Stack s = {.top=-1};
    Node* cur = root;
    while (cur || !empty(&s)) {
        while (cur) {
            push(&s, cur);
            cur = cur->left;
        }
        cur = pop(&s);
        printf("%c ", cur->data);
        cur = cur->right;
    }
}

// 후위 (Left-Right-Root)
void postorder(Node* root) {
    Stack s1 = {.top=-1}, s2 = {.top=-1};
    if (!root) return;
    push(&s1, root);
    while (!empty(&s1)) {
        Node* n = pop(&s1);
        push(&s2, n);
        if (n->left) push(&s1, n->left);
        if (n->right) push(&s1, n->right);
    }
    while (!empty(&s2)) {
        printf("%c ", pop(&s2)->data);
    }
}

// ---------------- main ----------------
int main() {
    static char buf[2000];
    fgets(buf, sizeof(buf), stdin);
    input = buf;

    Node* root = buildTree();

    printf("pre-order: ");
    preorder(root);
    printf("\n");

    printf("in-order: ");
    inorder(root);
    printf("\n");

    printf("post-order: ");
    postorder(root);
    printf("\n");

    return 0;
}
