#include "pch.h"
#include "Interpreter.h"

void Interpreter::add_file(wstring && filename)
{
	filenames.emplace_back(std::forward<wstring&&>(filename));
}

void Interpreter::interpret()
{
	this->push_default_header();

	//���� �о ����.
	for (auto& filename : filenames)
	{
		this->current_filename = filename;

		this->current_file_input.open(filename);

		this->line_num = 0;

		while (this->current_file_input)
		{
			this->read_line();
			this->interpret_global();
		}

		current_file_input.close();
	}

	this->finish();
}

void Interpreter::push_default_header()
{
	//��� �������� ��ü ����
	prototypes += L"#include <iostream>\n";
	prototypes += L"#include <string>\n";
	prototypes += L"using string = std::wstring;\n";
	prototypes += L"#include <vector>\n";
	prototypes += L"#include <array>\n";
	//
}

void Interpreter::finish()
{
	wofstream fout(this->_original_filepath + L"\\temp\\temp.cpp");

	for (auto& source : this->prototypes)
		fout << source;
	for (auto& source : this->bodys)
		fout << source;

	fout.close();
}

void Interpreter::read_line()
{
	if (this->current_file_input)
	{
		std::getline(this->current_file_input, this->line);
		this->line_num++;
		this->line_to_words();
	}
}

//���� �о ȭ��Ʈ�����̽� �������� �ܾ� �и�. 
//�ڸ�Ʈ�� ����. 
//���ڿ� ���ͷ��� wstring���� ó��
void Interpreter::line_to_words()
{
	wstring word;
	wstring text_literal = L"";

	bool in_text = false; // ���ڿ� ���ͷ��� Ž�����ΰ�?
	bool quote_is_open = false;

	bool is_escape = false;

	bool has_one_slash = false;
	bool in_comment = false;

	for (auto& c : this->line)
	{

		//���� �ּ� ����
		if (c == '/' && in_text == false) //���ڿ� ���ͷ� ���� �ƴϸ鼭, /��
		{
			if (has_one_slash == false) //�����ð� �ϳ��� �������� �ϳ� üũ
			{
				has_one_slash = true;
				continue;
			}
			else //�����ð� �ϳ� �־����� ���⼭���ʹ� �ּ�. ���� �ڸ��� break, ����
			{
				words.push(word);
				break;
			}
		}
		else //�����ð� ���ӵ��� ������, üũ ����.
			has_one_slash = false;


		if (c == '\"' && is_escape == false) //" ���ڷ� ���ڿ� ���ͷ��� ���� ��
		{
			if (in_text == false) //�� ��
			{
				text_literal = L"std::wstring(L\""; // ���� String���� ��ü ����
				in_text = true;
				continue;
			}
			else //���� ��
			{
				text_literal += L"\")";
				this->words.push(text_literal);
				in_text = false;
				continue;
			}
		}
		else if (in_text) //���ڿ� ���ͷ� Ž������ ���
		{
			if (is_escape == true) //üũ�Ǿ������� ����
				is_escape = false;
			else if (c == '\\') //�̽������� ���� üũ
				is_escape = true;

			text_literal += c;
			continue;
		}

		//��ȣ ���� ��� �� ��ȣ�� �� �ܾ�� �и�
		else if (c == '(' || c == ')' || c == '{' || c == '}' || c == ':' || c == '=')
		{
			if (!word.empty())
			{
				this->convert_typename(word);
				words.push(word);
				word.clear();
			}

			words.push(wstring(1, c));
			continue;
		}

		else if (c == '\n' || c == '\t' || c == ' ') //ȭ��Ʈ�����̽� ���� �и�
		{
			if (word != L"")
			{
				this->convert_typename(word);
				this->words.push(word);
				word.clear();
			}
			continue;
		}
		else
		{
			word += c;
			continue;
		}
	}
}

//Ÿ�� Ű���带 ���� ���� Ŭ���������� ����
void Interpreter::convert_typename(std::wstring& word) 
{
	auto to_upper = [](wchar_t& c) { if (c >= 'a' && c <= 'z') c -= 32; };

	if (word == keywords::OBJECT ||
		word == keywords::CHAR || keywords::STRING ||
		word == keywords::BOOL || keywords::BYTE ||
		word == keywords::INT ||
		word == keywords::FLOAT || word == keywords::DOUBLE)
		to_upper(word[0]);
	else
		return;
}

