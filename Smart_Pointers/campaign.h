#ifndef CAMPAIGN_H
#define CAMPAIGN_H

#include<iostream>
#include<stdexcept>
#include<string>
#include<vector>
#include <map>
#include <algorithm>
#include <memory>
using namespace std;

class Hero;
class Monster;

class Campaign {
private:
    string name;
    map <unsigned, weak_ptr<Hero>> heroes;
    unsigned min_level;

public: 
    Campaign(string name, unsigned min_level);

    void add_hero(shared_ptr<Hero> h);
    void remove_hero(unsigned id);
    
    void encounter_monster(unsigned id, Monster& m);

    unsigned remove_dead_and_expired();

    //getters
    unsigned get_min_level() const;
    string get_name() const;
    
    friend ostream& operator<<(ostream& o, const Campaign& p);

};
 
#endif    