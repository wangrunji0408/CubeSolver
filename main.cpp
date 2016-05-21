#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <map>
using namespace std;

template <int M>
class Exchange
{
	int v[M];
public:
	Exchange ()
	{
		for(int i=0; i<M; ++i)
			v[i] = i;
	}
	int& operator [] (int pos)
	{
		return v[pos];
	}
	const int& operator [] (int pos) const
	{
		return v[pos];
	}
	friend Exchange<M> operator * (const Exchange<M> &A, const Exchange<M> &B)
	{
		Exchange<M> C;
		for(int i=0; i<M; ++i)
			C[i] = B[A[i]];
		return C;
	}
	friend Exchange<M> pow (const Exchange<M> &A, int p)
	{
		Exchange<M> C;
		while(p--)
			C *= A;
		return C;
	}
	friend bool operator == (const Exchange<M> &A, const Exchange<M> &B)
	{
		for(int i=0; i<M; ++i)
			if(A[i] != B[i])
				return false;
		return true;
	}
	void operator *= (const Exchange<M> &B)
	{
		*this = B * (*this);
	}
	int find (int value) const
	{
		int pos = 0;
		while(v[pos] != value)
			++pos;
		return pos;
	}
	void roll (int a, int b, int c, int d)
	{
		int t = v[d];
		v[d] = v[c];
		v[c] = v[b];
		v[b] = v[a];
		v[a] = t;
	}
};

typedef Exchange<54> Cube;

/*	Code:
	F0 R1 U2 B3 L4 D5
	B  R  Y  G  O  W

				18 19 20		29 28 27
				21 22 23		32 31 30
				24 25 26 U		35 34 33 B

	36 37 38	00 01 02	09 10 11
	39 40 41	03 04 05	12 13 14
	42 43 44 L	06 07 08 F	15 16 17 R

				45 46 47
				48 49 50
				51 52 53 D
*/
const char surfaceName[7] = "FRUBLD";	//word of surface
const char colorName[7] = "BRYGOW";	//word of color
const char cc[4] = " 2'";		//word of appendix
const int nextPos[54] = {24, 25, 9, 41, 4, 12, 44, 46, 47,
						26, 23, 27, 5, 13, 30, 8, 50, 53,
						29, 28, 11, 37, 22, 10, 38, 1, 2,
						20, 19, 36, 14, 31, 39, 17, 52, 51,
						18, 21, 0, 32, 40, 3, 35, 48, 45,
						6, 7, 15, 43, 49, 16, 42, 34, 33};
int posByCharacter[6][6][6]; //const
map<char, int> surfaceNum;
Cube F, R, U, B, L, D, f, y;

inline int color (int pos)
{
	return pos / 9;
}

void makePosByCharacter ()
{
	for(int a=0; a<6; ++a)
		for(int b=0; b<6; ++b)
			for(int c=0; c<6; ++c)
				posByCharacter[a][b][c] = -1;
	for(int i=0; i<54; ++i)
	{
		int a = color(i);
		int b = color(nextPos[i]);
		int c = color(nextPos[nextPos[i]]);
		posByCharacter[a][b][c] = i;
	}
}

void readBySurface (istream& in, Cube &A)
{
	map<char,char> cc;
	char c[54];
	//for(int i=0; i<54; ++i)
	//	cin >> c[i];
	for(int i=0; i<9; ++i)
		cin >> c[i];
	for(int i=0; i<9; ++i)
		cin >> c[9*3+i];
	for(int i=0; i<9; ++i)
		cin >> c[9*4+i];
	for(int i=0; i<9; ++i)
		cin >> c[9*1+i];
	for(int i=0; i<9; ++i)
		cin >> c[9*2+i];
	for(int i=0; i<9; ++i)
		cin >> c[9*5+i];

	for(int k=0; k<6; ++k)
		cc[c[4 + k * 9]] = colorName[k];
	for(int i=0; i<54; ++i)
		c[i] = cc[c[i]];
	for(int i=0; i<54; ++i)
		c[i] = strchr(colorName, c[i]) - colorName;
	for(int i=0; i<54; ++i)
		A[i] = posByCharacter[c[i]][c[nextPos[i]]][c[nextPos[nextPos[i]]]];
}

