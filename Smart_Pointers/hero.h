#ifndef HERO_H
#define HERO_H

#include<iostream>
#include<stdexcept>
#include<string>
#include<vector>
#include <map>
#include "hero_info.h"
using namespace std;

class Monster;
class Campaign;

class Hero {
private: 
    unsigned id;
    string name;
    Hero_Class hero_class;
    Hero_Species hero_species;
    unsigned level;
    unsigned max_hp;
    unsigned current_hp;
    map <string, unsigned> abilities;
    void set_map(const map<string, unsigned>& abilities);
    static unsigned next_id;

    Campaign* current_campaign;
    map<string, unsigned> logbook;

public:
    Hero(string name, Hero_Class hero_class, Hero_Species hero_species, unsigned max_hp, const map<string, unsigned> abilities);

    unsigned level_up();
    unsigned level_up(const string& campaign_name);
    
    //campaign functions
    void join_campaign(Campaign* campaign);
    void leave_campaign();
    bool is_in_campaign() const;
    Campaign* get_current_campaign() const;

    //logbook
    ostream& log(ostream& o) const;

    bool fight(Monster& m);


    //getters
    unsigned get_id() const;
    unsigned get_level() const;
    unsigned get_current_hp() const;
    unsigned get_max_hp() const;

    friend ostream& operator<<(ostream& o, const Hero& obj);

};

#endif