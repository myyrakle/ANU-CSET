#include "pch.h"
#include "Tokenizer.h"

//���� �о ȭ��Ʈ�����̽� �������� �ܾ� �и�. done
//�ڸ�Ʈ�� ����. done
//���ڿ� ���ͷ��� String���� ó�� done
//ToDo : ������ Int�� ó�� not yet
//ToDo : �Ǽ��� Double�� ó�� not yet
//ToDo : ���ڴ� Char�� ó�� done
//ToDo : ������ �ڸ�Ʈ, ���ڿ� ó�� not yet 


//���� ���� ���� �÷����Դϴ�.
enum class StateFlag
{
	BLOCK_COMMENT, //������ �ּ��� �д� ���Դϴ�.
	BLOCK_STRING, //������ ���ڿ��� �д� ���Դϴ�.

	LINE_COMMENT, //�ּ��� �д� ���Դϴ�.
	STRING, //���ڿ��� �д� ���Դϴ�.
	CHAR, //���ڸ� �д� ���Դϴ�.
	NUMBER,
	INTEGER, //������ �д� ���Դϴ�.
	FLOAT,
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

	//std::wcout << line[0] << std::endl;


	for (int i = 0; line[i] != '\0'; i++)
	{
		const wchar_t c = line[i];

		//std::wcout << c << std::endl;

		switch (flag)
		{
		case StateFlag::DEFAULT: //�⺻ ������
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
			case ' ': /*case '\n':*/ case '\t':
				//������
				if (!word.empty())
				{
					tokens.push(std::move(word));
					tokens.emplace(1, c);
				}
				
				break;

				//�ٷιٷ� �и��ص� �Ǵ� �͵�
			case '(': case ')':
			case '{': case '}':
			case '[': case ']':
			case '$': case ';':
			case ',':
				if (!word.empty())
					tokens.push(std::move(word));
				tokens.emplace(1, c);
				break;

				//������ ó��
			case '+': case '-': case '|': case '&': case '=': case '<': case '>':
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

				//������ ��ȣ? �ּ�?
			case '/':
				if (!word.empty()) 
					tokens.push(std::move(word)); //�ϴ� ����������
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

				// ?/?? or ./.. or :/::
			case ':': case '?': case '.':
				if (!word.empty()) 
					tokens.push(std::move(word)); //�ϴ� ����������
				if (line[i + 1] == c) //??�� ..��
				{
					tokens.emplace(2, c);
					i++;
				}
				else // ?�� .��
					tokens.emplace(1, c);
				break;

				//�ܹ��ڳ� =������ ����
			case '*': case '~': case '!': case '%': case '^':
				if (!word.empty()) 
					tokens.push(std::move(word)); //�ϴ� ����������
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

			case L'a': case L'b': case L'c': case L'd': case L'e': case L'f': case L'g':
			case L'h': case L'i': case L'j': case L'k': case L'l': case L'm': case L'n':
			case L'o': case L'p': case L'q': case L'r': case L's': case L't': case L'u':
			case L'v': case L'w': case L'x': case L'y': case L'z':
			case L'A': case L'B': case L'C': case L'D': case L'E': case L'F': case L'G':
			case L'H': case L'I': case L'J': case L'K': case L'L': case L'M': case L'N':
			case L'O': case L'P': case L'Q': case L'R': case L'S': case L'T': case L'U':
			case L'V': case L'W': case L'X': case L'Y': case L'Z':
			case L'_':
				word += c;
				flag = StateFlag::ID;
				break;

			case '0': case '1': case '2': case '3': case '4': case '5': case'6':
			case '7': case '8': case '9':
				word += c;
				flag = StateFlag::NUMBER;
				break;

			default: //???
				//std::cout << "�̰� ����ǳ�" << std::endl;
				word += c;
				break;
			}
		} 
		break;

		case StateFlag::ID: //�ĺ��ڸ� �Ľ��մϴ�. //Ű����, ������, Ÿ�Ը� ��
		{
			switch (c)
			{
			case L'a': case L'b': case L'c': case L'd': case L'e': case L'f': case L'g':
			case L'h': case L'i': case L'j': case L'k': case L'l': case L'm': case L'n':
			case L'o': case L'p': case L'q': case L'r': case L's': case L't': case L'u':
			case L'v': case L'w': case L'x': case L'y': case L'z':
			case L'A': case L'B': case L'C': case L'D': case L'E': case L'F': case L'G':
			case L'H': case L'I': case L'J': case L'K': case L'L': case L'M': case L'N':
			case L'O': case L'P': case L'Q': case L'R': case L'S': case L'T': case L'U':
			case L'V': case L'W': case L'X': case L'Y': case L'Z':
			case L'_':
			case L'0': case L'1': case L'2': case L'3': case L'4': case L'5': case L'6':
			case L'7': case L'8': case L'9':
				word += c;
				break;

			default: //����/���ڰ� �ƴѰ� ������ �ĺ��ڷ� �и�
				tokens.push(std::move(word));
				flag = StateFlag::DEFAULT; //�ٽ� ����������
				continue;
			}
		}
			break;

		case StateFlag::NUMBER:
		{
			//TODO : �о���鼭 .�� ������ float��, ������ integer�� ����
		}
			break;

		case StateFlag::INTEGER:
		{

		}
			break;

		case StateFlag::FLOAT:
		{

		}
			break;

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

		case StateFlag::BLOCK_COMMENT: //�̱���
			break;

		case StateFlag::BLOCK_STRING: //�̱���
			break;
	
		}
	}

	if (!word.empty())
		tokens.push(std::move(word));

	tokens.emplace(1, '\n');

	return tokens;
}
