#include<bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int,int>;
using pll = pair<ll,ll>;


/* Modify the Parameters */

const string inputFile = "1.in";
const string outputFile = "1.out";
const int C = 3; // the number of maximum calculate degree in GAG
const int UPVAL = 100; // the increase priority value of the first search branch, by muilt-in
const int TIMESVAL = 50; // the increase priority value of the first search branch, by appear times
const double PERCENT = 0.4; // improve node percent
const int IROUND = 3; // improve round
const int LIMITCNT = 1e5; // more times in basic IIA
const int IIAFLAG = 1; // 0=basic, 1=linked list, 2=treap

/* Modify the Parameters */


/* Global Storage */

const ll inf=0x3f3f3f3f3f3f3f3f;
const int infInt=0x3f3f3f3f;
const int LIM=0x3f3f3f3f;
int tmpn,tmpm;
int totot;
vector<int> avgrb[10];
vector<vector<pii>>te,tg;
vector<int>tin,tout;
vector<int>inval,outval;

/* Global Storage */



namespace Input{
    int nodeid=0,m=0,n=0;
    map<string,int>id,cb;
    map<string,vector<array<ll,3>>>edg;
    vector<ll>val;
    vector<int>vis;

    void solve(vector<map<string, vector<string>>> &nodes, map<string, vector<ll>> &outputs){
        n=nodes.size();
        //cout<<n<<'\n';
        val.resize(n,0);
        vis.resize(n,0);
        for(auto mp:nodes){
            string x = mp["name"][0];
            if(id.find(x)==id.end()) id[x]=nodeid++;
            for(auto v:mp["output"]){
                cb[v]=id[x];
            }
        }
        
        for(auto mp:nodes){
            string x = mp["name"][0];
            for(auto fr:mp["input"]){
                array<ll,3> tmp={cb[fr],id[x],outputs[fr][0]};
                if(edg[fr].size()>0) tmp[2]=0;
                edg[fr].push_back(tmp);
                ++m;
            }
        }

        tmpn=n;tmpm=m;
        te.resize(n);tg.resize(n);
        for(auto [_,vec]:edg){
            for(auto it:vec){
                te[it[0]].push_back({it[1],it[2]});
                tg[it[1]].push_back({it[0],it[2]});
            }
        }
    }

    void getGraphInput(){
        freopen(inputFile.c_str(),"r",stdin);
        freopen(outputFile.c_str(),"w",stdout);

        cin>>n>>m;
        tmpn=n;tmpm=m;
        te.resize(n);tg.resize(n);

        for(int i=1,u,v,w;i<=m;++i){
            cin>>u>>v>>w; 
            te[u].push_back({v,w});
            tg[v].push_back({u,w});
        }
        cout<<"done input"<<endl;
    }

    void init(){
        inval.resize(n);outval.resize(n);
        tin.resize(n);tout.resize(n);
        for(int i=0;i<n;++i){
            tin[i]=tg[i].size();
            tout[i]=te[i].size();
            for(auto [v,w]:te[i]){
                inval[v]+=w;
                outval[i]+=w;
            }
        }
        cout<<"done init"<<endl;
    }

    void getInput(int op){
        getGraphInput();
        init();
    }
}

namespace Topsort{
    vector<int> in,out;
    ll getCost(vector<int>ord){
        if(ord.size()!=tmpn){
            cout<<"no enough nodes!"<<endl;
            return inf;
        }
        in=tin;out=tout;

        ll now=0,res=0;
        for(auto x:ord){
            if(in[x]){
                cout<<"not a topsort!"<<endl;
                return inf; // not a topsort
            }
            now+=outval[x];
            res=max(res,now);

            //dec
            for(auto [v,w]:te[x]){
                --in[v];
            }
            for(auto [v,w]:tg[x]){
                --out[v];
                if(!out[v]) now-=outval[v];
            }
        }
        return res;
    }
}

namespace PostOrderMemoryScheduler{
    int n,m;
    vector<vector<pii>>e,g;
    vector<int>ord,vis;

    void dfs(int x){
        vis[x]=1;
        for(auto [v,w]:e[x]){
            if(!vis[v]) dfs(v);
        }
        ord.push_back(x);
    }

    void init(){
        n=tmpn;m=tmpm;e=te;g=tg;
        vis.resize(n,0);
    }
    vector<int> solve(){
        init();
        for(int i=0;i<n;++i){
            if(!vis[i]) dfs(i);
        }
        reverse(ord.begin(),ord.end());
        //for(auto v:ord) cout<<v<<endl;
        return ord;
    }
}

namespace DFSMemoryScheduler{
    int n,m;
    vector<vector<pii>>e,g;
    vector<int>ord,vis,sa,rk; //sa[i] is the i-th rank node, rk[i] is the i node's rank 
    vector<pair<ll,ll> > val; //pair<int,int> first is extra_node=max(0,outdeg-1) ,second is outval

