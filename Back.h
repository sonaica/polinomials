#pragma once
#include <string>
#include <vector>

enum class States { StateH, StateA, StateB, StateC, StateD, StateE };

struct CheckReturn {
  int error_num;
  int position;
  char err;
};

struct node_pol {
  int numerator, denominator;
  std::vector<int> power;
  unsigned long long hash;
  node_pol *prev, *next;
};

struct node_list {
  node_pol *pol;
  node_list *prev, *next;
  bool color = false;
};

void CheckPolinomial(std::string input);
void Cast(node_pol *&P);
void ins(node_pol *&res, node_pol *&end, node_pol *&p);
std::pair<node_pol *, node_pol *> merge(node_pol *&L1, node_pol *&E1,
                                        node_pol *&L2, node_pol *&E2);
void merge_sort(node_pol *&L, node_pol *&E);
void CheckPolinomial(std::string input);
void DeleteFromPol(node_pol *&L, node_pol *&p, int call);
node_pol *CreatePol(std::string input);
void AddToBase(node_list *&base, node_list *&base_end, std::string input);



