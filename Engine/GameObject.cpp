#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "inc/TransformComponent.h"

REC::GameObject::~GameObject() = default;

void REC::GameObject::Update(float){}

void REC::GameObject::Render() const
{
	auto* transform = GetComponent<TransformComponent>();
	glm::vec2 pos{};
	if (transform) pos = transform->GetPosition();
	Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
}

void REC::GameObject::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}
