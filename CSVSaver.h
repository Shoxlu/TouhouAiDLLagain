#pragma once
#include <cstdio>
#include <vector>
#include <string>

using namespace std;
struct CSVFile {
	FILE* pFile = nullptr;
	size_t fileSize = 0;
	string rawContent;
	std::vector<string> keys;
	std::vector<size_t> lines_begin;
	size_t n_columns = 0;
	size_t n_lines = 0;
	string name;
	string last_key;
	size_t last_column;
};


class CSVSaver {
public:
	CSVSaver();
	CSVFile* create(FILE* file, string name);
	CSVFile* create(string name);
	void write(string toWrite, FILE* file);
	void write(string toWrite, string key, size_t index);
	size_t getColumn(CSVFile* file, string key);
	CSVFile* read(FILE* file);
	CSVFile* read(string name);
	string read(size_t indexbegin, size_t indexEnd, CSVFile* file = nullptr);
	string read(size_t indexbegin, size_t indexEnd, string key, CSVFile* file = nullptr);
	string read(string key, size_t index, CSVFile* file = nullptr);
	//string read(string key, CSVFile* file = nullptr);
	void reset(FILE* file);
	string getContent(FILE* pFile);
	std::vector<string> getKeys(CSVFile* file);
	std::vector<size_t> getLines(CSVFile* file);
	size_t getSize(FILE* file);
	string getSlice(string buffer, size_t begin, size_t end);
	void AddKey(string key);
	void Apply(CSVFile* file);
	void Apply();
	void Close(CSVFile* file = nullptr);
	size_t goToColumn(size_t pos, size_t column);

	CSVFile* currentfile;
private:

};