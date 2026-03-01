#include "../../inc/GameObject.h"
#include <Components/TrashTheCacheRenderComponent.h>
#include "../Renderer.h"

#include <chrono>
#include <numeric>
#include <iostream>
#include <algorithm>
#include <random>

REC::TrashTheCacheRenderComponent::TrashTheCacheRenderComponent(GameObject* owner)
	: RenderComponent(owner)
{
	m_IntArr.resize(m_AmountOfIntObjects);
	m_ObjArr.resize(m_AmountOfObjects);
	m_AltObjArr.resize(m_AmountOfObjects);
}

void REC::TrashTheCacheRenderComponent::Update(float)
{
	if (m_ShouldCalculateInt)
	{
		TrashTheCash(m_IntPlotData, m_AmountOfIntObjects, m_SampleCountInt, GetInt);
		m_ShouldCalculateInt = false;
	}
	if (m_ShouldCalculateObj)
	{
		TrashTheCash(m_ObjPlotData, m_AmountOfObjects, m_SampleCountObj, GetIntObj);
		m_ShouldCalculateObj = false;
	}
	if (m_ShouldCalculateAltObj)
	{
		TrashTheCash(m_AltObjPlotData, m_AmountOfObjects, m_SampleCountObj, GetIntObjAlt);
		m_ShouldCalculateAltObj = false;
	}
}

void REC::TrashTheCacheRenderComponent::Render()
{
	auto& r = Renderer::GetInstance();

	if (r.RenderPlotWindow("exercise 1", m_SampleCountInt, m_IntPlotData))
		m_ShouldCalculateInt = true;
	
	r.RenderPlotsWindow("exercise 2", m_SampleCountObj, m_ShouldCalculateObj, m_ShouldCalculateAltObj, m_ObjPlotData, m_AltObjPlotData);
}


void REC::TrashTheCacheRenderComponent::TrashTheCash(std::vector<float>& data, size_t arrSize, int samples, const std::function<int(TrashTheCacheRenderComponent*, size_t)>& getInt)
{
	data.clear();
	std::vector<int> copy(arrSize);
	for (size_t stepSize = 1; stepSize <= 1024; stepSize *= 2)
	{
		std::vector<float> totalTimes{};
		totalTimes.reserve(samples);
		for (size_t s{}; s < samples; ++s)
		{
			auto start = std::chrono::high_resolution_clock::now();
			for (size_t i = 0; i < arrSize; i += stepSize)
			{
				copy[i] = getInt(this, i);
			}
			auto end = std::chrono::high_resolution_clock::now();

			const std::chrono::duration<float, std::milli> elapsedSeconds = end - start;
			totalTimes.push_back(elapsedSeconds.count());
		}
		std::cout << std::accumulate(copy.begin(), copy.end(), 0) << " ";

		std::sort(totalTimes.begin(), totalTimes.end());
		float average = std::accumulate(totalTimes.begin() + 1, totalTimes.end() - 1, 0.f) / (totalTimes.size() - 2);
		//float average = std::accumulate(totalTimes.begin(), totalTimes.end(), 0.0f) / totalTimes.size();
		data.push_back(average);
	}
}
