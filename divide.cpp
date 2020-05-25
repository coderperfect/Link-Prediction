#include<bits/stdc++.h>
using namespace std;

void divide(string net){
    string line, val;

    int FIRST_LINE = 1, EDGES = 1;

    ifstream f("datasets/" + net + "_full.net");

    ofstream test(net + ("_test.net"), ios::app);
    ofstream train(net + ("_train.net"), ios::app);

    int edge_count = 0;

    while (getline(f,line)) {
        if(FIRST_LINE){
            stringstream s(line);
            int text[2],i=0;

            while (getline (s, val, ' ')){
                try{      
                    text[i++] = stoi(val);
                }
                catch(exception e){
                }
            }

            test << "*vertices ";
            train << "*vertices ";

            test << text[1] << endl;
            train << text[1] << endl;

            FIRST_LINE = 0;
            continue;
        }

        if(line != "*edges" && EDGES == 1)
            continue;
        else if(EDGES == 1){
            EDGES = 0;
            continue;
        }

        edge_count++;
    }

    ifstream f1("datasets/" + net + "_full.net");

    FIRST_LINE = 1; EDGES = 1;

    int random, test_size=floor(edge_count*0.1);

    test << "*edges" << endl;
    train << "*edges" << endl;

    while (getline(f1,line)) {
        if(FIRST_LINE){
            stringstream s(line);
            int text[2],i=0;

            while (getline (s, val, ' ')){
                try{      
                    text[i++] = stoi(val);
                }
                catch(exception e){}
            }

            FIRST_LINE = 0;
            continue;
        }

        if(line != "*edges" && EDGES == 1)
            continue;
        else if(EDGES == 1){
            EDGES = 0;
            continue;
        }

        if(edge_count > test_size && test_size){
            random = rand()%2;
        }
        else if(test_size){
            random = 1;
        }
        else{
            random = 0;
        }
        

        stringstream s(line);

        int edge[3],i=0;  

        while (getline(s, val, ' ')){      
            edge[i++] = stoi(val);
        }

        if(random){
            test << edge[0] << " " << edge[1] << " " << edge[2] << endl;
            test_size--;
        }else{
            train << edge[0] << " " << edge[1] << " " << edge[2] << endl;
        }

        edge_count--;
    }
}

void del(string net){
    while(remove((net + "_test.net").c_str()) != 0);
    while(remove((net + "_train.net").c_str()) != 0);
}