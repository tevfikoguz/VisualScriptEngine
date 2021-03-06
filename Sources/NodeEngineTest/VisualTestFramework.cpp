#include "VisualTestFramework.hpp"
#include "NUIE_InputEventHandler.hpp"
#include "BI_BuiltInCommands.hpp"
#include "BI_BuiltInNodes.hpp"
#include "SimpleTest.hpp"

#include <iostream>
#include <fstream>

class IncreaseNode : public BI::BasicUINode
{
	DYNAMIC_SERIALIZABLE(IncreaseNode);

public:
	IncreaseNode () :
		IncreaseNode (L"", Point ())
	{
	}

	IncreaseNode (const std::wstring& name, const Point& position) :
		BI::BasicUINode (name, position)
	{

	}

	virtual ~IncreaseNode ()
	{
	}

	virtual void Initialize () override
	{
		RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("in"), L"In", nullptr, NE::OutputSlotConnectionMode::Single)));
		RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("out"), L"Out")));
	}

	NE::ValueConstPtr Calculate (NE::EvaluationEnv& env) const override
	{
		NE::ValueConstPtr inValue = EvaluateInputSlot (NE::SlotId ("in"), env);
		if (!IsSingleType<NE::IntValue> (inValue)) {
			return nullptr;
		}
		return NE::ValuePtr (new NE::IntValue (NE::IntValue::Get (inValue) + 1));
	}

	virtual NE::Stream::Status Read (NE::InputStream& inputStream) override
	{
		NE::ObjectHeader header (inputStream);
		BasicUINode::Read (inputStream);
		return inputStream.GetStatus ();
	}

	virtual NE::Stream::Status Write (NE::OutputStream& outputStream) const override
	{
		NE::ObjectHeader header (outputStream, serializationInfo);
		BasicUINode::Write (outputStream);
		return outputStream.GetStatus ();
	}
};

NE::DynamicSerializationInfo IncreaseNode::serializationInfo (NE::ObjectId ("{8C06D4A9-B042-4E23-8556-410AA5ED2B35}"), NE::ObjectVersion (1), IncreaseNode::CreateSerializableInstance);

class MyCreateNodeCommand : public BI::CreateNodeCommand
{
public:
	enum class NodeType
	{
		Number,
		Integer,
		Addition,
		Increase,
		Viewer
	};

	MyCreateNodeCommand (NodeType nodeType, NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& uiEnvironment, const std::wstring& name, const NUIE::Point& position) :
		BI::CreateNodeCommand (name, uiManager, uiEnvironment, position),
		nodeType (nodeType)
	{
	
	}

	virtual NUIE::UINodePtr CreateNode (const NUIE::Point& modelPosition) override
	{
		switch (nodeType) {
			case NodeType::Number:
				return NUIE::UINodePtr (new BI::DoubleUpDownNode (L"Number", modelPosition, 0.0, 5.0));
			case NodeType::Integer:
				return NUIE::UINodePtr (new BI::IntegerUpDownNode (L"Integer", modelPosition, 0, 1));
			case NodeType::Addition:
				return NUIE::UINodePtr (new BI::AdditionNode (L"Addition", modelPosition));
			case NodeType::Increase:
				return NUIE::UINodePtr (new IncreaseNode (L"Increase", modelPosition));
			case NodeType::Viewer:
				return NUIE::UINodePtr (new BI::MultiLineViewerNode (L"Viewer", modelPosition, 5));
		}
		return nullptr;
	}

private:
	NodeType nodeType;
};

TestEventHandlers::TestEventHandlers () :
	commandToSelect (),
	paramSettingsHandler (nullptr)
{
	
}

MenuCommandPtr TestEventHandlers::OnContextMenu (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const Point& position, const MenuCommandStructure& commands)
{
	NUIE::MenuCommandStructure actualCommands = commands;
	NUIE::GroupMenuCommandPtr createCommandGroup (new NUIE::GroupMenuCommand (L"Add Node"));

	createCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::Number, uiManager, uiEnvironment, L"Create Number Node", position)));
	createCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::Integer, uiManager, uiEnvironment, L"Create Integer Node", position)));
	createCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::Addition, uiManager, uiEnvironment, L"Create Addition Node", position)));
	createCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::Increase, uiManager, uiEnvironment, L"Create Increase Node", position)));
	createCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::Viewer, uiManager, uiEnvironment, L"Create Viewer Node", position)));
	actualCommands.AddCommand (createCommandGroup);

	return SelectCommandByName (actualCommands);
}

