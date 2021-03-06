#include "SimpleTest.hpp"
#include "NE_MemoryStream.hpp"
#include "NUIE_NodeEditor.hpp"
#include "BI_InputUINodes.hpp"
#include "VisualTestFramework.hpp"

using namespace NE;
using namespace NUIE;
using namespace BI;

namespace NodeEditorTest
{

TEST (NodeEditorNeedToSaveTest)
{
	NodeEditorTestEnv env (GetDefaultSkinParams ());
	ASSERT (!env.nodeEditor.NeedToSave ());

	env.nodeEditor.AddNode (UINodePtr (new IntegerUpDownNode (L"Integer", Point (0.0, 0.0), 0, 1)));
	ASSERT (env.nodeEditor.NeedToSave ());
	
	MemoryOutputStream outputStream;
	ASSERT (env.nodeEditor.Save (outputStream, nullptr));
	ASSERT (!env.nodeEditor.NeedToSave ());
	
	env.nodeEditor.New ();
	ASSERT (!env.nodeEditor.NeedToSave ());
	env.nodeEditor.AddNode (UINodePtr (new IntegerUpDownNode (L"Integer", Point (0.0, 0.0), 0, 1)));
	ASSERT (env.nodeEditor.NeedToSave ());

	MemoryInputStream inputStream (outputStream.GetBuffer ());
	ASSERT (env.nodeEditor.Open (inputStream, nullptr));
	ASSERT (!env.nodeEditor.NeedToSave ());	
	env.nodeEditor.AddNode (UINodePtr (new IntegerUpDownNode (L"Integer", Point (0.0, 0.0), 0, 1)));
	ASSERT (env.nodeEditor.NeedToSave ());
}

}
