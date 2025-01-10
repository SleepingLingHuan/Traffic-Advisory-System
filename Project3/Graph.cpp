#include "Graph.h"
using namespace std;

//�����ļ�����ʼ��
Graph::Graph(string file1, string file2) {
    filePath1 = file1;
    filePath2 = file2;
    ReadFile1();
    ReadFile2();
    init();
}

//����ͨ�ı���Distance�б��ΪINT_MAX
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

//��ȡ�ļ�1
void Graph::ReadFile1() {
    ifstream file(filePath1);
    if (!file.is_open()) {
        cerr << "���ļ�ʧ��: " << filePath1 << endl;
        exit(1);
    }
    //��ȡ��һ�У��洢Ϊ�ַ�������
    string headerLine;
    getline(file, headerLine);
    istringstream header(headerLine);
    string headerCell;
    int index = 0;
    while (getline(header, headerCell, ',')) {
        citys.push_back({ index++,headerCell });
    }
    //���ж�ȡ�ļ��е���������
    string line;
    while (getline(file, line)) {
        istringstream str(line);
        string cell;
        vector<int> intRow;

        while (getline(str, cell, ',')) {
            try {
                int intValue = stoi(cell); //ת��Ϊ����
                intRow.push_back(intValue);
            }
            catch (invalid_argument&) {
                if (cell == "") intRow.push_back(0);
            }
        }
        Distance.push_back(intRow); //����ǰ�е��������ݴ洢����Ӧ��������
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
        cerr << "���ļ�ʧ��: " << filePath2 << endl;
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

//���ҳ������ƣ������±�
int Graph::find(const string& cityName) {
    for (int i = 0; i < (int)citys.size(); i++) {
        if (citys[i].second == cityName) {
            return i;
        }
    }
    return -1;
}

//dij�㷨
void Graph::dij(int startCity, int endCity, const vector<int>& byPass) {
    int n = (int)citys.size();
    vector<int> dis(n, INT_MAX); //��ʾ��㵽����i����̾���
    vector<bool> visited(n, false); //��ʾ����i�Ƿ񱻷��ʹ�
    vector<int> path(n, -1); //��¼ǰһ���ڵ㣬���ڻ�ԭ·��

    //��ʼ�����ж�i�Ƿ񲻿ɴ�
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

    //Dijkstra�㷨����
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

    //���յ���INT_MAX��˵�����ɴ�
    if (dis[endCity] == INT_MAX) {
        cout << "���в��ɵ���" << endl;
        return;
    }

    //����ԭ·��
    stack<int> st;
    int cur = endCity;
    while (cur != -1) {
        st.push(cur);
        cur = path[cur];
    }
    if (st.top() != startCity) {
        cout << "���в��ɵ���" << endl;
        return;
    }

    //������
    int totalDist = dis[endCity];
    int pre = st.top();
    st.pop();
    cout << citys[pre].second << " ";
    while (!st.empty()) {
        int nxt = st.top();
        st.pop();
        cout << "--����" << Distance[pre][nxt] << "-> ";
        pre = nxt;
        cout << citys[pre].second << " ";
    }
    cout << "  ��·���ܳ��� " << totalDist << endl;
}

//Floyd�㷨
void Graph::floyd(int startCity, int endCity, const vector<int>& byPass) {
    int n = (int)citys.size();
    vector<vector<int>> distCopy = Distance;
    vector<vector<int>> path(n, vector<int>(n, -1));

    //���ƿ��ĳ�����ΪINT_MAX
    for (int c : byPass) {
        for (int j = 0; j < n; j++) {
            distCopy[c][j] = INT_MAX;
            distCopy[j][c] = INT_MAX;
        }
    }

    //��ʼ��
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (distCopy[i][j] != INT_MAX) {
                path[i][j] = j;
            }
        }
    }

    //Floyd�㷨����
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            if (distCopy[i][k] == INT_MAX) continue; //i->k���ɴ�������
            for (int j = 0; j < n; j++) {
                if (distCopy[k][j] == INT_MAX) continue; //k->j���ɴ�������
                int newDist = distCopy[i][k] + distCopy[k][j];
                if (newDist < distCopy[i][j]) {
                    distCopy[i][j] = newDist;
                    path[i][j] = path[i][k];
                }
            }
        }
    }

    //���յ���INT_MAX��˵�����ɴ�
    if (distCopy[startCity][endCity] == INT_MAX) {
        cout << "���в��ɵ���" << endl;
        return;
    }

    //���·��
    cout << citys[startCity].second << " ";
    int cur = startCity;
    while (cur != endCity) {
        int nxt = path[cur][endCity];
        cout << "--����" << Distance[cur][nxt] << "-> ";
        cur = nxt;
        cout << citys[cur].second << " ";
    }
    cout << "  ��·���ܳ��� " << distCopy[startCity][endCity] << endl;
}

