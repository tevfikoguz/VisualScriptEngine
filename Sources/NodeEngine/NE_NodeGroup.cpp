#include "NE_NodeGroup.hpp"
#include "NE_MemoryStream.hpp"

namespace NE
{

SerializationInfo NodeGroup::serializationInfo (ObjectVersion (1));

NodeGroup::NodeGroup ()
{

}

NodeGroup::~NodeGroup ()
{

}

Stream::Status NodeGroup::Read (InputStream& inputStream)
{
	ObjectHeader header (inputStream);
	return inputStream.GetStatus ();
}

Stream::Status NodeGroup::Write (OutputStream& outputStream) const
{
	ObjectHeader header (outputStream, serializationInfo);
	return outputStream.GetStatus ();
}

NodeGroupPtr NodeGroup::Clone (const NodeGroupConstPtr& node)
{
	MemoryOutputStream outputStream;
	WriteDynamicObject (outputStream, node.get ());
	if (DBGERROR (node->Write (outputStream) != Stream::Status::NoError)) {
		return nullptr;
	}

	MemoryInputStream inputStream (outputStream.GetBuffer ());
	NodeGroupPtr result (ReadDynamicObject<NodeGroup> (inputStream));
	if (DBGERROR (result == nullptr)) {
		return nullptr;
	}

	return result;
}

}
