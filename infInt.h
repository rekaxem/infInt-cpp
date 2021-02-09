#include <bits/stdc++.h>
using namespace std;

class infInt;
infInt toggle();
infInt to_infInt();
bool operator< (const infInt& a, const infInt& b);
bool operator> (const infInt& a, const infInt& b);
bool operator<= (const infInt& a, const infInt& b);
bool operator>= (const infInt& a, const infInt& b);
bool operator== (const infInt& a, const infInt& b);
bool operator!= (const infInt& a, const infInt& b);
bool operator&& (const infInt& a, const infInt& b);
bool operator|| (const infInt& a, const infInt& b);
bool operator! (const infInt& a);
infInt operator+ (const infInt& a, const infInt& b);
infInt operator- (const infInt& a, const infInt& b);
infInt operator* (const infInt& a, const infInt& b);
infInt operator/ (const infInt& a, const infInt& b);
infInt operator% (const infInt& a, const infInt& b);

class infInt {
  public:
	vector<long long int> digit;
	bool sign;
	int len;
	infInt() {};
	infInt(string);
	void set_value(string);
	void print_value();
	infInt& operator+= (const infInt&);
	infInt& operator-= (const infInt&);
	infInt& operator*= (const infInt&);
	infInt& operator/= (const infInt&);
	infInt& operator%= (const infInt&);
	infInt& operator++ ();
	infInt operator++ (int);
	infInt& operator-- ();
	infInt operator-- (int);
};

infInt::infInt(string str) {
	int str_len=str.length();
	sign=(str[0]=='-');
	for (int i=str_len-1; i>=sign; i-=9) {
		long long int result=0LL;
		for (int j=8; j>=0; j--) if (i-j>=sign) result=result*10+str[i-j]-48;
		digit.push_back(result);
	}
	len=(str_len-sign+8)/9;
} 

void infInt::set_value(string str) {
	int str_len=str.length();
	sign=(str[0]=='-');
	for (int i=str_len-1; i>=sign; i-=9) {
		long long int result=0LL;
		for (int j=8; j>=0; j--) if (i-j>=sign) result=result*10+str[i-j]-48;
		digit.push_back(result);
	}
	len=(str_len-sign+8)/9;
}

void infInt::print_value() {
	if (sign) printf("-");
	printf("%lld", digit[len-1]);
	for (int i=len-2; i>=0; i--) printf("%09lld", digit[i]);
	printf("\n");
}

const long long int digit_max=1000000000;
const infInt infInt_zero("0");
const infInt infInt_one("1");

infInt toggle(infInt value) {
	value.sign^=1;
	return value;
}

infInt to_infInt(string str) {
	infInt result;
	int str_len=str.length();
	result.sign=(str[0]=='-');
	for (int i=str_len-1; i>=result.sign; i-=9) {
		long long int temp=0LL;
		for (int j=8; j>=0; j--) if (i-j>=result.sign) temp=temp*10+str[i-j]-48;
		result.digit.push_back(temp);
	}
	result.len=(str_len-result.sign+8)/9;
	return result;
}

bool operator< (const infInt& a, const infInt& b) {
	if (a.sign^b.sign) return a.sign;
	if (a.sign) {
		if (a.len!=b.len) return a.len>b.len;
		for (int i=a.len-1; i>=0; i--) if (a.digit[i]!=b.digit[i]) return a.digit[i]>b.digit[i];
		return false;
	} else {
		if (a.len!=b.len) return a.len<b.len;
		for (int i=a.len-1; i>=0; i--) if (a.digit[i]!=b.digit[i]) return a.digit[i]<b.digit[i];
		return false;
	}
}

bool operator> (const infInt& a, const infInt& b) {
	if (a.sign^b.sign) return b.sign;
	if (a.sign) {
		if (a.len!=b.len) return a.len<b.len;
		for (int i=a.len-1; i>=0; i--) if (a.digit[i]!=b.digit[i]) return a.digit[i]<b.digit[i];
		return false;
	} else {
		if (a.len!=b.len) return a.len>b.len;
		for (int i=a.len-1; i>=0; i--) if (a.digit[i]!=b.digit[i]) return a.digit[i]>b.digit[i];
		return false;
	}
}

bool operator!= (const infInt& a, const infInt& b) {
	return (a<b || a>b);
}

bool operator== (const infInt& a, const infInt& b) {
	return !(a!=b);
}

bool operator<= (const infInt& a, const infInt& b) {
	return (a<b || a==b);
}

bool operator>= (const infInt& a, const infInt& b) {
	return (a>b || a==b);
}

bool operator&& (const infInt& a, const infInt& b) {
	return (a!=infInt_zero && b!=infInt_zero);
}

bool operator|| (const infInt& a, const infInt& b) {
	return (a!=infInt_zero || b!=infInt_zero);
}

bool operator! (const infInt& a) {
	return a==infInt_zero;
}

infInt operator+ (const infInt& a, const infInt& b) {
	infInt result;
	if (a.sign && b.sign) return toggle(toggle(a)+toggle(b));
	else if (a.sign) return b-toggle(a);
	else if (b.sign) return a-toggle(b);
	result.sign=0;
	int max_len=max(a.len, b.len);
	bool carry=0;
	for (int i=0; i<max_len; i++) {
		result.digit.push_back((i<a.len?a.digit[i]:0LL)+(i<b.len?b.digit[i]:0LL)+carry);
		if (carry=(result.digit[i]>=digit_max)) result.digit[i]-=digit_max;
	}
	result.len=max_len;
	if (carry) result.digit.push_back(1LL), result.len++;
	return result;
}

