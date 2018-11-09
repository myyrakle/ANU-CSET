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
	if (module_name == L"gui") //���Ӽ� üũ
	{
		caller->builder.add_file(L"opengl32.lib");
		caller->builder.add_file(L"winmm.lib");
		caller->builder.add_file(L"freetype.lib");
		caller->builder.add_file(L"sfml-window-s.lib");
		caller->builder.add_file(L"sfml-graphics-s.lib");
		caller->builder.add_file(L"sfml-system-s.lib");
	}
	else if (module_name == L"audio")
	{
		caller->builder.add_file(L"sfml-audio-s-d.lib");
		caller->builder.add_file(L"openal32.lib");
		caller->builder.add_file(L"sfml-system-s-d.lib");
		caller->builder.add_file(L"flac.lib");
		caller->builder.add_file(L"vorbisenc.lib");
		caller->builder.add_file(L"vorbisfile.lib");
		caller->builder.add_file(L"vorbis.lib");
		caller->builder.add_file(L"ogg.lib");
	}
	else if (module_name == L"network")
	{
		caller->builder.add_file(L"sfml-network-s-d.lib");
		caller->builder.add_file(L"sfml-system-s-d.lib");
		caller->builder.add_file(L"ws2_32.lib");
	}

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

bool Interpreter::is_keyword(const std::wstring & word) const
{
	if (word == keywords::BOOL ||
		word == keywords::BYTE ||
		word == keywords::ALIAS ||
		word == keywords::BREAK ||
		word == keywords::CASE ||
		word == keywords::CHAR ||
		word == keywords::CLASS ||
		word == keywords::CONSTFUNC ||
		word == keywords::CONST_ ||
		word == keywords::CONTINUE ||
		word == keywords::DOUBLE ||
		word == keywords::ELSE ||
		word == keywords::ENUM ||
		word == keywords::FALSE_ ||
		word == keywords::FLOAT ||
		word == keywords::FOR ||
		word == keywords::FUNC ||
		word == keywords::FUNCTYPE ||
		word == keywords::IF ||
		word == keywords::IMPORT ||
		word == keywords::INT ||
		word == keywords::INT16 ||
		word == keywords::INT32 ||
		word == keywords::INT64 ||
		word == keywords::INT8 ||
		word == keywords::IN_ ||
		word == keywords::LABEL |
		word == keywords::LITERAL ||
		word == keywords::MATCH ||
		word == keywords::MUT ||
		word == keywords::NAMESPACE ||
		word == keywords::NEW ||
		word == keywords::NULL_ ||
		word == keywords::OBJECT ||
		word == keywords::PRIVATE ||
		word == keywords::PROTECTED ||
		word == keywords::PUBLIC ||
		word == keywords::RCNEW ||
		word == keywords::RFOR ||
		word == keywords::SIZEOF ||
		word == keywords::STATIC ||
		word == keywords::STRING ||
		word == keywords::SWITCH ||
		word == keywords::TRUE_ ||
		word == keywords::UINT ||
		word == keywords::UINT16 ||
		word == keywords::UINT32 ||
		word == keywords::UINT64 ||
		word == keywords::UINT8 ||
		word == keywords::USING ||
		word == keywords::WHILE)
		return true;
	else
		return false;
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
	wprintf(L"���� : %d\n", caller->line_number);
	wprintf(L"%s", log.data());
	
	this->finish();

	exit(EXIT_SUCCESS);
}

