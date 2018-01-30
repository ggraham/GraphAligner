#include <algorithm>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "CommonUtils.h"

int main(int argc, char** argv)
{
	std::string infilename {argv[1]};
	std::string outfilename {argv[2]};

	std::unordered_map<int, std::unordered_map<std::string, std::vector<std::pair<int, int>>>> positions;
	auto alignments = CommonUtils::LoadVGAlignments(infilename);
	std::unordered_set<std::string> alignmentNames;
	for (auto aln : alignments)
	{
		alignmentNames.insert(aln.name());
		int pos = aln.query_position();
		for (size_t i = 0; i < aln.path().mapping_size(); i++)
		{
			auto mapping = aln.path().mapping(i);
			positions[mapping.position().node_id()][aln.name()].emplace_back(pos, pos+mapping.edit(0).to_length());
			pos += mapping.edit(0).to_length();
		}
	}
	std::vector<std::string> readnames { alignmentNames.begin(), alignmentNames.end() };
	std::sort(readnames.begin(), readnames.end());
	std::ofstream out {outfilename};
	out << "node";
	for (auto read : readnames)
	{
		out << "," << read;
	}
	out << std::endl;
	for (auto node : positions)
	{
		out << node.first;
		for (auto read : readnames)
		{
			out << ",";
			if (node.second.count(read) == 1)
			{
				for (size_t i = 0; i < node.second[read].size(); i++)
				{
					if (i > 0) out << ";";
					out << node.second[read][i].first << "-" << node.second[read][i].second;
				}
			}
		}
		out << std::endl;
	}
}
