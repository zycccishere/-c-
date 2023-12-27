#include <vector>
#include <iostream>
#include <string>
#include <cstring>
#include <algorithm>
#include <stack>
#include <queue>
#include <cmath>
#include <stdlib.h>

// -> 蕴含
// | 合取
// & 析取
// ! 非
// ( 左括号
// ) 右括号
// <-> shuang
using namespace std;

struct pairvector{
    vector<string> lhs;
    vector<string> rhs;
};

queue <pairvector> q;
vector <int> ind;

bool simple(vector<string> a){
    if(a.size()==0) return true;
    bool sim=true;
    for(int i=0;i<a.size();i++){
        if(a[i].size()!=1){
            sim=false;
            break;
        }
    }
    return sim;
}
int test(pairvector a){
    bool left_simple=simple(a.lhs);
    bool right_simple=simple(a.rhs);
    if(!left_simple||!right_simple) return 0;
    for(int i=0;i<a.lhs.size();i++){
        string tmp=a.lhs[i];
        for(int j=0;j<a.rhs.size();j++){
            if(a.rhs[j]==tmp) return 1;
        }
    }
    return -1;
}//1:win
//0:continue
//-1:lose

bool is_letter(char a){
    if(a-'A'>=0&&a-'Z'<=0) return true;
    return false;
}

void g(vector<string> &a){
    if(a.size()<=1) return;
    string store;
    for(int i=0;i<a.size();i++){
        store=a[i];
        for(int j=i+1;j<a.size();j++){
            if(a[j]==store){
                ind.push_back(j);
                break;
            }
        }
    }
    sort(ind.begin(), ind.end());
    for(int i=int(ind.size()-1);i>=0;i--){
        a.erase(a.begin()+ind[i]);
    }
}

int findit(string str,int start){//从左括号下标开始
    int cnt=1;
    for(int i=start+1;i<str.size();i++){
        if(str[i]=='(') cnt++;
        if(str[i]==')') cnt--;
        if(cnt==0) return i;
    }
    return -1;
    
}

string cut(string str,int begin,int end){//闭区间
    return str.substr(begin,end-begin+1);
    
}

void gather(pairvector &a){
    g(a.lhs);
    g(a.rhs);
}


string cleanspace(string a){
    string ret="";
    for(auto i:a){
        if(i!=' ') ret+=i;
    }
    return ret;
}


