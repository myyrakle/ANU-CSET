#pragma once
#include "stdafx.h"
//��ȯ�� C++�ڵ带 �޾Ƽ� g++�� �������� �����մϴ�.

enum class CompileOption
{
	//...
};

class Compiler
{

public:
	void add_file(wstring&& filename);
	void add_option(CompileOption&& option);
	const vector<CompileOption> get_options() const;
	void compile() const;

private:
	vector<wstring> filenames;
	//vector<CompileOption> options

public: //�������� ����
	Compiler() = default;
	~Compiler() = default;
	Compiler(const Compiler&) = delete;
	Compiler(Compiler&&) = delete;
	Compiler& operator=(const Compiler&) = delete;
	Compiler& operator=(Compiler&&) = delete;
};