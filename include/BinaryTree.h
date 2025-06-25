#ifndef BinarySearchTree_H
#define BinarySearchTree_H

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

template <typename T>
class BinarySearchTree {
public:
    struct TreeNode {
        T data;
        TreeNode* left;
        TreeNode* right;
        TreeNode(const T& data, TreeNode* left = nullptr, TreeNode* right = nullptr) : data(data), left(left), right(right) {}
    };

private:
    TreeNode* root;
    TreeNode* lastNode;
    int size;

    TreeNode* insertTreeNode(TreeNode* root, TreeNode *node);
    TreeNode* removeTreeNode(TreeNode* node, const T& data);
    void clearTree(TreeNode* node);
    int sumSubtree(TreeNode* node);
    TreeNode* findLCA(TreeNode* node, const T& x, const T& y);
    int findShortestPath(TreeNode* node, const T& x, const T& y, int& distX, int& distY, int dist);
    TreeNode* searchNode(TreeNode* node, const T& data);
    void inorderTraversal(TreeNode* node, std::vector<T>& result);
    void preorderTraversal(TreeNode* node);
    void postorderTraversal(TreeNode* node);
    TreeNode* createTree(const std::vector<T>& data, int start, int end);
    int findDistance(TreeNode* node, const T& target, int dist);
    bool getPath(TreeNode* root, T target, std::vector<T>& path);

public:
    BinarySearchTree() : root(nullptr), size(0) {}
    BinarySearchTree(const T& data) : root(new TreeNode(data)), size(1) {}
    BinarySearchTree(const std::vector<T>& data) : root(nullptr), size(0) {
        for(const T& d : data) {
            append(d);
            // insertTreeNode(root, new TreeNode(d));
        }
    }
    BinarySearchTree(const BinarySearchTree &other) : root(nullptr), size(0) {
        if (other.root == nullptr) {
            return;
        }
        const std::vector<T> inorder = other.getInorder();
        root = createTree(inorder, 0, inorder.size() - 1);
        size = other.size;
    }
    BinarySearchTree &operator=(const BinarySearchTree &other) {
        if (this == &other || other.root == nullptr) {
            return *this;
        }
        clear();
        const std::vector<T> inorder = other.getInorder();
        root = createTree(inorder, 0, inorder.size() - 1);
        size = other.size;
        return *this;
    }
    ~BinarySearchTree() { clear(); }

    void append(const T& data);
    void remove(const T& data);
    void clear();
    void printTree(); // In-order traversal
    void printPreorder(); // In-order traversal
    void printPostorder(); // In-order traversal
    void printLevelOrder(); // In-order traversal
    int getSize() const { return size; }
    int getHeight();
    TreeNode* search(const T& data);
    TreeNode* getRoot() const { return root; }
    TreeNode* getLCA(const T& x, const T& y);
    int getSubtreeSum(const T& data);
    int getShortestPath(const T& x, const T& y);
    void mergeBST(BinarySearchTree<T> bst22);
    TreeNode* getSuccesor(TreeNode* node);
    TreeNode* getPredecessor(TreeNode* node);

    //get iterators for inorder, preorder, postorder, levelorder
    std::vector<T> getInorder() const;
    std::vector<T> getPreorder() const;
    std::vector<T> getPostorder() const;
    std::vector<std::vector<T>> getLevelorder() const;
    std::vector<T> getPathBetweenNodes(const T& x, const T& y);
};

// Implementation of the class methods
template <typename T>
typename BinarySearchTree<T>::TreeNode* BinarySearchTree<T>::insertTreeNode(TreeNode* root, TreeNode* node) {
    if(root == nullptr) {
        size++;
        return node;
    }

    if(node->data < root->data) {
        root->left = insertTreeNode(root->left, node);
    } else {
        root->right = insertTreeNode(root->right, node);
    }

    return root;
}

template <typename T>
typename BinarySearchTree<T>::TreeNode* BinarySearchTree<T>::removeTreeNode(TreeNode* node, const T& data) {
    if(node == nullptr) {
        return nullptr;
    }

    if(data < node->data) {
        node->left = removeTreeNode(node->left, data);
    } else if(data > node->data) {
        node->right = removeTreeNode(node->right, data);
    } else {
        if(node->left == nullptr) {
            TreeNode* temp = node->right;
            delete node;
            size--;
            return temp;
        } else if(node->right == nullptr) {
            TreeNode* temp = node->left;
            delete node;
            size--;
            return temp;
        }

        //get the inorder successor
        TreeNode* temp = node->right;
        while(temp->left != nullptr) {
            temp = temp->left;
        }
        node->data = temp->data;
        node->right = removeTreeNode(node->right, temp->data);
    }
    return node;
}

template <typename T>
void BinarySearchTree<T>::append(const T& data) {
    if(root == nullptr){
        root = new TreeNode(data);
        size++;
        return;
    }
    TreeNode* node = new TreeNode(data);
    insertTreeNode(root, node);
}

