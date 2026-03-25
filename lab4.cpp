#include <iostream>
#include <iomanip>

class ListNode {
public:
    ListNode(const double data, ListNode* next = nullptr)
        : _data(data), _nextPtr(next) {}
    double _data;
    ListNode* _nextPtr;
};

class List {
public:

    List() : _firstPtr(nullptr), _lastPtr(nullptr) {}

    ~List() {
        ListNode* current = _firstPtr;
        while (current != nullptr) {
            ListNode* next = current->_nextPtr;
            delete current;
            current = next;
        }
    }


    void insertAtFront(const double value) {
        ListNode* newNode = createNode(value);
        newNode->_nextPtr = _firstPtr;
        _firstPtr = newNode;
        if (_lastPtr == nullptr)
            _lastPtr = newNode;
    }

    void insertAtBack(const double value) {
        ListNode* newNode = createNode(value);
        if (isEmpty()) {
            _firstPtr = _lastPtr = newNode;
        } else {
            _lastPtr->_nextPtr = newNode;
            _lastPtr = newNode;
        }
    }

    bool removeFromFront(double& value) {
        if (isEmpty())
            return false;

        ListNode* nodeToDelete = _firstPtr;
        value = nodeToDelete->_data;
        _firstPtr = _firstPtr->_nextPtr;

        if (_firstPtr == nullptr)
            _lastPtr = nullptr;

        delete nodeToDelete;
        return true;
    }

    bool removeFromBack(double& value) {
        if (isEmpty())
            return false;

        if (_firstPtr == _lastPtr) {
            value = _firstPtr->_data;
            delete _firstPtr;
            _firstPtr = _lastPtr = nullptr;
            return true;
        }

        ListNode* current = _firstPtr;
        while (current->_nextPtr != _lastPtr) {
            current = current->_nextPtr;
        }

        value = _lastPtr->_data;
        delete _lastPtr;
        _lastPtr = current;
        _lastPtr->_nextPtr = nullptr;

        return true;
    }

    bool isEmpty() const {
        return _firstPtr == nullptr;
    }

    void print() const {
        if (isEmpty()) {
            std::cout << "The list is empty.\n";
            return;
        }

        ListNode* current = _firstPtr;
        std::cout << "List contents: ";
        while (current != nullptr) {
            std::cout << current->_data << " ";
            current = current->_nextPtr;
        }
        std::cout << std::endl;
    }

private:
    ListNode* createNode(const double value) const {
        return new ListNode(value);
    }

    ListNode* _firstPtr;
    ListNode* _lastPtr;
};

void instructions() {
    std::cout << "\nEnter the command:\n"
              << " 1 to insert at beginning of list\n"
              << " 2 to insert at end of list\n"
              << " 3 to delete from beginning of list\n"
              << " 4 to delete from end of list\n"
              << " 5 to end list processing\n"
              << "Your choice: ";
}

int main() {
    List list;
    int choice;
    double value;

    instructions();
    while (std::cin >> choice) {
        switch (choice) {
            case 1:
                std::cout << "Enter a double value: ";
                std::cin >> value;
                list.insertAtFront(value);
                list.print();
                break;
            case 2:
                std::cout << "Enter a double value: ";
                std::cin >> value;
                list.insertAtBack(value);
                list.print();
                break;
            case 3:
                if (list.removeFromFront(value))
                    std::cout << "Removed " << value << " from front.\n";
                else
                    std::cout << "List is empty, cannot remove.\n";
                list.print();
                break;
            case 4:
                if (list.removeFromBack(value))
                    std::cout << "Removed " << value << " from back.\n";
                else
                    std::cout << "List is empty, cannot remove.\n";
                list.print();
                break;
            case 5:
                std::cout << "End of list processing.\n";
                return 0;
            default:
                std::cout << "Try again.\n";
                break;
        }
        instructions();
    }
    return 0;
}