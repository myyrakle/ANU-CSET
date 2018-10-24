#pragma once
#include "basic.h"

//console ����Դϴ�,
#include <iostream>

namespace console
{
	//���ุ �մϴ�.
	void putline() { putchar('\n'); }

	//���ڿ��� ����մϴ�.
	void put(const String& val) { wprintf(L"%s", ((std::wstring)val).c_str()); }
	void putline(const String& val) { put(val); putline(); }

	//������ ����մϴ�.
	template<class IntType>
	void put(Int_Basic<IntType> val) { printf("%d", (IntType)val); }
	template<class IntType>
	void putline(Int_Basic<IntType> val) { put(val); putline(); }

	//���� ���ڸ� ����մϴ�.
	void put(Char val) { putwchar((wchar_t)val); }
	void putline(Char val) { put(val); putline(); }

	//�Ǽ��� ����մϴ�.
	template<class FloatType>
	void put(Float_Basic<FloatType> val) { printf("%f", (float)val); }
	template<class FloatType>
	void putline(Float_Basic<FloatType> val) { put(val); putline(); }

	//���� ����մϴ�.
	void put(Bool val) { val ? printf("true") : printf("false"); }
	void putline(Bool val) { put(val); putline(); }

	//���� ����
	void flush_out() { fflush(stdout); }
	void flush_in() { while (getchar() != EOF); }


	//��������� ���ڿ� �Է�
	String getline() 
	{ 
		String temp; 
		fgetws((wchar_t*) (((std::wstring)temp).data()), INT_MAX, stdin);
		return temp; 
	}

	//����'��' ���ö����� ��� �Է�
//#include <list>
//#include <cwchar>
//	std::list<String> getlines()
//	{
//		
//		std::list<String> lines;
//		String str = L"";
//		while (true)
//		{
//			while (true)
//			{
//				char c = std::getwchar();
//				if (c == '\n')
//					break;
//				else
//					str += c;
//			}
//			if (str == String(L"")) //���ุ ������ ����
//				return lines;
//			else
//			{
//				lines.push_back(str), str = L"";
//				continue;
//			}
//		}
//		
//	}

	//ȭ��Ʈ�����̽� ������ ���ڿ� �Է�
	String getword() 
	{ 
		String temp; 
		wscanf(L"%s", (wchar_t*) (((std::wstring)temp).data())); 
		return temp;
	}

	//���� ���� �Է�
	Char getc() 
	{ 
		return Char(wchar_t(getwchar()));
	}

	//���� �Է�
	void get(Int& value) { wscanf(L"%d", &(int&)value); }
	void get(Uint& value) { wscanf(L"%u", &(unsigned int&)value); }

	//���� �Է�
	void get(Char& value) { wscanf(L"%c", &(wchar_t&)value); }

	//�Ǽ� �Է�
	void get(Float& value) { wscanf(L"%f", &(float&)value); }
	void get(Double& value) { wscanf(L"%lf", &(double&)value); }
	void get(Ldouble& value) { wscanf(L"%lf", &(long double&)value); }

	//ȭ���� û���մϴ�.
	void clear() { system("cls"); }

	//�ܼ��� ����ŵ�ϴ�.
	void pause() { system("pause"); }

	//�齺���̽��� �Է��մϴ�.
	void backspace() { putchar('\b'); }
}