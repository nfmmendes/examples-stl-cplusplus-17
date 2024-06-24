#include <iostream>
#include <string>
using std::cout;
using std::endl;
using std::ostream;
using std::string;

template <class T = string>
class Node
{

public:
    explicit Node(const T &nodeValue) : value(nodeValue){};

    const T &getValue()
    {
        return value;
    }

    friend ostream &operator<<(ostream &os, const Node<T> &node)
    {
        os << " value: " << node.value;

        return os;
    }

private:
    const T value{};
};

template <class T = string>
class Tree
{

public:
    explicit Tree(Node<T> *rootNode) : root(rootNode != nullptr ? rootNode : new Node<T>(T{}))
    {
    }

    explicit Tree(T &rootValue) : Tree(new Node<T>(rootValue))
    {
    }

    explicit Tree(T &&rootValue) : Tree(new Node<T>(std::forward<T>(rootValue)))
    {
    }

    // To avoid double delete when handling with ranges.
    Tree(const Tree<T> &other)
    {
        root = new Node<T>(other.root->getValue());

        if (other.getLeftSubtree() != nullptr)
            leftSubtree = new Tree<T>(*other.getLeftSubtree());
        if (other.getRightSubtree() != nullptr)
            rightSubtree = new Tree<T>(*other.getRightSubtree());
    }

    ~Tree()
    {
        delete leftSubtree;
        delete rightSubtree;
        delete root;
    }

    const Node<T> *getRoot() const
    {
        return root;
    }

    void addNode(Node<T> *newNode)
    {
        if (newNode == nullptr)
            return;

        if (newNode->getValue() <= root->getValue())
        {
            if (leftSubtree == nullptr)
            {
                leftSubtree = new Tree(newNode);
                leftSubtree->setParent(this);
            }
            else
            {
                leftSubtree->addNode(newNode);
            }
        }
        else
        {
            if (rightSubtree == nullptr)
            {
                rightSubtree = new Tree(newNode);
                rightSubtree->setParent(this);
            }
            else
            {
                rightSubtree->addNode(newNode);
            }
        }
    }

    void addValue(T &&value)
    {
        auto *newNode = new Node(std::forward<T>(value));
        addNode(newNode);
    }

    const Tree<T> *getLeftSubtree() const
    {
        return leftSubtree;
    }

    void setLeftSubtree(const Tree<T> *left)
    {
        delete leftSubtree;
        leftSubtree = left;
        leftSubtree->parentTree = this;
    }

    const Tree<T> *getRightSubtree() const
    {
        return rightSubtree;
    }

    void setRightSubtree(const Tree<T> *right)
    {
        delete rightSubtree;
        rightSubtree = right;
        rightSubtree->parentTree = this;
    }

    void print() const
    {
        if (leftSubtree != nullptr)
        {
            cout << "GOING LEFT\n";
            leftSubtree->print();
        }

        cout << "PRINTING ROOT --> " << *root << endl;

        if (rightSubtree != nullptr)
        {
            cout << "GOING RIGHT\n";
            rightSubtree->print();
        }
    }

    const Tree<T> *getParent() const
    {
        return parentTree;
    }

private:
    Node<T> *root{};
    Tree *leftSubtree{};
    const Tree *parentTree{};
    Tree *rightSubtree{};

    void setParent(const Tree<T> *parent)
    {
        parentTree = parent;
    }
};

template <class T>
class TreeIterator
{
public:
    explicit TreeIterator(const Tree<T> *initTree) : tree(initTree) {}
    TreeIterator(TreeIterator &other) : tree(other.tree) {}

    const Tree<T> &operator*() const
    {
        return *tree;
    }

    TreeIterator<T> &operator++()
    {
        if (tree->getRightSubtree() == nullptr)
        {
            if (tree->getParent() == nullptr || tree->getParent()->getRightSubtree() == tree)
                tree = nullptr;
            else
                tree = tree->getParent();
        }
        else
        {
            tree = getLeftMostOnRight(tree);
        }

        return *this;
    }

    bool operator!=(const TreeIterator<T> &other) const
    {
        return this->tree != other.tree;
    }

    bool operator!=(const Tree<T> *other) const
    {
        return tree != other;
    }

private:
    const Tree<T> *getLeftMostOnRight(const Tree<T> *currentTree)
    {
        return leftMostSubTree(currentTree->getRightSubtree(), currentTree);
    }

    const Tree<T> *leftMostSubTree(const Tree<T> *currentTree, const Tree<T> *parentTree) const
    {
        if (currentTree->getLeftSubtree() != nullptr)
            return leftMostSubTree(currentTree->getLeftSubtree(), currentTree);

        return currentTree;
    }

    const Tree<T> *tree{};
};

template <class T>
class TreeRange
{
public:
    TreeRange(const Tree<T> *input) : tree(input){};

    TreeIterator<T> begin() const
    {
        auto mostToLeft = tree->getLeftSubtree();
        if (mostToLeft == nullptr)
            return TreeIterator<T>{tree};
        while (mostToLeft->getLeftSubtree())
            mostToLeft = mostToLeft->getLeftSubtree();

        return TreeIterator<T>{mostToLeft};
    }

    TreeIterator<T> end() const
    {
        return TreeIterator<T>{nullptr};
    }

private:
    const Tree<T> *tree;
};

int main()
{
    Tree<int> *tree = new Tree<int>(5);
    tree->addValue(10);
    tree->addValue(4);
    tree->addValue(8);
    tree->addValue(15);
    tree->addValue(2);
    tree->addValue(12);

    tree->print();

    // (5) --R--> (10) --L--> (8)
    //                 --R--> (15) --L--> (12)
    //
    //     --L--> (4)  --L--> (2)

    cout << "Print with iterators " << endl;
    for (TreeIterator<int> it{tree}; it != nullptr; ++it)
    {
        cout << *((*it).getRoot()) << endl;
    }

    cout << "Print with ranges " << endl;
    for (auto it : TreeRange<int>(tree))
    {
        cout << *it.getRoot() << endl;
    }

    cout << "Print smaller range " << endl;
    for (auto &it : TreeRange<int>(tree->getRightSubtree()))
    {
        cout << *it.getRoot() << endl;
    }

    delete tree;
    return 0;
}