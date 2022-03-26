#pragma once

#include <filesystem>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace Serializer
{
	struct IniEntry;
	struct IniSection;

	struct IniValueTypeError : public std::runtime_error
	{
	public:
		explicit IniValueTypeError(std::string_view msg);
		explicit IniValueTypeError(const char* msg);
	};

	struct IniParseError : public std::runtime_error
	{
	public:
		explicit IniParseError(std::string_view msg, size_t line);
		explicit IniParseError(const char* msg, size_t line);
	};

	class Ini
	{
	public:
		using EntryVec               = std::vector<IniEntry*>;
		using iterator               = EntryVec::iterator;
		using const_iterator         = EntryVec::const_iterator;
		using reverse_iterator       = EntryVec::reverse_iterator;
		using const_reverse_iterator = EntryVec::const_reverse_iterator;

		using SectionVec                      = std::vector<IniSection*>;
		using sections_iterator               = SectionVec::iterator;
		using sections_const_iterator         = SectionVec::const_iterator;
		using sections_reverse_iterator       = SectionVec::reverse_iterator;
		using sections_const_reverse_iterator = SectionVec::const_reverse_iterator;

	public:
		Ini()           = default;
		Ini(const Ini&) = default;
		Ini(Ini&&)      = default;
		Ini& operator=(const Ini&) = default;
		Ini& operator=(Ini&&) = default;
		Ini(IniSection* section);
		~Ini();

		std::string getFullName() const;
		Ini*        getParent();
		const Ini*  getParent() const;

		IniSection* addSection(std::string_view name);
		IniEntry*   addEntry(std::string_view name, std::string_view value);

		void removeSection(std::string_view name);
		void removeEntry(std::string_view name);

		void        clearSections();
		void        clearEntries();
		inline void clear()
		{
			clearSections();
			clearEntries();
		}

		IniSection*       getSection(std::string_view name);
		const IniSection* getSection(std::string_view name) const;

		IniEntry*       getEntry(std::string_view name);
		const IniEntry* getEntry(std::string_view name) const;

		inline size_t numSections() const { return this->sections.size(); }
		inline size_t numEntries() const { return this->entries.size(); }

		IniEntry*       operator[](std::string_view name);
		const IniEntry* operator[](std::string_view name) const;

		inline iterator               begin() { return this->entries.begin(); }
		inline iterator               end() { return this->entries.end(); }
		inline const_iterator         begin() const { return this->entries.begin(); }
		inline const_iterator         end() const { return this->entries.end(); }
		inline const_iterator         cbegin() const { return this->entries.cbegin(); }
		inline const_iterator         cend() const { return this->entries.cend(); }
		inline reverse_iterator       rbegin() { return this->entries.rbegin(); }
		inline reverse_iterator       rend() { return this->entries.rend(); }
		inline const_reverse_iterator rbegin() const { return this->entries.rbegin(); }
		inline const_reverse_iterator rend() const { return this->entries.rend(); }
		inline const_reverse_iterator crbegin() const { return this->entries.crbegin(); }
		inline const_reverse_iterator crend() const { return this->entries.crend(); }

		inline sections_iterator               beginSections() { return this->sections.begin(); }
		inline sections_iterator               endSections() { return this->sections.end(); }
		inline sections_const_iterator         beginSections() const { return this->sections.begin(); }
		inline sections_const_iterator         endSections() const { return this->sections.end(); }
		inline sections_const_iterator         cbeginSections() const { return this->sections.cbegin(); }
		inline sections_const_iterator         cendSections() const { return this->sections.cend(); }
		inline sections_reverse_iterator       rbeginSections() { return this->sections.rbegin(); }
		inline sections_reverse_iterator       rendSections() { return this->sections.rend(); }
		inline sections_const_reverse_iterator rbeginSections() const { return this->sections.rbegin(); }
		inline sections_const_reverse_iterator rendSections() const { return this->sections.rend(); }
		inline sections_const_reverse_iterator crbeginSections() const { return this->sections.crbegin(); }
		inline sections_const_reverse_iterator crendSections() const { return this->sections.crend(); }

	private:
		SectionVec  sections;
		EntryVec    entries;
		IniSection* section = nullptr;
	};

	struct IniEntry
	{
	public:
		IniEntry()                = default;
		IniEntry(const IniEntry&) = default;
		IniEntry(IniEntry&&)      = default;
		IniEntry& operator=(const IniEntry&) = default;
		IniEntry& operator=(IniEntry&&) = default;
		IniEntry(std::string_view name, Ini* ini);
		IniEntry(std::string_view name, std::string_view value, Ini* ini);

		IniEntry& operator=(std::string_view value);

		inline const std::string& getName() const { return this->name; }
		inline const std::string& getValue() const { return this->value; }
		inline Ini*               getIni() { return this->ini; }
		inline const Ini*         getIni() const { return this->ini; }

		template <class T>
		T getValueOfType();

		template <typename T, typename Alloc>
		std::vector<std::vector<T, Alloc>> getValueOfType()
		{
			Ini              ini;
			size_t           index     = 0;
			size_t           i         = 0;
			std::string_view valueView = std::string_view(value);
			while (i < valueView.size())
			{
				size_t begin    = i;
				bool   foundEnd = false;
				for (; i < valueView.size(); i++)
				{
					switch (valueView[i])
					{
					case '\\':
						i++;
						break;
					case ',':
						foundEnd = true;
						break;
					}

					if (foundEnd)
						break;
				}
				size_t end = i - 1;
				i++;

				for (; begin < end; begin++)
					if (!std::isspace(valueView[begin]))
						break;
				for (; end > begin; end--)
					if (!std::isspace(valueView[end]))
						break;
				std::string_view element = valueView.substr(begin, end - begin + 1);
				if (element.empty())
					throw IniValueTypeError("Element " + std::to_string(index) + ", is empty which is illegal");

				ini.addEntry(std::to_string(index), element);
				index++;
			}

			index = 0;
			std::vector<T, Alloc> vector(ini.numEntries());
			while (ini.numEntries() > 0)
			{
				for (auto& entry : ini)
				{
					if (entry->name == std::to_string(index))
					{
						try
						{
							vector[index] = entry->getValueOfType<T>();
						}
						catch (const std::exception& e)
						{
							throw IniValueTypeError("Element " + std::to_string(index) + ", is not of the requested type");
						}
						ini.removeEntry(entry->name);
						index++;
						break;
					}
				}
			}
			return vector;
		}

		template <class T>
		bool tryGetValueOfType(T& outValue) noexcept;

		template <typename T, typename Alloc>
		bool tryGetValueOfType(std::vector<T, Alloc>& outValue) noexcept
		{
			Ini              ini;
			size_t           index     = 0;
			size_t           i         = 0;
			std::string_view valueView = std::string_view(value);
			while (i < valueView.size())
			{
				size_t begin    = i;
				bool   foundEnd = false;
				for (; i < valueView.size(); i++)
				{
					switch (valueView[i])
					{
					case '\\':
						i++;
						break;
					case ',':
						foundEnd = true;
						break;
					}

					if (foundEnd)
						break;
				}
				size_t end = i - 1;
				i++;

				for (; begin < end; begin++)
					if (!std::isspace(valueView[begin]))
						break;
				for (; end > begin; end--)
					if (!std::isspace(valueView[end]))
						break;
				std::string_view element = valueView.substr(begin, end - begin + 1);
				if (element.empty())
					return false;

				ini.addEntry(std::to_string(index), element);
				index++;
			}

			index = 0;
			outValue.reserve(outValue.capacity() + ini.numEntries());
			while (ini.numEntries() > 0)
			{
				for (auto& entry : ini)
				{
					if (entry->name == std::to_string(index))
					{
						try
						{
							outValue.push_back(entry->getValueOfType<T>());
						}
						catch (const std::exception& e)
						{
							return false;
						}
						ini.removeEntry(entry->name);
						index++;
						break;
					}
				}
			}
			return true;
		}

	private:
		std::string name;
		std::string value;
		Ini*        ini;
	};

	struct IniSection
	{
	public:
		IniSection()                  = default;
		IniSection(const IniSection&) = default;
		IniSection(IniSection&&)      = default;
		IniSection& operator=(const IniSection&) = default;
		IniSection& operator=(IniSection&&) = default;
		IniSection(std::string_view name, Ini* parent);

		IniSection& operator=(const Ini& ini);
		IniSection& operator=(Ini&& ini);

		std::string               getFullName() const;
		inline const std::string& getName() const { return this->name; }
		inline Ini&               getIni() { return this->ini; }
		inline const Ini&         getIni() const { return this->ini; }
		inline Ini*               getParent() { return this->parent; }
		inline const Ini*         getParent() const { return this->parent; }

	private:
		std::string name;
		Ini         ini = Ini(this);
		Ini*        parent;
	};

	void parse(std::string_view iniContent, Ini& outIni);
	bool parseNoexcept(std::string_view iniContent, Ini& outIni) noexcept;
	void write(std::string& outIniContent, const Ini& ini);

	void readFile(const std::filesystem::path& file, Ini& outIni);
	bool readFileNoexcept(const std::filesystem::path& file, Ini& outIni) noexcept;
	void saveFile(const std::filesystem::path& file, const Ini& ini);
} // namespace Serializer::Ini