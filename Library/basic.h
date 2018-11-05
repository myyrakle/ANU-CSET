#pragma once

#pragma once

#include <utility>
#include <string>
#include <stdint.h>
#include <vector>


class Object; //�ֻ��� Ŭ����
class Char; //���� Ÿ��
class String; //���ڿ� Ÿ��
class Bool; //�� Ÿ��
class Byte; //����Ʈ Ÿ��

			//�Ǽ� Ÿ��
template<class FloatType>
class Float_Basic;
using Float = Float_Basic<float>;
using Double = Float_Basic<double>;
using Ldouble = Float_Basic<long double>;

//���� Ÿ��
template<class IntType>
class Int_Basic;
using Int = Int_Basic<int>;
using Uint = Int_Basic<unsigned int>;
using Int8 = Int_Basic<int8_t>;
using Int16 = Int_Basic<int16_t>;
using Int32 = Int_Basic<int32_t>;
using Int64 = Int_Basic<int64_t>;
using Uint8 = Int_Basic<uint8_t>;
using Uint16 = Int_Basic<uint16_t>;
using Uint32 = Int_Basic<uint32_t>;
using Uint64 = Int_Basic<uint64_t>;

class Range; //���� ǥ�� �����̳�
class Nullable; //�ɼų�

template <class T>
class Box; //������ ��� ����Ʈ������
template <class T>
class RcBox; //���۷��� ī���� ����Ʈ������

//class FuncType;


//�ֻ��� Ŭ�����Դϴ�.
class Object
{
public:
	Object() = default;
	virtual ~Object() = default;
public:
	Object(const Object&) = default;
	Object(Object&&) = default;
	Object& operator=(const Object&) = default;
	Object& operator=(Object&&) = default;
public:
	virtual Object&& move()
	{
		return std::move(*this);
	}
	virtual Uint get_size() const;
	virtual String get_typename() const;
	virtual String to_string() const;
};


//���� Ÿ���Դϴ�.
#include <cctype>
class Char : public Object
{
private:
	wchar_t value = 0;
public:
	bool is_alpha()  //���ĺ����� Ȯ���մϴ�.
	{
		return std::isalpha(value);
	}
	bool is_lower() const //���ĺ� �ҹ������� Ȯ���մϴ�.
	{
		return std::islower(value);
	}
	bool is_upper() const //���ĺ� �빮������ Ȯ���մϴ�.
	{
		return std::isupper(value);
	}
	bool is_digit() const //���� �������� Ȯ���մϴ�.
	{
		return std::isdigit(value);
	}
	bool is_whitespace() const //ȭ��Ʈ�����̽����� Ȯ���մϴ�.
	{
		return (value == ' ' || value == '\n' || value == '\t');
	}
	bool is_korean() const //�ѱ����� Ȯ���մϴ�.
	{
		return L'��' <= value && value <= L'�R';
	}
public:
	Char(wchar_t c) : value(c) {}
	Char& operator=(wchar_t c) { value = c; }
public:
	operator const wchar_t&() const { return value; }
	operator wchar_t&() { return value; }
public:
	Char() = default;
	virtual ~Char() = default;
	Char(const Char&) = default;
	Char(Char&&) = default;
	Char& operator=(const Char&) = default;
	Char& operator=(Char&&) = default;

public: //Object methods
	virtual Char&& move() override
	{
		return std::move(*this);
	}
	virtual Uint get_size() const override;
	virtual String get_typename() const override;
	virtual String to_string() const override;
};


