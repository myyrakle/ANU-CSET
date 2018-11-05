#pragma once

#include "Tokenizer.h"
#include "Interpreter.h"
#include "Builder.h"

//��� �ൿ�� �����մϴ�.

class Handler
{
public: 
	void run();
	bool readable() const;
	std::queue<std::wstring>&& read_line_then_tokenize();
public:
	void add_file(const std::wstring&);
private:
	std::wstring getline();
private:
	Tokenizer tokenizer;
	Interpreter interpreter;
	Builder builder;
private:
	std::wstring headers; //���� ��Ƴ��� ���漱��� or �⺻ ����
	std::wstring prototypes; //Ŭ���� �����ε�
	std::wstring bodys; //��ü��
private:
	std::queue<std::wstring> tokens; //tokenize �����
	int line_number = 0;
private:
	std::vector<std::wstring> filenames; //���� �̸���
	std::wifstream current_file_inputstream; //���� �����ִ� ���� �Է� ��Ʈ��
	const std::wstring _original_filepath; //�����Ϸ� ���� ���͸� ��ġ ����.
private: //���� �α׿�
	std::wstring_view current_filename; //���� �����ִ� ���ϸ�
	void print_error(std::wstring_view str) const;

public: 
	Handler(const std::wstring original_);
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

