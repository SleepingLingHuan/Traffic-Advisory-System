#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <queue>
#include <stack>
#include <algorithm>
#include <climits>
#include <cstring>
using namespace std;

struct Path {
    vector<int> cities; //��ǰ·���������ĳ����±�����
    int length; //��ǰ·������
    Path(const vector<int>& c, int l) : cities(c), length(l) {}

    //�������ȶ��бȽ�
    bool operator<(const Path& other){
        return length > other.length;
    }
};

class Graph {
private:
    string filePath1;
    string filePath2;
    vector<pair<int, string>> citys; //������Ϣ: pair<�±�, ����>
    vector<vector<int>> Distance; //�������
    vector<vector<int>> isNext; //�ڽӾ���
    vector<vector<int>> allPaths; //����DFS����·���Ľ��
    int allvisited[1000]; //DFS�б���ѷ��ʵĳ��У����⻷·

public:
    Graph(string file1, string file2);
    void init();
    void ReadFile1();
    void ReadFile2();
    int find(const string& cityName);
    void dij(int startIndex, int endIndex, const vector<int>& inaccessible);
    void floyd(int startIndex, int endIndex, const vector<int>& inaccessible);
    void bfs_check(int startIndex);
    void findAllPaths(const string& start, const string& end);
    void dfs_findAllPaths(int current, int end, vector<int>& path, int index, int maxNodes = 10);
    void quickSort(vector<vector<int>>& paths, int low, int high);
    int quickSortHelp(vector<vector<int>>& paths, int low, int high);
    int calculatePathLength(const vector<int>& path);
    void outputPathsToFile(const string& filename);
    void findKthShortestPath(int start, int end, int k);
};