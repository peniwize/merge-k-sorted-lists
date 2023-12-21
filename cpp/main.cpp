/*!
    \file "main.cpp"

    Author: Matt Ervin <matt@impsoftware.org>
    Formatting: 4 spaces/tab (spaces only; no tabs), 120 columns.
    Doc-tool: Doxygen (http://www.doxygen.com/)

    https://leetcode.com/problems/merge-k-sorted-lists/
*/

//!\sa https://github.com/doctest/doctest/blob/master/doc/markdown/main.md
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <algorithm>
#include <doctest/doctest.h> //!\sa https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md
#include <functional>
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

bool operator<(ListNode const& left, ListNode const& right) {
    return left.val < right.val;
}

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
    ListNode* mergeKLists_bruteForce(vector<ListNode*>& lists) const {
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
    ListNode* mergeKLists_heap(vector<ListNode*>& lists) const {
        ListNode* result = nullptr;
        
        auto const minHeapCap = lists.size();
        size_t minHeapSize = 0;
        ListNode* minHeap[minHeapCap];
        for (auto& list : lists) {
            if (list) {
                minHeap[minHeapSize++] = list;
            }
        }
        heapify(minHeap, minHeapSize);
        
        ListNode* resultTail = nullptr;
        while (0 < minHeapSize) {
            // Pop top item off of heap.
            auto node = minHeap[0];
            minHeapSize = heapPop(minHeap, minHeapSize);

            // Append popped item to result.
            if (!result) { result = node; }
            if (resultTail) { resultTail->next = node; }
            resultTail = node;
            if (node->next) { minHeapSize = heapPush(minHeap, minHeapSize, node->next); }
            resultTail->next = nullptr;
        }
        
        return result;
    }

    template <typename t_compare_t = std::less<ListNode>>
    void heapify(ListNode* heap[], size_t heapSize) const {
        for (decltype(heapSize) idx = 1; heapSize > idx; ++idx) {
            heapifyUp(heap, idx);
        }
    }

    template <typename t_compare_t = std::less<ListNode>>
    void heapifyUp(ListNode* heap[], size_t itemIdx) const {
        while (0 < itemIdx) {
            auto const parentIdx = (itemIdx - 1) >> 1;
            if (t_compare_t{}(*heap[itemIdx], *heap[parentIdx])) {
                std::swap(heap[parentIdx], heap[itemIdx]);
                itemIdx = parentIdx;
            } else {
                break;
            }
        }
    }

    template <typename t_compare_t = std::less<ListNode>>
    void heapifyDown(ListNode* heap[], size_t const heapSize, size_t itemIdx) const {
        while (heapSize > itemIdx) {
            auto const leftChildIdx = (itemIdx << 1) + 1;
            auto const rightChildIdx = (itemIdx << 1) + 2;
            auto swapIdx = itemIdx;
            if (heapSize > leftChildIdx && t_compare_t{}(*heap[leftChildIdx], *heap[swapIdx])) {
                swapIdx = leftChildIdx;
            }
            if (heapSize > rightChildIdx && t_compare_t{}(*heap[rightChildIdx], *heap[swapIdx])) {
                swapIdx = rightChildIdx;
            }
            if (swapIdx == itemIdx) {
                break;
            }
            std::swap(heap[swapIdx], heap[itemIdx]);
            itemIdx = swapIdx;
        }
    }

    template <typename t_compare_t = std::less<ListNode>>
    size_t heapPush(ListNode* heap[], size_t heapSize, ListNode* value) const {
        heap[heapSize] = value;
        heapifyUp<t_compare_t>(heap, heapSize);
        return heapSize + 1;
    }

    template <typename t_compare_t = std::less<ListNode>>
    size_t heapPop(ListNode* heap[], size_t heapSize) const {
        if (0 < heapSize) {
            --heapSize;
            if (0 < heapSize) {
                heap[0] = std::move(heap[heapSize]);
                heapifyDown<t_compare_t>(heap, heapSize, 0);
            }
        }
        return heapSize;
    }
    
    /*!
        Merge all lists into a single large list and then merge sort it.
        T=O(n*log2(n)) [Each recursive call processes 1/2 of the remaining data.]
        S=O(log2(n)) [For the call stack; no temp storage used to sort.]
    */
    ListNode* mergeKLists_append(vector<ListNode*>& lists) const {
        // Combine all lists into one long list.
        ListNode* head = nullptr;
        ListNode* tail = nullptr;
        size_t listSize{};
        for (auto& list : lists) {
            while (list) {
                if (!head) { head = list; }
                if (tail) { tail->next = list; }
                tail = list;

                ++listSize;

                list = list->next;
            }
        }

        return head && tail ? mergeSort(head, tail, listSize) : nullptr;
    }

    ListNode* mergeSort(ListNode* head, ListNode* tail, size_t size) const {
        if (2 >= size) {
            if (2 == size) {
                if (tail->val < head->val) {
                    std::swap(head->val, tail->val);
                }
            }
        } else {
            // Find left and right lists.
            auto const midIdx = size >> 1;
            ListNode* left_head = head;
            auto* left_tail = head;
            for (auto count = midIdx - 1; 0 < count; --count) {
                left_tail = left_tail->next;
            }
            auto* right_head = left_tail->next;
            auto* right_tail = tail;
            left_tail->next = nullptr;
            right_tail->next = nullptr;

            // Merge sort left and right lists.
            auto* left = mergeSort(left_head, left_tail, midIdx);
            auto* right = mergeSort(right_head, right_tail, size - midIdx);

            // Merge sorted left and right lists intoa single list.
            head = nullptr;
            tail = nullptr;
            while (left && right) {
                ListNode* &lesser = left->val < right->val ? left : right;
                if (!head) { head = lesser; }
                if (tail) { tail->next = lesser; }
                tail = lesser;
                lesser = lesser->next;
                tail->next = nullptr;
            }
            if (left) {
                if (!head) { head = left; }
                if (tail) { tail->next = left; }
            }
            if (right) {
                if (!head) { head = right; }
                if (tail) { tail->next = right; }
            }
        }

        return head;
    }

    ListNode* mergeKLists(vector<ListNode*>& lists) const {
        //return mergeKLists_bruteForce(lists);
        //return mergeKLists_heap(lists);
        return mergeKLists_append(lists);
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
    auto [mergedList, expected] = [&]{
        auto const listsValues = vector<vector<int>>{
            vector<int>{1, 4, 5}
            , vector<int>{1, 3, 4}
            , vector<int>{2, 6}
        };
        vector<int> expected{};
        for (auto const& list : listsValues) {
            for (auto const& value : list) {
                expected.push_back(value);
            }
        }
        std::sort(expected.begin(), expected.end());
        auto lists = makeLists(listsValues);
        ListNode* mergedList = Solution{}.mergeKLists(lists);
        return std::make_tuple(mergedList, expected);
    }();
    CHECK(expected == mergedList);
    freeList(mergedList);
    cerr << '\n';
}

TEST_CASE("Case 2")
{
    cerr << "Case 2" << '\n';
    auto [mergedList, expected] = [&]{
        auto const listsValues = vector<vector<int>>{
        };
        vector<int> expected{};
        for (auto const& list : listsValues) {
            for (auto const& value : list) {
                expected.push_back(value);
            }
        }
        std::sort(expected.begin(), expected.end());
        auto lists = makeLists(listsValues);
        ListNode* mergedList = Solution{}.mergeKLists(lists);
        return std::make_tuple(mergedList, expected);
    }();
    CHECK(expected == mergedList);
    freeList(mergedList);
    cerr << '\n';
}

TEST_CASE("Case 3")
{
    cerr << "Case 3" << '\n';
    auto [mergedList, expected] = [&]{
        auto const listsValues = vector<vector<int>>{
            vector<int>{4, 1, 5}
        };
        vector<int> expected{};
        for (auto const& list : listsValues) {
            for (auto const& value : list) {
                expected.push_back(value);
            }
        }
        std::sort(expected.begin(), expected.end());
        auto lists = makeLists(listsValues);
        ListNode* mergedList = Solution{}.mergeKLists(lists);
        return std::make_tuple(mergedList, expected);
    }();
    CHECK(expected == mergedList);
    freeList(mergedList);
    cerr << '\n';
}

TEST_CASE("Case 4")
{
    cerr << "Case 4" << '\n';
    auto [mergedList, expected] = [&]{
        auto const listsValues = vector<vector<int>>{
            vector<int>{2}
            , vector<int>{}
            , vector<int>{-1}
        };
        vector<int> expected{};
        for (auto const& list : listsValues) {
            for (auto const& value : list) {
                expected.push_back(value);
            }
        }
        std::sort(expected.begin(), expected.end());
        auto lists = makeLists(listsValues);
        ListNode* mergedList = Solution{}.mergeKLists(lists);
        return std::make_tuple(mergedList, expected);
    }();
    CHECK(expected == mergedList);
    freeList(mergedList);
    cerr << '\n';
}

TEST_CASE("Case 5")
{
    cerr << "Case 5" << '\n';
    auto [mergedList, expected] = [&]{
        auto const listsValues = vector<vector<int>>{
            vector<int>{-10, -9, -9, -3, -1, -1, 0}
            , vector<int>{-5}
            , vector<int>{4}
            , vector<int>{-8}
            , vector<int>{}
            , vector<int>{-9, -6, -5, -4, -2, 2, 3}
            , vector<int>{-3, -3, -2, -1, 0}
        };
        vector<int> expected{};
        for (auto const& list : listsValues) {
            for (auto const& value : list) {
                expected.push_back(value);
            }
        }
        std::sort(expected.begin(), expected.end());
        auto lists = makeLists(listsValues);
        ListNode* mergedList = Solution{}.mergeKLists(lists);
        return std::make_tuple(mergedList, expected);
    }();
    CHECK(expected == mergedList);
    freeList(mergedList);
    cerr << '\n';
}

/*
    End of "main.cpp"
*/
