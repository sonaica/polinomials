#include "Back.h"
#include <string>
#include <vector>
#include <algorithm>
#include <set>

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

std::pair<node_pol*, std::set<int>> CreatePol(std::string input) {
  std::set<int> var;
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
    if (str == "+0" || str == "-0") {
      delete q;
      continue;
    }
    if (str == "")
      q->numerator = 0;
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
    unsigned long long hash = 0;
    int p = 1;
    for (int i = 0; i < q->power.size(); ++i) {
      hash += q->power[i] * p;
      p *= 2;
      if (q->power[i] != 0) var.insert(i);
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
  return {P, var};
}

void AddToBase(node_list *&base, node_list *&base_end, std::string input) {
  node_list *new_pol = new node_list;
  std::pair<node_pol *, std::set<int>> value = CreatePol(input);
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
  for (node_pol *p = first; p != nullptr; p = p->next) {
    if (p->hash == q->hash && p->numerator == q->numerator &&
        p->denominator == q->denominator) {
      q = q->next;
    } else {
      return false;
    }
  }
  return true;
}

int gcd(int a, int b) {
  while (b) {
    a %= b;
    std::swap(a, b);
  }
  return a;
}

std::string Plus(node_pol *&first, node_pol *&second) { 
  node_pol *ans = nullptr, *ans_end = nullptr;
  for (node_pol *p = first; p != nullptr; p = p->next) {
    node_pol *t = new node_pol;
    t->denominator = p->denominator;
    t->numerator = p->numerator;
    t->hash = p->hash;
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
    t->hash = p->hash;
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
    if (t->next != nullptr) {
      if (t->hash == t->next->hash) {
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
          DeleteFromPol(ans, t, 2);
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

  std::string ans_str = PtrToString(ans);

  node_pol*p = ans;
  while (p != nullptr) {
    DeleteFromPol(ans, p, 2);
  }

  return ans_str;
}

node_pol* PlusStr(node_pol*& first, node_pol*& second) {
  node_pol *ans = nullptr, *ans_end = nullptr;
  for (node_pol *p = first; p != nullptr; p = p->next) {
    node_pol *t = new node_pol;
    t->denominator = p->denominator;
    t->numerator = p->numerator;
    t->hash = p->hash;
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
    t->hash = p->hash;
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
    if (t->next != nullptr) {
      if (t->hash == t->next->hash) {
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
          DeleteFromPol(ans, t, 2);
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
  return ans;
}

std::string PtrToString(node_pol* L) {
  std::string pol_str = "";
  for (node_pol *q = L; q != nullptr; q = q->next) {
    if (double(q->numerator) / q->denominator > 0 && pol_str.length() != 0) {
      pol_str += '+';
    }

    std::string res = "";
    if (q->numerator != 0) {
      if (q->denominator == 1) {
        res = std::to_string(q->numerator);
      } else {
        std::stringstream stream;
        stream << std::fixed << std::setprecision(2)
               << double(q->numerator) / q->denominator;
        std::string s = stream.str();
      }
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

std::string Mult(node_pol *&first, node_pol *&second) {
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
      int hash = 0;
      int pow = 1;
      for (int i = 0; i < ALPH; ++i) {
        power[i] = q->power[i] + p->power[i];
        hash += pow * power[i];
        pow *= 2;
      }
      t->power = power;
      t->hash = hash;
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
    if (t->next != nullptr) {
      if (t->hash == t->next->hash) {
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
          DeleteFromPol(ans, t, 2);
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

  std::string ans_str = PtrToString(ans);
  
  node_pol *p = ans;
  while (p != nullptr) {
    DeleteFromPol(ans, p, 2);
  }

  return ans_str;
}

double Count(std::vector<double> &value, node_pol *&p) {
  double ans = 0;
  for (node_pol *q = p; q != nullptr; q = q->next) {
    double a = 1;
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

std::string Derivative(int symb, node_pol*&p) { 
  std::string ans = "";
  for (node_pol *q = p; q != nullptr; q = q->next) {
    double coef = q->numerator * q->power[symb] / q->denominator;
    if (coef == 0) {
      continue;
    }
    if (coef > 0 && ans.length() != 0) ans += '+';
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << coef;
    ans += stream.str();

    for (int i = 0; i < q->power.size(); ++i) {
      if (i == symb) {
        if (q->power[i] > 1) {
          ans += (1, i + 'a');
        }
        if (q->power[i] > 2) {
          ans += '^' + std::to_string(q->power[i] - 1);
        }
      } else if (q->power[i] != 0) {
        ans += (1, i + 'a');
        if (q->power[i] > 1) {
          ans += '^' + std::to_string(q->power[i]);
        }
      }
    }
  }
  return ans;
}

std::pair<std::string, std::string> Div(node_pol *&first, node_pol *&second) {
  std::string quo = "", rem = "";

  node_pol *m = nullptr, *n = nullptr;
  for (node_pol *p = first; p != nullptr; p = p->next) {
    node_pol *temp = new node_pol;
    temp->numerator = p->numerator;
    temp->denominator = p->denominator;
    temp->hash = p->hash;
    temp->power = p->power;
    if (m == nullptr) {
      m = temp;
    }
    temp->next = nullptr;
    temp->prev = n;
    if (n != nullptr) n->next = temp;
    n = temp;
  }
  for (node_pol* q = m; q != nullptr; q = q->next) {
    node_pol *d = nullptr, *e = nullptr;
    for (node_pol *p = second; p != nullptr; p = p->next) {
      node_pol *temp = new node_pol;
      temp->numerator = p->numerator;
      temp->denominator = p->denominator;
      temp->hash = p->hash;
      temp->power = p->power;
      if (d == nullptr) {
        d = temp;
      }
      temp->next = nullptr;
      temp->prev = e;
      if (e != nullptr) e->next = temp;
      e = temp;
    }
    if (m->hash >= d->hash) {
      int num = m->numerator * d->denominator;
      int den = m->denominator * d->numerator;
      if (num == 0) num = 1;
      if (den == 0) den = 1;
      num = num / gcd(num, den);
      den = den / gcd(num, den);
      for (node_pol* p = d; p != nullptr; p = p->next) {
        if (p->numerator == 0) {
          p->numerator = 1;
        }
        if (p->denominator == 0) {
          p->denominator = 1;
        }
        p->numerator *= num;
        p->denominator *= den;
        p->numerator /= gcd(p->numerator, p->denominator);
        p->denominator /= gcd(p->numerator, p->denominator);
        p->numerator *= -1;
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
      if (num / den > 0 && quo.length() != 0) {
        quo += '+';
      }
      std::stringstream stream;
      stream << std::fixed << std::setprecision(2) << (double)num / den;
      quo += stream.str();
      if (pow != 0) {
        quo += (1, symb + 'a');
        if (pow > 1) {
          quo += '^' + std::to_string(pow);
        }
      }
      for (node_pol *p = d; p != nullptr; p = p->next) {
        p->power[symb] += pow;
        p->hash = std::pow(2, symb) * p->power[symb];
      }
      node_pol *k = m;
      m = PlusStr(m, d);
      while (k != nullptr) {
        DeleteFromPol(k, k, 2);
      }
    }
    else {
      rem = PtrToString(m);
    }
    while (d != nullptr) {
      DeleteFromPol(d, d, 2);
    }
    if (m == nullptr) {
      break;
    }
  }
  while (m != nullptr) {
    DeleteFromPol(m, m, 2);
  }
  return {quo, rem};
}