/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   DGraphModel.h
 * Author: LTSACH
 *
 * Created on 23 August 2020, 19:36
 */

#ifndef DGRAPHMODEL_H
#define DGRAPHMODEL_H
#include "graph/AbstractGraph.h"
#include "stacknqueue/Queue.h"
#include "stacknqueue/Stack.h"
#include "hash/xMap.h"
// #include "stacknqueue/PriorityQueue.h"
#include "sorting/DLinkedListSE.h"

//////////////////////////////////////////////////////////////////////
///////////// GraphModel: Directed Graph Model    ////////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
class DGraphModel : public AbstractGraph<T>
{
private:
public:
    using Edge = typename AbstractGraph<T>::Edge;
    using VertexNode = typename AbstractGraph<T>::VertexNode;
    using Iterator = typename AbstractGraph<T>::Iterator;

    DGraphModel(
        bool (*vertexEQ)(T &, T &),
        string (*vertex2str)(T &)) : AbstractGraph<T>(vertexEQ, vertex2str)
    {
    }

    void connect(T from, T to, float weight = 0)
    {
        // TODO
        VertexNode *fromNode = this->getVertexNode(from);
        VertexNode *toNode = this->getVertexNode(to);

        if (!fromNode)
            throw VertexNotFoundException(this->vertex2str(from));
        else if (!toNode)
            throw VertexNotFoundException(this->vertex2str(to));

        fromNode->connect(toNode, weight);
    }
    void disconnect(T from, T to)
    {
        // TODO
        VertexNode *fromNode = this->getVertexNode(from);
        VertexNode *toNode = this->getVertexNode(to);

        if (!fromNode)
            throw VertexNotFoundException(this->vertex2str(from));
        else if (!toNode)
            throw VertexNotFoundException(this->vertex2str(to));

        Edge *deleteEdge = fromNode->getEdge(toNode);
        if (!deleteEdge)
        {
            string deleteEdge = "E(" + this->vertex2Str(*fromNode) + ", " + this->vertex2Str(*toNode) + ")";
            throw EdgeNotFoundException(deleteEdge);
        }

        fromNode->removeTo(toNode);
        fromNode->setOutDegree(fromNode->outDegree() - 1);
        toNode->setInDegree(toNode->inDegree() - 1);
    }
    void remove(T vertex)
    {
        // TODO
        VertexNode *vertexNode = this->getVertexNode(vertex);

        if (!vertexNode)
            throw VertexNotFoundException(this->vertex2str(vertex));

        typename DLinkedList<VertexNode *>::Iterator nodeIt = this->nodeList.begin();
        while (nodeIt != this->nodeList.end())
        {
            VertexNode *node = *nodeIt;
            Edge *removeEdge = node->getEdge(vertexNode);
            if (removeEdge)
            {
                node->removeTo(vertexNode);
                node->setOutDegree(node->outDegree() - 1);
                vertexNode->setInDegree(vertexNode->inDegree() - 1);
            }
            nodeIt++;
        }

        while (!vertexNode->getOutwardEdges().empty())
        {
            Edge *edge = vertexNode->getOutwardEdges().get(0);
            vertexNode->removeTo(edge->getToNode());
            vertexNode->setOutDegree(vertexNode->outDegree() - 1);
            edge->getToNode()->setInDegree(edge->getToNode()->inDegree() - 1);
        }

        this->nodeList.removeItem(vertexNode);
    }

    static DGraphModel<T> *create(
        T *vertices, int nvertices, Edge *edges, int nedges,
        bool (*vertexEQ)(T &, T &),
        string (*vertex2str)(T &))
    {
        // TODO

        DGraphModel *graph = new DGraphModel<T>(vertexEQ, vertex2str);

        for (int i = 0; i < nvertices; i++)
        {
            graph->add(vertices[i]);
        }

        for (int i = 0; i < nedges; i++)
        {
            Edge edge = edges[i];
            graph->connect(edge.from, edge.to, edge.weight);
        }

        return graph;
    }
};

#endif /* DGRAPHMODEL_H */
