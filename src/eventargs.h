/**
* Contains: various event arguments.
*/
#ifndef EVENT_ARGS_H
#define EVENT_ARGS_H
#include "mud.h"
#include "option.h"
#include "baseObject.h"

/**
* LookArgs
*
* Called when a player looks at an object.
*/
class LookArgs:public EventArgs
{
public:
    /**
    * @param [in] caller the player doing the looking.
    * @param [in] targ the target the player is looking at.
    * @param [in,out] desc the description.
    */
    LookArgs(Player* caller, BaseObject* targ,std::string &desc):_caller(caller),_targ(targ),_desc(desc) {}
    ~LookArgs() {}

    Player* _caller;
    BaseObject* _targ;
    std::string &_desc;
};

/**
* Used when a component is attached to an object.
*/
class ComponentAttachedArgs:public EventArgs
{
public:
    /**
    * @param [in] obj the object the component is attached to.
    */
    ComponentAttachedArgs(BaseObject* _obj):obj(_obj) {}
    BaseObject* obj;
};

class Editor;
/**
* Used when an editor save is called.
*/
class EditorSavedArgs:public EventArgs
{
public:
    /**
    * @param [in] ed the editor responsible for saving.
    */
    EditorSavedArgs(Editor* ed):editor(ed) {}
    EditorSavedArgs();
    Editor* editor;
};

/**
* Used when an editor loads.
*/
class EditorLoadedArgs:public EventArgs
{
public:
    /**
    * @param [in] ed the editor responsible for saving.
    */
    EditorLoadedArgs(Editor* ed):editor(ed) {}
    EditorLoadedArgs() {}
    Editor* editor;
};

/**
* Used when an editor is exited.
*/
class EditorExitedArgs:public EventArgs
{
public:
    /**
    * @param [in] ed the editor responsible for saving.
    */
    EditorExitedArgs(Editor* ed):editor(ed) {}
    EditorExitedArgs() {}
    Editor* editor;
};
/**
* Used when an editor aborts.
*/
class EditorAbortedArgs:public EventArgs
{
public:
    /**
    * @param [in] ed the editor responsible for saving.
    */
    EditorAbortedArgs(Editor* ed):editor(ed) { }
    EditorAbortedArgs() { }
    ~EditorAbortedArgs() { }
    Editor* editor;
};

class Option;
/**
* Used when an option changes.
*/
class OptionChangedArgs:public EventArgs
{
public:
    /**
    * @param [in] opt the option that is changing.
    */
    OptionChangedArgs(Option* _opt):opt(_opt) {}
    Option* opt;
};
#endif
