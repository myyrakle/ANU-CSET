#pragma once
#pragma once
#include "pch.h"

//CSET �ڵ带 C++ �ڵ�� ��ȯ�մϴ�.
//+ �ϳ��� cpp ���Ϸ� ��Ĩ�ϴ�.(�ٲ� �� ����)

enum class InterpretOption
{
	//...
};

class Interpreter
{
public:
	void add_file(wstring&& filename); //��ȯ�� ���� �߰�
	void interpret(); //��ȯ ����.
	void push_default_header();
	void finish(); //�� ������ �̾Ƴ���

private: //��������Ʈ ���� �۾���
	void read_line(); //���� ���Ͽ��� ���� �о��
	void read_line_if_empty();
	void line_to_words();
	void convert_typename(std::wstring&);
	void convert_unused_keywords(std::wstring&);
	void do_import(const std::wstring&);
private:
	void print_error(wstring_view) const;
private:
	void interpret_global();
	void interpret_local();
	void interpret_function();
	void interpret_class();
	void interpret_variable(); //const, mut, literal
private:
	void interpret_array();
	void interpret_vector();

public:
	void set_original_filepath(const wstring&);
private:
	wstring _original_filepath;

private:
	wstring TEMP_name = L"__temp__name";

private:
	vector<wstring> filenames; //����
	wstring_view current_filename;
	wifstream current_file_input;

private:
	wstring line;
	int line_num = 0;
	queue<wstring> words;

private:
	int unclosed_bracket = 0;

private: //main �Լ� ó����
	bool in_main_func = false;
	wstring cmdline_arg_name;

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