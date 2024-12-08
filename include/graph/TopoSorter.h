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

protected:
    DGraphModel<T> *graph;
    int (*hash_code)(T &, int);

public:
    TopoSorter(DGraphModel<T> *graph, int (*hash_code)(T &, int) = 0)
    {
        // TODO

        this->graph = graph;
        this->hash_code = hash_code;
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
        DLinkedList<T> zeroInDegrees = listOfZeroInDegrees(inDegree);

        DLinkedListSE<T> result;
        Queue<T> queue;

        for (auto it = zeroInDegrees.begin(); it != zeroInDegrees.end(); ++it)
        {
            queue.push(*it);
        }

        while (!queue.empty())
        {
            T vertex = queue.front();
            queue.pop();
            result.add(vertex);

            DLinkedList<T> neighbors = this->graph->getOutwardEdges(vertex);
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
            throw std::runtime_error("Graph contains a cycle!");

        return sorted ? result.sort() : result.toDLinkedList();
    }

    DLinkedList<T> dfsSort(bool sorted = true)
    {
        // TODO
        DLinkedListSE<T> result;
        xMap<T, bool> visited(this->hash_code);
        DLinkedList<T> vertices = this->graph->nodeList;

        for (auto it = vertices.begin(); it != vertices.end(); ++it)
        {
            visited.put(*it, false);
        }

        for (auto it = vertices.begin(); it != vertices.end(); ++it)
        {
            if (!visited.get(*it))
            {
                dfsVisit(*it, visited, result);
            }
        }

        if (result.size() != this->graph->vertices().size())
            throw std::runtime_error("Graph contains a cycle!");

        return sorted ? result.sort() : result.toDLinkedList();
    }

protected:
    // Helper functions
    void dfsVisit(T vertex, xMap<T, bool> &visited, DLinkedListSE<T> &result)
    {
        visited.put(vertex, true);

        DLinkedList<T> neighbors = this->graph->getOutwardEdges(vertex);
        for (auto it = neighbors.begin(); it != neighbors.end(); ++it)
        {
            if (!visited.get(*it))
            {
                dfsVisit(*it, visited, result);
            }
        }

        result.add(0, vertex);
    }

    xMap<T, int> vertex2inDegree(int (*hash)(T &, int))
    {
        xMap<T, int> inDegree(hash);
        DLinkedList<T> vertices = this->graph->nodeList;

        for (auto it = vertices.begin(); it != vertices.end(); ++it)
        {
            inDegree.put(*it, 0);
        }

        for (auto it = vertices.begin(); it != vertices.end(); ++it)
        {
            T vertex = *it;
            DLinkedList<T> neighbors = this->graph->getOutwardEdges(vertex);
            for (auto neighborIt = neighbors.begin(); neighborIt != neighbors.end(); ++neighborIt)
            {
                T neighbor = *neighborIt;
                inDegree.put(neighbor, inDegree.get(neighbor) + 1);
            }
        }

        return inDegree;
    }
    xMap<T, int> vertex2outDegree(int (*hash)(T &, int));
    DLinkedList<T> listOfZeroInDegrees(xMap<T, int> &inDegree)
    {
        DLinkedList<T> result;

        for (auto vertex : this->graph->nodeList)
        {
            if (inDegree.get(vertex) == 0)
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

/////////////////////////////End of TopoSorter//////////////////////////////////

#endif /* TOPOSORTER_H */