//BFS���
void Graph::bfs_check(int startCity) {
    int n = (int)citys.size();
    if (startCity < 0 || startCity >= n) {
        cerr << "��ЧBFS���" << endl;
        return;
    }

    vector<int> skipCity = { 33, 32, 28, 26 }; //Ĭ������̨��������ۺ���
    vector<bool> visited(n, false);
    vector<int> parent(n, -1);
    queue<int> q;
    q.push(startCity);
    visited[startCity] = true;

    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        for (int i = 0; i < n; i++) {
            if (std::find(skipCity.begin(), skipCity.end(), i) != skipCity.end()) continue; //���i��skipCity��������
            //���cur->i��ͨ����δ���ʹ��������
            if (isNext[cur][i] && !visited[i]) {
                visited[i] = true;
                parent[i] = cur;
                q.push(i);
            }
        }
    }

    //���
    for (int i = 0; i < n; i++) {
        if (i == startCity) continue; //�ų��Լ�
        if (i == 33 || i == 32 || i == 28 || i == 26) continue; //�ų���۰���̨������
        cout << citys[i].second << " -> " << citys[startCity].second << "��";
        int node = i;
        int countMid = -1; //ͳ��;����������
        while (node != -1) {
            cout << citys[node].second << " ";
            node = parent[node];
            countMid++;
        }
        if (countMid < 0) countMid = 0;
        cout << "\t�м���и�����" << --countMid;
        if (countMid >= 0 && countMid <= 2) cout << "\t\t����" << endl;
        if (countMid >= 3) cout << "\t\t������" << endl;
    }
}

//����ȫ��·��
void Graph::findAllPaths(const string& startCity, const string& endCity) {
    allPaths.clear();
    memset(allvisited, 0, sizeof(allvisited));
    int sIdx = find(startCity);
    int eIdx = find(endCity);
    if (sIdx == -1 || eIdx == -1) {
        cerr << "�������Ʋ����ڣ��޷���������·��" << endl;
        return;
    }
    allvisited[sIdx] = 1;
    vector<int> path;
    path.push_back(sIdx);
    dfs_findAllPaths(sIdx, eIdx, path, 1);

    //�����·����������
    if (!allPaths.empty()) {
        quickSort(allPaths, 0, (int)allPaths.size() - 1);
    }
}

//DFS�ݹ鸨������
void Graph::dfs_findAllPaths(int current, int end, vector<int>& path, int index, int maxNodes) {
    //�����յ㱣��·��
    if (current == end) {
        allPaths.push_back(path);
        return;
    }
    if (index >= maxNodes + 5) return;

    //�ݹ�
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

//��������
void Graph::quickSort(vector<vector<int>>& paths, int low, int high) {
    if (low < high) {
        int pivotIndex = quickSortHelp(paths, low, high);
        quickSort(paths, low, pivotIndex - 1);
        quickSort(paths, pivotIndex + 1, high);
    }
}

//���ŷָ��
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

//����·���ܳ�
int Graph::calculatePathLength(const vector<int>& path) {
    int total = 0;
    for (int i = 0; i < (int)path.size() - 1; i++) {
        if (Distance[path[i]][path[i + 1]] == INT_MAX) return INT_MAX; //���в��ɴ����INT_MAX
        total += Distance[path[i]][path[i + 1]];
    }
    return total;
}

//���ȫ��·��
void Graph::outputPathsToFile(const string& filename) {
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cerr << "�޷�������ļ�: " << filename << endl;
        return;
    }

    int count = 1;
    for (auto& p : allPaths) {
        int length = calculatePathLength(p);
        outFile << count++ << ". ";
        for (int i = 0; i < (int)p.size() - 1; i++) {
            outFile << citys[p[i]].second << " --���� "
                << Distance[p[i]][p[i + 1]]
                << "-> ";
        }
        outFile << citys[p.back()].second
            << " : ·���ܳ��� "
            << (length == INT_MAX ? -1 : length)
            << endl << endl;
    }
    outFile.close();
}

//���ҵ�K��·�������ȶ���
void Graph::findKthShortestPath(int start, int end, int k) {
    if (start < 0 || end < 0|| start >= (int)citys.size()|| end >= (int)citys.size()) {
        cout << "��Ч�����յ�" << endl;
        return;
    }
    if (k <= 0) {
        cout << "K ֵ������� 0" << endl;
        return;
    }

    priority_queue<Path> pq; //���ȶ����д洢·�����ܾ���
    pq.push(Path({ start }, 0)); //��ʼ·��
    int foundCount = 0;
    while (!pq.empty()) {
        Path curPath = pq.top();
        pq.pop();
        int lastCity = curPath.cities.back();
        if (lastCity == end) {
            foundCount++; //�ҳ�һ��Count+1
            if (foundCount == k) {
                cout << "�� " << k << " ��·��: "; //��ӡ��·��
                for (int i = 0; i < (int)curPath.cities.size(); i++) {
                    cout << citys[curPath.cities[i]].second;
                    if (i < (int)curPath.cities.size() - 1) {
                        cout << " -> ";
                    }
                }
                cout << "  ·���ܳ��� " << curPath.length << endl;
                return;
            }
            continue; //��û��k�������Ӷ���ȡ��һ��
        }

        //��չ·������һ������
        int n = (int)citys.size();
        for (int i = 0; i < n; i++) {
            if (std::find(curPath.cities.begin(), curPath.cities.end(), i) != curPath.cities.end()) continue; //������
            if (isNext[lastCity][i] && Distance[lastCity][i] != INT_MAX) {
                vector<int> newCities = curPath.cities;
                newCities.push_back(i);
                int newLen = curPath.length + Distance[lastCity][i];
                pq.push(Path(newCities, newLen));
            }
        }
    }
    cout << "�޷��ҵ��� " << k << " ��·��" << endl; //����Ϊ����δ�ҵ�·��
}