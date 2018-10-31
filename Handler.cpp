#include "pch.h"
#include "Handler.h"

void Handler::run()
{
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
				//���� �о�ɴϴ�.
				auto line = this->getline();

				//��ūȭ�մϴ�.
				this->tokens = std::move(this->tokenizer.tokenize(line));

				//��ȯ�۾��� �����մϴ�.
				this->interpreter.interpret(tokens);
			}
		}
		current_file_inputstream.close();

		this->builder.build();
	}
	//compiler.add_file(original_filepath + L"\\temp\\temp.cpp");
	//compiler.compile();
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
