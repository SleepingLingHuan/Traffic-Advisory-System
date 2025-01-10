#include "Graph.h"
using namespace std;

//读入文件并初始化
Graph::Graph(string file1, string file2) {
    filePath1 = file1;
    filePath2 = file2;
    ReadFile1();
    ReadFile2();
    init();
}

//不连通的边在Distance中标记为INT_MAX
void Graph::init() {
    int n = (int)citys.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (!isNext[i][j]) {
                Distance[i][j] = INT_MAX;
            }
        }
    }
}

//读取文件1
void Graph::ReadFile1() {
    ifstream file(filePath1);
    if (!file.is_open()) {
        cerr << "打开文件失败: " << filePath1 << endl;
        exit(1);
    }
    //读取第一行，存储为字符串数组
    string headerLine;
    getline(file, headerLine);
    istringstream header(headerLine);
    string headerCell;
    int index = 0;
    while (getline(header, headerCell, ',')) {
        citys.push_back({ index++,headerCell });
    }
    //逐行读取文件中的数字数据
    string line;
    while (getline(file, line)) {
        istringstream str(line);
        string cell;
        vector<int> intRow;

        while (getline(str, cell, ',')) {
            try {
                int intValue = stoi(cell); //转换为整数
                intRow.push_back(intValue);
            }
            catch (invalid_argument&) {
                if (cell == "") intRow.push_back(0);
            }
        }
        Distance.push_back(intRow); //将当前行的整数数据存储到相应的数组中
    }
    file.close();
    for (int i = 0; i < citys.size(); i++) {
        for (int j = i; j < citys.size(); j++) {
            if (i == j) Distance[i][j] = INT_MAX;
            else Distance[i][j] = Distance[j][i];
        }
    }
}

void Graph::ReadFile2() {
    ifstream file2(filePath2);
    if (!file2.is_open()) {
        cerr << "打开文件失败: " << filePath2 << endl;
        exit(1);
    }
    string headerLine;
    getline(file2, headerLine);
    string line;
    while (getline(file2, line)) {
        istringstream str(line);
        string cell;
        vector<int> intRow;
        while (getline(str, cell, ',')) {
            try {
                int intValue = stoi(cell); 
                intRow.push_back(intValue);
            }
            catch (invalid_argument&) {}
        }
        isNext.push_back(intRow);
    }
    file2.close();
}

//查找城市名称，返回下标
int Graph::find(const string& cityName) {
    for (int i = 0; i < (int)citys.size(); i++) {
        if (citys[i].second == cityName) {
            return i;
        }
    }
    return -1;
}

//dij算法
void Graph::dij(int startCity, int endCity, const vector<int>& byPass) {
    int n = (int)citys.size();
    vector<int> dis(n, INT_MAX); //表示起点到城市i的最短距离
    vector<bool> visited(n, false); //表示城市i是否被访问过
    vector<int> path(n, -1); //记录前一个节点，用于还原路径

    //初始化，判断i是否不可达
    for (int i = 0; i < n; i++) {
        if (std::find(byPass.begin(), byPass.end(), i) != byPass.end()) {
            dis[i] = INT_MAX;
            path[i] = -1;
        }
        else {
            dis[i] = Distance[startCity][i];
            if (dis[i] != INT_MAX && i != startCity) {
                path[i] = startCity;
            }
        }
    }
    dis[startCity] = 0;

    //Dijkstra算法主体
    for (int i = 0; i < n - 1; i++) {
        int p = -1;
        int minDist = INT_MAX;
        for (int j = 0; j < n; j++) {
            if (!visited[j] && dis[j] < minDist) {
                minDist = dis[j];
                p = j;
            }
        }
        if (p == -1) break;
        visited[p] = true;
        for (int k = 0; k < n; k++) {
            if (std::find(byPass.begin(), byPass.end(), k) != byPass.end()) continue;
            if (!visited[k] && Distance[p][k] != INT_MAX && dis[p] != INT_MAX) {
                int newDist = dis[p] + Distance[p][k];
                if (newDist < dis[k]) {
                    dis[k] = newDist;
                    path[k] = p;
                }
            }
        }
    }

    //若终点是INT_MAX，说明不可达
    if (dis[endCity] == INT_MAX) {
        cout << "城市不可到达" << endl;
        return;
    }

    //逆向还原路径
    stack<int> st;
    int cur = endCity;
    while (cur != -1) {
        st.push(cur);
        cur = path[cur];
    }
    if (st.top() != startCity) {
        cout << "城市不可到达" << endl;
        return;
    }

    //输出结果
    int totalDist = dis[endCity];
    int pre = st.top();
    st.pop();
    cout << citys[pre].second << " ";
    while (!st.empty()) {
        int nxt = st.top();
        st.pop();
        cout << "--距离" << Distance[pre][nxt] << "-> ";
        pre = nxt;
        cout << citys[pre].second << " ";
    }
    cout << "  ：路径总长度 " << totalDist << endl;
}

