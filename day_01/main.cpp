/*
Fuel required to launch a given module is based on its mass. Specifically, to find the fuel required for a module, take its mass, 
divide by three, round down, and subtract 2.

For example:

    For a mass of 12, divide by 3 and round down to get 4, then subtract 2 to get 2.
    For a mass of 14, dividing by 3 and rounding down still yields 4, so the fuel required is also 2.
    For a mass of 1969, the fuel required is 654.
    For a mass of 100756, the fuel required is 33583.

The Fuel Counter-Upper needs to know the total fuel requirement. To find it, individually calculate the fuel needed for the mass of 
each module (your puzzle input), then add together all the fuel values.

What is the sum of the fuel requirements for all of the modules on your spacecraft?
*/

// clang++ -o day_01 main.cpp

#include <iostream>
#include <cstdint>

int main(int argc [[maybe_unused]], const char** argv [[maybe_unused]]) {
  // read from stdin, write to stdout

  uint64_t sum = 0;

  while(std::cin.good()) {
    uint64_t cur_module = 0;
    std::cin >> cur_module;
    if (!std::cin.good()) break;
    
    auto fuel = (cur_module / 3) - 2;
    sum += fuel;
  }
  
  std::cout << sum << std::endl;
  
  return 0;
}
