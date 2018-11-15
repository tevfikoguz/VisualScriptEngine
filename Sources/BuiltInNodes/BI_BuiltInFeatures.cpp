#include "BI_BuiltInFeatures.hpp"
#include "NUIE_NodeUIManager.hpp"
#include "NUIE_ContextDecorators.hpp"
#include "NUIE_SkinParams.hpp"
#include "NUIE_UINodeCommonParameters.hpp"

namespace BI
{

const NUIE::FeatureId EnableDisableFeatureId ("{A60BA8C8-ADFB-48D2-A112-ADD99F0B6CE7}");
const NUIE::FeatureId ValueCombinationFeatureId ("{B8F03216-5CB8-49FC-B748-94479BD2C8CA}");

NE::DynamicSerializationInfo EnableDisableFeature::serializationInfo (NE::ObjectId ("{1C89FD8B-085E-45C8-B0B8-E75883F53C68}"), NE::ObjectVersion (1), EnableDisableFeature::CreateSerializableInstance);
NE::DynamicSerializationInfo ValueCombinationFeature::serializationInfo (NE::ObjectId ("{7BC21A4E-4D2E-4B00-BD73-9897DB3616BA}"), NE::ObjectVersion (1), ValueCombinationFeature::CreateSerializableInstance);

static void EnableDisableNode (bool enable, NUIE::NodeUIManager& uiManager, NE::EvaluationEnv& env, NUIE::UINodePtr& uiNode)
{
	std::shared_ptr<EnableDisableFeature> enableDisableFeature = GetEnableDisableFeature (uiNode);
	if (DBGERROR (enableDisableFeature == nullptr)) {
		return;
	}
	enableDisableFeature->SetEnableState (enable);
	uiNode->OnFeatureChange (EnableDisableFeatureId, env);
	uiManager.InvalidateNodeDrawing (uiNode);
	uiManager.RequestRecalculate ();
}

class EnableDisableNodeCommand : public NUIE::NodeCommand
{
public:
	EnableDisableNodeCommand (const std::wstring& name, bool isChecked, bool enable) :
		NUIE::NodeCommand (name, isChecked),
		enable (enable)
	{

	}

	virtual bool IsApplicableTo (const NUIE::UINodePtr& uiNode) override
	{
		return uiNode->HasFeature (EnableDisableFeatureId);
	}

	virtual void Do (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& uiEnvironment, NUIE::UINodePtr& uiNode) override
	{
		EnableDisableNode (enable, uiManager, uiEnvironment.GetEvaluationEnv (), uiNode);
	}

private:
	bool enable;
};

static void SetNodeValueCombination (NE::ValueCombinationMode valueCombination, NUIE::NodeUIManager& uiManager, NE::EvaluationEnv& env, NUIE::UINodePtr& uiNode)
{
	std::shared_ptr<ValueCombinationFeature> valueCombinationFeature = GetValueCombinationFeature (uiNode);
	if (DBGERROR (valueCombinationFeature == nullptr)) {
		return;
	}
	valueCombinationFeature->SetValueCombinationMode (valueCombination);
	uiNode->OnFeatureChange (ValueCombinationFeatureId, env);
	uiManager.InvalidateNodeValue (uiNode);
	uiManager.InvalidateNodeDrawing (uiNode);
	uiManager.RequestRecalculate ();
}

class SetValueCombinationModeCommand : public NUIE::NodeCommand
{
public:
	SetValueCombinationModeCommand (const std::wstring& name, bool isChecked, NE::ValueCombinationMode valueCombination) :
		NUIE::NodeCommand (name, isChecked),
		valueCombination (valueCombination)
	{

	}

	virtual bool IsApplicableTo (const NUIE::UINodePtr& uiNode) override
	{
		return uiNode->HasFeature (ValueCombinationFeatureId);
	}

