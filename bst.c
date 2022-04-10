#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

struct node{
    int val;
    struct node* l;
    struct node* r;
};

struct node* new_node(int num){
    struct node* a=(struct node*)malloc(sizeof(struct node));
    a->val=num;
    a->l=NULL;
    a->r=NULL;
    return a;
}

struct node* search(struct node* a, int num){
    if(a==NULL) return NULL;
    struct node* ret;
    ret=num<(a->val)?search(a->l, num): search(a->r, num);
    if(ret==NULL) return a;
    return ret;
}

void assert(struct node* a, int num){
    struct node *ret=search(a, num);
    if(num==ret->val) printf("already exists\n");
    if(num<(ret->val)) ret->l=new_node(num);//printf("now %d left follows %d\n",ret->l->val, ret->val);} 把ret错打成了a ~_~
    if(num>(ret->val)) ret->r=new_node(num);
}

struct node* create_tree(int *list, int len){
    struct node* root=new_node(list[0]);
    for(int i=1; i<len; i++){
        assert(root, list[i]);
    }
    return root;
}

void dsp(struct node* a){
    if(a==NULL) return;
    dsp(a->l);
    printf("%d ",a->val);
    dsp(a->r);
}

void plot_node(struct node *a, FILE *f){
    fprintf(f,"node[shape=ellipse,style=filled,color=aquamarine,fontcolor=blue];\n"); 
    fprintf(f,"%d[label=\"%d\"];\n", a->val, a->val);
}

void plot_dsp(struct node* a, FILE *f){
    if(a==NULL) return;
    plot_node(a, f);
    if(a->l!=NULL){
        plot_node(a->l, f);
        fprintf(f,"%d:sw->%d;\n", a->val, a->l->val); 
    }
    if(a->r!=NULL){
        plot_node(a->r, f);
        fprintf(f,"%d:se->%d;\n", a->val, a->r->val); 
    }
    plot_dsp(a->l, f);
    plot_dsp(a->r, f);
}

void plot(struct node* a){
    FILE *f;
    f = fopen("bst.dot","w+");
    fprintf(f,"digraph G{\n");
    plot_dsp(a, f);
    fprintf(f,"}");
    fclose(f);
    system("dot -Tpng bst.dot -o bst.png");
}

int* generateN(int len){
    srand(time(NULL));
    int *a=(int*)malloc(len*sizeof(int));
    for(int i=0; i<len; i++){//可用洗牌算法
        LOOP:
        a[i]=rand()%len;
        for(int j=0; j<i; j++){
            if(a[j]==a[i]) goto LOOP;
        }
    }
    return a;

    struct node *root =create_tree(a, len);
    dsp(root);
    plot(root);
    
}

struct node* plot_Nnode_tree_auto(int len){
    int *a=generateN(len);
    struct node *root =create_tree(a, len);
    plot(root);
    return root;
}

int main(int argc, char **argv){
    struct node* root;
    for(int i=1; i<argc; i++){
        if(strcmp(argv[i], "-r")==0){//产生随机数字
            root=plot_Nnode_tree_auto(atoi(argv[i+1]));
            i++;
        }
        if(strcmp(argv[i], "-l")==0){//产生数字
            i++;
            int len=atoi(argv[i]);
            int *a=(int*)malloc(len*sizeof(int));
            for(int j=0;j<len;j++){
                i++;
                a[j]=atoi(argv[i]);
            }
            root=create_tree(a, len);
            plot(root);
        }
        if(strcmp(argv[i], "-v")==0){//版本信息
            printf("bst version 1.0.0\nauthor sayoriaaa\n");
            return 0;
        }
        if(strcmp(argv[i], "-h")==0){//帮助
            printf("initialization:\n-r num (-r 30,initial bst which has 30 nodes,ranging from 0 to 29, and generate graph simutaneously)\n");
            printf("-l len num1 num2 ... num_len (-r 3 1 5 4,initial bst which has 3 nodes, and generate graph file simutaneously)\n");
            printf("stdin:\nassert num(add node num)\nexit\n\n");
            return 0;
        }
    }
    char s[256];
    while(1){
        scanf("%s",s);
        if(strcmp(s,"exit")==0) return 0;
        if(strcmp(s,"assert")==0){
            int node;
            scanf("%d",&node);
            assert(root, node);
            plot(root);
        }
    }
    
}