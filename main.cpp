#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Front.h"
#include "Back.h"
#include "windows.h"


// подсветка полиномов


// 0 при вводе 
// производная от константы 
// clear


extern Button button_enter;
extern Button button_del;
extern Button button_func;
extern Button button_comp;
extern Button button_plus;
extern Button button_mult;
extern Button button_deriv;
extern Button button_div;
extern Button button_roots;
extern Button button_save;
extern Button button_load;

extern Button button_add;
extern Button button_clear;

extern std::vector<Field> printpol;
extern Field field_out;

node_list *base = nullptr, *base_end = nullptr;

void button_enter_click() {
  extern Field field_enter;
  extern Button button_enter_ok;
  std::string input;
  sf::RenderWindow w(sf::VideoMode(600, 300), "Enter");
   while (w.isOpen()) {
     sf::Event event;
     while (w.pollEvent(event)) {
       if (event.type == sf::Event::Closed) w.close();
       if (field_enter.OnClicked(event)) {
         field_enter.clicked = true;
       }
       if (event.type == sf::Event::TextEntered && event.text.unicode != 8 && event.text.unicode != 13) {
         if (field_enter.clicked) {
           input += static_cast<char>(event.text.unicode);
         }
       }
       if (event.type == sf::Event::KeyPressed) {
         if (event.key.code == sf::Keyboard::BackSpace &&
             input.length() > 0) {
           input.erase(input.length() - 1);
         }
       }
       if (button_enter_ok.OnClicked(event)) {
         std::string error = "";
         try {
           CheckPolinomial(input);
         } catch (CheckReturn ret) {
           std::string ch(1, ret.err);
           for (int i = 0; i < ret.position; ++i) {
             error += '~';
           }
           error += "^\n";
           if (ret.error_num == 1) {
             if (ret.err != ' ') {
               error += "ERROR: expected number or letter, but found \"" + ch +
                        "\"\n";
             } else {
               error += "ERROR: expected number or letter, but input ended\n";
             }
           } else if (ret.error_num == 2) {
             error +=
                 "ERROR: letter duplication: \"" + ch + "\" already declared\n";
           } else if (ret.error_num == 3) {
             error +=
                 "ERROR: expecter number, letter, \'+\', or \'-\', but found "
                 "\"" +
                 ch + "\"\n";
           } else if (ret.error_num == 4) {
             error +=
                 "ERROR: expected letter, \'+\', \'-\', or \'^\', but "
                 "found \"" +
                 ch + "\"\n";
           } else if (ret.error_num == 5) {
             if (ret.err != ' ') {
               error += "ERROR: expected number, but found \"" + ch + "\"\n";
             } else {
               error += "ERROR: expected number, but input ended\n";
             }
           }
         }
         if (error.length() != 0) {
           Information_window(input, error);
           input = "";
           error = "";
         } else {
           AddToBase(base, base_end, input);
           printpol.clear();
           UpdatePolinomials(base);
           w.close();
         }
         field_enter.clicked = false;
       }
     }
     w.clear(sf::Color(255, 255, 255));
     Interface_enter(w, input);
     w.display();
   }
}

double button_func_click(node_pol* p, std::vector<bool>v) {
  std::vector<bool> var;
  var = v;
  std::vector<double> value(26);
  int it = 0;
  while (!var[it]) {
    ++it;
  }
  extern Field field_func;
  extern Button button_func_ok;
  sf::RenderWindow w(sf::VideoMode(600, 300), "Function");
  std::string input = "";
  while (w.isOpen()) {
    sf::Event event;
    while (w.pollEvent(event)) {
      if (field_func.OnClicked(event)) {
        field_func.clicked = true;
      }
      if (event.type == sf::Event::Closed) {
        return 0;
        w.close();
      }
      if (event.type == sf::Event::TextEntered && event.text.unicode != 8 &&
          event.text.unicode != 13) {
        if (field_func.clicked) {
          input += static_cast<char>(event.text.unicode);
        }
      }
      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::BackSpace && input.length() > 0) {
          input.erase(input.length() - 1);
        }
      }
      if (button_func_ok.OnClicked(event) ||
          event.type == sf::Event::KeyPressed &&
              event.key.code == sf::Keyboard::Enter) {
        value[it] = stod(input);
        if (input != "") {
          ++it;
          while (it < var.size() && !var[it]) {
            ++it;
          }
          field_func.clicked = false;
        }
        input = "";
        if (it == var.size()) {
          w.close();
          return Count(value, p);
        }
      }
    }
    w.clear(sf::Color(255, 255, 255, 0));
    Interface_func(w, it, input);
    w.display();
  }
}

