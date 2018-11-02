#pragma once
class Tokenizer
{
public:
	std::queue<std::wstring> tokenize(const std::wstring&); //�������� �۾��� �����մϴ�.
public: //�⺻ ����/����
	Tokenizer() = default;
	virtual ~Tokenizer() = default;
	Tokenizer(const Tokenizer&) = delete;
	Tokenizer& operator=(const Tokenizer&) = delete;
	Tokenizer(Tokenizer&&) = delete;
	Tokenizer& operator=(Tokenizer&&) = delete;
private:
	bool in_block_comment = false;
	bool in_block_string = false;
};