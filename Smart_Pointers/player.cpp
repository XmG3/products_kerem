#include "hero.h"
#include "campaign.h"
#include "monster.h"
#include "hero_info.h"
#include "player.h"
using namespace std;


Player::Player(string first_name, string last_name) : first_name(first_name), last_name(last_name) {
    if(first_name.empty() || last_name.empty()) {
        throw runtime_error("first and last name cant be empty");
    }
}

unsigned Player::create_hero(const string& name, Hero_Class hero_class, Hero_Species hero_species, unsigned max_hp, const map<string, unsigned>& abilities) {
    if(max_hp < 1){throw runtime_error("max hp too small");}
    auto new_hero = make_shared<Hero>(name, hero_class, hero_species, max_hp, abilities);
    heroes[new_hero->get_id()] = new_hero;
    return new_hero->get_id();
}

void Player::create_campaign(string name, unsigned min_level, unsigned id){
    if(campaign != nullptr) {
        campaign.reset();}
    campaign = make_unique<Campaign>(name, min_level);
    try {
        auto hero = heroes.at(id);
        if (hero->is_in_campaign()) {
            // Remove from current campaign 
            hero->leave_campaign();
        }

        campaign->add_hero(heroes.at(id));
    } catch (const out_of_range& e) {
        throw runtime_error("Hero not found in campaign.");
    }
}

void Player::join_friends_campaign(Player& pfriend, unsigned id) {
    if(pfriend.campaign == nullptr || pfriend.campaign->get_min_level() > heroes.at(id)->get_level() || this->heroes.at(id) == nullptr) {
        throw runtime_error("problem adding hero");
    } else {
        // Check if hero is already in another campaign and handle it
        auto hero = heroes.at(id);
        if (hero->is_in_campaign()) {
            // Remove from current campaign first
            hero->leave_campaign();
        }
        pfriend.campaign -> add_hero(heroes.at(id));
    }
}

void Player::transfer_campaign(Player& pfriend) {
    if(campaign == nullptr) {
        throw runtime_error("no campaign");
    } else {
        pfriend.campaign = move(campaign);
        campaign = nullptr;
        }
    }

unsigned Player::remove_dead() {
    if (campaign) {
        return campaign->remove_dead_and_expired();
    }
    return 0;
}

shared_ptr<Hero> Player::get_hero(unsigned id) {
    auto it = heroes.find(id);
    if (it != heroes.end()) {
        return it->second;
    }
    return nullptr;
}

void Player::encounter_monster_in_campaign(unsigned hero_id, Monster& monster) {
    if (campaign) {
        campaign->encounter_monster(hero_id, monster);
    } else {
        throw runtime_error("No campaign available");
    }
}

ostream& Player::print_campaign(ostream& o) const {
    if(campaign != nullptr) {
        o << *campaign;
    } else {
        o<< "[]";
    }
    return o;
    }

ostream& operator<<(ostream& o, const Player& p){
    o << "[" << p.first_name << " " << p.last_name << ", {";
    bool first = true; 
    for(const auto& [id, hero_ptr] : p.heroes) {
        if(hero_ptr) {
            if(!first) {
                o << ", ";
            }
            o << *hero_ptr;
            first = false;
        }
    }
    o << "}, "; 
    p.print_campaign(o);
    o << "]";
    return o;
}