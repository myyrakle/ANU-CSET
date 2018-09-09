#pragma once
#include "stdafx.h"

//CSET �ڵ带 C++ �ڵ�� ��ȯ�մϴ�.
//+ �ϳ��� cpp ���Ϸ� ��Ĩ�ϴ�.(�ٲ� �� ����)

class Interpreter
{
public:
	void add_file(wstring&& filename); //��ȯ�� ���� �߰�
	void add_option(); //��������Ʈ �ɼ� �߰�
	void interpret(); //��ȯ ����.


private:
	vector<wstring> filenames;
	//vector<?> options;
	vector<wstring> sources;


public: //�������� ����
	Interpreter() = default;
	~Interpreter() = default;
	Interpreter(const Interpreter&) = delete;
	Interpreter(Interpreter&&) = delete;
	Interpreter& operator=(const Interpreter&) = delete;
	Interpreter& operator=(Interpreter&&) = delete;
};