#include <iostream>
#include "Graph.h"

using namespace std;

void showMenu() {
    cout << "欢迎使用我的交通咨询系统！" << endl;
    cout << "--------------------------------" << endl;
    cout << "选项:" << endl;
    cout << "1. 验证全国其他省会城市到武汉中间不超过两个省会是否成立" << endl;
    cout << "2. 查找两个城市之间的最短路径" << endl;
    cout << "3. 查找避开指定城市的条件下两个城市之间的最短路径" << endl;
    cout << "4. 查找两个城市之间的第K短路径" << endl;
    cout << "5. 退出" << endl;
    cout << "请选择一个选项: ";
}

int main() {
    Graph map("Matrix_2.csv", "Matrix_1.csv");

    while (true) {
        showMenu();
        int choice;
        cin >> choice;
        //验证到武汉不超过两个城市是否成立
        if (choice == 1) {
            int wuhan = map.find("武汉");
            if (wuhan == -1)  cerr << "未找到武汉" << endl;
            else map.bfs_check(wuhan);
        }
        //查找两个城市之间的最短路径
        else if (choice == 2) {
            string startCity, endCity;
            cout << "输入起始城市: ";
            cin >> startCity;
            cout << "输入目标城市: ";
            cin >> endCity;
            int startIndex = map.find(startCity);
            int endIndex = map.find(endCity);
            if (startIndex == -1 || endIndex == -1) {
                cerr << "未找到对应城市" << endl;
                continue;
            }
            cout << "选择算法:" << endl
                << "1. 迪杰斯特拉算法" << endl
                << "2. 费洛伊德算法" << endl
                << "3. 两个一起" << endl;
            int choiceway;
            cin >> choiceway;
            vector<int> empty; //不绕过任何城市
            if (choiceway == 1) {
                cout << "迪杰斯特拉算法: " << endl;
                map.dij(startIndex, endIndex, empty);
            }
            else if (choiceway == 2) {
                cout << "费洛伊德算法: " << endl;
                map.floyd(startIndex, endIndex, empty);
            }
            else if (choiceway == 3) {
                cout << "迪杰斯特拉算法: " << endl;
                map.dij(startIndex, endIndex, empty);
                cout << "费洛伊德算法: " << endl;
                map.floyd(startIndex, endIndex, empty);
            }
            else {
                cout << "输入有误，请重试。" << endl;
                continue;
            }
            map.findAllPaths(startCity, endCity);
            map.outputPathsToFile("全部路径.txt");
        }
        //查找避开指定城市的最短路径
        else if (choice == 3) {
            int numCitiesToAvoid;
            cout << "请输入要绕过的城市数量: ";
            cin >> numCitiesToAvoid;
            vector<int> inaccessible;
            for (int i = 0; i < numCitiesToAvoid; i++) {
                cout << "请输入要绕过的城市名称: ";
                string cityName;
                cin >> cityName;
                int cityIndex = map.find(cityName);
                if (cityIndex == -1) {
                    cout << "无效的城市名称，请重新输入。" << endl;
                    i--;
                }
                else {
                    inaccessible.push_back(cityIndex);
                }
            }
            string startCity, endCity;
            cout << "输入起始城市: ";
            cin >> startCity;
            cout << "输入目标城市: ";
            cin >> endCity;
            int startIndex = map.find(startCity);
            int endIndex = map.find(endCity);
            if (startIndex == -1 || endIndex == -1) {
                cerr << "未找到对应城市" << endl;
                continue;
            }
            cout << "选择算法:" << endl
                << "1. 迪杰斯特拉算法" << endl
                << "2. 费洛伊德算法" << endl
                << "3. 两个一起" << endl;
            int choiceway;
            cin >> choiceway;
            if (choiceway == 1) {
                cout << "迪杰斯特拉算法: " << endl;
                map.dij(startIndex, endIndex, inaccessible);
            }
            else if (choiceway == 2) {
                cout << "费洛伊德算法: " << endl;
                map.floyd(startIndex, endIndex, inaccessible);
            }
            else if (choiceway == 3) {
                cout << "迪杰斯特拉算法: " << endl;
                map.dij(startIndex, endIndex, inaccessible);
                cout << "费洛伊德算法: " << endl;
                map.floyd(startIndex, endIndex, inaccessible);
            }
            else {
                cout << "输入有误，请重试。" << endl;
            }

        }
        //查找第K短路径
        else if (choice == 4) {
            string startCity, endCity;
            cout << "输入起始城市: ";
            cin >> startCity;
            cout << "输入目标城市: ";
            cin >> endCity;
            int startIndex = map.find(startCity);
            int endIndex = map.find(endCity);
            if (startIndex == -1 || endIndex == -1) {
                cerr << "未找到对应城市" << endl;
                continue;
            }
            int k;
            cout << "请输入 K 值(第 K 短路径): ";
            cin >> k;
            map.findKthShortestPath(startIndex, endIndex, k);
        }
        else if (choice == 5) break;     
        else cout << "无效选项，请重新选择。" << endl;
        cout << endl << "按回车键继续..." << endl;
        system("pause");
        system("cls");
    }
    return 0;
}