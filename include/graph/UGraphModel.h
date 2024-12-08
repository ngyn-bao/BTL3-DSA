/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   UGraphModel.h
 * Author: LTSACH
 *
 * Created on 24 August 2020, 15:16
 */

#ifndef UGRAPHMODEL_H
#define UGRAPHMODEL_H

#include "graph/AbstractGraph.h"
// #include "stacknqueue/PriorityQueue.h"

//////////////////////////////////////////////////////////////////////
///////////// UGraphModel: Undirected Graph Model ////////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
class UGraphModel : public AbstractGraph<T>
{
private:
public:
    // class UGraphAlgorithm;
    // friend class UGraphAlgorithm;

    UGraphModel(
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

        if (fromNode == toNode)
            fromNode->connect(toNode, weight);
        else
        {
            fromNode->connect(toNode, weight);
            toNode->connect(fromNode, weight);
        }
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

        Edge *edge = fromNode->getEdge(toNode);
        if (!edge)
            throw EdgeNotFoundException(this->vertex2Str(from), this->vertex2Str(to));

        if (fromNode == toNode)
        {
            fromNode->removeTo(toNode);
        }
        else
        {
            fromNode->removeTo(toNode);
            toNode->removeTo(fromNode);
        }
    }
    void remove(T vertex)
    {
        // TODO
        VertexNode *vertexNode = this->getVertexNode(vertex);

        if (!vertexNode)
            throw VertexNotFoundException(this->vertex2Str(vertex));

        typename DLinkedList<VertexNode *>::Iterator nodeIt = this->nodeList.begin();
        while (nodeIt != this->nodeList.end())
        {
            VertexNode *otherNode = *nodeIt;
            if (otherNode->getEdge(vertexNode))
            {
                otherNode->removeTo(vertexNode);
            }
            if (vertexNode->getEdge(otherNode))
            {
                vertexNode->removeTo(otherNode);
            }

            nodeIt++;
        }

        this->nodeList.removeItem(vertexNode);
    }
    static UGraphModel<T> *create(
        T *vertices, int nvertices, Edge<T> *edges, int nedges,
        bool (*vertexEQ)(T &, T &),
        string (*vertex2str)(T &))
    {
        // TODO
        UGraphModel<T> *graph = new UGraphModel<T>(vertexEQ, vertex2str);

        for (int i = 0; i < nvertices; i++)
            graph->addVertex(vertices[i]);

        for (int i = 0; i < nedges; i++)
        {
            Edge<T> edge = edges[i];
            graph->connect(edge.from, edge.to, edge.weight);
        }
        return graph;
    }
};

#endif /* UGRAPHMODEL_H */
