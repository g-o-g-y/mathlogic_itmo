#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <sstream>
#include <map>
#include <set>
#include <unordered_map>

using namespace std;

string st;
unsigned int len;
string m2;
//Преобразование выражение к типовому виду
string Expression();
string Disjunction();
string Conjunction();
string Not();
string Parentheses();
string Variable();

string Expression() {
	string out = Disjunction();
	while (len < st.length() && (st[len] == '-')) {
		/*if (p == 0 && m1 == "") {
			m = true;
			m1 = out;
		}*/
		len += 2;
		//m2 - правое разбиение строки
		m2 = Expression();
		out = "(" + out + "->" + m2 + ")";
	}
	return out;
}

string Disjunction() {
	string out = Conjunction();
	while (len < st.length() && (st[len] == '|')) {
		len++;
		out = "(" + out + "|" + Conjunction() + ")";
	}
	return out;
}

string Conjunction() {
	string out = Not();
	while (len < st.length() && (st[len] == '&')) {
		len++;
		out = "(" + out + "&" + Not() + ")";
	}
	return out;
}

string Not() {
	string out = "";
	if (len < st.length() && (st[len] == '!')) {
		len++;
		out = "!" + Not();
	}
	else {
		out = Parentheses();
	}
	return out;
}

string Parentheses() {
	string out = "";
	if (len < st.length() && (st[len] == '(')) {
		len++;
		out = Expression();
		len++;
	}
	else {
		out = Variable();
	}
	return out;
}

string Variable() {
	string out = "";
	while (((st[len] >= 'A' && st[len] <= 'Z') || (st[len] >= '0' && st[len] <= '9') || st[len] == '\'' || st[len] == '‘' || st[len] == '’') && (len < st.length())) {
		out += st[len];
		len++;
	}
	return out;
}

string New_str(string str) {
	len = 0;
	st = str;
	m2 = "";
	return Expression();
}

//Убрать внешние скобки выражения
string Parentheses_func(string::iterator first, string::iterator last) {
	if (*first == '(' && *(last - 1) == ')')
		return { ++first, --last };
	return { first, last };
}

//Поиск местонахождения знака ch для разбиения строки на 2 части
template< class InputIt >
InputIt Find(const InputIt &first, const InputIt &last, const char& ch) {
	if (first == last) return last;
	InputIt n = first, temp = first;	
	while (n != last) {
		temp = find(n, last, ch);
		if (count(first, temp, '(') == count(first, temp, ')') && temp != last) {
			return temp;
		}
		else {
			if (temp == last) n = temp;
			else n = temp + 1;
		}
	}
	return temp;
}

//Разбиение строки на 2 части по знаку ch
vector<string> For2(string &s, char ch = '-') {
	if (s.length() == 0) return { "","" };
	string str = Parentheses_func(s.begin(), s.end());
	auto i = Find(str.begin(), str.end(), ch);
	string a = { str.begin(), i };
	if (i != end(str) && ch == '-')
		str = { i + 2, str.end() };
	else
		if (i != end(str) && ch != '-')
			str = { i + 1, str.end() };
		else str = "";

	return { a,str };
}

//Структура выражения
struct Str {
	Str() {};
	Str(string str, long long n = 0) {
		this->n = n;
		this->str = str;
		//type = "";
		//use = 0;
		vector<string> v = For2(str);
		if (v[0] != "" && v[1] != "") {
			left = v[0];
			right = v[1];
		}
		else {
			left = "";
			right = "";
		}

		/*if (m2 != "") {
			left = { begin(str) + 1,end(str) - 3 - m2.size() };
			right = m2;
		}
		else {
			left = "";
			right = "";
		}*/
	}

	string str;					//Утверждение
	long long n;		//Номер утверждения (для гипотез)
	//string type;				//Аксиома-Гипотеза-MP
	//int use;					//Для обозначения используемых в доказательстве выражений

	//Запомню отдельно, будет проще обращаться
	//Str* MP1;	//Первое выражение для вывода MP
	//Str* MP2;	//Второе выражение для вывода MP

	//long long count;			//Для поиска кратчайшего доказательства, вроде не нужно
	string left, right;			//Левое и правое разбиение строки
};

