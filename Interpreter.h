#pragma once
#pragma once
#include "pch.h"

//CSET �ڵ带 C++ �ڵ�� ��ȯ�մϴ�.
//+ �ϳ��� cpp ���Ϸ� ��Ĩ�ϴ�.(�ٲ� �� ����)

enum class State
{
	In_Global,
	In_Func,
	In_Main_Func,
	In_Local,
};

class Handler;

class Interpreter
{
private:
	Handler* caller;
public:
	void set_caller(Handler* ptr);

public:
	void interpret(const std::queue<std::wstring>& tokens); //��ȯ ����.
	void push_default_header();
	void finish(); //�� ������ �̾Ƴ���

private: //��������Ʈ ���� �۾���
	void read_line(); //���� ���Ͽ��� ���� �о��
	void read_line_if_empty();
	void line_to_words(); //tokenize
	void convert_typename(std::wstring&);
	void convert_unused_keywords(std::wstring&);
	void do_import(const std::wstring&);
private:
	void print_error(wstring_view);
private:
	void interpret_global();
	void interpret_local();
	void interpret_function();
	void interpret_class();
	void interpret_variable(); //const, mut, literal
private:
	void interpret_array();
	void interpret_vector();

private:
	wstring TEMP_name = L"__temp__name";
private:
	int line_num = 0;

private:
	int unclosed_bracket = 0;

private: //main �Լ� ó����
	bool in_main_func = false;
	wstring cmdline_arg_name;


public: //�������� ����
	Interpreter() = default;
	~Interpreter() = default;
	Interpreter(const Interpreter&) = delete;
	Interpreter(Interpreter&&) = delete;
	Interpreter& operator=(const Interpreter&) = delete;
	Interpreter& operator=(Interpreter&&) = delete;
};