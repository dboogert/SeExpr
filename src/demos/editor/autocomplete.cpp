//
// Autocomplete.cpp
//

#include "autocomplete.h"

Autocomplete::Autocomplete()
{
}

Autocomplete::~Autocomplete()
{
}

void Autocomplete::AddItem(const Option& option)
{
	RemoveItem(option.name);
	m_allOptions.push_back(option);
}

void Autocomplete::RemoveItem(const std::string& name)
{
	for (std::vector<Option>::iterator it = m_allOptions.begin(); it != m_allOptions.end(); ++it)
	{
		if (it->name == name)
		{
			m_allOptions.erase(it);
			break;
		}
	}
}

void Autocomplete::UpdateFilter(const std::string& filter)
{
	m_filteredOptions.clear();
	for (std::vector<Option>::const_iterator it = m_allOptions.begin(); it != m_allOptions.end(); ++it)
	{
		if (it->name.find(filter, 0) == 0)
			m_filteredOptions.push_back(*it);
	}

	std::sort(m_filteredOptions.begin(), m_filteredOptions.end());
}

const std::vector<Autocomplete::Option>& Autocomplete::FilteredOptions() const
{
	return m_filteredOptions;
}

const std::vector<Autocomplete::Option>& Autocomplete::AllOptions() const
{
	return m_allOptions;
}
