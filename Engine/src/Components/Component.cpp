#include <Components/Component.h>
#include <GameObject.h>
#include <stdexcept>

REC::Component::Component(GameObject *const owner)
	: m_pOwner(owner)
{
	if (m_pOwner == nullptr)
		throw std::runtime_error("Cannot have an invalid owner!");
}

void REC::Component::Destroy()
{
	m_IsAboutToBeDestroyed = true;
	m_pOwner->ShouldCleanUpComponents();
}
