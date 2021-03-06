#include<bits/stdc++.h>

using namespace std;

template <typename T1, typename T2>
struct less_second {
    typedef pair<T1, T2> type;
    bool operator ()(type const& a, type const& b) const {
        return a.second > b.second;
    }
};

class Graph 
{ 
    int V; 
    map<int, list<int>> adj; 

public:

    int getV();
    void setV(int);
    void addEdge(int, int);  

    double clusteringCoeff(int);

    double averageClusteringCoeff();

    map<pair<int, int>, double> findSimilarityValues(unordered_set<int> &, double, double);

    bool isEdge(int, int);
}; 
  
void Graph::setV(int V) 
{ 
    this->V = V; 
}

int Graph::getV(){
    return V;
}
  
void Graph::addEdge(int v, int w) 
{ 
    adj[v].push_back(w);
    // adj[w].push_back(v);
}

bool Graph::isEdge(int x, int y){
    return find(adj[x].begin(), adj[x].end(), y) != adj[x].end();
}

double Graph::clusteringCoeff(int v){

    set<pair<int, int>> clusters;

    list<int>::iterator i;

    for(i = adj[v].begin(); i != adj[v].end(); ++i){
        list<int>::iterator j;

        for(j = adj[*i].begin(); j != adj[*i].end(); ++j){
            if(find(adj[v].begin(), adj[v].end(), *j) != adj[v].end()){
                int a[2] = {*i,*j};
                sort(a,a+2);

                clusters.insert(make_pair(a[0],a[1]));
            }
        }
    }

    if((adj[v].size())*(adj[v].size()-1) == 0)
        return 0;

    return (double)clusters.size()/((adj[v].size())*(adj[v].size()-1));
}

double Graph::averageClusteringCoeff(){
    double totalClusteringCoeff = 0;

    map<int, list<int>>::iterator itr;

    for (itr = adj.begin(); itr != adj.end(); ++itr) { 
         totalClusteringCoeff += clusteringCoeff(itr->first);
    }

    return totalClusteringCoeff/V;
}

map<pair<int, int>, double> Graph:: findSimilarityValues(unordered_set<int> &vertices, double C, double beta){
    map<pair<int, int>, double> similarityValues;

    unordered_set<int>::iterator itrF, itrS;

    for(itrF = vertices.begin(); itrF != vertices.end(); ++itrF){
        for(itrS = vertices.begin(); itrS != vertices.end(); ++itrS){
            if(*itrF == *itrS)
                continue;

            int x = *itrF, y = *itrS;
            if(find(adj[x].begin(), adj[x].end(), *itrS) == adj[x].end()){

                similarityValues[make_pair(x,y)] = 0;

                set<int> z;

                list<int>::iterator itr;

                for(itr = adj[x].begin(); itr != adj[x].end(); ++itr){
                    if(find(adj[y].begin(), adj[y].end(), *itr) != adj[y].end() || find(adj[*itr].begin(), adj[*itr].end(), y) != adj[*itr].end())
                        z.insert(*itr);
                }

                set<int>::iterator i;

                for(i = z.begin(); i != z.end(); ++i){
                    int tao = adj[*i].size();
                    
                    int cz = 0;

                    for(itr = adj[*i].begin(); itr != adj[*i].end(); ++itr){
                        if(find(z.begin(), z.end(), *itr) != z.end())
                            cz++;
                    }

                    if(cz == 0 || tao == 0)
                        continue;
                    
                    similarityValues[make_pair(x,y)] += (double)abs(cz) * pow((double)abs(tao), beta*C);
                }
            }
        }
    }

    return similarityValues;
}

set<pair<int, int>> predictLinks(string file, double beta, int test_size){
    string line, val;

    Graph *g = new Graph();
    unordered_set<int> vertices;

    int FIRST_LINE = 1, EDGES = 1;

    ifstream f(file);

    while (getline(f,line)) {
        if(FIRST_LINE){
            stringstream s(line);
            int text[2],i=0;

            while (getline (s, val, ' ')){
                try{      
                    text[i++] = stoi(val);
                }
                catch(exception e){}
            }

            g->setV(text[1]);

            FIRST_LINE = 0;
            continue;
        }

        if(line != "*edges" && EDGES == 1)
            continue;
        else if(EDGES == 1){
            EDGES = 0;
            continue;
        }

        stringstream s(line);

        int edge[3],i=0;  

        while (getline(s, val, ' ')){      
            edge[i++] = stoi(val);
        }
        vertices.insert({edge[0],edge[1]});
        g->addEdge(edge[0],edge[1]);
    }

    double averageClusteringCoeff = g->averageClusteringCoeff();

    averageClusteringCoeff = 0.49;

    cout << "<C>: " << averageClusteringCoeff << "\n";

    map<pair<int, int>, double> similarityValues = g->findSimilarityValues(vertices, averageClusteringCoeff, beta);

    map<pair<int, int>, double>::iterator itr;

    vector<pair<pair<int,int>, double>> sortedSimilarityValues;

    set<pair<int, int>> listed;

    for(itr = similarityValues.begin(); itr != similarityValues.end(); ++itr){
        sortedSimilarityValues.push_back(make_pair(itr->first, itr->second));
    }

    sort(sortedSimilarityValues.begin(), sortedSimilarityValues.end(), less_second<pair<int, int>, double>());

    set<pair<int, int>> predictedLinks;

    for(int i=0; i<sortedSimilarityValues.size() && test_size; i++){
        if(find(listed.begin(), listed.end(), sortedSimilarityValues[i].first) != listed.end()){
            continue;
        }

        listed.insert(sortedSimilarityValues[i].first);
        listed.insert(make_pair(sortedSimilarityValues[i].first.second, sortedSimilarityValues[i].first.first));

        predictedLinks.insert(sortedSimilarityValues[i].first);

        test_size--;
    }

    return predictedLinks;
}