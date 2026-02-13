#include "../inc/Component.h"
#include "../GameObject.h"

void REC::Component::Destroy()
{
	m_IsAboutToBeDestroyed = true;
	m_pOwner->ShouldCleanUpComponents();
}