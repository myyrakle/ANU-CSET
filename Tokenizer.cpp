#include "pch.h"
#include "Tokenizer.h"

//���� �о ȭ��Ʈ�����̽� �������� �ܾ� �и�. done
//�ڸ�Ʈ�� ����. done
//���ڿ� ���ͷ��� String���� ó�� done
//ToDo : ������ Int�� ó�� not yet
//ToDo : �Ǽ��� Double�� ó�� not yet
//ToDo : ���ڴ� Char�� ó�� done
//ToDo : ������ �ڸ�Ʈ, ���ڿ� ó��


//���� ���� ���� �÷����Դϴ�.
enum class StateFlag
{
	BLOCK_COMMENT, //������ �ּ��� �д� ���Դϴ�.
	BLOCK_STRING, //������ ���ڿ��� �д� ���Դϴ�.

	LINE_COMMENT, //�ּ��� �д� ���Դϴ�.
	STRING, //���ڿ��� �д� ���Դϴ�.
	CHAR, //���ڸ� �д� ���Դϴ�.
	INTEGER, //������ �д� ���Դϴ�.
	ID, //��Ÿ �ĺ����Դϴ�.
	DEFAULT, //�⺻ �÷����Դϴ�.
};

std::queue<std::wstring> Tokenizer::tokenize(const std::wstring& line)
{
	std::queue<std::wstring> tokens; //��ūȭ�� �����

	wstring word; //�⺻ ���ִܾ� �ϳ�

	StateFlag flag = StateFlag::DEFAULT;
	if (this->in_block_comment)
		flag = StateFlag::BLOCK_COMMENT;
	if (this->in_block_string)
		flag = StateFlag::BLOCK_STRING;

	bool in_escape = false; //�̽������� ���� ������ �÷���

	for (int i = 0; i < line.length(); i++)
	{
		const wchar_t c = line[i];

		switch (flag)
		{
		case StateFlag::DEFAULT:
		{
			switch (c)
			{
				//���� ���ͷ� ó��
			case '\'': flag = StateFlag::CHAR;
				if (!word.empty())
					tokens.push(std::move(word));
				word = L"Char(L\'";
				break;

				//���ڿ� ���ͷ� ó��
			case '\"': flag = StateFlag::STRING;
				if (!word.empty())
					tokens.push(std::move(word));
				word = L"String(L\"";
				break;

				//ȭ��Ʈ�����̽� ó��
			case ' ':
			case '\n':
			case '\t':
				//������
				break;

				//�ٷιٷ� �и��ص� �Ǵ� �͵�
			case '(':
			case ')':
			case '{':
			case '}':
			case '[':
			case ']':
			case '$':
			case ';':
			case ':':
				if (!word.empty())
					tokens.push(std::move(word));
				tokens.emplace(1, c);
				break;

				//������ ó��
			case '+':
			case '-':
			case '|':
			case '&':
			case '=':
			case '<':
			case '>':
				if (!word.empty()) tokens.push(std::move(word)); //�ϴ� ����������
				if (line[i + 1] != c && line[i + 1] != '=') //2���� ������ �ƴϸ�
				{
					tokens.emplace(1, c);
				}
				else //�α��� �����ڸ�
				{
					word += c;
					word += line[i + 1];
					i++;
					tokens.push(std::move(word));
				}
				break;

			case '/':
				if (!word.empty()) tokens.push(std::move(word)); //�ϴ� ����������
				if (line[1] == '/') //�ּ���ȣ��
					flag = StateFlag::LINE_COMMENT;
				else if (line[1] == '*') // ������ �ּ� ǥ��. ���� �̱���
					;
				else if (line[1] == '=') // /=��
				{
					tokens.emplace(L"/=");
					i++;
				}
				else //�׳� /��
					tokens.emplace(1, c);
				break;

				// ?/?? or ./..
			case '?':
			case '.':
				if (!word.empty()) tokens.push(std::move(word)); //�ϴ� ����������
				if (line[i + 1] == c) //??�� ..��
				{
					tokens.emplace(2, c);
					i++;
				}
				else // ?�� .��
					tokens.emplace(1, c);
				break;

				//�ܹ��ڳ� =������ ����
			case '*':
			case '~':
			case '!':
			case '%':
			case '^':
				if (!word.empty()) tokens.push(std::move(word)); //�ϴ� ����������
				if (line[i + 1] == '=')
				{
					word += c;
					word += '=';
					tokens.push(std::move(word));
					i++;
				}
				else
					tokens.emplace(1, c);
				break;

			default:
				word += c;
			}
		} break;

		case StateFlag::LINE_COMMENT: //�ּ��� ó����
		{
			i = line.length(); //���� Ż��
		} break;

		case StateFlag::STRING: //���ڿ� ���ͷ� ó����
		{
			if (c == '\\')
				if (in_escape == false)
				{
					in_escape = true;
					word += c;
					continue;
				}

				else if (c == '\"'&&in_escape == false)
				{
					word += L"\")";
					tokens.push(std::move(word));
					flag = StateFlag::DEFAULT;
					continue;
				}

			if (in_escape == true)
				in_escape = false;

			word += c;

		} break;

		case StateFlag::CHAR: //���� ���ͷ� ó����
		{
			if (c == '\\')
				if (in_escape == false)
				{
					in_escape = true;
					word += c;
					continue;
				}

				else if (c == '\"'&&in_escape == false)
				{
					word += L"\')";
					tokens.push(std::move(word));
					flag = StateFlag::DEFAULT;
					continue;
				}

			if (in_escape == true)
				in_escape = false;

			word += c;
		} break;

		case StateFlag::BLOCK_COMMENT:
			break;

		case StateFlag::BLOCK_STRING:
			break;
	
		}

		if (!word.empty())
			tokens.push(std::move(word));

		return tokens;
	}
}
