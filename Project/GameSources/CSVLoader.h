#pragma once
#include "stdafx.h"

// ----------------------------------------------------------------------------------
//	CSVLoader CSV�t�@�C���̓ǂݍ��ݗp(media�t�H���_��CSV�t�H���_����ǂݍ���)
// ----------------------------------------------------------------------------------
class CSVLoader
{
private:

	CSVLoader() {}
	~CSVLoader() {}

public:

	static vector<vector<string>>LoadFile(const string& filePath);
	static vector<vector<int>> ReadDataToInt(const vector<vector<string>>& data);
	static vector<vector<float>> ReadDataToFloat(const vector<vector<string>>& data);
	static vector<vector<double>> ReadDataToDouble(const vector<vector<string>>& data);
	static vector<int> ReadDataToInt(const vector<string>& data);
	static vector<int> ReadDataToInt(const vector<vector<string>>& data, const size_t& index);
	static vector<float> ReadDataToFloat(const vector<string>& data);
	static vector<float> ReadDataToFloat(const vector<vector<string>>& data, const size_t& index);
	static vector<double> ReadDataToDouble(const vector<string>& data);
	static vector<double> ReadDataToDouble(const vector<vector<string>>& data, const size_t& index);
};