string rand_move (int n)
{
	string s;
	while(n--)
	{
		s += surfaceName[rand() % 6];
		s += cc[rand() % 3];
		s += ' ';
	}
	return s;
}

int GetK (char c)
{
	switch(c)
	{
		case '\'': case '3':	return 3;
		case '2': 				return 2;
		default:				return 1;
	}
}

void move (Cube &A, char const *s)
{
	while(*s)
	{
		char c = *s++;
		int k = GetK(*s);
		if(*s)	++s;
		while(k--)
			switch(c)
			{
				case 'F':	A *= F; break;
				case 'R':	A *= R; break;
				case 'U':	A *= U; break;
				case 'B':	A *= B; break;
				case 'L':	A *= L; break;
				case 'D':	A *= D; break;
				case 'f': 	A *= f; break;
				case 'y':	A *= y; break;
			}
		while(*s == ' ')
			++s;
	}
}

void move (Cube &A, char const *s, string &str)
{
	str += s, str += ' ';
	move(A, s);
}

void print_cube (const Cube& A)
{
	for(int i=0; i<6; ++i)
	{
		cerr << surfaceName[i] << endl;
		for(int j=0; j<3; ++j)
		{
			for(int k=0; k<3; ++k)
				cerr << A[i * 9 + j * 3 + k] / 9 << ' ';
			cerr << endl;
		}
		cerr << endl;
	}
}

void init ()
{
	F.roll(0, 2, 8, 6);
	F.roll(1, 5, 7, 3);
	F.roll(25, 12, 46, 41);
	F.roll(24, 9, 47, 44);
	F.roll(26, 15, 45, 38);

	R.roll(9, 11, 17, 15);
	R.roll(10, 14, 16, 12);
	R.roll(23, 30, 50, 5);
	R.roll(26, 27, 53, 8);
	R.roll(20, 33, 47, 2);

	U.roll(19, 23, 25, 21);
	U.roll(18, 20, 26, 24);
	U.roll(1, 37, 28, 10);
	U.roll(0, 36, 27, 9);
	U.roll(2, 38, 29, 11);

	B.roll(28, 32, 34, 30);
	B.roll(27, 29, 35, 33);
	B.roll(19, 39, 52, 14);
	B.roll(18, 42, 53, 11);
	B.roll(20, 36, 51, 17);

	L.roll(37, 41, 43, 39);
	L.roll(36, 38, 44, 42);
	L.roll(21, 3, 48, 32);
	L.roll(0, 45, 35, 18);
	L.roll(6, 51, 29, 24);

	D.roll(46, 50, 52, 48);
	D.roll(45, 47, 53, 51);
	D.roll(7, 16, 34, 43);
	D.roll(6, 15, 33, 42);
	D.roll(8, 17, 35, 44);

	y = U * pow(D, 3);
	y.roll(4, 40, 31, 13);
	y.roll(3, 39, 30, 12);
	y.roll(5, 41, 32, 14);

	f = F;
	f.roll(22, 13, 49, 40);
	f.roll(21, 10, 50, 43);
	f.roll(23, 16, 48, 37);

	makePosByCharacter();
	for(int i=0; i<6; ++i)
		surfaceNum[surfaceName[i]] = i;
}

