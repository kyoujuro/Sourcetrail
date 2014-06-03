#include "utility/ConfigManager.h"

#include "logging/logging.h"
#include "utility/text/TextAccess.h"

std::shared_ptr<ConfigManager> ConfigManager::createAndLoad(const std::shared_ptr<TextAccess> textAccess)
{
	std::shared_ptr<ConfigManager> configManager = std::shared_ptr<ConfigManager>(new ConfigManager(textAccess));
	configManager->load();
	return configManager;
}

bool ConfigManager::getValue(const std::string& key, std::string& value) const
{
	std::map<std::string, std::string>::const_iterator it = m_values.find(key);

	if(it != m_values.end())
	{
		value = it->second;
		return true;
	}
	else
	{
		// LOG ERROR
		return false;
	}
}

bool ConfigManager::getValue(const std::string& key, int& value) const
{
	std::string valueString;
	if (getValue(key, valueString))
	{
		value = atoi(valueString.c_str());
		return true;
	}
	return false;
}

bool ConfigManager::getValue(const std::string& key, float& value) const
{
	std::string valueString;
	if (getValue(key, valueString))
	{
		value = static_cast<float>(atof(valueString.c_str()));
		return true;
	}
	return false;
}

bool ConfigManager::getValue(const std::string& key, bool& value) const
{
	std::string valueString;
	if (getValue(key, valueString))
	{
		value = (atoi(valueString.c_str()) != 0);
		return true;
	}
	return false;
}

void ConfigManager::setValue(const std::string& key, const std::string& value)
{
	std::map<std::string, std::string>::iterator it = m_values.find(key);

	if(it != m_values.end())
	{
		it->second = value;
	}
	else
	{
		// LOG ERROR
	}
}

void ConfigManager::setValue(const std::string& key, const int value)
{
	setValue(key, std::to_string(value));
}

void ConfigManager::setValue(const std::string& key, const float value)
{
	setValue(key, std::to_string(value));
}

void ConfigManager::setValue(const std::string& key, const bool value)
{
	setValue(key, (value ? "1" : "0"));
}


void ConfigManager::load()
{
	std::string text = m_textAccess->getText();

	TiXmlDocument doc;
	const char* pTest = doc.Parse(text.c_str(), 0, TIXML_ENCODING_UTF8);
	if (pTest != NULL)
	{
		TiXmlHandle docHandle(&doc);
		TiXmlNode *rootNode = docHandle.FirstChild("config").ToNode();
		for(TiXmlNode *childNode = rootNode->FirstChild(); childNode; childNode = childNode->NextSibling())
		{
			parseSubtree(childNode, "");
		}
	}
	else
	{
		// LOG ERROR Unable to load file.
	}
}

void ConfigManager::save()
{
	LOG_ERROR("function: configmanager::save not implemented");
	// LOG ("Saving config to file: " + m_filePath)
}

ConfigManager::ConfigManager(const std::shared_ptr<TextAccess> textAccess)
	: m_textAccess(textAccess)
{
}

void ConfigManager::parseSubtree(TiXmlNode* currentNode, const std::string& currentPath)
{
	if (currentNode->Type() == TiXmlNode::TINYXML_TEXT)
	{
		std::string key = currentPath.substr(0, currentPath.size() - 1);
		m_values[key] = currentNode->ToText()->Value();
	}
	else
	{
		for(TiXmlNode *childNode = currentNode->FirstChild(); childNode; childNode = childNode->NextSibling())
		{
			parseSubtree(childNode, currentPath + std::string(currentNode->Value()) + "/");
		}
	}
}