//���� Ÿ���� ���ø��Դϴ�.
#include <limits>
template<class IntType>
class Int_Basic : public Object
{
private:
	IntType value = 0;
public:
	Int_Basic(IntType in) :value(in) {}
	Int_Basic& operator=(IntType in) { value = in; }
public:
	operator const IntType&() const { return value; }
	operator IntType&() { return value; }
public:
	Int_Basic() = default;
	virtual ~Int_Basic() = default;
	Int_Basic(const Int_Basic&) = default;
	Int_Basic(Int_Basic&&) = default;
	Int_Basic& operator=(const Int_Basic&) = default;
	Int_Basic& operator=(Int_Basic&&) = default;
public:
	static constexpr Int_Basic max() 
	{
		return std::numeric_limits<IntType>::max();
	}
	static constexpr Int_Basic min()
	{
		return std::numeric_limits<IntType>::min();
	}
	static constexpr bool is_signed()
	{
		return std::numeric_limits<IntType>::is_signed();
	}
public:

public: //Object methods
	virtual Int_Basic&& move() override
	{
		return std::move(*this);
	}
	virtual Uint get_size() const override
	{
		return Uint(sizeof(value));
	}
	virtual String get_typename() const override;
	virtual String to_string() const override;
};



template <class FloatType>
class Float_Basic : public Object
{
private:
	FloatType value = 0.0;
public:
	Float_Basic(FloatType f) : value(f) {}
	Float_Basic& operator=(FloatType f) { value = f; }
public:
	operator const FloatType&() const { return value; }
	operator FloatType&() { return value; }
public:
	Float_Basic() = default;
	virtual ~Float_Basic() = default;
	Float_Basic(const Float_Basic&) = default;
	Float_Basic(Float_Basic&&) = default;
	Float_Basic& operator=(const Float_Basic&) = default;
	Float_Basic& operator=(Float_Basic&&) = default;
public: //static
	static constexpr FloatType PI = FloatType(3.1415926535897932385);

public: //Object methods
	virtual Float_Basic&& move() override
	{
		return std::move(*this);
	}
	virtual Uint get_size() const override
	{
		return Uint(sizeof(value));
	}
	virtual String get_typename() const override;
	virtual String to_string() const override;
};



class Bool : public Object
{
private:
	bool value = false;
public:
	Bool(bool b) : value(b) {}
	Bool& operator=(bool b) { value = b; }
public:
	void flip() { value = !value; }
public:
	operator const bool&() const { return value; }
	operator bool&() { return value; }
public: //����Ʈ
	Bool() = default;
	virtual ~Bool() = default;
	Bool(const Bool&) = default;
	Bool(Bool&&) = default;
	Bool& operator=(const Bool&) = default;
	Bool& operator=(Bool&&) = default;

public: //Object methods
	virtual Bool&& move() override
	{
		return std::move(*this);
	}
	virtual Uint get_size() const override
	{
		return Uint(sizeof(value));
	}
	virtual String get_typename() const override;
	virtual String to_string() const override;
};

class Byte : public Object
{
private:
	union {
		uint8_t value = 0;
		struct bit : public Object
		{
			uint8_t _1 : 1;
			uint8_t _2 : 1;
			uint8_t _3 : 1;
			uint8_t _4 : 1;
			uint8_t _5 : 1;
			uint8_t _6 : 1;
			uint8_t _7 : 1;
			uint8_t _8 : 1;
		};
	};
public:
	Byte(uint8_t in) :value(in) {}
	Byte& operator=(uint8_t in) { value = in; }
public:
	//String to_bits() const {return }
public:
	operator const uint8_t&() const { return value; }
	operator uint8_t&() { return value; }
public: //����Ʈ
	Byte() = default;
	virtual ~Byte() = default;
	Byte(const Byte&) = default;
	Byte(Byte&&) = default;
	Byte& operator=(const Byte&) = default;
	Byte& operator=(Byte&&) = default;
};

