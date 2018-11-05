#pragma once
//��ȯ�� C++�ڵ带 �޾Ƽ� g++�� �������� �����մϴ�.

class Builder
{
public:
	void build(); //�������� �۾��� �����մϴ�.
public:
	void add_file(const std::wstring& filename);
	void compile() const;

public:
	void set_original_filepath(const std::wstring&);
private:
	wstring _original_filepath;

private:
	vector<wstring> filenames;
	//array<wstring> headers;
	//vector<CompileOption> options

public: //�������� ����
	Builder() = default;
	~Builder() = default;
	Builder(const Builder&) = delete;
	Builder(Builder&&) = delete;
	Builder& operator=(const Builder&) = delete;
	Builder& operator=(Builder&&) = delete;
};