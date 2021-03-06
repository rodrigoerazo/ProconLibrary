/**
 * @file
 * ## 最大流(Dinitz)
 *
 * 多くの文献で"Dinic"として紹介されているが，原論文を英訳するときに著者名がそう訳されたためそちらが広まっただけで，その後の自著では"Dinitz"と表記されている．
 *
 * ネットワークをBFSで層別ネットワークに分け，その中での極大フローを探し，増加させることを繰り返し最大流量を求める．
 *
 * ### 計算量
 *
 *   * O(V^2 E)
 *
 * ### ソースコード
 *
 * @include dinitz.cpp
 *
 * ### 確認済み問題
 *
 *   * AOJ 2304
 *
 * ### 参考
 *
 *   * グラフ・ネットワーク・組合せ論
 */
Weight blockingFlow(Graph &g, vector<int> &l, int v, int t, Weight f){
    if(v == t) return f;
    EACH(i, g[v]){
        if(i -> weight > 0 && l[v] < l[i -> to]){
            Weight d = blockingFlow(g, l, i -> to, t, min(f, i -> weight));
            if(d > 0){
                i -> weight -= d;
                g[i -> to][i -> rev].weight += d;
                return d;
            }
        }
    }
    return 0;
}
/**
 *Ford-Fulkersonと同様にネットワークとソース，シンクの頂点を渡す．グラフを構築するときに逆辺を付け忘れないように．
 */
Weight dinitz(Graph &g, int s, int t){
    int n = g.size();
    Weight flow = 0;
    vector<int> l(n, -1);
    for(;;){
        queue<int> Q;
        fill(ALL(l), -1);
        l[s] = 0;
        Q.push(s);
        while(!Q.empty()){
            int v = Q.front(); Q.pop();
            EACH(i, g[v])if(i -> weight > 0 && l[i -> to] < 0){
                l[i -> to] = l[v] + 1;
                Q.push(i -> to);
            }
        }
        if(l[t] < 0) return flow;
        Weight f;
        while((f = blockingFlow(g, l, s, t, INF)) > 0) flow += f;
    }
}
