#pragma once
#include <filesystem>
#include <string>
#include <memory>
#include <map>
#include "Singleton.h"
#include <nlohmann/json.hpp>

namespace dae
{
	class Texture2D;
	class Font;
	class SpriteSheet;

	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		void Init(const std::filesystem::path& data);
		std::shared_ptr<Texture2D> LoadTexture(const std::string& file);
		std::shared_ptr<Font> LoadFont(const std::string& file, uint8_t size);
		std::shared_ptr<SpriteSheet> LoadSpriteSheet(const std::string& file, int frameWidth, int frameHeight);
		nlohmann::json LoadJson(const std::string& file);

		std::filesystem::path GetDataPath() { return m_dataPath; };
	private:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;
		std::filesystem::path m_dataPath;

		void UnloadUnusedResources();

		std::map<std::string, std::shared_ptr<Texture2D>> m_loadedTextures;
		std::map<std::pair<std::string, uint8_t>, std::shared_ptr<Font>> m_loadedFonts;
		std::unordered_map<std::string, std::shared_ptr<SpriteSheet>> m_SpriteSheets;
	};
}
