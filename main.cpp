//
//  main.cpp
//  王浩算法
//
//  Created by zhangyc on 2023/12/26.
//
#include <vector>
#include <iostream>
#include <string>
#include <cstring>
#include <algorithm>
#include <stack>
#include <queue>
#include <cmath>
#include <stdlib.h>

using namespace std;

struct pairvector
{
    vector<string> lhs;
    vector<string> rhs;
};

queue <pairvector> q;//Store all the status to be tested/executed

string cleanSpace(string a)
{
    //Better input
    string ret="";
    for(auto i:a)
    {
        if(i!=' ') ret+=i;
    }
    return ret;
}

string cut(string str,int begin,int end)
{
    return str.substr(begin,end-begin+1);
}

bool simplified(vector<string> a)
{
    if(a.size()==0) return true;
    bool sim=true;
    for(int i=0;i<a.size();i++)
    {
        if(a[i].size()!=1)
        {
            sim=false;
            break;
        }
    }
    return sim;
}

int test(pairvector a)
{
    bool left_simplified=simplified(a.lhs);
    bool right_simplified=simplified(a.rhs);
    if(!left_simplified||!right_simplified) return 0;
    for(int i=0;i<a.lhs.size();i++)
    {
        string tmp=a.lhs[i];
        for(int j=0;j<a.rhs.size();j++)
        {
            if(a.rhs[j]==tmp) return 1;
        }
    }
    return -1;
}//1:true and return
//0:continue
//-1:false and return

vector <int> ind;//Store the index

void partiallyGather(vector<string> &a)
{
    if(a.size()<=1) return;
    string store;
    for(int i=0;i<a.size();i++)
    {
        store=a[i];
        for(int j=i+1;j<a.size();j++)
        {
            if(a[j]==store)
            {
                ind.push_back(j);
                break;
            }
        }
    }
    sort(ind.begin(), ind.end());
    for(int i=int(ind.size()-1);i>=0;i--)
    {
        a.erase(a.begin()+ind[i]);
    }
}

void gather(pairvector &a)
{
    partiallyGather(a.lhs);
    partiallyGather(a.rhs);
}

int findTerminal(string str,int start)
{
    //Start from a left bracket
    int cnt=1;
    for(int i=start+1;i<str.size();i++)
    {
        if(str[i]=='(') cnt++;
        if(str[i]==')') cnt--;
        if(cnt==0) return i;
    }
    return -1;
}

int findLinkword(string str)
{
    //Decide where the linkword to be executed is
    int cnt=0,index=0;
    for(int i=0;i<str.size();i++)
    {
        if(str[i]=='(') cnt++;
        if(str[i]==')') cnt--;
        if((str[i]=='&'||str[i]=='|'||str[i]=='-'||str[i]=='<')&&cnt==0)
        {
            index=i;
            break;
        }
    }
    return index;
}

void scan(queue<pairvector> &a)
{
    if(a.size()==0)
    {
        cout<<"TRUE"<<endl;
        return;
    }
    pairvector now1;
    now1.lhs=a.front().lhs;
    now1.rhs=a.front().rhs;
    a.pop();
    int t=test(now1);
    if(t==-1)
    {
        cout<<"FALSE"<<endl;
        return;
    }
    if(t==1)
    {
        scan(a);
    }
    else
    {
        //t==0 continue
        if(!simplified(now1.rhs))
        {
            int bindex=0;
            while(bindex<now1.rhs.size()) {
                if(now1.rhs[bindex].size()!=1) break;
                bindex++;
            }
            string str=now1.rhs[bindex];
            now1.rhs.erase(now1.rhs.begin()+bindex);
            if(str[0]=='('&&findTerminal(str,0)==str.size()-1)
            {
                str=cut(str,1,int(str.size())-2);
                now1.rhs.push_back(str);
                a.push(now1);
            }else{
                int linkindex=findLinkword(str);
                if(linkindex==0)
                {
                    now1.lhs.push_back(cut(str,1,int(str.size())-1));
                    a.push(now1);
                }else{
                    char c=str[linkindex];
                    if(c=='|')
                    {
                        string str1=cut(str,0,linkindex-1);
                        string str2=cut(str,linkindex+1,int(str.size())-1);
                        now1.rhs.push_back(str1);
                        now1.rhs.push_back(str2);
                        a.push(now1);
                    }
                    else if(c=='&')
                    {
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
                    else if(c=='-')
                    {
                        string str1=cut(str,0,linkindex-1);
                        string str2=cut(str,linkindex+2,int(str.size())-1);
                        now1.lhs.push_back(str1);
                        now1.rhs.push_back(str2);
                        a.push(now1);
                    }
                    else if(c=='<')
                    {
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
                        cout<<"unknown error!"<<endl;
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
            if(str[0]=='('&&findTerminal(str,0)==str.size()-1)
            {
                str=cut(str,1,int(str.size())-2);
                now1.lhs.push_back(str);
                a.push(now1);
            }else{
                int linkindex=findLinkword(str);
                if(linkindex==0)
                {
                    now1.rhs.push_back(cut(str,1,int(str.size())-1));
                    a.push(now1);
                }
                else
                {
                    char c=str[linkindex];
                    if(c=='&')
                    {
                        string str1=cut(str,0,linkindex-1);
                        string str2=cut(str,linkindex+1,int(str.size())-1);
                        now1.lhs.push_back(str1);
                        now1.lhs.push_back(str2);
                        a.push(now1);
                    }
                    else if(c=='|')
                    {
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
                    else if(c=='-')
                    {
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
                    else if(c=='<')
                    {
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
                    else
                    {
                        cout<<"unknown error!"<<endl;
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
    str=cleanSpace(str);
    pairvector start;
    start.rhs.push_back(str);
    gather(start);
    q.push(start);
    scan(q);
    return 0;
}
