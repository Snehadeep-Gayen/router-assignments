#include <vector>
#include <memory>

template <typename T>
class HTrie
{
public:

    HTrie()
    {
        head = std::make_shared<Node>();
    }

    std::shared_ptr<T> insert(std::vector<bool> path)
    {
        std::shared_ptr<Node> current = head;
        for(auto key : path)
        {
            if(!key && current->left==nullptr)
                current->left = std::make_shared<Node>();
            if(key && current->right==nullptr)
                current->right = std::make_shared<Node>();
            if(key)
                current = current->right;
            else
                current = current->left;
        }
        // insert means, that element cannot be nullptr, and will be caught by search always
        if(current->t == nullptr)
            current->t = std::make_shared<T>(T());
        return current->t;
    }

    static std::vector<bool> IntToPath(uint32_t intPath)
    {
        std::vector<bool> path;
        std::bitset<32> bits(intPath);
        for(int i=31; i>=0; i--)
            path.push_back(bits[i]);
        return path;
    }

    std::vector<std::shared_ptr<T>> search(std::vector<bool> path)
    {
        std::vector<std::shared_ptr<T>> ptrs;
        std::shared_ptr<Node> cur = head;
        if(cur->t!=nullptr)
            ptrs.push_back(cur->t);
        for(auto key : path)
        {
            if(key)
                cur = cur->right;
            else
                cur = cur->left;

            if(cur==nullptr)
                break;
            else if(cur->t!=nullptr)
                ptrs.push_back(cur->t);
        }
        return ptrs;
    }

private:

    struct Node
    {
        std::shared_ptr<Node> left, right;
        std::shared_ptr<T> t;
    };

    std::shared_ptr<Node> head;
};