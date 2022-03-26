#include "MonsterEngine/Serializer/Ini.h"

#include <algorithm>
#include <fstream>
#include <sstream>

namespace Serializer
{
	IniValueTypeError::IniValueTypeError(std::string_view msg)
	    : std::runtime_error(std::string(msg)) {}

	IniValueTypeError::IniValueTypeError(const char* msg)
	    : std::runtime_error(msg) {}

	IniParseError::IniParseError(std::string_view msg, size_t line)
	    : std::runtime_error(std::string(msg) + ", at line: " + std::to_string(line)) {}

	IniParseError::IniParseError(const char* msg, size_t line)
	    : std::runtime_error(std::string(msg) + ", at line: " + std::to_string(line)) {}

	Ini::Ini(IniSection* section) : section(section) {}

	Ini::~Ini()
	{
		clear();
	}

	std::string Ini::getFullName() const
	{
		if (this->section)
			return this->section->getFullName();
		return "";
	}

	Ini* Ini::getParent()
	{
		if (this->section)
			return this->section->getParent();
		return nullptr;
	}

	const Ini* Ini::getParent() const
	{
		if (this->section)
			return this->section->getParent();
		return nullptr;
	}

	IniSection* Ini::addSection(std::string_view name)
	{
		for (auto itr = this->sections.begin(), end = this->sections.end(); itr != end; itr++)
			if ((*itr)->getName() == name)
				return (*itr);

		IniSection* section = new IniSection(name, this);
		this->sections.push_back(section);
		return section;
	}

	IniEntry* Ini::addEntry(std::string_view name, std::string_view value)
	{
		for (auto itr = this->entries.begin(), end = this->entries.end(); itr != end; itr++)
			if ((*itr)->getName() == name)
				return (*itr);

		IniEntry* entry = new IniEntry(name, value, this);
		this->entries.push_back(entry);
		return entry;
	}

	void Ini::removeSection(std::string_view name)
	{
		auto itr = std::find_if(this->sections.begin(), this->sections.end(), [name](const IniSection* section) -> bool
		                        { return section->getName() == name; });
		if (itr != this->sections.end())
			this->sections.erase(itr);
	}

	void Ini::removeEntry(std::string_view name)
	{
		auto itr = std::find_if(this->entries.begin(), this->entries.end(), [name](const IniEntry* entry) -> bool
		                        { return entry->getName() == name; });
		if (itr != this->entries.end())
			this->entries.erase(itr);
	}

	void Ini::clearSections()
	{
		for (auto section : this->sections)
			delete section;
		this->sections.clear();
	}

	void Ini::clearEntries()
	{
		for (auto entry : this->entries)
			delete entry;
		this->entries.clear();
	}

	IniSection* Ini::getSection(std::string_view name)
	{
		auto itr = std::find_if(this->sections.begin(), this->sections.end(), [name](const IniSection* section) -> bool
		                        { return section->getName() == name; });
		if (itr != this->sections.end())
			return *itr;
		return nullptr;
	}

	const IniSection* Ini::getSection(std::string_view name) const
	{
		auto itr = std::find_if(this->sections.begin(), this->sections.end(), [name](const IniSection* section) -> bool
		                        { return section->getName() == name; });
		if (itr != this->sections.end())
			return *itr;
		return nullptr;
	}

	IniEntry* Ini::getEntry(std::string_view name)
	{
		auto itr = std::find_if(this->entries.begin(), this->entries.end(), [name](const IniEntry* entry) -> bool
		                        { return entry->getName() == name; });
		if (itr != this->entries.end())
			return *itr;
		return nullptr;
	}

	const IniEntry* Ini::getEntry(std::string_view name) const
	{
		auto itr = std::find_if(this->entries.begin(), this->entries.end(), [name](const IniEntry* entry) -> bool
		                        { return entry->getName() == name; });
		if (itr != this->entries.end())
			return *itr;
		return nullptr;
	}

