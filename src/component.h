/**
* Contains: Component
*
* The component class is responsible for adding functionality to an object.
* With components, objects are no longer has-a relatoinships, but has-a relationships.
* For example, in order to enable weapons to be worn (like gloves with claws),
* weapon logic would need to be part of the inheritance structure or enable all objects to be worn.
* This is problematic, because not every object should be a weapon or worn in certain scenarios.
* Components enable you to add the functionality to individual objects without complicating the
* inheritance structure of the object.
*/
#ifndef COMPONENT_H
#define COMPONENT_H
#include <string>
#include <vector>
#include <functional>
#include <tinyxml.h>
#include "mud.h"
#include "conf.h"
#include "entity.h"
#include "event.h"
#include "eventManager.h"
#include "componentMeta.hpp"
#include "property.h"

/**
* Components enable for the extension of objects.
*/
class Component
{
protected:
    BaseObject* _object; //The object associated with the component (it's parent).
    IComponentMeta* _parent;
    BOOL _attached; //set if the component is attached to an object.
public:
    Property variables;
    EventManager events;
    /**
    * Constructs a component object.
    * @param [in] the parent component meta class.
    */
    Component(IComponentMeta* parent);
    Component();
    virtual ~Component();
    virtual void Serialize(TiXmlElement* root);
    virtual void Deserialize(TiXmlElement* root);

    /**
    * Sets the object that this component is attached to.
    *
    * @param [in] the object which the component belongs to.
    */
    void SetObject(BaseObject* obj);
    /**
    * Gets the object which this component is attached to.
    */
    BaseObject* GetObject() const;
    /**
    * Gets the component meta for this component.
    */
    IComponentMeta* GetMeta();
    /**
    * Attaches the component to the object, sets the object if needed.
    *
    * Also adds dependencies to the object, if they are not already added.
    * @param [in] the object to attach to.
    * @todo check for dependency circulation.
    */
    void Attach(BaseObject* obj);
    /*
    *Detaches the dependency from the object.
    */
    void Detach();

    void Initialize();
};

/**
* This is the function pointer passed to the component factory.
* Used to point to the create methods of a component.
*/
typedef std::function<void ()> COMINITCB ;
#define COMINIT(name) void name()
#define CCOMINIT(class, name) void class::name()
#endif
