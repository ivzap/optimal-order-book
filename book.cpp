/*
Order Book Class Logic
*/

#include "book.h"

Book::Book(){

}

void Book::post_buy_LO(double price, int shares){
    if(buys_quantity.count(price)){
        buys_quantity[price] += shares;
        return;
    }else{
        buys_quantity[price] = shares;
    }
    buys.insert(price);
    update();
    show_book();
}

void Book::post_sell_LO(double price, int shares){
    if(sells_quantity.count(price)){
        sells_quantity[price] += shares;
    }else{
        sells_quantity[price] = shares;
    }
    sells.insert(price);
    update();
    show_book();
}

void Book::post_buy_MO(int shares){
    // Performs instantly. If shares cant be found convert to a buy LO: Edge case RARELY happens in live markets.
    if(!sells_quantity.size() || sells.size()){
        return;
    }
    double last_price = *sells.rbegin();
    while(shares && sells.size()){
        int max_sell = std::min(sells_quantity[*sells.rbegin()], shares);
        sells_quantity[*sells.rbegin()] -= max_sell;
        last_price = *sells.rbegin();
        if(!sells_quantity[*sells.rbegin()]){
            sells.erase(std::prev(sells.end()));
        }
    }
    if(shares){ // we need to make a limit order at the last seen price
        buys_quantity[last_price] += shares;
    }
    update();
    show_book();
}

void Book::post_sell_MO(int shares){
    // Performs instantly. If shares cant be found convert to a sell LO: Edge case RARELY HAPPENS IN MARKET.
    if(!buys_quantity.size() || buys.size()){
        return;
    }
    double last_price = *buys.rbegin();
    while(shares && buys.size()){
        int max_sell = std::min(buys_quantity[*buys.rbegin()], shares);
        buys_quantity[*buys.rbegin()] -= max_sell;
        last_price = *buys.rbegin();
        if(!buys_quantity[*buys.rbegin()]){
            buys.erase(std::prev(buys.end()));
        }
    }
    if(shares){ // we need to make a limit order at the last seen price
        sells_quantity[last_price] += shares;
    }
    update();
}

void Book::update(){
    while (buys.size() && sells.size() && *buys.rbegin() >= *sells.rbegin() && buys_quantity[*buys.rbegin()] && sells_quantity[*sells.rbegin()]){
        int max_sell = std::min(buys_quantity[*buys.rbegin()], sells_quantity[*sells.rbegin()]); // the max amount of shares we can sell
        sells_quantity[*sells.rbegin()] -= max_sell;
        buys_quantity[*buys.rbegin()] -= max_sell;
        if(!sells_quantity[*sells.rbegin()]){
            *sells.rbegin();
        }
        if(!buys_quantity[*buys.rbegin()]){
            buys.erase(std::prev(buys.end()));
        }
    }
}

double Book::get_spread(){
    return std::abs(*buys.rbegin() - *sells.rbegin());
}

double Book::get_depth(){
    return std::abs(*buys.begin() - *sells.begin());
}

void Book::show_book(){
    // Show the buys
    std::cout<<"Buys: ";
    for(auto it = buys.begin(); it != buys.end(); it++){
        std::cout<<"<p="<<*it<<", q="<<buys_quantity[*it]<<"> ";
    }
    std::cout<<std::endl;
    // show the sells
    std::cout<<"Sells: ";
    for(auto it = sells.begin(); it != sells.end(); it++){
        std::cout<<"<p="<<*it<<", q="<<sells_quantity[*it]<<"> ";
    }
    std::cout<<std::endl;
}