#include <iostream>
#include <vector>
#include <cstring>
#include <csignal>
#include <chrono>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

struct lottery{
  string drawer_file;
  int drawer_num;
  string winner;
  vector<string> v;
};

struct lottery *l;

void readFile(struct lottery *l){
  FILE *fp;
  int line = 0;
  char buffer[100];
  
  fp = fopen(l->drawer_file.c_str(), "r");
  if(fp == NULL){
    perror("fopen()");
    exit(-1);
  }

  fseek(fp, 0 ,SEEK_SET);

  while(fgets(buffer, sizeof(buffer), fp) != NULL){
    l->v.push_back(buffer);
    //cout << buffer << endl;
  }

  fclose(fp);
  l->drawer_num = l->v.size();
  //cout << "There are " << l->drawer_num << " drawers" << endl;
}

void winner_result(string winner){
  cout << endl;
  cout << "☆*:.｡.o(≧▽≦)o.｡.:*☆" << endl;
  cout << endl;
  cout << "The winner is: " << winner << endl;
}

unsigned long long get_sys_random_seed(){
  unsigned long long seed;
  int fd = open("/dev/random", O_RDONLY);
  if (fd == -1) {
      perror("open()");
      exit(1);
  }

  int n = read(fd, (void*)&seed, sizeof(seed));
  if (n == -1) {
      perror("read()");
      exit(1);
  }
  
  close(fd);
  //cout << "random seed: " << seed << endl;
  return seed;
}

int get_lucky_num(void)
{
  int random_num;

  /**
  srand(time(0));
  **/

  /**
  struct timeval time_now {};
  gettimeofday(&time_now, nullptr);
  time_t msecs_time = (time_now.tv_sec * 1000) + (time_now.tv_usec / 1000);
  srand(msecs_time);
  **/

  unsigned long long seed = get_sys_random_seed();
  srand(seed);
  random_num = rand() % l->drawer_num;

  return random_num;
}

void lottery_drawing(struct lottery *l){

  int random_num = get_lucky_num();
  l->winner = l->v.at(random_num);
  winner_result(l->v.at(random_num));
}


void test_distribution(){
  int arr[l->drawer_num];
  int n = 0;
  for(int i = 0; i < l->drawer_num; i++){
    arr[i] = 0;
  }

  for(int i = 0; i < 500; i++){
    n = get_lucky_num();
    arr[n] += 1;
  }

  for(int i = 0; i < l->drawer_num; i++){
    cout << arr[i] << endl;
  }
}


void signalHandler(int signum)
{
  cout << "Pick the winner !!!" << endl;
  winner_result(l->winner);
  exit(signum);  
}

void lottery_drawing_2(struct lottery *l){
  
  int i = 0;
  while(true) {
    l->winner = l->v[i];
    cout << l->v[i];
    i++;
    if(i == l->drawer_num){
      i = 0;
    }
  }
}

void lottery_help(){
  cout << "Usage: lottery [options] filename [lottery method]random|pick" << endl;
  cout << "Lottery method default is random, but your choose pick/Ctrl-c pick the winner" << endl;
  cout << "Options:" << endl;
  cout << "--h --help Display this information." << endl;
}

int main(int argc, char **argv){

  l = (struct lottery*)malloc(sizeof(struct lottery));
  if(l == NULL){
    perror("malloc()");
    return -1;
  }

  if(argc < 2){
    cout << "Params are missing!" << endl;
    lottery_help();
    return -1;
  }
 
  l->drawer_file = argv[1];

  if(strcmp(argv[1], "--h") == 0 || strcmp(argv[1], "--help") == 0){
    lottery_help();
  }else if(argc > 2 && strcmp(argv[2], "test") == 0){
    readFile(l);
    test_distribution();
  }else if(argc > 2 && strcmp(argv[2], "pick") == 0){
    signal(SIGINT, signalHandler);
    readFile(l);
    lottery_drawing_2(l);
  }else{
    readFile(l);
    lottery_drawing(l);
  }

  free(l);
  
}


