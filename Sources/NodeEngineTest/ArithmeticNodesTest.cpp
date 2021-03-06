#include "SimpleTest.hpp"
#include "NUIE_NodeUIManager.hpp"
#include "BI_ArithmeticUINodes.hpp"
#include "BI_InputUINodes.hpp"
#include "TestUtils.hpp"

using namespace NE;
using namespace NUIE;
using namespace BI;

namespace ArithmeticNodesTest
{

TEST (TestAllArithmeticNodes)
{
	TestDrawingEnvironment env;
	NodeUIManager uiManager (env);

	UINodePtr val1 = uiManager.AddNode (UINodePtr (new DoubleUpDownNode (L"Value1", Point (0, 0), 1.0, 1.0)), EmptyEvaluationEnv);
	UINodePtr val2 = uiManager.AddNode (UINodePtr (new DoubleUpDownNode (L"Value1", Point (0, 0), 2.0, 1.0)), EmptyEvaluationEnv);
	UINodePtr val3 = uiManager.AddNode (UINodePtr (new DoubleUpDownNode (L"Value3", Point (0, 0), 3.0, 1.0)), EmptyEvaluationEnv);
	UINodePtr add = uiManager.AddNode (UINodePtr (new AdditionNode (L"Addition", Point (0, 0))), EmptyEvaluationEnv);
	UINodePtr sub = uiManager.AddNode (UINodePtr (new SubtractionNode (L"Subtracion", Point (0, 0))), EmptyEvaluationEnv);
	UINodePtr mul = uiManager.AddNode (UINodePtr (new MultiplicationNode (L"Multiplication", Point (0, 0))), EmptyEvaluationEnv);
	UINodePtr div = uiManager.AddNode (UINodePtr (new DivisionNode (L"Division", Point (0, 0))), EmptyEvaluationEnv);

	uiManager.ConnectOutputSlotToInputSlot (val1->GetUIOutputSlot (NE::SlotId ("out")), add->GetUIInputSlot (NE::SlotId ("a")));
	uiManager.ConnectOutputSlotToInputSlot (val2->GetUIOutputSlot (NE::SlotId ("out")), add->GetUIInputSlot (NE::SlotId ("b")));
	uiManager.ConnectOutputSlotToInputSlot (val1->GetUIOutputSlot (NE::SlotId ("out")), sub->GetUIInputSlot (NE::SlotId ("a")));
	uiManager.ConnectOutputSlotToInputSlot (val2->GetUIOutputSlot (NE::SlotId ("out")), sub->GetUIInputSlot (NE::SlotId ("b")));
	uiManager.ConnectOutputSlotToInputSlot (add->GetUIOutputSlot (NE::SlotId ("result")), mul->GetUIInputSlot (NE::SlotId ("a")));
	uiManager.ConnectOutputSlotToInputSlot (sub->GetUIOutputSlot (NE::SlotId ("result")), mul->GetUIInputSlot (NE::SlotId ("b")));
	uiManager.ConnectOutputSlotToInputSlot (mul->GetUIOutputSlot (NE::SlotId ("result")), div->GetUIInputSlot (NE::SlotId ("a")));
	uiManager.ConnectOutputSlotToInputSlot (val3->GetUIOutputSlot (NE::SlotId ("out")), div->GetUIInputSlot (NE::SlotId ("b")));

	ValueConstPtr val = div->Evaluate (EmptyEvaluationEnv);
	ASSERT (IsSingleType<DoubleValue> (val));
	ASSERT (IsEqual (DoubleValue::Get (CreateSingleValue (val)), -1.0));
}

}