#include <cwchar>
class String : public Object
{
public:
	using string_t = std::basic_string<Char>;
private:
	string_t value;
public:
	String(const wchar_t* str)
	{
		const int len = std::wcslen(str);
		value.reserve(len);
		for (int i = 0; i < len; i++)
			value += str[i];
	}
	String(const std::wstring& str) 
	{
		value.reserve(str.length());
		for (auto c : str)
			value += c;
	}
	String(const string_t& str) : value(str)
	{}
	String(string_t&& str) : value(std::move(str))
	{}
	String(Char c) : value(std::move(string_t(1, c))) 
	{}
	template<class IntType>
	String(Int_Basic<IntType> v) : value( std::move(std::to_wstring((IntType)v)) ) 
	{}
	template<class FloatType>
	String(Float_Basic<FloatType> f) : value( std::move(std::to_wstring((FloatType)f)) )
	{}
public: //�׼��� �������Դϴ�.
	Char& operator[](Uint pos)
	{ return value[(unsigned int)pos]; }
	const Char& operator[](Uint pos) const 
	{ return value[(unsigned int)pos]; }
public:
	Uint length() const //���ڿ��� ���̸� ��ȯ�մϴ�.
	{ return value.length(); }
	bool is_empty() const //����ִٸ� true
	{ return value.empty(); }
	bool not_empty() const //������� �ʴٸ� true
	{ return !value.empty(); }
public:
	bool includes(const String& str) const //���ڿ��� ���� ���θ� Ȯ���մϴ�.
	{ 
		return value.find((string_t)str) != std::wstring::npos; 
	}
	bool includes(const Char& c) const //������ ���� ���θ� Ȯ���մϴ�.
	{
		return value.find(c) != string_t::npos;
	}
public:
	void reserve(Uint size) //����� �����մϴ�. 
	{ 
		value.reserve((unsigned int)size);
	}
	void clear() //�����͸� û���մϴ�.
	{
		value.clear();
	}
public:
	Uint find(const String& str) const //���ڿ� Ž�� �� �ε��� ��ȯ
	{ 
		return value.find((string_t)str); 
	}
	Uint find(const Char& c) const //���� Ž�� �� �ε��� ��ȯ
	{ 
		return value.find(c); 
	}
	String front_string(Uint index) const //�ε����� �������� �պκ� ���ڿ� ��ȯ
	{
		return String(value.substr(0, (unsigned int)index));
	}
	String back_string(Uint index) const //�ε����� �������� �޺κ� ���ڿ� ��ȯ
	{
		return String(value.substr((unsigned int)index));
	}
	String substring(Uint begin, Uint end) const //being~end�ε��� ������ �κй��ڿ� ��ȯ
	{ 
		return String(value.substr((unsigned int)begin, (unsigned int)end)); 
	}
	std::pair<String, String> split(Uint index) const //�ε����� �������� ���ڿ��� ������ ������ ��ȯ.
	{
		return std::pair<String, String>(this->front_string(index), this->back_string(index));
	}
public:
	void replace(const String& target, const String& source)
	{

	}
	void replace(const Char& target, const Char& source)
	{

	}
	void replace(size_t begin, size_t length, const String& source)
	{

	}
public://���ͷ�����
	decltype(auto) begin() { return value.begin(); }
	decltype(auto) end() { return value.end(); }
	decltype(auto) begin() const { return value.begin(); }
	decltype(auto) end() const { return value.end(); }
	decltype(auto) rbegin() { return value.begin(); }
	decltype(auto) rend() { return value.end(); }
	decltype(auto) rbegin() const { return value.begin(); }
	decltype(auto) rend() const { return value.end(); }
public:

public: //����Ʈ �����ڵ�
	String() = default;
	~String() = default;
	String(const String& str) = default;
	String(String&& str) = default;
	String& operator=(const String& str) = default;
	String& operator=(String&& str) = default;
public: //������ ����ȯ
	operator string_t&() { return value; }
	operator const string_t&() const { return value; }
public: // 
	String & operator+=(const String& rhs)
	{
		(string_t)value += (string_t)rhs;
	}
	String & operator+=(const Char& rhs)
	{
		(string_t)value += rhs;
	}
	friend String operator+=(const String& lhs, const Char& rhs)
	{
		return ((string_t)lhs) += rhs;
	}
	friend String operator+=(const Char& lhs, const String& rhs)
	{
		return operator+(rhs, lhs); //??
	}
public: //Object methods
	String && move() override
	{
		return std::move(*this);
	}
	Uint get_size() const override
	{
		return value.length() * sizeof(wchar_t);
	}
	String get_typename() const override
	{
		return L"String";
	}
	String to_string() const override
	{
		return *this;
	}
public:
	friend String operator+(const String& lhs, const String& rhs)
	{
		String temp(lhs.length() + rhs.length());
		temp.value =  std::move(lhs.value + rhs.value);
		return temp;
	}
	friend String operator+(const String& lhs, const Char& c)
	{
		String temp(lhs.length() + 1);
		temp.value = std::move(lhs.value + c);
		return temp;
	}
	friend String operator+(const Char& c, const String& rhs)
	{
		return operator+(rhs, c); //����
	}
};


