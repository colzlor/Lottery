#include <iostream>
#include <vector>
#include <cstring>
#include <csignal>
#include <chrono>
#include <sys/time.h>

using namespace std;

struct lottery{
  string drawer_file;//抽奖人员名单文件
  int drawer_num;//抽奖人员数量
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
    perror("文件不存在或打开文件发生错误!");
    exit(-1);
  }

  fseek(fp, 0 ,SEEK_SET);

  while(fgets(buffer, sizeof(buffer), fp) != NULL){
    l->v.push_back(string(buffer));
  }

  l->drawer_num = l->v.size();
  cout << "目前一共" << l->drawer_num << "位抽奖者" << endl;
}

void winner_result(string winner){
  cout << endl;
  cout << "☆*:.｡.o(≧▽≦)o.｡.:*☆" << endl;
  cout << endl;
  cout << "中奖者是: " << winner << endl;
  cout << "让我们恭喜 " << winner << endl;
}

void lottery_drawing(struct lottery *l){
  
  struct timeval time_now {};
  gettimeofday(&time_now, nullptr);
  time_t msecs_time = (time_now.tv_sec * 1000) + (time_now.tv_usec / 1000);
  srand(msecs_time);
  //srand(time(0));
  int random_num = rand() % l->drawer_num;
  l->winner = l->v.at(random_num);
  winner_result(l->v.at(random_num));
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
  cout << "Lottery method default is random, but your choose pick/Ctrl-c pick the winner";
  cout << "Options:" << endl;
  cout << "--h --help Display this information." << endl;
}

int main(int argc, char **argv){

  l = (struct lottery*)malloc(sizeof(struct lottery));
  if(l == NULL){
    perror("内存分配失败!");
    return -1;
  }

  if(argc < 2){
    perror("参数不足!");
    lottery_help();
    return -1;
  }


  if(strcmp(argv[1], "--h") == 0 || strcmp(argv[1], "--help") == 0){
    lottery_help();
  }else if(argc == 3 && strcmp(argv[2], "pick") == 0){
    signal(SIGINT, signalHandler);
    string filename = argv[1];
    l->drawer_file = filename;
    readFile(l);
    lottery_drawing_2(l);
  }else{
    string filename = argv[1];
    l->drawer_file = filename;
    readFile(l);
    lottery_drawing(l);
  }
  
}


