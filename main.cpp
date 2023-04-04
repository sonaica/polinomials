#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <set>
#include <iostream>
#include "Front.h"
#include "Back.h"
#include "windows.h"


// нормальный перенос строки в информационном окне
// вылезание за строку поля 
// состояния кнопок
// закрытие окна выбора переменной при клике вне окна 
// адекватный вывод в окно out
// потестить

// выбор полинома в окне field_out
// убрать тупой coord
// почистить div, проверить
// когда clear удалять еще answer_sec
// проверить сложение!!!!!!


extern Button button_enter;
extern Button button_del;
extern Button button_func;
extern Button button_comp;
extern Button button_plus;
extern Button button_mult;
extern Button button_deriv;
extern Button button_div;
extern Button button_roots;

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

double button_func_click(node_pol* p, std::set<int>v) {
  std::set<int> var;
  var = v;
  std::vector<double> value(26);
  std::set<int>::iterator it = var.begin();
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
        value[*it] = stod(input);
        if (input != "") {
          ++it;
          field_func.clicked = false;
        }
        input = "";
        if (it == var.end()) {
          w.close();
          return Count(value, p);
        }
      }
    }
    w.clear(sf::Color(255, 255, 255, 0));
    Interface_func(w, *it, input);
    w.display();
  }
}

std::string button_deriv_click(node_pol* p, std::set<int> v, sf::RenderWindow& window) {
  double ans = 0;
  double deriv_heigth = v.size() * 31 + 1;
  sf::RenderWindow w(sf::VideoMode(102, deriv_heigth), "", sf::Style::None);
  extern std::vector<std::pair<Field, int>> var_arr;
  CreateFields(v.size(), v);
  while (w.isOpen()) {
    sf::Event event;
    while (w.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        w.close();
        return "";
      }
      for (int i = 0; i < var_arr.size(); ++i) {
        if (var_arr[i].first.OnClicked(event)) {
          var_arr[i].first.clicked = true;
          Interface_deriv(w);
          w.display();
          Sleep(100);
          w.close();
          return Derivative(var_arr[i].second, p);
        }
      }
    }
    w.clear(sf::Color(255, 255, 255, 0));
    Interface_deriv(w);
    w.display();
  }
}


int main() {
  int width = 1200;
  int height = 800;
  std::vector<Field*> selected;
  int operation = -1;
  std::string answer = "";
  std::string answer_sec = "";
  double coord = 0;
  sf::RenderWindow window(sf::VideoMode(width, height), "Polinomials");
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
    }
    if (operation == 2) {
    
    } else if (operation == 3 && selected.size() == 1) {
      std::stringstream stream;
      stream << std::fixed << std::setprecision(2)
             << button_func_click(selected[0]->pol, selected[0]->var);
      answer = stream.str();
      selected[0]->SetColorText(false);
      selected.clear();
      operation = -1;
      coord = 0;
    } else if (operation == 4 && selected.size() == 2) {
      bool ans = Compare(selected[0]->pol, selected[1]->pol);
      if (ans)
        answer = "TRUE";
      else
        answer = "FALSE";
     
      button_comp.state = 0;
      selected[0]->SetColorText(false);
      selected[1]->SetColorText(false);
      selected.clear();
      operation = -1;
      coord = 0;
    } else if (operation == 5 && selected.size() == 2) {
      answer = Plus(selected[0]->pol, selected[1]->pol);
      button_plus.state = 0;
      selected[0]->SetColorText(false);
      selected[1]->SetColorText(false);
      selected.clear();
      operation = -1;
      button_add.state = 1;
      coord = 0;
    } else if (operation == 6 && selected.size() == 2) {
      answer = Mult(selected[0]->pol, selected[1]->pol);
      button_mult.state = 0;
      selected[0]->SetColorText(false);
      selected[1]->SetColorText(false);
      selected.clear();
      operation = -1;
      button_add.state = 1;
      coord = 0;
    } else if (operation == 7 && selected.size() == 1) {
      answer = button_deriv_click(selected[0]->pol, selected[0]->var, window);
      selected[0]->SetColorText(false);
      selected.clear();
      operation = -1;
      button_add.state = 1;
    } else if (operation == 8 && selected.size() == 2) {
       if (selected[0]->var.size() > 1) {
        Information_window(
            "", "ERROR: there is more than 1 variable in the FIRST polynomial");
        operation = 10;
       } else if (selected[1]->var.size() > 1) {
         Information_window(
             "",
             "ERROR: there is more than 1 variable in the SECOND polynomial");
         operation = 10;
       }
       // ошибка если разные переменные
       else {
         button_div.state = 0;
         std::pair<std::string, std::string> ret =
             Div(selected[0]->pol, selected[1]->pol);
         answer = ret.first;
         answer_sec = ret.second;
         selected[0]->SetColorText(false);
         selected[1]->SetColorText(false);
         selected.clear();
         operation = -1;
       }
    } else if (operation == 9) {
      if (button_add.state == 1) {
        if (answer != "") {
          AddToBase(base, base_end, answer);
          printpol.clear();
          UpdatePolinomials(base);
          answer = "";
          button_add.state = 0;
        }
      }
      operation = -1;
      coord = 0;
    }
    if (operation == 10) {
      answer = "";
      if (selected.size() > 0)
        selected[0]->SetColorText(false);
      else if (selected.size() == 2)
        selected[1]->SetColorText(false);
      selected.clear();
      button_comp.state = 0;
      button_del.state = 0;
      button_div.state = 0;
      button_mult.state = 0;
      button_plus.state = 0;
      button_add.state = 0;
      operation = -1;
      coord = 0;
    }
    window.clear(sf::Color(255, 255, 255, 0));
    Interface(window);
    PrintPolinomials(window);
    field_out.PrintAnswer(window, answer, 0);
    if (answer_sec.length() != 0) {
      field_out.PrintAnswer(window, answer_sec, 20);
    }
    window.display();
  }

  return EXIT_SUCCESS;
}