#include <iostream>
#include <time.h>
#include <cstdlib>
#include <fstream>
using namespace std;
#define Max_bit 300
class bigInt
{
public:
	bigInt() {
		len = 0; sign = 1;
		for (int i = 0; i<Max_bit; i++)
			a[i] = 0;
	}

	bigInt(short l, short c[Max_bit], short s) {
		len = l; sign = s;
		for (int i = len - 1; i >= 0; i--)
			a[i] = c[len - 1 - i];
		for (int i = len; i<Max_bit; i++)
			a[i] = 0;
	}
	bigInt(char c[Max_bit])
	{
		sign = 1; len = 0; int t = 0;
		while (int(c[t])>0)t++;
		len = t;
		for (int i = len - 1; i >= 0; i--)
			a[i] = int(c[len - i - 1]) - 48;
		for (int i = len; i<Max_bit; i++)
			a[i] = 0;
	}

	bigInt(int p) {
		len = 0;
		if (p<0) { p = -p; sign = -1; }
		else sign = 1;
		while (p>0)
		{
			a[len] = p % 10;
			p = p / 10;
			len += 1;
		}
		for (int i = len; i<Max_bit; i++)
			a[i] = 0;
	}

	void random(int n) {
		bigInt t, n1(1);
		do {
			t = 2;
			*this = n1;
			for (int i = 1; i<n - 1; i++) {
				int p = rand() % 10;
				p = p / 5;
				if (p == 1)
					*this = *this + t;
				t = t * 2;
			}
			*this = *this + t;
		} while (!((*this).is_prime()));
	}

	void random1(int n) {
		bigInt t;
		t = 1;
		*this = 0;
		for (int i = 0; i<n; i++) {
			int p = rand() % 10;
			p = p / 5;
			*this = *this + bigInt(p)*t;
			t = t * 2;
		}
		//*this=*this+t;
	}


	bigInt operator +(const bigInt &p) {
		bigInt n; n.sign = 1;
		bigInt a = *this, b = p;
		if (a.sign == -1 && b.sign == -1)
			n.sign = -1;
		if (a.sign == -1 && b.sign == 1) {
			a.sign = 1;
			n = b - a;
			return n;
		}
		if (a.sign == 1 && b.sign == -1) {
			b.sign = 1;
			n = a - b;
			return n;
		}
		short t = 0;
		if (len>p.len) n.len = len; else n.len = p.len;
		for (int i = 0; i<n.len; i++)
		{
			n.a[i] = a.a[i] + b.a[i] + t;
			t = 0;
			if (n.a[i]>9) {
				t = n.a[i] / 10;
				n.a[i] %= 10;
			}
		}
		if (t>0) {              //!!!!!
			n.a[n.len] = t;
			n.len++;
		}

		return n;
	}

	bigInt operator -() {
		bigInt n = *this;
		n.sign = -n.sign;
		return n;
	}

	bigInt operator -(const bigInt &p) {
		if (sign == -1 && p.sign == 1)
		{
			bigInt a = -*this;
			bigInt b = a + p;
			return -b;
		}
		if (sign == 1 && p.sign == -1)
		{
			bigInt a = p;
			a.sign = -a.sign;
			bigInt b = *this + a;
			return b;
		}
		bigInt a = max(*this, p);
		bigInt b = min(*this, p);
		bigInt n;
		short t = 0;
		for (int i = 0; i<a.len; i++) {
			n.a[i] = a.a[i] - b.a[i] - t;
			t = 0;
			if (n.a[i]<0) {
				while (n.a[i]<0)
				{
					t++;
					n.a[i] += 10;
				}
			}
		}
		n.sign = a.sign;
		n.len = a.len;
		while (n.a[n.len - 1] == 0) n.len--;
		if (a == p && !(a == b))
			n.sign = -n.sign;
		return n;
	}

	bigInt operator *(const bigInt &p) {
		short t = 0;
		bigInt n;
		int i = 0, j = 0;
		while (j<p.len || t != 0)
		{
			i = 0;
			while (i<len || t != 0)
			{
				n.a[i + j] = n.a[i + j] + a[i] * p.a[j] + t;
				t = 0; n.len = i + j + 1;
				if (n.a[i + j]>9)
				{
					t = n.a[i + j] / 10;
					n.a[i + j] %= 10;
				}
				i++;
			}
			j++;
		}
		n.sign = sign*p.sign;
		return n;
	}

	bigInt operator /(const bigInt &p) {
		if (bigInt(1) == p) return *this;
		bigInt t, n, c = *this;
		t.len = len;
		t.a[len - 1] = 1;
		//char c;
		while (c >= p)
		{
			bigInt tmp = t*p;
			while (tmp>c && tmp.len>0) {
				t.a[t.len - 1] = 0;
				t.len--;
				t.a[t.len - 1] = 1;
				tmp = t*p;
				tmp.a[tmp.len] = 0;
			}
			if (t.len>n.len) n.len = t.len;
			while (c >= tmp) {
				c = c - tmp;
				n.a[t.len - 1]++;
			}
		}
		return n;
	}

