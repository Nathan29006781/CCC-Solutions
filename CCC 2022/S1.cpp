#include <iostream>
#include <cmath>

//Uncommented
//Has been cleaned since contest (does not reflect submitted code)

int main(){
  int num;
  std::cin >> num;

  int counter = 0;
  int fours = num/4+1;
  while(fours-->0) counter += (num - 4*fours) % 5 ? 0 : 1;

  std::cout << counter << std::endl;
  return 0;
}