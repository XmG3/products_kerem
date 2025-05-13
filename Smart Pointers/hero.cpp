#include "hero.h"
#include "monster.h"
#include "campaign.h"
using namespace std;

unsigned Hero::next_id = 0;

Hero::Hero(string name, Hero_Class hero_class, Hero_Species hero_species, unsigned max_hp, map<string, unsigned> abilities): name(name), hero_class(hero_class), hero_species(hero_species), max_hp(max_hp), abilities(abilities), level(1) {
  if (name.empty()) throw runtime_error("Name cant be empty.");
  if (max_hp < 1) throw runtime_error("max hp must be positive.");
  if (abilities.size() != 6) throw runtime_error("needs 6 ability scores");
    set_map(abilities);

    id = next_id++;
    level = 1;
    current_hp = max_hp;

}

unsigned Hero::level_up() {
    if (level < 20) {
        level++;
          }
    return level;
} 

void Hero::set_map(const map<string, unsigned>& abilities) {
    static const vector <string> keys = {"STR", "DEX", "CON", "INT", "WIS", "CHA"};
    for(const auto& key : keys) {
      if (abilities.find(key) == abilities.end()) 
        throw runtime_error("missing ability score");
      unsigned val = abilities.at(key);
      if (val < 1 || val > 20)
        throw runtime_error("ability score invalid"); 
    }   
  }

bool Hero::fight(Monster& m) {
  if(current_hp == 0|| m.is_dead()) {
      throw runtime_error("One of the parties have no health to fight.");
        }

  auto max_ability = max_element(abilities.begin(), abilities.end(), [](const pair<string, unsigned>& a, const pair<string, unsigned>& b) {
    return a.second < b.second;
    });

  unsigned damage = level * max_ability -> second; 
    
    while(current_hp > 0 && !m.is_dead()) {
       m.take_damage(damage);
      if(!m.is_dead()) {
        unsigned monster_damage = m.calculate_damage(m.get_attack());
        if(monster_damage > current_hp) {current_hp = 0;}
        else {current_hp -= monster_damage;}
      }
    }
    return current_hp > 0;
    }

    unsigned Hero::get_id() const {return id;}
    unsigned Hero::get_level() const {return level;}

ostream& operator<<(ostream& o, const Hero& obj) { 
    o << "[ " << obj.id << ", (" << obj.hero_class << ", " << obj.hero_species << ", " << obj.level << "), {" 
    << "STR:" << obj.abilities.at("STR") << ", " 
    << "DEX: " << obj.abilities.at("DEX") << ", " 
    << "CON: " << obj.abilities.at("CON") << ", " 
    << "INT: " << obj.abilities.at("INT") << ", " 
    << "WIS: " << obj.abilities.at("WIS") << ", " 
    << "CHA: " << obj.abilities.at("CHA") << "}, (" 
    << obj.current_hp << "/" << obj.max_hp << ") HP]";
    return o; 
}