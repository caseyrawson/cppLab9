#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

struct Node {
    std::string name;
    std::shared_ptr<Node> next_shared;
    std::weak_ptr<Node>   next_weak;

    Node(const std::string& n) : name(n) {}
    ~Node() { std::cout << "Node [" << name << "] destructor called\n"; }
};

class LinkedList {
    std::vector<std::shared_ptr<Node>> nodes;
    std::vector<std::weak_ptr<Node>> weak_nodes; 
    std::shared_ptr<Node> root;

public:
    LinkedList(const std::vector<std::string>& names) {
        nodes.reserve(names.size());
        for (auto& n : names)
            nodes.push_back(std::make_shared<Node>(n));
    }

    void SERVER_buildCircularLinkedList() {
        auto N = nodes.size();
        root = nodes[0];
        for (size_t i = 0; i < N; ++i) {
            auto next = nodes[(i+1)%N];
            nodes[i]->next_shared = next;
            nodes[i]->next_weak   = next;
        }

        // capture weak ptrs for printing
        weak_nodes.clear();
        for (auto& shp : nodes) {
            weak_nodes.push_back(shp->next_weak);
        }
    }

    // default to cout ostream
    void SERVER_printLinkedList(std::ostream& out = std::cout) const {
        if (!root) { out << "[empty]\n"; return; }
        auto cur = root;
        do {
            out << "[" << cur->name << "] : use_count: "
                << cur.use_count()
                << " address: " << cur.get()
                << " next (shared): " << cur->next_shared.get()
                << "\n";
            cur = cur->next_shared;
        } while (cur != root);
    }

    void CLIENT_printLinkedList(std::ostream& out = std::cout) const {
        if (weak_nodes.empty()) { out << "[empty]\n"; return; }
        for (auto& wk : weak_nodes) {
            if (auto shp = wk.lock()) {
                out << "[" << shp->name << "] next (weak): " << shp.get() << "\n";
            } else {
                out << "Yipes! shared_ptr not available\n";
            }
        }
    }

    void SERVER_deleteCircularLinkedList() {
        for (auto& shp : nodes) {
            shp->next_shared.reset();
        }
        root.reset();
        nodes.clear();
    }

    // simple, for tests
    std::vector<std::weak_ptr<Node>> getWeakNodes() const {
        return weak_nodes;
    }
};

#endif
