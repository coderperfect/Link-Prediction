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

    return (double)clusters.size()/(adj[v].size())*(adj[v].size()-1);
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

int main(){
    string line, val;

    Graph *g = new Graph();
    Graph *gTraining = new Graph();

    unordered_set<int> vertices;
    unordered_set<int> TrainingVertices;
    
    int no_of_edges=0;

    int FIRST_LINE = 1, EDGES = 1;

    ifstream f("datasets/BUP_train_0.net");

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
        no_of_edges++;
        vertices.insert({edge[0],edge[1]});
        g->addEdge(edge[0],edge[1]);
    }
    
    int size=vertices.size();

    int TEST_SIZE = no_of_edges/10;
    int TestArray[TEST_SIZE][2];
    int TrainingArray[no_of_edges-TEST_SIZE][2];

    int Random[TEST_SIZE];

    int i=0;
    srand(time(0));
    while(i<TEST_SIZE){
        int r=rand()%no_of_edges;
        int flag=0;
        for(int j=0;j<i;j++){
            if(r==Random[j]){
                flag=1;
            }
        }
        if(flag==0){
            Random[i]=r;
            i++;
        }
    }

    sort(Random,Random+TEST_SIZE);

    int j=0,k=0;
    
    ifstream fin;
    fin.open("datasets/BUP_train_0.net");
    FIRST_LINE = 1;
    EDGES = 1;

//    cout<<"Training Set"<<endl;

    while (getline(fin,line)) {
        if(FIRST_LINE){
            stringstream s(line);
            int text[2];
            i=0;

            while (getline (s, val, ' ')){
                try{      
                    text[i++] = stoi(val);
                }
                catch(exception e){}
            }

            gTraining->setV(text[1]);

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

        int edge[3];
        i=0;  

        while (getline(s, val, ' ')){      
            edge[i++] = stoi(val);
        }
        if(Random[k]==j){
            TestArray[k][0]=edge[0];
            TestArray[k][1]=edge[1];
//            cout<<edge[0]<<"->"<<edge[1]<<endl;
            k++;
            j++;
            continue;
        }
        TrainingVertices.insert({edge[0],edge[1]});
        gTraining->addEdge(edge[0],edge[1]);
        TrainingArray[j-k][0]=edge[0];
        TrainingArray[j-k][1]=edge[1];
        j++;
    }
    
    fin.close();

    double averageClusteringCoeff = gTraining->averageClusteringCoeff();

//    cout << gTraining->averageClusteringCoeff();

    double beta = -1.84;                   //Set beta value here

    map<pair<int, int>, double> similarityValues = gTraining->findSimilarityValues(TrainingVertices, averageClusteringCoeff, beta);

    map<pair<int, int>, double>::iterator itr;

    vector<pair<pair<int,int>, double>> sortedSimilarityValues;

    set<pair<int, int>> listed;

    for(itr = similarityValues.begin(); itr != similarityValues.end(); ++itr){
        sortedSimilarityValues.push_back(make_pair(itr->first, itr->second));
    }

    sort(sortedSimilarityValues.begin(), sortedSimilarityValues.end(), less_second<pair<int, int>, double>());

    cout << "\n\n\nTest size = "<<TEST_SIZE<<"\n";

    cout << "Predicted links are:\n--------------------------\n\n";

    double tp=0,fp=0;
    k=0;
    for(int i=0; i<sortedSimilarityValues.size() && k<TEST_SIZE; i++){ 
        int flag=0;
        for(int j=0;j<no_of_edges-TEST_SIZE;j++){
            if(TrainingArray[j][0]==sortedSimilarityValues[i].first.first && TrainingArray[j][1]==sortedSimilarityValues[i].first.second){
                flag=1;
                break;
            }
            else if(TrainingArray[j][1]==sortedSimilarityValues[i].first.first && TrainingArray[j][0]==sortedSimilarityValues[i].first.second){
                flag=1;
                break;
            }
        }
        if(flag==1){
            continue;
        }
        for(int j=0;j<TEST_SIZE;j++){
            if(TestArray[j][0]==sortedSimilarityValues[i].first.first && TestArray[j][1]==sortedSimilarityValues[i].first.second){
                cout<<sortedSimilarityValues[i].first.first<<"->"<<sortedSimilarityValues[i].first.second<<endl;
                tp++;
            }
            if(TestArray[j][1]==sortedSimilarityValues[i].first.first && TestArray[j][0]==sortedSimilarityValues[i].first.second){
                cout<<sortedSimilarityValues[i].first.second<<"->"<<sortedSimilarityValues[i].first.first<<endl;
                tp++;
            }
        }
        k++;
    }

    fp=TEST_SIZE-tp;
    double fn=fp;
    double tn=((size-1)*size)/2-no_of_edges-tp-fp-fn;
    cout <<"\n\n\n";
    cout <<"TP:"<<tp<<"\nTN:"<<tn<<"\nFP:"<<fp<<"\nFN:"<<fn<<endl;
    double accuracy=(tp+tn)/(tp+tn+fp+fn);
    double precision=tp/(tp+fp);
    cout<<"Accuracy: "<<accuracy<<"\nPrecision:"<<precision<<endl;

    return 0;
}