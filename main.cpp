#include <iostream>
#include <iomanip>
#include <sstream> //lệnh compiler: g++ -Idemo -Iinclude main.cpp
#include <string>  //lệnh run: ./main.exe hoặc ./a.exe (tùy file thực thi máy xuất ra sau khi compile thành công)
#include <fstream>
#include "include/list/listheader.h"
#include "demo/graph/DGraphDemo.h"
#include "demo/graph/UGraphDemo.h"
#include "demo/stacknqueue/QueueDemo.h"
#include "demo/stacknqueue/StackDemo.h"
#include "include/graph/AbstractGraph.h"
#include "include/graph/DGraphModel.h"
#include "include/graph/UGraphModel.h"
#include "include/stacknqueue/Queue.h"
#include "include/stacknqueue/Stack.h"

using namespace std;

int main(int argc, char **argv)
{
    ofstream outFile1("outputdgraph.txt");
    ofstream outFile2("outputugraph.txt");
    ofstream outFile3("outputqueue.txt");
    ofstream outFile4("outputstack.txt");

    if (!outFile1)
    {
        cerr << "Không thể mở file outputdgraph.txt để ghi!" << endl;
        return 1;
    }

    if (!outFile2)
    {
        cerr << "Không thể mở file outputugraph.txt để ghi!" << endl;
        return 1;
    }

    if (!outFile3)
    {
        cerr << "Không thể mở file outputqueue.txt để ghi!" << endl;
        return 1;
    }

    if (!outFile4)
    {
        cerr << "Không thể mở file outputstack.txt để ghi!" << endl;
        return 1;
    }
    // TEST QUEUE:
    void (*queueDemos[])() = {0, queueDemo1, queueDemo2, queueDemo3};
    outFile3 << "TEST QUEUE DEMO:......................................." << endl;

    for (int i = 1; i <= 3; i++)
    {
        outFile3 << "Demo " << i << "-------------------------" << endl;
        // outFile1 << "Demo " << 3 << "-------------------------" << endl;
        outFile3 << endl;

        streambuf *coutBuffer = cout.rdbuf();
        cout.rdbuf(outFile3.rdbuf());
        queueDemos[i]();
        // DGraphDemo2();
        // hashDemo3();
        cout.rdbuf(coutBuffer);
    }

    outFile3.close();

    // TEST STACK:
    void (*stackDemos[])() = {0, stackDemo1, stackDemo2, stackDemo3};
    outFile4 << "TEST STACK DEMO:......................................." << endl;

    for (int i = 1; i <= 3; i++)
    {
        outFile4 << "Demo " << i << "-------------------------" << endl;
        // outFile1 << "Demo " << 3 << "-------------------------" << endl;
        outFile4 << endl;

        streambuf *coutBuffer = cout.rdbuf();
        cout.rdbuf(outFile4.rdbuf());
        stackDemos[i]();
        // DGraphDemo2();
        // hashDemo3();
        cout.rdbuf(coutBuffer);
    }

    outFile4.close();

    // TEST D-GRAPH:
    void (*dGraphDemos[])() = {0, DGraphDemo1, DGraphDemo2, DGraphDemo3, DGraphDemo4};
    outFile1 << "TEST D-GRAPH DEMO:......................................." << endl;

    for (int i = 1; i <= 4; i++)
    {
        outFile1 << "Demo " << i << "-------------------------" << endl;
        // outFile1 << "Demo " << 3 << "-------------------------" << endl;
        outFile1 << endl;

        streambuf *coutBuffer = cout.rdbuf();
        cout.rdbuf(outFile1.rdbuf());
        dGraphDemos[i]();
        // DGraphDemo2();
        // hashDemo3();
        cout.rdbuf(coutBuffer);
    }

    outFile1.close();

    // TEST U-GRAPH:
    void (*ugraphDemos[])() = {0, ugraphDemo1, ugraphDemo2};
    outFile2 << "TEST U-GRAPH DEMO:......................................." << endl;

    for (int i = 1; i <= 2; i++)
    { // test heap i<=3
        outFile2 << "Demo " << i << "-------------------------" << endl;
        outFile2 << endl;

        streambuf *coutBuffer = cout.rdbuf();
        cout.rdbuf(outFile2.rdbuf());
        ugraphDemos[i]();
        cout.rdbuf(coutBuffer);
    }

    outFile2.close();

    return 0;
}
