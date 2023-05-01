#pragma once
#include <string>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "Back.h"

class Button {
 private:
  double x, y;
  int heigth, width;
  sf::Color color;
  std::string text;

 public:
  int state = 0;
  // 0 - неактивно
  // 1 - активно
  // 2 - выбрано
  Button(double input_x, double input_y, int input_heigth, int input_width);
  void SetButton(double input_x, double input_y, int input_heigth,
                 int input_width);
  void SetColor(sf::Color input_color);
  void SetText(std::string input_text, sf::RenderWindow &window, int text_r, int text_g, int text_b);
  void DrawButton(sf::RenderWindow &window);
  bool OnClicked(sf::Event &event);
};


class Field {
  private:
    double x, y;
    int heigth, width;
    int r, g, b;
    double frame;
    int frame_r, frame_g, frame_b;
    bool color_text = false;
    
  public:
    node_pol *pol;
    bool clicked;
    std::vector<bool> var;
    std::vector<std::string> text;
    Field(double input_x, double input_y, int input_heigth, int input_width, bool input_clicked);
    void SetField(double input_x, double input_y, int input_heigth, int input_width, bool input_clicked);
    void SetColor(int input_r, int input_g, int input_b);
    void SetFrame(int input_frame, int input_frame_r, int input_frame_g, int input_frame_b);
    void DrawField(sf::RenderWindow &window);
    bool OnClicked(sf::Event &event);
    void SetText(std::string input_text);
    void SetColorText(bool input_color);
    void DrawPolinomial(sf::RenderWindow &window, double &coord);
    void PrintAnswer(sf::RenderWindow &window, std::string str, double coord);

};

void Interface(sf::RenderWindow &window);
void PrintText(sf::RenderWindow &window, double x, double y, std::string str, int r, int g, int b);
void Interface_enter(sf::RenderWindow &window, std::string input);
void Information_window(std::string input, std::string error);
void PrintPolinomials(sf::RenderWindow &window);
void UpdatePolinomials(node_list *&L);

void Interface_func(sf::RenderWindow &window, int symb, std::string input);
void CreateFields(std::vector<bool>v);
void Interface_deriv(sf::RenderWindow &window);

void Interface_file(sf::RenderWindow &window, std::string input);
void Interface_numder(sf::RenderWindow &window, std::string input);

