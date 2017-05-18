/* ***********************************************************************

  > File Name: str_test.cc
  > Author: Aut
  > Mail: aut@taomee.com 
  > Created Time: 2017年05月18日 星期四 17时39分50秒

 ********************************************************************** */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

bool strLess(const string str1, const string str2)
{
    if (str1.size() == str2.size())
        return str1 < str2;
    return str1.size() < str2.size();
}

int main(int argc, const char* argv[])
{
    int n;
    string input;
    vector<string> strSet;

    cin >> n;
    for (int i = 0; i < n; ++i) {
        cin >> input;
        strSet.push_back(input);
    }
    sort(strSet.begin(), strSet.end(), strLess);
    for (auto it : strSet)
        cout << it << endl;

    return 0;
}