    void dfs1(int x){
        vis[x]=1;
        for(auto [v,w]:e[x]){
            if(!vis[v]) dfs1(v);
            val[x].first=min(inf,val[x].first+val[v].first);
            val[x].second=min(inf,val[x].second+val[v].second);
        }
    }

    void dfs2(int x){
        vis[x]=1;
        for(auto [v,w]:e[x]){
            if(!vis[v]) dfs2(v);
        }
        ord.push_back(x);
    }

    void init(){
        n=tmpn;m=tmpm;e=te;g=tg;
        vis.resize(n,0);val.resize(n,{0,0});rk.resize(n);sa.resize(n);

        for(int i=0;i<n;++i){
            val[i]=make_pair(max(0,tout[i]-1),outval[i]);
            sa[i]=i;
        }
        for(int i=0;i<n;++i){
            if(!vis[i]) dfs1(i);
        }
        for(int i=0;i<n;++i){
            vis[i]=0;
        }
        sort(sa.begin(),sa.end(),[&](const int x,const int y){
            if(val[x]!=val[y]) return val[x]>val[y];
            return x<y;
        });
        for(int i=0;i<n;++i){
            rk[sa[i]]=i;
        }
        for(int i=0;i<n;++i){
            sort(e[i].begin(),e[i].end(),[&](const pii&x, const pii&y){
                return rk[x.first]<rk[y.first];
            });
        }
    }
    vector<int> solve(){
        init();
        for(int i=0;i<n;++i){
            if(!vis[sa[i]]) dfs2(sa[i]);
        }
        reverse(ord.begin(),ord.end());
        //for(auto v:ord) cout<<v<<endl;
        return ord;
    }
}

namespace ListMemoryScheduler{
    int n,m;
    vector<vector<pii>>e,g;
    vector<int>ord,inq;
    vector<int>in,out,rout,freem,vis;

    struct myq{
        int c;
        myq(int c=0):c(c){}
        const bool operator ==(const myq&y) const{
            return c==y.c;
        }
        // use set
        const bool operator <(const myq&y) const{
            if(freem[c]==freem[y.c]){
                if(rout[c]==rout[y.c]){
                    return c>y.c;
                }
                return rout[c]>rout[y.c];
            }
            return freem[c]>freem[y.c];
        }
    };

    void init(){
        n=tmpn;m=tmpm;e=te;g=tg;out=tout;in=tin;rout=tout;
        freem.resize(n,0);inq.resize(n,0);vis.resize(n,0);
    }
    void topsort(){
        set<myq>q; // {can free memory, outdeg}
        set<int>tmp;
        for(int i=0;i<n;++i){
            if(!in[i]){
                q.insert(myq(i)); 
                inq[i]=1;
                tmp.insert(i);
            }
        }
        while(!q.empty()){
            auto it=q.begin();
            int x=it->c; inq[x]=0; vis[x]=1;

            ord.push_back(x);
            q.erase(it);

            // first try insert
            for(auto [v,w]:e[x]){
                --in[v];
                if(!in[v]){
                    inq[v]=1;
                    q.insert(myq(v));
                }
            }

            // check if next can free
            for(auto [v,w]:g[x]){
                --out[v];
                if(out[v]==1){
                    for(auto [z,_]:e[v]){
                        if(vis[z]) continue;
                        bool flag=0;
                        if(inq[z]){
                            flag=1;
                            q.erase(q.find(myq(z)));
                        }
                        freem[z]+=outval[v];
                        if(flag){
                            q.insert(myq(z));
                        }    
                    }
                }
            }
        }
    }
    vector<int> solve(){
        init();
        topsort();
        //for(auto v:ord) cout<<v<<endl;
        return ord;
    }
}

namespace BfsMemoryScheduler{
    int n,m;
    vector<vector<pii>>e,g;
    vector<int>ord;
    vector<int>in,vis;

    void init(){
        n=tmpn;m=tmpm;e=te;g=tg;in=tin;
        vis.resize(n,0);
    }
    void topsort(){
        queue<int>q;
        for(int i=0;i<n;++i){
            if(!in[i]){
                q.push(i); 
            }
        }
        while(!q.empty()){
            int x=q.front();
            q.pop();

            ord.push_back(x);
            for(auto [v,w]:e[x]){
                --in[v];
                if(!in[v]){
                    q.push(v);
                }
            }
        }
    }
    vector<int> solve(){
        init();
        topsort();
        return ord;
    }
}

namespace GraphAnalysisScheduler{
    /*
    当图越稀疏时，效果越好
    */
    