	bigInt operator %(const bigInt &p) {
		if (bigInt(1) == p) return bigInt(0);
		bigInt t = *this / p;
		bigInt n = t*p;
		n = *this - n;
		return n;
	}

	bigInt operator =(const bigInt &p) {
		len = p.len;
		sign = p.sign;
		int i;
		for (i = 0; i<len; i++)
			a[i] = p.a[i];
		for (i; i<Max_bit; i++)
			a[i] = 0;
		return p;
	}

	bool operator ==(const bigInt &p) {
		if (sign == p.sign)
			if (len == p.len)
			{
				for (int i = 0; i<len; i++)
					if (a[i] != p.a[i])
						return false;
				return true;
			}
		return false;
	}

	bigInt operator &(bigInt &p) {             //
		bigInt t1 = *this, t2 = p;
		bigInt n0(0);
		short b1[512], b2[512];
		short len1 = 0, len2 = 0;
		bigInt b(2);
		bigInt c(1);
		while (t1>n0)
		{
			b1[len1++] = extract(t1%b);
			t1 = t1 / b;
		}
		while (t2>n0)
		{
			b2[len2++] = extract(t2%b);
			t2 = t2 / b;
		}
		int m;
		if (len1>len2)m = len2;
		else m = len1;
		for (int i = m; i<512; i++)  b1[i] = b2[i] = 0;
		t1 = c; t2 = n0;
		for (int i = 0; i<m; i++)
		{
			if (!(b1[i] * b2[i] == 0))
				t2 = t2 + t1;
			t1 = t1*b;
		}
		return t2;
	}

	bool operator >(const bigInt &p) {
		if (len>p.len) return true;
		else if (len<p.len) return false;
		for (int i = len - 1; i >= 0; i--)
			if (a[i]<p.a[i]) return false;
			else if (a[i]>p.a[i]) return true;
			return false;
	}

	bool operator >=(const bigInt &p) {
		if (len>p.len) return true;
		else if (len<p.len) return false;
		for (int i = len - 1; i >= 0; i--)
			if (a[i]<p.a[i]) return false;
			else if (a[i]>p.a[i]) return true;
			return true;
	}

	bool operator <(const bigInt &p) {
		if (len>p.len) return false;
		else if (len<p.len) return true;
		for (int i = len - 1; i >= 0; i--)
			if (a[i]>p.a[i]) return false;
			else if (a[i]<p.a[i]) return true;
			return false;
	}

	bool operator <=(const bigInt &p) {
		if (len>p.len) return false;
		else if (len<p.len) return true;
		for (int i = len - 1; i >= 0; i--)
			if (a[i]>p.a[i]) return false;
			else if (a[i]<p.a[i]) return true;
			return true;
	}

	friend bigInt max(const bigInt a, const bigInt b) {
		if (a.len>b.len) return a;
		else if (b.len>a.len) return b;
		for (int i = a.len - 1; i >= 0; i--)
		{
			if (a.a[i]>b.a[i]) return a;
			else if (b.a[i]>a.a[i]) return b;
		}
		return a;
	}

	friend bigInt min(const bigInt a, const bigInt b) {
		if (a.len>b.len) return b;
		else if (b.len>a.len) return a;
		for (int i = a.len - 1; i >= 0; i--)
		{
			if (a.a[i]>b.a[i]) return b;
			else if (b.a[i]>a.a[i]) return a;
		}
		return a;
	}

	friend bigInt exp(bigInt a, bigInt b) {
		bigInt t, n(1), n2(2), n1(1); t = a;
		while (b>0) {
			//			bigInt tmp=n1&b;
			bigInt tmp = extract(b%n2);
			if (n1 == tmp)
				n = n*t;
			t = t*t;
			b = b / n2;
		}
		return n;
	}

	friend bigInt expmod(bigInt a, bigInt b, bigInt p) {
		bigInt t, n(1), n1(1), n2(2); t = a%p;
		while (b>0) {
			//			bigInt tmp=n1&b;
			bigInt tmp = extract(b%n2);
			if (n1 == tmp) {
				n = n*t;
				n = n%p;
			}
			t = t*t;
			t = t%p;
			b = b / n2;
		}
		return n;
	}

	friend int extract(const bigInt &p) {
		if (p.len == 0) return 0;
		if (p.len == 1)
			return p.a[0];
		else return -1;
	}