int NumDeriv() {
  extern Field field_numder;
  extern Button button_numder_ok;
  std::string input;
  sf::RenderWindow w(sf::VideoMode(600, 300), "Derivative");
  while (w.isOpen()) {
    sf::Event event;
    while (w.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        return 0;
        w.close();
      }
      if (field_numder.OnClicked(event)) {
        field_numder.clicked = true;
      }
      if (event.type == sf::Event::TextEntered && event.text.unicode != 8 &&
          event.text.unicode != 13 && event.text.unicode >= 48 && event.text.unicode <= 57) {
        if (field_numder.clicked) {
          input += static_cast<char>(event.text.unicode);
        }
      }
      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::BackSpace && input.length() > 0) {
          input.erase(input.length() - 1);
        }
      }
      if (button_numder_ok.OnClicked(event)) {
        field_numder.clicked = false;
        return std::stoi(input);
      }
    }
    w.clear(sf::Color(255, 255, 255));
    Interface_numder(w, input);
    w.display();
  }
}

node_pol* button_deriv_click(node_pol* p, std::vector<bool>v, sf::RenderWindow& window) {
  double ans = 0;
  int size = 0;
  for (int i = 0; i < v.size(); ++i) {
    if (v[i]) ++size;
  }
  double deriv_heigth = size * 31 + 1;
  sf::RenderWindow w(sf::VideoMode(102, deriv_heigth), "", sf::Style::None);
  extern std::vector<std::pair<Button, int>> var_arr;
  CreateFields(v);
  while (w.isOpen()) {
    sf::Event event;
    while (w.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        w.close();
        return nullptr;
      }
      for (int i = 0; i < var_arr.size(); ++i) {
        if (var_arr[i].first.OnClicked(event)) {
          var_arr[i].first.state = 2;
          Interface_deriv(w);
          w.display();
          Sleep(50);
          w.close();
          int n = NumDeriv();
          return Derivative(var_arr[i].second, p, n);
        }
      }
    }
    w.clear(sf::Color(255, 255, 255, 0));
    if (var_arr.size() == 0) {
      w.close();
      int n = NumDeriv();
      return Derivative(0, p, n);
    }
    Interface_deriv(w);
    w.display();
  }
}

std::string GetFileName() {
  extern Field field_name;
  extern Button button_name_ok;
  std::string input;
  sf::RenderWindow w(sf::VideoMode(600, 300), "File");
  while (w.isOpen()) {
    sf::Event event;
    while (w.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        return "";
        w.close();
      }
      if (field_name.OnClicked(event)) {
        field_name.clicked = true;
      }
      if (event.type == sf::Event::TextEntered && event.text.unicode != 8 &&
          event.text.unicode != 13) {
        if (field_name.clicked) {
          input += static_cast<char>(event.text.unicode);
        }
      }
      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::BackSpace && input.length() > 0) {
          input.erase(input.length() - 1);
        }
      }
      if (button_name_ok.OnClicked(event)) {
        field_name.clicked = false;
        return input;
      }
    }
    w.clear(sf::Color(255, 255, 255));
    Interface_file(w, input);
    w.display();
  }
}