    const int N=2e5+10,M=5e5+10,inf=0x3f3f3f3f;
    int n,m;
    vector<int> outval,indeg,outdeg,vis,remout,lasdeg;
    vector<pair<int,int>>train;
    vector<int>okval,firstch;
    vector<vector<pii> > e,g;
    vector<int>vec;
    vector<int>ord,ordid; // ord is the idfs order, ord[ordid[i]] = i
    queue<int>qu;
    vector<set<pii>> suf_multi_in; // the outdegree from 多入度全部来自同一个多出度节点，先走这些路，每个pair存(point,nxt) 
    vector<set<pii>> pre_multi_out; // the indegree from which muilt-outdegree node, (pre, nxt)
    vector<map<int,int>> pre_multi_in_id; // where are the muilti in from, and the ID(x)
    vector<int>ordres;
    

    
    struct node{
        int deg,val,fr;
        node(int deg=0,int val=0,int fr=0):deg(deg),val(val),fr(fr){}
        const bool operator<(const node&y) const{
            /*smaller degree , bigger outval  takes priority*/
            if(deg==y.deg){
                if(val==y.val) return fr<y.fr;
                return val>y.val;
            }
            return deg<y.deg;
        }
        const bool operator==(const node&y) const{
            return deg==y.deg && val==y.val && fr==y.fr;
        }
    };
    vector<multiset<node>> st; // st is a (+x-y) greedy
    
    
    struct myq{
	int c;
	myq(int c=0):c(c){}
	const bool operator ==(const myq&y) const{
		return c==y.c;
	}

    // can be replace
	const bool operator <(const myq&y) const{
		auto tx=*st[c].begin();
		auto ty=*st[y.c].begin();

            if(tx.deg<=1 && ty.deg<=1){

                if(okval[c]!=okval[y.c]){
                    return okval[c]>okval[y.c];
                }
                return c<y.c;
            }
            if(tx.deg<=1 || ty.deg<=1){
                return tx.deg<ty.deg;
            }
            
            if(firstch[c]!=firstch[y.c]){
                return firstch[c]>firstch[y.c];
            }
            //return c<y.c;
            
            if(tx.deg==ty.deg){
                if(remout[tx.fr]!=remout[ty.fr]) return remout[tx.fr]>remout[ty.fr];
                if(firstch[c]!=firstch[y.c]){
                    return firstch[c]>firstch[y.c];
                }
                if(train[c].second!=train[y.c].second){
                    return train[c].second>train[y.c].second;
                }
                else if(train[c].first!=train[y.c].first){
                    return train[c].first<train[y.c].first;
                }
                
                if(tx.val==ty.val){
                    if(okval[c]!=okval[y.c]){
                        return okval[c]>okval[y.c];
                    }
                    if(remout[c]==remout[y.c]){
                        if(outval[c]==outval[y.c]){
                            return ordid[c]>ordid[y.c];
                        }
                        return outval[c]<outval[y.c];
                    }
                    return remout[c]>remout[y.c];
                }
                return tx.val>ty.val;
            }
            return tx.deg<ty.deg;
        }
    };
    set<myq> q;

    void dfs(int x,int preval,int pre,int nxt,int mxval=0,int nowval=0,int ismulti=0){
        //cout<<x<<' '<<pre<<" "<<nxt<<endl;
        if(indeg[x]>1 && pre!=-1){
            if(!ismulti){
                suf_multi_in[pre].insert(make_pair(x,nxt));
                pre_multi_out[x].insert(make_pair(pre,nxt));
            }
            else{
                pre_multi_out[x].insert(make_pair(pre,nxt));
            }
            /* exist a single chain */
            //cout<<x<<" "<<pre<<" "<<nxt<<" "<<mxval<<" "<<nowval<<endl;
            if(mxval!=0){
                train[nxt]=make_pair(mxval,mxval-nowval);
            }
        }
        if(mxval!=0 && train[x].second==1e9){
            train[x]=make_pair(mxval,mxval-nowval);
        }
        if(vis[x]) return;
        vis[x]=1;
        
        for(auto [v,w]:e[x]){
            if(outdeg[x]>1){
                dfs(v,0,x,v,0,0,0);
            }
            else if(indeg[x]==0){
                dfs(v,0,x,x,0,0,0);
            }
            else{
                if(indeg[x]<=1){
                    dfs(v,w,pre,nxt,max(mxval,nowval+w),nowval+w-preval,ismulti);
                }
                else{
                    dfs(v,0,x,x,0,0,1);
                }
                
            }
        }
    }

