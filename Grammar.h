#include  <math.h>
#include  <iostream>
#include  <limits>
#include <cstring>
#include <string>
#include <float.h>
#include <set>
#include <curses.h>
#include <vector>
#include <map>
#include <istream>
#include <stack>
#include <algorithm>

using namespace std;

class Grammar {
    map<int, pair<pair<char,string>,pair<int,string>>> productions;

    string resultStr="";
    string nonterminals = "QSABC";
    string terminals = "abc$";
    map<pair<char,char>,vector<int>>table;
    vector<string> helpLogOut;
    map<string,pair<string,int>> product_action;
    long ampunt_money;
    void setProd(){
        productions[1]= make_pair(make_pair('Q', "S$"),make_pair(1,"+1"));
        productions[2]= make_pair(make_pair('S', "AaS"),make_pair(2,"+2"));
        productions[3]= make_pair(make_pair('S', "b"),make_pair(3,"-1"));
        productions[4]= make_pair(make_pair('A', "CAb"),make_pair(4,"-2"));
        productions[5]= make_pair(make_pair('A', "B"),make_pair(5,"*2"));
        productions[6]= make_pair(make_pair('B', "cSA"),make_pair(6,"/2"));
        productions[7]= make_pair(make_pair('B', ""),make_pair(7,"+3"));
        productions[8]= make_pair(make_pair('C', "c"),make_pair(8,"*(-1)"));
        productions[9]= make_pair(make_pair('C', "ab"),make_pair(9,"*5"));
    }
    void setTable(){
        table[make_pair('Q','a')]={1};
        table[make_pair('Q','b')]={1};
        table[make_pair('Q','c')]={1};

        table[make_pair('S','a')]={2};
        table[make_pair('S','b')]={3};
        table[make_pair('S','c')]={2};

        table[make_pair('A','a')]={4,5};
        table[make_pair('A','b')]={5};
        table[make_pair('A','c')]={4,5};

        table[make_pair('B','a')]={7};
        table[make_pair('B','b')]={7};
        table[make_pair('B','c')]={6};

        table[make_pair('C','a')]={9};
        table[make_pair('C','c')]={8};
    }

public:
    Grammar(){
        setProd();
        setTable();
        srand((unsigned int)time(NULL));
    }

    string  generateRandom(){
        stack<char> st;
        resultStr.clear();
        while (!st.empty()) {
            st.pop();
        }
        st.push('Q');
        while(!st.empty()){
            char simbol = st.top();
            st.pop();

            if(nonterminals.find(simbol) != string::npos){
                int numberProd;
                do{
                    numberProd = rand()%9+1;
                }
                while (productions[numberProd].first.first!=simbol);
                string forAddToStack = productions[numberProd].first.second;
                reverse(forAddToStack.begin(),forAddToStack.end());
                for(auto e: forAddToStack){
                    st.push(e);
                }
            }
            else if(terminals.find(simbol) != string::npos){
                resultStr.push_back(simbol);
            }
            else{
                cout<<"  Generation error"<<endl;
            }
        }
        if(resultStr == "b$" && rand()%2==0){
            resultStr = generateRandom();
        }
        if(resultStr.size()>100){
            resultStr = generateRandom();
        }
        return resultStr;
    }


    int calculateMoney(int currentMoney, int actionNumber){
        switch (actionNumber) {
            case 1:
                currentMoney+=1;
                break;
            case 2:
                currentMoney+=2;
                break;
            case 3:
                currentMoney-=1;
                break;
            case 4:
                currentMoney-=2;
                break;
            case 5:
                currentMoney*=2;
                break;
            case 6:
                currentMoney/=2;
                break;
            case 7:
                currentMoney+=3;
                break;
            case 8:
                currentMoney*(-1);
                break;
            case 9:
                currentMoney*=5;
                break;
            default:
                break;
        }
        return currentMoney;
    }



    bool RecCheck(stack<char>st,string strIn,vector<string> logOut, long localAmountMoney, string pushBackingString=""){
        localAmountMoney = localAmountMoney %400'000'000;
        if(pushBackingString !="")
            logOut.push_back(pushBackingString);

        if(st.empty() && strIn.empty()){
            helpLogOut = logOut;
            ampunt_money=localAmountMoney;
            return true;
        }
        if(st.empty() || strIn.empty()){
            return false;
        }

        char topSt=st.top();
        st.pop();
        if(terminals.find(topSt) != string::npos) {                  // на вершине стека терминал
            if (topSt == strIn[0]) {
                strIn.erase(0, 1);
                return RecCheck(st, strIn, logOut, localAmountMoney);
            }

            if (topSt != strIn[0]) {
                return false;
            }
        }
        else if(nonterminals.find(topSt) != string::npos) {              // на вершине стека нетерминал
            vector<int> productionsVector = table[make_pair(topSt,strIn[0])];
            for(int numberProduction:productionsVector){
                stack<char> helpSt = st;
                pair<char,string> productionPair;
                productionPair = productions[numberProduction].first;
                string action =productions[numberProduction].second.second;
                int actionNumber = productions[numberProduction].second.first;
                string nextPushBackingString = (string(1, productionPair.first) == "Q"? "S'":string(1, productionPair.first))
                        +" -> "+
                        (productionPair.second != "" ? productionPair.second : "empty")
                        + "      ( "+action +" )";
                localAmountMoney = calculateMoney(localAmountMoney,actionNumber);
                reverse(productionPair.second.begin(),productionPair.second.end());
                for(auto e: productionPair.second){
                    helpSt.push(e);
                }
                if(RecCheck(helpSt,strIn,logOut,localAmountMoney,nextPushBackingString) == true){
                    return true;
                }

            }

        }
        return false;
    }

    bool GlobalCheck(string strIn){
        ampunt_money=0;
        long localAmountMoney=0;
        stack<char>st;
        st.push('Q');
        string strOut="";
        vector<string> logOutput;
        helpLogOut.clear();
        bool isCorrect = RecCheck(st,strIn,logOutput,localAmountMoney);
        if(isCorrect){
            cout<<"Строка соответсвует грамматике. Вывод строки:"<<endl;
            for(auto e: helpLogOut){
                cout<< e<<endl;
            }
            cout<<"Ваш выйгрыш: "<<ampunt_money;
            cout<< endl<<endl;

            return true;
        }
        else{
            cout<<"Строка не соотвествует грамматике."<<endl<<endl<<endl;
            return false;
        }
    }
};