//Floyd算法
void Graph::floyd(int startCity, int endCity, const vector<int>& byPass) {
    int n = (int)citys.size();
    vector<vector<int>> distCopy = Distance;
    vector<vector<int>> path(n, vector<int>(n, -1));

    //把绕开的城市设为INT_MAX
    for (int c : byPass) {
        for (int j = 0; j < n; j++) {
            distCopy[c][j] = INT_MAX;
            distCopy[j][c] = INT_MAX;
        }
    }

    //初始化
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (distCopy[i][j] != INT_MAX) {
                path[i][j] = j;
            }
        }
    }

    //Floyd算法主体
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            if (distCopy[i][k] == INT_MAX) continue; //i->k不可达则跳过
            for (int j = 0; j < n; j++) {
                if (distCopy[k][j] == INT_MAX) continue; //k->j不可达则跳过
                int newDist = distCopy[i][k] + distCopy[k][j];
                if (newDist < distCopy[i][j]) {
                    distCopy[i][j] = newDist;
                    path[i][j] = path[i][k];
                }
            }
        }
    }

    //若终点是INT_MAX，说明不可达
    if (distCopy[startCity][endCity] == INT_MAX) {
        cout << "城市不可到达" << endl;
        return;
    }

    //输出路径
    cout << citys[startCity].second << " ";
    int cur = startCity;
    while (cur != endCity) {
        int nxt = path[cur][endCity];
        cout << "--距离" << Distance[cur][nxt] << "-> ";
        cur = nxt;
        cout << citys[cur].second << " ";
    }
    cout << "  ：路径总长度 " << distCopy[startCity][endCity] << endl;
}

//BFS检查
void Graph::bfs_check(int startCity) {
    int n = (int)citys.size();
    if (startCity < 0 || startCity >= n) {
        cerr << "无效BFS起点" << endl;
        return;
    }

    vector<int> skipCity = { 33, 32, 28, 26 }; //默认跳过台北澳门香港海口
    vector<bool> visited(n, false);
    vector<int> parent(n, -1);
    queue<int> q;
    q.push(startCity);
    visited[startCity] = true;

    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        for (int i = 0; i < n; i++) {
            if (std::find(skipCity.begin(), skipCity.end(), i) != skipCity.end()) continue; //如果i在skipCity里则跳过
            //如果cur->i连通，且未访问过，则访问
            if (isNext[cur][i] && !visited[i]) {
                visited[i] = true;
                parent[i] = cur;
                q.push(i);
            }
        }
    }

    //输出
    for (int i = 0; i < n; i++) {
        if (i == startCity) continue; //排除自己
        if (i == 33 || i == 32 || i == 28 || i == 26) continue; //排除香港澳门台北海口
        cout << citys[i].second << " -> " << citys[startCity].second << "：";
        int node = i;
        int countMid = -1; //统计途径城市数量
        while (node != -1) {
            cout << citys[node].second << " ";
            node = parent[node];
            countMid++;
        }
        if (countMid < 0) countMid = 0;
        cout << "\t中间城市个数：" << --countMid;
        if (countMid >= 0 && countMid <= 2) cout << "\t\t成立" << endl;
        if (countMid >= 3) cout << "\t\t不成立" << endl;
    }
}

//查找全部路径
void Graph::findAllPaths(const string& startCity, const string& endCity) {
    allPaths.clear();
    memset(allvisited, 0, sizeof(allvisited));
    int sIdx = find(startCity);
    int eIdx = find(endCity);
    if (sIdx == -1 || eIdx == -1) {
        cerr << "城市名称不存在，无法搜索所有路径" << endl;
        return;
    }
    allvisited[sIdx] = 1;
    vector<int> path;
    path.push_back(sIdx);
    dfs_findAllPaths(sIdx, eIdx, path, 1);

    //结果按路径长度排序
    if (!allPaths.empty()) {
        quickSort(allPaths, 0, (int)allPaths.size() - 1);
    }
}

