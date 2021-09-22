#include <iostream>
#include <windows.h>
#include <sstream>
#include <ctype.h>
#include "boost/filesystem.hpp"
#include <boost/algorithm/string.hpp>.


class Directory {
public:
	Directory(std::string dirName, std::string location)
	{
		this->dirName = dirName;
		this->location = location;
		this->fullPath = location + "\\" + dirName;
	}
	Directory(std::string dirName, std::string location, std::vector<std::string> prefferedTypes)
	{
		this->dirName = dirName;
		this->location = location;
		this->fullPath = location + "\\" + dirName;
		this->prefferedFileTypes = prefferedTypes;
	}
	bool createDirectory(Directory directory)
	{
		bool isCreated = false;
		if (!directory.directoryExists()) {
			isCreated = boost::filesystem::create_directory(fullPath + "\\" + directory.getDirName());
			std::cout << "New directory " << directory.getDirName() << " was created!" << std::endl;
		}
		return isCreated;
	}
	void movePrefferedFiles(Directory sourceDirectory) {
		for (const auto dirEntry : boost::filesystem::directory_iterator(sourceDirectory.getFullPath()))
		{
			if (!boost::filesystem::is_directory(dirEntry)) {
				for (std::string type : prefferedFileTypes) {
					std::string filePath = dirEntry.path().string();
					std::size_t found = filePath.rfind(".");
					if (found != std::string::npos) {
						std::string extension = filePath.substr(found);
						if (boost::algorithm::to_lower_copy(extension) == boost::algorithm::to_lower_copy(type)) {
							std::string fileName = filePath.substr(filePath.rfind("\\") + 1);
							std::string dest = fullPath + "\\" + fileName;
							if (boost::filesystem::exists(dest))
							{
								bool exit = 0;
								for (int i = 1; !exit; i++)
								{
									if (i == 1)
										dest = dest.substr(0, dest.rfind(".")) + "(" + std::to_string(i) + ")" + extension;
									else
										dest = dest.substr(0, dest.rfind("(")) + "(" + std::to_string(i) + ")" + extension;

									for (const auto dirEntry : boost::filesystem::directory_iterator(fullPath))
									{
										if (!boost::filesystem::exists(dest))
										{
											exit = 1;
										}
									}
								}
							}
							boost::filesystem::copy_file(filePath, dest);
							if (boost::filesystem::exists(dest)) {
								boost::filesystem::remove(filePath);
								std::cout << "Moved " << filePath << " to " << dest << std::endl;
								if (boost::filesystem::exists(filePath)) {
									std::cout << "Failed to remove " << filePath << std::endl;
								}
							}
						}
					}
				}
			}
		}
	}

	std::string getDirName() { return this->dirName; }
	std::string getLocation() { return this->location; }
	std::string getFullPath() { return this->fullPath; }

	bool directoryExists()
	{
		return boost::filesystem::exists(fullPath);
	}
private:
	std::string dirName;
	std::string location;
	std::string fullPath;
	std::vector<std::string> prefferedFileTypes;
};


int main()
{
	// Change x in ShowWindow(window, x) to
	HWND window;
	window = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(window, 0);

	Directory currentDirectory("Downloads", "C:\\Users\\Lauris M");

	std::vector<Directory> preffered_directories{
		Directory("PDFs", currentDirectory.getFullPath(), {".pdf"}),
		Directory("MSWord", currentDirectory.getFullPath(), {".docx", ".doc"}),
		Directory("MSPowerPoint", currentDirectory.getFullPath(), {".pptx", ".ppt"}),
		Directory("MSExcel", currentDirectory.getFullPath(), {".xlsx", ".xls"}),
		Directory("ZIP", currentDirectory.getFullPath(), {".zip", ".7z", ".rar"}),
		Directory("Text", currentDirectory.getFullPath(), {".txt"}),
		Directory("Jar", currentDirectory.getFullPath(), {".jar"}),
		Directory("Installation", currentDirectory.getFullPath(), {".exe", ".msi"}),
		Directory("Pictures", currentDirectory.getFullPath(), {".jpg", ".eps", ".tif", ".tiff",
															   ".bmp", ".jpeg", ".gif", ".png",
															   ".raw", ".cr2", ".nef", ".orf", ".sr2"}),
		Directory("Torrent", currentDirectory.getFullPath(), {".torrent"}),
		Directory("Videos", currentDirectory.getFullPath(), {".mp4", ".mov"}),
		Directory("Music", currentDirectory.getFullPath(), {".mp3", ".3gp", ".3gp", ".aa",
		".aac", ".aax", ".act", ".aiff", ".alac", ".amr", ".ape", ".au", ".awb", ".dss",
		".dvf", ".flac", ".gsm", ".iklax", ".ivs", ".m4a", ".m4b", ".m4p", ".mmf", ".mp3",
		".mpc", ".msv", ".nmf", ".ogg", ".oga", ".mogg", ".opus", ".ra", ".rm", ".raw", ".rf64",
		".sln", ".tta", ".voc", ".vox", ".wav", ".wma", ".wv", ".webm", ".8svx", ".cda"}),
		Directory("Blender", currentDirectory.getFullPath(), {".blend"})
	};

	for (Directory dir : preffered_directories)
	{
		currentDirectory.createDirectory(dir);
	}

	while (1)
	{
		for (Directory dir : preffered_directories)
		{
			dir.movePrefferedFiles(currentDirectory);
		}
		Sleep(1 * 1000);
	}
}