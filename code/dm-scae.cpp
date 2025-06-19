/*
edge SCA
*/

#include<bits/stdc++.h>
#define fi first
#define se second
#define mkp make_pair
#define pb push_back
using namespace std;

/* Modify the Parameters */

const string inputFile = "1.in";
const string outputFilePre = "1";
const string outputFileSuf = ".in"; // format 1-80e.in, 1-60e.in, ...
const int MINVAL = 1;
const int MAXVAL = 100; // (1,100) relabel edge value

/* Modify the Parameters */


typedef long long ll;
unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::mt19937 rd(seed);
vector<int>outval;
vector<pair<int,int>> e;
int n,m;

struct edg{
    int u,v,w;
    edg(int u=0,int v=0,int w=0):u(u),v(v),w(w){}
};

vector<int>now,rem,ban;
vector<edg>g,h;

int rnd(int l,int r){
	return rd()%(r-l+1)+l;
}

map<int,int>M;
void relabel(vector<edg>g){
    e.clear();M.clear();
    
    int ind=0;
    
    for(auto p:g){
        if(M.find(p.u)==M.end()) M[p.u]=ind++;
        if(M.find(p.v)==M.end()) M[p.v]=ind++;
        e.push_back(make_pair(M[p.u],M[p.v]));
    }
    for(int i=0;i<ind;++i) outval[i]=0;

    int m=e.size();
	cout<<n<<" "<<m<<'\n';
	sort(e.begin(),e.end());
	for(auto [u,v]:e){
		cout<<u<<" "<<v<<" "<<rnd(MINVAL,MAXVAL)<<'\n';
	}
    cout<<endl;
}

int main(){
	ios::sync_with_stdio(0),cin.tie(0),cout.tie(0);
	freopen(inputFile.c_str(),"r",stdin);
    srand(time(0));
    string s;

	cin>>n>>m;
    ban.resize(n+5);outval.resize(n+5);
    int t=m/5,st=0;
    for(int i=0;i<n;++i) now.push_back(i);
    for(int i=0,u,v,w;i<m;++i){
        cin>>u>>v>>w;
        g.push_back(edg(u,v,w));
    }
    
    //80%
    s=outputFilePre + "-80e" + outputFileSuf;
    freopen(s.c_str(),"w",stdout);
    random_shuffle(g.begin(),g.end());
    for(int i=t,lim=g.size();i<lim;++i){
        h.push_back(g[i]);
    }
    g=h;h.clear();
    relabel(g);

    s=outputFilePre + "-60e" + outputFileSuf;
    freopen(s.c_str(),"w",stdout);
    random_shuffle(g.begin(),g.end());
    for(int i=t,lim=g.size();i<lim;++i){
        h.push_back(g[i]);
    }
    g=h;h.clear();
    relabel(g);

    s=outputFilePre + "-40e" + outputFileSuf;
    freopen(s.c_str(),"w",stdout);
    random_shuffle(g.begin(),g.end());
    for(int i=t,lim=g.size();i<lim;++i){
        h.push_back(g[i]);
    }
    g=h;h.clear();
    relabel(g);

    s=outputFilePre + "-20e" + outputFileSuf;
    freopen(s.c_str(),"w",stdout);
    random_shuffle(g.begin(),g.end());
    for(int i=t,lim=g.size();i<lim;++i){
        h.push_back(g[i]);
    }
    g=h;h.clear();
    relabel(g);

    return 0;
}

