#include "hero.h"
#include "monster.h"
#include "campaign.h"
using namespace std;


Monster::Monster(string name, unsigned health, unsigned attack) : name(name), health(health), attack(attack) {
        if(name.empty() || health == 0 || attack == 0) {
            throw runtime_error("Name, attack or health cannot be zero.");
        } 
    }

void Monster::take_damage(unsigned dmg) {
        if(dmg > health){health = 0;} 
        else {health -= dmg;}
        }

bool Monster::is_dead() const {
        return health == 0;
    }

unsigned Monster::get_attack() const {
    return attack;
}

ostream& operator<<(ostream& o, const Monster& m) {
        o << "[" << m.name << ", " << m.health << "HP, " << m.attack << " ATK " << m.additional_information() << "]";
        return o;
    }

Elite_Monster::Elite_Monster(string name, unsigned health, unsigned attack, unsigned defense) : Monster(name, health, attack), defense(defense) {
    if(defense == 0) {
        throw runtime_error("defense cant be zero");
    }
}

unsigned Elite_Monster::calculate_damage(unsigned dmg) const  {
    unsigned new_dmg = (dmg > defense) ? (dmg - defense) : 0;
    return new_dmg;
}

string Elite_Monster::additional_information() const{
    return ", defense " +  to_string(defense);
}

Standard_Monster::Standard_Monster(string name, unsigned health, unsigned attack) : Monster(name, health, attack) {}

unsigned Standard_Monster::calculate_damage(unsigned dmg) const {
    return dmg;
}

string Standard_Monster::additional_information() const{
    return "";
}