template <typename T>
void BinarySearchTree<T>::remove(const T& data) {
    root = removeTreeNode(root, data);
}

template <typename T>
void BinarySearchTree<T>::clearTree(TreeNode* node) {
    if(node == nullptr) {
        return;
    }
    clearTree(node->left);
    clearTree(node->right);
    delete node;
}

template <typename T>
void BinarySearchTree<T>::clear() {
    clearTree(root);
    root = nullptr;
    size = 0;
}

template <typename T>
void BinarySearchTree<T>::printTree() {
    if(root == nullptr) {
        return;
    }

    for(auto &data : getInorder()) {
        std::cout << data << " ";
    }
    std::cout<<std::endl;
}

template <typename T>
void BinarySearchTree<T>::printPreorder() {
    if(root == nullptr) {
        return;
    }

    for(auto &data : getPostorder()) {
        std::cout << data << " ";
    }
    std::cout<<std::endl;
}

template <typename T>
void BinarySearchTree<T>::printPostorder() {
    if(root == nullptr) {
        return;
    }

    for(auto &data : getPreorder()) {
        std::cout << data << " ";
    }
    std::cout<<std::endl;
}


template <typename T>
void BinarySearchTree<T>::printLevelOrder() {
    if(root == nullptr) {
        return;
    }

    for(auto &row : getLevelorder()) {
        for(auto &data : row) {
            std::cout << data << " ";
        }
        std::cout<<std::endl;
    }
    // std::cout<<std::endl;
}


template <typename T>
int BinarySearchTree<T>::getHeight() {
    if(root == nullptr) {
        return 0;
    }

    std::queue<TreeNode*> q;
    q.push(root);
    int height = 0;
    while(!q.empty()) {
        int size = q.size();
        while(size--) {
            TreeNode* curr = q.front();
            q.pop();
            if(curr->left != nullptr) {
                q.push(curr->left);
            }
            if(curr->right != nullptr) {
                q.push(curr->right);
            }
        }
        height++;
    }
    return height;
}

template <typename T>
typename BinarySearchTree<T>::TreeNode* BinarySearchTree<T>::searchNode(TreeNode* node, const T& data) {
    if(node == nullptr || node->data == data) {
        return node;
    }

    if(data < node->data) {
        return searchNode(node->left, data);
    } else {
        return searchNode(node->right, data);
    }
}

template <typename T>
typename BinarySearchTree<T>::TreeNode* BinarySearchTree<T>::search(const T& data) {
    return searchNode(root, data);
}

template <typename T>
typename BinarySearchTree<T>::TreeNode* BinarySearchTree<T>::findLCA(TreeNode* node, const T& x, const T& y) {
    if(node == nullptr) {
        return nullptr;
    }

    if(node->data == x || node->data == y) {
        return node;
    }

    TreeNode* left = findLCA(node->left, x, y);
    TreeNode* right = findLCA(node->right, x, y);

    if(left != nullptr && right != nullptr) {
        return node;
    }

    return left != nullptr ? left : right;
}

template <typename T>
typename BinarySearchTree<T>::TreeNode* BinarySearchTree<T>::getLCA(const T& x, const T& y) {
    return findLCA(root, x, y);
}

template <typename T>
int BinarySearchTree<T>::sumSubtree(TreeNode* node) {
    if(node == nullptr) {
        return 0;
    }

    return node->data + sumSubtree(node->left) + sumSubtree(node->right);
}

template <typename T>
int BinarySearchTree<T>::getSubtreeSum(const T& data) {
    TreeNode* node = search(data);
    if(node == nullptr) {
        return 0;
    }

    return sumSubtree(node);
}

template <typename T>
int BinarySearchTree<T>::findDistance(TreeNode* node, const T& target, int dist) {
    if (node == nullptr) {
        return -1;
    }

    if (node->data == target) {
        return dist;
    }

    int leftDist = findDistance(node->left, target, dist + 1);
    if (leftDist != -1) {
        return leftDist;
    }

    return findDistance(node->right, target, dist + 1);
}

template <typename T>
int BinarySearchTree<T>::getShortestPath(const T& x, const T& y) {
    // Find the LCA of x and y
    TreeNode* lca = findLCA(root, x, y);

    if (lca == nullptr) {
        return -1;  // One or both nodes are not present in the tree
    }

    // Find the distance from LCA to x and LCA to y
    int distX = findDistance(lca, x, 0);
    int distY = findDistance(lca, y, 0);

    return distX + distY;  // Shortest path between x and y
}


template <typename T>
void BinarySearchTree<T>::inorderTraversal(TreeNode* node, std::vector<T>& result) {
    if(node == nullptr) {
        return;
    }

    inorderTraversal(node->left, result);
    result.push_back(node->data);
    inorderTraversal(node->right, result);
}