MenuCommandPtr TestEventHandlers::OnContextMenu (NodeUIManager&, NodeUIEnvironment&, const Point&, const UINodePtr&, const MenuCommandStructure& commands)
{
	return SelectCommandByName (commands);
}

MenuCommandPtr TestEventHandlers::OnContextMenu (NodeUIManager&, NodeUIEnvironment&, const Point&, const UIOutputSlotConstPtr&, const MenuCommandStructure& commands)
{
	return SelectCommandByName (commands);
}

MenuCommandPtr TestEventHandlers::OnContextMenu (NodeUIManager&, NodeUIEnvironment&, const Point&, const UIInputSlotConstPtr&, const MenuCommandStructure& commands)
{
	return SelectCommandByName (commands);
}

MenuCommandPtr TestEventHandlers::OnContextMenu (NodeUIManager&, NodeUIEnvironment&, const Point&, const UINodeGroupPtr&, const MenuCommandStructure& commands)
{
	return SelectCommandByName (commands);
}

bool TestEventHandlers::OnParameterSettings (ParameterInterfacePtr paramInterface)
{
	if (DBGERROR (paramSettingsHandler == nullptr)) {
		return false;
	}
	bool result = paramSettingsHandler (paramInterface);
	paramSettingsHandler = nullptr;
	return result;
}

void TestEventHandlers::SetNextCommandName (const std::wstring& nextCommandName)
{
	DBGASSERT (commandToSelect.empty ());
	commandToSelect = nextCommandName;
}

void TestEventHandlers::SetNextCommandParameterSettings (const ParameterSettingsHandler& handler)
{
	SetNextCommandName (L"Set Parameters");
	paramSettingsHandler = handler;
}

MenuCommandPtr TestEventHandlers::SelectCommandByName (const MenuCommandStructure& commands)
{
	DBGASSERT (!commandToSelect.empty ());
	MenuCommandPtr selectedCommand = nullptr;
	commands.EnumerateCommands ([&] (const MenuCommandPtr& command) {
		if (selectedCommand == nullptr) {
			selectedCommand = SelectCommandByName (command);
		}
	});
	DBGASSERT (selectedCommand != nullptr);
	commandToSelect.clear ();
	return selectedCommand;
}

MenuCommandPtr TestEventHandlers::SelectCommandByName (const MenuCommandPtr& command)
{
	if (command->HasChildCommands ()) {
		MenuCommandPtr foundCommand = nullptr;
		command->EnumerateChildCommands ([&] (const MenuCommandPtr& childCommand) {
			if (foundCommand == nullptr) {
				foundCommand = SelectCommandByName (childCommand);
			}
		});
		return foundCommand;
	} else {
		if (command->GetName () == commandToSelect) {
			return command;
		}
	}
	return nullptr;
}

TestNodeUIEnvironment::TestNodeUIEnvironment (NodeEditor& nodeEditor, const BasicSkinParams& skinParams) :
	NUIE::NodeUIEnvironment (),
	nodeEditor (nodeEditor),
	stringSettings (GetDefaultStringSettings ()),
	skinParams (skinParams),
	drawingContext (800, 600),
	eventHandlers (),
	evaluationEnv (nullptr),
	windowScale (1.0)
{
	
}

const StringSettings& TestNodeUIEnvironment::GetStringSettings ()
{
	return stringSettings;
}

const SkinParams& TestNodeUIEnvironment::GetSkinParams ()
{
	return skinParams;
}

DrawingContext& TestNodeUIEnvironment::GetDrawingContext ()
{
	return drawingContext;
}

double TestNodeUIEnvironment::GetWindowScale ()
{
	return windowScale;
}

EvaluationEnv& TestNodeUIEnvironment::GetEvaluationEnv ()
{
	return evaluationEnv;
}

void TestNodeUIEnvironment::OnEvaluationBegin ()
{

}

void TestNodeUIEnvironment::OnEvaluationEnd ()
{

}

void TestNodeUIEnvironment::OnValuesRecalculated ()
{

}

void TestNodeUIEnvironment::OnRedrawRequested ()
{
	nodeEditor.Draw ();
}

EventHandlers& TestNodeUIEnvironment::GetEventHandlers ()
{
	return eventHandlers;
}

double TestNodeUIEnvironment::GetMouseMoveMinOffset ()
{
	return 2.0;
}

void TestNodeUIEnvironment::SetNextCommandName (const std::wstring& nextCommandName)
{
	eventHandlers.SetNextCommandName (nextCommandName);
}

void TestNodeUIEnvironment::SetNextCommandParameterSettings (const ParameterSettingsHandler& handler)
{
	eventHandlers.SetNextCommandParameterSettings (handler);
}

