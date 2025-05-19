#ifndef HERO_INFO_H
#define HERO_INFO_H

#include <iostream>
using namespace std;

enum class Hero_Class{BARBARIAN, BARD, CLERIC, DRUID, FIGHTER, MONK, PALADIN, RANGER, ROGUE, SORCERER, WARLOCK, WIZARD};
enum class Hero_Species{DRAGONBORN, DWARF, ELF, GNOME, AASIMAR, HALFLING, ORC, GOLIATH, HUMAN, TIEFLING};

ostream& operator<<(ostream& o, Hero_Class hc);
ostream& operator<<(ostream& o, Hero_Species hs);

#endif