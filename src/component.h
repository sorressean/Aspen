#ifndef COMPONENT_H
#define COMPONENT_H
#include <string>
#include <vector>
#include <tinyxml.h>
#include <functional>
#include "mud.h"
#include "conf.h"
#include "entity.h"
#include "event.h"
#include "eventManager.h"
#include "componentMeta.h"
#include "property.h"

class Component
{
    BaseObject* _object; //The object associated with the component (it's parent).
    ComponentMeta* _parent;
    BOOL _attached; //set if the component is attached to an object.
public:
    Property variables;
    EventManager events;
    Component(ComponentMeta* parent);
    Component();
    virtual ~Component();
    virtual void Serialize(TiXmlElement* root);
    virtual void Deserialize(TiXmlElement* root);

    /*
    *Getters and setters
    *Getters:
    *Return: the value being requested.
    *Setters:
    *Param: [in] the value to set
    */
    void SetObject(BaseObject* obj);
    BaseObject* GetObject() const;
    ComponentMeta* GetMeta();
    /*
    *Attaches the component to the object, sets the object if needed.
    *Also adds dependencies to the object, if they are not already added.
    *Param: [in] the object to attach to.
    */
    void Attach(BaseObject* obj);
    /*
    *Detaches the dependency from the object.
    */
    void Detach();

    virtual void _Init();
};

/*
*A function pointer passed to the component factory.
*Used to point to the create methods of a component.
*/
typedef std::function<void ()> COMINITCB ;
#define COMINIT(name) void name()
#define CCOMINIT(class, name) void class::name()

#endif
