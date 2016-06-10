/*
 * 必要であれば，自動生成に対応
 * 中身を書き換えていたからできない気もする
 */
#include <bits/stdc++.h>
using namespace std; 

#define ALL(x) (x).begin(),(x).end() 

typedef long long ll;
const double eps = 1e-10;

template<typename T> 
struct LasySegmentTree{
	struct node_t{
		T min, sum, add;
		int pos;
	};
	int n;
	vector<node_t> dat;
	inline void eval(int k, int l, int r){
		if(dat[k].add == 0) return;
		if(r - l > 1){
			dat[k*2+1].add += dat[k].add;
			dat[k*2+2].add += dat[k].add;
		}
		dat[k].min += dat[k].add;
		dat[k].sum += dat[k].add * (r - l);
		dat[k].add = 0;
	}
	inline void merge(int k){
		int lch = k*2+1, rch = k*2+2;
		T lm = dat[lch].min, rm = dat[rch].min;
		dat[k].min = min(lm, rm);
		dat[k].pos = lm <= rm ? dat[lch].pos: dat[rch].pos;
		dat[k].sum = dat[lch].sum + dat[rch].sum;
	}
	void init(vector<T> &v){
		for(int i = 0; i < v.size(); ++i){
			dat[i+n-1].min = dat[i+n-1].sum = v[i];
			dat[i+n-1].pos = i;
		}
		for(int k = n - 2; k >= 0; --k)
			merge(k);
	}
	LasySegmentTree(int n_){
		for(n = 1; n < n_; n *= 2);
		dat = vector<node_t>(n * 2 - 1);
	}
	T get(int a, int b, int &pos, int k, int l, int r){
		eval(k, l, r);
		if(b <= l || r <= a)
			return numeric_limits<T>::max();
		else if(a <= l && r <= b){
			pos = dat[k].pos;
			return dat[k].min;
		} else{
			int m = (l+r)/2, lp = -1, rp = -1;
			T lm = get(a, b, lp, k*2+1, l, m);
			T rm = get(a, b, rp, k*2+2, m, r);
			merge(k);
			pos = lm <= rm ? lp: rp;
			return min(lm, rm);
		}
	}
	T get(int a, int b, int &p){
		p = -1;
		return get(a, b, p, 0, 0, n);
	}
	T get(int a, int b){ int p; return get(a, b, p); }
	T sum(int a, int b, int k, int l, int r){
		eval(k, l, r);
		if(b <= l || r <= a)
			return 0;
		else if(a <= l && r <= b)
			return dat[k].sum;
		else{
			T lv = sum(a, b, k * 2 + 1, l, (l + r) / 2);
			T rv = sum(a, b, k * 2 + 2, (l + r) / 2, r);
			merge(k);
			return lv + rv;
		}
	}
	T sum(int a, int b){ return sum(a, b, 0, 0, n); }
	void add(int a, int b, T d, int k, int l, int r){
		eval(k, l, r);
		if(b <= l || r <= a)
			return;
		if(a <= l && r <= b){
			dat[k].add += d;
			eval(k, l, r);
			return;
		}
		int m = (l + r) / 2;
		add(a, b, d, k*2+1, l, m);
		add(a, b, d, k*2+2, m, r);
		merge(k);
	}
	T add(int a, int b, T d){ add(a, b, d, 0, 0, n); }
};

int N, Q;
string s;
int main(){
	cin >> N >> Q >> s;
	vector<int> v(N), q(Q);
	set<int> cl;
	for(int i = 0; i < Q; ++i){
		cin >> q[i];
	}
	for(int i = 0; i < N; ++i){
		v[i] = s[i] == '(' ? 1: -1;
		if(v[i] < 0) cl.insert(i);
		if(i > 0) v[i] += v[i-1];
	}
	LasySegmentTree<int> st(N);
	st.init(v);
	for(int I = 0; I < Q; ++I){
		int l = q[I]-1;
		if(s[l] == '('){
			s[l] = ')';
			cl.insert(l);
			st.add(l, N, -2);
			l = *(cl.begin());
			cl.erase(l);
			s[l] = '(';
			st.add(l, N, +2);
		}
		else{
			s[l] = '(';
			cl.erase(l);
			st.add(l, N, +2);
			l = -1;
			int r = N-1;
			while(r - l > 1){
				int c =  (l + r) / 2;
				if(st.get(c, r) >= 2)
					r = c;
				else
					l = c;
			}
			l = r;
			cl.insert(l);
			s[l] = ')';
			st.add(l, N, -2);
		}
		cout << l+1 << endl;
	}
	return 0;
}