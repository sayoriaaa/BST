#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
int NODE_NUM=0;
int stack[1024];
void print_stack(){
    for(int i=0;i<NODE_NUM;i++) printf("%d ", stack[i]);
}


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

void mid_order(struct node *a){
    if(a==NULL) return;
    mid_order(a->l);
    stack[NODE_NUM]=a->val;
    NODE_NUM++;
    mid_order(a->r);
}

void pre_order(struct node *a){
    if(a==NULL) return;
    stack[NODE_NUM]=a->val;
    NODE_NUM++;
    pre_order(a->l); 
    pre_order(a->r);  
}

struct node* search(struct node* a, int num){
    if(a==NULL) return NULL;
    if(a->val==num) return a;
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

struct node* search_parent(struct node* a, int num){
    struct node* ret=a;
    if(a->l!=NULL){
        if(a->l->val==num) return a;
        ret=search_parent(a->l,num);
    }
    if(a->r!=NULL){
        if(a->r->val==num) return a;
        ret=search_parent(a->r, num);
    }
    return ret;
}

struct node* get_max(struct node *a){
    if(a->r==NULL) return a;
    get_max(a->r);
}

void delete(struct node* a, int num){
    struct node *to_delete=search(a, num);//分类讨论
    if(to_delete->val!=num) return;
    struct node *parent=a;
    if(to_delete!=a) parent=search_parent(a, num);
    if(to_delete->l==NULL&&to_delete->r==NULL){//是叶节点的情况，把父节点的指向删除     
        if(parent->l!=NULL&&parent->l->val==num) parent->l=NULL;
        if(parent->r!=NULL&&parent->r->val==num) parent->r=NULL;
    }
    else if((to_delete->l==NULL)|(to_delete->r==NULL)){//有一个节点
        if(to_delete->l==NULL){
            if(parent->l!=NULL&&parent->l->val==num) parent->l=to_delete->r;
            if(parent->r!=NULL&&parent->r->val==num) parent->r=to_delete->r;
        }
        if(to_delete->r==NULL){
            if(parent->l!=NULL&&parent->l->val==num) parent->l=to_delete->l;
            if(parent->r!=NULL&&parent->r->val==num) parent->r=to_delete->l;
        }
    }
    else{//满子节点
        struct node *left_max=get_max(to_delete->l);
        struct node *left_max_parent=search_parent(a, left_max->val);
        to_delete->val=left_max->val;
        if(left_max_parent==to_delete){//特殊情况，因为要删的是左节点
            to_delete->l=left_max->l;}
        else{
            to_delete->l->r=NULL;}
    }
}

int max_depth(struct node* a){
    int depth=0;
    if(a==NULL) return 0;
    depth=max_depth(a->l)>max_depth(a->r)?max_depth(a->l)+1:max_depth(a->r)+1;
    return depth;
}

int min_depth(struct node* a){
    int depth=0;//这边比最大深度复杂一点，最大深度一边为空允许继续递归，但最小深度就停止了，即便不是叶节点，所以就加了个单孩子节点的逻辑
    if(a->l==NULL&&a->r==NULL) return 1;
    else if(a->l!=NULL&&a->r!=NULL) return min_depth(a->l)<min_depth(a->r)?min_depth(a->l)+1:min_depth(a->r)+1;
    else{
        if(a->l==NULL) return min_depth(a->r)+1;
        else return min_depth(a->l)+1;
    }
    return depth;
}

void sym(struct node* a){
    if(a==NULL) return;
    struct node *tmp=a->l;
    a->l=a->r;
    a->r=tmp;
    sym(a->l);
    sym(a->r);  
}

struct node *R(struct node* a){//右旋
    struct node *A=a;
    struct node *B=a->l;
    struct node *tmp=B->r;

    B->r=A;
    A->l=tmp;

    return B;
}

struct node *L(struct node* a){//左旋
    struct node *A=a;
    struct node *B=a->r;
    struct node *tmp=B->l;

    B->l=A;
    A->r=tmp;

    return B;
}

int avl_factor(struct node *a){//通过正负性判断左右旋
    if(a==NULL) return 0;
    return max_depth(a->l)-max_depth(a->r);
}

struct node* rebalance(struct node *a){
    if(a==NULL) return a;
    int fac=avl_factor(a);
    if(fac>1&&avl_factor(a->l)>0) return R(a);
    if(fac<-1&&avl_factor(a->r)<=0) return L(a);
    if(fac>1&&avl_factor(a->l)<=0){//LR
        a->l=L(a->l);
        return R(a);
    }
    if(fac<-1&&avl_factor(a->r)>0){//RL
        a->r=R(a->r);
        return L(a);
    }
    return a;
}

struct node* struct_avl(struct node *a){
    NODE_NUM=0;
    pre_order(a);
    struct node* root=new_node(stack[0]);
    for(int i=1; i<NODE_NUM; i++){
        assert(root, stack[i]);
        struct node *adjust=search_parent(root,stack[i]);
        while(adjust!=root){
            adjust->l=rebalance(adjust->l);
            adjust->r=rebalance(adjust->r);
            adjust=search_parent(root,adjust->val);
        }
        root->l=rebalance(root->l);
        root->r=rebalance(root->r);
        root=rebalance(root);
    }
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
        if(strcmp(s,"delete")==0){
            int node;
            scanf("%d",&node);
            delete(root, node);
            plot(root);
        }
        if(strcmp(s,"sym")==0){
            sym(root);
            plot(root);
        }
        if(strcmp(s,"depth")==0){
            printf("%d\n", max_depth(root));
        }
        if(strcmp(s,"min_depth")==0){
            printf("%d\n", min_depth(root));
        }
        if(strcmp(s,"avl")==0){
            root=struct_avl(root);
            plot(root);
        }
        if(strcmp(s,"mid")==0){
            NODE_NUM=0;
            mid_order(root);
            print_stack();
        }
        if(strcmp(s,"pre")==0){
            NODE_NUM=0;
            pre_order(root);
            print_stack();
        }
    }
    
}