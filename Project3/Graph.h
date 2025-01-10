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
    vector<int> cities; //当前路径所经过的城市下标序列
    int length; //当前路径长度
    Path(const vector<int>& c, int l) : cities(c), length(l) {}

    //用于优先队列比较
    bool operator<(const Path& other){
        return length > other.length;
    }
};

class Graph {
private:
    string filePath1;
    string filePath2;
    vector<pair<int, string>> citys; //城市信息: pair<下标, 名称>
    vector<vector<int>> Distance; //距离矩阵
    vector<vector<int>> isNext; //邻接矩阵
    vector<vector<int>> allPaths; //储存DFS搜索路径的结果
    int allvisited[1000]; //DFS中标记已访问的城市，避免环路

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