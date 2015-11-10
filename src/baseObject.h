/**
* Contains: BaseObject
*
* The BaseObject class is responsible for holding all information needed for game objects.
* This includes rooms and entities.
*/
#pragma once
#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H
#include <tinyxml2.h>
#include <list>
#include <string>
#include "mud.h"
#include "conf.h"
#include "event.h"
#include "eventManager.h"
#include "eventargs.h"
#include "variant.h"
#include "property.h"
#include "serializer.h"
#include "command.h"

/**
* Holds information relevant to all objects.
*/
class BaseObject:public ISerializable
{
protected:
    std::vector <Component*> _components;
    std::vector<std::string> _aliases;
    VNUM _onum;
    std::string _name;
    std::string _desc;
    std::string _script; //the scripting associated with this object.
    Zone* _zone;
    BOOL _persistent;
public:
    EventManager events;
    Property variables;
    Commandable commands;

    BaseObject();
    virtual ~BaseObject();

    /**
    * Gets the name of the object.
    */
    virtual std::string GetName() const;
    /**
    * Sets the name of the object.
    * @param [in] name The name of the object.
    */
    virtual void SetName(const std::string &s);
    /**
    * Gets the description of the object.
    *
    * The description is what will be seen when a player or NPC looks at the object.
    * @return The description of the object.
    */
    virtual std::string GetDescription() const;
    /**
    * Sets the object's description.
    * @param [in] description The new description for the object.
    */
    virtual void SetDescription(const std::string &s);
    /**
    * Gets the scripting associated with this object.
    *
    * The script for the object is the angelscript code that runs when objects are initialized.
    * @return The full code for the script associated with this object.
    */
    virtual std::string GetScript() const;
    /**
    * Sets the script code associated with this object.
    * @param [in] script The new code.
    */
    virtual void SetScript(const std::string &script);
    /**
    * Determines if the object will be saved.
    *
    * @return True if the object is persistent, false otherwise.
    */
    virtual BOOL GetPersistent() const;
    /**
    * Toggles whether or not the object will be saved.
    *
    * @param [in] s True if the object should be persistent, false otherwise.
    */
    virtual void SetPersistent(BOOL s);
    /**
    * Gets a pointer to the zone object.
    *
    * The zone object is the object which owns this.
    * In otherwords, this is the zone that this object belongs to.
    */
    Zone* GetZone() const;
    /**
    * Sets the zone object that this object belongs to.
    * @param [in] s The new zone object.
    */
    void SetZone(Zone* s);
    /**
    * Return The object's vnum.
    *
    * The VNum of an object is the identifier used to identify the object.
    */
    VNUM GetOnum() const;
    /**
    * Sets the object's num.
    * @param [in] num the vnum for the object.
    */
    void SetOnum(VNUM num);
    /**
    * Adds the specified component to the list.
    *
    * @param [in] component a pointer to the component object.
    * @return true if the component could be added, false otherwise.
    */
    BOOL AddComponent(Component* component);
    /**
    * Removes the specified component from the list.
    *
    * @param [in] component a pointer to the component to be added.
    * @Return true on success, false on failure.
    */
    BOOL RemoveComponent(Component* component);
    /**
    * Checks for the existance of a component by name.
    *
    * @param [in] name the name of the component.
    * @return True if the component exists, false otherwise.
    */
    bool HasComponent(const std::string &name);
    /**
    * Retrieves a pointer to the specified component.
    *
    * @param [in] name the name of the component.
    * @return A pointer to the component, or NULL if it was not found.
    */
    Component* GetComponent(const std::string &name);
    /**
    * Attaches each component to the object it belongs to.
    *
    * When an component is attached to an object, it looks for all dependencies.
    * This can also be used to process other component-specific logic.
    */
    void Attach();
    /**
    * Adds the specified alias to the object.
    *
    * Aliases are other identifiers by which an object can be known.
    * For example, a sword could be known as a sword or by a name.
    * @param [in] alias the name of the alias to add.
    * @return True if the alias could be added, false otherwise.
    */
    BOOL AddAlias(const std::string &alias);
    /**
    * checks to see if the alias exists.
    *
    * @param: [in] the name of the alias.
    * @return True on success, false on failure.
    */
    BOOL AliasExists(const std::string &name);
    /**
    * Returns a pointer to the list of aliases.
    * @return a pointer to a vector of strings.
    */
    std::vector<std::string>* GetAliases();
//TODO: finish docs when this code is cleaned.
    virtual std::string Identify(Player* mobile);
    virtual void Serialize(tinyxml2::XMLElement* root);
    virtual void Deserialize(tinyxml2::XMLElement* root);
    virtual void Copy(BaseObject* obj) const;
    virtual std::string DoLook(Player* mobile);
    virtual BOOL IsPlayer() const;
    virtual BOOL IsLiving() const;
    virtual BOOL IsRoom() const;
    virtual BOOL IsNpc() const;
    virtual BOOL IsObject() const;
};

bool InitializeBaseObjectOlcs();
#endif