//DFS递归辅助函数
void Graph::dfs_findAllPaths(int current, int end, vector<int>& path, int index, int maxNodes) {
    //到达终点保存路径
    if (current == end) {
        allPaths.push_back(path);
        return;
    }
    if (index >= maxNodes + 5) return;

    //递归
    int n = (int)citys.size();
    for (int i = 0; i < n; i++) {
        if (!allvisited[i] && isNext[current][i] && Distance[current][i] != INT_MAX) {
            allvisited[i] = 1;
            path.push_back(i);
            dfs_findAllPaths(i, end, path, index + 1, maxNodes);
            path.pop_back();
            allvisited[i] = 0;
        }
    }
}

//快速排序
void Graph::quickSort(vector<vector<int>>& paths, int low, int high) {
    if (low < high) {
        int pivotIndex = quickSortHelp(paths, low, high);
        quickSort(paths, low, pivotIndex - 1);
        quickSort(paths, pivotIndex + 1, high);
    }
}

//快排分割函数
int Graph::quickSortHelp(vector<vector<int>>& paths, int low, int high) {
    vector<int> pivotPath = paths[high];
    int pivotLen = calculatePathLength(pivotPath);
    int i = low - 1;
    for (int j = low; j <= high - 1; j++) {
        int currLen = calculatePathLength(paths[j]);
        if (currLen <= pivotLen) {
            i++;
            swap(paths[i], paths[j]);
        }
    }
    swap(paths[i + 1], paths[high]);
    return i + 1;
}

//计算路径总长
int Graph::calculatePathLength(const vector<int>& path) {
    int total = 0;
    for (int i = 0; i < (int)path.size() - 1; i++) {
        if (Distance[path[i]][path[i + 1]] == INT_MAX) return INT_MAX; //若有不可达，返回INT_MAX
        total += Distance[path[i]][path[i + 1]];
    }
    return total;
}

//输出全部路径
void Graph::outputPathsToFile(const string& filename) {
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cerr << "无法打开输出文件: " << filename << endl;
        return;
    }

    int count = 1;
    for (auto& p : allPaths) {
        int length = calculatePathLength(p);
        outFile << count++ << ". ";
        for (int i = 0; i < (int)p.size() - 1; i++) {
            outFile << citys[p[i]].second << " --距离 "
                << Distance[p[i]][p[i + 1]]
                << "-> ";
        }
        outFile << citys[p.back()].second
            << " : 路径总长度 "
            << (length == INT_MAX ? -1 : length)
            << endl << endl;
    }
    outFile.close();
}

//查找第K短路径，优先队列
void Graph::findKthShortestPath(int start, int end, int k) {
    if (start < 0 || end < 0|| start >= (int)citys.size()|| end >= (int)citys.size()) {
        cout << "无效起点或终点" << endl;
        return;
    }
    if (k <= 0) {
        cout << "K 值必须大于 0" << endl;
        return;
    }

    priority_queue<Path> pq; //优先队列中存储路径和总距离
    pq.push(Path({ start }, 0)); //初始路径
    int foundCount = 0;
    while (!pq.empty()) {
        Path curPath = pq.top();
        pq.pop();
        int lastCity = curPath.cities.back();
        if (lastCity == end) {
            foundCount++; //找出一条Count+1
            if (foundCount == k) {
                cout << "第 " << k << " 短路径: "; //打印该路径
                for (int i = 0; i < (int)curPath.cities.size(); i++) {
                    cout << citys[curPath.cities[i]].second;
                    if (i < (int)curPath.cities.size() - 1) {
                        cout << " -> ";
                    }
                }
                cout << "  路径总长度 " << curPath.length << endl;
                return;
            }
            continue; //若没到k，继续从队列取下一个
        }

        //扩展路径到下一个城市
        int n = (int)citys.size();
        for (int i = 0; i < n; i++) {
            if (std::find(curPath.cities.begin(), curPath.cities.end(), i) != curPath.cities.end()) continue; //不含环
            if (isNext[lastCity][i] && Distance[lastCity][i] != INT_MAX) {
                vector<int> newCities = curPath.cities;
                newCities.push_back(i);
                int newLen = curPath.length + Distance[lastCity][i];
                pq.push(Path(newCities, newLen));
            }
        }
    }
    cout << "无法找到第 " << k << " 短路径" << endl; //队列为空且未找到路径
}