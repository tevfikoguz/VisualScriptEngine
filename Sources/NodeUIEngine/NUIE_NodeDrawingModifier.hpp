#ifndef NUIE_NODEDRAWINGMODIFIER_HPP
#define NUIE_NODEDRAWINGMODIFIER_HPP

#include "NE_NodeId.hpp"
#include "NE_SlotId.hpp"
#include "NUIE_Geometry.hpp"
#include <functional>

namespace NUIE
{

class NodeDrawingModifier
{
public:
	NodeDrawingModifier ();
	virtual ~NodeDrawingModifier ();

	virtual void	EnumerateSelectionRectangles (const std::function<void (const Rect&)>& processor) const = 0;
	virtual void	EnumerateTemporaryConnections (const std::function<void (const Point&, const Point&)>& processor) const = 0;
	virtual bool	NeedToDrawConnection (const NE::NodeId& outputNodeId, const NE::SlotId& outputSlotId, const NE::NodeId& inputNodeId, const NE::SlotId& inputSlotId) const = 0;
	virtual Point	GetNodeOffset (const NE::NodeId& nodeId) const = 0;
};

}

#endif
