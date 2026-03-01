#pragma once
#include "RenderComponent.h"

namespace REC
{
	class GameObject;

	class TrashTheCacheRenderComponent final : public RenderComponent
	{
	public:
		explicit TrashTheCacheRenderComponent(GameObject* owner);
		virtual ~TrashTheCacheRenderComponent() = default;

		TrashTheCacheRenderComponent(const TrashTheCacheRenderComponent& other) = delete;
		TrashTheCacheRenderComponent(TrashTheCacheRenderComponent&& other) = delete;
		TrashTheCacheRenderComponent& operator=(const TrashTheCacheRenderComponent& other) = delete;
		TrashTheCacheRenderComponent& operator=(TrashTheCacheRenderComponent&& other) = delete;

		virtual void Update(float deltaT) override;
		virtual void Render() override;

	private:
		struct transform
		{
			float matrix[16] = {
				1,0,0,0,
				0,1,0,0,
				0,0,1,0,
				0,0,0,1
			};
		};

		class gameObject
		{
		public:
			transform local{};
			int id{};
		};

		class gameObjectAlt
		{
		public:
			transform* local{};
			int id{};
		};

		bool m_ShouldCalculateInt{ false };

		int m_SampleCountInt{ 10 };
		int m_AmountOfIntObjects{ 10000000 };
		std::vector<float> m_IntPlotData{};
		std::vector<int> m_IntArr{};

		bool m_ShouldCalculateObj{ false };
		bool m_ShouldCalculateAltObj{ false };

		int m_SampleCountObj{ 100 };
		int m_AmountOfObjects{ 10000000 };

		std::vector<float> m_ObjPlotData{};
		std::vector<gameObject> m_ObjArr{};

		std::vector<float> m_AltObjPlotData{};
		std::vector<gameObjectAlt> m_AltObjArr{};

		void TrashTheCash(std::vector<float>& data, size_t arrSize, int samples, const std::function<int(TrashTheCacheRenderComponent*, size_t)>& getInt);

		static int GetInt(TrashTheCacheRenderComponent* me, size_t index)
		{
			return ++me->m_IntArr[index];
		}

		static int GetIntObj(TrashTheCacheRenderComponent* me, size_t index)
		{
			return ++me->m_ObjArr[index].id;
		}

		static int GetIntObjAlt(TrashTheCacheRenderComponent* me, size_t index)
		{
			return ++me->m_AltObjArr[index].id;
		}
	};
}