//Для возможности в unordered_map использовать Str
hash<string> hs;
struct Hs
{
	size_t operator()(const Str& x) const {
		return hs(x.str);
	}
};

//Проверка на аксиому
/*(1) α → β → α
(2) (α → β) →(α → β → γ) →(α → γ)
(3) α → β → α&β
(4) α&β → α
(5) α&β → β
(6) α → α ∨ β
(7) β → α ∨ β
(8) (α → γ) →(β → γ) →(α ∨ β → γ)
(9) (α → β) →(α → ¬β) → ¬α
(10) ¬¬α → α*/
pair<string, int> Axiom(Str &s) {
	string str = s.str;
	vector<string> v1, v2;

	vector<string> v = { s.left, s.right };
	if (v[0] != "" && v[1] != "") {
		//"Ax. sch. 10"
		if (v[0] == "!!" + v[1] || "!!" + v[0] == v[1] || v[0] == "!!(" + v[1] + ")" || "!!(" + v[0] + ")" == v[1])
			return { "Ax. sch. ", 10 }; //"Ax. sch. 10";
		v1 = For2(v[0], '&');			//скобка 1
		if (v1[0] != "" && v1[1] != "") {
			//"Ax. sch. 4"
			if (v1[0] == v[1])
				return { "Ax. sch. ", 4 }; //"Ax. sch. 4";
			//"Ax. sch. 5"
			if (v1[1] == v[1])
				return { "Ax. sch. ", 5 }; //"Ax. sch. 5";
		}
		v2 = For2(v[1], '|');			//скобка 2
		if (v2[0] != "" && v2[1] != "") {
			//"Ax. sch. 6"
			if (v[0] == v2[0])
				return { "Ax. sch. ", 6 }; //"Ax. sch. 6";
			//"Ax. sch. 7"
			if (v[0] == v2[1])
				return { "Ax. sch. ", 7 }; //"Ax. sch. 7";
		}
	}

	vector<string> v3, v4, v5, v6;
	vector<string> temp = For2(v[1]);
	v = { v[0], temp[0], temp[1] };
	if (v[0] != "" && v[1] != "" && v[2] != ""){
	//"Ax. sch. 1"
		if (v[0] == v[2])
			return { "Ax. sch. ", 1 }; //"Ax. sch. 1";
		v6 = For2(v[2], '&');	//скобка 3.1
		if (v6[0] != "" && v6[1] != "") {
	//"Ax. sch. 3"
			if ((v[0] == v6[0] && v[1] == v6[1]) || (v[0] == v6[1] && v[1] == v6[0]))
				return { "Ax. sch. ", 3 }; //"Ax. sch. 3";
		}
		v1 = For2(v[0]);			//скобка 1
		v2 = For2(v[1]);			//скобка 2
		if (v1[0] != "" && v1[1] != "" && v2[0] != "" && v2[1] != "") {
			v4 = For2(v[2]);			//скобка 3
			if (v4[0] != "" && v4[1] != "") {
				v3 = For2(v2[1]);		//скобка 2/1
				if (v3[0] != "" && v3[1] != "") {
	//"Ax. sch. 2"
					if (v1[0] == v2[0] && v1[0] == v4[0] && v1[1] == v3[0] && v3[1] == v4[1])
						return { "Ax. sch. ", 2 }; //"Ax. sch. 2";
				}
				v5 = For2(v4[0], '|');	//скобка 3/1
				if (v5[0] != "" && v5[1] != "") {
	//"Ax. sch. 8"
					if ((v1[0] == v5[0] && v2[0] == v5[1] && v1[1] == v2[1] && v1[1] == v4[1]) || (v1[0] == v5[1] && v2[0] == v5[0] && v1[1] == v2[1] && v1[1] == v4[1]))
						return { "Ax. sch. ", 8 }; //"Ax. sch. 8";
				}
			}
	//"Ax. sch. 9"
			if (v1[0] == v2[0] && (v1[1] == "!" + v2[1] || "!" + v1[1] == v2[1] || v1[1] == "!(" + v2[1] + ")" || "!(" + v1[1] + ")" == v2[1]) && (v1[0] == "!" + v[2] || "!" + v1[0] == v[2] || v1[0] == "!(" + v[2] + ")" || "!(" + v1[0] + ")" == v[2]))
				return { "Ax. sch. ", 9 }; //"Ax. sch. 9";
			
		}
	}
	return { "", -1 };;
}

