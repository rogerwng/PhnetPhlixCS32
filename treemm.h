#ifndef TREEMULTIMAP_INCLUDED
#define TREEMULTIMAP_INCLUDED

#include <vector>

template <typename KeyType, typename ValueType>
class TreeMultimap
{
  public:
    class Iterator
    {
      public:
        Iterator()
        {
            valid = false;
        }
        
        Iterator(std::vector<ValueType>* values)
        {
            itValues = values;
            it = values->begin();
            valid = true;
        }

        ValueType& get_value() const
        {
            return *it;
        }

        bool is_valid() const
        {
            if (valid == false)
                return false;
            return true;
        }

        void advance()
        {
            it++;
            
            if (it == itValues->end())
                valid = false;
        }

      private:
        bool valid;
        typename std::vector<ValueType>* itValues;
        typename std::vector<ValueType>::iterator it;
    };

    TreeMultimap()
    {
        root = nullptr;
    }

    ~TreeMultimap()
    {
        clearNodes(root);
    }

    void insert(const KeyType& key, const ValueType& value)
    {
        // need to find node w/ Key, then add value to its vector
        // if no node contains Key, create node, do we have to traverse again?
        // when looking for key, dont search whole tree, just up to the point where the Key should be
        
        // if tree is empty
        if (root == nullptr)
        {
            root = new Node(key, value);
            return;
        }
        
        Node* p = root;
        while (p != nullptr)
        {
            // if key exists at this node, add value to its vector
            if (p->m_key == key)
            {
                p->m_values.push_back(value);
                p = nullptr;
                break;
            }
            
            // if key is greater than node, go down right branch
            if (key > p->m_key && p->right == nullptr)
            {
                p->right = new Node(key, value);
                p = nullptr;
                break;
            }
            else if (key > p->m_key)
            {
                p = p->right;
                continue;
            }
            
            // if key is less than node, go down left branch
            if (key < p->m_key && p->left == nullptr)
            {
                p->left = new Node(key, value);
                p = nullptr;
                break;
            }
            else if (key < p->m_key)
            {
                p = p->left;
                continue;
            }
        }
    }

    Iterator find(const KeyType& key) const
    {
        Node* p = root;
        while (p != nullptr)
        {
            // if key exists at this node, add value to its vector
            if (p->m_key == key)
            {
                return Iterator(&p->m_values);
            }
            
            // key not found go down a branch
            if (key > p->m_key)
            {
                p = p->right;
            }
            else if (key < p->m_key)
            {
                p = p->left;
            }
        }
        
        return Iterator();  // if tree empty or item not found return invalid iterator
    }
    
private:
    struct Node
    {
        Node(KeyType key, ValueType value)
        {
            m_key = key;
            m_values.push_back(value);
            left = nullptr;
            right = nullptr;
        };
        
        std::vector<ValueType> m_values;
        KeyType m_key;
        Node* left;
        Node* right;
    };
    
    void clearNodes(Node* p)
    {
        if (p == nullptr)
            return;
        
        clearNodes(p->right);
        clearNodes(p->left);
        delete p;
    }
    
    Node* root;
};

#endif // TREEMULTIMAP_INCLUDED
