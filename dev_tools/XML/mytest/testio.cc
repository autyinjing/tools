/* ***********************************************************************

  > File Name: testio.cc
  > Author: Aut
  > Mail: aut@taomee.com 
  > Created Time: 2017年03月23日 星期四 16时29分59秒

 ********************************************************************** */

#include <iostream>
#include <sstream>

int main()
{
    std::cout << "The number 42 in octal:   " << std::oct << 42 << '\n'
        << "The number 42 in decimal: " << std::dec << 42 << '\n'
        << "The number 42 in hex:     " << std::hex << 42 << '\n';
    int n;
    std::istringstream("1219") >> std::dec >> n;
    std::cout << std::dec << "Parsing \"2A\" as hex gives " << n << '\n';

    return 0;
}