	IniEntry* Ini::operator[](std::string_view name)
	{
		auto itr = std::find_if(this->entries.begin(), this->entries.end(), [name](const IniEntry* entry) -> bool
		                        { return entry->getName() == name; });
		if (itr != this->entries.end())
			return *itr;
		IniEntry* entry = new IniEntry(name, this);
		this->entries.push_back(entry);
		return entry;
	}

	const IniEntry* Ini::operator[](std::string_view name) const
	{
		auto itr = std::find_if(this->entries.begin(), this->entries.end(), [name](const IniEntry* entry) -> bool
		                        { return entry->getName() == name; });
		if (itr != this->entries.end())
			return *itr;
		return nullptr;
	}

	IniEntry::IniEntry(std::string_view name, Ini* ini)
	    : name(name), ini(ini) {}

	IniEntry::IniEntry(std::string_view name, std::string_view value, Ini* ini)
	    : name(name), value(value), ini(ini) {}

	IniEntry& IniEntry::operator=(std::string_view value)
	{
		this->value = value;
		return *this;
	}

	template <>
	int8_t IniEntry::getValueOfType()
	{
		int8_t             value;
		std::istringstream str(this->value);
		if (!(str >> value))
			throw IniValueTypeError("Value: \"" + this->value + "\" is not a number");
		return value;
	}

	template <>
	int16_t IniEntry::getValueOfType()
	{
		int16_t            value;
		std::istringstream str(this->value);
		if (!(str >> value))
			throw IniValueTypeError("Value: \"" + this->value + "\" is not a number");
		return value;
	}

	template <>
	int32_t IniEntry::getValueOfType()
	{
		int32_t            value;
		std::istringstream str(this->value);
		if (!(str >> value))
			throw IniValueTypeError("Value: \"" + this->value + "\" is not a number");
		return value;
	}

	template <>
	int64_t IniEntry::getValueOfType()
	{
		int64_t            value;
		std::istringstream str(this->value);
		if (!(str >> value))
			throw IniValueTypeError("Value: \"" + this->value + "\" is not a number");
		return value;
	}

	template <>
	uint8_t IniEntry::getValueOfType()
	{
		uint8_t            value;
		std::istringstream str(this->value);
		if (!(str >> value))
			throw IniValueTypeError("Value: \"" + this->value + "\" is not a number");
		return value;
	}

	template <>
	uint16_t IniEntry::getValueOfType()
	{
		uint16_t           value;
		std::istringstream str(this->value);
		if (!(str >> value))
			throw IniValueTypeError("Value: \"" + this->value + "\" is not a number");
		return value;
	}

	template <>
	uint32_t IniEntry::getValueOfType()
	{
		uint32_t           value;
		std::istringstream str(this->value);
		if (!(str >> value))
			throw IniValueTypeError("Value: \"" + this->value + "\" is not a number");
		return value;
	}

	template <>
	uint64_t IniEntry::getValueOfType()
	{
		uint64_t           value;
		std::istringstream str(this->value);
		if (!(str >> value))
			throw IniValueTypeError("Value: \"" + this->value + "\" is not a number");
		return value;
	}

	template <>
	bool IniEntry::tryGetValueOfType(int8_t& outValue) noexcept
	{
		std::istringstream str(this->value);
		return static_cast<bool>(str >> outValue);
	}

	template <>
	bool IniEntry::tryGetValueOfType(int16_t& outValue) noexcept
	{
		std::istringstream str(this->value);
		return static_cast<bool>(str >> outValue);
	}

	template <>
	bool IniEntry::tryGetValueOfType(int32_t& outValue) noexcept
	{
		std::istringstream str(this->value);
		return static_cast<bool>(str >> outValue);
	}

	template <>
	bool IniEntry::tryGetValueOfType(int64_t& outValue) noexcept
	{
		std::istringstream str(this->value);
		return static_cast<bool>(str >> outValue);
	}

	template <>
	bool IniEntry::tryGetValueOfType(uint8_t& outValue) noexcept
	{
		std::istringstream str(this->value);
		return static_cast<bool>(str >> outValue);
	}