	bool is_prime() {

		bigInt a(2);  //{2 3 5 7 11 13 17 19 23 29}
		if (*this == a) return true;
		bigInt t(1), p = *this;
		bigInt s = p - t;
		bigInt o = expmod(a, s, p);
		if (!(o == t)) return false;

		a = bigInt(3);
		if (*this == a) return true;
		o = expmod(a, s, p);
		if (!(o == t)) return false;

		a = bigInt(5);
		if (*this == a) return true;
		o = expmod(a, s, p);
		if (!(o == t)) return false;

		/*		a=bigInt(7);
		if (*this==7) return true;
		o=expmod(a,s,p);
		if (!(o==t)) return false;

		a=bigInt(11);
		if (*this==11) return true;
		o=expmod(a,s,p);
		if (!(o==t)) return false;*/

		return true;
	}

	void prt()
	{
		if (len == 0) { cout << 0 << endl; return; }
		if (sign == -1) cout << '-';
		for (int i = len - 1; i >= 0; i--)
			cout << a[i];
		//	cout<<endl;   
	}

	void prttxt_tokey(ofstream &file_out) {
		//ofstream file_out("RSA_key.txt", ios::out | ios::app);
		if (file_out.fail())
		{
			cout << "错误" << endl;
			exit(1);
		}
		for (int i = len - 1; i >= 0; i--)
			file_out.put(char(a[i] + 48));
		file_out.put('\n');
	}

	void prttxt_toencrypt(ofstream &file_out) {
		//ofstream file_out("RSA_encrypt.txt", ios::out | ios::app);
		if (file_out.fail())
		{
			cout << "错误" << endl;
			exit(1);
		}
		for (int i = len - 1; i >= 0; i--)
			file_out.put(char(a[i] + 48));
	}

	friend void encrypt(char m[]);
	friend void decrypt(char buffer0[]);

private:
	short len, sign;
	short a[Max_bit];
};

bigInt ExtGCD(bigInt a, bigInt b, bigInt &x, bigInt &y)
{
	if (b == 0)
	{
		x = bigInt(1);
		y = bigInt(0);
		return a;
	}
	bigInt ans = ExtGCD(b, a%b, x, y);
	bigInt tmp = x;
	x = y; y = tmp - a / b*y;
	return ans;
}

int get_bit(bigInt p) {
	int t = 0, n2(2);
	while (p>0) {
		p = p / n2;
		t++;
	}
	return t;
}

bigInt encrypt(char i, bigInt e, bigInt n)
{
	int p = int(i);
	bigInt m(p);
	bigInt c = expmod(m, e, n);
	return c;
}

void encrypt(char m[])                        //link_list
{
	char k[Max_bit]; char l[Max_bit];
	int i = 0;
	int d[10000];
	int u = 0;
	//	bigInt q;
	cin >> k;                                //e 
	cin >> l;                                //n
	bigInt e(k);
	bigInt n(l);                         //d
	while (int(m[i])>0)
		i++;
	ofstream file_out("RSA_encrypt.txt", ios::out);
	for (int t = 0; t<i; t++) {
		bigInt p = encrypt(m[t], e, n);
		int q = 0, tmp = p.len;
		while (tmp>0) {
			q++;
			tmp /= 10;
		}
		tmp = p.len;
		file_out.put(char(q + 48));
		while (tmp>0) {
			file_out.put(char(tmp % 10 + 48));
			tmp /= 10;
		}



		if (file_out.fail())
		{
			cout << "错误" << endl;
			exit(1);
		}
		for (int i = p.len - 1; i >= 0; i--)
			file_out.put(char(p.a[i] + 48));
		/*cout<<q;
		cout<<p.len;
		p.prt();*/

		d[u++] = q;
		q = p.len;
		while (q>0) {
			d[u++] = q % 10;
			q /= 10;
		}
		for (int i = 0; i<p.len; i++)
			d[u + i] = p.a[i];
		u += p.len;
	}
	cout << endl;
	file_out.close();
}




void decrypt(char buffer0[]) {
	ofstream file_out("RSA_final.txt", ios::out);
	int i = 0; bigInt n10(10); char k[Max_bit], l[Max_bit]; int t = 0;
	cin >> k;
	bigInt d(k);
	cin >> l;
	bigInt n(l);
	while (buffer0[t]>0)t++;
	while (i<t) {
		int q = int(buffer0[i]) - 48;
		i++;
		int len = 0;
		for (int j = q - 1; j >= 0; j--)
			len = len * 10 + int(buffer0[i + j]) - 48;
		i += q;
		bigInt p;
		for (int j = 0; j<len; j++)
			p.a[len - j - 1] = int(buffer0[i + j]) - 48;
		p.len = len;
		i += len;
		bigInt ans = expmod(p, d, n);
		int tmp = 0;
		for (int j = ans.len - 1; j >= 0; j--)
			tmp = tmp * 10 + ans.a[j];
		cout << char(tmp);
		file_out.put(char(tmp));
	}cout << endl;
	file_out.close();
}