//Операторы для сравнения выражений
bool operator==(const Str &left_handle, const Str &right_handle) {
	if (left_handle.str == right_handle.str)
		return true;
	return false;
}

bool operator!=(const Str &left_handle, const Str &right_handle) {
	if (left_handle.str != right_handle.str)
		return true;
	return false;
}

bool operator<(const Str &left_handle, const Str &right_handle) {
	return left_handle.str < right_handle.str;
}

//Для нахождения кратчайшего доказательства, вроде бесполезно, но пока пусть будет

/*long long Existence_s(vector<Str> &proof, vector<Str> &assumption, vector<Str>::iterator item) {
	map<long long, vector<vector<Str>::iterator>> options = {};
	if ((*item).type == "") {
		for (auto i = proof.begin(); i < item; ++i) {
			if ((*i).right == (*item).str) {
				for (auto j = proof.begin(); j < item; ++j) {
					if ((*i).left == (*j).str) {
						long long a = Existence_s(proof, assumption, i);
						long long b = Existence_s(proof, assumption, j);
						if (a > 0 && b > 0) {
							options.insert({ (a + b + 1),{ i,j } });
						}
					}
				}
			}
		}
	}
	if (options.size() > 0) {
		(*item).type = "M.P.";
		(*item).count = (*options.begin()).first;
		(*item).MP1 = (*options.begin()).second[0];
		(*item).MP2 = (*options.begin()).second[1];
		return (*item).count;
	}
	if ((*item).type == "") return 0;
}*/

//Проверка выражения
/*bool Existence(vector<Str> &proof, vector<Str> &assumption, vector<Str>::iterator item) {
	//Проверка на MP
	for (auto i = proof.begin(); i < item; ++i) {	//Находим строку такую, что наше утверждение ее часть справа
		if ((*i).right == (*item).str) {
			for (auto j = proof.begin(); j < item; ++j) {//Находим недостающую левую часть
				if ((*i).left == (*j).str) {
					//Если обе части найдены, сохраняем как строки для вывода MP. Они обе уже доказаны
					(*item).type = "M.P.";
					(*item).MP1 = i;
					(*item).MP2 = j;
					return true;
				}
			}
		}
	}

	//Утверждение не доказано
	if ((*item).type == "") return false;
}*/

//Проходимся по дереву и отмечаем используемые выражения
/*void Suitability(vector<Str>::iterator item) {
	(*item).use++;
	if ((*item).type == "M.P.") {
		Suitability((*item).MP1);
		Suitability((*item).MP2);
	}
}*/

//Использовать вместо указателей на MP просто контейнер с ними, а то я уже запутался в этом всем...
vector<bool> Suitability(size_t size, vector<pair<long long, long long>> place_in) {
	vector<bool> out(size);
	out.back() = true;
	for (long long i = (long long)(out.size()) - 1; i >= 0; --i) {
		if (out[i] && place_in[i].first >= 0) {
			out[place_in[i].first] = true;
			out[place_in[i].second] = true;
		}
	}
	return out;
}

//Строит по вектору с утв. доказательство (без первой строки)
//Убрать лишние указатели -> добавить обыкновенный вектор! (2)
vector<string> Build_proof(vector<bool> proof_p, vector<pair<long long, long long>> place_in, vector<Str> all) {
	int count = 1;
	vector<string> out;
	vector<int> v(proof_p.size());
	for (size_t i = 0; i < proof_p.size(); ++i) {
		if (proof_p[i]) {
			v[i] = count;
			if (place_in[i].first == -1) {
				//[1. Hypothesis 1] (A -> B)
				string str = "[" + to_string(count) + ". Hypothesis " + to_string(place_in[i].second) + "] " + all[i].str; 
				out.push_back(str);
			}
			else {
				if (place_in[i].first == -2) {
					//[1. Ax. sch. 1] (A -> (A -> A))
					string str = "[" + to_string(count) + ". Ax. sch. " + to_string(place_in[i].second) + "] " + all[i].str;
					out.push_back(str);
				}
				else {
					//[7. M.P. 6, 4] !A
					string str = "[" + to_string(count) + ". M.P. " + to_string(v[place_in[i].first]) + ", " + to_string(v[place_in[i].second]) + "] " + all[i].str;
					out.push_back(str);
				}
			}
			count++;
		}
	}
	return out;
}