string solve (Cube &A)
{
	Cube Target;
	string ans;

	//cross in down
	for(int k=0; k<4; ++k)
	{
		const char* w[54];
		w[23] = "R2";
		w[19] = "U";
		w[21] = "U2";
		w[25] = "U'";

		w[ 1] = "F R' F'";
		w[10] = "U";
		w[28] = "U2";
		w[37] = "U'";
		
		w[30] = "R";
		w[39] = "D B D'";
		w[ 3] = "D2 L D2";
		w[12] = "D' F D";
		
		w[ 5] = "R'";
		w[14] = "D B' D'";
		w[32] = "D2 L' D2";
		w[41] = "D' F' D";
	
		w[16] = "R";
		w[ 7] = "F' R'";
		w[43] = "L'";
		w[34] = "B R";

		w[50] = "";
		w[46] = "F2";
		w[48] = "L2";
		w[52] = "B2";

		int tar = Target[50], pos;
		while((pos = A.find(tar)) != 50)
			move(A, w[pos], ans);
		ans += "y\n", A *= y, Target *= y;
	}

	//F2L
	for(int k=0; k<4; ++k)
	{
		const char* w[54];
		w[ 9] = "";
		w[ 2] = "F' U2 F U";
		w[26] = "R U2 R' U'";
		
		w[24] = w[38] = w[ 0] = "U'";
		w[18] = w[29] = w[36] = "U2";
		w[20] = w[11] = w[27] = "U";

		w[8] = w[15] = w[47] = "R U' R'";	
		w[ 6] = w[44] = w[45] = "y' R U' R' y";
		w[42] = w[35] = w[51] = "y2 R U' R' y2";
		w[33] = w[17] = w[53] = "y R U' R' y'";

		int tar = Target[47], pos;
		while((pos = A.find(tar)) != 9)
			move(A, w[pos], ans);

		w[ 1] = "U' F' U F";
		w[19] = "R U R'";

		//top->19
		w[23] = "U' R U' R' U";
		w[25] = "U F' U2 F U'";
		w[21] = "U' R U R' U";

		//-> 1
		w[37] = "U F' U' F U'";
		w[28] = "U F' U2 F U'";
		w[10] = "R U' R' U2 F' U' F";//special

		//other->19
		w[ 5] = "U R U R' U2";
		w[12] = "U2 F' U F U";
		w[30] = "U R' U R U2";
		w[14] = "y R U R' U' y'";
		w[32] = "U' L U L'";
		w[39] = "y R' U R U' y'";
		w[41] = "U2 F U F' U";
		w[ 3] = "U' L' U L";

		while(A.find(tar) != 47)
			move(A, w[A.find(Target[5])], ans);
		ans += "y\n", A *= y, Target *= y;
	}
	
	//cross in top
	{
		char w0[] = "F R U R' U' F'";
		char w1[] = "U2 F U R U' R' F'";
		int cntY = (A[25]/9 == 2) + (A[21]/9 == 2) + (A[19]/9 == 2) + (A[23]/9 == 2);
		if(cntY == 0)
			move(A, w1, ans), move(A, w0, ans);
		else if(cntY == 2)
		{
			while(A[23]/9 != 2)
				move(A, "U", ans);
			if(A[25]/9 == 2)
				move(A, w1, ans);
			else if(A[19]/9 == 2)
				move(A, "U", ans), move(A, w1, ans);
			else	//A[21]/9 == 2
				move(A, w0, ans);
		}
	}
	ans += '\n';

	//test whether have finished
	for(int k=0; k<4; ++k)
	{
		if(A == Cube())
		{
			ans += 'U';
			if(k == 2)
				ans += '2';
			else if(k == 3)
				ans += '\'';
			return ans;
		}
		move(A, "U");
	}

	return ans;
}

string solveZhao (Cube A)
{	
	//print_cube(A);

	for(int n=1; true; ++n)
	{
		Cube B = A;
		string r = rand_move(10), t;
		move(B, r.c_str());
		string s = solve(B);
		if(B == Cube())
			return r + "\n" + s;
	}
}

string Standardize (const string &s)
{
	string t;
	Exchange<6> ex, y;
	y.roll(0, 4, 3, 1);
	for(string::const_iterator it = s.begin(); it != s.end(); )
	{
		char c = *it++;
		if(c == 'y')
		{
			int k = GetK(*it);
			if(it != s.end())
				++it;
			ex *= pow(y, k);
		}
		else
		{
			c = surfaceName[ex[surfaceNum[c]]];
			t.push_back(c);
			switch(*it)
			{
				case '3': case '\'':	++it; t.push_back('i');	break;
				case '2':				++it;
										t.push_back('\n');
										t.push_back(c);
										break;
			}
			t.push_back('\n');
		}
		while(it != s.end() && (*it == ' ' || *it == '\n'))
			++it;
	}
	return t;
}

int main ()
{
	init();
	Cube A;
	readBySurface(cin, A);
	string s = solveZhao(A);
	print_cube(A);
	cout << s << endl << endl << endl;
	cout << Standardize(s) << endl;
}