//���� ���� ����� �ĺ��ڷ� ���� �ְ� ����
void Interpreter::convert_unused_keywords(std::wstring & word)
{
	if (word == keywords::unuse::AUTO || word == keywords::unuse::DELETE_ ||
		word == keywords::unuse::GOTO || word == keywords::unuse::LONG)
		word.insert(0, this->TEMP_name);
}

void Interpreter::do_import(const std::wstring& module_name)
{
	//To Do : Library ����� .h ��� �о����
	wifstream read_module(this->_original_filepath + L"\\Library\\" + module_name + L".h");
	if (!read_module) this->print_error(L"����� �ȿ����׿�.");
	while (read_module)
		this->prototypes += read_module.get();
	read_module.close();
	//if (fin.good()) printf("\n����\n");
	//else printf("\n�ȿ���\n");
}

void Interpreter::read_line_if_empty()
{
	if (current_file_input)
		if (words.empty())
			this->read_line();
}

void Interpreter::print_error(wstring_view log) const
{
	wprintf(L"���ϸ� %s\n", this->current_filename.data());
	wprintf(L"���� : %d\n", this->line_num);
	wprintf(L"%s", log.data());
	exit(EXIT_SUCCESS);
}

void Interpreter::interpret_global()
{
	auto head_word = words.front();

	//func Ű���� ���� ���
	if (this->words.front() == keywords::FUNC)
	{
		this->interpret_function(); //�ñ׳�ó �Ľ�
		this->interpret_local(); //��ü ����
	}

	//const�� mut, literal Ű������ ���
	else if (head_word == keywords::CONST_ || head_word == keywords::MUT || head_word == keywords::LITERAL)
		this->interpret_variable();

	//class Ű������ ���
	else if (head_word == keywords::CLASS)
		;//this->interpret_class();

	else if (this->words.front() == keywords::NAMESPACE)
	{
		//To do
	}

	else if (this->words.front() == keywords::USING)
	{
		this->words.pop();
		this->bodys += L"using namespace ";
		this->bodys += words.front();
		this->bodys += ';';
		throw new exception("using�� ���� �� ���׿�?");
	}

	//import Ű����
	else if (this->words.front() == keywords::IMPORT)
	{
		cout << "�̰� ���� �ȵǳ�" << endl << endl;
		words.pop();
		if (words.empty())
			this->print_error(L"import�� ����� �����?");
		else
		{
			this->do_import(words.front());
			words.pop();
		}
	}

	else //�ƹ��͵� �ƴ� ���
	{
		this->bodys += this->line;
		bodys += ';\n';
		return;
	}
}

void Interpreter::interpret_local()
{
	if (in_main_func) //main �Լ��� ��� ����� ���� ����Ʈ
	{
		bodys += L"\nsetlocale(LC_ALL, \"\");\n";
		if (!cmdline_arg_name.empty())
		{
			//�� ���·� ��ȯ��
			/*
			auto ___strlen = [](const char* str){ size_t i=0; for(;s[i]!='\0';i++); return i; };
			std::vector<std::wstring> �����_���ڸ�;
			for(int i = 0; i < ����_����; i++)
				�����_���ڸ�.emplace_back(wstring(&����_�迭[0], &����_�迭[___strlen(str)]))
			*/
			bodys += L"auto ___strlen = [](const char* str){ size_t i=0; for(;s[i]!='\0';i++); return i; };";
			bodys += L"std::vector<std::wstring> "; bodys += /*����� ����*/cmdline_arg_name;
			bodys += L";\n";

			bodys += L"for(int i=0;i<"; bodys += TEMP_argc; bodys += L";i++)\n\t";
			bodys += cmdline_arg_name;
			bodys += L".emplace_back(std::wstring(&"; bodys += TEMP_args; bodys += L"[0], &"; bodys += TEMP_args; bodys += L"[___strlen(str)]";
			in_main_func = false;
			cmdline_arg_name.clear();
		}
	}

	while (this->unclosed_bracket != 0) //�߰�ȣ �� ���������� ����
	{
		if (words.empty())
		{
			bodys += L";\n";
			this->read_line_if_empty();
		}

		if (words.front() == L"{")
		{
			this->unclosed_bracket++;
			this->bodys += words.front(); words.pop();
			continue;
		}
		else if (words.front() == L"}")
		{
			this->unclosed_bracket--;
			this->bodys += words.front(); words.pop();
			continue;
		}
		//const�� mut, literal Ű������ ���
		else if (words.front() == keywords::CONST_ || words.front() == keywords::MUT
			|| words.front() == keywords::LITERAL)
		{
			this->interpret_variable();
			bodys += ';';
		}
		//To do
		else
		{
			bodys += words.front(); words.pop();
		}
	}

	return;
}

