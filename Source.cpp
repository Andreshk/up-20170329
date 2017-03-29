#include <iostream>
using namespace std;

/* // ���������� �� � ���������...
void strcpy(char* dest, const char* src)
{
    while (*dest++ = *src++); // �������
}*/

class LinkedList
{
private:
    // box � ������ ���� � ��� ���� � private, ����� ����� �� ����� �� ��������� � ������� � �������
    // => ���� � ���������� + ����������. ���� �������� ���� ��� � �����������, ��� ����������
    // �������� ����������� ������ �� ������ � ���� - push, pop, insert, front � �.�.
    struct box
    {
        int data;
        box* next;

        box(int _d, box* _p = nullptr) : data(_d), next(_p) {}
    };

    // ���������� � ���� �������� ��� ������� ������� - ������� � �� ��������� � �������
    box* first;
    // box* last; // �������� � �������� ��� ���������� ������� � ��������� ����� ������ ������
    int _size;

    void copyFrom(const LinkedList& other)
    {
        // ��������: ����� ����� �������� (�.�. push_back) ����� ��� ����� ���� �� �������!!!
        // ������ �� � ���� ������, �� ��� for ����� �� �������� ���� int-���...
        for (box* ptr = other.first; ptr != nullptr; ptr = ptr->next)
            push_back(ptr->data);
    }
    void clearEverything()
    {
        while (!empty())
            pop();
        // ���� ���� first == nullptr, � ���� ����� �� �� ������ �� ����
    }
public:
    // ����������
    LinkedList() : first(nullptr), _size(0) {}
    // ���������� �� ������������: ����� �� �� ������� ���������, �.�. ��������
    // �� �� ������������ �� ������� �����, ���� ����� �� �� ������� ������.
    LinkedList(const LinkedList& other) : LinkedList() { copyFrom(other); }
    LinkedList& operator=(const LinkedList& other)
    {
        if (this != &other)
        {
            clearEverything();
            copyFrom(other);
        }
        return *this;
    }
    ~LinkedList()
    {
        clearEverything();
    }

    // ���������� �� ���������, �������� � ���������� �� ��������.
    void push(int x)
    {
        box* newbox = new box(x, first);
        first = newbox;
        ++_size;
    }
    // �� ������ �� ������ ������ (undefined behaviour) - ��� ������
    // ������� �� �� ��������, ��� �� �� ������� �� ���� ����� (exception, return...)
    void pop()
    {
        box* tmp = first;
        first = first->next;
        delete tmp;
        --_size;
    }
    
    // ��� �������� � ������, pos ���� �� � ���������� � �� ������
    // ��� �������� � ��������, ������ pos <= size() - ����� undefined behaviour :(
    void insertAt(int x, int pos)
    {

        if (empty() || pos == 0)
        {
            push(x);
            return;
        }
        box* tmp = first;
        while (pos > 1)
        {
            --pos;
            tmp = tmp->next;
        }
        tmp->next = new box(x, tmp->next);
        ++_size;
    }
    // ������ pos �� � ������� ������, �.�. pos < size()
    void removeAt(int pos)
    {
        if (pos == 0)
        {
            pop();
            return;
        }
        box* tmp = first;
        while (pos > 1)
        {
            --pos;
            tmp = tmp->next;
        }
        box* toRemove = tmp->next;
        tmp->next = toRemove->next; // tmp->next->next
        delete toRemove;
        --_size;
    }
    // ����� - �.�. ����� ���� �������� ��,� ������� �������, ���� � ����� �����!
    // �� ����� ����� ��-����� � �����, ��� ������� � �������� ��� ���������� �������.
    void push_back(int x)
    {
        insertAt(x, size());
    }

    // ��-����� �� �� ����� get ��� at, � �� operator[], ������ � ����� ��������!
    // ��������� �� ����� �� ����.
    int& operator[](int idx)
    {
        box* tmp = first;
        while (idx)
        {
            --idx;
            tmp = tmp->next;
        }
        return tmp->data;
    }

    // ����������
    bool empty() const
    {
        return (first == nullptr);
        // return !first;
    }
    int size() const { return _size; }
    // ������ �� ������ ������� (�������) �� �������
    int front() const { return first->data; }
    // ��������� �� ����� ������
    void print() const
    {
        box* curr = first;
        while (curr)
        {
            cout << curr->data << " ";
            curr = curr->next;
        }
        cout << "\n";
    }

    // ��������� � ������ � � friend, �� �� �������� ����-������� �� ��������� (first)
    // ������� �� ��������� �� ����� ��������� �������������!
    // ������ ����������� ���������� �� push_back() � ���� ����� �����!
    friend LinkedList merge(const LinkedList& lst1, const LinkedList& lst2)
    {
        LinkedList result;
        box* p1 = lst1.first;
        box* p2 = lst2.first;
        // ����� �������� ���������� �� ����� ������� ������������, ������ �� ��������� ������ �� ���.
        while (p1&&p2)
        {
            if (p1->data < p2->data)
            {
                result.push_back(p1->data);
                p1 = p1->next;
            }
            else
            {
                result.push_back(p2->data);
                p2 = p2->next;
            }
        }
        // ��� p1 � ������� <=> ��������� ��� l2 � ������ �� ������� �������� �� l1
        while (p1)
        {
            result.push_back(p1->data);
            p1 = p1->next;
        }
        // ����������, ��� p2 � �������
        while (p2)
        {
            result.push_back(p2->data);
            p2 = p2->next;
        }
        // ������ ������� �����������
        return result;
    }
};

int main()
{
    LinkedList lst;
    lst.push_back(3);     // {3}
    lst.push(2);          // {2,3}
    lst.push_back(10);    // {2,3,10}
    lst.insertAt(20, 0);  // {20,2,3,10}, ���������� �� push(20)
    lst.print();
    lst.pop();

    LinkedList lst2;
    lst2.push(12);
    lst2.push(10);
    lst2.push(5);
    lst2.push(1);

    merge(lst, lst2).print();
    cout << "Hello, world!\n";
}