#include "Back.h"
#include <string>
#include <vector>

const int ALPH = 26;

//void Cast(node_pol *&P) {
//  double coef = P->coef;
//  for (node_pol *q = P; q != nullptr; q = q->next) {
//    q->coef = q->coef / coef;
//  }
//  return;
//}

void ins(node_pol *&res, node_pol *&end, node_pol *&p) {
  node_pol *temp = p;
  temp->next = end->next;
  temp->prev = end;
  end->next = temp;
  end = temp;
}

std::pair<node_pol *, node_pol *> merge(node_pol *&L1, node_pol *&E1,
                                        node_pol *&L2, node_pol *&E2) {
  node_pol *left = L1;
  node_pol *right = E2;
  node_pol *mid = L2;
  node_pol *res = nullptr, *end = nullptr;
  while (left != nullptr && mid != nullptr) {
    if (left->hash > mid->hash) {
      node_pol *temp_left = left;
      left = left->next;
      if (res == nullptr) {
        res = temp_left;
        end = temp_left;
        res->next = nullptr;
        end->prev = nullptr;
      } else {
        ins(res, end, temp_left);
      }
    } else {
      node_pol *temp_mid = mid;
      mid = mid->next;
      if (res == nullptr) {
        res = temp_mid;
        end = temp_mid;
        res->next = nullptr;
        end->prev = nullptr;
      } else {
        ins(res, end, temp_mid);
      }
    }
  }
  while (left != nullptr) {
    node_pol *temp = left;
    left = left->next;
    ins(res, end, temp);
  }
  while (mid != nullptr) {
    node_pol *temp = mid;
    mid = mid->next;
    ins(res, end, temp);
  }
  return {res, end};
}

void merge_sort(node_pol *&L, node_pol *&E) {
  int i = 0;
  for (node_pol *p = L; p != nullptr; p = p->next) {
    if (p == E) {
      break;
    }
    ++i;
  }
  if (i > 1) {
    node_pol *L1 = nullptr, *E1 = nullptr, *L2 = nullptr, *E2 = nullptr;
    node_pol *f = L, *s = E;
    node_pol *f_temp = f, *s_temp = s;
    while (f_temp->next != nullptr && s_temp != nullptr && f != s) {
      f = f->next;
      s = s->prev;
      if (L1 == nullptr) {
        L1 = f_temp;
        E1 = f_temp;
        L1->next = nullptr;
        L1->prev = nullptr;
      } else {
        ins(L1, E1, f_temp);
      }
      if (L2 == nullptr) {
        L2 = s_temp;
        E2 = s_temp;
        L2->next = nullptr;
        L2->prev = nullptr;
      } else {
        ins(L2, E2, s_temp);
      }
      f_temp = f;
      s_temp = s;
    }
    if (f_temp == s_temp) {
      ins(L1, E1, f_temp);
    }
    merge_sort(L1, E1);
    merge_sort(L2, E2);
    std::pair<node_pol *, node_pol *> pr = merge(L1, E1, L2, E2);
    L = pr.first;
    E = pr.second;
  } else if (i == 1) {
    if (L->hash < E->hash) {
      std::swap(L, E);
      L->next = E;
      L->prev = nullptr;
      E->next = nullptr;
      E->prev = L;
    }
  }
}