class Range : public Object
{
private:
	int head;
	int current_value = 0;
	int tail;
public:
	class iterator : public Object
	{
		using pointer = Range * ;
	private:
		pointer obj = nullptr;
	public:
		iterator(pointer p) : obj(p) { p->current_value = p->head; } //begin/rbegin ����
		iterator(nullptr_t __n) : obj(__n) {} // end/rend ����
		virtual ~iterator() = default;
		iterator() = delete;
		iterator(const iterator&) = default;
		iterator(iterator&&) = default;
		iterator& operator=(const iterator& rhs) = default;
		iterator& operator=(iterator&& rhs) = default;
	public:
		iterator & operator++()
		{
			if (obj == nullptr) return *this;

			if (obj->current_value != obj->tail)
				(obj->current_value)++;
			else
				obj = nullptr;
			return *this;
		}
		iterator operator++(int) { return this->operator++(); } //����
		int& operator*() { return obj->current_value; }
		const int& operator*() const { return obj->current_value; }
		bool operator==(const iterator& rhs)
		{
			return this->obj == rhs.obj;
		}
		bool operator!=(const iterator& rhs)
		{
			return this->obj != rhs.obj;
		}
	};
	class reverse_iterator : public Object
	{
		using pointer = Range * ;
	private:
		pointer obj = nullptr;
	public:
		reverse_iterator(pointer p) : obj(p) { p->current_value = p->tail; }
		reverse_iterator(nullptr_t __n) : obj(__n) {}
		virtual ~reverse_iterator() = default;
		reverse_iterator() = delete;
		reverse_iterator(const reverse_iterator&) = default;
		reverse_iterator(reverse_iterator&&) = default;
		reverse_iterator& operator=(const reverse_iterator&) = default;
		reverse_iterator& operator=(reverse_iterator&&) = default;
	public:
		reverse_iterator & operator++()
		{
			if (obj == nullptr) return *this;

			if (obj->current_value != obj->tail)
				(obj->current_value)++;
			else
				obj = nullptr;
			return *this;
		}
		reverse_iterator operator++(int) { return this->operator++(); } //����
		int& operator*() { return obj->current_value; }
		const int& operator*() const { return obj->current_value; }
		bool operator==(const reverse_iterator& rhs)
		{
			return this->obj == rhs.obj;
		}
		bool operator!=(const reverse_iterator& rhs)
		{
			return this->obj != rhs.obj;
		}
	};
public: //�ݺ���
	Range::iterator begin()
	{
		current_value = head;
		return iterator(this);
	}
	Range::iterator end()
	{
		return iterator(nullptr);
	}
public: //���� �ݺ���
	Range::reverse_iterator rbegin()
	{
		current_value = tail;
		return reverse_iterator(this);
	}
	Range::reverse_iterator rend()
	{
		return reverse_iterator(nullptr);
	}
public:
	Range() = delete;
	virtual ~Range() = default;
	Range(int _lhs, int _rhs) : head(_lhs), tail(_rhs) {}
};


/*.*/
inline Uint Object::get_size() const
{
	return Uint(0);
}

inline String Object::get_typename() const
{
	return String(L"Object");
}

inline String Object::to_string() const
{
	return String(L"");
}

template<class IntType>
inline String Int_Basic<IntType>::get_typename() const
{
	return String(L"String");
}

