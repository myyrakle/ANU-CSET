#pragma once

#include "Tokenizer.h"
#include "Interpreter.h"
#include "Builder.h"

//��� �ൿ�� �����մϴ�.

class Handler
{
public: 
	void run();
public:
	void add_file(std::wstring&&);
	void set_original_filepath(const std::wstring&);
private:
	std::wstring getline();
private:
	Tokenizer tokenizer;
	Interpreter interpreter;
	Builder builder;
private:
	std::wstring prototypes; //���� ��Ƴ��� ���漱���
	std::wstring bodys; //��ü��
private:
	std::queue<std::wstring> tokens; //tokenize �����
	int line_number = 0;
private:
	std::vector<std::wstring> filenames; //���� �̸���
	std::wifstream current_file_inputstream; //���� �����ִ� ���� �Է� ��Ʈ��
	std::wstring _original_filepath; //�����Ϸ� ���� ���͸� ��ġ ����.
private: //���� �α׿�
	std::wstring_view current_filename; //���� �����ִ� ���ϸ�
	void print_error(std::wstring_view str) const;

public: 
	Handler() = default;
	virtual ~Handler() = default;
public:
	Handler(const Handler&) = delete;
	Handler(Handler&&) = delete;
	Handler& operator=(const Handler&) = delete;
	Handler& operator=(Handler&&) = delete;

public:
	friend Interpreter;
};

