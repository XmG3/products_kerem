#include <iostream>
#include "player.h"
#include "hero.h"
#include "campaign.h"
#include "monster.h"
#include "hero_info.h"
#include <map>
using namespace std;

int main() {
    cout << "Testing 1.8" << endl;
    
    try {

    // Create players
    Player player1("Kerem", "Guc");
    cout << player1 << endl;
    Player player2("Brad", "Pitt");
    cout << player2 << endl;
    
    // Create heroes
    map<string, unsigned> abilities1 = {
        {"Strength", 12}, {"Dexterity", 17}, {"Constitution", 15}, 
        {"Intelligence", 12}, {"Wisdom", 11}, {"Charisma", 12}
    };

    map<string, unsigned> abilities2 = {
        {"Strength", 12}, {"Dexterity", 16}, {"Constitution", 12}, 
        {"Intelligence", 14}, {"Wisdom", 8}, {"Charisma", 18}
    };
    
    unsigned hero1_id = player1.create_hero("Aragorn", Hero_Class::RANGER, Hero_Species::HUMAN, 100, abilities1);
    unsigned hero2_id = player1.create_hero("Illidan", Hero_Class::SORCERER, Hero_Species::ELF, 90, abilities2);
    unsigned hero3_id = player2.create_hero("Legolas", Hero_Class::RANGER, Hero_Species::ELF, 100, abilities1);
        
        cout << "TEST 1: Creating heroes" << endl;
        cout << "Player 1: " << player1 << endl;
        cout << "Player 2: " << player2 << endl;
        
        // Test 2
        cout << "\n TEST 2: Campaign participation" << endl;
        player1.create_campaign("Fellowship Quest", 1, hero1_id);
        cout << "After creating campaign: " << player1 << endl;
        
       
        try {
            player2.create_campaign("Rohan Defense", 1, hero3_id);
            player1.join_friends_campaign(player2, hero1_id); 
            cout << "After joining friend's campaign: " << player1 << endl;
            cout << "Friend's campaign: " << player2 << endl;
        } catch (const exception& e) {
            cout << "Expected constraint handling: " << e.what() << endl;
        }
        
        // Test 3
        cout << "\n TEST 3: Hero leaving campaign" << endl;
        auto hero = player1.get_hero(hero1_id);
        if (hero) {
            hero->leave_campaign();
            cout << "After hero leaves campaign: " << player1 << endl;
            cout << "Friend's campaign after hero leaves: " << player2 << endl;
        }
        
        // Test 4
        cout << "\n TEST 4: Level up logging" << endl;
        player1.create_campaign("Moria Expedition", 1, hero2_id);
        
        // Create monsters and fight
        Standard_Monster orc("Orc Warrior", 40, 8);
        Elite_Monster troll("Cave Troll", 80, 15, 5);
        
        cout << "Before fighting: " << player1 << endl;
        
        player1.encounter_monster_in_campaign(hero2_id, orc);
        cout << "After fighting orc: " << player1 << endl;
        
        player1.encounter_monster_in_campaign(hero2_id, troll);
        cout << "After fighting troll: " << player1 << endl;
        
        // Test 5
        cout << "\n TEST 5: Hero's logbook" << endl;
        auto gimli = player1.get_hero(hero2_id);
        if (gimli) {
            gimli->log(cout);
        }
        
        // Test 6
        cout << "\n TEST 6: Sequential campaign participation" << endl;
        player1.create_campaign("Helm's Deep", 2, hero2_id);
        cout << "After joining new campaign: " << player1 << endl;
        
    
        Standard_Monster uruk("Uruk-hai", 60, 12);
        player1.encounter_monster_in_campaign(hero2_id, uruk);
        cout << "After fighting in new campaign: " << player1 << endl;
        
        // Print logbook
        cout << "\n Updated logbook" << endl;
        if (gimli) {
            gimli->log(cout);
        }
        
        // Test 7
        cout << "\n TEST 7: Transfer campaign" << endl;
        cout << "Before transfer - Player1: " << player1 << endl;
        cout << "Before transfer - Player2: " << player2 << endl;
        
        player1.transfer_campaign(player2);
        cout << "After transfer - Player1: " << player1 << endl;
        cout << "After transfer - Player2: " << player2 << endl;
        
        // Test 8: Remove dead heroes
        cout << "\n TEST 8: Remove dead heroes" << endl;
        // Create a very strong monster to kill the hero
        Standard_Monster dragon("Ancient Dragon", 200, 1000);
        player2.encounter_monster_in_campaign(hero2_id, dragon);
        cout << "After fighting dragon: " << player2 << endl;
        
        unsigned removed = player2.remove_dead();
        cout << "Removed " << removed << " dead heroes." << endl;
        cout << "After removing dead: " << player2 << endl;
        
        // Final logbook check
        cout << "\n Final logbook check" << endl;
        if (gimli && gimli->get_current_hp() > 0) {
            gimli->log(cout);
        } else {
            cout << "Hero is no longer available (dead or removed)." << endl;
        }
        
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
    
    return 0;
}