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
	//...
	while(!tokens.empty())
		this->interpret_global(tokens);

	this->finish();
}

void Interpreter::do_import(const std::wstring& module_name)
{
	//Library ��� ��� �о����
	std::wstring module_path = caller->_original_filepath + L"\\Library\\" 
		+ module_name + L".h";

	wifstream read_module;
	
	read_module.open(module_path);

	if (!read_module) 
		this->print_error(L"����� �ȿ����׿�. : "+caller->_original_filepath);
	
	std::wstring temp;

	while (read_module)
		temp += read_module.get();

	caller->headers += temp;

	caller->headers += L"\n\n";

	read_module.close();
}

void Interpreter::finish()
{
	wofstream fout(caller->_original_filepath + L"\\temp\\temp.cpp");

	for (auto& source : caller->headers)
		fout << source;
	for (auto& source : caller->prototypes)
		fout << source;
	for (auto& source : caller->bodys)
		fout << source;

	fout.close();
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
	wprintf(L"���ϸ� %s\n",caller->current_filename.data());
	wprintf(L"���� : %d\n", this->line_num);
	wprintf(L"%s", log.data());
	
	this->finish();

	exit(EXIT_SUCCESS);
}

void Interpreter::interpret_global(std::queue<std::wstring>& tokens)
{
	std::wcout << tokens.front() << std::endl;

	//func Ű���� ���� ���
	if (tokens.front() == keywords::FUNC)
	{
		this->interpret_function(tokens); //�ñ׳�ó �Ľ�
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
		this->interpret_variable(tokens);
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
		this->print_error(L"���� Ŭ���� �����");
	}

	else if (tokens.front() == keywords::NAMESPACE)
	{
		//To do
		this->print_error(L"���� ���ӽ����̽� �����");
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
		while (!tokens.empty())
		{
			caller->bodys = tokens.front(); tokens.pop();
		}
		caller->prototypes += L";\n";
		return;
	}
}

void Interpreter::interpret_local(std::queue<std::wstring>& tokens, int unclosed_bracket)
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
			caller->bodys += L"[i]="; caller->bodys += TEMP_args; caller->bodys += L"[i];";
			
			cmdline_arg_name.clear();
		}
	}

	//�߰�ȣ �� ���������� ����
	while (unclosed_bracket > 0) 
	{
		if (tokens.empty())
			if (caller->readable())
				tokens = std::move(caller->read_line_then_tokenize());

		if (tokens.empty())
		{
			caller->bodys += L";\n";
		}

		if (tokens.front() == L"{")
		{
			unclosed_bracket++;
			caller->bodys += tokens.front(); tokens.pop();
			continue;
		}
		else if (tokens.front() == L"}")
		{
			unclosed_bracket--;
			caller->bodys += tokens.front(); tokens.pop();
			continue;
		}

		//const�� mut, literal Ű������ ���

		else if (tokens.front() == keywords::STATIC)
		{
			caller->bodys += L"static ";
			tokens.pop();
		}

		else if (tokens.front() == keywords::CONST_ || tokens.front() == keywords::MUT
			|| tokens.front() == keywords::LITERAL)
		{
			this->interpret_variable(tokens);
			caller->bodys += ';';
		}

		else if (tokens.front() == keywords::CLASS)
		{
			this->print_error(L"���� ���� Ŭ���� ������");
		}

		else if (tokens.front() == keywords::ENUM)
		{
			this->print_error(L"���� ���� ���� ������");
		}

		else if (tokens.front() == L"$")
		{
			caller->bodys += L"[&]";
			this->interpret_lambda(tokens);
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
			
			if (tokens.front() == L"string")
			{
				tokens.pop();
			}
			else
				this->print_error(L"����� ���ڴ� string �迭�� �ü� �־��");

			if (tokens.front() == L"[")
				tokens.pop();
			else
				this->print_error(L"����� ���ڴ� string �迭�� �ü� �־��");

			if (tokens.front() == L"]")
				tokens.pop();
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
		this->interpret_local(tokens,1);
	}

	return;
}

void Interpreter::interpret_lambda(std::queue<std::wstring>& tokens)
{
	if (tokens.front() == L"(") //�Ķ���� ���� ���
	{
		//interpret_function ����

		std::wstring func_signature;

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
		caller->bodys += '\n'; while (tokens.front() != L")")
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
	else if(tokens.front() == L"{")//�Ķ���� ���� ���
	{
		this->interpret_local(tokens, 1);
	}
}

std::wstring Interpreter::interpret_types(std::queue<std::wstring>& tokens)
{
	//��ü�� ���� �ʿ�
	//���� ������ ����

	std::wstring type;

	//Ÿ�� Ű���� �浹 ����
	if (tokens.front() == L"int")
		type += L"Int", tokens.pop();
	else if (tokens.front() == L"double")
		type += L"Double", tokens.pop();
	else if (tokens.front() == L"float")
		type += L"Float", tokens.pop();
	else if (tokens.front() == L"char")
		type += L"Char", tokens.pop();
	else if (tokens.front() == L"bool")
		type += L"Bool", tokens.pop();
	else
		type += tokens.front(), tokens.pop();

	std::wstring array_expr;
	int unclosed_bracket = 0;

	//�迭 ��� ��ȯ
	while(tokens.front()==L"[")
	{
		if (tokens.front() == L"]") //dynamic_array
		{
			array_expr += L"DynamicArray<"; tokens.pop();
			unclosed_bracket++;
		}
		else //static_array<����, Ÿ��>
		{
			array_expr += L"Array<";
			array_expr += tokens.front(); tokens.pop();

			if (tokens.front() == L"]")
				tokens.pop();
			else
				this->print_error(L"�ݴ°�ȣ ������");

			unclosed_bracket++;
		}
	}
	type = array_expr + type + std::wstring(unclosed_bracket, '>');
	
	//�ڽ� üũ
	if (tokens.front() == L"*")
	{
		//���� �̱���
	}

	//rc �ڽ� üũ
	else if (tokens.front() == L"^")
	{
		//���� �̱���
	}

	//nullable üũ
	else if (tokens.front() == L"?")
	{
		//���� �̱���
	}

	else
	{
		type += tokens.front(); tokens.pop();
	}

	return type;
}

void Interpreter::interpret_variable(std::queue<std::wstring>& tokens)
{
	wstring variable_expr = L"";
	bool has_init = false;

	auto head_word = tokens.front(); tokens.pop();

	if (head_word == keywords::LITERAL)
	{
		variable_expr += L"constexpr auto ";

		while (!tokens.empty())
		{
			variable_expr += tokens.front();
			variable_expr += ' ';
			tokens.pop();
		}
	}

	else if (head_word == keywords::CONST_ || head_word == keywords::MUT)
	{
		//const ���� üũ
		if (head_word == keywords::CONST_)
			variable_expr += keywords::CONST_, variable_expr += ' ';
		tokens.pop();

		//���� �̸� ����
		wstring var_name = tokens.front();
		tokens.pop();

		//Ÿ�� üũ
		if (tokens.front() == L":") //Ÿ���� ��õ� ���
		{
			tokens.pop();
			variable_expr += interpret_types(tokens);
		}
		else //������ �߷�
		{
			variable_expr += L"auto";
		}

		variable_expr += ' ';
		variable_expr += var_name;

		//������ ���ٺ��̱�
		while (!tokens.empty())
		{
			variable_expr += tokens.front();
			variable_expr += ' ';
			tokens.pop();
		}
	}
	else
		print_error(L"���ߤ�");


	caller->bodys += variable_expr;
}

