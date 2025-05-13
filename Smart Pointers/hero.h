#include<iostream>
#include<stdexcept>
#include<string>
#include<vector>
#include <map>
#include "monster.h"
#include "campaign.h"
#include "hero_info.h"
using namespace std;

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

public:
    Hero(string name, Hero_Class hero_class, Hero_Species hero_species, unsigned max_hp, const map<string, unsigned> abilities);

    unsigned level_up();    

    bool fight(Monster& m);

    unsigned get_id() const;
    unsigned get_level() const;

    friend ostream& operator<<(ostream& o, const Hero& obj);

};
