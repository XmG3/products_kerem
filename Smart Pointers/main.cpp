#include <iostream>
#include "Player.h"
#include "Hero.h"
#include "Campaign.h"
#include "Monster.h"

int main() {

    //create player
    Player player("Kerem", "Güc"); 
    cout << player << endl; 

    //create campaign
    player.create_campaign("Campaign1", 1, 0); 

    //creating heroes
    player.create_hero("Jon Snow", Hero_Class::FIGHTER, Hero_Species::HUMAN, 100, {{"Strength", 15}, {"Dexterity", 18}, {"Constitution", 13}, {"Intelligence", 11}, {"Wisdom", 10}, {"Charisma", 16}});
    player.create_hero("Illidan Stormrage", Hero_Class::SORCERER, Hero_Species::ELF, 120, {{"Strength", 16}, {"Dexterity", 16}, {"Constitution", 12}, {"Intelligence", 10}, {"Wisdom", 12}, {"Charisma", 14}});



}