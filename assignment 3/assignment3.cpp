// Ece Çınar, 150150138

#include "hw3.h"
#include <iostream>
#include <vector>

using namespace std;

typedef struct HW3_Result Wareh;

struct Node{
    int ordinal_num;
    int package_size;
    char color;         //1 for red 0 for black
    Node* left = NULL;
    Node* right = NULL;
    Node* parent = NULL;
};


class RedBlackTree{
    private:
        Node* root;
        Node* kth;
        vector<Node*> minnodes;
        int count;
    public:
        RedBlackTree(){   root = NULL;    }
        Node* search(Node*,int);
        void insert(int,int);
        void colorInsert(Node*);
        void colorDelete(Node*);
        void deleteNode(Node*);
        Node* getKth(){    return kth; }
        void initKth(){   count = 0; kth = NULL; }
        Node* search(int);
        void findMinNodes(Node*,int);
        void print(Node*);
        void rotateRight(Node*);
        void rotateLeft(Node*);
        void transplant(Node*,Node*);
        void clearVector(){   minnodes.clear();   }
        Node* treeMin(Node*);
        Node * getRoot(){   return root;    }
        void findKthSmall(Node*,int);
        vector<Node*> getMinNodes(){  return minnodes;  }
};


/*******************************MINTREE   FUNCTION***********************************/
/* This function finds the minimum package sized node in a red-black tree and returns it*/

Node* RedBlackTree::treeMin(Node* n){
    while(n->left)
        n = n->left;
    return n;
}

/*******************************FIND KTH SMALLEST FUNCTION***********************************/
/* This function finds the minimum package sized node in a red-black tree and returns it*/


void RedBlackTree::findKthSmall(Node* n,int k){
    if(n==NULL)
        return;
    findKthSmall(n->left,k);
    if(count == k){
        kth = n;
        count += 1;
    }
    else
        count += 1;
    findKthSmall(n->right,k);
} 


/******************************FINDMINNODES FUNCTION**********************************/
/*Finds the nodes with minimum package size(from getminval) and adds them to minnodes vector*/

void RedBlackTree::findMinNodes(Node* n,int min_package){
    if(n==NULL)
        return;
    findMinNodes(n->left,min_package);
    if(n->package_size == min_package)
        minnodes.push_back(n);
    findMinNodes(n->right,min_package);
} 


/***********************SEARCH FUNCTION WITH TWO PARAMETERS****************************/
/*searchs the tree to find where to insert the new node */

Node* RedBlackTree::search(Node* n, int new_package){
    if(!n)
        return NULL;
    else if(n->package_size == new_package){
        if(n->right==NULL)
            return n;
        else if(n->right->package_size>=new_package)
            return search(n->right,new_package);
    }
    else if(n->package_size > new_package){
        if(!n->left)
            return n;
        else
            return search(n->left,new_package);
    }
    else if(n->package_size < new_package){
        if(!n->right)
            return n;
        else
            return search(n->right,new_package);
    }
    return root;
}



/*********************************INSERT FUNCTION**************************************/
/*inserts a node into the red black tree*/

void RedBlackTree::insert(int new_package,int new_ordinal){
    Node *temp = search(root,new_package);
    Node *newnode = new Node();
    newnode->package_size = new_package;
    newnode->ordinal_num = new_ordinal;
    if(temp==NULL){
        root = newnode;
        root->color = 'B'; //black
    }
    else if(new_package < temp->package_size){
        newnode->color = 'R'; //red
        temp->left = newnode;
        newnode->parent = temp;
    }

    else{
        newnode->color = 'R'; //red
        temp->right = newnode;
        newnode->parent = temp;
    }
    if(newnode->parent && newnode->parent->parent)
        colorInsert(newnode);
}


/*****************************COLOR FUNCTION FOR INSERT**********************************/
/* after insertion, tree colors and nodes are rearranged to maintain red black tree property*/

