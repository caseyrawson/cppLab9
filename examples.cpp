#include "LinkedList.h"
#include <iostream>
#include <vector>
#include <string>

int main() {
    // List with 5 nodes
    std::vector<std::string> names = { "zero", "one", "two", "three", "four" };

    {
        std::cout << "Shared_ptr-only circular list:\n";

        // Build a simple ring of shared_ptrs
        std::vector<std::shared_ptr<Node>> nodes;
        for (auto& n : names)
            nodes.push_back(std::make_shared<Node>(n));
        for (size_t i = 0; i < nodes.size(); ++i)
            nodes[i]->next_shared = nodes[(i+1) % nodes.size()];

        std::cout << "clearing the vector of shared_ptrs \n";
        nodes.clear();  
        std::cout << "(no Node destructors printed)\n\n";
    }

    LinkedList list(names);

    std::cout << "Shared_ptr and weak_ptr circular list:\n";
    std::cout << "SERVER: build circular list\n";
    list.SERVER_buildCircularLinkedList();

    // SERVER traversal via shared_ptr
    std::cout << "\nSERVER_PRINT (shared_ptr traversal)\n";
    list.SERVER_printLinkedList();

    // CLIENT traversal via weak_ptr before deletion
    std::cout << "\nCLIENT_PRINT (weak_ptr traversal before delete)\n";
    list.CLIENT_printLinkedList();

    // SERVER deletes its shared_ptrs
    std::cout << "\nSERVER: deleting circular list\n";
    list.SERVER_deleteCircularLinkedList();

    // weak_ptrs should be expired
    std::cout << "\nCLIENT_PRINT (after deletion)\n";
    list.CLIENT_printLinkedList();

    return 0;
}
