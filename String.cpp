#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;

class String {
private:
	char* symbol;
public:
	//коструктор по умолчанию
	String() {
		symbol = new char[1];
		symbol[0] = '\0';
	}
	//конструктор по значению
	String(const char* sml) {
		size_t size = strlen(sml);
		symbol = new char[size + 1];
		strcpy(symbol, sml);
	}
	//конструктор копирования
	String(const String& othear) {
		size_t size = strlen(othear.symbol);
		symbol = new char[size + 1];
		strcpy(symbol, othear.symbol);
	}
	//move конструктор
	String(String&& othear) noexcept :symbol(othear.symbol) {
		othear.symbol = nullptr;
	}
	//деструктор 
	~String() { 
		delete[] symbol;
	}

	String& operator=(const String& othear);                         //= копированием
	String& operator=(String && othear) noexcept;					 //= с move
	String& operator+=(const String& othear);		                 //String += String
	String operator+(const String& othear);			                 //String + String
	String operator+(const char* othear);			                 //String + char[]
	friend String operator+(const char* l, const String& r);         //char[] + String
	String& operator+=(const char* othear);			                 //String += char[]

	const char* c_str() { return symbol; };                          //получение C-строки
	size_t len_str() { return strlen(symbol); };                     //длина строки
	char operator[](size_t idx) { return symbol[idx]; };             //перегрузка [] 
	friend bool readline(istream& is, String& othear, char d);       //чтение строк (для примера)
};
//= копированием
String& String::operator=(const String& othear) {
	size_t size = strlen(othear.symbol);
	delete[] this->symbol;
	this->symbol = new char[size+1];
	strcpy(symbol, othear.symbol);
	return *this;
}
//= с move
String& String::operator=(String&& othear) noexcept {
	delete[] symbol;
	symbol = othear.symbol;
	othear.symbol = nullptr;
	return *this;
}

// String+String
String String::operator+(const String& othear) {
	
	size_t size_1 = strlen(this->symbol);
	size_t size_2 = strlen(othear.symbol);
	size_t size_sum = size_1 + size_2;
	String ans;
	ans.symbol = new char[size_sum + 1];
	strcpy(ans.symbol, symbol);
	strcpy(ans.symbol + size_1, othear.symbol);
	return ans;
}
//String + char[]
String String::operator+(const char* othear) {
	if (!othear) othear = "";
	size_t size_1 = strlen(this->symbol);
	size_t size_2 = strlen(othear);
	size_t size_sum = size_1 + size_2;
	String ans;
	ans.symbol = new char[size_sum + 1];
	strcpy(ans.symbol, symbol);
	strcpy(ans.symbol + size_1, othear);
	return ans;
}
//char[] + String
String operator+(const char* l, const String& r) {
	if (!l) l = "";
	size_t size_1 = strlen(l);
	size_t size_2 = strlen(r.symbol);
	size_t size_sum = size_1 + size_2;
	String ans;
	ans.symbol = new char[size_sum + 1];
	strcpy(ans.symbol, l);
	strcpy(ans.symbol + size_1, r.symbol);
	return ans;
}

//String += String
String& String::operator+=(const String& othear) {
	size_t size_1 = strlen(this->symbol);
	size_t size_2 = strlen(othear.symbol);
	size_t size_sum = size_1 + size_2;
	char* tmp = new char[size_sum + 1];
	strcpy(tmp, symbol);
	strcpy(tmp + size_1, othear.symbol);
	delete[] this->symbol;
	this->symbol = tmp;
	return *this;
}
//String += char[]
String& String::operator+=(const char* othear) {
	size_t size_1 = strlen(this->symbol);
	size_t size_2 = strlen(othear);
	size_t size_sum = size_1 + size_2;
	char* tmp = new char[size_sum + 1];
	strcpy(tmp, symbol);
	strcpy(tmp + size_1, othear);
	delete[] this->symbol;
	this->symbol = tmp;
	return *this;
}

//функции для реализации примера:
//чтение строк
bool readline(istream& is, String& othear, char d='\n') {
	char buff[1024];
	if (!is.getline(buff, sizeof(buff), d)) {
		return false;
	}
	delete[] othear.symbol;
	othear.symbol = new char[strlen(buff) + 1];
	strcpy(othear.symbol, buff);
	return true;
}
//сравнение строк + приведение к общему регистру 
bool compare(String* a, String* b) {
	size_t i = 0;
	size_t min_len = min(a->len_str(), b->len_str());
	while (i < min_len) {
		char ca = (*a)[i];
		char cb = (*b)[i];
		if (ca >= 'A' && ca <= 'Z') ca += 32;
		if (cb >= 'A' && cb <= 'Z') cb += 32;
		if (ca != cb)
			return ca < cb;
		i++;
	}
	return a->len_str() < b->len_str();
}
//сортировка указателей вместо строк
//в данном случае эффективность спорная + доп. выделение памяти
//но мне просто нравится идея :)
vector<String*> sort_pointer(vector<String>& a) {
	vector<String*> pointers;
	pointers.reserve(a.size());
	for (auto& s : a) {
		pointers.push_back(&s);
	}
	sort(pointers.begin(), pointers.end(), [](auto& a, auto& b)
		{
			return !compare(a, b);
		});
	return pointers;
}
//вывод строк в обратном порядке
void print_ans(vector<String>& a) {
	vector<String*> pointers = sort_pointer(a);
	for (auto& p : pointers) {
		cout << p->c_str() << '\n';
	}
}

int main() {
	vector<String> a;
	String t;
	while (readline(cin, t)) {
		if (t.len_str() == 0) break;
		a.push_back(move(t));
	}
	print_ans(a);
	
}