	template <>
	bool IniEntry::tryGetValueOfType(uint16_t& outValue) noexcept
	{
		std::istringstream str(this->value);
		return static_cast<bool>(str >> outValue);
	}

	template <>
	bool IniEntry::tryGetValueOfType(uint32_t& outValue) noexcept
	{
		std::istringstream str(this->value);
		return static_cast<bool>(str >> outValue);
	}

	template <>
	bool IniEntry::tryGetValueOfType(uint64_t& outValue) noexcept
	{
		std::istringstream str(this->value);
		return static_cast<bool>(str >> outValue);
	}

	IniSection::IniSection(std::string_view name, Ini* parent)
	    : name(name), parent(parent) {}

	IniSection& IniSection::operator=(const Ini& ini)
	{
		this->ini = ini;
		return *this;
	}

	IniSection& IniSection::operator=(Ini&& ini)
	{
		this->ini = std::move(ini);
		return *this;
	}

	std::string IniSection::getFullName() const
	{
		if (this->parent)
		{
			std::string parentFullName = this->parent->getFullName();
			if (parentFullName.length() > 0)
				return parentFullName + "." + getName();
		}
		return getName();
	}

	static std::string printableToValue(std::string_view printable)
	{
		std::string output = std::string(printable);
		for (size_t i = 0; i < output.size(); i++)
		{
			switch (output[i])
			{
			case '\\':
			{
				char c = output[i + 1];
				switch (c)
				{
				case '\\':
					output.replace(i, 2, "\\");
					break;
				case '\'':
					output.replace(i, 2, "\'");
					break;
				case '\"':
					output.replace(i, 2, "\"");
					break;
				case '\0':
					output.replace(i, 2, "\0");
					break;
				case '\a':
					output.replace(i, 2, "\a");
					break;
				case '\b':
					output.replace(i, 2, "\b");
					break;
				case '\t':
					output.replace(i, 2, "\t");
					break;
				case '\r':
					output.replace(i, 2, "\r");
					break;
				case '\n':
					output.replace(i, 2, "\n");
					break;
				case '0':
					output.replace(i, 2, "\0");
					break;
				case 'a':
					output.replace(i, 2, "\a");
					break;
				case 'b':
					output.replace(i, 2, "\b");
					break;
				case 't':
					output.replace(i, 2, "\t");
					break;
				case 'r':
					output.replace(i, 2, "\r");
					break;
				case 'n':
					output.replace(i, 2, "\n");
					break;
				case ';':
					output.replace(i, 2, ";");
					break;
				case '#':
					output.replace(i, 2, "#");
					break;
				case '=':
					output.replace(i, 2, "=");
					break;
				case ':':
					output.replace(i, 2, ":");
					break;
				case 'x':
					int32_t  num              = 0;
					uint32_t unicodeCharacter = 0;
					for (; num < 4; num++)
						if (!std::isxdigit(output[i + 1 + num]))
							break;
					for (int32_t j = num - 1; j >= 0; j--)
					{
						char     xc = output[i + 1 + j];
						uint32_t v  = 0;
						if (std::isdigit(xc))
							v = static_cast<uint32_t>(xc - '0');
						else if (std::isupper(xc))
							v = static_cast<uint32_t>(xc - 'A');
						else if (std::islower(xc))
							v = static_cast<uint32_t>(xc - 'a');
						unicodeCharacter |= v << (4 * (num - 1 - j));
					}
					output.replace(i, num, reinterpret_cast<char*>(&unicodeCharacter));
					break;
				}
				break;
			}
			case '\"':
			{
				output.replace(i, 1, "");
				i--;
				break;
			}
			case '\'':
			{
				output.replace(i, 1, "");
				i--;
				break;
			}
			}
		}
		return output;
	}

