#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "Front.h"
#include "Back.h"

// проблема с кнопкой в информационном окне
// нормальный перенос строки в информационном окне
// хранение числителя и знаменателя
// вылезание за строку поля 

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
           UpdatePolinomials(base);
           w.close();
         }
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
  sf::RenderWindow window(sf::VideoMode(width, height), "Polinomials");
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();
      if (button_enter.OnClicked(event)) {
        button_enter_click();
      }
      if (button_del.OnClicked(event)) {

      }
      if (button_func.OnClicked(event)) {

      }
      if (button_comp.OnClicked(event)) {

      }
      if (button_plus.OnClicked(event)) {

      }
      if (button_mult.OnClicked(event)) {

      }
      if (button_deriv.OnClicked(event)) {

      }
      if (button_div.OnClicked(event)) {

      } 
      if (button_add.OnClicked(event)) {

      }
      if (button_clear.OnClicked(event)) {

      }
      for (int i = 0; i < printpol.size(); ++i) {
        if (printpol[i].OnClicked(event)) {
          printpol[i].SetColorText(true);
        }
      }
    }
    window.clear(sf::Color(255, 255, 255, 0));
    Interface(window);
    PrintPolinomials(window);
    window.display();
  }

  return EXIT_SUCCESS;
}