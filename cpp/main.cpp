/*!
    \file "main.cpp"

    Author: Matt Ervin <matt@impsoftware.org>
    Formatting: 4 spaces/tab (spaces only; no tabs), 120 columns.
    Doc-tool: Doxygen (http://www.doxygen.com/)

    https://leetcode.com/problems/merge-k-sorted-lists/
*/

//!\sa https://github.com/doctest/doctest/blob/master/doc/markdown/main.md
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h> //!\sa https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md
#include <iterator>
#include <vector>

using namespace std;

// [----------------(120 columns)---------------> Module Code Delimiter <---------------(120 columns)----------------]

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    /*!
        Merge the first and second lists.
        Merge the result with the third list.
        Etc... until the last list is merged with the result.

        Time = O(n^2)
               Where n == sum of all list sizes, i.e. total number of list items.
               Every node in every list must be visited up to n times (theoretically).
        
        Space O(1)
    */
    ListNode* mergeKLists_bruteForce(vector<ListNode*>& lists) {
        ListNode* result{};

        if (!lists.empty()) {
            if (1 == lists.size()) {
                result = lists.front();
            } else {
                // Find first non-empty list.
                vector<ListNode*>::iterator first{};
                for (first = lists.begin(); lists.end() != first && !*first; ++first) {}
                auto const first_node_exists = lists.end() != first;
                if (first_node_exists) {
                    // For each remaining non-empty list:
                    auto* firstNode = *first;
                    for (auto second = first + 1; lists.end() != second; ++second) {
                        auto* secondNode = *second;
                        ListNode* resultTail = nullptr;
                        if (secondNode) {
                            // Iterate over each list and combine into result.
                            while (firstNode && secondNode) {
                                auto& lesserNode = firstNode->val <= secondNode->val ? firstNode : secondNode;

                                // Append node to result.
                                if (!result) { result = lesserNode; }
                                if (resultTail) { resultTail->next = lesserNode; }
                                resultTail = lesserNode;

                                // Advance to next node.
                                lesserNode = lesserNode->next;
                            }
                        }

                        // Append remaining first list nodes, if any.
                        for (; firstNode; firstNode = firstNode->next) {
                            if (!result) { result = firstNode; }
                            if (resultTail) { resultTail->next = firstNode; }
                            resultTail = firstNode;
                        }

                        // Append remaining second list nodes, if any.
                        for (; secondNode; secondNode = secondNode->next) {
                            if (!result) { result = secondNode; }
                            if (resultTail) { resultTail->next = secondNode; }
                            resultTail = secondNode;
                        }
                        
                        resultTail = nullptr;
                        firstNode = result;
                        result = nullptr;
                    }

                    result = firstNode;
                }
            }
        }
        
        return result;
    }
    
    /*!
        Add the first item in each list to a min-heap.
        Each item in the heap refers to it's list.
        Pop the item from the heap.
        Add the item to the new list.
        Remove the item from its original list.
        Add the next item from the original list to the min heap.
        Continue until the heap is empty.

        Time = O(n*log(k))
               Where: n is the sum of all list sizes, i.e. total number of list items.
                      k is the number of lists.
    
        Space = O(k)
    */
    ListNode* mergeKLists_heap(vector<ListNode*>& lists) {
        //
        //!\todo TODO: >>> Under Construction <<<
        //
        return nullptr;
    }
    
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        return mergeKLists_bruteForce(lists);
    }
};

// [----------------(120 columns)---------------> Module Code Delimiter <---------------(120 columns)----------------]

vector<ListNode*> makeLists(vector<vector<int>> const& lists) {
    vector<ListNode*> result{};
    for (auto const& list : lists) {
        ListNode *head{}, *tail{};
        for (auto const& element : list) {
            ListNode* node = new ListNode{element};
            if (!head) { head = node; }
            if (tail) { tail->next = node; }
            tail = node;
        }
        result.emplace_back(std::move(head));
    }
    return result;
}

void freeList(ListNode* list) {
    while (list) {
        auto next = list->next;
        delete list;
        list = next;
    }
}

void freeLists(vector<ListNode*> lists) {
    for (auto& list : lists) {
        freeList(list);
    }
}

bool operator==(vector<int> const& expected, ListNode const* list) {
    auto expectedIter = expected.begin();
    for (; expected.end() != expectedIter && list; ++expectedIter, list = list->next) {
        if (*expectedIter != list->val) {
            return false;
        }
    }
    return expected.end() == expectedIter && !list;
}

TEST_CASE("Case 1")
{
    cerr << "Case 1" << '\n';
    auto mergedList = [&]{
        auto lists = makeLists(vector<vector<int>>{
            vector<int>{1, 4, 5}
            , vector<int>{1, 3, 4}
            , vector<int>{2, 6}
        });
        ListNode* mergedList = Solution{}.mergeKLists(lists);
        return mergedList;
    }();
    CHECK(vector<int>{1, 1, 2, 3, 4, 4, 5, 6} == mergedList);
    freeList(mergedList);
    cerr << '\n';
}

TEST_CASE("Case 2")
{
    cerr << "Case 2" << '\n';
    auto mergedList = [&]{
        auto lists = makeLists(vector<vector<int>>{});
        ListNode* mergedList = Solution{}.mergeKLists(lists);
        return mergedList;
    }();
    CHECK(vector<int>{} == mergedList);
    freeList(mergedList);
    cerr << '\n';
}

TEST_CASE("Case 4")
{
    cerr << "Case 4" << '\n';
    auto mergedList = [&]{
        auto lists = makeLists(vector<vector<int>>{
            vector<int>{2}
            , vector<int>{}
            , vector<int>{-1}
        });
        ListNode* mergedList = Solution{}.mergeKLists(lists);
        return mergedList;
    }();
    CHECK(vector<int>{-1, 2} == mergedList);
    freeList(mergedList);
    cerr << '\n';
}

/*
    End of "main.cpp"
*/
