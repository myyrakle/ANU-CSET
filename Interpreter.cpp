#include "pch.h"
#include "Interpreter.h"
#include "Handler.h"

enum class Interpret_Flag
{
	GLOBAL,
	FUNC_SIG,
	LOCAL,
	CLASS,
	METHOD_SIG,
};

void Interpreter::set_caller(Handler * ptr)
{
	caller = ptr;
}

void Interpreter::interpret(std::queue<std::wstring>& tokens)
{
	this->do_import(L"basic");
	//...
	while(!tokens.empty())
		this->interpret_global(tokens);

	this->finish();
}

void Interpreter::do_import(const std::wstring& module_name)
{
	//Library ��� ��� �о����
	wifstream read_module(((caller->_original_filepath + L"\\Library\\") + module_name) + L".h");
	
	if (!read_module) this->print_error(L"����� �ȿ����׿�.");
	
	while (read_module)
		caller->prototypes += read_module.get();
	caller->prototypes += L"\n\n";

	read_module.close();
}

void Interpreter::finish()
{
	/*wofstream fout(this->_original_filepath + L"\\temp\\temp.cpp");

	
	for (auto& source : this->prototypes)
		fout << source;
	for (auto& source : this->bodys)
		fout << source;

	fout.close();*/
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


void Interpreter::print_error(wstring_view log)
{
	/*wprintf(L"���ϸ� %s\n", this->current_filename.data());
	wprintf(L"���� : %d\n", this->line_num);
	wprintf(L"%s", log.data());
	
	this->finish();

	exit(EXIT_SUCCESS);*/
}

void Interpreter::interpret_global(std::queue<std::wstring>& tokens)
{
	//func Ű���� ���� ���
	if (tokens.front() == keywords::FUNC)
	{
		this->interpret_function(); //�ñ׳�ó �Ľ�
		this->interpret_local(); //��ü ���� ����
	}

	//static Ű���� ���� ���
	else if (tokens.front() == keywords::STATIC)
	{
		this->print_error(L"���������� static �����");
	}

	//const�� mut, literal Ű������ ���
	else if (tokens.front() == keywords::CONST_ || tokens.front() == keywords::MUT
		|| tokens.front() == keywords::LITERAL)
	{
		this->interpret_variable();
	}

	//import Ű������ ���
	else if (tokens.front() == keywords::IMPORT)
	{
		tokens.pop(); //import ����

		if (tokens.empty())
			this->print_error(L"import�� ����� �����?");
		else
		{
			this->do_import(tokens.front()); //console�̸� console.h �о����
			tokens.pop();
		}
	}

	//class Ű������ ���
	else if (tokens.front() == keywords::CLASS)
	{
		//this->interpret_class();
	}

	else if (tokens.front() == keywords::NAMESPACE)
	{
		//To do
	}

	else if (tokens.front() == keywords::USING)
	{
		tokens.pop();
		caller->bodys += L"using namespace ";
		caller->bodys += tokens.front();
		caller->bodys += ';';
		//this->print_error(L"using�� ���� �� ���׿�?");
	}

	else //�ƹ��͵� �ƴ� ���
	{
		//tokens += token;
		caller->prototypes += L";\n";
		return;
	}
}

void Interpreter::interpret_local(std::queue<std::wstring>& tokens)
{
	if (this->in_main_func) //����� ���� �ִ� main �Լ��� ���
	{
		in_main_func = false;

		caller->bodys += L"\nsetlocale(LC_ALL, \"\");\n";
		if (!cmdline_arg_name.empty())
		{
			//�� ���·� ��ȯ��
			/*
			DynamicArray<std::wstring> �����_���ڸ�(����_����);
			for(int i = 0; i < ����_����; i++)
				�����_���ڸ�[i] = ���ڵ�[i];
			*/
			//caller->bodys += L"auto ___strlen = [](const char* str){ size_t i=0; for(;s[i]!='\0';i++); return i; };";
			caller->bodys += L"DynamicArray<std::wstring> "; caller->bodys += cmdline_arg_name;
			caller->bodys += '('; caller->bodys += TEMP_argc; caller->bodys += L");\n";

			caller->bodys += L"for(int i=0;i<"; caller->bodys += TEMP_argc; caller->bodys += L";i++)\n\t";
			caller->bodys += cmdline_arg_name;
			caller->bodys += L"[i]="; caller->bodys += TEMP_args; caller->bodys += L'[i];';
			
			cmdline_arg_name.clear();
		}
	}

	while (this->unclosed_bracket != 0) //�߰�ȣ �� ���������� ����
	{
		if (tokens.empty())
		{
			caller += L";\n";
			this->read_line_if_empty();
		}

		if (tokens.front() == L"{")
		{
			this->unclosed_bracket++;
			caller->bodys += tokens.front(); tokens.pop();
			continue;
		}
		else if (tokens.front() == L"}")
		{
			this->unclosed_bracket--;
			caller->bodys += tokens.front(); tokens.pop();
			continue;
		}
		//const�� mut, literal Ű������ ���
		else if (tokens.front() == keywords::CONST_ || tokens.front() == keywords::MUT
			|| tokens.front() == keywords::LITERAL)
		{
			this->interpret_variable();
			caller->bodys += ';';
		}

		else if (tokens.front() == keywords::CLASS)
		{
			this->print_error(L"���� Ŭ���� ������");
		}

		//To do
		else
		{
			caller->bodys += tokens.front(); tokens.pop();
		}
	}

	return;
}

void Interpreter::interpret_function(std::queue<std::wstring>& tokens)
{
	wstring func_signature = L"";

	if (tokens.front() == keywords::FUNC)
		func_signature += L"auto ", tokens.pop();

	//main �Լ� ���� üũ
	if (tokens.front() == L"main")
		this->in_main_func = true;

	//�Լ� �̸�
	func_signature += in_main_func ? L"wmain" : tokens.front();
	tokens.pop();

	//�Ķ���� üũ
	if (tokens.front() != L"(")
		this->print_error(L"���°�ȣ ������");
	else
	{
		func_signature += tokens.front();
		tokens.pop();
	}

	/*	main �Լ�	*/
	if (this->in_main_func)
	{
		if (tokens.front() == L")") //�Ķ���Ͱ� ���� ���
		{

		}
		else //�Ķ���Ͱ� ���� ���
		{
			this->cmdline_arg_name = tokens.front(); tokens.pop(); //����� ���ڸ� ȹ��

			// : üũ
			if (tokens.front() == L":")
			{
				tokens.pop();
			}
			else
				this->print_error(L"Ÿ�� ��𰬾��");
			
			if (tokens.front() == L"string[]")
			{
				tokens.pop();
			}
			else
				this->print_error(L"����� ���ڴ� string �迭�� �ü� �־��");

			if (tokens.front() != L")")
				this->print_error(L"�ݴ°�ȣ ��𰬾��?");

			func_signature += L"int ";
			func_signature += TEMP_argc;
			func_signature += L",wchar_t** ";
			func_signature += TEMP_args;
			func_signature += L')';

			if(tokens.empty())
			{
				func_signature += L"-> int";
			}
			else
			{
				if (tokens.front() == L"->")
					func_signature += L"->", tokens.pop();
				else
					this->print_error(L"��ȯŸ�� ������");

				if (tokens.front() == L"int")
					func_signature += L"int", tokens.pop();
				else
					this->print_error(L"main �Լ� ��ȯ���� ������ int����");
			}
		}
	}

	/*main �Լ��� �ƴ� ����*/
	else
	{
		while (tokens.front() != L")")
		{
			//������
			if (tokens.front() == keywords::MUT)
				tokens.pop();
			else if (tokens.front() == keywords::CONST_)
			{
				func_signature += L"const ";
				tokens.pop();
			}
			else //����Ʈ const
				func_signature += L"const ";

			//������ ����
			auto var_name = tokens.front();
			tokens.pop();

			if (tokens.front() != L":")
				this->print_error(L"�Ķ���Ϳ� �ݷ� ��𰬾��?");
			tokens.pop();

			//Ÿ��
			func_signature += tokens.front();
			func_signature += ' ';
			func_signature += var_name;
			tokens.pop();

			//�ݷ�
			if (tokens.front() == L",")
			{
				func_signature += tokens.front();
				tokens.pop();
			}
		}

		func_signature += tokens.front();  tokens.pop(); // ) ����

	//����Ÿ��
		if (tokens.empty() || tokens.front() == L"{") //������ void
			func_signature += L"->void";

		else if (tokens.front() == L"->")
		{
			func_signature += tokens.front();
			tokens.pop();
		}
		else
			this->print_error(L"����Ÿ�� �κ��� �̻��ѰŰ�����");

		caller->prototypes += func_signature;
		caller->prototypes += L";\n";
		caller->bodys += func_signature;
		caller->bodys += '\n';
	}
	
	//
	if (tokens.empty())
		if (caller->readable())
			tokens = std::move(caller->read_line_then_tokenize());

	if (tokens.front() == L"{")
	{
		caller->bodys += tokens.front(); tokens.pop();
		this->unclosed_bracket++;
		this->interpret_local(tokens);
	}

	return;
}


void Interpreter::interpret_variable(std::queue<std::wstring>& tokens)
{
	//wstring variable_expr = L"";
	//bool has_init = false;

	//auto head_word = this->words.front(); this->words.pop();

	//if (head_word == keywords::LITERAL)
	//{
	//	variable_expr += L"constexpr auto ";

	//	while (!words.empty())
	//	{
	//		variable_expr += this->words.front();
	//		variable_expr += ' ';
	//		this->words.pop();
	//	}
	//}

	//else if (head_word == keywords::CONST_ || head_word == keywords::MUT)
	//{
	//	//const ���� üũ
	//	if (head_word == keywords::CONST_)
	//		variable_expr += keywords::CONST_, variable_expr += ' ';
	//	this->words.pop();

	//	//���� �̸� ����
	//	wstring var_name = words.front();
	//	words.pop();

	//	//Ÿ�� üũ
	//	if (words.front() == L":") //Ÿ���� ��õ� ���
	//	{
	//		words.pop();
	//		variable_expr += words.front();
	//		words.pop();
	//	}
	//	else //������ �߷�
	//	{
	//		variable_expr += L"auto";
	//	}

	//	variable_expr += ' ';
	//	variable_expr += var_name;

	//	//������ ���ٺ��̱�
	//	while (!words.empty())
	//	{
	//		variable_expr += words.front();
	//		variable_expr += ' ';
	//		words.pop();
	//	}
	//}

	//this->bodys += variable_expr;
}

