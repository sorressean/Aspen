/*
*The base object is required for holding information about each object.
*Entity is the actual in-game object.
*/
#pragma once
#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H
#include <tinyxml.h>
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

class BaseObject:public ISerializable
{
protected:
    std::vector <Component*> *_components;
    std::vector<std::string>* _aliases;
    VNUM _onum;
    std::string _name;
    std::string _short; //the description you see in a room.
    std::string _desc;
    std::string _script; //the scripting associated with this object.
    std::string _plural;
    Zone* _zone;
    BOOL _persistent;
public:
    EventManager events;
    Property variables;
    Commandable commands;

    BaseObject();
    virtual ~BaseObject();
    virtual std::string GetName() const;
    virtual void SetName(const std::string &s);
    virtual std::string GetShort() const;
    virtual void SetShort(const std::string &s);
    virtual std::string GetDescription() const;
    virtual void SetDescription(const std::string &s);
    virtual std::string GetPlural() const;
    virtual void SetPlural(const std::string &s);
    virtual std::string GetScript() const;
    virtual void SetScript(const std::string &script);
    virtual BOOL GetPersistent() const;
    virtual void SetPersistent(BOOL s);
    Zone* GetZone() const;
    void SetZone(Zone* s);
    /*
    *Returns: The object's vnum.
    */
    VNUM GetOnum() const;
    /*
    *Sets the object's num.
    *Param: [in] the vnum for the object.
    */
    void SetOnum(VNUM num);
    /*
    *Adds the specified component to the list.
    *Param: [in] a pointer to the component object.
    *Return: true if the component could be added, false otherwise.
    */
    BOOL AddComponent(Component* component);
    /*
    *Removes the specified component from the list.
    *Param: [in] a pointer to the component.
    *Return: true on success, false on failure.
    */
    BOOL RemoveComponent(Component* component);
    /*
    *Checks for the existance of a component by name.
    **Param: [in] the name of the component.
    *Return: True if the component exists, false otherwise.
    */
    bool HasComponent(const std::string &name);
    /*
    *Retrieves a pointer to the component.
    *Param: [in] the name of the component.
    *Return: A pointer to the component, or NULL if it was not found.
    */
    Component* GetComponent(const std::string &name);
    void Attach();
    /*
    *Attaches the object to all the components in the list.
    *Should be called after adding a component.
    *Param: [in] the object to attach the components to.
    */
    void Attach(BaseObject * obj);
    /*
    *Adds the specified alias.
    *Param: [in] the name of the alias to add.
    *Return: True if the alias could be added, false otherwise.
    */
    BOOL AddAlias(const std::string &alias);
    /*
    *checks to see if the alias exists.
    *Param: [in] the name of the alias.
    *Return: True on success, false on failure.
    */
    BOOL AliasExists(const std::string &name);
    /*
    *Returns a pointer to the list of aliases.
    *Return: a pointer to a vector of strings.
    */
    std::vector<std::string>* GetAliases();

    virtual std::string Identify(Player* mobile);
    virtual void Serialize(TiXmlElement* root);
    virtual void Deserialize(TiXmlElement* root);
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
