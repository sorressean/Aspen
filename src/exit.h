/*
*A basic exit object;
*Can be inherited to create doors, etc.
*/
#ifndef EXIT_H
#define EXIT_H
#include <string>
#include <tinyxml.h>
#include "mud.h"
#include "conf.h"
#include "living.h"

class Exit
{
    VNUM _to; //the vnum that the exit points to.
    ExitDirection _direction;
public:
    Exit(VNUM to);
    Exit();
    virtual ~Exit();

//getters and setters:
    virtual VNUM GetTo() const;
    virtual void SetTo(VNUM to);
    virtual ExitDirection GetDirection() const;
    virtual void SetDirection(ExitDirection dir);
    /*
    *Translates the direction to a human-readable name.
    *Return: The name of the direction for the exit.
    */
    std::string GetName() const;
    /*
    *Checks to see if the npc/player can enter through the exit.
    *Return: true if the player/npc can enter, false otherwise.
    */
    virtual BOOL CanEnter(Living* mobile);

//serialization
    virtual void Serialize(TiXmlElement* root);
    virtual void Deserialize(TiXmlElement* node);
};

#endif
