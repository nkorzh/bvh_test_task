#pragma once
#include <cstdint>
#include <iostream>
#include <stack>
#include <vector>

using namespace std;

struct float3
{
    float3() {
        vec[0] = 0;
        vec[1] = 0;
        vec[2] = 0;
    }

    float vec[3];

    //Френд функция для удобного вывода
    friend std::ostream& operator<< (std::ostream& out, const float3& point) {
       out << point.vec[0] << ", " << point.vec[1] << ", " << point.vec[2] << ", ";
       return out;
    }
};

//Структура для хранения нодов в массиве
struct BvhNode
{
    uint32_t child0;
    uint32_t child1;
    uint32_t parent;
    uint32_t update;

    float3 aabb0_min_or_v0;
    float3 aabb0_max_or_v1;
    float3 aabb1_min_or_v2;
    float3 aabb1_max_or_v3;
    friend std::ostream& operator << (std::ostream& out, const BvhNode& node) {
        out << node.child0 << std::endl << node.child1 << std::endl << node.parent
            << std::endl << node.update << std::endl << node.aabb0_min_or_v0 << std::endl << node.aabb0_max_or_v1 << std::endl
            << node.aabb1_min_or_v2 << std::endl << node.aabb1_max_or_v3 << std::endl << std::endl;
        return out;
    }
};

//Структура для хранения нодов в 
struct BvhNodeTree
{   
    BvhNodeTree();

    BvhNodeTree(uint32_t update, float3 aabb0_min_or_v0, float3 aabb0_max_or_v1,
        float3 aabb1_min_or_v2, float3 aabb1_max_or_v3, BvhNodeTree* child0, BvhNodeTree*child1, BvhNodeTree* parent, uint32_t index) {

        this->aabb0_min_or_v0 = aabb0_min_or_v0;  
        this->aabb0_max_or_v1 = aabb0_max_or_v1;  
        this->aabb1_min_or_v2 = aabb1_min_or_v2;  
        this->aabb1_max_or_v3 = aabb1_max_or_v3;
        this->child0 = child0; 
        this->child1 = child1; 
        this->parent = parent; 
        this->update = update; 
        this->index = index; 

    }
    BvhNodeTree* child0;  //Если объект треугольник, то этот ребенок будет иметь адрес 4294967295, не пугайтесь
    BvhNodeTree* child1;  //Если объект треугольник, то есть лист, то он все равно будет иметь мнимого ребенка(child1), это будет родительский нод
                          //без понятия зачем они это сделали, но имеем что имеем, очевидно на него тоже не обращаем внимания
    BvhNodeTree* parent;
    uint32_t update;      //Ненужная хуйня, ее просто скипаем, но пусть будет
    uint32_t index;       //Индекс из массива с нодами


    float3 aabb0_min_or_v0;  //Координаты вершин 
    float3 aabb0_max_or_v1;  //если объект треугольник (то есть часть модели), то они имеет три координаты, то есть иметь 
    float3 aabb1_min_or_v2;  //координаты 0.0, 0.0, 0.0
    float3 aabb1_max_or_v3;

    //Френд функция для удобного вывода
    friend std::ostream& operator << (std::ostream& out, BvhNodeTree node) {
        out << "++++" << node.index << "++++" << std::endl << node.aabb0_min_or_v0 << std::endl << node.aabb0_max_or_v1 << std::endl
            << node.aabb1_min_or_v2 << std::endl << node.aabb1_max_or_v3 << std::endl << std::endl;
        return out;
    }
};

//Класс для построения дерева
class Tree {
public: 
    Tree(BvhNode* BvhArray)
    {
        //Принимает распаршенный массив нодов из дамб файла
        this->BvhArray = BvhArray; 
    };
    ~Tree()
    {
        destroy_tree();
    };

    //Рекурсивный алгоритм построения дерева из массива
    BvhNodeTree* createTree(BvhNode item, BvhNodeTree* last, bool isLeft) {
        if (item.child0 == 4294967295) {
            /*return new BvhNodeTree(item.update, item.aabb0_min_or_v0, item.aabb0_max_or_v1, item.aabb1_min_or_v2, item.aabb1_max_or_v3, 
                nullptr, nullptr, getNodeByIndex(item.parent), BvhArray[item.child1].parent);*/
            uint32_t ind = isLeft ? BvhArray[last->index].child0 : BvhArray[last->index].child1;
            BvhNodeTree* buff = new BvhNodeTree(item.update, item.aabb0_min_or_v0, item.aabb0_max_or_v1, item.aabb1_min_or_v2, item.aabb1_max_or_v3,
                nullptr, nullptr, last, ind);
            leafArr.push_back(buff);
            return buff;
        }
        BvhNode left = BvhArray[item.child0];
        BvhNode right = BvhArray[item.child1];
        BvhNodeTree* rootNode = new BvhNodeTree(item.update, item.aabb0_min_or_v0, item.aabb0_max_or_v1, item.aabb1_min_or_v2, item.aabb1_max_or_v3, 
            nullptr, nullptr, last, BvhArray[item.child1].parent);
        rootNode->child0 = createTree(left, rootNode, true);
        rootNode->child1 = createTree(right, rootNode, false);
        return rootNode;
    }
    //Отрисовка для отладки
    void drawTree(const BvhNodeTree* root);
    void destroy_tree();

    //Поиск нода по индексу, нужен для поиска родителя и в целом может еще потом пригодиться 
    BvhNodeTree* getNodeByIndex(uint32_t item) {


        BvhNodeTree* node = root;

        std::stack<BvhNodeTree*> s;
        s.push(node);
        while (!s.empty())
        {
            node = s.top();
            s.pop();
            if (node->index == item)
            {
                break;
                return node;
            }
            if (node->child0 != nullptr)
                s.push(node->child0);
            if (node->child1 != nullptr)
                s.push(node->child1);
        }

        
    }

    std::vector<BvhNodeTree*> leafArr;

private:
    BvhNodeTree* root = new BvhNodeTree(0u, float3(), float3(), float3(), float3(), nullptr, nullptr, nullptr, 0);
    void destroy_tree(BvhNodeTree* leaf);
    BvhNode* BvhArray;
    const long long leaf = 4294967295;
};

