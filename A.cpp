#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

string st;
unsigned int len;

/*template< class InputIt >
InputIt Find(InputIt first, InputIt last, const char& ch) {
	InputIt n = first;
	while (n != last) {
		InputIt i = find(n, last, ch);
		if (count(first, i, '(') == count(first, i, ')')) {
			return i;
		}
		else {
			if (i == last) n = i;
			else n = i + 1;
		}
	}
	return last;
}

template< class InputIt >
InputIt Findq(InputIt first, InputIt last, const char& ch) {
	InputIt out = last;
	InputIt n = first;
	while (n != last) {
		InputIt i = find(n, last, ch);
		if ((count(first, i, '(') == count(first, i, ')')) && i != last) {
			out = i;
		}
			if (i == last) n = i;
			else n = i + 1;
	}
	return out;
}

string Function(const string& st) {
	string apex1, apex2, sign, out;
	auto iterator = Find(st.begin(), st.end(), '-');
	if (iterator != end(st)) {
		apex1 = Function({ st.begin(), iterator });
		iterator += 2;
		apex2 = Function({ iterator, st.end() });
		sign = "->";
		out = "(" + sign + "," + apex1 + "," + apex2 + ")";
		return out;
	}
	else {
		iterator = Findq(st.begin(), st.end(), '|');
		if (iterator != end(st)) {
			apex1 = Function({ st.begin(), iterator });
			iterator++;
			apex2 = Function({ iterator, st.end() });
			sign = "|";
			out = "(" + sign + "," + apex1 + "," + apex2 + ")";
			return out;
		}
		else {
			iterator = Findq(st.begin(), st.end(), '&');
			if (iterator != end(st)) {
				apex1 = Function({ st.begin(), iterator });
				iterator++;
				apex2 = Function({ iterator, st.end() });
				sign = "&";
				out = "(" + sign + "," + apex1 + "," + apex2 + ")";
				return out;
			}
			else {
				iterator = Find(st.begin(), st.end(), '!');
				if (iterator != end(st)) {
					iterator++;
					apex2 = Function({ iterator, st.end() });
					sign = "!";
					out = "(" + sign + apex2 + ")";
					return out;
				}
				else {
					if (st[0] != '(') {
						return st;
					}
					else {
						return Function({ st.begin() + 1, st.end() - 1 });
					}
				}
			}
		}
	}
}*/

string Expression();
string Disjunction();
string Conjunction();
string Not();
string Parentheses();
string Variable();

string Expression() {
	string out = Disjunction();
	while (len < st.length() && (st[len] == '-')) {
		len+=2;
		out = "(->," + out + "," + Expression() + ")";
	}
	return out;
}

string Disjunction() {
	string out = Conjunction();
	while (len < st.length() && (st[len] == '|')) {
		len++;
		out = "(|," + out + "," + Conjunction() + ")";
	}
	return out;
}

string Conjunction() {
	string out = Not();
	while (len < st.length() && (st[len] == '&')) {
		len++;
		out = "(&," + out + "," + Not() + ")";
	}
	return out;
}

string Not() {
	string out = "";
	if (len < st.length() && (st[len] == '!')) {
		len++;
		out = "(!" + Not() + ")";
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

/*string Function_New() {
	string out = "";
	if (len < st.length()) {
		out = Expression();
	}
	return out;
}*/

/*void Space() {
	for (auto i = st.begin(); i < st.end(); ++i) {
		if (*i == ' ') {
			st.erase(i);
			--i;
		}
	}
}*/

int main()
{
	getline(cin, st);
	len = 0;
	auto a = remove(st.begin(), st.end(), ' ');
	//Space();
	st = { st.begin(), a };
	//cout << Function(st);
	cout << Expression();
}