/*
template <typename T>
struct list_node 
{
    struct list_node* prev;
    T data;
    struct list_node* next;
};

template <typename T> class List
{
private:
    struct list_node<T> *head;
public:
    List()
    {
        head = NULL;
    }

    void Push(T val)
    {
        struct list_node<T>* node = new struct list_node<T>();   
        n->data = val;             
        n->next = head;        
        head = n;              
    }

    T Pop()
    {
        if (head) 
        {
            T p = head->data;
            head = head->next;
            return p;
        }
    }
    
    bool Search(T val) 
    {
        struct list_node<T>* temp = head;
        while (temp->next) 
        {
            if (temp->data == val)
            {
                return true;
            }
            else
            {
                temp = temp->next;
            }
        }
        delete temp;
        return false;
    }
};
*/