	static std::string valueToPrintable(std::string_view value)
	{
		std::string output = std::string(value);
		for (size_t i = 0; i < output.size(); i++)
		{
			switch (output[i])
			{
			case '\\':
			case '\'':
			case '\"':
			case ';':
			case '#':
			case '=':
			case ':':
				output.insert(output.begin() + i, '\\');
				i++;
				break;
			case '\0':
				output[i] = '0';
				output.insert(output.begin() + i, '\\');
				i++;
				break;
			case '\a':
				output[i] = 'a';
				output.insert(output.begin() + i, '\\');
				i++;
				break;
			case '\b':
				output[i] = 'b';
				output.insert(output.begin() + i, '\\');
				i++;
				break;
			case '\t':
				output[i] = 't';
				output.insert(output.begin() + i, '\\');
				i++;
				break;
			case '\r':
				output[i] = 'r';
				output.insert(output.begin() + i, '\\');
				i++;
				break;
			case '\n':
				output[i] = 'n';
				output.insert(output.begin() + i, '\\');
				i++;
				break;
			}
		}

		if (std::isspace(output[0]) || std::isspace(output[output.size() - 1]))
			output = "\"" + output + "\"";
		return output;
	}

	void parse(std::string_view iniContent, Ini& outIni)
	{
		Ini*   currentIni = &outIni;
		size_t line       = 1;
		for (size_t i = 0; i < iniContent.size();)
		{
			switch (iniContent[i])
			{
			case '\r':
				i++;
				if (iniContent[i] != '\n')
					throw IniParseError("Expected \"Name = Value\" but got \"\\r\"", line);
				i++;
				line++;
				break;
			case '\n':
			{
				i++;
				line++;
				break;
			}
			case ';':
			{
				bool foundEnd = false;
				for (; i < iniContent.size(); i++)
				{
					switch (iniContent[i])
					{
					case '\\':
						i++;
						if (iniContent[i] == '\r')
							i++;
						if (iniContent[i] == '\n')
							line++;
						break;
					case '\n':
						line++;
						foundEnd = true;
						break;
					}
					if (foundEnd)
						break;
				}
				break;
			}
			case '[':
			{
				i++;
				size_t sectionNameBegin = i;
				bool   foundEnd         = false;
				for (; i < iniContent.size(); i++)
				{
					switch (iniContent[i])
					{
					case '"':
					{
						bool inside = true;
						for (; i < iniContent.size(); i++)
						{
							switch (iniContent[i])
							{
							case '"':
								inside = false;
								break;
							case '\\':
								i++;
								if (iniContent[i] == '\r')
									i++;
								if (iniContent[i] == '\n')
									line++;
								break;
							}
							if (!inside)
								break;
						}
						break;
					}
					case '\'':
					{
						bool inside = true;
						for (; i < iniContent.size(); i++)
						{
							switch (iniContent[i])
							{
							case '\'':
								inside = false;
								break;
							case '\\':
								i++;
								if (iniContent[i] == '\r')
									i++;
								if (iniContent[i] == '\n')
									line++;
								break;
							}
							if (!inside)
								break;
						}
						break;
					}
					case '\\':
						i++;
						if (iniContent[i] == '\r')
							i++;
						if (iniContent[i] == '\n')
							line++;
						break;
					case '\n':
						throw IniParseError("Expected ']' but got '\n' if that is intentional add '\\' before the end of the line", line);
					case ']':
						foundEnd = true;
						break;
					}
					if (foundEnd)
						break;
				}
				size_t sectionNameEnd = i - 1;

				for (; sectionNameBegin < sectionNameEnd; sectionNameBegin++)
					if (!std::isspace(iniContent[sectionNameBegin]))
						break;
				for (; sectionNameEnd > sectionNameBegin; sectionNameEnd--)
					if (!std::isspace(iniContent[sectionNameEnd]))
						break;
				std::string_view sectionNameV = iniContent.substr(sectionNameBegin, sectionNameEnd - sectionNameBegin + 1);
				if (sectionNameV.empty())
					throw IniParseError("Expected string but found nothing", line);

				std::string sectionName = printableToValue(sectionNameV);

				if (sectionName[0] == '.')
				{
					if (sectionName[1] == '.')
					{
						Ini* parent = currentIni->getParent();
						if (parent)
							currentIni = parent;
					}
					else
					{
						std::string fullSectionName = currentIni->getFullName();
						if (fullSectionName.empty())
							fullSectionName = sectionName.substr(1);
						else
							fullSectionName += sectionName;
						currentIni = &outIni.addSection(fullSectionName)->getIni();
					}
				}
				else
				{
					currentIni = &outIni.addSection(sectionName)->getIni();
				}

				i = iniContent.find_first_of('\n', i) + 1;
				line++;
				break;
			}
			default:
			{
				size_t nameBegin = i;
				bool   foundEnd  = false;
				for (; i < iniContent.size(); i++)
				{
					switch (iniContent[i])
					{
					case '"':
					{
						bool inside = true;
						for (; i < iniContent.size(); i++)
						{
							switch (iniContent[i])
							{
							case '"':
								inside = false;
								break;
							case '\\':
								i++;
								if (iniContent[i] == '\r')
									i++;
								if (iniContent[i] == '\n')
									line++;
								break;
							}
							if (!inside)
								break;
						}
						break;
					}
					case '\'':
					{
						bool inside = true;
						for (; i < iniContent.size(); i++)
						{
							switch (iniContent[i])
							{
							case '\'':
								inside = false;
								break;
							case '\\':
								i++;
								if (iniContent[i] == '\r')
									i++;
								if (iniContent[i] == '\n')
									line++;
								break;
							}
							if (!inside)
								break;
						}
						break;
					}
					case '\\':
						i++;
						if (iniContent[i] == '\r')
							i++;
						if (iniContent[i] == '\n')
							line++;
						break;
					case '\n':
						throw IniParseError("Expected '=' but got '\n' if that is intentional add '\\' before the end of the line", line);
					case '=':
						foundEnd = true;
						break;
					}
					if (foundEnd)
						break;
				}
				size_t nameEnd    = i - 1;
				size_t valueBegin = i + 1;
				foundEnd          = false;
				for (; i < iniContent.size(); i++)
				{
					switch (iniContent[i])
					{
					case '"':
					{
						bool inside = true;
						for (; i < iniContent.size(); i++)
						{
							switch (iniContent[i])
							{
							case '"':
								inside = false;
								break;
							case '\\':
								i++;
								if (iniContent[i] == '\r')
									i++;
								if (iniContent[i] == '\n')
									line++;
								break;
							}
							if (!inside)
								break;
						}
						break;
					}
					case '\'':
					{
						bool inside = true;
						for (; i < iniContent.size(); i++)
						{
							switch (iniContent[i])
							{
							case '\'':
								inside = false;
								break;
							case '\\':
								i++;
								if (iniContent[i] == '\r')
									i++;
								if (iniContent[i] == '\n')
									line++;
								break;
							}
							if (!inside)
								break;
						}
						break;
					}
					case '\\':
						i++;
						if (iniContent[i] == '\r')
							i++;
						if (iniContent[i] == '\n')
							line++;
						break;
					case '\n':
						foundEnd = true;
						break;
					}
					if (foundEnd)
						break;
				}
				size_t valueEnd = i - 1;
				for (; nameBegin < nameEnd; nameBegin++)
					if (!std::isspace(iniContent[nameBegin]))
						break;
				for (; nameEnd > nameBegin; nameEnd--)
					if (!std::isspace(iniContent[nameEnd]))
						break;
				for (; valueBegin < valueEnd; valueBegin++)
					if (!std::isspace(iniContent[valueBegin]))
						break;
				for (; valueEnd > valueBegin; valueEnd--)
					if (!std::isspace(iniContent[valueEnd]))
						break;

				std::string_view name  = iniContent.substr(nameBegin, nameEnd - nameBegin + 1);
				std::string_view value = iniContent.substr(valueBegin, valueEnd - valueBegin + 1);

				if (name.empty())
					throw IniParseError("Expected name but got nothing", line);

				currentIni->addEntry(printableToValue(name), printableToValue(value));

				i = iniContent.find_first_of('\n', i) + 1;
				line++;
				break;
			}
			}
		}
	}

