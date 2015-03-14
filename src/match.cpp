#include <string>
#include <vector>
#include <list>
#include "mud.h"
#include "conf.h"
#include "match.h"
#include "world.h"
#include "entity.h"
#include "baseObject.h"
#include "player.h"
#include "npc.h"
#include "utils.h"

bool FullMatch(const std::string& a, const std::string& b, bool icase)
{
    if (icase)
        {
            return a == b? true : false;
        }
    else
        {
            return iequals(a,b);
        }
}

Entity* World::MatchKeyword(const std::string &name, Player* caller)
{
    if ((name=="me")||(name==caller->GetName()))
        {
            return (Entity*)caller;
        }
    if (name.length() < caller->GetName().length() && caller->GetName().substr(name.length()) == name)
        {
            return (Entity*)caller;
        }
    if (name == "here")
        {
            return (caller->GetLocation());
        }

    return NULL;
}

Entity* World::MatchObject(const std::string &name,Player* caller)
{
    std::list<Entity*> *contents; //holds contents for the location and current caller.
    std::list<Entity*>* val;
    std::list<Entity*>::iterator it, itEnd;
    Entity* obj = NULL;

    obj = MatchKeyword(name, caller);
    if (obj)
        {
            return obj;
        }

    contents = new std::list<Entity*>();
    val = caller->GetLocation()->GetContents();
    contents->insert(contents->begin(), val->begin(), val->end());
    val = caller->GetContents();
    contents->insert(contents->begin(), val->begin(), val->end());
    obj = MatchObjectInList(name, val);
    delete contents;
    return obj;
}
Entity* World::MatchObjectInList(const std::string &name, std::list<Entity*> *olist)
{
    std::list<Entity*>::iterator it, itEnd;
    std::string sub; //used for holding the subpart of the number.
    std::string temp; //used for holding a temp copy of the name after it is trimmed.
    std::string alias; //holds a copy of the alias.
    Entity* obj = NULL; //the object we are currently examining/returning.
    std::vector<std::string>* aliases = NULL; //a list of aliases.
    std::vector<std::string>::iterator ait, aitEnd;
    int number = 0; //used for holding the actual number.
    int count = 0; //used for holding the number of objects found.
    size_t marker; //used for holding the position of the '.'.

    if (!olist->size())
        {
            return NULL;
        }

//we check to see if the string has a '.', if so, there's a number.
    marker = name.find_first_of(".");
//check to see if it is 1) at the beginning, or 2) at the end.
    if ((marker == 0) || (marker == name.length()))
        {
            return NULL;
        }

    if (marker != std::string::npos)   //we found something.
        {
            sub = name.substr(marker); //the subnumber.
            temp = name.substr(marker+1); //trim off the x. bit
            number = atoi(sub.c_str());
            if (number == 0)
                {
                    return NULL;
                }

            itEnd = olist->end();
            for (it = olist->begin(); it != itEnd && count < number; ++it)
                {
                    obj = *it;
                    if (obj->GetName().length() < temp.length())   //we check for a partial match
                        {
                            if (obj->GetName().substr(temp.length()) == temp)
                                {
                                    count++; //we found a match, increase the counter.
                                    continue;
                                }
                        }
                    if (obj->GetName() == temp)   //full match
                        {
                            count++;
                            continue;
                        }

//now we check a list of aliases.
                    aliases = obj->GetAliases();
                    if (aliases->size())
                        {
                            aitEnd = aliases->end();
                            for (ait = aliases->begin(); ait != aitEnd; ++ait)
                                {
                                    alias = (*ait);
                                    if (alias.length() < temp.length())   //we check for a partial match
                                        {
                                            if (alias.substr(temp.length()) == temp)
                                                {
                                                    count++; //we found a match, increase the counter.
                                                    continue;
                                                }
                                        }
                                    if (alias == temp)   //full match
                                        {
                                            count++;
                                            continue;
                                        }
                                }
                        }
                }
            if (count != (number -1))
                {
                    return NULL;
                }
            else
                {
                    return obj;
                }
        }
    else
        {
            itEnd = olist->end();
            for (it = olist->begin(); it != itEnd; ++it)
                {
                    obj = *it;

                    if (obj->GetName().length() < temp.length())   //we check for a partial match
                        {
                            if (obj->GetName().substr(temp.length()) == temp)
                                {
                                    return obj;
                                }
                        }
                    if (obj->GetName() == temp)   //full match
                        {
                            return obj;
                        }

//now we check a list of aliases.
                    aliases = obj->GetAliases();
                    if (aliases->size())
                        {
                            aitEnd = aliases->end();
                            for (ait = aliases->begin(); ait != aitEnd; ++ait)
                                {
                                    alias = *ait;
                                    if (alias.length() < temp.length())   //we check for a partial match
                                        {
                                            if (alias.substr(temp.length()) == temp)
                                                {
                                                    return obj;
                                                }
                                        }
                                    if (alias == temp)   //full match
                                        {
                                            return obj;
                                        }
                                }
                        }
                }
        }

    return NULL;
}
Entity* World::MatchObjectInVector(const std::string &name, std::vector<Entity*> *olist)
{
    std::vector<Entity*>::iterator it, itEnd;
    std::string sub; //used for holding the subpart of the number.
    std::string temp; //used for holding a temp copy of the name after it is trimmed.
    std::string alias; //holds a copy of the alias.
    Entity* obj = NULL; //the object we are currently examining/returning.
    std::vector<std::string>* aliases = NULL; //a list of aliases.
    std::vector<std::string>::iterator ait, aitEnd;
    int number = 0; //used for holding the actual number.
    int count = 0; //used for holding the number of objects found.
    size_t marker; //used for holding the position of the '.'.

    if (!olist->size())
        {
            return NULL;
        }

//we check to see if the string has a '.', if so, there's a number.
    marker = name.find_first_of(".");
//check to see if it is 1) at the beginning, or 2) at the end.
    if ((marker == 0) || (marker == name.length()))
        {
            return NULL;
        }

    if (marker != std::string::npos)   //we found something.
        {
            sub = name.substr(marker); //the subnumber.
            temp = name.substr(marker+1); //trim off the x. bit
            number = atoi(sub.c_str());
            if (number == 0)
                {
                    return NULL;
                }

            itEnd = olist->end();
            for (it = olist->begin(); it != itEnd && count < number; ++it)
                {
                    obj = *it;
                    if (obj->GetName().length() < temp.length())   //we check for a partial match
                        {
                            if (obj->GetName().substr(temp.length()) == temp)
                                {
                                    count++; //we found a match, increase the counter.
                                    continue;
                                }
                        }
                    if (obj->GetName() == temp)   //full match
                        {
                            count++;
                            continue;
                        }

//now we check a list of aliases.
                    aliases = obj->GetAliases();
                    if (aliases->size())
                        {
                            aitEnd = aliases->end();
                            for (ait = aliases->begin(); ait != aitEnd; ++ait)
                                {
                                    alias = (*ait);
                                    if (alias.length() < temp.length())   //we check for a partial match
                                        {
                                            if (alias.substr(temp.length()) == temp)
                                                {
                                                    count++; //we found a match, increase the counter.
                                                    continue;
                                                }
                                        }
                                    if (alias == temp)   //full match
                                        {
                                            count++;
                                            continue;
                                        }
                                }
                        }
                }
            if (count != (number -1))
                {
                    return NULL;
                }
            else
                {
                    return obj;
                }
        }
    else
        {
            itEnd = olist->end();
            for (it = olist->begin(); it != itEnd; ++it)
                {
                    obj = *it;

                    if (obj->GetName().length() < temp.length())   //we check for a partial match
                        {
                            if (obj->GetName().substr(temp.length()) == temp)
                                {
                                    return obj;
                                }
                        }
                    if (obj->GetName() == temp)   //full match
                        {
                            return obj;
                        }

//now we check a list of aliases.
                    aliases = obj->GetAliases();
                    if (aliases->size())
                        {
                            aitEnd = aliases->end();
                            for (ait = aliases->begin(); ait != aitEnd; ++ait)
                                {
                                    alias = *ait;
                                    if (alias.length() < temp.length())   //we check for a partial match
                                        {
                                            if (alias.substr(temp.length()) == temp)
                                                {
                                                    return obj;
                                                }
                                        }
                                    if (alias == temp)   //full match
                                        {
                                            return obj;
                                        }
                                }
                        }
                }
        }

    return NULL;
}