void Interpreter::interpret_function()
{
	wstring func_signature = L"";

	if (this->words.front() == keywords::STATIC)
		func_signature += keywords::STATIC, this->words.pop();

	if (this->words.front() == keywords::FUNC)
		func_signature += L"auto ", this->words.pop();


	//�Լ��� ����
	if (words.front() == L"main") //main �Լ��� ���� ó��
		this->in_main_func = true;

	func_signature += words.front();
	words.pop();

	//�Ķ���� üũ
	if (words.front() != L"(")
		this->print_error(L"����� ���ڿ��� �ϳ��� ���� �� �־��");
	else
	{
		func_signature += words.front();
		words.pop();
	}

	while (words.front() != L")")
	{

		/*	main �Լ�	*/
			if (this->in_main_func)
			{
				this->cmdline_arg_name = words.front();
				words.pop();

				if (words.front() != L")") //���� �� ����
				{
					wprintf(func_signature.c_str());
					this->read_line_if_empty();
					if (words.front() != L":")
					{
						words.pop();
						words.pop();
					}
					else
						this->print_error(L"����� ���ڿ��� �ϳ��� ���� �� �־��");
				}

				func_signature += L"int ";
				func_signature += TEMP_argc;
				func_signature += L",char** ";
				func_signature += TEMP_args;

				break;
			}
		/*	main �Լ�	*/


		//������
		if (words.front() == keywords::MUT)
			words.pop();
		else if (words.front() == keywords::CONST_)
		{
			func_signature += L"const ";
			words.pop();
		}
		else //����Ʈ const
			func_signature += L"const ";


		//������ ����
		auto var_name = words.front();
		words.pop();

		if (words.front() != L":")
			this->print_error(L"�Ķ���Ϳ� �ݷ� ��𰬾��?");
		words.pop();

		//Ÿ��
		func_signature += words.front();
		func_signature += ' ';
		func_signature += var_name;
		words.pop();

		//�ݷ�
		if (words.front() == L",")
		{
			func_signature += words.front();
			words.pop();
		}
	}
	func_signature += words.front();
	words.pop();

	//����Ÿ��
	if (words.empty() || words.front() == L"{") //������ void
		if (in_main_func)
			func_signature += L"->int";
		else
			func_signature += L"->void";
	else if (words.front().find(L"->") != wstring::npos)
	{
		if (words.front() == L"->") //->�� Ÿ���� ���������� ���
		{
			func_signature += words.front();
			words.pop();
		}
		func_signature += words.front();
		words.pop();
	}
	else
		this->print_error(L"����Ÿ�� �κ��� �̻��ѰŰ�����");

	this->prototypes += func_signature;
	this->prototypes += L";\n";
	this->bodys += func_signature;
	this->bodys += '\n';

	//
	if (words.empty())
		this->read_line();
	if (words.front() == L"{")
	{
		this->bodys += words.front(); words.pop();
		this->unclosed_bracket++;
		this->interpret_local();
	}
	return;
}


void Interpreter::interpret_variable()
{
	wstring variable_expr = L"";
	bool has_init = false;

	auto head_word = this->words.front(); this->words.pop();

	if (head_word == keywords::LITERAL)
	{
		variable_expr += L"constexpr auto ";

		while (!words.empty())
		{
			variable_expr += this->words.front();
			variable_expr += ' ';
			this->words.pop();
		}
	}

	else if (head_word == keywords::CONST_ || head_word == keywords::MUT)
	{
		//const ���� üũ
		if (head_word == keywords::CONST_)
			variable_expr += keywords::CONST_, variable_expr += ' ';
		this->words.pop();

		//���� �̸� ����
		wstring var_name = words.front();
		words.pop();

		//Ÿ�� üũ
		if (words.front() == L":") //Ÿ���� ��õ� ���
		{
			words.pop();
			variable_expr += words.front();
			words.pop();
		}
		else //������ �߷�
		{
			variable_expr += L"auto";
		}

		variable_expr += ' ';
		variable_expr += var_name;

		//������ ���ٺ��̱�
		while (!words.empty())
		{
			variable_expr += words.front();
			variable_expr += ' ';
			words.pop();
		}
	}

	this->bodys += variable_expr;
}

void Interpreter::set_original_filepath(const wstring &path)
{
	this->_original_filepath = path;
}