#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "Front.h"
#include "Back.h"


// нормальный перенос строки в информационном окне
// вылезание за строку поля 
// состояния кнопок
// если не было добавления в базу, то очистить память
// проверить сортировку

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


int main() {
  int width = 1200;
  int height = 800;
  std::vector<Field*> selected;
  int operation = -1;
  std::string answer = "";
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
        if (selected.size() > 0) {

        }
      }
      // 4
      if (button_comp.OnClicked(event)) {
        if (selected.size() > 0) {
          operation = 4;
          button_comp.state = 2;
        }
      }
      // 5
      if (button_plus.OnClicked(event)) {
        if (selected.size() > 0) {
          operation = 5;
          button_plus.state = 2;
        }
      }
      // 6
      if (button_mult.OnClicked(event)) {
        if (selected.size() > 0) {
          operation = 6;
          button_mult.state = 2;
        }
      }
      // 7
      if (button_deriv.OnClicked(event)) {
      }
      // 8
      if (button_div.OnClicked(event)) {

      }
      // 9
      if (button_add.OnClicked(event)) {
        operation = 9;
      }
      // 10
      if (button_clear.OnClicked(event)) {
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
    
    } else if (operation == 3) {
    
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
    } else if (operation == 5 && selected.size() == 2) {
      node_pol *ans = Plus(selected[0]->pol, selected[1]->pol);
      answer = PtrToString(ans);
      button_plus.state = 0;
      selected[0]->SetColorText(false);
      selected[1]->SetColorText(false);
      selected.clear();
      operation = -1;
      button_add.state = 1;
    } else if (operation == 6 && selected.size() == 2) {
      node_pol *ans = Mult(selected[0]->pol, selected[1]->pol);
      answer = PtrToString(ans);
      button_mult.state = 0;
      selected[0]->SetColorText(false);
      selected[1]->SetColorText(false);
      selected.clear();
      operation = -1;
      button_add.state = 1;
    } else if (operation == 7) {

    } else if (operation == 8) {
    
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
    }
    window.clear(sf::Color(255, 255, 255, 0));
    Interface(window);
    PrintPolinomials(window);
    field_out.PrintAnswer(window, answer);
    window.display();
  }

  return EXIT_SUCCESS;
}