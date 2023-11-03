#include <iostream>
#include <string>

namespace storage
{
    template<typename T>
    struct node
    {
        T key;
        struct node* prev, * next;
        node() : prev(nullptr), next(nullptr) {}
    };

    template<typename T>
    class list
    {
    public:
        list() : head(nullptr), tail(nullptr), cnt(0) {}
        void push_front(T el);
        void push_back(T el);
        T pop_front(void);
        T pop_back(void);
        bool empty(void);
        size_t size(void);

        void delete_after_last_min(void);
        void map_min_in_place(void (*body)(T& el));

        T& operator[](size_t idx);
        const T& operator[](size_t idx) const;


    private:
        struct node<T>* head, * tail;
        size_t cnt;
    };

    template<typename T>
    void list<T>::push_front(T el)
    {
        struct node<T>* n = new struct node<T>;
        n->key = el;
        if (!cnt)
        {
            this->cnt = 1;
            this->head = this->tail = n;
        }
        else {
            n->next = this->head;
            this->head->prev = n;
            this->head = n;
            this->cnt++;
        }
    }

    template<typename T>
    void list<T>::push_back(T el)
    {
        struct node<T>* n = new struct node<T>;
        n->key = el;
        if (!cnt)
        {
            this->cnt = 1;
            this->head = this->tail = n;
        }
        else {
            n->prev = this->tail;
            this->tail->next = n;
            this->tail = n;
            this->cnt++;
        }
    }

    template<typename T>
    T list<T>::pop_front(void)
    {
        T ret;
        if (!this->cnt)
        {
            throw std::out_of_range("list is empty");
        }
        ret = this->head->key;
        struct node<T>* old;
        old = this->head;
        this->head = this->head->next;
        this->cnt--;
        delete old;

        return ret;
    }

    template<typename T>
    T list<T>::pop_back(void)
    {
        T ret;
        if (!this->cnt)
        {
            throw std::out_of_range("list is empty");
        }
        ret = this->tail->key;
        struct node<T>* old;
        old = this->tail;
        this->tail = this->tail->prev;
        this->cnt--;
        delete old;

        return ret;
    }

    template<typename T>
    void list<T>::delete_after_last_min(void)
    {
        if (this->cnt <= 1)
            return;

        T m = this->head->key;
        for (struct node<T>* s = this->head; s != nullptr; s = s->next)
        {
            m = min(m, s->key);
        }

        struct node<T>* last_min = nullptr;
        for (struct node<T>* s = this->head; s != nullptr; s = s->next)
        {
            if (s->key == m)
            {
                last_min = s;
            }
        }
        struct node<T>* to_delete = last_min->next;
        if (to_delete == NULL)
        {
            return;
        }

        if (to_delete == this->tail)
        {
            this->pop_back();
            return;
        }

        to_delete->prev->next = to_delete->next;
        to_delete->next->prev = to_delete->prev;
        delete to_delete;
        this->cnt--;
    }

    template<typename T>
    void list<T>::map_min_in_place(void (*body)(T& el))
    {
        if (!this->cnt)
            return;

        T m = this->head->key;
        for (struct node<T>* s = this->head; s != nullptr; s = s->next)
        {
            m = min(m, s->key);
        }

        for (struct node<T>* s = this->head; s != nullptr; s = s->next)
        {
            if (s->key == m)
            {
                body(s->key);
            }
        }
    }

    template<typename T>
    bool list<T>::empty(void)
    {
        return this->cnt == 0;
    }

    template<typename T>
    size_t list<T>::size(void)
    {
        return this->cnt;
    }

    template<typename T>
    T& list<T>::operator[](size_t idx)
    {
        if (idx >= this->cnt)
        {
            throw std::out_of_range("index is out of range");
        }

        struct node<T>* el = this->head;
        for (int i = 0; i < idx && el != nullptr; el = el->next, i++);

        return el->key;
    }

    template<typename T>
    const T& list<T>::operator[](size_t idx) const
    {
        if (idx >= this->cnt)
        {
            throw std::out_of_range("index is out of range");
        }

        struct node<T>* el = this->head;
        for (int i = 0; i < idx && el != nullptr; el = el->next, i++);

        return el->key;
    }
}

int main()
{
    storage::list<std::string> a;
    for (int i = 0; i < 26; i++)
    {
        std::string q(1, (char)('a' + i));
        a.push_back(q);
    }

    for (int i = 0; i < 26; i++)
    {
        a[i] += "z";
        std::cout << a[i] << ' ';
    }
    std::cout << std::endl;

    a.map_min_in_place([](std::string& s) { s += "z"; });
    for (int i = 0; i < 26; i++)
    {
        std::cout << a[i] << ' ';
    }
    std::cout << std::endl;

    a.delete_after_last_min();
    a.delete_after_last_min();

    for (int i = 0; i < a.size(); i++)
    {
        std::cout << a[i] << ' ';
    }
    std::cout << std::endl;

    while (a.size() > 1)
    {
        a.delete_after_last_min();
    }

    a.delete_after_last_min();

    for (int i = 0; i < a.size(); i++)
    {
        std::cout << a[i] << ' ';
    }
    std::cout << std::endl;

    return 0;
}