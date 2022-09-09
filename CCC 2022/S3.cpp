#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

// #define printf(...)

//This is a brute-force solution
//It takes very long for problems with largeish numbers
//By far no where near the easiest/most efficient solution

typedef std::uint64_t Type;
typedef std::vector<Type> vec;

Type N, M, K;
vec piece;
vec bad_result = vec(0);

bool good(vec::iterator start, vec::iterator end){
  for (int i = 1; i <= M; i++){
    if (std::count(start, end, i) > 1) return false;
  }
  return true;
}

Type samples(vec piece){
  int samples = 0;
  vec sample;

  for (vec::iterator start = piece.begin(); start != piece.end(); start++){
    for (vec::iterator end = start; end != piece.end(); end++){
      sample = vec(start, end+1);
      if(good(sample.begin(), sample.end())) samples++;
    }
  }

  return samples;
}

vec permute(vec::iterator start = piece.begin()){
  if(start == piece.end()) return bad_result; //Recursion end condition
  for (int i = 1; i <= M; i++){
    *start = i;
    printf("\n%lld samples: ", samples(piece));
    for (vec::iterator it = piece.begin(); it != piece.end(); it++) printf("%lld ", *it);
    if(samples(piece) == K) return piece;

    vec result = permute(start+1);
    if (result != bad_result) return result; //End condition based on good answer
  }
  return bad_result;
}

int main(){
  std::cin >> N >> M >> K;

  if (K < N || K > N*(N+1)/2) std::cout << -1;
  else{
    piece.resize(N);
    std::fill(piece.begin(), piece.end(), 1);
    vec result = permute();

    if (result == bad_result) std::cout << -1;
    else{
      printf("\nResult of %lld %lld %lld: ", N, M, K);
      for (vec::iterator it = piece.begin(); it != piece.end(); it++) std::cout << *it << " ";
    }
  }

  std::cout << std::endl;
  return 0;
}