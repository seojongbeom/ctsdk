#include "json/json.h"
#include <fstream>
#include <algorithm>
#include <array>
#include <iostream>
#include <stdio.h>

/** \brief Parse from stream, collect comments and capture error info.
 * Example Usage:
 */

const char* kName = "name";
const char* kType = "type";
const char* kDescription = "description";
const char* kVersion = "version";
const char* kResources = "resources";
const char* kPath = "path";

const size_t kBUF_SIZE = 4096;

const char* kHeaderData = ""
	"#pragma once\n\n"
	"namespace uix {\n\n"
	"struct Resource {\n"
	"const char* name;\n"
	"const char* path;\n"
	"const unsigned char* data;\n"
	"const size_t size;\n"
	"};\n\n"
	"typedef struct Resource Resource;\n"
	"extern Resource gResources[];\n"
	"extern const unsigned int gMaxResourcesNumber;\n\n"
	"} // namespace uix\n\n\n\n";

bool CheckFormat(const Json::Value& root) {
	if (root.isMember(kName) == false) {
		std::cout << "Warning : name is [not found | is null ]." << std::endl;
	}

	if (root.isMember(kVersion) == false|| root[kVersion].asString() != "0.0.1") {
		std::cout << "Error : type is [not found | is null | not supported version ]." << std::endl;
	}

	if (root.isMember(kType) == false || root[kType].asString() != "uix_resource") {
		std::cout << "Error : type is [not found | is null | not supported type ]." << std::endl;
		return false;
	}

//	if (root.isMember(kDescription) == false) {
//
//	}

	if (root.isMember(kResources) == false || root[kResources].isNull() || root[kResources].isArray() == false || root[kResources].size() == 0) {
		std::cout << "Error : resources is [not found | is null | not array ]." << std::endl;
		return false;
	}

	return true;
}


std::string toHex(const char data) {
	char* hex[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"};
	auto value = std::string("0x");

//	printf("org : 0x%02x\n", data);

	value.append(hex[(data&0xF0) >> 4]);
	value.append(hex[data&0x0F]);

//	printf("cvt : %s\n", value.c_str());

	return value;
}

std::string toEscapeName(const std::string& path) {
	auto result = std::string(path);
	std::replace_if(result.begin(), result.end(),
		[](char c)->bool{
			return c == ':' || c=='/' || c=='\\' || c=='-' || c ==' ' || c == ',' || c == '.';
		}, '_');
	return result;
}

int main(int argc, char* argv[]) {
	Json::Value root;
	std::ifstream ifs;
	ifs.open(argv[1]);

	Json::CharReaderBuilder builder;
	builder["collectComments"] = true;
	JSONCPP_STRING errs;
	if (!parseFromStream(builder, ifs, &root, &errs)) {
		std::cout << errs << std::endl;
		return EXIT_FAILURE;
	}

	if (CheckFormat(root)) {
		auto resources = root[kResources];
		auto oheader = std::ofstream("resource.h", std::ios::out | std::ios::binary);
		auto osource = std::ofstream("resource.cpp", std::ios::out | std::ios::binary);

		if (osource.is_open() && oheader.is_open()) {
			auto header_data = std::string(kHeaderData);
			oheader.write(header_data.c_str(), header_data.size());
			oheader.flush();
			oheader.close();

			struct RSData {
				std::string data;
				size_t size;
			};

			std::map<std::string, std::string> nameMapper;
			std::map<std::string, RSData*> dataMapper;

			auto source_data = std::string();
			auto rs_size = resources.size();

			source_data.append("namespace uix { \n\n");

			for (auto idx = 0; idx < rs_size; ++idx)
			{
				auto path = resources[idx][kPath].asString();
				auto escapeName = toEscapeName(path);
				source_data.append("extern const unsigned char " + escapeName + "[];\n");
				nameMapper[path] = escapeName;
			}
			source_data.append("\n");

			for (auto idx = 0; idx < rs_size; ++idx) {
				if (resources[idx].isMember(kPath) == false || resources[idx][kPath].isNull()){
					// TODO Error message.
					return EXIT_FAILURE;
				}

				FILE* resource = nullptr;
				auto error_code = fopen_s(&resource, resources[idx][kPath].asString().c_str(), "rb");
				if (error_code != 0) {
					// TODO Error message.
					return EXIT_FAILURE;
				}

				auto total_size = size_t(0);
				auto bytes = std::string();
				char buf[kBUF_SIZE];
				memset(static_cast<void*>(buf), 0, kBUF_SIZE);
				while(auto read_count = fread_s(static_cast<void*>(buf), kBUF_SIZE, 1, kBUF_SIZE, resource)) {
					total_size += read_count;
					for (auto ridx = size_t(0); ridx < read_count; ++ridx)
					{
						if (bytes.empty() == false) {
							bytes.append(", ");
						}
						bytes.append(toHex(buf[ridx]));
					}
				}
				fclose(resource);


				dataMapper[resources[idx][kPath].asString()] = new RSData {bytes, total_size };

				if (idx == 0) {
					source_data.append("Resource gResources[] = {{\n\"");
				} else {
					source_data.append("},{\n\"");
				}
				source_data.append(resources[idx][kName].isNull() ? "" : resources[idx][kName].asString());
				source_data.append("\",\n\"");
				source_data.append(resources[idx][kPath].isNull() ? "" : resources[idx][kPath].asString());
				source_data.append("\",\n");
				source_data.append(nameMapper[resources[idx][kPath].asString()]);
				source_data.append(",\n");
				source_data.append(std::to_string(total_size));
			}
			source_data.append("\n}\n};\n\n");
			source_data.append("const unsigned int gMaxResourcesNumber=");
			source_data.append(std::to_string(rs_size));
			source_data.append(";\n\n");

			for (auto idx = 0; idx < rs_size; ++idx)
			{
				auto path = resources[idx][kPath].asString();
				source_data.append("const unsigned char " + nameMapper[path] + "[]={");
				source_data.append(dataMapper[path]->data);
				source_data.append("};\n");
			}

			source_data.append("\n\n");


			source_data.append("} // namespace uix \n");

			osource << "#include \"resource.h\"" << std::endl;
			osource << source_data << std::endl;
			osource.flush();
			osource.close();

			for (auto item : dataMapper) {
				item.second->data.clear();
				delete item.second;
				item.second = nullptr;
			}
		} else {
			// TODO Error message.
		}
	} else {
		std::cout << root << std::endl;
	}




	for (auto item = root.begin(); item != root.end(); ++item) {
		if (item->isArray()) {
			for (auto idx = 0; idx < item->size(); ++idx) {

			}
		} else {
			std::cout << item.key();
			std::cout << " : " <<  item->asString() << std::endl;
		}
	}

	return EXIT_SUCCESS;
}
