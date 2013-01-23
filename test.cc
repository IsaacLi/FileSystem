
/**
 * @Brief:  by Isaac Li
 * 我没有去重，其实要去一下重的
 */
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <vector>
using namespace std;

vector<vector<int> > f[105][10000];
int num[105];

int main()
{
	int n, sum;
	scanf("%d", &n);
	scanf("%d", &sum);
	for (int i = 0; i < n; i++) scanf("%d", num + i);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j <= sum; j++) {
			vector<vector<int> > vv;
			vv = f[i][j];
			if (vv.size() == 0 && j==0) {
				vector<int> v;
				v.push_back(i);
				vv.push_back(v);
			}
			else {
				for (vector<vector<int> >::iterator it = vv.begin(); it != vv.end(); it++) {
					it->push_back(i);
					//cout << it->size()<< endl;
				}
			}
			for (vector<vector<int> >::iterator it = f[i][j + num[i]].begin(); it != f[i][j + num[i]].end(); it++) {
				vv.push_back(*it);
			}
			f[i + 1][j + num[i]] = vv;
			cout << "f[" << i + 1 << "][" << j + num[i] << "]" << endl;
			for (vector<vector<int> >:: iterator it = vv.begin(); it != vv.end(); it++) {
			//cout << "2" << endl;
				for (vector<int>:: iterator it2= it->begin(); it2 != it->end(); it2++)
					cout << num[*it2] << " ";
				cout << endl;
			}
			cout << endl;
		}
		//cout << "1" << endl;
	}
	for (vector<vector<int> >:: iterator it = f[n][sum].begin(); it != f[n][sum].end(); it++) {
		//cout << "2" << endl;
		for (vector<int>:: iterator it2= it->begin(); it2 != it->end(); it2++)
			cout << num[*it2] << " ";
		cout << endl;
	}
	return 0;
}
