#include "DataFile.h"

#include <Resources/ResourceCreateInfos.h>

REC::DataFile::DataFile(const std::string& filePath)
	: m_FileStream{std::ifstream(filePath)}
	, m_Parser{std::make_unique<JSONParser>()}
{
}

REC::DataFile::~DataFile()
{
	m_FileStream.close();
}

void REC::DataFile::Parse(LoadTypes types)
{
	// TODO: data management, store FrameInfo, Animations, and textureFont in this file or ResourceManager?
	m_Parser->LoadFromFile(m_FileStream);

	if (uint8_t(types & LoadTypes::Frames))
	{
		m_Parser->GetFrames(m_FrameInfos);
	}

	if (uint8_t(types & LoadTypes::Animations))
	{
		m_Parser->GetAnimations(m_AnimationInfos);
	}

	if (uint8_t(types & LoadTypes::TextureFont))
	{
		m_Parser->GetTextureFonts(m_TextureFontInfos);
	}

	if (uint8_t(types & LoadTypes::Levels))
	{
		m_Parser->GetLevels(m_Levels);
	}
}
