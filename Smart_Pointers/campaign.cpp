#include "hero.h"
#include "monster.h"
#include "campaign.h"
#include <map>
#include <memory>

using namespace std;

Campaign::Campaign(string name, unsigned min_level) : name(name), min_level(min_level) {
        if(name.empty()|| min_level < 1) {throw runtime_error("Name cant be empty.");}   
    }

void Campaign::add_hero(shared_ptr<Hero> h) {
    auto it = heroes.find(h->get_id());
    if (it != heroes.end()) {
        throw runtime_error("Hero already in campaign.");
    } else if (h->get_level() < min_level) {
        throw runtime_error("Hero level too low.");
    }
    if (h->is_in_campaign()) {
        throw runtime_error("Hero already in another campaign.");
    }
    heroes[h->get_id()] = h; 
    h ->join_campaign(this);
}

void Campaign::remove_hero(unsigned id) {
        heroes.erase(id);
    }

void Campaign::encounter_monster(unsigned id, Monster& m) {
        if(heroes.find(id) == heroes.end()) {
            throw runtime_error("hero doesnt exist.");
        }
        auto it = heroes.find(id);
        auto hero = it->second.lock();

        if (!hero) {
            throw runtime_error("Hero reference is no longer valid.");
        }
        if (hero->fight(m)) {
            hero->level_up(name);
        }
    }

unsigned Campaign::remove_dead_and_expired() {
    unsigned count = 0;
    for (auto it  = heroes.begin(); it != heroes.end();) {
        if(it->second.expired()) {
            it = heroes.erase(it);
            ++count;
        } else if(auto hero = it-> second.lock(); hero && hero -> get_current_hp() == 0) {
            hero->leave_campaign();
            it = heroes.erase(it);
            ++count;
        } else {
            ++it;
            }
        }
    return count;
    }

unsigned Campaign::get_min_level() const {return min_level;}
string Campaign::get_name() const {return name;}

ostream& operator<<(ostream& o, const Campaign& p) {
        o<< "[" << p.name << " Campaign, " <<"Min_Level " <<  p.min_level << ", {";
        bool first = true; 
        for(const auto& [id, weak_hero] : p.heroes) {
            if(auto hero = weak_hero.lock()) {
                if(!first) {
                    o << ", ";
                }
                o << *hero;
                first = false;
                }
            }
            o << "}]";
            return o;
        }