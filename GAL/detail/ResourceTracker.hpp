#ifndef GAL_RESOURCE_TRACKER_HPP
#define GAL_RESOURCE_TRACKER_HPP

#include <vector>

namespace gal::detail
{
	/// @brief Internal class used to track resources for deletion when calling obr::terminate().
	template<typename IDType, void(*DeleteFunc)(IDType)>
	class ResourceTracker
	{
	public:
		GAL_INLINE void add(IDType id)
		{
			resources.push_back(id);
		}

		GAL_INLINE void remove(IDType id)
		{
			auto it = std::find(resources.begin(), resources.end(), id);

			if (it != resources.end())
			{
				DeleteFunc(*it);
				resources.erase(it);
			}

			// No error if trying to delete a non-existent item.
		}

		GAL_INLINE void clear()
		{
			for (IDType id : resources)
				DeleteFunc(id);

			resources.clear();
		}

	private:
		std::vector<IDType> resources;
	};
}

#endif
