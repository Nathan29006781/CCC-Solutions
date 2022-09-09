#include <iostream>
#include <cmath>
#include <unordered_map>
#include <tuple>

//Uncommented
//Counts violations twice for symmetry. Can be simplified for efficiency

//1 is good, -1 is bad, 0, is none
std::unordered_map<std::string, std::unordered_map<std::string, int> > students;
std::unordered_map<std::string, std::pair<std::string, std::string> > groups;

void save(std::string student1, std::string student2, int value){
  students[student1][student2] = value;
  students[student2][student1] = value;
}

int main(){
  int X, Y, G;
  std::string student1, student2;
  
  std::cin >> X;
  for (int i = 0; i < X; i++){
    std::cin >> student1 >> student2;
    save(student1, student2, 1);
  }

  std::cin >> Y;
  for (int i = 0; i < Y; i++){
    std::cin >> student1 >> student2;
    save(student1, student2, -1);
  }

  std::cin >> G;
  int violated = 0;
  std::string student3;
  for (int i = 0; i < G; i++){
    std::cin >> student1 >> student2 >> student3;

    groups[student1] = std::make_pair(student2, student3);
    groups[student2] = std::make_pair(student1, student3);
    groups[student3] = std::make_pair(student1, student2);
  }

  bool grouped;
  for (std::unordered_map<std::string, std::unordered_map<std::string, int> >::iterator it = students.begin(); it != students.end(); it++){
    for (std::unordered_map<std::string, int>::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++){
      grouped = groups[it->first].first == it2->first || groups[it->first].second == it2->first;
      if((it2->second == 1 && grouped == 0) || (it2->second == -1 && grouped == 1)) violated++;
    }
  }

  std::cout << violated/2 << std::endl;
  return 0;
}