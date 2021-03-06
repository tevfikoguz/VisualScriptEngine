#ifndef NUIE_INTERACTIONHANDLER_HPP
#define NUIE_INTERACTIONHANDLER_HPP

#include "NE_InputSlot.hpp"
#include "NE_OutputSlot.hpp"
#include "NUIE_EventHandlers.hpp"
#include "NUIE_MenuCommands.hpp"
#include "NUIE_UIEventHandlers.hpp"
#include <memory>

namespace NUIE
{

class NodeUIManager;

class PastePositionCalculator
{
public:
	PastePositionCalculator ();

	Point	CalculatePastePosition (const NodeUIManager& uiManager, NodeUIEnvironment& env);

private:
	Point	lastPastePosition;
	int		samePositionPasteCounter;
};

class NodeInputEventHandler : public InputEventHandler
{
public:
	NodeInputEventHandler (NodeUIManager& uiManager, UINodePtr& uiNode);

	virtual EventHandlerResult	HandleMouseDragStart (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position) override;
	virtual EventHandlerResult	HandleMouseDragStop (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position) override;
	virtual EventHandlerResult	HandleMouseDrag (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, const Point& position) override;
	virtual EventHandlerResult	HandleMouseClick (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position) override;
	virtual EventHandlerResult	HandleMouseDoubleClick (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position) override;
	virtual EventHandlerResult	HandleMouseWheel (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseWheelRotation rotation, const Point& position) override;
	virtual EventHandlerResult	HandleKeyPress (NodeUIEnvironment& env, const Key& pressedKey) override;

private:
	EventHandlerResult			ForwardEventToNode (const std::function<EventHandlerResult ()>& forwardEvent);

	NodeUIManager&	uiManager;
	UINodePtr&		uiNode;
};

class InteractionHandler : public InputEventHandler
{
public:
	InteractionHandler (NodeUIManager& uiManager);
	~InteractionHandler ();

	const NodeDrawingModifier*		GetDrawingModifier ();

	virtual EventHandlerResult		HandleMouseDragStart (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position) override;
	virtual EventHandlerResult		HandleMouseDragStop (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position) override;
	virtual EventHandlerResult		HandleMouseDrag (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, const Point& position) override;
	virtual EventHandlerResult		HandleMouseClick (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position) override;
	virtual EventHandlerResult		HandleMouseDoubleClick (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position) override;
	virtual EventHandlerResult		HandleMouseWheel (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseWheelRotation rotation, const Point& position) override;
	virtual EventHandlerResult		HandleKeyPress (NodeUIEnvironment& env, const Key& pressedKey) override;

private:
	NodeUIManager&					uiManager;
	MultiMouseMoveHandler			multiMouseMoveHandler;
	PastePositionCalculator			pastePositionCalculator;
};

}

#endif
