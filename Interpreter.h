#pragma once
#include "stdafx.h"

//CSET �ڵ带 C++ �ڵ�� ��ȯ�մϴ�.
//+ �ϳ��� cpp ���Ϸ� ��Ĩ�ϴ�.(�ٲ� �� ����)

enum class InterpretOption
{
	//...
};

bool is_whitespace(wchar_t c)
{
	if (c == ' ' || c == '\n' || c == '\t')
		return true;
	else
		return false;
}

class Interpreter
{
public:
	void add_file(wstring&& filename); //��ȯ�� ���� �߰�
	void interpret(); //��ȯ ����.
	void finish(); //�� ������ �̾Ƴ���

private: //��������Ʈ ���� �۾���
	bool read_line(); //���� ���Ͽ��� ���� �о� (bool)fin ����
	void line_to_words(); 
	bool read_line_if_words_empty();
	//void chars_to_wstring(); //deprecated
	//void remove_comment(); //deprecated

	void interpret_global();
	void interpret_local();
	void interpret_function();
	void interpret_function_main(const wstring& func_signature);
	void interpret_class();
	void interpret_parameter();
	void interpret_variable(); //const, mut, literal
	void interpret_variable_static();
	void interpret_class();
	
	void interpret_array();

private:
	string temp_name = "__temp__name";
	array<string, 30> unuse_cpp_keywords
	{""};

	vector<wstring> filenames; //����
	wifstream fin;

	wstring line;
	int line_num=0;
	queue<wstring> words;

	int unclosed_bracket=0;

private: //result
	wstring prototypes; //���� ��Ƴ��� ���漱���
	wstring bodys; //


public: //���� ������� ����
	const vector<InterpretOption>& get_options() const;
	void add_option(InterpretOption&& option); //��������Ʈ �ɼ� �߰�
private:
	vector<InterpretOption> options;


public: //�������� ����
	Interpreter() = default;
	~Interpreter() = default;
	Interpreter(const Interpreter&) = delete;
	Interpreter(Interpreter&&) = delete;
	Interpreter& operator=(const Interpreter&) = delete;
	Interpreter& operator=(Interpreter&&) = delete;
};