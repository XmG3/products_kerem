#include "hero_info.h"
#include "hero.h"
#include <iostream>
#include <string>

ostream& operator<<(ostream& o, Hero_Class hc){
    switch (hc){
        case Hero_Class::BARBARIAN: o << "Barbarian"; break;
        case Hero_Class::BARD: o << "Bard"; break;
        case Hero_Class::CLERIC: o << "Cleric"; break;
        case Hero_Class::DRUID: o << "Druid"; break;
        case Hero_Class::FIGHTER: o << "Fighter"; break;
        case Hero_Class::MONK: o << "Monk"; break;
        case Hero_Class::PALADIN: o << "Paladin"; break;
        case Hero_Class::RANGER: o << "Ranger"; break;
        case Hero_Class::ROGUE: o << "Rogue"; break;
        case Hero_Class::SORCERER: o << "Sorcerer"; break;
        case Hero_Class::WARLOCK: o << "Warlock"; break;
        case Hero_Class::WIZARD: o << "Wizard"; break;
    }
    return o;
}
ostream& operator<<(ostream& o, Hero_Species hs){
    switch (hs){
        case Hero_Species::DRAGONBORN: o << "Dragonborn"; break;
        case Hero_Species::DWARF: o << "Dwarf"; break;
        case Hero_Species::ELF: o << "Elf"; break;
        case Hero_Species::GNOME: o << "Gnome"; break;
        case Hero_Species::AASIMAR: o << "Aasimar"; break;
        case Hero_Species::HALFLING: o << "Halfling"; break;
        case Hero_Species::ORC: o << "Orc"; break;
        case Hero_Species::GOLIATH: o << "Goliath"; break;
        case Hero_Species::HUMAN: o << "Human"; break;
        case Hero_Species::TIEFLING: o << "Tiefling"; break;
    }
    return o;
}