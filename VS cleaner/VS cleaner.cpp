/*
2022 June 11

Automate cleaning up the hundreds of megs of VS temp and build files.
*/

#include <iostream>
#include <filesystem>

#include "utility.hpp"

void vs_cleaner()
{
	namespace fs = std::filesystem;

	const fs::path dir = fs::current_path();

	// make sure that our location contains "C++" somewhere in the path
	if (!dir.string().contains("C++"))
	{
		std::cout << "Could not find C++ directory.\n";
		return;
	}

	std::cout << "Running from " << dir << '\n';

	size_t erased = 0, erased_bytes = 0;

	for (auto it = fs::recursive_directory_iterator{ dir }; it != fs::end(it); )
	{
		if (!is_directory(*it) || it->path().filename() != ".vs")
		{
			++it;
			continue;
		}

		// cache the current path
		const fs::path path = it->path();

		const size_t folder_size = util::get_folder_size(path);

		// Disable recursion for the next iteration...
		it.disable_recursion_pending();
		// ...then iterate...
		++it;

		// ...and then erase from the cached path.
		try
		{
			fs::remove_all(path);

			erased += 1;
			erased_bytes += folder_size;
		}
		catch (...)
		{
			std::cout << "Failed to erase from " << path << '\n';
			continue;
		}

		std::cout << "Erased " << path << ", " << util::file_size{ folder_size } << '\n';
	}

	std::cout << "Erased " << util::file_size{ erased_bytes } << " in " << erased << (erased == 1 ? " directory" : " directories") << ".\n";
}

int main()
{
	vs_cleaner();

	std::cout << "(press any key to continue)\n";
	std::cin.ignore();
}