    void init(){
        n=tmpn;m=tmpm;e=te;g=tg;
        outval.resize(n,0);vis.resize(n,0);
        indeg.resize(n,0);outdeg.resize(n,0);remout.resize(n);
        train.resize(n);
        okval.resize(n,0);firstch.resize(n,0);
        lasdeg.resize(n,0);
        st.resize(n);

        for(int i=0;i<n;++i) st[i].clear();
        for(int i=0;i<n;++i){
            for(auto [v,w]:e[i]){
                outval[i]+=w;
                indeg[v]++;
            }
            outdeg[i]=lasdeg[i]=e[i].size();
            for(auto [v,w]:e[i]){
                st[v].insert(node(outdeg[i],outval[i],i));
                if(outdeg[i]==1){
                    okval[v]+=outval[i];
                }
            }
        }
        for(int i=0;i<n;++i){
            vis[i]=0;
            train[i]=make_pair(0,1e9);
        }

        suf_multi_in.resize(n+1);
        pre_multi_out.resize(n+1);
        for(int i=0;i<n;++i){
            if(!indeg[i]){
                st[i].insert(node(outdeg[i],outval[i],n));
                dfs(i,0,-1,-1);
            }
        }

        /*calc branch value*/
        for(int i=0,tid=0;i<n;++i){
            vis[i]=0;
            if(outdeg[i]<=1) continue;
            suf_multi_in[i].insert(make_pair(inf,0));
            int las=-1,tim=0;
            vector<int>tmp;
            for(auto it:suf_multi_in[i]){
                int p=it.first,nxt=it.second;
                //cout<<"muilt:"<<i<<" "<<p<<" "<<nxt<<endl;
                if(p==las){
                    ++tim;
                   tmp.push_back(nxt);
                } 
                else{
                    if(las!=-1){
                        if(indeg[las]==tim){
                            /* all from a same muilti-out, not appear in data*/
                            for(auto v:tmp){
                                firstch[v]+=10000000;
                            }
                        }
                        else{
                            for(auto v:tmp){
                                firstch[v]+=50*tim;
                            }
                        }
                    }
                    las=p;tim=0;tmp.clear();
                    tmp.push_back(nxt);
                }
            }
        }
    }

    void solve_idfs(){
        queue<int>q;
        for(int i=0;i<n;++i){
            remout[i]=outdeg[i];
            if(!outdeg[i]){
                q.push(i);
            }
        }
        while(!q.empty()){
            int x=q.front();q.pop();
            ord.push_back(x);
            for(auto [v,w]:g[x]){
                --outdeg[v];
                if(!outdeg[v]){
                    q.push(v);
                }
            }
        }
        reverse(ord.begin(),ord.end());
        ordid.resize(n,0);
        for(int i=0;i<n;++i){
            ordid[ord[i]]=i;
            outdeg[i]=remout[i];
        }
        for(auto x:ord){
            for(auto [v,w]:e[x]){
                remout[x]+=remout[v];
                remout[x]=min(remout[x],inf);
            }
        }
    }

    vector<int> solve(){
        init();
        solve_idfs();
        for(int i=0;i<n;++i){
            //cout<<train[i].first<<" "<<train[i].second<<"--"<<firstch[i]<<endl;
            if(!indeg[i]){
                q.insert(myq(i));
            }
        }

        int now=0,ans=0,cnt=0,done=0;
        set<int>rem;

        while(!q.empty()){
            auto it=q.begin();
            int x=(*it).c;
            ordres.push_back(x);
            ++done;

            q.erase(it);
            vis[x]=1;

            ans=max(ans,now+outval[x]);
            now+=outval[x];
            ++cnt;
            rem.insert(x);

            /* check memory release */
            for(auto [v,w]:g[x]){
                int tout=outdeg[v],tval=outval[v];
                --outdeg[v];

                if(tout>=C) continue;
                /* should be modify now */
                for(auto [z,c]:e[v]){
                    if(vis[z]) continue;
                    
                    /* if in the queue */
                    bool flag=false;
                    if(q.find(myq(z))!=q.end()){
                        flag=true;
                        q.erase(myq(z));
                    }

                    /* directly release memory */
                    assert(st[z].find(node(lasdeg[v],tval,v))!=st[z].end());
                    st[z].erase(st[z].find(node(lasdeg[v],tval,v)));
                    st[z].insert(node(tout-1,tval,v));
                    
                    if(tout==2){
                        okval[z]+=tval;
                    } 
                    if(flag){
                        q.insert(myq(z));
                    }
                }
                lasdeg[v]=tout-1;

                if(tout==1){
                    --cnt;
                    now-=outval[v];
                    rem.erase(v);
                }
            }

            for(auto [v,w]:e[x]){
                --indeg[v];
                if(!indeg[v]){
                    q.insert(myq(v));
                }
                else{
                    /* for a muilt-in, if from different muilt-in has completed, increase firstch*/
                    if(indeg[v]>C) continue; // still limit

                    for(auto [pre,nxt]:pre_multi_out[v]){
                        bool flag=false;
                        if(q.find(myq(nxt))!=q.end()){
                            flag=true;
                            q.erase(myq(nxt));
                        }
                        firstch[nxt]+=UPVAL;
                        if(flag){
                            q.insert(myq(nxt));
                        }
                    }
                }
            }
        }
        return ordres;
    }
}