const SvgDrawingContext& TestNodeUIEnvironment::GetSvgDrawingContext () const
{
	return drawingContext;
}

void TestNodeUIEnvironment::SetWindowScale (double newWindowScale)
{
	windowScale = newWindowScale;
}

NodeEditorTestEnv::NodeEditorTestEnv (const BasicSkinParams& skinParams) :
	uiEnvironment (nodeEditor, skinParams),
	nodeEditor (uiEnvironment)
{
	nodeEditor.Update ();
}

bool NodeEditorTestEnv::CheckReference (const std::string& referenceFileName)
{
	const SvgDrawingContext& context = uiEnvironment.GetSvgDrawingContext ();

	std::string testFilesPath = SimpleTest::GetAppFolderLocation () + "VisualTestFiles" + PATH_SEPARATOR;
	std::string referenceFilePath = testFilesPath + referenceFileName;
	std::wifstream referenceFile;
	referenceFile.open (referenceFilePath);
	if (!referenceFile.is_open ()) {
		context.WriteToFile (testFilesPath + "Current_" + referenceFileName);
		return false;
	}

	std::wstringstream referenceFileBuffer;
	referenceFileBuffer << referenceFile.rdbuf ();
	std::wstring referenceContent = referenceFileBuffer.str ();
	referenceFile.close ();

	std::wstring currentContent = context.GetAsString ();
	referenceContent = ReplaceAll (referenceContent, L"\r\n", L"\n");
	currentContent = ReplaceAll (currentContent, L"\r\n", L"\n");
	if (referenceContent != currentContent) {
		std::wcout << std::endl << L"=== CURRENT ===" << std::endl;
		std::wcout << currentContent << std::endl;
		std::wcout << L"=== REFERENCE ===" << std::endl;
		std::wcout << referenceContent << std::endl;
		context.WriteToFile (testFilesPath + "Current_" + referenceFileName);
		return false;
	}
	return true;
}

void NodeEditorTestEnv::Click (const Point& point)
{
	nodeEditor.OnMouseDown (EmptyModifierKeys, MouseButton::Left, (int) point.GetX (), (int) point.GetY ());
	nodeEditor.OnMouseUp (EmptyModifierKeys, MouseButton::Left, (int) point.GetX (), (int) point.GetY ());
}

void NodeEditorTestEnv::CtrlClick (const Point& point)
{
	nodeEditor.OnMouseDown (ModifierKeys ({ ModifierKeyCode::Control }), MouseButton::Left, (int) point.GetX (), (int) point.GetY ());
	nodeEditor.OnMouseUp (ModifierKeys ({ ModifierKeyCode::Control }), MouseButton::Left, (int) point.GetX (), (int) point.GetY ());
}

void NodeEditorTestEnv::RightClick (const Point& point)
{
	nodeEditor.OnMouseDown (EmptyModifierKeys, MouseButton::Right, (int) point.GetX (), (int) point.GetY ());
	nodeEditor.OnMouseUp (EmptyModifierKeys, MouseButton::Right, (int) point.GetX (), (int) point.GetY ());
}

void NodeEditorTestEnv::Wheel (MouseWheelRotation rotation, const Point& point)
{
	nodeEditor.OnMouseWheel (EmptyModifierKeys, rotation, (int) point.GetX (), (int) point.GetY ());
}

void NodeEditorTestEnv::DragDrop (const Point& from, const Point& to, const std::function<void ()>& beforeMouseUp)
{
	DragDrop (EmptyModifierKeys, from, to, beforeMouseUp);
}

void NodeEditorTestEnv::DragDrop (const ModifierKeys& keys, const Point& from, const Point& to, const std::function<void ()>& beforeMouseUp)
{
	nodeEditor.OnMouseDown (keys, MouseButton::Left, (int) from.GetX (), (int) from.GetY ());
	nodeEditor.OnMouseMove (keys, (int) to.GetX (), (int) to.GetY ());
	if (beforeMouseUp != nullptr) {
		beforeMouseUp ();
	}
	nodeEditor.OnMouseUp (keys, MouseButton::Left, (int) to.GetX (), (int) to.GetY ());
}

void NodeEditorTestEnv::SetNextCommandName (const std::wstring& nextCommandName)
{
	uiEnvironment.SetNextCommandName (nextCommandName);
}

void NodeEditorTestEnv::SetNextCommandParameterSettings (const ParameterSettingsHandler& handler)
{
	uiEnvironment.SetNextCommandParameterSettings (handler);
}
