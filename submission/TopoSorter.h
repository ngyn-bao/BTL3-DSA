/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   TopoSorter.h
 * Author: ltsach
 *
 * Created on July 11, 2021, 10:21 PM
 */

#ifndef TOPOSORTER_H
#define TOPOSORTER_H
#include "graph/DGraphModel.h"
#include "list/DLinkedList.h"
#include "sorting/DLinkedListSE.h"

template <class T>
class TopoSorter
{
public:
    static int DFS;
    static int BFS;
    static int hashInt(int &key, int capacity);
    static int hashChar(char &key, int capacity);
    static int hashString(std::string &key, int capacity);

protected:
    DGraphModel<T> *graph;
    int (*hash_code)(T &, int);

public:
    TopoSorter(DGraphModel<T> *graph, int (*hash_code)(T &, int) = 0)
    {
        // TODO
        this->graph = graph;
        if (!hash_code)
        {
            if constexpr (std::is_same<T, int>::value)
            {
                this->hash_code = hashInt;
            }
            else if constexpr (std::is_same<T, std::string>::value)
            {
                this->hash_code = hashString;
            }
            else if constexpr (std::is_same<T, char>::value)
            {
                this->hash_code = hashChar;
            }
            else
            {
                this->hash_code = nullptr;
            }
        }
        else
        {
            this->hash_code = hash_code;
        }
    }
    DLinkedList<T> sort(int mode = 0, bool sorted = true)
    {
        // TODO
        if (mode == DFS)
            return dfsSort(sorted);
        else if (mode == BFS)
            return bfsSort(sorted);
        else
            throw std::invalid_argument("Invalid sorting mode!");
    }

    DLinkedList<T> bfsSort(bool sorted = true)
    {
        // TODO
        xMap<T, int> inDegree = vertex2inDegree(this->hash_code);

        DLinkedList<T> zeroInDegrees = this->listOfZeroInDegrees();

        if (sorted)
        {
            DLinkedListSE<T> sortedZeroOutDegrees(zeroInDegrees);

            sortedZeroOutDegrees.sort();

            zeroInDegrees = sortedZeroOutDegrees.toDLinkedList();
        }

        DLinkedListSE<T> result;
        Queue<T> queue;

        for (auto it = zeroInDegrees.begin(); it != zeroInDegrees.end(); ++it)
        {
            queue.push(*it);
        }

        while (!queue.empty())
        {
            T vertex = queue.peek();
            queue.pop();
            result.add(vertex);

            DLinkedList<T> neighbors = this->graph->getOutwardEdges(vertex);

            if (neighbors.empty())
            {
                continue;
            }

            for (auto it = neighbors.begin(); it != neighbors.end(); ++it)
            {
                T neighbor = *it;

                int currentInDegree = inDegree.get(neighbor);
                inDegree.put(neighbor, currentInDegree - 1);

                if (inDegree.get(neighbor) == 0)
                {
                    queue.push(neighbor);
                }
            }
        }

        if (result.size() != this->graph->vertices().size())
        {
            throw std::runtime_error("Graph contains a cycle or vertices mismatch!");
        }

        return result.toDLinkedList();
    }

    DLinkedList<T> dfsSort(bool sorted = true)
    {
        // TODO
        xMap<T, bool> visited(this->hash_code);
        DLinkedListSE<T> vertices = graph->vertices();
        if (sorted)
        {
            vertices.sort(&SortSimpleOrder<T>::compare4Desending);
        }
        for (auto it = vertices.begin(); it != vertices.end(); it++)
        {
            visited.put(*it, false);
        }
        DLinkedList<T> result;
        Stack<T> stack;
        for (auto it = vertices.begin(); it != vertices.end(); ++it)
        {
            T startVertex = *it;
            if (!visited.get(startVertex))
            {
                stack.push(startVertex);
                while (!stack.empty())
                {
                    T currentVertex = stack.peek();
                    bool boolcheck = false;
                    DLinkedList<T> temp = graph->getOutwardEdges(currentVertex);
                    DLinkedListSE<T> neighbors = temp;
                    if (sorted)
                    {
                        neighbors.sort(&SortSimpleOrder<T>::compare4Desending);
                    }
                    for (auto neighbor = neighbors.begin(); neighbor != neighbors.end(); ++neighbor)
                    {
                        if (!visited.get(*neighbor))
                        {
                            stack.push(*neighbor);
                            boolcheck = true;
                            break;
                        }
                    }
                    if (!boolcheck)
                    {
                        stack.pop();
                        if (!visited.get(currentVertex))
                        {
                            result.add(0, currentVertex);
                            visited.put(currentVertex, true);
                        }
                    }
                }
            }
        }

        if (result.size() != this->graph->vertices().size())
            throw std::runtime_error("Graph contains a cycle!");
        return result;
    }

protected:
    xMap<T, int> vertex2inDegree(int (*hash)(T &, int))
    {
        xMap<T, int> inDegree(hash);

        DLinkedList<T> vertices = this->graph->vertices();

        for (auto it = vertices.begin(); it != vertices.end(); ++it)
        {

            T vertex = *it;
            inDegree.put(vertex, 0);
        }

        for (auto it = vertices.begin(); it != vertices.end(); ++it)
        {

            T vertex = *it;

            DLinkedList<T> neighbors = this->graph->getOutwardEdges(vertex);

            for (auto neighborIt = neighbors.begin(); neighborIt != neighbors.end(); ++neighborIt)
            {
                T neighbor = *neighborIt;

                if (!inDegree.containsKey(neighbor))
                {

                    inDegree.put(neighbor, 0);
                }

                int currentInDegree = inDegree.get(neighbor);
                inDegree.put(neighbor, currentInDegree + 1);
            }
        }

        return inDegree;
    }

    xMap<T, int> vertex2outDegree(int (*hash)(T &, int))
    {
        xMap<T, int> outDegree(hash);
        DLinkedList<T> vertices = this->graph->vertices();

        for (auto vertex : vertices)
        {
            outDegree.put(vertex, 0);
        }

        for (auto vertex : vertices)
        {
            DLinkedList<T> neighbors = this->graph->getOutwardEdges(vertex);
            outDegree.put(vertex, neighbors.size());
        }
        // cout << outDegree.toString() << endl;

        return outDegree;
    }

    DLinkedList<T> listOfZeroInDegrees()
    {
        DLinkedList<T> result;
        auto vertices = this->graph->vertices();

        for (auto it = vertices.begin(); it != vertices.end(); ++it)
        {
            T vertex = *it;
            int inDegree = this->graph->inDegree(vertex);

            if (inDegree == 0)
            {
                result.add(vertex);
            }
        }

        return result;
    }

}; // TopoSorter
template <class T>
int TopoSorter<T>::DFS = 0;
template <class T>
int TopoSorter<T>::BFS = 1;
template <class T>
int TopoSorter<T>::hashInt(int &key, int capacity)
{
    return key % capacity;
}
template <class T>
int TopoSorter<T>::hashString(std::string &key, int capacity)
{
    int hashValue = 0;
    for (char ch : key)
    {
        hashValue = (hashValue * 31 + ch) % capacity;
    }
    return hashValue;
}
template <class T>
int TopoSorter<T>::hashChar(char &key, int capacity)
{
    return static_cast<int>(key) % capacity;
}

/////////////////////////////End of TopoSorter//////////////////////////////////

#endif /* TOPOSORTER_H */