namespace Improve{
    int n,m;
    vector<vector<pii> > e,g;
    vector<map<int,int>>mpe;
    vector<int>st,ed,out,in,val,sa;
    vector<ll> kval;
    vector<int>ford,ordid;
    vector<int>aft;

    void init(){
        n=tmpn;m=tmpm;
        e=te;g=tg;in=tin;out=tout;val=outval;
        st.resize(n,0);ed.resize(n,0);kval.resize(n,0);
        ordid.resize(n,0);ford.resize(n,0);sa.resize(n,0);
        mpe.resize(n);aft.resize(n,0);
        for(int i=0;i<n;++i){
            aft[i]=0;
            for(auto [v,w]:e[i]) mpe[i][v]=1;
        }
    }

    void reclear(){
        out=tout;
    }

    ll sqr(ll x){
        return x*x;
    }

    /* basic */
    vector<int> tryMove(vector<int>a){
        // use order a to adapt
        //cout<<"trymove!!"<<endl;
        int tim=0;
        for(auto x:a){
            ++tim;
            st[x]=tim;
            for(auto [v,w]:g[x]){
                --out[v];
                if(!out[v]) ed[v]=tim;
            }
        }
        //cout<<"copy!!"<<endl;

        for(int i=0;i<n;++i){
            ordid[a[i]]=i;
            sa[i]=i;
            ford[i]=a[i];
            kval[i]=sqr(ed[i]-st[i]+1)*val[i];
        }
        sort(sa.begin(),sa.end(),[&](const int &x,const int &y){
            if(kval[x]==kval[y])
                return (ed[x]-st[x])>(ed[y]-st[y]);
            return kval[x]>kval[y];
        });

        int mvcnt=0,nowi=0;
        int limN=max(int(n*PERCENT),min(n,20)),limCnt=LIMITCNT;
        while(mvcnt<limCnt && nowi<limN){
            //now try move sa[nowi], x is at ordid[x], y is ford[ordid[x]+1], y is at ordid[x]+1
            int x=sa[nowi++];
            ++mvcnt;
            while(ordid[x]<n-1){
                //swap ford[ordid[x]],ford[ordid[x]+1]
                int y=ford[ordid[x]+1];
                if(mpe[x].count(y)){
                    //x y can't swap
                    break;
                }
                ++mvcnt;
                swap(ford[ordid[x]],ford[ordid[y]]);
                swap(ordid[x],ordid[y]);
            }
            //cout<<mvcnt<<" "<<nowi<<endl;
        }
        return ford;
    }

    vector<int> tryMoveTogether(vector<int>a){
        // use order a to adapt
        //cout<<"trymove!!"<<endl;
        int tim=0;
        for(auto x:a){
            ++tim;
            st[x]=tim;
            for(auto [v,w]:g[x]){
                --out[v];
                if(!out[v]) ed[v]=tim;
            }
        }
        //cout<<"copy!!"<<endl;

        for(int i=0;i<n;++i){
            ordid[a[i]]=i;
            sa[i]=i;
            ford[i]=a[i];
            kval[i]=sqr(ed[i]-st[i]+1)*val[i];
        }
        sort(sa.begin(),sa.end(),[&](const int &x,const int &y){
            if(kval[x]==kval[y])
                return (ed[x]-st[x])>(ed[y]-st[y]);
            return kval[x]>kval[y];
        });

        int mvcnt=0,nowi=0;
        int limN=max(int(n*PERCENT),min(n,20)),limCnt=LIMITCNT;
        while(mvcnt<limCnt && nowi<limN){
            //now try move sa[nowi], x is at ordid[x], y is ford[ordid[x]+1], y is at ordid[x]+1
            int x=sa[nowi++];
            ++mvcnt;
            while(ordid[x]<n-1){
                //swap ford[ordid[x]],ford[ordid[x]+1]
                int y=ford[ordid[x]+1];
                if(mpe[x].count(y)){
                    //x y can't swap
                    break;
                }
                if(aft[x]>4){
                    aft[x]=4;
                }

                bool flag=true; //can move
                for(int i=0;flag && i<aft[x];++i){
                    int z=ford[ordid[x]-i-1];
                    if(mpe[z].count(y)){
                        //z y can't swap
                        flag=0;
                        break;
                    }
                }
                if(!flag){
                    break;
                }

                mvcnt+=aft[x]+1;
                for(int i=0,beg=ordid[x];i<aft[x]+1;++i){
                    int z=ford[beg-i];
                    swap(ford[ordid[z]],ford[ordid[y]]);
                    swap(ordid[z],ordid[y]);
                    //cout<<"swap:"<<z<<" "<<y<<" "<<ordid[z]<<" "<<ordid[y]<<endl;
                }
            }
            if(ordid[x]<n-1){
                aft[ford[ordid[x]+1]]=aft[x]+1;
            }
            //cout<<mvcnt<<" "<<nowi<<endl;
        }
        //for(auto v:ford) cout<<v<<" ";
        //cout<<endl;
        return ford;
    }

