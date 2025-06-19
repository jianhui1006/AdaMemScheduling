/*
RANDOM TREE + EDGES
*/

#include<bits/stdc++.h>
#define fi first
#define se second
#define mkp make_pair
#define pb push_back
using namespace std;


/* Modify the Parameters */
const int N = 20;
const int M = 30;
const int MINVAL = 1; // min edge value
const int MAXVAL = 200; // max edge value
const string outputFile = "1.in";

/* Modify the Parameters */


unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::mt19937 rd(seed);
vector<int>outval;
vector<pair<int,int>> e;
vector<int>bl;//0=st,1=ed,2=mid
vector<int>head;

int rnd(int l,int r){
	return rd()%(r-l+1)+l;
}

int main(){
	ios::sync_with_stdio(0),cin.tie(0),cout.tie(0);
	freopen(outputFile.c_str(),"w",stdout);

	int n=N,m=M;
    cout<<n<<" "<<m<<'\n';
	outval.resize(n);bl.resize(n);
    for(int i=1;i<n;++i){
        int u=rnd(0,i-1);
        cout<<u<<" "<<i<<" "<<rnd(MINVAL,MAXVAL)<<'\n';
    }
	for(int i=n-1;i<m;++i){
        int u=rnd(0,n-1),v=rnd(0,n-1);
        while(u==v) v=rnd(0,n-1);
        if(u>v) swap(u,v);
		cout<<u<<" "<<v<<" "<<rnd(MINVAL,MAXVAL)<<'\n';
	}
}

/*

8 9
0 1 10
1 2 10
2 3 10
0 4 0
4 5 40
5 3 10
0 6 0
6 7 10
7 3 10

12 14
0 3 10
1 4 8
2 5 11
0 6 7
1 7 10
2 8 12
3 9 7
4 9 5
5 9 10
6 10 8
7 10 4
8 10 5
9 11 20
10 11 10
*/
