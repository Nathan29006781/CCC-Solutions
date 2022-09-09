#include <iostream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <forward_list>
#include <vector>

//2022 GOOGLE HASH CODE
//This code is not perfect, it reflects my contest solution
//This code has not been cleaned since the contest date, so it may not be easy to read nor efficient at running
//This is not guaranteed to give the best solution

#define printf(...) //used to silence debugging print statements
#define FOR(n) while(n-->0)
#define loop(vec) for(decltype(vec)::iterator it = vec.begin(); it != vec.end(); it++)

typedef uint count;
typedef std::string contributor;
typedef std::string skill;
typedef std::string project;
typedef int level;

class Contributors;
class Projects;

std::vector <Contributors*> contributors;
std::forward_list <Projects*> projects;
std::unordered_set<skill> skill_list;

std::string filename = "F";
int max_days;
int started = 0;

class Contributors{
  public:
    std::string name;
    bool busy = false;
    std::unordered_map<skill, level> skills;

    void update_skill(std::pair<skill, level> skill);
    static Contributors* least(std::pair<skill, level> skill);
    static Contributors* least_with_mentor(std::pair<skill, level> skill);
    static Contributors* find_mentor(Projects* project, std::pair<skill, level> skill);
    static void input(count length);

    Contributors(std::string name);
};

class Projects{
  public:
    std::string name;
    bool completed = false;
    int days, score, best;
    int start_date = std::numeric_limits<int>::max();

    std::vector<Contributors*> contributors;
    std::vector<std::pair<skill, level> > roles;
    std::unordered_map<skill, level > roles_m;

    bool start(int day);
    bool end(int day);
    void abort();
    bool better_roles(Projects& r);
    bool operator<(Projects& r);
    static bool pcomp(Projects* l, Projects* r);
    static void input(count length);

    Projects(std::string name, int days, int score, int best);
};

Contributors::Contributors(std::string name):
name(name){
  contributors.push_back(this);
};

Projects::Projects(std::string name, int days, int score, int best):
name(name), days(days), score(score), best(best){
  projects.push_front(this);
};

void Contributors::input(count length){
  std::string name, skill;
  int level;
  count skills;
  FOR(length){
    std::cin >> name >> skills;
    Contributors* contributor = new Contributors(name);

    FOR(skills){
      std::cin >> skill >> level;
      skill_list.insert(skill);
      contributor->skills[skill] = level;
    }
  }
}

void Projects::input(count length){
  std::string name, skill;
  int days, score, best, level;
  count roles;

  FOR(length){
    std::cin >> name >> days >> score >> best >> roles;
    Projects* project = new Projects(name, days, score, best);
    max_days = std::max(max_days, best);

    FOR(roles){
      std::cin >> skill >> level;
      skill_list.insert(skill);
      project->roles_m[skill] = level;
      project->roles.push_back(std::make_pair(skill, level));
    }
  }
}

bool Projects::pcomp(Projects* l, Projects* r){
  return *l < *r;
}

bool Projects::operator < (Projects& r){
  if (this->better_roles(r) == 0){
    if (this->days == r.days){
      if (this->best == r.best){
        if (this->score == r.score){
          return this->roles.size() < r.roles.size();
        }
        return this->score > r.score;
      }
      return this->best > r.best;
    }
    return this->days < r.days;
  }
  return this->better_roles(r);
}

Contributors* Contributors::least(std::pair<skill, level> skill){
  loop(contributors){
    if(!(*it)->busy && (*it)->skills[skill.first] >= skill.second) return *it;
  }

  return nullptr;
}

Contributors* Contributors::least_with_mentor(std::pair<skill, level> skill){
  loop(contributors){
    if(!(*it)->busy && (*it)->skills[skill.first] == skill.second - 1){
      printf("Possibly %s with Mentor\n", (*it)->name.c_str());
    }
  }


  return nullptr;
}

Contributors* Contributors::find_mentor(Projects* project, std::pair<skill, level> skill){
  loop(project->contributors){
    if((*it)->skills[skill.first] >= skill.second) return *it;
  }

  return nullptr;
}

void Contributors::update_skill(std::pair<skill, level> skill){
  if(skills[skill.first] <= skill.second) skills[skill.first] = skill.second+1;
}

void Projects::abort(){
  loop(contributors) (*it)->busy = false;
  contributors.clear();
  start_date = std::numeric_limits<int>::max();
}

bool Projects::better_roles(Projects& r){
  int better = 0;
  skill skill;
  loop(skill_list){
    skill = *it;
    if(this->roles_m[skill] && r.roles_m[skill]) better += r.roles_m[skill]-this->roles_m[skill];
  }
  return better > 0;
}

std::ofstream outfile(filename + "Out.txt");

bool Projects::start(int day){
  if(start_date == std::numeric_limits<int>::max() && !completed){
    printf("\nTrying to start %s\n", name.c_str());
    start_date = day;
    
    Contributors* contributor = nullptr;
    Contributors* mentor = nullptr;
    loop(roles){
      contributor = nullptr;
      mentor = nullptr;
      
      // mentor = Contributors::find_mentor(this, *it);
      contributor = mentor ? Contributors::least_with_mentor(*it) : Contributors::least(*it);

      if(!contributor){
        printf("Aborting %s\n", name.c_str());
        abort();
        return false;
      }

      contributors.push_back(contributor);
      contributor->busy = true;
      printf("%s for %s\n", contributor->name.c_str(), it->first.c_str());
    }

    for(int i = 0; i < contributors.size(); i++) contributors[i]->update_skill(roles[i]);
    printf("Started %s on day %d\n\n", name.c_str(), day);
    outfile << name << std::endl;
    loop(contributors) outfile << (*it)->name << ' ';
    outfile << std::endl;
    started++;
    return true;
  }
  return false;
}

bool Projects::end(int day){
  if (start_date == day - days){
    printf("Ending %s\n", name.c_str());
    abort();
    completed = true;
    projects.remove(this);
    return true;
  }
  return false;
}

int main(){
  std::ifstream in(filename + ".txt");
  std::streambuf* cinbuf = std::cin.rdbuf(in.rdbuf()); //save and redirect

  count contributor_l, project_l; 
  std::cin >> contributor_l >> project_l;
  Contributors::input(contributor_l);
  Projects::input(project_l);

  projects.sort(Projects::pcomp);

  for(int i = 0; i < max_days; i++){
    printf("\nDAY %d\n", i);
    loop(projects){
      Projects* project = *it;
      project->start(i);
      if(project->end(i)){
        i--;
        break;
      }
    }
  }

  outfile << started << std::endl;
  outfile.close();

  return 0;
}