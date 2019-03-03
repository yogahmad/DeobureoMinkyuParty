#include <bits/stdc++.h>
using namespace std;

using lint = long long;
using pi = pair<lint, lint>;


namespace FuckFuck{
	static char buf[1<<19];
	static int idx = 0;
	static int bytes = 0;
	static inline int _read(){
		if(!bytes || idx == bytes){
			bytes = (int)fread(buf, sizeof(buf[0]), sizeof(buf), stdin);
			idx = 0;
		}
		return buf[idx++];
	}
	static inline int _readInt(){
		int x = 0, s = 1;
		int c = _read();
		while(c <= 32) c = _read();
		if(c == '-') s = -1, c = _read();
		while(c > 32) x = 10 * x + (c - '0'), c = _read();
		if(s < 0) x = -x;
		return x;
	}
}

lint gcd(lint x, lint y){ return y ? gcd(y, x%y) : x; }

const int N = 1100005;

using pii = pair<int, int>;

class SuffixArray {
public:
	int A[7 * N / 10], B[7 * N / 10], cnt[N + 2], SAV[N];
	int mem[5 * N]; int* mem_pt = mem;
	void clear(int n){
		int *ptr = mem;
		while(ptr != mem_pt){
			*ptr = 0;
			ptr++;
		}
		mem_pt = mem;
		for(int i=0; i<n+10 && i < 7 * N / 10; i++) A[i] = B[i] = 0;
		for(int i=0; i<n+2; i++) cnt[i] = 0;
		for(int i=0; i<n; i++) SAV[i] = 0;
	}
	inline int* mloc(size_t sz) {
		int* ret = mem_pt; mem_pt = mem_pt + sz;
		return ret;
	}
	void rsort(int* a, int* b, int* dat, int n, int k) { 
		for (int i = 0; i <= k; i++) cnt[i] = 0;
		for (int i = 0; i < n; i++) SAV[i] = dat[a[i]], cnt[SAV[i]]++;
		for (int i = 1; i <= k; i++) cnt[i] += cnt[i - 1];
		for (int i = n - 1; i >= 0; i--) b[--cnt[SAV[i]]] = a[i];
	}

#define I(x) ((x)%3==1)?((x)/3):((x)/3+num1)
#define I2(x) (x<num1)?(3*x+1):(3*(x-num1)+2)
	static int cmp(int x, int y, int str[], int A[], int num1) {
		if (x % 3 == 1) {
			if (y % 3 == 2) return A[I(x)] < A[I(y)];
			else return str[x] < str[y] || str[x] == str[y] && A[I(x + 1)] < A[I(y + 1)];
		}
		else {
			return str[x] < str[y] || str[x] == str[y] && cmp(x + 1, y + 1, str, A, num1);
		}
	}
	void make(int* str, int* sa, int n, int k) {
		if (n == 0) return;
		int num1 = (n + 2) / 3, num2 = n / 3;
		int num = num1 + num2;
		str[n] = str[n + 1] = str[n + 2] = 0;
		int *nsa = mloc(num), *nstr = mloc(num + 3);

		for (int i = 0, j = 0; i < n; i++) if (i % 3) A[j++] = i;
		if (n % 3 == 1) A[num - 1] = n;
		rsort(A, B, str + 2, num, k); rsort(B, A, str + 1, num, k); rsort(A, B, str, num, k);

		int cnt = 1;
		nstr[I(B[0])] = 1;
		for (int i = 1; i < num; i++) {
			int c = B[i], p = B[i - 1];
			if (str[p] != str[c] || str[p + 1] != str[c + 1] || str[p + 2] != str[c + 2]) cnt++;
			nstr[I(c)] = cnt;
		}
		if (cnt == num) for (int i = 0; i < num; i++) nsa[nstr[i] - 1] = i;
		else make(nstr, nsa, num, cnt);
		
		for (int i = 0, j = 0; i < num; i++) if (nsa[i] < num1) A[j++] = 3 * nsa[i];
		rsort(A, B, str, num1, k);
		for (int i = 0; i < num; i++) A[nsa[i]] = i, nsa[i] = I2(nsa[i]);
		A[num] = -1;
		merge(B, B + num1, nsa + (n % 3 == 1), nsa + num, sa, [&](int x, int y) {
			return cmp(x, y, str, A, num1); 
		});
		return;
	}
}sa;
void sfxarray(int n, int *str, int *sfx, int *rev, int *lcp) {
	sa.make(str, sfx, n, 128);
	sa.clear(n);
	for(int i=0; i<n; i++) rev[sfx[i]] = i;
	int h = 0;
	for(int i=0; i<n; i++){
		if(rev[i]){
			int prv = sfx[rev[i] - 1];
			while(str[prv + h] == str[i + h]) h++;
			lcp[rev[i]] = h;
		}
		if(h) h--;
	}
}