template<class IntType>
inline String Int_Basic<IntType>::to_string() const
{
	return String(std::to_wstring(value));
}


class Collection : public Object
{};

template <class T>
class IArray : public Collection
{
public:
	virtual Uint length() const = 0;
	virtual T& operator[](Uint) = 0;
	virtual const T& operator[](Uint) const = 0;
};

#pragma once

#include <stdexcept>
#include <algorithm>
#include <array>

template <size_t Length, class T>
class Array;

template <size_t Length, class T>
class Array : public IArray
{
private:
	std::array<T, Length> value;
public:
	constexpr Uint length() const
	{
		return value.size();
	}
	constexpr Bool is_empty() const
	{
		return value.empty();
	}
public: //�׼���
	T& operator[](Uint index)
	{
		return value[(size_t)index];
	}
	const T& operator[](Uint index) const
	{
		return value[(size_t)index];
	}
	T& at(Uint index)
	{
		return value.at((size_t)index);
	}
	const T& at(Uint index) const
	{
		return value.at((size_t)index);
	}
public:
	Array() = default;
	virtual ~Array() = default;
	Array(const Array&) = default;
	Array& operator=(const Array&) = default;
	Array(Array&&) = default;
	Array& operator=(Array&&) = default;
public:
	template<class T>
	Array(std::initializer_list<T> list) : value(list)
	{}
	template <class T>
	Array & operator=(std::initializer_list<T> list)
	{ value = list; }
public:
	decltype(auto) begin()
	{ return value.begin(); }
	decltype(auto) end()
	{ return value.end(); }
	decltype(auto) begin() const
	{ return value.begin(); }
	decltype(auto) end() const
	{ return value.end(); }
	decltype(auto) rbegin()
	{ return value.rbegin(); }
	decltype(auto) rend()
	{ return value.rend(); }
	decltype(auto) rbegin() const
	{ return value.rbegin(); }
	decltype(auto) rend() const
	{ return value.rend(); }
};

template <class T>
class DynamicArray;

template <class T>
using dynamic_array = DynamicArray<T>;

