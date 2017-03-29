#include <iostream>
using namespace std;

/* // обсъждахме го в почивката...
void strcpy(char* dest, const char* src)
{
    while (*dest++ = *src++); // красота
}*/

class LinkedList
{
private:
    // box е вложен клас и тъй като е private, извън класа НЕ МОЖЕМ да създаваме и работим с кутийки
    // => това е капсулация + абстракция. Само списъкът знае как е организиран, ние получаваме
    // различни непрозрачни методи за работи с него - push, pop, insert, front и т.н.
    struct box
    {
        int data;
        box* next;

        box(int _d, box* _p = nullptr) : data(_d), next(_p) {}
    };

    // достатъчен е само указател към първата кутийка - другото е за улеснение и скорост
    box* first;
    // box* last; // добавете и указател към последната кутийка и променете които методи трябва
    int _size;

    void copyFrom(const LinkedList& other)
    {
        // ВНИМАНИЕ: много бавна операция (т.к. push_back) всеки път търси края на списъка!!!
        // никога не е било казано, че във for можем да ползваме само int-ове...
        for (box* ptr = other.first; ptr != nullptr; ptr = ptr->next)
            push_back(ptr->data);
    }
    void clearEverything()
    {
        while (!empty())
            pop();
        // след това first == nullptr, и няма нужда да се грижим за това
    }
public:
    // стандартни
    LinkedList() : first(nullptr), _size(0) {}
    // Делегиране на конструктори: първо ще се изпълни дефолтния, т.е. списъкът
    // ще се инициализира по валиден начин, след което ще се изпълни тялото.
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

    // Стандартни за списъците, вмъкване и премахване от началото.
    void push(int x)
    {
        box* newbox = new box(x, first);
        first = newbox;
        ++_size;
    }
    // Не работи за празен списък (undefined behaviour) - или трябва
    // изрично да го споменем, или да се справим по друг начин (exception, return...)
    void pop()
    {
        box* tmp = first;
        first = first->next;
        delete tmp;
        --_size;
    }
    
    // Ако списъкът е празен, pos може да е произволна и ще работи
    // Ако списъкът е непразен, трябва pos <= size() - иначе undefined behaviour :(
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
    // Трябва pos да е валиден индекс, т.е. pos < size()
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
    // Важно - т.к. имаме само указател къ,м първата кутийка, това е супер бавно!
    // Ще става много по-бързо и лесно, ако пазихме и указател към последната кутийка.
    void push_back(int x)
    {
        insertAt(x, size());
    }

    // по-добре да се казва get или at, а не operator[], понеже е бавна операция!
    // Позициите се броят от нула.
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

    // Стандартни
    bool empty() const
    {
        return (first == nullptr);
        // return !first;
    }
    int size() const { return _size; }
    // Достъп до първия елемент (главата) на списъка
    int front() const { return first->data; }
    // Обхождане на целия списък
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

    // Функцията е външна и е friend, за да достъпва член-данните на списъците (first)
    // ИЗИСКВА СЕ СПИСЪЦИТЕ ДА БЪДАТ СОРТИРАНИ ПРЕДВАРИТЕЛНО!
    // Заради постоянното използване на push_back() е също много бавно!
    friend LinkedList merge(const LinkedList& lst1, const LinkedList& lst2)
    {
        LinkedList result;
        box* p1 = lst1.first;
        box* p2 = lst2.first;
        // първо копираме елементите от двата списъка едновременно, докато не изчерпаме единия от тях.
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
        // ако p1 е валиден <=> изчерпали сме l2 и трябва да добавим остатъка от l1
        while (p1)
        {
            result.push_back(p1->data);
            p1 = p1->next;
        }
        // аналогично, ако p2 е валиден
        while (p2)
        {
            result.push_back(p2->data);
            p2 = p2->next;
        }
        // накрая връщаме насъбраното
        return result;
    }
};

int main()
{
    LinkedList lst;
    lst.push_back(3);     // {3}
    lst.push(2);          // {2,3}
    lst.push_back(10);    // {2,3,10}
    lst.insertAt(20, 0);  // {20,2,3,10}, аналогично на push(20)
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