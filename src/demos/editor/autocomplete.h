//
// Autocomplete.h
//

#ifndef AUTOCOMPLETE_H_
#define AUTOCOMPLETE_H_

#include <string>
#include <vector>

//!
class Functions
{
public:
	Functions& operator+=(const std::string& name) { m_names.push_back(name); return *this;}

	const std::vector<std::string>& AllFunctions() const { return m_names; }
private:
	std::vector<std::string> m_names;
};

//! Manage a list of auto complete options
class Autocomplete
{
public:
	enum Type
	{
		Function,
		Variable
	};

	struct Option
	{
		Option(Type type, const std::string& name)
		: type(type),
		  name(name)
		{}

		Type type;
		std::string name;
	};

	Autocomplete();
	virtual ~Autocomplete();

	void AddItem(const Option& option);
	void RemoveItem(const std::string& name);
	void UpdateFilter(const std::string& filter);

	const std::vector<Option>& FilteredOptions() const;
	const std::vector<Option>& AllOptions() const;

private:
	std::string m_filter;
	std::vector<Option> m_filteredOptions;
	std::vector<Option> m_allOptions;
};

inline bool operator<(const Autocomplete::Option& lhs, const Autocomplete::Option& rhs)
{
	return lhs.name < rhs.name;
}

#endif /* AUTOCOMPLETE_H_ */
