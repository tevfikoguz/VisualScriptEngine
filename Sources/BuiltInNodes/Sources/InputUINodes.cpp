#include "InputUINodes.hpp"
#include "UINodePanels.hpp"
#include "SkinParams.hpp"

#include <algorithm>

namespace NUIE
{

NE::SerializationInfo			NumericUpDownUINode::serializationInfo (NE::ObjectId ("{F888C04D-FF22-4225-AC9A-90464D01ACF9}"), NE::ObjectVersion (1));
NE::DynamicSerializationInfo	IntegerUpDownUINode::serializationInfo (NE::ObjectId ("{53207AEE-8865-4DC0-AA38-B9988E39EDFD}"), NE::ObjectVersion (1), IntegerUpDownUINode::CreateSerializableInstance);
NE::DynamicSerializationInfo	DoubleUpDownUINode::serializationInfo (NE::ObjectId ("{84797692-0189-4DC6-9731-353B1251DB46}"), NE::ObjectVersion (1), DoubleUpDownUINode::CreateSerializableInstance);
NE::DynamicSerializationInfo	IntegerRangeNode::serializationInfo (NE::ObjectId ("{B697B7DE-7AB9-479D-8DBE-8D3CCB6E4F50}"), NE::ObjectVersion (1), IntegerRangeNode::CreateSerializableInstance);

NumericUpDownUINode::NumericUpDownUINode () :
	UINode ()
{

}

NumericUpDownUINode::NumericUpDownUINode (const std::wstring& name, const Point& position) :
	UINode (name, position)
{

}

NumericUpDownUINode::~NumericUpDownUINode ()
{

}

NUIE::EventHandlerResult NumericUpDownUINode::HandleMouseClick (NodeUIEnvironment& env, const KeySet&, MouseButton mouseButton, const Point& position)
{
	if (mouseButton != MouseButton::Left) {
		return EventHandlerResult::EventNotHandled;
	}

	Rect minusButtonRect = GetSpecialRect (env, "minus");
	Rect plusButtonRect = GetSpecialRect (env, "plus");

	if (minusButtonRect.Contains (position)) {
		Decrease ();
		return EventHandlerResult::EventHandled;
	} else if (plusButtonRect.Contains (position)) {
		Increase ();
		return EventHandlerResult::EventHandled;
	}
	return EventHandlerResult::EventNotHandled;
}

void NumericUpDownUINode::UpdateNodeDrawingImage (NodeUIEnvironment& env, NodeDrawingImage& drawingImage) const
{
	std::wstring nodeText = L"<empty>";
	NE::ValuePtr nodeValue = Evaluate (env.GetEvaluationEnv ());
	if (nodeValue != nullptr) {
		nodeText = nodeValue->ToString ();
	}

	NodeUIPanelDrawer drawer;
	drawer.AddPanel (NodeUIPanelPtr (new NodeUIHeaderPanel (GetNodeName ())));
	drawer.AddPanel (NodeUIPanelPtr (new NodeUISlotPanel (*this, env)));
	drawer.AddPanel (NodeUIPanelPtr (new NodeUILeftRightButtonsPanel ("minus", L"<", "plus", L">", nodeText, env)));
	drawer.Draw (env, drawingImage);
}

void NumericUpDownUINode::CalculationPostProcess (const NE::ValuePtr&, NE::EvaluationEnv&) const
{

}

NE::Stream::Status NumericUpDownUINode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	UINode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status NumericUpDownUINode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	UINode::Write (outputStream);
	return outputStream.GetStatus ();
}

IntegerUpDownUINode::IntegerUpDownUINode () :
	NumericUpDownUINode (),
	val (0),
	step (0)
{

}

IntegerUpDownUINode::IntegerUpDownUINode (const std::wstring& name, const Point& position, int val, int step) :
	NumericUpDownUINode (name, position),
	val (val),
	step (step)
{

}

void IntegerUpDownUINode::RegisterSlots ()
{
	RegisterUIOutputSlot (UIOutputSlotPtr (new UIOutputSlot (NE::SlotId ("out"), L"Output")));
}