	bool parseNoexcept(std::string_view iniContent, Ini& outIni) noexcept
	{
		Ini* currentIni = &outIni;
		for (size_t i = 0; i < iniContent.size();)
		{
			switch (iniContent[i])
			{
			case '\r':
				i++;
				if (iniContent[i] != '\n')
					return false;
				i++;
				break;
			case '\n':
			{
				i++;
				break;
			}
			case ';':
			{
				bool foundEnd = false;
				for (; i < iniContent.size(); i++)
				{
					switch (iniContent[i])
					{
					case '\\':
						i++;
						if (iniContent[i] == '\r')
							i++;
						break;
					case '\n':
						foundEnd = true;
						break;
					}
					if (foundEnd)
						break;
				}
				break;
			}
			case '[':
			{
				i++;
				size_t sectionNameBegin = i;
				bool   foundEnd         = false;
				for (; i < iniContent.size(); i++)
				{
					switch (iniContent[i])
					{
					case '"':
					{
						bool inside = true;
						for (; i < iniContent.size(); i++)
						{
							switch (iniContent[i])
							{
							case '"':
								inside = false;
								break;
							case '\\':
								i++;
								if (iniContent[i] == '\r')
									i++;
								break;
							}
							if (!inside)
								break;
						}
						break;
					}
					case '\'':
					{
						bool inside = true;
						for (; i < iniContent.size(); i++)
						{
							switch (iniContent[i])
							{
							case '\'':
								inside = false;
								break;
							case '\\':
								i++;
								if (iniContent[i] == '\r')
									i++;
								break;
							}
							if (!inside)
								break;
						}
						break;
					}
					case '\\':
						i++;
						if (iniContent[i] == '\r')
							i++;
						break;
					case '\n':
						return false;
					case ']':
						foundEnd = true;
						break;
					}
					if (foundEnd)
						break;
				}
				size_t sectionNameEnd = i - 1;

				for (; sectionNameBegin < sectionNameEnd; sectionNameBegin++)
					if (!std::isspace(iniContent[sectionNameBegin]))
						break;
				for (; sectionNameEnd > sectionNameBegin; sectionNameEnd--)
					if (!std::isspace(iniContent[sectionNameEnd]))
						break;
				std::string_view sectionNameV = iniContent.substr(sectionNameBegin, sectionNameEnd - sectionNameBegin + 1);
				if (sectionNameV.empty())
					return false;

				std::string sectionName = printableToValue(sectionNameV);

				if (sectionName[0] == '.')
				{
					if (sectionName[1] == '.')
					{
						Ini* parent = currentIni->getParent();
						if (parent)
							currentIni = parent;
					}
					else
					{
						std::string fullSectionName = currentIni->getFullName();
						if (fullSectionName.empty())
							fullSectionName = sectionName.substr(1);
						else
							fullSectionName += sectionName;
						currentIni = &outIni.addSection(fullSectionName)->getIni();
					}
				}
				else
				{
					currentIni = &outIni.addSection(sectionName)->getIni();
				}

				i = iniContent.find_first_of('\n', i) + 1;
				break;
			}
			default:
			{
				size_t nameBegin = i;
				bool   foundEnd  = false;
				for (; i < iniContent.size(); i++)
				{
					switch (iniContent[i])
					{
					case '"':
					{
						bool inside = true;
						for (; i < iniContent.size(); i++)
						{
							switch (iniContent[i])
							{
							case '"':
								inside = false;
								break;
							case '\\':
								i++;
								if (iniContent[i] == '\r')
									i++;
								break;
							}
							if (!inside)
								break;
						}
						break;
					}
					case '\'':
					{
						bool inside = true;
						for (; i < iniContent.size(); i++)
						{
							switch (iniContent[i])
							{
							case '\'':
								inside = false;
								break;
							case '\\':
								i++;
								if (iniContent[i] == '\r')
									i++;
								break;
							}
							if (!inside)
								break;
						}
						break;
					}
					case '\\':
						i++;
						if (iniContent[i] == '\r')
							i++;
						break;
					case '\n':
						return false;
					case '=':
						foundEnd = true;
						break;
					}
					if (foundEnd)
						break;
				}
				size_t nameEnd    = i - 1;
				size_t valueBegin = i + 1;
				foundEnd          = false;
				for (; i < iniContent.size(); i++)
				{
					switch (iniContent[i])
					{
					case '"':
					{
						bool inside = true;
						for (; i < iniContent.size(); i++)
						{
							switch (iniContent[i])
							{
							case '"':
								inside = false;
								break;
							case '\\':
								i++;
								if (iniContent[i] == '\r')
									i++;
								break;
							}
							if (!inside)
								break;
						}
						break;
					}
					case '\'':
					{
						bool inside = true;
						for (; i < iniContent.size(); i++)
						{
							switch (iniContent[i])
							{
							case '\'':
								inside = false;
								break;
							case '\\':
								i++;
								if (iniContent[i] == '\r')
									i++;
								break;
							}
							if (!inside)
								break;
						}
						break;
					}
					case '\\':
						i++;
						if (iniContent[i] == '\r')
							i++;
						break;
					case '\n':
						foundEnd = true;
						break;
					}
					if (foundEnd)
						break;
				}
				size_t valueEnd = i - 1;
				for (; nameBegin < nameEnd; nameBegin++)
					if (!std::isspace(iniContent[nameBegin]))
						break;
				for (; nameEnd > nameBegin; nameEnd--)
					if (!std::isspace(iniContent[nameEnd]))
						break;
				for (; valueBegin < valueEnd; valueBegin++)
					if (!std::isspace(iniContent[valueBegin]))
						break;
				for (; valueEnd > valueBegin; valueEnd--)
					if (!std::isspace(iniContent[valueEnd]))
						break;

				std::string_view name  = iniContent.substr(nameBegin, nameEnd - nameBegin + 1);
				std::string_view value = iniContent.substr(valueBegin, valueEnd - valueBegin + 1);

				if (name.empty())
					return false;

				currentIni->addEntry(printableToValue(name), printableToValue(value));

				i = iniContent.find_first_of('\n', i) + 1;
				break;
			}
			}
		}
		return true;
	}

