#include "../../inc/Components/GridComponent.h"

REC::GridComponent::GridComponent(GameObject* owner, const GridDescriptor& desc)
	: Component(owner)
	, m_Descriptor{desc}
{

}

void REC::GridComponent::Update(float) {}