infInt operator- (const infInt& a, const infInt& b) {
	infInt result;
	if (a.sign && b.sign) return toggle(b)-toggle(a);
	else if (a.sign) return toggle(toggle(a)+b);
	else if (b.sign) return a+toggle(b);
	if (a<b) return toggle(b-a);
	result.sign=0;
	int max_len=max(a.len, b.len);
	bool carry=0;
	for (int i=0; i<max_len; i++) {
		result.digit.push_back((i<a.len?a.digit[i]:0LL)-(i<b.len?b.digit[i]:0LL)-carry);
		if (carry=(result.digit[i]<0)) result.digit[i]+=digit_max;
	}
	result.len=max_len;
	for (int i=max_len-1; i>0; i--) {
		if (result.digit[i]==0LL) result.digit.erase(result.digit.begin()+i), result.len--;
		else break;
	}
	return result;
}

infInt operator* (const infInt& a, const infInt& b) {
	infInt result;
	result.len=1;
	result.sign=0;
	result.digit.push_back(0);
	if (a.len==1 && a.digit[0]==0 || b.len==1 && b.digit[0]==0) return result;
	long long int carry=0;
	for (int i=0; i<a.len; i++) {
		for (int j=0; j<b.len; j++) {
			if (i+j+1>=result.len) {
				result.digit.push_back(0LL);
				result.len++;
			}
			result.digit[i+j]+=a.digit[i]*b.digit[j];
			carry=result.digit[i+j]/digit_max;
			result.digit[i+j]%=digit_max, result.digit[i+j+1]+=carry;
		}
	}
	for (int i=result.len-1; i>0; i--) {
		if (result.digit[i]==0LL) result.digit.erase(result.digit.begin()+i), result.len--;
		else break;
	}
	if (a.sign^b.sign) result.sign=1;
	return result;
}

infInt operator/ (const infInt& a, const infInt& b) {
	if (a<b) return infInt_zero;
	if (a.sign && b.sign) return toggle(a)/toggle(b);
	else if (a.sign) return toggle(toggle(a)/b);
	else if (b.sign) return toggle(a/toggle(b));
	infInt left=infInt_one, right;
	right.sign=right.len=0;
	for (int i=0; i<a.len-b.len; i++) {
		right.digit.push_back(0);
		right.len++;
	}
	right.digit.push_back(digit_max);
	right.len++;
	while (left<right) {
		infInt mid=left+right;
		bool carry=0;
		for (int i=mid.len-1; i>0; i--) {
			if (mid.digit[i]&1) mid.digit[i-1]+=digit_max;
			mid.digit[i]/=2;
		}
		mid.digit[0]/=2;
		if (mid.digit[mid.len-1]==0LL) mid.digit.erase(mid.digit.end()-1), mid.len--;
		if (mid*b<=a) {
			left=mid+infInt_one;
		} else {
			right=mid;
		}
	}
	return right-infInt_one;
}

infInt operator% (const infInt& a, const infInt& b) {
	if (a.sign && b.sign) return toggle(toggle(a)%toggle(b));
	else if (a.sign) return toggle(toggle(a)%b);
	else if (b.sign) return a%toggle(b);
	return a-a/b*b;
}

infInt& infInt::operator+= (const infInt& rhs) {
	return *this=(*this)+rhs;
}

infInt& infInt::operator-= (const infInt& rhs) {
	return *this=(*this)-rhs;
}

infInt& infInt::operator*= (const infInt& rhs) {
	return *this=(*this)*rhs;
}

infInt& infInt::operator/= (const infInt& rhs) {
	return *this=(*this)/rhs;
}

infInt& infInt::operator%= (const infInt& rhs) {
	return *this=(*this)%rhs;
}

infInt& infInt::operator++ () {
	return *this+=infInt_one;
}

infInt infInt::operator++ (int) {
	infInt temp=(*this);
	++*this;
	return temp;
}

infInt& infInt::operator-- () {
	return *this-=infInt_one;
}

infInt infInt::operator-- (int) {
	infInt temp=(*this);
	--*this;
	return temp;
}

void main() {
	/*
	
	Hello! This is the alpha version of the infInt-cpp project.
	We currently support the following operators:
		!=, ==, <, >, <=, >=, &&, ||, !, +, -, *, /, %, +=, -=, *=, /=, %=, ++, --
	
	To create an infInt, do
		infInt [variable_name];
	To convert string into infInt, do
		[infInt].set_value( [string] );
	To process multiple infInts, do
		[infInt_A]  <operator>  [infInt_B];
	To assign infInt_A to infInt_B, do
		[infInt_B] = [infInt_A];
	To print your infInt, do
		[infInt].print_value();
	
	We now support negative infInt as well. By putting a '-' character before the integer, your infInt will become negative.
	
	*/
	
	// Here is an example of finding the quotient and remainder of a/b
	
	string str_a, str_b;
	cin>>str_a>>str_b;
	
	infInt a(str_a), b(str_b), answer;
	
	answer = a / b;
	answer.print_value();
	
	answer = a % b;
	answer.print_value();
	
}