int main() {
  std::ifstream in;
  std::ofstream out;
  int width = 1200;
  int height = 800;
  std::vector<Field*> selected;
  int operation = -1;
  node_pol* answer = nullptr;
  node_pol* answer_sec = nullptr;
  std::string answer_str = "", answer_sec_str = "";
  sf::RenderWindow window(sf::VideoMode(width, height), "Polinomials");
  Field field_out1(600, 380, 550, 0, false);
  Field field_out2(600, 380 + 24, 550, 0, false);
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();
      // 1
      if (button_enter.OnClicked(event)) {
        button_enter_click();
      }
      // 2
      if (button_del.OnClicked(event)) {
        if (selected.size() == 1)
          operation = 2;
      }
      // 3
      if (button_func.OnClicked(event)) {
        if (selected.size() == 1) {
          operation = 3;
        }
      }
      // 4
      if (button_comp.OnClicked(event)) {
        if (selected.size() == 1) {
          operation = 4;
          button_comp.state = 2;
        }
      }
      // 5
      if (button_plus.OnClicked(event)) {
        if (selected.size() == 1) {
          operation = 5;
          button_plus.state = 2;
        }
      }
      // 6
      if (button_mult.OnClicked(event)) {
        if (selected.size() == 1) {
          operation = 6;
          button_mult.state = 2;
        }
      }
      // 7
      if (button_deriv.OnClicked(event)) {
        if (selected.size() == 1) {
          operation = 7;
        }
      }
      // 8
      if (button_div.OnClicked(event)) {
        if (selected.size() == 1) {
          operation = 8;
          button_div.state = 2;
        }
      }
      // 9
      if (button_add.OnClicked(event)) {
        operation = 9;
      }
      // 10
      if (button_clear.OnClicked(event)) {
        operation = 10;
      }
      if (button_roots.OnClicked(event)) {
        if (selected.size() == 1) {
          operation = 11;
        }
      }
      if (button_save.OnClicked(event)) {
        std::string name = GetFileName();
        if (name != "") {
          out.open(name, std::ios::app);
          out.clear();
          for (node_list* p = base; p != nullptr; p = p->next) {
            std::string save_str = PtrToString(p->pol);
            out << save_str << '\n';
          }
          out.close();
        }
      } 
      if (button_load.OnClicked(event)) {
        std::string name = GetFileName();
        if (name != "") {
          in.open(name);
          while (!in.is_open()) {
            Information_window("", "ERROR: This file do not exist");
            std::string name = GetFileName();
            in.open(name);
          }
          std::string input;
          while (std::getline(in, input)) {
            std::string error = "";
            try {
              CheckPolinomial(input);
            } catch (CheckReturn ret) {
              error = "-1";
            }
            if (error.length() != 0) {
              continue;
            } else {
              AddToBase(base, base_end, input);
              printpol.clear();
              UpdatePolinomials(base);
            }
          }
          in.close();
        }
      }
      for (int i = 0; i < printpol.size(); ++i) {
        if (printpol[i].OnClicked(event)) {
          if (selected.size() == 0) {
            printpol[i].SetColorText(true);
            selected.push_back(&printpol[i]);
          } 
          else if (operation != -1 && selected.size() == 1) {
            printpol[i].SetColorText(true);
            selected.push_back(&printpol[i]);
          }
        }
      }

      if (field_out1.OnClicked(event) && !field_out2.clicked) {
        field_out1.clicked = true;
        field_out1.SetColorText(true);
      }

      if (field_out2.OnClicked(event) && !field_out1.clicked) {
        field_out2.clicked = true;
        field_out2.SetColorText(true);
      }
    }
    if (operation == 2) {
      node_list *p = nullptr;
      for (node_list* q = base; q != nullptr; q = q->next) {
        if (q->pol == selected[0]->pol) {
          p = q;
          break;
        }
      }
      DeleteFromList(base, p, base_end);
      printpol.clear();
      UpdatePolinomials(base);
      selected[0]->SetColorText(false);
      selected.clear();
      operation = -1;
    } else if (operation == 3 && selected.size() == 1) {
      DeletePol(answer);
      DeletePol(answer_sec);
      answer_sec_str = "";
      answer_str =
          ConvertDouble(button_func_click(selected[0]->pol, selected[0]->var));
      selected[0]->SetColorText(false);
      selected.clear();
      operation = -1;
    } else if (operation == 4 && selected.size() == 2) {
      bool ans = Compare(selected[0]->pol, selected[1]->pol);
      if (ans)
        answer_str = "TRUE";
      else
        answer_str = "FALSE";
     
      button_comp.state = 0;
      selected[0]->SetColorText(false);
      selected[1]->SetColorText(false);
      selected.clear();
      operation = -1;
    } else if (operation == 5 && selected.size() == 2) {
      DeletePol(answer);
      DeletePol(answer_sec);
      answer = Plus(selected[0]->pol, selected[1]->pol);
      answer_str = PtrToString(answer);
      answer_sec = nullptr;
      answer_sec_str = "";
      button_plus.state = 0;
      selected[0]->SetColorText(false);
      selected[1]->SetColorText(false);
      selected.clear();
      operation = -1;
      button_add.state = 1;
    } else if (operation == 6 && selected.size() == 2) {
      DeletePol(answer);
      DeletePol(answer_sec);
      answer = Mult(selected[0]->pol, selected[1]->pol);
      answer_str = PtrToString(answer);
      answer_sec = nullptr;
      answer_sec_str = "";
      button_mult.state = 0;
      selected[0]->SetColorText(false);
      selected[1]->SetColorText(false);
      selected.clear();
      operation = -1;
      button_add.state = 1;
    } else if (operation == 7 && selected.size() == 1) {
      answer = button_deriv_click(selected[0]->pol, selected[0]->var, window);
      answer_sec = nullptr;
      answer_str = PtrToString(answer);
      answer_sec_str = "";
      selected[0]->SetColorText(false);
      selected.clear();
      operation = -1;
      button_add.state = 1;
    } else if (operation == 8 && selected.size() == 2) {
      int count1 = 0, count2 = 0;
      for (int i = 0; i < selected[0]->var.size(); ++i) {
        if (selected[0]->var[i]) {
          ++count1;
        } 
        if (selected[1]->var[i]) {
          ++count2;
        }
      }
       if (count1 > 1) {
        Information_window(
            "", "ERROR: there is more than 1 variable in the FIRST polynomial");
        operation = 10;
       } else if (count2 > 1) {
         Information_window(
             "",
             "ERROR: there is more than 1 variable in the SECOND polynomial");
         operation = 10;
       } else if (selected[0]->var != selected[1]->var && count1 != 0 && count2 != 0) {
         Information_window("",
                            "ERROR: there is various variables in polynomials");
         operation = 10;
       }
       else {
         button_div.state = 0;
         std::pair<node_pol*, node_pol*> ret =
             Div(selected[0]->pol, selected[1]->pol);
         DeletePol(answer);
         DeletePol(answer_sec);
         answer = ret.first;
         answer_sec = ret.second;
         answer_str = PtrToString(answer);
         answer_sec_str = PtrToString(answer_sec);
         selected[0]->SetColorText(false);
         selected[1]->SetColorText(false);
         selected.clear();
         operation = -1;
         button_add.state = 1;
       }
    } else if (operation == 9) {
      if (button_add.state == 1) {
        if (field_out1.clicked&& answer != nullptr || answer_sec == nullptr) {
          AddToBasePtr(base, base_end, answer);
          printpol.clear();
          UpdatePolinomials(base);
          answer = answer_sec;
          answer_str = answer_sec_str;
          answer_sec_str = "";
          answer_sec = nullptr;
          if (answer == nullptr)
            button_add.state = 0;
      } else if (field_out2.clicked && answer_sec != nullptr) {
        AddToBasePtr(base, base_end, answer_sec);
        printpol.clear();
        UpdatePolinomials(base);
        answer_sec_str = "";
        answer_sec = nullptr;
        if (answer == nullptr)
          button_add.state = 0;
      }
      field_out1.clicked = false;
      field_out2.clicked = false;
      field_out1.SetColorText(false);
      field_out2.SetColorText(false);
      }
      operation = -1;
    }
    if (operation == 10) {
      answer_str = "";
      answer_sec_str = "";
      if (selected.size() > 0)
        selected[0]->SetColorText(false);
      if (selected.size() == 2)
        selected[1]->SetColorText(false);
      DeletePol(answer);
      DeletePol(answer_sec);
      selected.clear();
      button_comp.state = 0;
      button_del.state = 0;
      button_div.state = 0;
      button_mult.state = 0;
      button_plus.state = 0;
      button_add.state = 0;
      operation = -1;
      field_out1.clicked = false;
      field_out2.clicked = false;
      field_out1.SetColorText(false);
      field_out2.SetColorText(false);
    }
    if (operation == 11) {
      int count1 = 0;
      for (int i = 0; i < selected[0]->var.size(); ++i) {
        if (selected[0]->var[i]) {
          ++count1;
        }
      }
      if (count1 > 1) {
        Information_window(
            "", "ERROR: there is more than 1 variable in the polynomial");
        operation = 10;
      } else {
        answer_str = "";
        answer_sec_str = "";
        answer_str = Roots(selected[0]->pol);
        button_div.state = 0;
        if (answer != nullptr)
          DeletePol(answer);
        if (answer_sec != nullptr)
          DeletePol(answer_sec);
        selected[0]->SetColorText(false);
        selected.clear();
        operation = -1;
      }
    }
    window.clear(sf::Color(255, 255, 255, 0));
    Interface(window);
    PrintPolinomials(window);
    int k = 1;
    field_out1.SetText(answer_str);
    k = field_out1.text.size();
    field_out2.SetField(600, 380 + 24 * k, 550, 0, field_out2.clicked);
    field_out2.SetText(answer_sec_str);
    double coord = 381;
    field_out1.SetColor(255, 255, 255);
    field_out2.SetColor(255, 255, 255);
    field_out1.DrawPolinomial(window, coord);
    field_out2.DrawPolinomial(window, coord);

    window.display();
  }

  return EXIT_SUCCESS;
}