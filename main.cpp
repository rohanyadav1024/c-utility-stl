#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include "DoublyLinkedList.h"
#include "SinglyLinkedList.h"
// #include "BinaryTree.h"
#include <BinaryTree.h>
#include <Graph.h>

using namespace std;

int main(){
    // BinarySearchTree<int> bst();
    // BinarySearchTree<int> bst1({12, 4, 200, 400, 1002, 1});
    // // bst.append(10);
    // // bst.append(5);
    // // bst.append(15);
    // // bst.append(3);

    // bst1.printTree();
    // // for(auto i : bst.getPostorder()){
    // //     cout << i << " ";
    // // }
    // BinarySearchTree<int> bst2({34, 89, 167, 45});
    // bst2.printTree();
    // // bst.append(10);
    // // bst.append(5);
    // // bst.append(15);
    // // bst.append(3);

    // cout<<"Sum of all nodes in bst1: "<<bst1.getSubtreeSum(4)<<endl;
    // cout<<"Sum of all nodes in bst2: "<<bst2.getSubtreeSum(89)<<endl;

    // bst1.printLevelOrder();
    // cout<<"Shortest path between 1 and 1002 "<<bst1.getShortestPath(12, 1002)<<endl;
    // cout<<"path between 1 and 1002 "<<endl;
    // for(auto i : bst1.getPathBetweenNodes(4, 1002)){
    //     cout << i << " ";
    // }
    // // cout<<"LCA of nodes 1 and 1002 "<<bst1.getLCA(1, 1002)->data<<endl;

    // // bst1 = bst2;
    // // bst1.mergeBST(bst2);
    // // bst1.printTree();
    // // bst1.printTree();

    // Graph<int> g;
    // g.addEdge(1, 2);
    // g.addEdge(1, 3);
    // g.addEdge(2, 4);
    // g.addEdge(2, 5);

    // Graph<int> g({{1, 2}, {1, 3}, {2, 4}, {2, 5}, {5, 3}, {3, 2}}, true);
    Graph<int> g({{1, 2},{2, 4},{2,3}, {4, 5}, {5, 6}, {3, 7},{3, 8}, {8,7}}, true);
    // Graph<string> g1({{"Alpha", "Beta", 3}, {"Alpha", "Chad", 2}, {"Beta", "Delta", 5}, {"Beta", "Ephan", 7}});


    g.printGraph();
    // g1.printGraph();

    // auto p = g.getSinglePointShortestPath(1);

    for(auto &[v, w]: g.getSinglePointShortestPath(1)){
        cout<<v<<" "<<w<<endl;
    }
    // for(auto &i: g.getTopologicalOrderDFS()){
    //     cout<<i<<" ";
    // }cout<<endl;

    // for(auto &i: g.getTopologicalOrderBFS()){
    //     cout<<i<<" ";
    // }cout<<endl;

    return 0;
}