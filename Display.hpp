#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <string>
#include <vector>

char read_char();

class DisplayI
{
  public:
    virtual void pop_line() = 0;
    virtual void add_line(const std::string &s) = 0;
    virtual void add_char(char ch) = 0;
    virtual void remove_last_char() = 0;
    virtual ~DisplayI() {};
};


class Display: public DisplayI
{
  public:
    Display();

    void pop_line() override;
    void add_line(const std::string &s) override;
    void add_char(char ch) override;
    void remove_last_char() override;
};

#endif