template <class T>
class DynamicArray
{
public: /*�ݺ��� Ÿ��*/
	class iterator;
	class reverse_iterator;
	class const_iterator;
	class const_reverse_iterator;

private: /*�Ӽ��Դϴ�*/
	T * arr = nullptr;
	size_t _length = 0;

public: /*�⺻ ����/����*/
	DynamicArray() = default;
	virtual ~DynamicArray()
	{
		delete[] arr;
	}
	DynamicArray(const DynamicArray& other) : _length(other._length), arr(new T[other._length])
	{
		int i = 0;
		for (const auto& e : other)
		{
			arr[i] = e;
			i++;
		}
	}
	DynamicArray& operator=(const DynamicArray& other)
	{
		delete[] arr;

		this->_length = other._length;
		arr = new T[_length];

		int i = 0;
		for (const auto& e : other)
		{
			arr[i] = e;
			i++;
		}
	}
	DynamicArray(DynamicArray&& other) : _length(other._length), arr(other.arr)
	{
		other._length = 0;
		other.arr = nullptr;
	}
	DynamicArray& operator=(DynamicArray&& other)
	{
		delete[] arr;
		this->_length = other._length;
		this->arr = other.arr;
	}

public: /*��� ����/�����Դϴ�.*/
	DynamicArray(size_t _length) : _length(_length), arr(new T[_length])
	{}
	DynamicArray(std::initializer_list<T> init) : _length(init.size()), arr(new T[init.size()])
	{
		int i = 0;
		for (auto&& e : init)
		{
			arr[i] = std::move(e);
			i++;
		}
	}
	DynamicArray& operator=(std::initializer_list<T> init)
	{
		delete[] arr;

		_length = init.size();
		arr = new T[_length];

		int i = 0;
		for (auto&& e : init)
		{
			arr[i] = std::move(e);
			i++;
		}
	}

public: /*�׼��� �޼����Դϴ�.*/
	T & operator[](size_t index)
	{
		return arr[index];
	}
	const T& operator[](size_t index) const
	{
		return arr[index];
	}
	T & at(size_t index)
	{
		if (0 <= index && index < length)
			return arr[index];
		else
			throw std::out_of_range();
	}
	const T& at(size_t index) const
	{
		if (0 <= index && index < length)
			return arr[index];
		else
			throw std::out_of_range();
	}

public:
	T * data() //�ο� ������ ��ȯ
	{
		return arr;
	}
	const T* data() const
	{
		return arr;
	}
	size_t length() const //���� ��ȯ
	{
		return _length;
	}

public:
	void clear() //û��
	{
		delete[] arr;
		_length = 0;
	}
	void create_from_length(size_t _length) //���̷� ����
	{
		clear();
		this->_length = _length;
		arr = new T[_length];
	}
	void create_from_init(std::initializer_list<T> init) //�߰�ȣ �ʱ�ġ�� ����
	{
		delete[] arr;

		_length = init.size();
		arr = new T[_length];

		int i = 0;
		for (auto&& e : init)
		{
			arr[i] = std::move(e);
			i++;
		}
	}
	void swap(DynamicArray&& other)
	{
		DynamicArray&& temp = std::move(other);
		other = *this;
		*this = std::move(temp);
	}

public: /*Ž��/Ȯ��*/
	iterator find(const T& value) //Ž�� �� ��ġ �ݺ��� ��ȯ
	{
		for (int i = 0; i < _length; i++)
			if (arr[i] == value) return iterator(arr + i);

		return iterator(arr + _length);
	}
	const_iterator find(const T& value) const
	{
		for (int i = 0; i < _length; i++)
			if (arr[i] == value) return const_iterator(arr + i);

		return const_iterator(arr + _length);
	}
	size_t find_index(const T& value) const //Ž�� �� ��ġ �ε��� ��ȯ
	{
		for (int i = 0; i < _length; i++)
			if (arr[i] == value) return i;
		return -1;
	}
	bool empty() const //�������� ��������� Ȯ���մϴ�. ����ִٸ� true�� ��ȯ�մϴ�.
	{
		return length == 0;
	}
	bool not_empty() const //�������� ������� �������� Ȯ���մϴ�. ������� �ʴٸ� true�� ��ȯ�մϴ�.
	{
		return length != 0;
	}
	bool includes(const T& value) const //���� ���θ� Ȯ���մϴ�. �ش� ���� �����Ѵٸ� true�� ��ȯ�մϴ�.
	{
		for (int i = 0; i < _length; i++)
			if (arr[i] == value) return true;
		return false;
	}
	size_t count(const T& value) const //�ش� ���� ��ġ�Ǵ� ������ ��ȯ�մϴ�.
	{
		int c = 0;
		for (int i = 0; i < _length; i++)
			if (arr[i] == value) c++;
		return c;
	}
public: //����
	void sort() //������������ �����մϴ�.
	{
		std::sort<T*>(arr, arr + _length);
	}
	void rsort() //������������ �����մϴ�.
	{
		std::sort<T*>(arr, arr + _length, std::greater<T>());
	}
	bool is_sorted() const //�������� ���Ļ������� Ȯ���մϴ�.
	{
		return std::is_sorted<T*>(arr, arr + _length);
	}
	bool is_rsorted() const //�������� ���Ļ������� Ȯ���մϴ�.
	{
		return std::is_sorted<T*>(arr, arr + _length, std::greater<T>());
	}
public:
	void reverse() //�������ϴ�.
	{
		std::reverse<T*>(arr, arr + _length);
	}
	void fill(const T& value) //�ش� ������ ä��
	{
		for (int i = 0; i < _length; i++)
			arr[i] = value;
	}
public: //�ݺ���
	iterator begin()
	{
		return iterator(arr);
	}
	const_iterator begin() const
	{
		return const_iterator(arr);
	}
	iterator end()
	{
		return iterator(arr + _length);
	}
	const_iterator end() const
	{
		return const_iterator(arr + _length);
	}
public: //���� �ݺ���
	reverse_iterator rbegin()
	{
		return reverse_iterator(arr + _length - 1);
	}
	const_reverse_iterator rbegin() const
	{
		return const_reverse_iterator(arr + _length - 1);
	}
	reverse_iterator rend()
	{
		return reverse_iterator(arr - 1);
	}
	const_reverse_iterator rend() const
	{
		return const_reverse_iterator(arr - 1);
	}
public: //����� ����ݺ���
	const_iterator cbegin() const
	{
		return const_iterator(arr);
	}
	const_iterator cend() const
	{
		return const_iterator(arr + _length);
	}
	const_reverse_iterator crbegin() const
	{
		return const_reverse_iterator(arr + _length - 1);
	}
	const_reverse_iterator crend() const
	{
		return const_reverse_iterator(arr - 1);
	}
public:

	/*������� �ݺ��� �����Դϴ�.*/
	//���� �ݺ����Դϴ�.
	class iterator
	{
	private:
		mutable T* ptr = nullptr;
	public: //�⺻ ����/����
		iterator() = default;
		virtual ~iterator() = default;
		iterator(const iterator&) = default;
		iterator& operator=(const iterator&) = default;
		iterator(iterator&&) = default;
		iterator& operator=(iterator&&) = default;
	public:
		iterator(T* p) : ptr(p) {}
	public:
		const iterator& operator++() const
		{
			ptr++;
			return *this;
		}
		iterator operator++(int) const
		{
			ptr++;
			return *this;
		}
		const iterator& operator--() const
		{
			ptr--;
			return *this;
		}
		iterator operator--(int) const
		{
			ptr--;
			return *this;
		}
	public:
		T & operator*()
		{
			return *ptr;
		}
		const T& operator*() const
		{
			return *ptr;
		}
		T& operator[](size_t index)
		{
			return ptr[index];
		}
		const T& operator[](size_t index) const
		{
			return ptr[index];
		}
		bool operator!=(const iterator& rhs) const
		{
			return this->ptr != rhs.ptr;
		}
		bool operator==(const iterator& rhs) const
		{
			return this->ptr == rhs.ptr;
		}
	};

	//���� �ݺ����Դϴ�.
	class reverse_iterator
	{
	private:
		mutable T* ptr = nullptr;
	public: //�⺻ ����/����
		reverse_iterator() = default;
		virtual ~reverse_iterator() = default;
		reverse_iterator(const reverse_iterator&) = default;
		reverse_iterator& operator=(const reverse_iterator&) = default;
		reverse_iterator(reverse_iterator&&) = default;
		reverse_iterator& operator=(reverse_iterator&&) = default;
	public:
		reverse_iterator(T* p) : ptr(p) {}
	public:
		const reverse_iterator & operator++() const
		{
			ptr--;
			return *this;
		}
		reverse_iterator operator++(int) const
		{
			ptr--;
			return *this;
		}
		const reverse_iterator& operator--() const
		{
			ptr++;
			return *this;
		}
		reverse_iterator operator--(int) const
		{
			ptr++;
			return *this;
		}
	public: //�׼��� �������Դϴ�.
		T & operator*()
		{
			return *ptr;
		}
		const T& operator*() const
		{
			return *ptr;
		}
	public: //��, ���
		bool operator!=(const reverse_iterator& rhs) const
		{
			return this->ptr != rhs.ptr;
		}
		bool operator==(const reverse_iterator& rhs) const
		{
			return this->ptr == rhs.ptr;
		}
	};