void Interpreter::interpret_global(std::queue<std::wstring>& tokens)
{
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
			caller->bodys += L"DynamicArray<String> "; caller->bodys += cmdline_arg_name;
			caller->bodys += '('; caller->bodys += TEMP_argc; caller->bodys += L");\n";

			caller->bodys += L"for(int i=0;i<"; caller->bodys += TEMP_argc; caller->bodys += L";i++)\n\t";
			caller->bodys += cmdline_arg_name;
			caller->bodys += L"[i]="; caller->bodys += TEMP_args; caller->bodys += L"[i];";
			
			cmdline_arg_name.clear();
		}
	}

	//�߰�ȣ �� ���������� ����
	while (true) 
	{
		while (tokens.empty())
		{
			if (caller->readable())
				tokens = std::move(caller->read_line_then_tokenize());
			else
				return;

			if (!this->pre_word.empty())
				caller->bodys += pre_word;

			//��..
			caller->bodys += L";\n";
			if (straight)
				caller->bodys += L"}}";
		}

		if (tokens.front() == L"{")
		{
			unclosed_bracket++;
			caller->bodys += tokens.front(); tokens.pop();
			caller->bodys += L'\n';
			//continue;
		}
		else if (tokens.front() == L"}")
		{
			unclosed_bracket--;
		
			caller->bodys += L";\n";
			caller->bodys += tokens.front(); tokens.pop();
			caller->bodys += L'\n';

			

			/*�ӽù���*/

			/*�ӽù���*/

			//continue;
		}

		//const�� mut, literal Ű������ ���

		else if (tokens.front() == keywords::STATIC)
		{
			caller->bodys += L"static ";
			tokens.pop();
		}

		else if (tokens.front() == keywords::CONST_ 
			|| tokens.front() == keywords::MUT
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
			state = InterpreterState::In_LAMBDA;
			caller->bodys += L"[&]"; tokens.pop();
			this->interpret_lambda(tokens);
		}

		else if (tokens.front() == L"for")
		{
			this->interpret_for(tokens);

			while (tokens.empty())
			{
				if (caller->readable())
					tokens = std::move(caller->read_line_then_tokenize());
				else
					return;
			}
		}

		else if (tokens.front() == L"rfor")
		{
			this->print_error(L"rfor�� ���� �̿ϼ��̿���");
			this->interpret_rfor(tokens);

			while (tokens.empty())
			{
				if (caller->readable())
					tokens = std::move(caller->read_line_then_tokenize());
				else
					return;
			}

			if (tokens.front() == L"{")
			{
				//Todo
			}
			else
			{
				straight = true;
			}
		}

		else if(tokens.front() ==L"while")
		{ 
			this->interpret_while(tokens);
		}

		else if (tokens.front() == L"if")
		{
			this->interpret_if(tokens);
		}

		else if (tokens.front() == L"else")
		{
			this->interpret_if(tokens);
		}

		else //�������� �׳�
		{
			interpret_check_Range(tokens);
		}

		if (unclosed_bracket == 0)
		{
			break;
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
		this->print_error(L"���°�ȣ ������. �̰� �����׿�-> "+tokens.front());
	else
	{
		func_signature+=tokens.front(); tokens.pop();
	}

	/*	main �Լ�	*/
	if (this->in_main_func)
	{
		if (tokens.front() == L")") //�Ķ���Ͱ� ���� ���
		{
			func_signature += L')'; tokens.pop();
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
			tokens.pop();

			func_signature += L"int ";
			func_signature += TEMP_argc;
			func_signature += L",wchar_t** ";
			func_signature += TEMP_args;
			func_signature += L')';
		}

		if(tokens.empty()||tokens.front()==L"{")
		{
			func_signature += L"->int";
		}
		else
		{
			if (tokens.front() == L"->")
				func_signature += L"->", tokens.pop();
			else
				this->print_error(L"��ȯŸ�� ������"+tokens.front()+L"��");

			if (tokens.front() == L"int")
				func_signature += L"int", tokens.pop();
			else
				this->print_error(L"main �Լ� ��ȯ���� ������ int�� void����");
		}
	}

	/*main �Լ��� �ƴ� ����*/
	else
	{
		while (tokens.front() != L")")
		{
			bool is_mut = false;

			if (tokens.front() == keywords::MUT)
				tokens.pop(), is_mut = true;

			//������
			else if (tokens.front() == keywords::CONST_)
			{
				tokens.pop();
			}
			
			if (!is_mut)
				func_signature += L"const ";

			//������ ����
			auto var_name = tokens.front(); tokens.pop();

			if (tokens.front() != L":")
				this->print_error(L"�Ķ���Ϳ� �ݷ� ��𰬾��? <- "+tokens.front());
			else
				tokens.pop();

			func_signature += interpret_types(tokens);

			//Ÿ��
			func_signature += L" " + var_name;

			//�ݷ�
			if (tokens.front() == L",")
			{
				func_signature += tokens.front();
				tokens.pop();
			}
			else
			{
				if (tokens.front() != L")")
					print_error(L"���� �̰� <-"+tokens.front());
			}
		}

		func_signature += tokens.front();  tokens.pop(); // ) ����	

		while (tokens.empty())
			if (caller->readable())
				tokens = std::move(caller->read_line_then_tokenize());

		if (tokens.empty() || tokens.front() == L"{") //������ void
			func_signature += L"->void";
		else if (tokens.front() == L"->")
		{
			func_signature += tokens.front(); tokens.pop();
			func_signature += this->interpret_types(tokens);
		}
		else
			this->print_error(L"����Ÿ�� �κ��� �̻��ѰŰ�����");
	}

	caller->headers += func_signature;
	caller->headers += L";\n";
	caller->prototypes += func_signature;
	caller->prototypes += L";\n";
	caller->bodys += func_signature;
	caller->bodys += '\n';

	while (tokens.empty())
		if (caller->readable())
			tokens = std::move(caller->read_line_then_tokenize());

	if (tokens.front() == L"{")
	{
		caller->bodys += tokens.front(); tokens.pop();
		this->interpret_local(tokens,1);
	}
	return;
}

void Interpreter::interpret_check_Range(std::queue<std::wstring>& tokens)
{
	if (tokens.front() == L"(")
	{
		unclosed_small_bracket++;
	}
	else if (tokens.front() == L")")
	{
		unclosed_small_bracket--;
	}

	if (in_range)
	{
		post_word += tokens.front(); tokens.pop();

		if (unclosed_small_bracket == 0)
		{
			std::wstring temp = L"Range(";
			temp += pre_word;
			temp += L',';
			temp += post_word;
			temp += L')';

			pre_word.clear();
			post_word.clear();
			in_range = false;

			caller->bodys += temp;
		}
	}

	//�⺻����
	else
	{
		//.. ���������� ����ؼ� operand1 ����		
		pre_word += tokens.front(); tokens.pop(); //�̾Ƴ���

		if (unclosed_small_bracket == 0)
		{
			if (tokens.front() == L"..") //..���� �˻�
			{
				in_range = true;
				tokens.pop(); //���������� ó�������� �̵�
			}
			else
			{
				caller->bodys += pre_word;
				pre_word.clear(); //�ƴϸ� �׳� �ѱ�
			}
		}
	}
}

/*����ġ�� �ʿ�*/
void Interpreter::interpret_lambda(std::queue<std::wstring>& tokens)
{
	if (tokens.front() == L"(") //�Ķ���� ���� ���
	{
		//interpret_function ����

		std::wstring lambda_signature = L"("; tokens.pop();

		while (tokens.front() != L")")
		{
			bool is_mut = false;

			if (tokens.front() == keywords::MUT)
				tokens.pop(), is_mut = true;

			//������
			else if (tokens.front() == keywords::CONST_)
			{
				tokens.pop();
			}

			if (!is_mut)
				lambda_signature += L"const ";

			//������ ����
			auto var_name = tokens.front(); tokens.pop();

			if (tokens.front() != L":")
				this->print_error(L"�Ķ���Ϳ� �ݷ� ��𰬾��?");
			tokens.pop();

			lambda_signature += interpret_types(tokens);

			//Ÿ��
			lambda_signature += ' ';
			lambda_signature += var_name;
			
			//�ݷ�
			if (tokens.front() == L",")
			{
				lambda_signature += tokens.front();
				tokens.pop();
			}
			else
				print_error(L"���� �̰�" + tokens.front());
		}

		lambda_signature += tokens.front();  tokens.pop(); // ) ����

	//����Ÿ��
		while (tokens.empty())
			if(caller->readable())
				tokens = std::move(caller->read_line_then_tokenize());

		if(tokens.front() == L"{") //������ void
			lambda_signature += L"->void";
		else if (tokens.front() == L"->")
		{
			lambda_signature += tokens.front(); tokens.pop();
			lambda_signature += this->interpret_types(tokens);
		}
		else
			this->print_error(L"����Ÿ�� �κ��� �̻��ѰŰ�����");

		caller->bodys += lambda_signature;
		caller->bodys += '\n';
	}
	
	if(tokens.front() == L"{")//�Ķ���� ���� ���
	{
		this->interpret_local(tokens, 1);
	}
}

void Interpreter::interpret_for(std::queue<std::wstring>& tokens)
{
	caller->bodys += L"for"; tokens.pop();

	if (tokens.front() != L"(")
		this->print_error(L"for�� ���°�ȣ ������");
	
	caller->bodys += tokens.front(); tokens.pop(); // ( ����

	int unclosed_bracket = 1; //�ȴ��� �Ұ�ȣ

	//each ������ mutable�ΰ�?
	bool each_is_mut = false;
	
	if (tokens.front() == L"mut")
		each_is_mut = true, tokens.pop();
	else if (tokens.front() == L"const")
		tokens.pop();

	//each ���� üũ
	std::wstring each_name = tokens.front(); tokens.pop();

	if (tokens.front() == keywords::IN_)
	{
		if (each_is_mut)
			caller->bodys += L"auto&";
		else
			caller->bodys += L"const auto&";

		caller->bodys += each_name;

		caller->bodys += L":"; tokens.pop();
	}
	else
	{
		print_error(L"in ������");
	}

	if (tokens.empty())
		print_error(L"�����̳� ������");

	//�ݴ°�ȣ ���ö����� �Է�
	
	while(unclosed_bracket!=0)
	{
		if (tokens.front() == L"(")
			unclosed_bracket++;
		else if (tokens.front() == L")")
			unclosed_bracket--;

		if (unclosed_bracket == 0)
			break;

		this->interpret_check_Range(tokens);
	}
	caller->bodys += tokens.front(); tokens.pop();
}

/*
{
auto �����̳� = ǥ����;
for(auto i = �����̳�.rbegin(); i!=�����̳�.rend();i++)
...
}
*/

/*����������*/
void Interpreter::interpret_rfor(std::queue<std::wstring>& tokens)
{
	caller->bodys += L"{\n"; more_bracket_in_rfor++;
	tokens.pop(); //rfot ����

	if (tokens.front() != L"(")
		this->print_error(L"rfor�� ���°�ȣ ������");

	tokens.pop(); // ( ��

	int unclosed_bracket = 1; //�ȴ��� �Ұ�ȣ

	//each ������ mutable�ΰ�?
	bool each_is_mut = false;

	if (tokens.front() == L"mut")
		each_is_mut = true, tokens.pop();
	else if (tokens.front() == L"const")
		tokens.pop();

	//each ���� üũ
	std::wstring each_name = tokens.front(); tokens.pop();

	std::wstring container_name = TEMP_name + L"__contanier";

	if (tokens.front() == keywords::IN_)
	{
		caller->bodys += L"auto " + container_name;

		tokens.pop();
	}
	else
	{
		print_error(L"in ������");
	}

	if (tokens.empty())
		print_error(L"�����̳� ������");

	//�ݴ°�ȣ ���ö����� �Է�

	while (unclosed_bracket != 0)
	{
		if (tokens.front() == L"(")
			unclosed_bracket++;
		else if (tokens.front() == L")")
			unclosed_bracket--;

		if (unclosed_bracket == 0)
			break;

		this->interpret_check_Range(tokens);
	}
	tokens.pop(); // ) ����
	
	auto iter_name = TEMP_name + L"__iter";

	caller->bodys += L"for(auto "; caller->bodys += iter_name;

	caller->bodys += '='; caller->bodys += container_name + L".rbegin(); ";

	caller->bodys += iter_name; caller->bodys += L"!="; 
	
	caller->bodys += container_name + L"rend();";

	caller->bodys += iter_name + L"++)\n";

	caller->bodys += L"{\n"; more_bracket_in_rfor++;

	caller->bodys += each_is_mut ? L"auto& " : L"const auto& ";

	caller->bodys += each_name + L"*" + iter_name + L";\n";
}

void Interpreter::interpret_while(std::queue<std::wstring>& tokens)
{
	caller->bodys += L"while"; tokens.pop();

	if (tokens.front() != L"(")
		this->print_error(L"while�� ���°�ȣ ������");
	else
	{
		caller->bodys += tokens.front(); tokens.pop(); // ( ����
	}

	int unclosed_if_bracket = 1;

	while (unclosed_if_bracket != 0)
	{
		if (tokens.front() == L"(")
			unclosed_if_bracket++;
		else if (tokens.front() == L")")
			unclosed_if_bracket--;

		if (unclosed_if_bracket == 0)
			break;

		this->interpret_check_Range(tokens);
	}
	caller->bodys += tokens.front(); tokens.pop();
}

void Interpreter::interpret_if(std::queue<std::wstring>& tokens)
{
	caller->bodys += L"if"; tokens.pop();

	if (tokens.front() != L"(")
		this->print_error(L"if�� ���°�ȣ ������");
	else
	{
		caller->bodys += tokens.front(); tokens.pop(); // ( ����
	}

	int unclosed_if_bracket = 1;

	while (unclosed_if_bracket != 0)
	{
		if (tokens.front() == L"(")
			unclosed_if_bracket++;
		else if (tokens.front() == L")")
			unclosed_if_bracket--;

		if (unclosed_if_bracket == 0)
			break;

		this->interpret_check_Range(tokens);
	}
	caller->bodys += tokens.front(); tokens.pop();
}

void Interpreter::interpret_else(std::queue<std::wstring>& tokens)
{
	caller->bodys += L"else\n"; tokens.pop();
}


std::wstring Interpreter::interpret_types(std::queue<std::wstring>& tokens)
{
	//��ü�� ���� �ʿ�
	//���� ������ ����

	std::wstring type;

	std::wstring array_expr;
	int unclosed_bracket = 0;

	while (true)
	{
		if (tokens.empty() ||
			tokens.front() == L")" ||
			tokens.front() == L"," ||
			tokens.front() == L"=" ||
			tokens.front() ==L"{" ||
			tokens.front() ==L";")
			break;

		//Ÿ�� Ű���� �浹 ����
		if (tokens.front() == L"int" ||
			tokens.front() == L"double" || 
			tokens.front() == L"float" ||
			tokens.front() == L"char" ||
			tokens.front() == L"bool")
		{
			tokens.front()[0] -= ((int)'a' - (int)'A');
			type += tokens.front(); tokens.pop();
		}

		//�迭 ��� ��ȯ
		else if(tokens.front()==L"[")
		{
			//static_array<����, Ÿ��>
			array_expr += L"Array<";
			array_expr += tokens.front(); tokens.pop();

			unclosed_bracket++;
		}
		else if (tokens.front() == L"]") //dynamic_array
		{
			unclosed_bracket--;
			array_expr += L"DynamicArray<"; tokens.pop();
			
			if (tokens.front() == L"[")
			{

			}
			else
			{
				type = array_expr + type + std::wstring(unclosed_bracket, '>'); //??
			}
		}
			
		//�ڽ� üũ
		else if (tokens.front() == L"*")
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
	}

	return type;
}

void Interpreter::interpret_variable(std::queue<std::wstring>& tokens)
{
	wstring variable_expr = L"";
	bool has_init = false;

	

	if (tokens.front() == keywords::LITERAL)
	{
		variable_expr += L"constexpr auto "; tokens.pop();

		while (!tokens.empty())
		{
			variable_expr += tokens.front();
			variable_expr += ' ';
			tokens.pop();
		}
	}

	else if (tokens.front() == keywords::CONST_ 
		|| tokens.front() == keywords::MUT)
	{
		//const ���� üũ
		bool is_mut = false;

		if (tokens.front() == L"mut")
			is_mut = true;
		else {}
		tokens.pop();

		//���� �̸� ����
		wstring var_name = tokens.front(); tokens.pop();

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

		std::wcout << variable_expr << std::endl;

		variable_expr += ' ';
		variable_expr += var_name;

		//������ ���ٺ��̱�
		if (!tokens.empty())
		{
			if(tokens.front()==L"=")
			{
				while(!tokens.empty())
				{
					variable_expr += tokens.front();
					variable_expr += ' ';
					tokens.pop();
				}
			}
		}
	}
	else
		print_error(L"���ߤ�");


	caller->bodys += variable_expr;
}