int portion(string str){
    int cnt=0,index=0;
    for(int i=0;i<str.size();i++){
        if(str[i]=='(') cnt++;
        if(str[i]==')') cnt--;
        if((str[i]=='&'||str[i]=='|'||str[i]=='-'||str[i]=='<')&&cnt==0){
            index=i;
            break;
        }
        
    }
    return index;
}
void scan(queue<pairvector> &a){
    if(a.size()==0){
        cout<<"correct!"<<endl;
        exit(0);
    }
    pairvector now1;
    now1.lhs=a.front().lhs;
    now1.rhs=a.front().rhs;
    a.pop();
    int t=test(now1);
    if(t==-1){
        cout<<"wrong!"<<endl;
        exit(0);
    }
    if(t==1) {
        scan(a);
    }else{
        //t==0 continue
        if(!simple(now1.rhs)){
            int bindex=0;
            while(bindex<now1.rhs.size()) {
                if(now1.rhs[bindex].size()!=1) break;
                bindex++;
            }
            string str=now1.rhs[bindex];
            now1.rhs.erase(now1.rhs.begin()+bindex);
            if(str[0]=='('&&findit(str,0)==str.size()-1){
                str=cut(str,1,int(str.size())-2);
                now1.rhs.push_back(str);
                a.push(now1);
            }else{
                int linkindex=portion(str);
                if(linkindex==0){
                    now1.lhs.push_back(cut(str,1,int(str.size())-1));
                    a.push(now1);
                }else{
                    char c=str[linkindex];
                    if(c=='|'){
                        string str1=cut(str,0,linkindex-1);
                        string str2=cut(str,linkindex+1,int(str.size())-1);
                        now1.rhs.push_back(str1);
                        now1.rhs.push_back(str2);
                        a.push(now1);
                    }
                    else if(c=='&'){
                        pairvector now2;
                        now2.lhs=now1.lhs;
                        now2.rhs=now1.rhs;
                        string str1=cut(str,0,linkindex-1);
                        string str2=cut(str,linkindex+1,int(str.size())-1);
                        now1.rhs.push_back(str1);
                        now2.rhs.push_back(str2);
                        a.push(now1);
                        a.push(now2);
                    }
                    else if(c=='-'){
                        string str1=cut(str,0,linkindex-1);
                        string str2=cut(str,linkindex+2,int(str.size())-1);
                        now1.lhs.push_back(str1);
                        now1.rhs.push_back(str2);
                        a.push(now1);
                    }
                    else if(c=='<'){
                        pairvector now2;
                        now2.lhs=now1.lhs;
                        now2.rhs=now1.rhs;
                        string str1=cut(str,0,linkindex-1);
                        string str2=cut(str,linkindex+3,int(str.size())-1);
                        now1.lhs.push_back(str1);
                        now1.rhs.push_back(str2);
                        now2.rhs.push_back(str1);
                        now2.lhs.push_back(str2);
                        a.push(now1);
                        a.push(now2);
                    }
                    else{
                        cout<<"fatal error!"<<endl;
                        exit(0);
                    }
                    
                }
                
            }
        }
        else{//lhs
            int bindex=0;
            while(bindex<now1.lhs.size()) {
                if(now1.lhs[bindex].size()!=1) break;
                bindex++;
            }
            string str=now1.lhs[bindex];
            now1.lhs.erase(now1.lhs.begin()+bindex);
            if(str[0]=='('&&findit(str,0)==str.size()-1){
                str=cut(str,1,int(str.size())-2);
                now1.lhs.push_back(str);
                a.push(now1);
            }else{
                int linkindex=portion(str);
                if(linkindex==0){
                    now1.rhs.push_back(cut(str,1,int(str.size())-1));
                    a.push(now1);
                }else{
                    char c=str[linkindex];
                    if(c=='&'){
                        string str1=cut(str,0,linkindex-1);
                        string str2=cut(str,linkindex+1,int(str.size())-1);
                        now1.lhs.push_back(str1);
                        now1.lhs.push_back(str2);
                        a.push(now1);
                    }
                    else if(c=='|'){
                        pairvector now2;
                        now2.lhs=now1.lhs;
                        now2.rhs=now1.rhs;
                        string str1=cut(str,0,linkindex-1);
                        string str2=cut(str,linkindex+1,int(str.size())-1);
                        now1.lhs.push_back(str1);
                        now2.lhs.push_back(str2);
                        a.push(now1);
                        a.push(now2);
                    }
                    else if(c=='-'){
                        pairvector now2;
                        now2.lhs=now1.lhs;
                        now2.rhs=now1.rhs;
                        string str1=cut(str,0,linkindex-1);
                        string str2=cut(str,linkindex+2,int(str.size())-1);
                        now1.lhs.push_back(str2);
                        now2.rhs.push_back(str1);
                        a.push(now1);
                        a.push(now2);
                    }
                    else if(c=='<'){
                        pairvector now2;
                        now2.lhs=now1.lhs;
                        now2.rhs=now1.rhs;
                        string str1=cut(str,0,linkindex-1);
                        string str2=cut(str,linkindex+3,int(str.size())-1);
                        now1.lhs.push_back(str1);
                        now1.lhs.push_back(str2);
                        now2.rhs.push_back(str1);
                        now2.rhs.push_back(str2);
                        a.push(now1);
                        a.push(now2);
                    }
                    else{
                        cout<<"fatal error!"<<endl;
                        exit(0);
                    }
                    
                }
                
            }
        }
        scan(a);
    }
}

int main(int argc, const char * argv[]) {
    string str;
    getline(cin, str);
    str=cleanspace(str);
    pairvector start;
    start.rhs.push_back(str);
    gather(start);
    q.push(start);
    scan(q);
    return 0;
}
