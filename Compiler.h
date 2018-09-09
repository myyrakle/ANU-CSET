#pragma once
#include "stdafx.h"
//��ȯ�� C++�ڵ带 �޾Ƽ� g++�� �������� �����մϴ�.

class Compiler
{

public:
	void add_file(wstring&& filename);
	void compile() const;

private:
	//CompileFlag flag = COMPILE;
	vector<wstring> filenames;

public: //�������� ����
	Compiler() = default;
	~Compiler() = default;
	Compiler(const Compiler&) = delete;
	Compiler(Compiler&&) = delete;
	Compiler& operator=(const Compiler&) = delete;
	Compiler& operator=(Compiler&&) = delete;
};