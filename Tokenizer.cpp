#include "pch.h"
#include "Tokenizer.h"

//���� �о ȭ��Ʈ�����̽� �������� �ܾ� �и�. 
//�ڸ�Ʈ�� ����. 
//���ڿ� ���ͷ��� String���� ó��
//ToDo : ������ Int�� ó��
//ToDo : �Ǽ��� Double�� ó��
//ToDo : ���ڴ� Char�� ó��


//�� ���� ���� �÷����Դϴ�.
enum class BlockFlag
{
	BlockComment,
	BlockString,
	DEFAULT,
};

//���� ���� ���� �÷����Դϴ�.
enum class LineFlag
{
	COMMENT, //�ּ��� �д� ���Դϴ�.
	STRING, //���ڿ��� �д� ���Դϴ�.
	CHAR, //���ڸ� �д� ���Դϴ�.
	INTEGER, //������ �д� ���Դϴ�.
	ID, //��Ÿ �ĺ����Դϴ�.
	DEFAULT, //�⺻ �÷����Դϴ�.
};

std::queue<std::wstring> Tokenizer::tokenize(const std::wstring& line)
{
	std::queue<std::wstring> tokens; //��ūȭ�� �����

	wstring word; //�ܾ� �ϳ�

	LineFlag line_flag = LineFlag::DEFAULT;
	static BlockFlag block_flag = BlockFlag::DEFAULT;

	bool in_escape = false;

	for (auto& c : line)
	{
		switch (line_flag)
		{
		case LineFlag::DEFAULT : 
		{
			switch (c)
			{
			case '\'': line_flag = LineFlag::CHAR; 
				if (!word.empty()) 
					tokens.push(std::move(word));
				word = L"Char(L\'";
				break;

			case '\"': line_flag = LineFlag::STRING; 
				if (!word.empty())
					tokens.push(std::move(word));
				word = L"String(L\"";
				break;

			//ȭ��Ʈ�����̽� ó��
			case ' ' : 
			case '\n' :
			case '\t' :
				break;

			//��ȣ ó��
			case '(': 
			case ')':
			case '{':
			case '}':
			case '[':
			case ']':
				if (!word.empty()) 
					tokens.push(std::move(word));
				tokens.emplace(1, c);
				break;

			//������ ó��
			case '+':
			case '-':
			case '/':
			case '*':
			case '%':
			case '&':
			case '|':
			case '?':
			case '!':
			case '=':
			case '<':
			case '>':
			case '^':
			case '~':
			case '.':
			case ';':
			case ':':
				if (!word.empty()) 
					tokens.push(std::move(word));
				word += c;
				break;

			default: word += c;
				if (word == L"//")
				{
					line_flag = LineFlag::COMMENT;
					continue;
				}
			}
		} break;

		case LineFlag::COMMENT : //�ּ��� ó����
		{
			
		} break;

		case LineFlag::STRING : //���ڿ� ���ͷ� ó����
		{
			if (c == '\\')
				if(in_escape==false)
				{
					in_escape = true;
					word += c;
					continue;
				}

			else if (c == '\"'&&in_escape==false)
			{
				word += L"\")";
				tokens.push(std::move(word));
				line_flag = LineFlag::DEFAULT;
				continue;
			}

			if (in_escape == true)
				in_escape = false;

			word += c;

		} break;

		case LineFlag::CHAR : //���� ���ͷ� ó����
		{

		} break;

		}


		//ȭ��Ʈ�����̽� ���� �и�
		if (c == L'\n' || c == L'\t' || c == L' ')
		{
			if (!word.empty())
			{
				this->convert_typename(word);
				this->words.push(word); /*üũ��*/ wcout << L"üũ : " << word << endl; /*üũ��*/
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