template <typename T>
std::vector<T> BinarySearchTree<T>::getInorder() const{
    std::vector<T> result;
    std::stack<TreeNode*> s;
    TreeNode* curr = root;
    while(curr != nullptr || !s.empty()) {
        while(curr != nullptr) {
            s.push(curr);
            curr = curr->left;
        }
        curr = s.top();
        s.pop();
        result.push_back(curr->data);
        curr = curr->right;
    }
    // inorderTraversal(root, result);
    return result;
}

template <typename T>
std::vector<T> BinarySearchTree<T>::getPreorder() const{
    std::vector<T> result;
    std::stack<TreeNode*> s;
    s.push(root);
    while(!s.empty()) {
        TreeNode* curr = s.top();
        s.pop();
        result.push_back(curr->data);
        if(curr->right != nullptr) {
            s.push(curr->right);
        }
        if(curr->left != nullptr) {
            s.push(curr->left);
        }
    }
    return result;
}

template <typename T>
std::vector<T> BinarySearchTree<T>::getPostorder() const{
    std::vector<T> result;
    std::stack<TreeNode*> s;
    TreeNode* curr = root;
    while(curr != nullptr || !s.empty()) {
        while(curr != nullptr) {
            s.push(curr);
            s.push(curr);
            curr = curr->left;
        }
        curr = s.top();
        s.pop();
        if(!s.empty() && s.top() == curr) {
            curr = curr->right;
        } else {
            result.push_back(curr->data);
            curr = nullptr;
        }
    }
    return result;
}

template <typename T>
std::vector<std::vector<T>> BinarySearchTree<T>::getLevelorder() const{
    std::vector<std::vector<T>> result;
    std::vector<T> level;
    if(root == nullptr) {
        return result;
    }

    std::queue<TreeNode*> q;
    q.push(root);
    q.push(nullptr);
    while(!q.empty()) {
        TreeNode* curr = q.front();
        q.pop();

        if(curr == nullptr) {
            if(!level.empty()) {
                result.push_back(level);
                level.clear();
                if(!q.empty()) {
                    q.push(nullptr);
                }
            }
        }
        else{
            level.push_back({curr->data});
            if(curr->left != nullptr) {
                q.push(curr->left);
            }
            if(curr->right != nullptr) {
                q.push(curr->right);
            }
        }
    }
    return result;
}

template <typename T>
typename BinarySearchTree<T>::TreeNode* BinarySearchTree<T>::createTree(const std::vector<T>& data, int start, int end) {
    if(start > end) {
        return nullptr;
    }

    int mid = start + (end - start) / 2;
    TreeNode* node = new TreeNode(data[mid]);
    node->left = createTree(data, start, mid - 1);
    node->right = createTree(data, mid + 1, end);
    return node;
}

template <typename T>
void BinarySearchTree<T>::mergeBST(BinarySearchTree<T> bst2){
    TreeNode* root2 = bst2.getRoot();
    
    if(root == nullptr){
        *this = bst2;
        return;
    }
    if(root2 == nullptr){
        return;
    }

    std::vector<T> inorder1 = getInorder();
    std::vector<T> inorder2 = bst2.getInorder();

    std::vector<T> merged;
    int i = 0, j = 0;
    while(i < inorder1.size() && j < inorder2.size()){
        if(inorder1[i] < inorder2[j]){
            merged.emplace_back(inorder1[i++]);
        }
        else{
            merged.emplace_back(inorder2[j++]);
        }
    }

    while(i < inorder1.size()){
        merged.emplace_back(inorder1[i++]);
    }
    while(j < inorder2.size()){
        merged.emplace_back(inorder2[j++]);
    }

    // copy the merged vector to the tree
    clear();
    inorder1.clear();
    inorder2.clear();
    root = createTree(merged, 0, merged.size() - 1);
    size = merged.size();

}


template <typename T>
bool BinarySearchTree<T>::getPath(TreeNode* root, T target, std::vector<T>& path) {
    if (root == nullptr) {
        return false;
    }

    path.push_back(root->data);
    if (root->data == target) {
        return true;
    }

    if (getPath(root->left, target, path) || getPath(root->right, target, path)) {
        return true;
    }

    path.pop_back();
    return false;
}

template <typename T>
std::vector<T> BinarySearchTree<T>::getPathBetweenNodes(const T& x, const T& y) {
    std::vector<T> path1, path2;

    TreeNode* lca = getLCA(x, y);
    getPath(lca, x, path1);
    getPath(lca, y, path2);

    std::vector<T> path;
    path1.erase(path1.begin());

    if(x < y){
        std::reverse(path1.begin(), path1.end());
        path.insert(path.end(), path1.begin(), path1.end());
        path.insert(path.end(), path2.begin(), path2.end());
    }
    else{
        std::reverse(path2.begin(), path2.end());
        path.insert(path.end(), path2.begin(), path2.end());
        path.insert(path.end(), path1.begin(), path1.end());
    }

    return path;
}

#endif