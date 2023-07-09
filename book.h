/*
Class header for Order Book
*/

#include <unordered_map>
#include <queue>
#include <set>
#include <iostream>
class Book
{
private:
    std::set<double> buys;
    std::set<double, std::greater<double> > sells;
    std::unordered_map<double, int> buys_quantity;
    std::unordered_map<double, int> sells_quantity;
public:
    Book();
    void post_buy_LO(double price, int shares);
    void post_sell_LO(double price, int shares);
    void post_buy_MO(int shares);
    void post_sell_MO(int shares);
    double get_spread();
    double get_depth();
    void update();
    void show_book();
};