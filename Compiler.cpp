#include "stdafx.h"
#include "Compiler.h"

void Compiler::add_file(wstring && filename)
{
	filenames.emplace_back(std::forward<wstring&&>(filename));
}

void Compiler::compile() const
{
	//g++ ��� Ž��
	wifstream fin(path_filename);
	wstring gpp_path;
	fin>>gpp_path;
	fin.close();

	//Ŀ�ǵ忡 �� ��ɾ� ����
	wstring cmd = gpp_path + L"\\g++.exe";
	for (auto& filename : filenames)
		(cmd += ' ') += filename;
	
	_wsystem(cmd.c_str());
}