	void write(std::string& outIniContent, const Ini& ini)
	{
		for (auto itr = ini.begin(), end = ini.end(); itr != end; itr++)
		{
			outIniContent += valueToPrintable((*itr)->getName());
			outIniContent += " = ";
			outIniContent += valueToPrintable((*itr)->getValue());
			outIniContent += "\n";
		}

		if (ini.numEntries() > 0)
			outIniContent += "\n";

		for (auto itr = ini.beginSections(), end = ini.endSections(); itr != end; itr++)
		{
			outIniContent += "[";
			outIniContent += valueToPrintable((*itr)->getFullName());
			outIniContent += "]\n";
			write(outIniContent, (*itr)->getIni());
		}
	}

	void readFile(const std::filesystem::path& file, Ini& outIni)
	{
		std::ifstream stream(file, std::ios::ate);
		if (stream)
		{
			std::string iniContent(stream.tellg(), '\0');
			stream.seekg(0);
			stream.read(iniContent.data(), iniContent.size());
			stream.close();
			iniContent.shrink_to_fit();
			parse(iniContent, outIni);
		}
	}

	bool readFileNoexcept(const std::filesystem::path& file, Ini& outIni) noexcept
	{
		std::ifstream stream(file, std::ios::ate);
		if (stream)
		{
			std::string iniContent(stream.tellg(), '\0');
			stream.seekg(0);
			stream.read(iniContent.data(), iniContent.size());
			stream.close();
			iniContent.shrink_to_fit();
			return parseNoexcept(iniContent, outIni);
		}
		return false;
	}

	void saveFile(const std::filesystem::path& file, const Ini& ini)
	{
		std::ofstream stream(file);
		if (stream)
		{
			std::string iniContent;
			write(iniContent, ini);
			stream.write(iniContent.data(), iniContent.size());
			stream.close();
		}
	}
} // namespace Serializer