    /* Treap */
    struct Treap{
        int rt,tot;
        struct Node{
            int ls,rs,fa,sz;
            int id,key,tag;
        };
        vector<Node>tr;
        vector<int>res;

        void pushup(int x){
            tr[x].sz=tr[tr[x].ls].sz+tr[tr[x].rs].sz+1;
        }
        void split(int p,int k,int &x,int &y,int fx=0,int fy=0){
            if(!p){
                x=y=0;return;
            }
            if(k<=tr[tr[p].ls].sz){
                tr[p].fa=fy;y=p;
                split(tr[p].ls,k,x,tr[p].ls,fx,p);
                pushup(p);
            }
            else{
                tr[p].fa=fx;x=p;
                split(tr[p].rs,k-tr[tr[p].ls].sz-1,tr[p].rs,y,p,fy);
                pushup(p);
            }
        }
        int merge(int x,int y){
            if(!x || !y) return x+y;
            if(tr[x].key<tr[y].key){
                tr[x].rs=merge(tr[x].rs,y);
                tr[tr[x].rs].fa=x;
                pushup(x);
                return x;
            }
            else{
                tr[y].ls=merge(x,tr[y].ls);
                tr[tr[y].ls].fa=y;
                pushup(y);
                return y;
            }
        }

        int newnode(int x){
            ++x;
            tr[x].sz=1;
            tr[x].key=rand();
            tr[x].id=x;
            tr[x].fa=tr[x].ls=tr[x].rs=0;
            return x;
        }
        void insert(int k,int val){
            int x,y;
            split(rt,k,x,y);
            rt=merge(merge(x,newnode(val)),y);
        }
        int get_rank(int x){
            ++x;
            int res=tr[tr[x].ls].sz+1;
            int t=x;
            while(t!=rt && x){
                if(x==tr[tr[x].fa].rs) res+=tr[tr[tr[x].fa].ls].sz+1;
                x=tr[x].fa;
            }
            return res;
        }
        void solve_move(int x,int y){
            // move posx to pos (mnrk-2)
            // divide [1,xpos-1] [xpos,xpos] [xpos+1,mnrk-1] [mnrk,n]
            // or reverse [xpos,mnrk-1] [xpos,mnrk-2]
            int r1,r2,r3,r4;
            split(rt,y-1,r3,r4);
            split(r3,x,r2,r3);
            split(r2,x-1,r1,r2);
            rt=merge(r1,merge(r3,merge(r2,r4)));
        }
        void dfso(int x){
            if(tr[x].ls) dfso(tr[x].ls);
            cout<<x-1<<" ";
            if(tr[x].rs) dfso(tr[x].rs);
        }
        void output(){
            dfso(rt);
            cout<<endl;
        }
        void dfs(int x){
            if(tr[x].ls) dfs(tr[x].ls);
            res.push_back(x-1);
            if(tr[x].rs) dfs(tr[x].rs);
        }
        vector<int> get_array(){
            res.clear();
            dfs(rt);
            return res;
        }
    }T;
    vector<int> ori;
    void buildTreap(vector<int>a){
        T.tr.clear();T.rt=0;
        T.tr.resize(a.size()+10);
        int c=0;
        for(auto v:a){
            T.insert(c++,v);
        }
    }

