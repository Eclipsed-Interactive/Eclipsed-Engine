#pragma once

#include <string>

#include "cereal/cereal.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/String.hpp"
#include "cereal/types/vector.hpp"

#include <fstream>



struct ProjectData {
	std::string path;
	std::string engineVersion;

	bool isHovered = false;

	template <class Archive>
	void serialize(Archive& ar, std::uint32_t version)
	{
		ar(
			CEREAL_NVP(path),
			CEREAL_NVP(engineVersion)
		);
	}
};

struct Projects {
	std::vector<ProjectData> projects;

	template <class Archive>
	void serialize(Archive& ar, std::uint32_t version)
	{
		ar(
			CEREAL_NVP(projects)
		);
	}

	~Projects()
	{
		std::ofstream file(projectsJsonPath);
		cereal::JSONOutputArchive ar(file);
		ar(projects);
	}

	Projects()
	{
		if (!std::filesystem::exists(projectsJsonPath)) return;

		std::ifstream file(projectsJsonPath);
		cereal::JSONInputArchive ar(file);
		ar(projects);
	}

private:
	static inline const char* projectsJsonPath = "projects.json";
};

CEREAL_CLASS_VERSION(ProjectData, 0)
CEREAL_CLASS_VERSION(Projects, 0)