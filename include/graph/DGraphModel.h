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
#include "hash/XMap.h"
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
            throw VertexNotFoundException(this->vertex2Str(from));
        else if (!toNode)
            throw VertexNotFoundException(this->vertex2Str(to));

        fromNode->connect(toNode, weight);
    }
    void disconnect(T from, T to)
    {
        // TODO
        VertexNode *fromNode = this->getVertexNode(from);
        VertexNode *toNode = this->getVertexNode(to);

        if (!fromNode)
            throw VertexNotFoundException(this->vertex2Str(from));
        else if (!toNode)
            throw VertexNotFoundException(this->vertex2Str(to));

        Edge *deleteEdge = fromNode.getEdge(toNode);
        if (!deleteEdge)
            throw EdgeNotFoundException(this->edge2Str(deleteEdge));

        fromNode->removeTo(toNode);
        fromNode->outDegree_--;
        toNode->inDegree_--;
    }
    void remove(T vertex)
    {
        // TODO
        VertexNode *vertexNode = this->getVertexNode(vertex);

        if (!vertexNode)
            throw VertexNotFoundException(this->vertex2Str(vertex));

        typename DLinkedList<VertexNode *>::Iterator nodeIt = nodeList.begin();
        while (nodeIt != nodeList.end())
        {
            VertexNode *node = *nodeIt;
            Edge *removeEdge = node->getEdge(vertexNode);
            if (removeEdge)
            {
                node->removeTo(vertexNode);
                node->outDegree_--;
                vertexNode->inDegree_--;
            }
            nodeIt++;
        }

        while (!VertexNode.adList.empty())
        {
            Edge *edge = vertexNode->adList.get(0);
            vertexNode->removeTo(edge->to);
            vertexNode->outDegree_--;
            edge->to->inDegree_--;
        }

        this->nodeList.removeItem(vertexNode);
    }

    static DGraphModel<T> *create(
        T *vertices, int nvertices, Edge<T> *edges, int nedges,
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
