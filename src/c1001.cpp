#include <string>
#include <iostream>
#include <vector>
using namespace std;

typedef vector<int> int_vector;


string tostring(int_vector iv)
{
    string val;
    for(int_vector::const_iterator iter = iv.begin();
                iter != iv.end(); ++iter)
    {
        val.push_back((char)(*iter + '0'));
    }
    return val;
}

int_vector tovector(const string& vv)
{
    string v = vv;
    int_vector iv;
    for(string::const_iterator iter = v.begin();
        iter != v.end(); ++iter)
    {
        iv.push_back(*iter - '0');
    }
    return iv;
}

string trimstr_left(string s)
{
    string::iterator iter = s.begin();
    while(iter != s.end())
    {
        if(*iter == '0')
        {
            s.erase(iter);
            iter = s.begin();
        }
        else
        {
            break;
        }
    }
    return s;
}

string trimstr_right(string ss)
{
    string s = ss;
    do
    {
        int idx = s.size() - 1;
        if(idx < 0)
        {
            break;
        }
        char v = s[idx];
        if(v != '0')
        {
            break;
        }
        string::iterator iter = s.begin() + idx;
        s.erase(iter);
    }
    while(true);
    return s;
}

class Number
{
public:
    int_vector data;
    int        count;

    void eval(const string& ss)
    {
        string s = trimstr_left(ss);
        string left;
        string right;
        bool find_dot = false;
        for(string::const_iterator iter = s.begin(); iter != s.end(); ++iter)
        {
            if(*iter == '.')
            {
                find_dot = true;
                continue;
            }
            if(find_dot)
            {
                right.push_back(*iter);
            }
            else
            {
                left.push_back(*iter);
            }
        }

        if(!find_dot)
        {
            int_vector iv = tovector(left);
            data.swap(iv);
            return;
        }

        // has dot, trim left & trim right
        left = trimstr_left(left);
        right = trimstr_right(right);
        count = right.size();
        data = tovector(left + right);
    }
};
class StringNumber
{
public:
    string data;
    void eval(int_vector iv,int c)
    {
        if(c == 0)
        {
            bool bZero = true;
            for(int_vector::iterator i = iv.begin(); i != iv.end(); ++i)
            {
                if(*i != 0)
                {
                    bZero = false;
                }
                else
                {
                    if(bZero)
                    {
                        continue;
                    }
                }
                data.insert(data.begin(),1,char(*i) + '0');
            }
        }
        else
        {
            data.clear();
            int cursor = 0;
            for(int i = iv.size() - 1; i >= 0; --i)
            {
                data.push_back((char)(iv[i] + '0'));
                ++cursor;
                if(cursor == c)
                {
                    data.push_back('.');
                }
            }
            if(cursor < c)
            {
                while(cursor < c)
                {
                    data.push_back('0');
                    ++cursor;
                }
                data.push_back('.');
            }            
        }
    }
};

int_vector mul(int_vector aa,int bb,int zcount)
{
    int diff = 0;
    int b = bb;
    int_vector a(aa);
    for(int i = a.size() - 1; i >= 0; --i)
    {
        int v = a[i];
        v *= b;
        v += diff;
        diff = 0;
        if( v > 9)
        {
            diff = v / 10;
            v = v  % 10;
        }
        a[i] = v;
    }

    if(diff > 0)
    {
        a.insert(a.begin(),diff);
    }
    for(int i = 0;i < zcount; ++i)
    {
        a.push_back(0);
    }

    return a;
}

// a is hight vector
int_vector add(int_vector aa,int_vector bb)
{
    int_vector a(aa);
    int_vector b(bb);
    while(b.size() < a.size())
    {
        b.insert(b.begin(),0);
    }
    if(a.size() < b.size())
    {
        a.push_back(0);
    }
    int diff = 0;
    int_vector c;
    for(int i = 0;i < a.size(); ++i)
    {
        int idx = a.size() - i - 1;
        int v = a[idx] + b[idx] + diff;
        diff = 0;
        if(v > 9)
        {
            diff = v / 10;
            v = v % 10;
        }
        c.insert(c.begin(),v);
    }
    if(diff > 0)
    {
        c.insert(c.begin(),diff);
    }
    return c;
}

int_vector mul(int_vector aa,int_vector bb)
{
    int_vector a(aa);

    int_vector c;
    int bb_len = bb.size();
    for(int i = bb_len - 1;i >= 0; --i)
    {
        int v = bb[i];
        int z = bb_len - i - 1;
        int_vector d = mul(a,v,z);
        c = add(d,c);
    }
    return c;
}

int_vector power(int_vector iv,int n)
{
    if(n == 0)
    {
        int_vector iv;
        iv.push_back(1);
        return iv;
    }
    int_vector c(iv);
    for(int i = 0;i < n - 1; ++i)
    {
        c = mul(c,iv);
    }
    return c;
}

int main(int argc,char* argv[])
{
    string str;
    string s;
    int np;
    while(cin >> s >> np)
    {
        if(np == 0)
        {
            cout << 1 << endl;
            continue;
        }
        Number num;
        num.eval(s);
        if(num.data.size() == 0)
        {
            cout << 0 << endl;
            continue;
        }
        int n = num.count;
    
        int_vector dst = power(num.data,np);
        int pos = 0;
        for(int i = 0;i < np; ++i)
        {
            pos += n;
        }

        StringNumber snum;
        snum.eval(dst,pos);
        
        for(string::reverse_iterator i = snum.data.rbegin(); i != snum.data.rend(); ++i)
        {
            cout << *i;
        }
        cout << endl;
    }
    return 0;
}