void RedBlackTree::colorInsert(Node* child){
    Node* uncle;
    while(child->parent->color == 'R'){
        if(child->parent == child->parent->parent->left){ //xin parentı left children
            if(child->parent->parent->right){
                uncle = child->parent->parent->right;
                if(uncle->color == 'R'){
                    child->parent->parent->color = 'R';
                    child->parent->color = 'B';
                    uncle->color = 'B';
                    child = child->parent->parent;
                }
                else{
                    if (child == child->parent->right) {
						child = child->parent;
						rotateLeft(child);
					}
                    child->parent->color = 'B';
                    child->parent->parent->color = 'R';
                    rotateRight(child->parent->parent);
                }
            }
            else if(!child->parent->parent->right){
                if (child == child->parent->right) {
                    child = child->parent;
                    rotateLeft(child);
				}
                child->parent->color = 'B';
                child->parent->parent->color = 'R';
                rotateRight(child->parent->parent);
            }
        }
        else{
            if(child->parent->parent->left){
                uncle = child->parent->parent->left;
                if(uncle->color == 'R'){
                    child->parent->parent->color = 'R';
                    child->parent->color = 'B';
                    uncle->color = 'B';
                    child = child->parent->parent;
                }
                else{
                    if (child == child->parent->left) {
						child = child->parent;
						rotateRight(child);
					}
                    child->parent->color = 'B';
                    child->parent->parent->color = 'R';
                    rotateLeft(child->parent->parent);
                }
            }
            else if(!child->parent->parent->left){
                if (child == child->parent->left) {
                    child = child->parent;
                    rotateRight(child);
                }
                child->parent->color = 'B';
                child->parent->parent->color = 'R';
                rotateLeft(child->parent->parent);
            }
        }
        if(child == root)
            break;
    }
    root->color = 'B';
}

/*********************************TRANSPLANT FUNCTION**************************************/
/* Performs new parent child assignments */

void RedBlackTree::transplant(Node* n,Node* m){
    if(n->parent == NULL)
        root=m;
    else if(n==n->parent->left)
        n->parent->left = m;
    else
        n->parent->right = m;
    if(m!=NULL)
        m->parent = n->parent;
}


/*********************************DELETE FUNCTION**************************************/
/* deletes a given node*/

void RedBlackTree::deleteNode(Node* m){
    bool fe = false;
    Node* x;
    Node* n = m;
    char n_color = n->color; 
    if(m->left==NULL && m->right==NULL){
        if(m->parent){
            x = m->parent;
            if(m == m->parent->left){
                m->parent->left = NULL;
                m->parent = NULL;
            }
            else{
                m->parent->right = NULL;
                m->parent = NULL;
            }
        }
        else{
            root = NULL;
            fe = true;
        }   
    }
    else if(m->left == NULL){
        x = m->right;
        transplant(m,m->right);
    }
    else if(m->right == NULL){
        x = m->left;
        transplant(m,m->left);
    }
    else{
        n=treeMin(m->right);
        n_color = n->color;
        x=n->right;
        if(n->parent == m){
            if(x)
                x->parent = n;
        }
        else{
            transplant(n,n->right);
            n->right = m->right;
            n->right->parent = n;
        }
        transplant(m,n);
        n->left = m->left;
        n->left->parent = n;
        n->color = m->color;
    }
    // if(n_color == 'B' and fe==false){
    //     colorDelete(x);
    // }
}

/*****************************COLOR FUNCTION FOR DELETE**********************************/
/* after deletion, tree colors and nodes are rearranged to maintain red black tree property*/

