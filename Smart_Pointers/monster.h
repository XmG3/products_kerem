#ifndef MONSTER_H
#define MONSTER_H

#include<iostream>
#include<stdexcept>
#include<string>
using namespace std;

class Monster {
private:
    string name;
    unsigned health;
    unsigned attack;

public:
    Monster(string name, unsigned health, unsigned attack);

    virtual ~Monster() = default;

    virtual unsigned calculate_damage(unsigned dmg) const = 0; 

    void take_damage(unsigned dmg);
    
    bool is_dead() const;

    unsigned get_attack() const;

    virtual string additional_information() const = 0;

    friend ostream& operator<< (ostream& o, const Monster& m);
        
};

class Elite_Monster : public Monster {
private:
    unsigned defense;

public:
Elite_Monster(string name, unsigned health, unsigned attack, unsigned defense);

unsigned calculate_damage(unsigned dmg) const override;

string additional_information() const override;

};

class Standard_Monster : public Monster {
public:
Standard_Monster(string name, unsigned health, unsigned attack);

unsigned calculate_damage(unsigned dmg) const override;

string additional_information() const override;

};

#endif