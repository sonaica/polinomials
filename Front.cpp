#include "Front.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <string>
#include <sstream>
#include <iomanip>
#include "Back.h"

//width - 1200
//height - 800


//BUTTON

void PrintText(sf::RenderWindow& window, double x, double y, std::string str,
               int r, int g, int b) {
  sf::Font font;
  const int size = 18;
  if (font.loadFromFile("DroidSansMono.ttf")) {
    sf::Text text;
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(size);
    text.setFillColor(sf::Color(r, g, b));
    // text.setStyle(sf::Text::Bold);
    text.move(x, y);
    window.draw(text);
  }
  return;
}

void Button::SetButton(double input_x, double input_y, int input_width,
                       int input_heigth) {
  x = input_x;
  y = input_y;
  heigth = input_heigth;
  width = input_width;
  return;
}

Button::Button(double input_x, double input_y, int input_width,
               int input_heigth) {
  SetButton(input_x, input_y, input_width, input_heigth);
}

void Button::SetColor(int input_r, int input_g, int input_b) {
  r = input_r;
  g = input_g;
  b = input_b;
  return;
}

void Button::SetText(std::string input_text, sf::RenderWindow& window,
                     int text_r, int text_g, int text_b) {
  text = input_text;
  double text_length = text.length() * 11;
  double text_x = x + (width - text_length) / 2;
  double text_y = y + (heigth - 18) / 2;
  PrintText(window, text_x, text_y, text, text_r, text_g, text_b);
  return;
}

void Button::DrawButton(sf::RenderWindow& window) {
  sf::RectangleShape shape(sf::Vector2f(width, heigth));
  shape.setFillColor(sf::Color(r, g, b));
  shape.move(x, y);
  window.draw(shape);
  return;
}

bool Button::OnClicked(sf::Event& event) {
  if (event.type == sf::Event::MouseButtonPressed) {
    if (event.mouseButton.button == sf::Mouse::Left) {
      double mouse_x = event.mouseButton.x;
      double mouse_y = event.mouseButton.y;
      if (mouse_x >= x && mouse_x <= x + width && mouse_y >= y &&
          mouse_y <= y + heigth) {
        return true;
      }
    }
  }
  return false;
}

//FIELD

void Field::SetField(double input_x, double input_y, int input_width,
  int input_heigth, bool input_clicked) {
  x = input_x;
  y = input_y;
  heigth = input_heigth;
  width = input_width;
  clicked = input_clicked;
  return;
}

Field::Field(double input_x, double input_y, int input_width,
             int input_heigth, bool input_clicked) {
  SetField(input_x, input_y, input_width, input_heigth, input_clicked);
  return;
}


void Field::SetColor(int input_r, int input_g, int input_b) {
  r = input_r;
  g = input_g;
  b = input_b;
  return;
}

void Field::SetFrame(int input_frame, int input_frame_r, int input_frame_g,
                     int input_frame_b) {
  frame = input_frame;
  frame_r = input_frame_r;
  frame_g = input_frame_g;
  frame_b = input_frame_b;
  return;
}

void Field::DrawField(sf::RenderWindow& window, double &coord_y) {
  if (text.size() != 0) {
    int count = (width - 4) / 11;
    while (text[text.size() - 1].length() > count) {
      std::string push = text[text.size() - 1].substr(count);
      text[text.size() - 1] = text[text.size() - 1].substr(0, count);
      text.push_back(push);
    }
    heigth = 20 * text.size();
  }

  int frame_heigth = heigth + 2 * frame;
  int frame_width = width + 2 * frame;
  double frame_x = x - frame;
  double frame_y = y - frame;
  sf::RectangleShape shape_frame(sf::Vector2f(frame_width, frame_heigth));
  shape_frame.setFillColor(sf::Color(frame_r, frame_g, frame_b));
  shape_frame.move(frame_x, frame_y);
  window.draw(shape_frame);

  sf::RectangleShape shape(sf::Vector2f(width, heigth));
  shape.setFillColor(sf::Color(r, g, b));
  shape.move(x, y);
  window.draw(shape);

  int text_r = 0;
  if (color_text) {
    text_r = 255;
  }
  for (int i = 0; i < text.size(); ++i) {
    PrintText(window, 52, coord_y, text[i], text_r, 0, 0);
    coord_y += 20;
  }
  return;
}

void Field::SetText(std::string input_text) {
  text.clear();
  text.push_back(input_text);
  return;
}

void Field::SetColorText(bool input_color) {
  color_text = input_color;
  return;
}

bool Field::OnClicked(sf::Event& event) {
  if (event.type == sf::Event::MouseButtonPressed) {
    if (event.mouseButton.button == sf::Mouse::Left) {
      double mouse_x = event.mouseButton.x;
      double mouse_y = event.mouseButton.y;
      if (mouse_x >= x && mouse_x <= x + width && mouse_y >= y &&
          mouse_y <= y + heigth) {
        return true;
      }
    }
  }
  return false;
}

//INTERFACE
Button button_enter(600, 50, 100, 60);
Button button_del(600, 160, 100, 60);
Button button_func(600, 270, 100, 60);
Button button_comp(900, 160, 100, 60);
Button button_plus(750, 50, 100, 60);
Button button_mult(750, 160, 100, 60);
Button button_deriv(750, 270, 100, 60);
Button button_div(900, 270, 100, 60);
Button button_roots(900, 50, 100, 60);
Button button_save(1050, 50, 100, 60);
Button button_load(1050, 160, 100, 60);