void CheckPolinomial(std::string input) {
  States state = States::StateH;
  CheckReturn ret;
  ret.error_num = -1;
  char cur;
  const int SIZE = 26;
  std::vector<int> alph(SIZE, 0);
  int let = -1;
  std::string pow = "";
  for (int i = 0; i < input.length(); ++i) {
    cur = input[i];
    switch (state) {
      case States::StateH:
        if (cur == '-') {
          alph.clear();
          alph.resize(SIZE);
          state = States::StateA;
        } else if (cur >= '0' && cur <= '9') {
          state = States::StateB;
        } else if (cur >= 'a' && cur <= 'z') {
          let = cur - 'a';
          alph[let] = 1;
          state = States::StateC;
        } else {
          ret.error_num = 1;
          ret.position = i;
          ret.err = cur;
          throw ret;
        }
        break;
      case States::StateA:
        if (cur >= '0' && cur <= '9') {
          state = States::StateB;
        } else if (cur >= 'a' && cur <= 'z') {
          state = States::StateC;
          let = cur - 'a';
          alph[let] = 1;
        } else {
          ret.error_num = 1;
          ret.position = i;
          ret.err = cur;
          throw ret;
        }
        break;
      case States::StateB:
        if (cur == '+' || cur == '-') {
          state = States::StateA;
          alph.clear();
          alph.resize(SIZE);
        } else if (cur >= 'a' && cur <= 'z') {
          state = States::StateC;
          let = cur - 'a';
          alph[let] = 1;
        } else if (cur >= '0' && cur <= '9')
          state = States::StateB;
        else {
          ret.error_num = 3;
          ret.position = i;
          ret.err = cur;
          throw ret;
        }
        break;
      case States::StateC:
        if (cur == '+' || cur == '-') {
          state = States::StateA;
          alph.clear();
          alph.resize(26);
        } else if (cur == '^') {
          state = States::StateD;
        } else if (cur >= 'a' && cur <= 'z') {
          state = States::StateC;
          let = cur - 'a';
          if (alph[let] != 0) {
            ret.error_num = 2;
            ret.position = i;
            ret.err = cur;
            throw ret;
          }
          alph[let] = 1;
        } else {
          ret.error_num = 4;
          ret.position = i;
          ret.err = cur;
          throw ret;
        }
        break;
      case States::StateD:
        if (cur >= '0' && cur <= '9')
          state = States::StateE;
        else {
          ret.error_num = 5;
          ret.position = i;
          ret.err = cur;
          throw ret;
        }
        break;
      case States::StateE:
        if (cur == '+' || cur == '-') {
          state = States::StateA;
          alph.clear();
          alph.resize(SIZE);
        } else if (cur >= 'a' && cur <= 'z') {
          state = States::StateC;
          let = cur - 'a';
          if (alph[let] != 0) {
            ret.error_num = 2;
            ret.err = cur;
            ret.position = i;
            throw ret;
          }
          alph[let] = 1;
        } else if (cur >= '0' && cur <= '9') {
          state = States::StateE;
        } else {
          ret.error_num = 3;
          ret.err = cur;
          ret.position = i;
          throw ret;
        }
        break;
    }
  }
  if (state == States::StateH || state == States::StateA) {
    ret.error_num = 1;
    ret.position = input.length();
    ret.err = ' ';
    throw ret;
  }
  if (state == States::StateD) {
    ret.error_num = 5;
    ret.position = input.length();
    ret.err = ' ';
    throw ret;
  }
  return;
}

void DeleteFromPol(node_pol *&L, node_pol *&p, int call) {
  node_pol *q = p;
  if (call == 1) {
    p = p->prev;
  } else if (call == 2) {
    p = p->next;
  }
  if (q->next != nullptr) {
    q->next->prev = q->prev;
  }
  if (q->prev != nullptr) {
    q->prev->next = q->next;
  } else {
    L = q->next;
  }
  delete q;
  return;
}

node_pol *CreatePol(std::string input) {
  node_pol *P = nullptr, *E = nullptr;
  int i = 0;
  while (i < input.length()) {
    node_pol *q = new node_pol;
    q->power.resize(ALPH);
    std::string str;
    bool sign = false;
    while (!(input[i] >= 'a' && input[i] <= 'z') && i < input.length()) {
      if (input[i] == '+' || input[i] == '-') {
        if (sign) {
          break;
        } else {
          sign = true;
        }
      }
      str += input[i];
      ++i;
    }
    if (str == "-" || str == "+") {
      str += '1';
    }
    if (str == "+0" || str == "-0") {
      delete q;
      continue;
    }
    q->numerator = std::stod(str);
    q->denominator = 1;
    str = "";
    while (input[i] != '-' && input[i] != '+' && i < input.length()) {
      int let = input[i] - 'a';
      std::string pow = "";
      ++i;
      if (input[i] == '^') ++i;
      while (!(input[i] >= 'a' && input[i] <= 'z') && input[i] != '+' &&
             input[i] != '-' && i < input.length()) {
        pow += input[i];
        ++i;
      }
      if (pow == "") pow = "1";
      q->power[let] = std::stoi(pow);
    }
    if (P == nullptr) {
      P = q;
    }
    q->next = nullptr;
    q->prev = E;
    unsigned long long hash = 0;
    int p = 1;
    for (int i = 0; i < q->power.size(); ++i) {
      hash += q->power[i] * p;
      p *= 2;
    }
    q->hash = hash;
    if (E != nullptr) E->next = q;
    E = q;
  }
  merge_sort(P, E);

  //упрощение
  for (node_pol *t = P; t != nullptr;) {
    if (t->next != nullptr) {
      if (t->hash == t->next->hash) {
        t->numerator = t->numerator + t->next->numerator;
        DeleteFromPol(P, t->next, 1);
        if (t->numerator == 0) {
          DeleteFromPol(P, t, 2);
        } else {
          t = t->next;
        }
      } else {
        t = t->next;
      }
    } else {
      t = t->next;
    }
  }

  //Cast(P);
  return P;
}

void AddToBase(node_list *&base, node_list *&base_end, std::string input) {
  node_list *new_pol = new node_list;
  new_pol->pol = CreatePol(input);
  new_pol->color = false;
  new_pol->next = nullptr;
  new_pol->prev = base_end;
  if (base_end == nullptr) base_end = new_pol;
  else
    base_end->next = new_pol;
  if (base == nullptr) base = new_pol;
  return;
}