    vector<int> tryMoveByTreap(vector<int>a){
        buildTreap(a);
        int tim=0;
        for(auto x:a){
            ++tim;
            st[x]=tim;
            for(auto [v,w]:g[x]){
                --out[v];
                if(!out[v]) ed[v]=tim;
            }
        }

        for(int i=0;i<n;++i){
            ordid[a[i]]=i;
            sa[i]=i;
            kval[i]=sqr(ed[i]-st[i]+1)*val[i];
        }
        sort(sa.begin(),sa.end(),[&](const int &x,const int &y){
            if(kval[x]!=kval[y])
                return kval[x]>kval[y];
            if(ed[x]-st[x]==ed[y]-st[y]) return st[x]>st[y];
            return (ed[x]-st[x])>(ed[y]-st[y]);
        });

        int mvcnt=0,nowi=0;
        int limN=max(int(n*PERCENT),min(n,20)),limCnt=n*100;
        while(mvcnt<limCnt && nowi<limN){
            //now try move sa[nowi], x is at ordid[x], y is ford[ordid[x]+1], y is at ordid[x]+1
            int x=sa[nowi++];
            ++mvcnt;
            if(!e[x].size()) continue;
            int xpos=T.get_rank(x);
            int mnrk=infInt,id=-1;
            for(auto [v,w]:e[x]){
                int pos=T.get_rank(v);
                if(pos<mnrk){
                    mnrk=pos;id=v;
                } 
            }
            if(xpos+1==mnrk) continue;
            //mvcnt+=mnrk-xpos;
            // move x to pos (mnrk-2)
            // divide [1,xpos-1] [xpos,xpos] [xpos+1,mnrk-1] [mnrk,n]
            // or reverse [xpos,mnrk-1] [xpos,mnrk-2]
            T.solve_move(xpos,mnrk);
        }
        ford=T.get_array();
        return ford;
    }

    //here use link list to maintain
    const ll MAXNUM=0x3f3f3f3f3f3f3f3f;
    
    struct Node{
        int pre,nxt;
        ll val;
        Node(int pre=-1,int nxt=-1,ll val=MAXNUM):pre(pre),nxt(nxt),val(val){}
    };
    int hd;
    vector<Node>S;
    vector<ll>invS;

    void insertList(int x,int y,ll val){
        //insert x after y with value val
        S[x]=Node(y,S[y].nxt,val);
        S[y].nxt=x;
        invS[x]=val;
    }

    void removeList(int x){
        //remove x from list
        if(S[x].pre!=-1 && S[x].nxt!=-1){
            S[S[x].pre].nxt=S[x].nxt;
            S[S[x].nxt].pre=S[x].pre;
        }
        else if(S[x].pre==-1){
            hd=S[x].nxt;
            S[S[x].nxt].pre=-1;
        }
        else if(S[x].nxt==-1){
            S[S[x].pre].nxt=-1;
        }
         S[x]=Node(-1,-1,S[x].val);
    }

    void buildList(vector<int>a){
        invS.resize(n);S.resize(n+5);
        ll step=MAXNUM/(n+1),now=1;
        for(int i=0;i<n;++i,now+=step){
            if(i!=0){
                insertList(a[i],a[i-1],now);
            }
            else{
                hd=a[i];
                S[i]=Node(-1,-1,now);
            }
            invS[a[i]]=now;
        }
    }

    vector<int> getListArray(){
        vector<int>res;
        for(int i=0;i<n;++i) res.push_back(i);
        sort(res.begin(),res.end(),[&](const int&x,const int&y){
            return invS[x]<invS[y];
        });
        return res;
    }

    int rbcnt=0;
    void rebuildList(){
        ++rbcnt;
        totot++;
        vector<int> tmp = getListArray();
        buildList(tmp);
    }

    void solveListMove(int x,int y){
        //move x befor y
        if(S[x].nxt==y) return;

        ll xval=invS[x],yval=invS[y];
        ll pval=S[S[y].pre].val;
        ll newval=(pval+yval)/2;

        if(yval-newval<2 || newval-pval<2){
            rebuildList();
            solveListMove(x,y);
            return;
        }
        
        removeList(x);
        insertList(x,S[y].pre,newval);
        invS[x]=newval;
    }

    vector<int> tryMoveByList(vector<int>a){
        rbcnt=0;
        buildList(a);
        int tim=0;
        for(auto x:a){
            ++tim;
            st[x]=tim;
            for(auto [v,w]:g[x]){
                --out[v];
                if(!out[v]) ed[v]=tim;
            }
        }
        for(int i=0;i<n;++i){
            ordid[a[i]]=i;
            sa[i]=i;
            kval[i]=sqr(ed[i]-st[i]+1)*val[i];
        }
        sort(sa.begin(),sa.end(),[&](const int &x,const int &y){
            if(kval[x]!=kval[y]) return kval[x]>kval[y];
            if(ed[x]-st[x]==ed[y]-st[y]) return st[x]>st[y];
            return (ed[x]-st[x])>(ed[y]-st[y]);
        });

        int mvcnt=0,nowi=0;
        int limN=max(int(n*PERCENT),min(n,20)),limCnt=n*100;
        while(mvcnt<limCnt && nowi<limN){
            //now try move sa[nowi], x is at ordid[x], y is ford[ordid[x]+1], y is at ordid[x]+1
            int x=sa[nowi++];
            ++mvcnt;
            if(!e[x].size()) continue;

            ll mnnum=inf;int mnid=-1;
            for(auto [v,w]:e[x]){
                if(invS[v]<mnnum) {
                    mnnum=invS[v];
                    mnid=v;
                }
            }
            solveListMove(x,mnid);
        }
        ford=getListArray();
        //cout<<"=================="<<endl;
        //for(int i=0;i<n;++i) cout<<ford[i]<<" ";
        //cout<<"\n==================="<<endl;
        //cout<<"\n[rebuild:"<<rbcnt<<"]\n";
        return ford;
    }

