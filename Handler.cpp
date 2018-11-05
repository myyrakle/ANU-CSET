#include "pch.h"
#include "Handler.h"

void Handler::run()
{
	this->interpreter.set_caller(this);

	interpreter.do_import(L"basic");

	//���� �о ����.
	for (auto& filename : filenames)
	{
		this->current_filename = filename;

		this->current_file_inputstream.open(filename);
		this->current_file_inputstream.imbue(std::locale(""));

		if (!current_file_inputstream)
			this->print_error(L"�ҽ������� �ȿ����׿�. ���̷���");
		else
		{
			this->line_number = 0;

			while (this->current_file_inputstream)
			{
				//��ūȭ�մϴ�.
				this->tokens = this->read_line_then_tokenize();

				//��ȯ�۾��� �����մϴ�.
				this->interpreter.interpret(tokens);

				this->builder.add_file(this->_original_filepath);
			}
		}
		this->current_file_inputstream.close();

		this->builder.set_original_filepath(_original_filepath);
		this->builder.add_file(_original_filepath + L"\\temp\\temp.cpp");
		this->builder.build();
	}
	//compiler.add_file(original_filepath + L"\\temp\\temp.cpp");
	//compiler.compile();
}

bool Handler::readable() const
{
	return bool(this->current_file_inputstream);
}

std::queue<std::wstring>&& Handler::read_line_then_tokenize()
{
	//���� �о�ɴϴ�.
	auto line = this->getline();
	//��ūȭ�մϴ�.
	this->tokens = std::move(this->tokenizer.tokenize(line));

	return std::move(tokens);
}

void Handler::add_file(const std::wstring& filename)
{
	filenames.emplace_back(filename);
}

std::wstring Handler::getline()
{
	std::wstring temp;
	if (this->current_file_inputstream)
	{
		std::getline(this->current_file_inputstream, temp);
		this->line_number++;
	}
	return temp;
}

void Handler::print_error(std::wstring_view str) const
{
	wprintf(L"���ϸ� : %s\n",current_filename);
	wprintf(L"%s\n", str);
	exit(EXIT_SUCCESS);
}

Handler::Handler(const std::wstring original_) : _original_filepath(original_)
{}