void RedBlackTree::colorDelete(Node* x){
    Node* w;
    while(x != root && x->color == 'B'){
        if(x == x->parent->left){
            if(x->parent->right){
                w = x->parent->right;
                if(w->color == 'R'){
                    w->color = 'B';
                    x->parent->color = 'R';
                    rotateLeft(x->parent);
                    w = x->parent->right;
                }
                if((w->left->color == 'B' && w->right->color == 'B') || (w->left==NULL && w->right==NULL)){
                    w->color = 'R';
                    x = x->parent;
                }
                else{
                    if(w->right->color == 'B'){
                        w->left->color = 'B';
                        w->color = 'R';
                        rotateRight(w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = 'B';
                    w->right->color = 'B';
                    rotateLeft(x->parent);
                    x = root;
                }
            }
        }
        else{
            if(x->parent->left){
                w = x->parent->left;
                if(w->color == 'R'){
                    w->color = 'B';
                    x->parent->color = 'R';
                    rotateRight(x->parent);
                    w = x->parent->left;
                }
                if(w->left->color == 'B' && w->right->color == 'B'){
                    w->color = 'R';
                    x = x->parent;
                }
                else{
                    if(w->left->color == 'B'){
                        w->right->color = 'B';
                        w->color = 'R';
                        rotateLeft(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = 'B';
                    w->left->color = 'B';
                    rotateRight(x->parent);
                    x = root;
                }
            }
        }
    }
    x->color = 'B';
} 

/*****************************INORDER PRINT FUNCTION**********************************/
/* to be able to observe the states of tree during development*/
void RedBlackTree::print(Node* n){
    if(n==NULL)
        return;
    print(n->left);
    cout << n->package_size << " " <<  n->ordinal_num << " " << n->color << endl;
    print(n->right);
} 


/******************************ROTATE LEFT FUNCTION**********************************/
/* rotates the given subtree left*/
void RedBlackTree::rotateLeft(Node* n) {
		Node* m = n->right;
		n->right = m->left;
		if (m->left != NULL) {
			m->left->parent = n;
		}
		m->parent = n->parent;
		if (n->parent == NULL) 
			root = m;
        else if (n == n->parent->left) 
			n->parent->left = m;
        else
			n->parent->right = m;
		m->left = n;
		n->parent = m;
}



/******************************ROTATE RIGHT FUNCTION**********************************/
/*rotates the given subtree right*/

void RedBlackTree::rotateRight(Node* n) {
    Node* m = n->left;
    n->left = m->right;
    if (m->right != NULL) {
        m->right->parent = n;
    }
    m->parent = n->parent;
    if (n->parent == NULL)
        root = m;
    else if (n == n->parent->right)
        n->parent->right = m;
    else
        n->parent->left = m;
    m->right = n;
    n->parent = m;
}

/********************************HW3 FUNCTION**********************************************/
/*function which arranges the shipments between two warehouses*/

Wareh hw3(int eastWarehousePackageCount, int eastWarehousePackageSizes [],
                int eastWarehousePackageOrdinals [], int westWarehousePackageCount,
                int westWarehousePackageSizes [], int westWarehousePackageOrdinals [] ){ 
    
    int east_size = eastWarehousePackageCount;
    int west_size = westWarehousePackageCount;

    RedBlackTree eastwh;
    RedBlackTree westwh;

    int turn = 0;
    Wareh h;
    Node* to_be_sent;
    h.blackNodeCount = 0; h.redNodeCount = 0; h.packageCount = 0;
    vector<Node*> min_east,min_west;

    for(int i = 0; i < eastWarehousePackageCount; i++)
        eastwh.insert(eastWarehousePackageSizes[i],eastWarehousePackageOrdinals[i]);
    for(int i = 0; i < westWarehousePackageCount; i++)
        westwh.insert(westWarehousePackageSizes[i],westWarehousePackageOrdinals[i]);

    int minord,minidx;

    eastwh.findMinNodes(eastwh.getRoot(),eastwh.treeMin(eastwh.getRoot())->package_size);
    westwh.findMinNodes(westwh.getRoot(),westwh.treeMin(westwh.getRoot())->package_size);

    min_east = eastwh.getMinNodes();

    cout << "East Warehouse after insertion: " << endl;
    eastwh.print(eastwh.getRoot());
      
    cout << "West Warehouse after insertion: " << endl;
    westwh.print(westwh.getRoot());

    for(int i = 0; i < min_east.size(); i++){
        if(i==0){
            minord = min_east[0]->ordinal_num;
            minidx = 0;
        }
        else{
            if(min_east[i]->ordinal_num < minord){
                minord= min_east[i]->ordinal_num;
                minidx = i;
            }
        }
        eastwh.deleteNode(min_east[i]);
        east_size -= 1;
    }

    while(1){
        if(turn==0){ //west warehouse
            if(minord <= west_size || eastwh.getRoot()==NULL){
                if(eastwh.getRoot()){
                    westwh.initKth();
                    westwh.findKthSmall(westwh.getRoot(),minord-1);
                    min_west.clear();
                    min_west.push_back(westwh.getKth()); // will be sent
                    cout << "Package to be sent from West House = Package size " << min_west[0]->package_size <<", ordinal " << min_west[0]->ordinal_num << endl;
                    westwh.deleteNode(min_west[0]);
                    west_size -= 1;
                    minord = min_west[0]->ordinal_num;
                }
                for(int i = 0; i < min_east.size(); i++){
                    westwh.insert(min_east[i]->package_size,min_east[i]->ordinal_num);
                    west_size += 1;
                }
                if(eastwh.getRoot()==NULL)
                    break;
            }
            else{
                if(eastwh.getRoot()==NULL)
                    break;
                westwh.clearVector();
                westwh.findMinNodes(westwh.getRoot(),westwh.treeMin(westwh.getRoot())->package_size);
                min_west=westwh.getMinNodes(); //will be sent
                for(int i = 0; i < min_west.size(); i++){
                    if(i==0){
                        minord = min_west[0]->ordinal_num;
                        minidx = 0;
                    }
                    else{
                        if(min_west[i]->ordinal_num < minord){
                            minord= min_west[i]->ordinal_num;
                            minidx = i;
                        }
                    }         
                    westwh.deleteNode(min_west[i]);
                    west_size -= 1;
                }
            }
            turn = 1;
        }
        if(turn==1){ //east warehouse
            if(minord <= east_size || westwh.getRoot()==NULL){
                if(westwh.getRoot()){
                    eastwh.initKth();
                    eastwh.findKthSmall(eastwh.getRoot(),minord-1);
                    min_east.clear();
                    min_east.push_back(eastwh.getKth()); // will be sent
                    cout << "Package to be sent from East House = Package size " << min_east[0]->package_size <<", ordinal " << min_east[0]->ordinal_num << endl;
                    eastwh.deleteNode(min_east[0]);
                    east_size -= 1;
                    minord = min_east[0]->ordinal_num;
                }
                for(int i = 0; i < min_west.size(); i++){
                    eastwh.insert(min_west[i]->package_size,min_west[i]->ordinal_num);
                    east_size += 1;
                }
                if(westwh.getRoot()==NULL)
                    break;
            }
            else{
                if(westwh.getRoot()==NULL)
                    break;
                eastwh.clearVector();
                eastwh.findMinNodes(eastwh.getRoot(),eastwh.treeMin(eastwh.getRoot())->package_size);
                min_east=eastwh.getMinNodes(); //will be sent
                for(int i = 0; i < min_east.size(); i++){
                    if(i==0){
                        minord = min_east[0]->ordinal_num;
                        minidx = 0;
                    }
                    else{
                        if(min_east[i]->ordinal_num < minord){
                            minord= min_east[i]->ordinal_num;
                            minidx = i;
                        }
                    }         
                    eastwh.deleteNode(min_east[i]);
                    east_size -= 1;
                }
            }
            turn = 0;
        }
    }
    if(eastwh.getRoot()){
        cout << "West Warehouse is empty." << endl;
        cout << "Contents of East Warehouse:" << endl;
        eastwh.print(eastwh.getRoot());
        cout << "Package count: " << east_size << endl;
        h.packageCount = east_size;
    }      
    else{
        cout << "East Warehouse is empty." << endl;
        cout << "Contents of West Warehouse:" << endl;
        westwh.print(westwh.getRoot());
        cout << "Package count: " << west_size << endl;
        h.packageCount = west_size;
    }
    return h;
}

