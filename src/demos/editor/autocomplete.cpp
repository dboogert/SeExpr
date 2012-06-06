//
// Autocomplete.cpp
//

#include "autocomplete.h"
#include <QString>

#include <iostream>

Autocomplete::Autocomplete()
: m_filterSet(false)
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

bool Autocomplete::UpdateFilter(const std::string& filter)
{
	if (filter == m_filter && m_filterSet)
		return false;

	m_filter = filter;
	m_filterSet = true;

	m_filteredOptions.clear();
	for (std::vector<Option>::const_iterator it = m_allOptions.begin(); it != m_allOptions.end(); ++it)
	{
		if (it->name.find(filter, 0) == 0)
			m_filteredOptions.push_back(*it);
	}

	std::sort(m_filteredOptions.begin(), m_filteredOptions.end());
	return true;
}

const std::vector<Autocomplete::Option>& Autocomplete::FilteredOptions() const
{
	return m_filteredOptions;
}

const std::vector<Autocomplete::Option>& Autocomplete::AllOptions() const
{
	return m_allOptions;
}
