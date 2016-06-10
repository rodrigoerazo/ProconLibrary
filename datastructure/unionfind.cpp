struct UnionFind{
	vector<int> parent, rank;
	UnionFind(int n){ parent = vector<int>(n, -1); rank = vector<int>(n, 0); }
	int find(int x){
		if(parent[x] == -1) return x;
		else return (parent[x] = find(parent[x]));
	}
	bool unite(int x, int y){
		x = find(x);
		y = find(y);
		if(x == y) return false;
		if(rank[x] < rank[y])
			parent[x] = y;
		else
			parent[y] = x;
		if(rank[x] == rank[y])
		++rank[x];
		return true;
	}
	bool same(int x, int y){
		return find(x) == find(y);
	}
};