	virtual void Do (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& env, NUIE::UINodePtr& uiNode) override
	{
		SetNodeValueCombination (valueCombination, uiManager, env.GetEvaluationEnv (), uiNode);
	}

private:
	NE::ValueCombinationMode valueCombination;
};

EnableDisableFeature::EnableDisableFeature () :
	EnableDisableFeature (true)
{

}

EnableDisableFeature::EnableDisableFeature (bool nodeEnabled) :
	NUIE::UINodeFeature (EnableDisableFeatureId),
	nodeEnabled (nodeEnabled)
{

}

EnableDisableFeature::~EnableDisableFeature ()
{

}

bool EnableDisableFeature::GetEnableState () const
{
	return nodeEnabled;
}

void EnableDisableFeature::SetEnableState (bool isNodeEnabled)
{
	nodeEnabled = isNodeEnabled;
}

void EnableDisableFeature::RegisterCommands (NUIE::NodeCommandRegistrator& commandRegistrator) const
{
	NUIE::NodeGroupCommandPtr setNodeStatusGroup (new NUIE::NodeGroupCommand<NUIE::NodeCommandPtr> (L"Set Node Status"));
	setNodeStatusGroup->AddChildCommand (NUIE::NodeCommandPtr (new EnableDisableNodeCommand (L"Enable", nodeEnabled, true)));
	setNodeStatusGroup->AddChildCommand (NUIE::NodeCommandPtr (new EnableDisableNodeCommand (L"Disable", !nodeEnabled, false)));
	commandRegistrator.RegisterNodeGroupCommand (setNodeStatusGroup);
}

void EnableDisableFeature::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	class EnableDisableParameter : public NUIE::EnumerationFeatureParameter<EnableDisableFeature>
	{
	public:
		EnableDisableParameter () :
			NUIE::EnumerationFeatureParameter<EnableDisableFeature> (L"Status", { L"Enable", L"Disable" }, EnableDisableFeatureId)
		{

		}

		virtual NE::ValuePtr GetValueInternal (const NUIE::UINodePtr& uiNode) const override
		{
			bool enableState = GetEnableDisableFeature (uiNode)->GetEnableState ();
			int enableStateInt = (enableState ? 0 : 1);
			return NE::ValuePtr (new NE::IntValue (enableStateInt));
		}

		virtual bool SetValueInternal (NUIE::NodeUIManager& uiManager, NE::EvaluationEnv& evaluationEnv, NUIE::UINodePtr& uiNode, const NE::ValuePtr& value) override
		{
			int enableStateInt = NE::IntValue::Get (value);
			bool enableState = (enableStateInt == 0 ? true : false);
			EnableDisableNode (enableState, uiManager, evaluationEnv, uiNode);
			return true;
		}
	};

	parameterList.AddParameter (NUIE::NodeParameterPtr (new EnableDisableParameter ()));
}

void EnableDisableFeature::DrawInplace (NUIE::NodeUIDrawingEnvironment& env, const std::function<void (NUIE::NodeUIDrawingEnvironment&)>& drawer) const
{
	if (nodeEnabled) {
		drawer (env);
	} else {
		NUIE::ColorBlenderContextDecorator disabledContext (env.GetDrawingContext (), env.GetSkinParams ().GetBackgroundColor ());
		NUIE::NodeUIDrawingEnvironmentContextDecorator disabledEnv (env, disabledContext);
		drawer (disabledEnv);
	}
}

NE::Stream::Status EnableDisableFeature::Read (NE::InputStream & inputStream)
{
	NE::ObjectHeader header (inputStream);
	UINodeFeature::Read (inputStream);
	inputStream.Read (nodeEnabled);
	return inputStream.GetStatus ();
}

NE::Stream::Status EnableDisableFeature::Write (NE::OutputStream & outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	UINodeFeature::Write (outputStream);
	outputStream.Write (nodeEnabled);
	return outputStream.GetStatus ();
}

ValueCombinationFeature::ValueCombinationFeature () :
	ValueCombinationFeature (NE::ValueCombinationMode::Longest)
{

}

ValueCombinationFeature::ValueCombinationFeature (NE::ValueCombinationMode valueCombinationMode) :
	NUIE::UINodeFeature (ValueCombinationFeatureId),
	valueCombinationMode (valueCombinationMode)
{

}

ValueCombinationFeature::~ValueCombinationFeature ()
{

}

NE::ValueCombinationMode ValueCombinationFeature::GetValueCombinationMode () const
{
	return valueCombinationMode;
}

void ValueCombinationFeature::SetValueCombinationMode (NE::ValueCombinationMode newValueCombinationMode)
{
	valueCombinationMode = newValueCombinationMode;
}

bool ValueCombinationFeature::CombineValues (const std::vector<NE::ValuePtr>& values, const std::function<bool (const NE::ValueCombination&)>& processor) const
{
	return NE::CombineValues (valueCombinationMode, values, processor);
}

