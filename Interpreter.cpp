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
		wifstream fin(filename);
		
		while (fin)
			sources.back() += fin.get();

		fin.close();
	}

	/*
	----------------------��ȯ�۾�---------------------
	*/
	//cpp �ҽ����Ϸ� ��ȯ
	wofstream fout("temp.cpp");
	for (auto& source : sources)
		fout << source;
	fout.close();
}
