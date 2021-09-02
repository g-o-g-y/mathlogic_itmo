#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

string st;
unsigned int len;

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

int main()
{
	getline(cin, st);
	len = 0;
	auto a = remove(st.begin(), st.end(), ' ');
	st = { st.begin(), a };
	cout << Expression();
}