void ValueCombinationFeature::RegisterCommands (NUIE::NodeCommandRegistrator& commandRegistrator) const
{
	NUIE::NodeGroupCommandPtr setValueCombinationModeGroup (new NUIE::NodeGroupCommand<NUIE::NodeCommandPtr> (L"Set Value Combination"));
	setValueCombinationModeGroup->AddChildCommand (NUIE::NodeCommandPtr (new SetValueCombinationModeCommand (L"Shortest", valueCombinationMode == NE::ValueCombinationMode::Shortest, NE::ValueCombinationMode::Shortest)));
	setValueCombinationModeGroup->AddChildCommand (NUIE::NodeCommandPtr (new SetValueCombinationModeCommand (L"Longest", valueCombinationMode == NE::ValueCombinationMode::Longest, NE::ValueCombinationMode::Longest)));
	setValueCombinationModeGroup->AddChildCommand (NUIE::NodeCommandPtr (new SetValueCombinationModeCommand (L"Cross Product", valueCombinationMode == NE::ValueCombinationMode::CrossProduct, NE::ValueCombinationMode::CrossProduct)));
	commandRegistrator.RegisterNodeGroupCommand (setValueCombinationModeGroup);
}

void ValueCombinationFeature::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	class ValueCombinationParameter : public NUIE::EnumerationFeatureParameter<ValueCombinationFeature>
	{
	public:
		ValueCombinationParameter () :
			NUIE::EnumerationFeatureParameter<ValueCombinationFeature> (L"Value Combination", { L"Shortest", L"Longest", L"Cross Product" }, ValueCombinationFeatureId)
		{
		
		}

		virtual NE::ValuePtr GetValueInternal (const NUIE::UINodePtr& uiNode) const override
		{
			NE::ValueCombinationMode valueCombination = GetValueCombinationFeature (uiNode)->GetValueCombinationMode ();
			int valueCombinationInt = (int) valueCombination;
			return NE::ValuePtr (new NE::IntValue (valueCombinationInt));
		}
		
		virtual bool SetValueInternal (NUIE::NodeUIManager& uiManager, NE::EvaluationEnv& evaluationEnv, NUIE::UINodePtr& uiNode, const NE::ValuePtr& value) override
		{
			int valueCombinationInt = NE::IntValue::Get (value);
			NE::ValueCombinationMode valueCombination = (NE::ValueCombinationMode) valueCombinationInt;
			SetNodeValueCombination (valueCombination, uiManager, evaluationEnv, uiNode);
			return true;
		}
	};

	parameterList.AddParameter (NUIE::NodeParameterPtr (new ValueCombinationParameter ()));
}

NE::Stream::Status ValueCombinationFeature::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	UINodeFeature::Read (inputStream);
	size_t valueCombinationModeInt = 0;
	inputStream.Read (valueCombinationModeInt);
	valueCombinationMode = (NE::ValueCombinationMode) valueCombinationModeInt;
	return inputStream.GetStatus ();
}

NE::Stream::Status ValueCombinationFeature::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	UINodeFeature::Write (outputStream);
	size_t valueCombinationModeInt = (size_t) valueCombinationMode;
	outputStream.Write (valueCombinationModeInt);
	return outputStream.GetStatus ();
}

std::shared_ptr<EnableDisableFeature> GetEnableDisableFeature (const NUIE::UINode* uiNode)
{
	return NUIE::GetUINodeFeature<EnableDisableFeature> (uiNode, EnableDisableFeatureId);
}

std::shared_ptr<EnableDisableFeature> GetEnableDisableFeature (const NUIE::UINodePtr& uiNode)
{
	return NUIE::GetUINodeFeature<EnableDisableFeature> (uiNode, EnableDisableFeatureId);
}

std::shared_ptr<ValueCombinationFeature> GetValueCombinationFeature (const NUIE::UINode* uiNode)
{
	return NUIE::GetUINodeFeature<ValueCombinationFeature> (uiNode, ValueCombinationFeatureId);
}

std::shared_ptr<ValueCombinationFeature> GetValueCombinationFeature (const NUIE::UINodePtr& uiNode)
{
	return NUIE::GetUINodeFeature<ValueCombinationFeature> (uiNode, ValueCombinationFeatureId);
}

}