    vector<int> solve(vector<int>a,int tim=IROUND){
        init();
        bool flag=false;
        int nowCost=infInt,bestCost=Topsort::getCost(a);
        vector<int>res,best=a;
        totot = 0;
        int t=0;
        while(tim--){
            ++t;
            // choose method here
            if(IIAFLAG == 0){
                res=tryMove(a);
            }
            else if(IIAFLAG == 1){
                res=tryMoveByList(a);
            } 
            else{
                res=tryMoveByTreap(a);
            }
            nowCost=Topsort::getCost(res);
            if(nowCost<bestCost){
                best=res;
                bestCost=nowCost;
            }
            a=res;
            reclear();
            if(t<=7){
                avgrb[t].push_back(totot);
                totot = 0;
            }
            //cout<<"best: "<<bestCost<<" "<<"now: "<<nowCost<<endl;
        }
        return best;
    }

    vector<int> solveTogether(vector<int>a,int tim=IROUND){
        init();

        bool flag=false;
        int nowCost=infInt,bestCost=Topsort::getCost(a);
        vector<int>res,best=a;
        while(tim--){
            res=tryMoveTogether(a);
            nowCost=Topsort::getCost(res);
            if(nowCost<bestCost){
                best=res;
            }
            a=res;
            reclear();
            //cout<<nowCost<<"-------"<<bestCost<<endl;
        }
        return best;
    }
};

void getAns(){
    int st,ed;
    ll ans;
    vector<int>ord;

    cout<<endl;

    st=clock();
    ord=PostOrderMemoryScheduler::solve();
    ed=clock();
    ans=Topsort::getCost(ord);
    cout<<"PostOrderMemoryScheduler  "<<"Cost:"<<ans<<"  Time:"<<ed-st<<endl;

    st=clock();
    ord=Improve::solve(ord,IROUND);
    ed=clock();
    ans=Topsort::getCost(ord);
    cout<<"Improve  "<<"Cost:"<<ans<<"  Time:"<<ed-st<<endl<<endl;

    st=clock();
    ord=DFSMemoryScheduler::solve();
    ed=clock();
    ans=Topsort::getCost(ord);
    cout<<"DFSMemoryScheduler  "<<"Cost:"<<ans<<"  Time:"<<ed-st<<endl;

    st=clock();
    ord=Improve::solve(ord,IROUND);
    ed=clock();
    ans=Topsort::getCost(ord);
    cout<<"Improve  "<<"Cost:"<<ans<<"  Time:"<<ed-st<<endl<<endl;

    st=clock();
    ord=ListMemoryScheduler::solve();
    ed=clock();
    ans=Topsort::getCost(ord);
    cout<<"ListMemoryScheduler  "<<"Cost:"<<ans<<"  Time:"<<ed-st<<endl;

    st=clock();
    ord=Improve::solve(ord,IROUND);
    ed=clock();
    ans=Topsort::getCost(ord);
    cout<<"Improve  "<<"Cost:"<<ans<<"  Time:"<<ed-st<<endl<<endl;

    st=clock();
    ord=BfsMemoryScheduler::solve();
    ed=clock();
    ans=Topsort::getCost(ord);
    cout<<"BfsMemoryScheduler  "<<"Cost:"<<ans<<"  Time:"<<ed-st<<endl;

    st=clock();
    ord=Improve::solve(ord,IROUND);
    ed=clock();
    ans=Topsort::getCost(ord);
    cout<<"Improve  "<<"Cost:"<<ans<<"  Time:"<<ed-st<<endl<<endl;

    st=clock();
    ord=GraphAnalysisScheduler::solve();
    ed=clock();
    ans=Topsort::getCost(ord);
    cout<<"GraphAnalysisScheduler  "<<"Cost:"<<ans<<"  Time:"<<ed-st<<endl;

    st=clock();
    ord=Improve::solve(ord,IROUND);
    ed=clock();
    ans=Topsort::getCost(ord);
    cout<<"Improve  "<<"Cost:"<<ans<<"  Time:"<<ed-st<<endl<<endl;

    cout<<"\nAVG REBUILD:\n";
    for(int i=1;i<=7;++i){
        for(auto v:avgrb[i]){
            cout<<v<<" ";
        }
        cout<<endl;
    }
}   

int main() {
    srand(time(0));
    Input::getInput(1);
    getAns();
    return 0;
}