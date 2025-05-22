#include "hero.h"
#include "monster.h"
#include "campaign.h"
using namespace std;

unsigned Hero::next_id = 0;

Hero::Hero(string name, Hero_Class hero_class, Hero_Species hero_species, unsigned max_hp, map<string, unsigned> abilities): name(name), hero_class(hero_class), hero_species(hero_species), max_hp(max_hp), abilities(abilities){
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
    static const vector <string> keys = {"Strength", "Dexterity", "Constitution", "Intelligence", "Wisdom", "Charisma"};
    for(const auto& key : keys) {
      if (abilities.find(key) == abilities.end()) 
        throw runtime_error("missing ability score");
      unsigned val = abilities.at(key);
      if (val < 1 || val > 20)
        throw runtime_error("ability score invalid"); 
    }   
  }

unsigned Hero::level_up(const string& campaign_name) {
    unsigned old_level = level;
    level_up();
    if (level > old_level) {
        // Log the level gained in this campaign
        logbook[campaign_name]++;
    }
    return level;
}
    

void Hero::join_campaign(Campaign* campaign) {
    current_campaign = campaign;
}

void Hero::leave_campaign() {
    current_campaign = nullptr;
}

bool Hero::is_in_campaign() const {
    return current_campaign != nullptr;
}

Campaign* Hero::get_current_campaign() const {
    return current_campaign;
}

ostream& Hero::log(ostream& o) const {
    o << "Logbook of " << name << " (ID: " << id << "):" << endl;
    if (logbook.empty()) {
        o << "  No entries yet." << endl;
    } else {
        for (const auto& [campaign_name, levels_gained] : logbook) {
            o << "  " << campaign_name << ": " << levels_gained << " levels gained" << endl;
        }
    }
    return o;
}

bool Hero::fight(Monster& m) {

    if(current_hp == 0) {
        return false; 
    }
    if(m.is_dead()) {
        return true; 
    }

  
    auto max_ability = max_element(abilities.begin(), abilities.end(), 
        [](const pair<string, unsigned>& a, const pair<string, unsigned>& b) {
            return a.second < b.second;
        });
    unsigned hero_damage = level * max_ability->second;
    
    while(current_hp > 0 && !m.is_dead()) {
        // Hero attacks first
        m.take_damage(hero_damage);
        
        // If monster survived, it attacks back
        if(!m.is_dead()) {
            unsigned monster_damage = m.get_attack();
            if(monster_damage >= current_hp) {
                current_hp = 0;
            } else {
                current_hp -= monster_damage;
            }
        }
    }
    return current_hp > 0;
}

    unsigned Hero::get_id() const {return id;}
    unsigned Hero::get_level() const {return level;}
    unsigned Hero::get_current_hp() const {return current_hp;}
    unsigned Hero::get_max_hp() const {return max_hp;}

ostream& operator<<(ostream& o, const Hero& obj) { 
    o << "[" << obj.id << ", " << obj.name << ", (" << obj.hero_class << ", " << obj.hero_species << ", " << obj.level << "), {" 
    << obj.abilities.at("Charisma") << ", " 
    << obj.abilities.at("Constitution") << ", " 
    << obj.abilities.at("Dexterity") << ", " 
    << obj.abilities.at("Intelligence") << ", " 
    << obj.abilities.at("Strength") << ", " 
    << obj.abilities.at("Wisdom") << "}, (" 
    << obj.current_hp << "/" << obj.max_hp << ") HP]";
    return o; 
}