void readtxt(char buffer[]) {
	ifstream in("RSA.txt");
	if (!in.is_open())
	{
		cout << "Error opening file"; exit(1);
	}
	while (!in.eof())
	{
		in.getline(buffer, 10000);
		cout << buffer << endl;
	}
}

void readtxt_encrypt(char buffer[]) {
	ifstream in("RSA_encrypt.txt");
	if (!in.is_open())
	{
		cout << "Error opening file"; exit(1);
	}
	while (!in.eof())
	{
		in.getline(buffer, 10000);
		cout << buffer << endl;
	}
}

int main()
{
	srand(time(NULL));
	cout << "生成p.q.n.e.d,请输入位数：" << endl;
	int bits;
	ofstream key_out("RSA_key.txt", ios::out);
	//ofstream encrpt_out()
	cin >> bits;
	int l = 0, i = 0; char buffer[10000]; char buffer0[10000]; int dn = 0;
	while (1) {
		l = rand() % bits;
		if (l >= bits * 2 / 5 && l <= bits * 3 / 5) {
			bigInt p, q;
			p.random(l);
			q.random(bits - l);
			if (get_bit(p*q) == bits) {
				cout << "p的值为：";
				p.prt();
				p.prttxt_tokey(key_out);
				cout << endl;
				cout << "q的值为：";
				q.prt();
				q.prttxt_tokey(key_out);
				cout << endl;
				bigInt n = (p - 1)*(q - 1);  //
				bigInt e;
				do {
					l = rand() % bits;
				} while (l <= bits * 2 / 5 || l >= bits * 3 / 5 + 1);
				e.random(l);
				bigInt x, y;
				bigInt d = ExtGCD(e, n, x, y);
				x = x + n;
				if (x>n) x = x - n;
				n = p*q;
				cout << "n的值为：";
				n.prt();
				n.prttxt_tokey(key_out);
				cout << endl;
				cout << "e的值为：";
				e.prt();
				e.prttxt_tokey(key_out);
				cout << endl;
				cout << "d的值为：";
				x.prt();
				x.prttxt_tokey(key_out);
				cout << endl;
				break;
			}
		}
	}
	cout << endl;
	key_out.close();

	cout << "输入明文：" << endl;
	readtxt(buffer);

	cout << "进行加密,请输入e、n：" << endl;
	encrypt(buffer);
	cout << endl;

	cout << "读取密文：" << endl;
	readtxt_encrypt(buffer0);

	cout << "进行解密，请输入d,n: " << endl;
	decrypt(buffer0);


	/*int p1[Max_bit]={3,2,7,4,1};
	int p2[Max_bit]={7,9,8};
	bigInt a(5,p1,1),b(3,p2,1);
	a.prt();b.prt();
	bigInt c;
	c=a%b;
	c.prt();*/

	/*char x[Max_bit];char y[Max_bit];
	cin>>x;
	cin>>y;
	bigInt a(x),b(y);
	//	bigInt x,y;
	bigInt c=exp(a,b);
	c.prt();//x.prt();
	return 0;*/

	/*	bigInt a(58231);
	if (a.is_prime()) cout<<"true"<<endl;
	else cout<<"false"<<endl;*/

	/*    bigInt a(512);
	int c=get_bit(a);
	cout<<c<<endl;*/



	/*  bigInt p;int i=0;
	while (i<100){
	p.random(5);
	p.prt();
	++;
	}*/

	/*bigInt p,q,r;
	p.random1(128);q.random1(128);r.random1(128) ;
	cout<<"x:" ;
	p.prt();cout<<endl;
	cout<<"e:";
	q.prt();cout<<endl;
	cout<<"n:";
	r.prt();cout<<endl;
	cout<<"结果：";
	bigInt t=expmod(p,q,r) ;
	t.prt() ; */

	/*    int t=0;int y=0;
	while(t!=q.len){
	int s=0;
	for(int x=0;x<q.a[t];x++){
	s+=q.a[t+x]*pow(10,x);
	}
	u[y]=char(s);y++;
	t=t+q.a[t]+1;
	}
	cout<<u<<endl;*/


	/*
	2791
	199
	555409
	3779
	45901*/
	/*
	bigInt p(2791),q(199);
	bigInt n=p*q;
	bigInt En=(p-1)*(q-1);
	bigInt e(3779);
	bigInt d,y;
	bigInt tmp=ExtGCD(e,En,d,y);
	En.prt();cout<<endl;
	d.prt();cout<<endl;
	tmp=En+d;
	tmp.prt();cout<<endl;
	return 0; */
	system("PAUSE");
	return 0;
}

