#include <iostream>
#include "Graph.h"

using namespace std;

void showMenu() {
    cout << "��ӭʹ���ҵĽ�ͨ��ѯϵͳ��" << endl;
    cout << "--------------------------------" << endl;
    cout << "ѡ��:" << endl;
    cout << "1. ��֤ȫ������ʡ����е��人�м䲻��������ʡ���Ƿ����" << endl;
    cout << "2. ������������֮������·��" << endl;
    cout << "3. ���ұܿ�ָ�����е���������������֮������·��" << endl;
    cout << "4. ������������֮��ĵ�K��·��" << endl;
    cout << "5. �˳�" << endl;
    cout << "��ѡ��һ��ѡ��: ";
}

int main() {
    Graph map("Matrix_2.csv", "Matrix_1.csv");

    while (true) {
        showMenu();
        int choice;
        cin >> choice;
        //��֤���人���������������Ƿ����
        if (choice == 1) {
            int wuhan = map.find("�人");
            if (wuhan == -1)  cerr << "δ�ҵ��人" << endl;
            else map.bfs_check(wuhan);
        }
        //������������֮������·��
        else if (choice == 2) {
            string startCity, endCity;
            cout << "������ʼ����: ";
            cin >> startCity;
            cout << "����Ŀ�����: ";
            cin >> endCity;
            int startIndex = map.find(startCity);
            int endIndex = map.find(endCity);
            if (startIndex == -1 || endIndex == -1) {
                cerr << "δ�ҵ���Ӧ����" << endl;
                continue;
            }
            cout << "ѡ���㷨:" << endl
                << "1. �Ͻ�˹�����㷨" << endl
                << "2. ���������㷨" << endl
                << "3. ����һ��" << endl;
            int choiceway;
            cin >> choiceway;
            vector<int> empty; //���ƹ��κγ���
            if (choiceway == 1) {
                cout << "�Ͻ�˹�����㷨: " << endl;
                map.dij(startIndex, endIndex, empty);
            }
            else if (choiceway == 2) {
                cout << "���������㷨: " << endl;
                map.floyd(startIndex, endIndex, empty);
            }
            else if (choiceway == 3) {
                cout << "�Ͻ�˹�����㷨: " << endl;
                map.dij(startIndex, endIndex, empty);
                cout << "���������㷨: " << endl;
                map.floyd(startIndex, endIndex, empty);
            }
            else {
                cout << "�������������ԡ�" << endl;
                continue;
            }
            map.findAllPaths(startCity, endCity);
            map.outputPathsToFile("ȫ��·��.txt");
        }
        //���ұܿ�ָ�����е����·��
        else if (choice == 3) {
            int numCitiesToAvoid;
            cout << "������Ҫ�ƹ��ĳ�������: ";
            cin >> numCitiesToAvoid;
            vector<int> inaccessible;
            for (int i = 0; i < numCitiesToAvoid; i++) {
                cout << "������Ҫ�ƹ��ĳ�������: ";
                string cityName;
                cin >> cityName;
                int cityIndex = map.find(cityName);
                if (cityIndex == -1) {
                    cout << "��Ч�ĳ������ƣ����������롣" << endl;
                    i--;
                }
                else {
                    inaccessible.push_back(cityIndex);
                }
            }
            string startCity, endCity;
            cout << "������ʼ����: ";
            cin >> startCity;
            cout << "����Ŀ�����: ";
            cin >> endCity;
            int startIndex = map.find(startCity);
            int endIndex = map.find(endCity);
            if (startIndex == -1 || endIndex == -1) {
                cerr << "δ�ҵ���Ӧ����" << endl;
                continue;
            }
            cout << "ѡ���㷨:" << endl
                << "1. �Ͻ�˹�����㷨" << endl
                << "2. ���������㷨" << endl
                << "3. ����һ��" << endl;
            int choiceway;
            cin >> choiceway;
            if (choiceway == 1) {
                cout << "�Ͻ�˹�����㷨: " << endl;
                map.dij(startIndex, endIndex, inaccessible);
            }
            else if (choiceway == 2) {
                cout << "���������㷨: " << endl;
                map.floyd(startIndex, endIndex, inaccessible);
            }
            else if (choiceway == 3) {
                cout << "�Ͻ�˹�����㷨: " << endl;
                map.dij(startIndex, endIndex, inaccessible);
                cout << "���������㷨: " << endl;
                map.floyd(startIndex, endIndex, inaccessible);
            }
            else {
                cout << "�������������ԡ�" << endl;
            }

        }
        //���ҵ�K��·��
        else if (choice == 4) {
            string startCity, endCity;
            cout << "������ʼ����: ";
            cin >> startCity;
            cout << "����Ŀ�����: ";
            cin >> endCity;
            int startIndex = map.find(startCity);
            int endIndex = map.find(endCity);
            if (startIndex == -1 || endIndex == -1) {
                cerr << "δ�ҵ���Ӧ����" << endl;
                continue;
            }
            int k;
            cout << "������ K ֵ(�� K ��·��): ";
            cin >> k;
            map.findKthShortestPath(startIndex, endIndex, k);
        }
        else if (choice == 5) break;     
        else cout << "��Чѡ�������ѡ��" << endl;
        cout << endl << "���س�������..." << endl;
        system("pause");
        system("cls");
    }
    return 0;
}