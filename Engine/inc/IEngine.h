#pragma once
#include <filesystem>

#include <EngineContext.h>
#include <EngineSettings.h>

namespace REC
{
	// Preporation for dll
	class IEngine
	{
	public:
		virtual void Run(void(*load)(IEngine*)) = 0;
		virtual ~IEngine() = default;

		virtual const EngineContext& GetContext() const = 0;
		virtual void SetEngineSettings(const EngineSettings& data) = 0;
	};

	extern "C"
	{
		IEngine* CreateEngine(const std::filesystem::path& dataPath);
		void DestroyEngine(IEngine* engine);
	}
}