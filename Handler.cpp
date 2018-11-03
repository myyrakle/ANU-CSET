#include "pch.h"
#include "Handler.h"

void Handler::run()
{
	this->interpreter.set_caller(this);

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

				this->builder.add_file(std::move(this->_original_filepath));
			}
		}
		this->current_file_inputstream.close();

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



void Handler::add_file(std::wstring&& filename)
{
	filenames.emplace_back(std::move(filename));
}

void Handler::set_original_filepath(const wstring& path)
{
	this->_original_filepath = path;
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