#include <bits/stdc++.h>
using namespace std;
using lint = long long;
const int MAXN = 100005;
const int mod = 998244353;
using pi = pair<int, lint>;
const int invt = (mod + 1) / 2;

lint ipow(lint x, lint p){
	lint ret = 1, piv = x % mod;
	while(p){
		if(p & 1) ret = ret * piv % mod;
		piv = piv * piv % mod;
		p >>= 1;
	}
	return ret;
}
int n;
vector<pi> gph[MAXN];


struct offV{
	set<lint> s;
	lint offset;
	int size(){ return s.size(); }
	void insert(lint x){
		x -= offset;
		while(s.find(-x) != s.end()){
			s.erase(-x);
			x++;
		}
		s.insert(-x);
	}
}ds[MAXN];

bool cmp(int p, int q){
	if(ds[p].size() > ds[q].size()) return !cmp(q, p);
	auto itr = ds[q].s.begin();
	for(auto &i : ds[p].s){
		if(-i + ds[p].offset != -*itr + ds[q].offset){
			return -i + ds[p].offset < -*itr + ds[q].offset;
		}
		itr++;
	}
	if(ds[q].size() != ds[p].size()) return 1;
	return 0;
}

int dfs(int x){
	if(gph[x].size() == 0){
		return x;
	}
	vector<pi> loc;
	for(auto &i : gph[x]){
		loc.emplace_back(i.first, dfs(i.first));
		ds[loc.back().second].insert(i.second);
	}
	int maxpos = loc[0].second;
	for(int i=1; i<loc.size(); i++){
		if(cmp(maxpos, loc[i].second)) maxpos = loc[i].second;
	}
	ds[maxpos].offset++;
	sort(loc.begin(), loc.end(), [&](const pi &a, const pi &b){
		return ds[a.second].size() > ds[b.second].size();
	});
	for(int i=1; i<loc.size(); i++){
		for(auto &j : ds[loc[i].second].s){
			lint pos = -j + ds[loc[i].second].offset;
			ds[loc[0].second].insert(pos);
		}
	}
	return loc[0].second;
}

int main(){
	scanf("%d",&n);
	for(int i=2; i<=n; i++){
		int p; lint v;
		scanf("%d %lld",&p,&v);
		gph[p].emplace_back(i, v);
	}
	int val = dfs(1);
	lint ret = 0;
	for(auto &i : ds[val].s){
		ret += ipow(2, -i + ds[val].offset);
	}
	cout << ret % mod << endl;
}