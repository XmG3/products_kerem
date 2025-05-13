#pragma once
#include<iostream>
#include<stdexcept>
#include<string>
#include<vector>
#include <map>
#include <algorithm>
#include <memory>
#include "hero.h"
using namespace std;

class Campaign {
private:
    string name;
    map <unsigned, weak_ptr<Hero>> heroes;
    unsigned min_level;

public: 
    Campaign(string name, unsigned min_level);

    void add_hero(shared_ptr<Hero> h);
    
    void encounter_monster(unsigned id, Monster& m);

    unsigned remove_dead_and_expired();

    unsigned get_min_level() const;
    
    friend ostream& operator<<(ostream& o, const Campaign& p);

};

     
    