#ifndef NUIE_EVENTHANDLERS_HPP
#define NUIE_EVENTHANDLERS_HPP

#include "NUIE_UINode.hpp"
#include "NUIE_UINodeGroup.hpp"
#include "NUIE_MenuCommands.hpp"
#include "NUIE_ParameterInterface.hpp"
#include <vector>

namespace NUIE
{

class NodeUIManager;

class EventHandlers
{
public:
	EventHandlers ();
	virtual ~EventHandlers ();

	virtual NUIE::MenuCommandPtr	OnContextMenu (NodeUIManager& uiManager, NodeUIEnvironment& env, const Point& position, const MenuCommandStructure& commands) = 0;
	virtual NUIE::MenuCommandPtr	OnContextMenu (NodeUIManager& uiManager, NodeUIEnvironment& env, const Point& position, const UINodePtr& uiNode, const MenuCommandStructure& commands) = 0;
	virtual NUIE::MenuCommandPtr	OnContextMenu (NodeUIManager& uiManager, NodeUIEnvironment& env, const Point& position, const UIOutputSlotConstPtr& inputSlot, const MenuCommandStructure& commands) = 0;
	virtual NUIE::MenuCommandPtr	OnContextMenu (NodeUIManager& uiManager, NodeUIEnvironment& env, const Point& position, const UIInputSlotConstPtr& inputSlot, const MenuCommandStructure& commands) = 0;
	virtual NUIE::MenuCommandPtr	OnContextMenu (NodeUIManager& uiManager, NodeUIEnvironment& env, const Point& position, const UINodeGroupPtr& group, const MenuCommandStructure& commands) = 0;
	virtual bool					OnParameterSettings (ParameterInterfacePtr paramAccessor) = 0;
};

using EventHandlersPtr = std::shared_ptr<EventHandlers>;
using EventHandlersConstPtr = std::shared_ptr<const EventHandlers>;

class NullEventHandlers : public EventHandlers
{
public:
	NullEventHandlers ();
	virtual ~NullEventHandlers ();

	virtual NUIE::MenuCommandPtr	OnContextMenu (NodeUIManager& uiManager, NodeUIEnvironment& env, const Point& position, const MenuCommandStructure& commands) override;
	virtual NUIE::MenuCommandPtr	OnContextMenu (NodeUIManager& uiManager, NodeUIEnvironment& env, const Point& position, const UINodePtr& uiNode, const MenuCommandStructure& commands) override;
	virtual NUIE::MenuCommandPtr	OnContextMenu (NodeUIManager& uiManager, NodeUIEnvironment& env, const Point& position, const UIOutputSlotConstPtr& inputSlot, const MenuCommandStructure& commands) override;
	virtual NUIE::MenuCommandPtr	OnContextMenu (NodeUIManager& uiManager, NodeUIEnvironment& env, const Point& position, const UIInputSlotConstPtr& inputSlot, const MenuCommandStructure& commands) override;
	virtual NUIE::MenuCommandPtr	OnContextMenu (NodeUIManager& uiManager, NodeUIEnvironment& env, const Point& position, const UINodeGroupPtr& group, const MenuCommandStructure& commands) override;
	virtual bool					OnParameterSettings (ParameterInterfacePtr paramAccessor) override;
};

}

#endif
