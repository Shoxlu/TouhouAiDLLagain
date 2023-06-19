#include "CSVSaver.h"
#include <fstream>
#include <iostream>

#define checkFile(file) if (file == nullptr) { printf("File doesn't exist or is nullptr"); return nullptr;}
#define checkCSVFile(file) if (file == nullptr) { file = currentfile;}

CSVSaver::CSVSaver() : currentfile()
{
}

CSVFile* CSVSaver::create(FILE* file, string name)
{
	fopen_s(&file, name.c_str(), "w+");
	currentfile = new CSVFile();
	currentfile->name = name;
	return read(file);
}


CSVFile* CSVSaver::create(string name)
{
	FILE* file;
	if (fopen_s(&file, name.c_str(), "w+") != 0)
		printf("Failed to create file");
	currentfile = new CSVFile();
	currentfile->name = name;
	return read(file);
}

void CSVSaver::write(string toWrite, FILE* file)
{
	fputs(toWrite.c_str(), file);
}

void CSVSaver::write(string toWrite, string key, size_t index)
{
	size_t column = 0;
	if (0 == currentfile->keys.size()) {
		currentfile->rawContent.insert(0, key+",\n");
		currentfile->n_columns++;
		getLines(currentfile);
		currentfile->keys.emplace_back(key);
	}
	column = getColumn(currentfile, key);
	if (column == currentfile->n_columns) {
		AddKey(key);
		getLines(currentfile);
	}
	if (index >= currentfile->n_lines) {
		getLines(currentfile);
		for (size_t i = 0; i < index - currentfile->n_lines+1; i++) {
			currentfile->rawContent += "\n";
			for (size_t j = 0; j < currentfile->n_columns; j++) {
				currentfile->rawContent += "NULL,";
			}
		}
		getLines(currentfile);
	}
	size_t pos = currentfile->lines_begin[index];
	pos = goToColumn(pos, column);
	size_t finalPos = currentfile->rawContent.find(',', pos) + 1;
	string search =currentfile->rawContent.substr(pos, finalPos-pos);
	currentfile->rawContent.replace(pos, search.length(), "");
	currentfile->rawContent.insert(pos, toWrite+',');
	getLines(currentfile);
}


int CSVSaver::getColumn(CSVFile* file, string key) {
	size_t column = 0;
	for (size_t i = 0; i < file->keys.size(); i++) {
		if (file->keys[i] == key) {
			column = i;
			break;
		}
		else if (i == file->keys.size() - 1) {
			column = file->n_columns;
		}
	}
	return column;
}




void CSVSaver::Apply(CSVFile* file) {
	fputs(file->rawContent.c_str(),file->pFile);
}
void CSVSaver::Apply() {
	Apply(currentfile);
}

void CSVSaver::Close(CSVFile* file) {
	if (file == nullptr) {
		file = currentfile;
	}
	Apply(file);
	fclose(file->pFile);
	delete file;
}


int CSVSaver::goToColumn(int pos, int column)
{
	for (int i = 0; i < column; i++) {
		pos = currentfile->rawContent.find(',', pos)  + 1;
	}
	return pos;
}
void CSVSaver::AddKey(string key) {
	currentfile->rawContent.insert(currentfile->rawContent.find('\n'),key+ ',');
	currentfile->n_columns++;
	currentfile->keys.emplace_back(key);
	getLines(currentfile);//3ms
	for (size_t i = 0; i < currentfile->n_lines; i++) {
		if (read(key, i) == "") {//2ms
			size_t pos = currentfile->lines_begin[i];
			pos = goToColumn(pos, currentfile->n_columns-1);//2ms
			currentfile->rawContent.insert(pos, "NULL,");
			getLines(currentfile);//3ms
		}
	}
	getLines(currentfile);
	
}

std::vector<size_t> CSVSaver::getLines(CSVFile* file)
{
	if (file->pFile == nullptr) {
		return std::vector<size_t>();
	}
	if (file->rawContent == "")
	{
		file->rawContent = getContent(file->pFile);
	}
	int pos = 0;
	file->n_lines = 0;
	file->lines_begin.clear();
	while ((pos = file->rawContent.find('\n', pos+1)) != string::npos) {
		file->lines_begin.emplace_back(pos + 1);
		file->n_lines++;
	}
	file->fileSize = file->rawContent.length();
	return file->lines_begin;
}
size_t CSVSaver::getSize(FILE* file) {
	size_t n = 0;
	while (fgetc(file) != EOF) {
		++n;
	}
	return n;
}

string CSVSaver::getSlice(string buffer, int begin, int end)
{
	string newBuffer;
	for (int i = 0; i < end - begin; i++) {
		newBuffer += buffer[begin+i];
	}
	return newBuffer;
}


CSVFile* CSVSaver::read(string name)
{
	FILE* file;
	errno_t err = fopen_s(&file, name.c_str(), "r+");
	printf("%d", err);
	if ( err == 0) {
		currentfile = new CSVFile();
		return read(file);
	}
	return nullptr;
}
string CSVSaver::read(size_t indexbegin, size_t indexEnd, CSVFile* file)
{
	checkCSVFile(file);
	return string();
}
string CSVSaver::read(size_t indexbegin, size_t indexEnd, string key, CSVFile* file)
{
	checkCSVFile(file);
	return string();
}
string CSVSaver::read(string key, size_t index, CSVFile* file)
{
	checkCSVFile(file);
	size_t column = 0;
	if (index >= currentfile->n_lines) {
		//printf("Line doesn't exist.");
		return "";
	}
	column = getColumn(currentfile, key);
	if (column == currentfile->n_columns) {
		return "";
	}
	size_t pos = goToColumn(currentfile->lines_begin[index], column);
	if (currentfile->rawContent[pos] == '\n') {
		return "";
	}
	size_t finalpos = currentfile->rawContent.find(',', pos);
	string search = currentfile->rawContent.substr(pos, finalpos - pos);
	return search;
}
//string CSVSaver::read(string key, CSVFile* file)
//{
//	checkCSVFile(file);
//	return string();
//}
CSVFile* CSVSaver::read(FILE* file)
{
	checkFile(file);
	if (!currentfile)
		currentfile = new CSVFile();
	currentfile->pFile = file;
	currentfile->rawContent = getContent(file);
	currentfile->fileSize = currentfile->rawContent.length();
	getKeys(currentfile);
	getLines(currentfile);
	return currentfile;
}

void CSVSaver::reset(FILE* file)
{
	write("", file);
}


string CSVSaver::getContent(FILE* pFile) {
	char buffer[2000];
	fseek(pFile, 0, 0);
	string content;
	while (fgets(buffer, 2000, pFile) != NULL) {
		content += buffer;
	}
	return content;
}

std::vector<string> CSVSaver::getKeys(CSVFile* file) {
	if (file->pFile == nullptr) {
		return std::vector<string>();
	}
	if (file->rawContent == "")
	{
		file->rawContent = getContent(file->pFile);
	}
	file->n_columns = 0;
	file->keys.clear();

	int index = 0;
	int previndex = 0;
	int pos = file->rawContent.find('\n');
	while (index < pos)
	{
		index = file->rawContent.find(',', previndex) + 1;
		file->n_columns++;
		file->keys.emplace_back(getSlice(file->rawContent, previndex, index - 1));
		previndex = index;
	}
	return file->keys;
}