/*
State Compression Dynamic Programmin
*/

#include<bits/stdc++.h>
#define fi first
#define se second
#define mkp make_pair
#define pb push_back
using namespace std;

const int N=30,M=(1<<27)|100,inf=0x3f3f3f3f;
int n,m;
int in[N],out[N],inval[N],outval[N];
int f[M],h[M];// now, high
vector<int>fr[N];
int mp[N][N];

int main(){
	freopen("50.in","r",stdin);
	freopen("50dp.out","w",stdout);
	//ios::sync_with_stdio(0),cin.tie(0),cout.tie(0);
	memset(f,0x3f,sizeof(f));
	memset(h,0x3f,sizeof(h));

	cin>>n>>m;
	for(int i=1,u,v,w;i<=m;++i){
		cin>>u>>v>>w;
		if(!mp[u][v]){
			fr[v].pb(u);
		}
		in[v] |= (1<<u);
		out[u] |= (1<<v);
		mp[u][v]+=w;
		inval[v]+=w;
		outval[u]+=w;
	}

	for(int i=0;i<n;++i){
		if(!in[i]) f[1<<i]=h[1<<i]=outval[i];
	}

	for(int s=0,lim=1<<n;s<lim;++s){
		//cout<<s<<" "<<f[s]<<" "<<h[s]<<endl;
		if(f[s]>=inf) continue;

		for(int i=0;i<n;++i){
			if((s>>i)&1 || (in[i]&s)!=in[i]) continue;
			int ns=s|(1<<i);
			int nh=f[s]+outval[i],nf=f[s]+outval[i];
			nh=max(nh,h[s]);

			//cout<<"ok check: "<<s<<" "<<ns<<endl;
			
			if(nh>h[ns]) continue;

			if(f[ns]>=inf){
				//cout<<"calc:"<<s<<" "<<ns<<" "<<f[s]<<endl;
				for(int v:fr[i]){
					if((ns&out[v])==out[v]) 
						nf-=outval[v];
				}
			}
			else{
				nf=f[ns];
			}
			
			h[ns]=nh;f[ns]=nf;
		}
	}
	cout<<h[(1<<n)-1]<<'\n';
	
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


5 7
0 1 22
1 2 21
1 3 20
2 4 24
1 3 21
1 3 22
1 4 22


*/
