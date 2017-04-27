/* ***********************************************************************

  > File Name: sing_test.cc
  > Author: Aut
  > Mail: aut@taomee.com 
  > Created Time: 2017年04月27日 星期四 16时41分10秒

 ********************************************************************** */

#include <iostream>
#include "../c++/singleton.hpp"

class Test : public Singleton<Test>
{
public:
    Test() { std::cout << "Test()" << std::endl; }
    ~Test() { std::cout << "~Test()" << std::endl; }
};

int main(int argc, const char* argv[])
{
    Test::get_instance();
    Test::del_instance();

    return 0;
}