	//��� ���� �ݺ����Դϴ�.
	class const_iterator
	{
	private:
		mutable const T* ptr = nullptr;
	public: //�⺻ ����/����
		const_iterator() = default;
		virtual ~const_iterator() = default;
		const_iterator(const const_iterator&) = default;
		const_iterator& operator=(const const_iterator&) = default;
		const_iterator(const_iterator&&) = default;
		const_iterator& operator=(const_iterator&&) = default;
	public:
		const_iterator(const T* p) : ptr(p) {}
	public: //���� ������
		const const_iterator & operator++() const
		{
			ptr++;
			return *this;
		}
		const_iterator operator++(int) const
		{
			ptr++;
			return *this;
		}
		const const_iterator& operator--() const
		{
			ptr--;
			return *this;
		}
		const_iterator operator--(int) const
		{
			ptr--;
			return *this;
		}
	public: //�׼��� ������
		const T& operator*() const
		{
			return *ptr;
		}
	public: //��/��� ������
		bool operator!=(const const_iterator& rhs) const
		{
			return this->ptr != rhs.ptr;
		}
		bool operator==(const const_iterator& rhs) const
		{
			return this->ptr == rhs.ptr;
		}
	};

	//��� ���� �ݺ����Դϴ�.
	class const_reverse_iterator
	{
	private:
		mutable const T* ptr = nullptr;
	public: //�⺻ ����/����
		const_reverse_iterator() = default;
		virtual ~const_reverse_iterator() = default;
		const_reverse_iterator(const const_reverse_iterator&) = default;
		const_reverse_iterator& operator=(const const_reverse_iterator&) = default;
		const_reverse_iterator(const_reverse_iterator&&) = default;
		const_reverse_iterator& operator=(const_reverse_iterator&&) = default;
	public:
		const_reverse_iterator(const T* p) : ptr(p) {}
	public: //���� ����
		const const_reverse_iterator & operator++() const
		{
			ptr--;
			return *this;
		}
		const_reverse_iterator operator++(int) const
		{
			ptr--;
			return *this;
		}
		const const_reverse_iterator& operator--() const
		{
			ptr++;
			return *this;
		}
		const_reverse_iterator operator--(int) const
		{
			ptr++;
			return *this;
		}
	public: //�׼��� ������
		const T& operator*() const //��� ���ٸ� ����
		{
			return *ptr;
		}
	public: //��/��� ������
		bool operator!=(const const_reverse_iterator& rhs) const
		{
			return this->ptr != rhs.ptr;
		}
		bool operator==(const const_reverse_iterator& rhs) const
		{
			return this->ptr == rhs.ptr;
		}
	};
};

#include <functional>

template <class R, class... Args>
class FuncType : public Object, public Collection
{
private:
	std::function<R(Args...)> func;	
public:
	template <class F>
	FuncType(F f) : func(f)
	{}
	template <class F>
	FuncType& operator=(F f)
	{
		func = f;
	}
public:
	FuncType() = default;
	virtual ~FuncType() = default;
	FuncType(const FuncType&) = default;
	FuncType& operator=(const FuncType&) = default;
	FuncType(FuncType&&) = default;
	FuncType& operator=(FuncType&&) = default;
public:
	R operator()(Args... args)
	{
		return func(args...);
	}
	R invoke(Args... args)
	{
		return func(args...);
	}
public:
	virtual FuncType&& move() override
	{
		return std::move(*this);
	}
	virtual Uint get_size() const;
	virtual String get_typename() const;
	virtual String to_string() const;
};

#include <memory>

template <class T>
class Box : public Object
{
private:
	T * data = nullptr;
public:
	Box() = default;
	virtual ~Box()
	{
		delete data;
	}
	Box(const Box&) = delete;
	Box& operator=(const Box&) = delete;
	Box(Box&&) = default;
	Box& operator=(Box&&) = default;
public:
	
public:
	Box && move() override
	{
		return std::move(*this);
	}
	Uint get_size() const override
	{

	}
	String get_typename() const override
	{

	}
	String to_string() const override
	{

	}
};


template <class T>
class RcBox : public Object
{
private:
	std::shared_ptr<T> ptr;
public:
	void func()
	{
		
	}
public:
	template <class Y>
	RcBox(Y* y) : ptr(y)
	{}
	//template <class Y>
	
};