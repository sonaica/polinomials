#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

enum class States { StateH, StateA, StateB, StateC, StateD, StateE };

struct CheckReturn {
  int error_num;
  int position;
  char err;
};

struct node_pol {
  int numerator, denominator;
  std::vector<int> power;
  std::string powstr;
  node_pol *prev, *next;
};

struct node_list {
  node_pol *pol;
  node_list *prev, *next;
  bool color = false;
  std::vector<bool> var;
};

void CheckPolinomial(std::string input);
void Cast(node_pol *&P);
void ins(node_pol *&res, node_pol *&end, node_pol *&p);
std::pair<node_pol *, node_pol *> merge(node_pol *&L1, node_pol *&E1,
                                        node_pol *&L2, node_pol *&E2);
void merge_sort(node_pol *&L, node_pol *&E);
void CheckPolinomial(std::string input);
void DeleteFromPol(node_pol *&L, node_pol *&p, int call);
std::pair<node_pol *, std::vector<bool>> CreatePol(std::string input);
void AddToBase(node_list *&base, node_list *&base_end, std::string input);

bool Compare(node_pol *&first, node_pol *&second);
node_pol* Plus(node_pol *&first, node_pol *&second);
node_pol* Mult(node_pol *&first, node_pol *&second);
std::pair<node_pol*, node_pol*> Div(node_pol *&first, node_pol *&second);

std::string PtrToString(node_pol *L);

double Count(std::vector<double>&value, node_pol *&p);

node_pol* Derivative(int symb, node_pol *p, int n);
std::string ConvertDouble(double input);

void DeletePol(node_pol *&L);
void Insert(node_pol *&L, node_pol *&E, node_pol *&p);

void AddToBasePtr(node_list *&base, node_list *&base_end, node_pol *&p);

void DeleteFromList(node_list *&base, node_list *&p, node_list *&base_end);

std::string Roots(node_pol *p);
