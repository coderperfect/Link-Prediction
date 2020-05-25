#include "lpred.cpp"
#include "divide.cpp"

int main(){
    double precision = 0;
    double AUC = 0;

    for(int i=0; i<10; i++){
        string net = "BUP";
        
        divide(net);

        string train = net + "_train.net";

        train = "datasets/BUP_full.net";

        string test = net + "_test.net";

        set<pair<int, int>> predictedLinks;

        int test_size = 0;

        string line, val;

        Graph *g = new Graph();
        unordered_set<int> vertices;

        int FIRST_LINE = 1, EDGES = 1;

        ifstream f;
        f.open(test);

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
            test_size++;
        }

        predictedLinks = predictLinks(train, -1.84, test_size);

        set<pair<int, int>>::iterator itr;

        int correct_predictions = 0;

        for(itr = predictedLinks.begin(); itr != predictedLinks.end(); ++itr){
            if(g->isEdge(itr->first, itr->second) || g->isEdge(itr->second, itr->first))
                correct_predictions++;
        }

        precision += (double)correct_predictions/test_size;

        double accuracy=(correct_predictions+tn)/(correct_predictions+tn+fp+fn);

        f.close();

        del(net);
    }

    cout << "Precision: " << precision/10 << "\n";

    return 0;
}