const int FUCK = 12005000;

int n;
int a[int(1.1e6)];
int sfx[int(1.1e6)];
int rev[int(1.1e6)];
int lcp[int(1.1e6)];
pii stk[int(1.1e6)];
int sz;

lint freq[FUCK];
void solve() {
	n = FuckFuck::_readInt();
	for(int i = 0; i < n; i++) {
		a[i] = 1 + FuckFuck::_readInt();
	}
	a[n] = 0;
	sfxarray(n, a, sfx, rev, lcp);
	lint sum = lint(n) * (n+1) / 2;
	sz = 0;
	lcp[n] = 0;
	for(int i = 1; i <= n; i++) {
		if(n <= 4) cout << lcp[i] << " ";
		int j = i;
		while(sz > 0 && stk[sz-1].first >= lcp[i]){
			if(stk[sz-1].first > lcp[i]){
				freq[i - stk[sz-1].second + 1] += 1;
				sum -= lint(i - stk[sz-1].second + 1);
			}
			j = stk[--sz].second;
		}
		if(i < n && lcp[i] > 0)
			stk[sz++] = pii(lcp[i], j);
	}
	freq[1] = sum;
	if(n <= 4){
		for(int i=1; i<=n; i++) printf("%lld ", freq[i]);
		puts("");
	}
	lint ans = 0;
	lint nanuggy = 1ll * n * (n + 1) / 2;
	int curmax = n;
	for(int i=1; i<=curmax; i++){
		if(freq[i] == 0) continue;
		if(freq[i] >= 2){
			freq[2*i] += freq[i] / 2;
			ans += (freq[i] / 2) * 2 * i;
			curmax = max(curmax, 2 * i);
			freq[i] %= 2;
		}
		if(freq[i] == 1){
			int e = i + 1;
			while(e <= curmax && freq[e] == 0) e++;
			if(e > curmax) break;
			ans += i + e;
			freq[i]--;
			freq[e]--;
			freq[i + e] ++;
			curmax = max(curmax, i + e);
		}
		if(curmax > max(n * 4, 696969)) break;
	}
	priority_queue<pi, vector<pi>, greater<pi>> pq;
	for(int i=1; i<=curmax; i++){
		if(freq[i]) pq.push(pi(i, freq[i]));
		freq[i] = 0;
	}
	while(!pq.empty()){
		auto x = pq.top(); pq.pop();
		if(x.second >= 2){
			ans += (x.second / 2) * 2 * x.first;
			pq.push(pi(x.first * 2, x.second / 2));
			x.second %= 2;
			if(x.second) pq.push(pi(x.first, 1));
			continue;
		}
		if(pq.empty()) break;
		auto y = pq.top(); pq.pop();
		ans += x.first + y.first;
		pq.push(pi(x.first + y.first, 1));
		y.second--;
		if(y.second) pq.push(y);
	}
	lint g = gcd(ans, nanuggy);
	ans /= g;
	nanuggy /= g;
	if(nanuggy > 1) printf("%lld/%lld\n", ans, nanuggy);
	else printf("%lld\n", ans);

}

int main() {
	int T = FuckFuck::_readInt(); 
	while(T--) solve();
	return 0;
}