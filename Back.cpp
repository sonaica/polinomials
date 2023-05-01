#include "Back.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

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
  return;
}

std::pair<node_pol *, node_pol *> merge(node_pol *&L1, node_pol *&E1,
                                        node_pol *&L2, node_pol *&E2) {
  node_pol *left = L1;
  node_pol *right = E2;
  node_pol *mid = L2;
  node_pol *res = nullptr, *end = nullptr;
  while (left != nullptr && mid != nullptr) {
    if (left->powstr > mid->powstr) {
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
  while (mid != nullptr) {
    node_pol *temp = mid;
    mid = mid->next;
    ins(res, end, temp);
  }
  while (left != nullptr) {
    node_pol *temp = left;
    left = left->next;
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
    if (L->powstr < E->powstr) {
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

int gcd(int a, int b) {
  while (b) {
    a %= b;
    std::swap(a, b);
  }
  return a;
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

void DeletePol(node_pol *&L) {
  while (L != nullptr) {
    DeleteFromPol(L, L, 2);
  }
  return;
}

void DeleteFromList(node_list *&base, node_list *&p, node_list *&base_end) {
  DeletePol(p->pol);

  node_list *q = p;
  p = p->next;
  if (q->next != nullptr) {
    q->next->prev = q->prev;
  } else {
    base_end = q->prev;
  }
  if (q->prev != nullptr) {
    q->prev->next = q->next;
  } else {
    base = q->next;
  }
  delete q;
  return;
}

void Insert(node_pol*&L, node_pol*&E, node_pol*& p) {
  if (L == nullptr) {
    L = p;
  }
  p->next = nullptr;
  p->prev = E;
  if (E != nullptr) E->next = p;
  E = p;
  return;
}

std::pair<node_pol *, std::vector<bool>> CreatePol(std::string input) {
  node_pol *P = nullptr, *E = nullptr;
  int i = 0;
  while (i < input.length()) {
    node_pol *q = new node_pol;
    q->power.resize(ALPH);
    std::string str;
    bool sign = false;
    if (input[i] == '+' || input[i] == '-') {
      str += input[i];
      ++i;
    }
    while (!(input[i] >= 'a' && input[i] <= 'z') && i < input.length() &&
           input[i] != '+' && input[i] != '-') {
      str += input[i];
      ++i;
    }
    if (str == "-" || str == "+") {
      str += '1';
    }
    if (str == "")
      q->numerator = 1;
    else
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
    std::string powstr = "";
    for (int i = 0; i < q->power.size(); ++i) {
      powstr += std::to_string(q->power[i]);
    }
    q->powstr = powstr;
    if (E != nullptr) E->next = q;
    E = q;
  }
  merge_sort(P, E);

  for (node_pol *t = P; t != nullptr;) {
    if (t->numerator == 0) {
      DeleteFromPol(P, t, 2);
    } else {
      t = t->next;
    }
  }

  for (node_pol *t = P; t != nullptr;) {
    if (t->next != nullptr) {
      if (t->powstr == t->next->powstr) {
        int den = t->denominator * t->next->denominator /
                  gcd(t->denominator, t->next->denominator);
        int coef_1 = den / t->denominator;
        int coef_2 = den / t->next->denominator;
        int num = coef_1 * t->numerator + coef_2 * t->next->numerator;
        int coef = gcd(den, num);
        num = num / coef;
        den = den / coef;
        t->numerator = num;
        t->denominator = den;
        DeleteFromPol(P, t->next, 1);
        if (t->numerator == 0) {
          DeleteFromPol(P, t, 1);
        }
      } else {
        t = t->next;
      }
    } else {
      t = t->next;
    }
  }

  // Cast(P);
  std::vector<bool> var(26, false);
  if (P == nullptr) {
    node_pol *q = new node_pol;
    std::string str = "";
    for (int i = 0; i < 26; ++i) {
      str += '0';
    }
    q->powstr = str;
    q->numerator = 0;
    q->denominator = 1;
    std::vector<int> power(26, 0);
    q->power = power;
    P = q;
    q->prev = nullptr;
    q->next = nullptr;
  } else {
    for (node_pol *q = P; q != nullptr; q = q->next) {
      for (int i = 0; i < q->power.size(); ++i) {
        if (q->power[i] != 0) var[i] = true;
      }
    }
  }
  return {P, var};
}

void AddToBase(node_list *&base, node_list *&base_end, std::string input) {
  node_list *new_pol = new node_list;
  std::pair<node_pol *, std::vector<bool>> value = CreatePol(input);
  new_pol->pol = value.first;
  new_pol->color = false;
  new_pol->next = nullptr;
  new_pol->prev = base_end;
  new_pol->var = value.second;
  if (base_end != nullptr) base_end->next = new_pol;
  if (base == nullptr) base = new_pol;
  base_end = new_pol;
  return;
}


bool Compare(node_pol *&first, node_pol *&second) {
  node_pol *q = second;
  node_pol *p = first;
  for (; p != nullptr; p = p->next) {
    if (q == nullptr) break;
    if (p->powstr == q->powstr && p->numerator == q->numerator &&
        p->denominator == q->denominator) {
      q = q->next;
    } else {
      return false;
    }
  }
  if (q == nullptr && p == nullptr)
    return true;
  return false;
}

node_pol* Plus(node_pol *&first, node_pol *&second) { 
   node_pol *ans = nullptr, *ans_end = nullptr;
  for (node_pol *p = first; p != nullptr; p = p->next) {
    node_pol *t = new node_pol;
    t->denominator = p->denominator;
    t->numerator = p->numerator;
    t->powstr = p->powstr;
    t->power = p->power;
    if (ans == nullptr) {
      ans = t;
    }
    t->next = nullptr;
    t->prev = ans_end;
    if (ans_end != nullptr) ans_end->next = t;
    ans_end = t;
  }

  for (node_pol *p = second; p != nullptr; p = p->next) {
    node_pol *t = new node_pol;
    t->denominator = p->denominator;
    t->numerator = p->numerator;
    t->powstr = p->powstr;
    t->power = p->power;
    if (ans == nullptr) {
      ans = t;
    }
    t->next = nullptr;
    t->prev = ans_end;
    if (ans_end != nullptr) ans_end->next = t;
    ans_end = t;
  }

  merge_sort(ans, ans_end);

  for (node_pol *t = ans; t != nullptr;) {
    if (t->numerator == 0) {
      DeleteFromPol(ans, t, 2);
    } else {
      t = t->next;
    }
  }

  for (node_pol *t = ans; t != nullptr;) {
    if (t->next != nullptr) {
      if (t->powstr == t->next->powstr) {
        int den = t->denominator * t->next->denominator /
                  gcd(t->denominator, t->next->denominator);
        int coef_1 = den / t->denominator;
        int coef_2 = den / t->next->denominator;
        int num = coef_1 * t->numerator + coef_2 * t->next->numerator;
        int coef = gcd(den, num);
        num = num / coef;
        den = den / coef;
        t->numerator = num;
        t->denominator = den;
        DeleteFromPol(ans, t->next, 1);
        if (t->numerator == 0) {
          DeleteFromPol(ans, t, 1);
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

  if (ans == nullptr) {
    node_pol *q = new node_pol;
    std::string str = "";
    for (int i = 0; i < 26; ++i) {
      str += '0';
    }
    q->powstr = str;
    q->numerator = 0;
    q->denominator = 1;
    std::vector<int> power(26, 0);
    q->power = power;
    ans = q;
    q->prev = nullptr;
    q->next = nullptr;
  }

  return ans;
}

std::string PtrToString(node_pol *L) {
  std::string pol_str = "";
  for (node_pol *q = L; q != nullptr; q = q->next) {
    if (double(q->numerator) / q->denominator > 0 && pol_str.length() != 0) {
      pol_str += '+';
    }

    std::string res = "";
    if (q->denominator == 1 || q->denominator == 0) {
      res = std::to_string(q->numerator);
    } else {
      res = ConvertDouble((double)q->numerator / q->denominator);
    }

    pol_str += res;

    for (int i = 0; i < q->power.size(); ++i) {
      if (q->power[i] != 0) {
        char ch = i + 'a';
        pol_str += ch;
        if (q->power[i] != 1) {
          pol_str += '^' + std::to_string(q->power[i]);
        }
      }
    }
  }
  return pol_str;
}

node_pol *Mult(node_pol *&first, node_pol *&second) {
  node_pol *ans = nullptr, *ans_end = nullptr;
  for (node_pol *p = first; p != nullptr; p = p->next) {
    for (node_pol *q = second; q != nullptr; q = q->next) {
      node_pol *t = new node_pol;
      t->numerator = q->numerator * p->numerator;
      t->denominator = q->denominator * p->denominator;
      int coef = gcd(t->denominator, t->numerator);
      t->numerator = t->numerator / coef;
      t->denominator = t->denominator / coef;
      std::vector<int> power(ALPH);
      std::string powstr = "";
      for (int i = 0; i < ALPH; ++i) {
        power[i] = q->power[i] + p->power[i];
        powstr += std::to_string(power[i]);
      }
      t->power = power;
      t->powstr = powstr;
      if (ans == nullptr) {
        ans = t;
      }
      t->next = nullptr;
      t->prev = ans_end;
      if (ans_end != nullptr) ans_end->next = t;
      ans_end = t;
    }
  }
  merge_sort(ans, ans_end);

  for (node_pol *t = ans; t != nullptr;) {
    if (t->numerator == 0) {
      DeleteFromPol(ans, t, 2);
    } else {
      t = t->next;
    }
  }

  for (node_pol *t = ans; t != nullptr;) {
    if (t->next != nullptr) {
      if (t->powstr == t->next->powstr) {
        int den = t->denominator * t->next->denominator /
                  gcd(t->denominator, t->next->denominator);
        int coef_1 = den / t->denominator;
        int coef_2 = den / t->next->denominator;
        int num = coef_1 * t->numerator + coef_2 * t->next->numerator;
        int coef = gcd(den, num);
        num = num / coef;
        den = den / coef;
        t->numerator = num;
        t->denominator = den;
        DeleteFromPol(ans, t->next, 1);
        if (t->numerator == 0) {
          DeleteFromPol(ans, t, 1);
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

  if (ans == nullptr) {
    node_pol *q = new node_pol;
    std::string str = "";
    for (int i = 0; i < 26; ++i) {
      str += '0';
    }
    q->powstr = str;
    q->numerator = 0;
    q->denominator = 1;
    std::vector<int> power(26, 0);
    q->power = power;
    ans = q;
    q->prev = nullptr;
    q->next = nullptr;
  }

  return ans;
}

double Count(std::vector<double> &value, node_pol *&p) {
  long double ans = 0;
  for (node_pol *q = p; q != nullptr; q = q->next) {
    long double a = 1;
    for (int i = 0; i < q->power.size(); ++i) {
      if (q->power[i] != 0) {
        a *= std::pow(value[i], q->power[i]);
      }
    }
    a *= q->numerator;
    a /= q->denominator;
    ans += a;
  }
  return ans;
}

node_pol *Derivative(int symb, node_pol*p, int n) { 
  while (n) {
    node_pol *r = nullptr, *e = nullptr;
    for (node_pol *q = p; q != nullptr; q = q->next) {
      node_pol *t = new node_pol;
      double coef = q->numerator * q->power[symb] / q->denominator;
      t->numerator = q->numerator * q->power[symb];
      t->denominator = q->denominator;
      if (coef == 0) {
        continue;
      }

      std::vector<int> t_power(26);
      std::string powstr = "";
      for (int i = 0; i < q->power.size(); ++i) {
        if (i == symb) {
          if (q->power[i] >= 2) {
            t_power[symb] = q->power[i] - 1;
          }
        } else {
          t_power[i] = q->power[i];
        }
        powstr += std::to_string(q->power[i]);
      }
      t->power = t_power;
      t->powstr = powstr;
      Insert(r, e, t);
    }

    for (node_pol *t = r; t != nullptr;) {
      if (t->numerator == 0) {
        DeleteFromPol(r, t, 1);
      } else {
        t = t->next;
      }
    }

    if (r == nullptr) {
      node_pol *q = new node_pol;
      std::string str = "";
      for (int i = 0; i < 26; ++i) {
        str += '0';
      }
      q->powstr = str;
      q->numerator = 0;
      q->denominator = 1;
      std::vector<int> power(26, 0);
      q->power = power;
      r = q;
      q->prev = nullptr;
      q->next = nullptr;
    }
    p = r;
    --n;
  }
  return p;
}

std::pair<node_pol*, node_pol*> Div(node_pol *&first, node_pol *&second) {
  node_pol *quo = nullptr, *rem = nullptr;
  node_pol *quo_end = nullptr;

  node_pol *m = nullptr, *n = nullptr;
  for (node_pol *p = first; p != nullptr; p = p->next) {
    node_pol *temp = new node_pol;
    temp->numerator = p->numerator;
    temp->denominator = p->denominator;
    temp->powstr = p->powstr;
    temp->power = p->power;
    if (m == nullptr) {
      m = temp;
    }
    temp->next = nullptr;
    temp->prev = n;
    if (n != nullptr) n->next = temp;
    n = temp;
  }
  while (m != nullptr) {
    node_pol *q = new node_pol;
    node_pol *d = nullptr, *e = nullptr;
    for (node_pol *p = second; p != nullptr; p = p->next) {
      node_pol *temp = new node_pol;
      temp->numerator = p->numerator;
      temp->denominator = p->denominator;
      temp->powstr = p->powstr;
      temp->power = p->power;
      if (d == nullptr) {
        d = temp;
      }
      temp->next = nullptr;
      temp->prev = e;
      if (e != nullptr) e->next = temp;
      e = temp;
    }
    if (m->powstr >= d->powstr) {
      bool s = false;
      int num = m->numerator * d->denominator;
      if (num < 0) s = true;
      num = abs(num);
      int den = m->denominator * d->numerator;
      if (den < 0) {
        if (s)
          s = false;
        else
          s = true;
      }
      den = abs(den);
      double coef = gcd(num, den);
      num = num / coef;
      den = den / coef;
      if (s) num = -1 * num;
      for (node_pol *p = d; p != nullptr; p = p->next) {
        if (p->numerator == 0) {
          p->numerator = 1;
        }
        if (p->denominator == 0) {
          p->denominator = 1;
        }
        bool sign = true;
        p->numerator *= num;
        if (p->numerator < 0) sign = false; 
        p->numerator = std::abs(p->numerator);
        p->denominator *= den;
        p->denominator = std::abs(p->denominator);
        coef = gcd(p->numerator, p->denominator);
        p->numerator /= coef;
        p->denominator /= coef;
        if (sign) p->numerator *= -1;
      }
      int pow = 0;
      int symb = -1;
      for (int i = 0; i < m->power.size(); ++i) {
        if (m->power[i] != 0) {
          symb = i;
          pow = m->power[i] - d->power[i];
          break;
        }
      }

      q->numerator = num;
      q->denominator = den;

      for (node_pol *p = d; p != nullptr; p = p->next) {
        p->power[symb] += pow;
        unsigned long long pw = std::pow(31, symb);
        p->powstr = "";
        for (int i = 0; i < p->power.size(); ++i) { 
          p->powstr += std::to_string(p->power[i]);
        }
      }
      std::vector<int> q_power(26, 0);
      q_power[symb] = pow;
      q->power = q_power;
      std::string powstr = "";
      for (int i = 0; i < q->power.size(); ++i) {
        powstr += std::to_string(q->power[i]);
      }
      q->powstr = powstr;

      Insert(quo, quo_end, q);
      node_pol *k = m;
      m = Plus(m, d);
      while (k != nullptr) {
        DeleteFromPol(k, k, 2);
      }
    } else {
      rem = m;
      break;
    }
    while (d != nullptr) {
      DeleteFromPol(d, d, 2);
    }
  }

  for (node_pol *t = quo; t != nullptr;) {
    if (t->numerator == 0) {
      DeleteFromPol(quo, t, 1);
    } else {
      t = t->next;
    }
  }

  for (node_pol *t = rem; t != nullptr;) {
    if (t->numerator == 0) {
      DeleteFromPol(rem, t, 1);
    } else {
      t = t->next;
    }
  }

  if (quo == nullptr) {
    node_pol *q = new node_pol;
    std::string str = "";
    for (int i = 0; i < 26; ++i) {
      str += '0';
    }
    q->powstr = str;
    q->numerator = 0;
    q->denominator = 1;
    std::vector<int> power(26, 0);
    q->power = power;
    quo = q;
    q->prev = nullptr;
    q->next = nullptr;
  }

  if (rem == nullptr) {
    node_pol *q = new node_pol;
    std::string str = "";
    for (int i = 0; i < 26; ++i) {
      str += '0';
    }
    q->powstr = str;
    q->numerator = 0;
    q->denominator = 1;
    std::vector<int> power(26, 0);
    q->power = power;
    rem = q;
    q->prev = nullptr;
    q->next = nullptr;
  }
  return {quo, rem};
}

std::string ConvertDouble(double input) {
  std::stringstream stream;
  stream << std::fixed << std::setprecision(2) << input;
  std::string ans = stream.str();
  for (int i = ans.length() - 1; i >= 0; --i) {
    if (ans[i] == '0' || ans[i] == '.') {
      ans.erase(i, 1);
    }
    else {
      break;
    }
  }
  return ans;
}

void AddToBasePtr(node_list*& base, node_list*& base_end, node_pol*& p) {
  node_list *new_pol = new node_list;
  new_pol->pol = p;
  new_pol->color = false;
  new_pol->next = nullptr;
  new_pol->prev = base_end;
  std::vector<bool> var(26, false);
  for (int i = 0; i < p->power.size(); ++i) {
    if (p->power[i] != 0) var[i] = true;
  }
  new_pol->var = var; 
  if (base_end != nullptr) base_end->next = new_pol;
  if (base == nullptr) base = new_pol;
  base_end = new_pol;
  return;
}

std::string Roots(node_pol *p) {
  std::string r = "";
  int coef = 1;
  int den1 = 1, den2 = 1;
  int symb = 0;
  for (int i = 0; i < p->power.size(); ++i) {
    if (p->power[i] != 0) symb = i;
  }
  for (node_pol *q = p; q != nullptr; q = q->next) {
    if (q == p) {
      den1 = abs(q->denominator);
    }
    if (q->next == nullptr) {
      den2 = abs(q->denominator);
    }
  }
  int num1 = 0, num2 = 0;
  coef = den1 * den2;
  for (node_pol *q = p; q != nullptr; q = q->next) {
    q->numerator *= coef;
    q->denominator *= coef;
    int c = gcd(q->numerator, q->denominator);
    q->numerator /= c;
    q->denominator /= c;
    if (q == p) num1 = abs(q->numerator);
    if (q->next == nullptr) num2 = abs(q->numerator);
  }
  std::vector<int> d;
  int i = 1;
  while (i * i <= num2) {
    if (num2 % i == 0) {
      d.push_back(i);
      if (i != num2 / i)
        d.push_back(num2 / i);
    }
    ++i;
  }

  int size = d.size();
  for (int k = 0; k < size; ++k) {
    d.push_back(-1 * d[k]);
  }
  d.push_back(0);

  for (int j = 0; j < d.size(); ++j) {
    if (d[j] == 227) {
      int a = 0;
    }
    std::vector<double> temp(26);
    for (int k = 0; k < 26; ++k) {
      if (k == symb)
        temp[k] = (double)d[j];
      else
        temp[k] = 0.0;
    }
    double ans = Count(temp, p);
    if (ans == 0) {
      if (d[j] == 0) {
        r += "0 ";
      } else {
        r += ConvertDouble((double)d[j]) + " ";
      }
    }
  }
  if (r == "") {
    r = "No integer roots";
  }
  return r;
}