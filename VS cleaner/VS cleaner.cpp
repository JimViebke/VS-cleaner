/*
2022 June 11

Automate cleaning up the hundreds of megs of VS temp and build files.
*/

#include <iostream>
#include <filesystem>

#include "utility.hpp"

int main()
{
	namespace fs = std::filesystem;

	fs::path dir = fs::current_path();
	std::cout << "Launched from " << dir << '\n';

	// Move upward in search of a C++ folder
	while (dir.has_parent_path() && dir.filename() != "C++")
	{
		dir = dir.parent_path();
	}

	if (dir.filename() == "C++")
	{
		std::cout << "Cleaning from " << dir << '\n';
	}
	else
	{
		std::cout << "Could not find C++ directory.\n";
		return 0;
	}

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
			std::cout << "Erased ";
		}
		catch (...)
		{
			std::cout << "Couldn't erase ";
		}

		std::cout << path << ", " << util::file_size{ folder_size } << '\n';
	}

	std::cout << "Erased " << util::file_size{ erased_bytes } << " in " << erased << " directories.\n";
	std::cout << "(press any key to continue)\n";
	std::cin.ignore();
}