Button button_add(750, 690, 100, 60);
Button button_clear(900, 690, 100, 60);

void Interface(sf::RenderWindow& window) { 

  button_enter.SetColor(192, 192, 192);
  button_enter.DrawButton(window);
  button_enter.SetText("enter", window, 0, 0, 0);

  button_del.SetColor(192, 192, 192);
  button_del.DrawButton(window);
  button_del.SetText("del", window, 0, 0, 0);

  button_func.SetColor(192, 192, 192);
  button_func.DrawButton(window);
  button_func.SetText("f()", window, 0, 0, 0);

  button_comp.SetColor(192, 192, 192);
  button_comp.DrawButton(window);
  button_comp.SetText("compare", window, 0, 0, 0);

  button_plus.SetColor(192, 192, 192);
  button_plus.DrawButton(window);
  button_plus.SetText("+", window, 0, 0, 0);

  button_mult.SetColor(192, 192, 192);
  button_mult.DrawButton(window);
  button_mult.SetText("*", window, 0, 0, 0);

  button_deriv.SetColor(192, 192, 192);
  button_deriv.DrawButton(window);
  button_deriv.SetText("f'", window, 0, 0, 0);

  button_div.SetColor(192, 192, 192);
  button_div.DrawButton(window);
  button_div.SetText("/", window, 0, 0, 0);

  button_roots.SetColor(192, 192, 192);
  button_roots.DrawButton(window);
  button_roots.SetText("roots", window, 0, 0, 0);

  button_save.SetColor(192, 192, 192);
  button_save.DrawButton(window);
  button_save.SetText("save", window, 0, 0, 0);

  button_load.SetColor(192, 192, 192);
  button_load.DrawButton(window);
  button_load.SetText("load", window, 0, 0, 0);

  button_add.SetColor(105, 105, 105);
  button_add.DrawButton(window);
  button_add.SetText("ADD", window, 255, 255, 255);

  button_clear.SetColor(105, 105, 105);
  button_clear.DrawButton(window);
  button_clear.SetText("CLEAR", window, 255, 255, 255);

  double coord_y = 0;
  Field field_in(50, 50, 500, 700, false);
  field_in.SetColor(255, 255, 255);
  field_in.SetFrame(2, 105, 105, 105);
  field_in.DrawField(window, coord_y);

  Field field_out(600, 380, 550, 260, false);
  field_out.SetColor(255, 255, 255);
  field_out.SetFrame(2, 105, 105, 105);
  field_out.DrawField(window, coord_y);

  return;
}

Field field_enter(50, 100, 500, 50, false);
Button button_enter_ok(250, 200, 100, 60);

void Interface_enter(sf::RenderWindow& window, std::string input) { 
  field_enter.SetFrame(2, 105, 105, 105);
  if (field_enter.clicked) {
    field_enter.SetColor(211, 211, 211);
  }
  else {
    field_enter.SetColor(255, 255, 255);
  }
  double coord_y = 0;
  field_enter.DrawField(window, coord_y);
  PrintText(window, 50, 32, "Enter a polinomial:", 0, 0, 0);
  PrintText(window, 52, 116, input, 0, 0, 0);

  button_enter_ok.SetColor(192, 192, 192);
  button_enter_ok.DrawButton(window);
  button_enter_ok.SetText("OK", window, 0, 0, 0);

  return;
}

void Information_window(std::string input, std::string error) {
  int width = 600;
  int height = 300;
  std::vector<std::string> errors;
  int count = 500 / 11;
  while (error.length() * 11 > 500) {
    std::string temp = error.substr(0, count);
    error = error.substr(count);
    errors.push_back(temp);
  }
  errors.push_back(error);
  Button information_ok(250, 200, 100, 60);
  sf::RenderWindow w(sf::VideoMode(width, height), "ERROR");
  while (w.isOpen()) {
    sf::Event event;
    while (w.pollEvent(event)) {
      if (event.type == sf::Event::Closed) w.close();
    }
    w.clear(sf::Color(255, 255, 255, 0));
    PrintText(w, 50, 50, input, 0, 0, 0);
    int j = 0;
    for (int i = 0; i < errors.size(); ++i) {
      PrintText(w, 50, 70 + 20 * j, errors[i], 255, 0, 0);
      ++j;
      for (int k = 0; k < errors[i].length(); ++k) {
        if (errors[i][k] == '\n') ++j;
      }
    }
    w.display();
  }
  return;
}

std::vector<Field> printpol;

void UpdatePolinomials(node_list*&L) { 
  int k = 0;
  for (node_list* p = L; p != nullptr; p = p->next) {
    std::string pol_str = "";
    for (node_pol* q = p->pol; q != nullptr; q = q->next) {
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
    Field field(50, 50 + k * 20, 500, 0, false);
    field.SetColor(255, 255, 255);
    field.SetText(pol_str);
    printpol.push_back(field);
    ++k;
  }
  return;
}

void PrintPolinomials(sf::RenderWindow& window) {
  double coord_y = 51;
  for (int i = 0; i < printpol.size(); ++i) {
    printpol[i].DrawField(window, coord_y);
  }
  return;
}
