#pragma once

#include <filesystem>

namespace util
{
	namespace fs = std::filesystem;

	size_t get_folder_size(const fs::path& root)
	{
		size_t size = 0;

		for (auto& it : fs::recursive_directory_iterator{ root })
		{
			size += it.file_size();
		}

		return size;
	}

	struct file_size {
		size_t size{};
	private:
		friend std::ostream& operator<<(std::ostream& os, const file_size fs) {
			size_t i = 0;
			double mantissa = double(fs.size);
			for (; mantissa >= 1024.; mantissa /= 1024., ++i) {}
			mantissa = std::ceil(mantissa * 10.) / 10.;
			os << mantissa << "BKMGTPE"[i];
			return i == 0 ? os : os << "B";
		}
	};
}