NE::ValuePtr IntegerUpDownUINode::Calculate (NE::EvaluationEnv&) const
{
	return NE::ValuePtr (new NE::IntValue (val));
}

void IntegerUpDownUINode::Increase ()
{
	val = val + step;
	InvalidateValue ();
}

void IntegerUpDownUINode::Decrease ()
{
	val = val - step;
	InvalidateValue ();
}

NE::Stream::Status IntegerUpDownUINode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	NumericUpDownUINode::Read (inputStream);
	inputStream.Read (val);
	inputStream.Read (step);
	return inputStream.GetStatus ();
}

NE::Stream::Status IntegerUpDownUINode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	NumericUpDownUINode::Write (outputStream);
	outputStream.Write (val);
	outputStream.Write (step);
	return outputStream.GetStatus ();
}

DoubleUpDownUINode::DoubleUpDownUINode () :
	NumericUpDownUINode (),
	val (0),
	step (0)
{

}

DoubleUpDownUINode::DoubleUpDownUINode (const std::wstring& name, const Point& position, double val, double step) :
	NumericUpDownUINode (name, position),
	val (val),
	step (step)
{

}

void DoubleUpDownUINode::RegisterSlots ()
{
	RegisterUIOutputSlot (UIOutputSlotPtr (new UIOutputSlot (NE::SlotId ("out"), L"Output")));
}

NE::ValuePtr DoubleUpDownUINode::Calculate (NE::EvaluationEnv&) const
{
	return NE::ValuePtr (new NE::DoubleValue (val));
}

void DoubleUpDownUINode::Increase ()
{
	val = val + step;
	InvalidateValue ();
}

void DoubleUpDownUINode::Decrease ()
{
	val = val - step;
	InvalidateValue ();
}

NE::Stream::Status DoubleUpDownUINode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	NumericUpDownUINode::Read (inputStream);
	inputStream.Read (val);
	inputStream.Read (step);
	return inputStream.GetStatus ();
}

NE::Stream::Status DoubleUpDownUINode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	NumericUpDownUINode::Write (outputStream);
	outputStream.Write (val);
	outputStream.Write (step);
	return outputStream.GetStatus ();
}

IntegerRangeNode::IntegerRangeNode () :
	NormalUINode ()
{

}

IntegerRangeNode::IntegerRangeNode (const std::wstring& name, const Point& position) :
	NormalUINode (name, position)
{

}

IntegerRangeNode::~IntegerRangeNode ()
{

}

void IntegerRangeNode::RegisterSlots ()
{
	RegisterUIInputSlot (UIInputSlotPtr (new UIInputSlot (NE::SlotId ("start"), L"Start", NE::ValuePtr (new NE::IntValue (0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (UIInputSlotPtr (new UIInputSlot (NE::SlotId ("step"), L"Step", NE::ValuePtr (new NE::IntValue (1)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (UIInputSlotPtr (new UIInputSlot (NE::SlotId ("count"), L"Count", NE::ValuePtr (new NE::IntValue (10)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (UIOutputSlotPtr (new UIOutputSlot (NE::SlotId ("out"), L"List")));
}

NE::ValuePtr IntegerRangeNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValuePtr start = EvaluateSingleInputSlot (NE::SlotId ("start"), env);
	NE::ValuePtr step = EvaluateSingleInputSlot (NE::SlotId ("step"), env);
	NE::ValuePtr count = EvaluateSingleInputSlot (NE::SlotId ("count"), env);
	if (!NE::Value::IsType<NE::IntValue> (start) || !NE::Value::IsType<NE::IntValue> (step) || !NE::Value::IsType<NE::IntValue> (count)) {
		return nullptr;
	}

	int startInt = NE::IntValue::Get (start);
	int stepInt = NE::IntValue::Get (step);
	int countInt = NE::IntValue::Get (count);
	NE::ListValuePtr list (new NE::ListValue ());
	for (int i = 0; i < countInt; ++i) {
		list->Push (NE::ValuePtr (new NE::IntValue (startInt + i * stepInt)));
	}

	return list;
}

NE::Stream::Status IntegerRangeNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	NormalUINode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status IntegerRangeNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	NormalUINode::Write (outputStream);
	return outputStream.GetStatus ();
}

}
