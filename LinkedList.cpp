#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "include/doctest.h"
#include "LinkedList.h"
#include <sstream>   


TEST_CASE("Before deletion: weak pointers lock") {
    LinkedList list({ "zero", "one", "two", "three" });
    list.SERVER_buildCircularLinkedList();

    auto wks = list.getWeakNodes();
    REQUIRE(wks.size() == 4);
    for (auto& wkp : wks)
        CHECK(wkp.lock() != nullptr);
}

TEST_CASE("After deletion: weak pointers expired") {
    LinkedList list({ "a", "b", "c" });
    list.SERVER_buildCircularLinkedList();

    auto wks = list.getWeakNodes();
    list.SERVER_deleteCircularLinkedList();

    for (auto& wkp : wks)
        CHECK(wkp.expired());
}

TEST_CASE("CLIENT_printLinkedList output") {
    LinkedList list({ "x", "y" });
    list.SERVER_buildCircularLinkedList();

    // before deletion: should see next (weak):
    {
        std::ostringstream oss;
        list.CLIENT_printLinkedList(oss);
        std::string out = oss.str();
        CHECK(out.find("next (weak):") != std::string::npos);
    }

    // after deletion: should see Yipes!
    list.SERVER_deleteCircularLinkedList();
    {
        std::ostringstream oss;
        list.CLIENT_printLinkedList(oss);
        std::string out = oss.str();
        CHECK(out.find("Yipes!") != std::string::npos);
    }
}
