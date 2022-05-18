/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU GPL v2 license, you may redistribute it and/or modify it under version 2 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_TRIGGER_H
#define _PLAYERBOT_TRIGGER_H

#include "Common.h"
#include "Action.h"

class PlayerbotAI;
class Unit;

class Trigger : public AiNamedObject
{
	public:
        Trigger(PlayerbotAI* botAI, std::string const name = "trigger", int32 checkInterval = 1);

        virtual ~Trigger() { }

        virtual Event Check();
        virtual void ExternalEvent() { } // unused params "std::string const param, Player* owner = nullptr" - whipowill
        virtual void ExternalEvent() { } // unused params "WorldPacket& packet, Player* owner = nullptr" - whipowill
        virtual bool IsActive() { return false; }
        virtual NextAction** getHandlers() { return nullptr; }
        void Update() { }
        virtual void Reset() { }
        virtual Unit* GetTarget();
        virtual Value<Unit*>* GetTargetValue();
        virtual std::string const GetTargetName() { return "self target"; }

		bool needCheck();

    protected:
		int32 checkInterval;
		time_t lastCheckTime;
};

class TriggerNode
{
    public:
        TriggerNode(std::string const name, NextAction** handlers = nullptr) : name(name), trigger(nullptr), handlers(handlers) { } // name after handlers, handlers after trigger - whipowill

        virtual ~TriggerNode()
        {
            NextAction::destroy(handlers);
        }

        Trigger* getTrigger() { return trigger; }
        void setTrigger(Trigger* trigger) { this->trigger = trigger; }
        std::string const getName() { return name; }

        NextAction** getHandlers()
        {
            return NextAction::merge(NextAction::clone(handlers), trigger->getHandlers());
        }

        float getFirstRelevance()
        {
            return handlers[0] ? handlers[0]->getRelevance() : -1;
        }

    private:
        Trigger* trigger;
        NextAction** handlers;
        std::string const name;
};

#endif
