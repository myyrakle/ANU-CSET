#include "stdafx.h"
#include "Interpreter.h"

void Interpreter::add_file(wstring && filename)
{
	filenames.emplace_back(std::forward<wstring&&>(filename));
}

void Interpreter::interpret()
{
	//���� �о ����.
	for (auto& filename : filenames)
	{
		this->fin.open(filename);
		
		this->line_num = 0;
		while (this->read_line())
			this->interpret_global();

		fin.close();
	}
}

void Interpreter::finish()
{
	wofstream fout(L"temp\\temp.cpp");

	for (auto& source : this->prototypes)
		fout << source;
	for (auto& source : this->bodys)
		fout << source;

	fout.close();
}

bool Interpreter::read_line()
{
	if (this->fin)
	{
		std::getline(this->fin, this->line);
		this->line_num++;
		this->line_to_words();
	}
	return (bool)this->fin;
}

void Interpreter::line_to_words() //���� �о ȭ��Ʈ�����̽� �������� �ܾ� �и�. �ڸ�Ʈ�� ����. ���ڿ� ���ͷ��� wstring���� ó��
{
	wstring word;
	wstring text_literal=L"";

	bool in_text = false;
	bool quote_is_open = false;

	bool is_escape = false;

	bool has_one_slash = false;
	bool in_comment = false;

	for (auto& c : this->line)
	{
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

		if (c == '\\') //�̽������� ���� üũ
		{
			is_escape = true;
			text_literal += '\\';
		}
		else if (c == '\"')
		{
			if (quote_is_open == false) //�� ��
			{
				text_literal = L"wstring(";
				in_text = true;
			}
			else if (is_escape == false)//���� ��
			{
				text_literal += L"\")";
				this->words.push(text_literal);
				in_text = false;
			}
			else // " �̽������� ������ ��
				text_literal += c;
		}
		else if (in_text) //���ڿ� ���ͷ� Ž����
		{
			text_literal += c;
		}
		else if (c == '\n' || c == '\t' || c == ' ') //ȭ��Ʈ�����̽� ���� �и�
		{
			if (word != L"")
			{
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

bool Interpreter::read_line_if_words_empty()
{
	if (this->words.empty())
		return this->read_line();
	return false;
}

/* deprecated
void Interpreter::chars_to_wstring()
{
	auto left_quote = this->line.find('\"');

	if (left_quote != wstring::npos)
	{
		auto right_quote = this->line.rfind('\"');
		if (left_quote == left_quote)
			throw new exception("����ǥ �ϳ� ��𰬼�?");

		this->line.insert(left_quote - 1, L"wstring(");
		this->line.insert(right_quote + 1, L")");
	}
}
//�ڸ�Ʈ, ū����ǥ �켱���� �浹 ���ذ�
void Interpreter::remove_comment()
{
	if (this->line.find(L"//") != wstring::npos)
	{
		this->line = line.substr(0, this->line.find(L"//"));
	}
	else
		return;
}
*/

void Interpreter::interpret_global()
{
	auto head_word = words.front();

	//func Ű���� ���� ���
	if (this->words.front() == keywords::FUNC)
		this->interpret_function();

	//const�� mut, literal Ű������ ���
	else if (head_word == keywords::CONST || head_word == keywords::MUT || head_word == keywords::LITERAL)
		this->interpret_variable();

	//class Ű������ ���
	else if (head_word == keywords::CLASS)
		this->interpret_class;

	/*
	//static�� ���
	else if (head_word == keywords::STATIC)
	{
		if (head_word == keywords::FUNC) //static �Լ�
			this->interpret_function();

		else if (head_word == keywords::CONST || this->words.front() == keywords::MUT) // static ����
			this->interpret_variable();
	}
	*/

	else if (this->words.front() == keywords::NAMESPACE);

	else if (this->words.front() == keywords::USING)
	{
		this->words.pop();
		this->bodys += L"using namespace ";
		this->bodys += words.front();
		this->bodys += ';';
		throw new exception("using�� ���� �� ���׿�?");
	}

	else //�ƹ��͵� �ƴ� ���
	{
		this->bodys += this->line;
		bodys += ';';
		return;
	}
}

void Interpreter::interpret_local()
{

}

void Interpreter::interpret_function()
{
	wstring func_signature = L"";

	if (this->words.front() == keywords::STATIC)
		func_signature += keywords::STATIC, this->words.pop();
	
	if (this->words.front() == keywords::FUNC)
		func_signature += L"auto", this->words.pop();

	//�Լ��� ����
	{
		wstring func_name;
		auto left_bracket = words.front().find('(');
		if (left_bracket != wstring::npos) // (�� �پ����� ��� ����
		{
			func_name = words.front().substr(0, left_bracket);
			words.front() = words.front().substr(left_bracket);
		}
		else
		{
			func_name = words.front();
			words.pop();
		}
		func_signature += ' ';
		func_signature += func_name;

		if (func_name == L"main") //main �Լ��� ���� ó��
		{
			this->interpret_function_main(func_signature);
			return;
		}
	}

	//�Ķ���� üũ
	if (words.front() == L"(") //(�� ���������� ���
	{
		func_signature 
	}
	if (head_word == keywords::CONST || head_word == keywords::MUT)
	{
		//const ���� üũ
		if (head_word == keywords::CONST)
			variable_expr += keywords::CONST, variable_expr += ' ';
		this->words.pop();

		//���� �̸� ����
		wstring var_name;
		auto cur_word = this->words.front();
		if (cur_word.find(':') == wstring::npos)
		{
			var_name = cur_word;
			this->words.pop();
		}
		else // �̸��� : �پ����� ���
		{
			var_name = cur_word.substr(0, cur_word.find(':'));
			this->words.front() = cur_word.substr(cur_word.find(':'));
		}

		//Ÿ��üũ + ������ ����
		if (this->words.front().find(':') == wstring::npos) //:Ÿ�� ǥ�� ������ �ʱ�ȭ �߷�
		{
			variable_expr += L" auto ";
			variable_expr += var_name;
		}
		else //������ ǥ��
		{
			if (words.front() == L":") //:�� �޶� ������
			{
				words.pop(); //������

				variable_expr += words.front(); //���� ������ Ÿ�� ����
			}
			else //:�� Ÿ���̶� �پ�������
			{
				variable_expr += words.front().substr(1); //����� Ÿ�� ����
			}
			words.pop();
			variable_expr += ' ';

			variable_expr += var_name;
		}
	//

}

void Interpreter::interpret_function_main(const wstring& func_signature)
{

}

void Interpreter::interpret_parameter()
{

}

void Interpreter::interpret_variable()
{
	wstring variable_expr = L"";
	bool has_init = false;

	auto head_word = this->words.front(); this->words.pop();

	if (head_word == keywords::LITERAL)
	{
		variable_expr += L"constexpr auto ";
		
		while ( !words.empty() )
		{
			variable_expr += this->words.front();
			variable_expr += ' ';
			this->words.pop();
		}
	}

	else if (head_word == keywords::CONST || head_word == keywords::MUT)
	{
		//const ���� üũ
		if (head_word == keywords::CONST)
			variable_expr += keywords::CONST, variable_expr += ' ';
		this->words.pop();

		//���� �̸� ����
		wstring var_name;
		auto cur_word = this->words.front();
		if (cur_word.find(':') == wstring::npos)
		{
			var_name = cur_word;
			this->words.pop();
		}
		else // �̸��� : �پ����� ���
		{
			var_name = cur_word.substr(0, cur_word.find(':'));
			this->words.front() = cur_word.substr(cur_word.find(':'));
		}

		//Ÿ��üũ + ������ ����
		if (this->words.front().find(':') == wstring::npos) //:Ÿ�� ǥ�� ������ �ʱ�ȭ �߷�
		{
			variable_expr += L" auto ";
			variable_expr += var_name;
		}
		else //������ ǥ��
		{
			if (words.front() == L":") //:�� �޶� ������
			{
				words.pop(); //������

				variable_expr += words.front(); //���� ������ Ÿ�� ����
			}
			else //:�� Ÿ���̶� �پ�������
			{
				variable_expr += words.front().substr(1); //����� Ÿ�� ����
			}
			words.pop();
			variable_expr += ' ';

			variable_expr += var_name;
		}
		//

		//������ ���ٺ��̱�
		while (!words.empty()) 
		{
			variable_expr += words.front();
			variable_expr += ' ';
			words.pop();
		}
		variable_expr += ';';
	}


	
}