pair<string, long long> Existence(vector<Str> &assumption, Str &item) {
	//Проверка на гипотезу 
	auto temp = find(begin(assumption), end(assumption), item);
	if (temp != end(assumption)) {
		return { "Hypothesis " , (*temp).n };
	}

	//Проверка на аксиому (сразу выведет "", если не аксиома, убрать лишнее)
	return Axiom(item);

}

int main() {

	//Ввод контекста и утверждения
	string prime;
	getline(cin, prime);

	//Хорошо бы убрать итераторы, громоздко 
	//Удаляем пробелы
	auto whitespace = remove(prime.begin(), prime.end(), ' ');
	prime = { prime.begin(), whitespace };

	//Отделяем утверждение от контекста
	string::iterator a = prime.begin(), b = prime.begin();
	do {
		a = find(a, prime.end(), '|');
	} while (*(++a) != '-');
	b = a - 1;
	string last_str = New_str({ ++a,prime.end() });

	//Разбиваем контекст на гипотезы
	vector<Str> assumption = {};
	a = begin(prime);
	while (a < b) {
		auto temp = find(a, b, ',');
		assumption.push_back({ New_str({ a, temp }), assumption.size() + 1 });
		a = temp + 1;
	}

	//Основные контейнеры
	//unordered_map слабо упорядочен, нет лишней сортировки
	unordered_map<Str, vector<long long>, Hs> left_parts;
	unordered_map<Str, pair<long long, long long>, Hs> right_parts;
	unordered_map<Str, long long, Hs> ok;
	vector<pair<long long, long long>> place_in;
	vector<Str> all;
	string str = "", TEST30;
	long long number = 0;
	long long last = 0;

	//Ввод доказательства
	while (getline(cin, str)) {
		whitespace = remove(str.begin(), str.end(), ' ');
		str = New_str({ str.begin(), whitespace });
		TEST30 = str;
		Str statement = { str , number };

		//Проверяем каждое утверждение
		if (ok.count(statement) > 0)
			continue;
		else if (right_parts.count(statement)) {
			place_in.push_back(right_parts[statement]);
		}
		else {
			pair<string, long long> tmp = Existence(assumption, statement);
			if (tmp.second != -1) {
				if (tmp.first == "Hypothesis ")
					place_in.push_back({ -1, tmp.second });
				else
					place_in.push_back({ -2, tmp.second });
			}
			else {
				cout << "Proof is incorrect";
				return 0;
			}
		}

		//Запоминаем доказанное
		ok[statement] = number;
		all.push_back(statement);
		//Запомнить доказываемое выражение (если запомним раньше, то запомним его повтор)
		if (str == last_str)
			last = number;

		//Левая часть уже доказана -> добавть правую к доказанным правым частям
		if (statement.right != "") {
			Str a = Str(statement.left);
			Str b = Str(statement.right);
			if (ok.count(a) > 0) {
				right_parts[b] = { number, ok[a] };
			}
			left_parts[a].push_back(number);
		}

		//Левые части, ждущие доказательства, проверка
		if (left_parts.count(statement) > 0) {
			for (auto &j : left_parts[statement]) {
				Str a(all[j].right);
				right_parts[a] = { j, number };
			}
			left_parts.erase(statement);
		}
		number++;
	}

	//Удалить элементы после искомого
	while ((int)all.size() > last + 1) {
		all.pop_back();
		place_in.pop_back();
	}

	// Если оканчивается не утверждением
	if (TEST30 != last_str) {
		cout << "Proof is incorrect";
		return 0;
	}
	
	//Выделение используемых в доказательстве выражений для вывода
	vector<bool> proof_p = Suitability(all.size(), place_in);

	//Строми доказательство (контекст и утверждение отдельно)
	vector<string> out = Build_proof(proof_p, place_in, all);

	//Вывод доказательства
	cout << prime << endl;
	for (string &i : out) {
		cout << i << endl;
	}

}