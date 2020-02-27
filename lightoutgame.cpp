#include <iostream>
#include <vector>
#include <ctime>
#include <bitset>

using namespace std;

template<size_t N> 
void initA(vector<bitset<N> >& A, const int m, const int n)
{
	for (int r = 0; r < N; r++)
	{
		A[r][r] = 1;

		if (r / n + 1 < m)
		{
			A[r][n * (r / n + 1) + r % n] = 1;
			A[n * (r / n + 1) + r % n][r] = 1;
		}

		if (r % n + 1 < n)
		{
			A[r][n * (r / n) + r % n + 1] = 1;
			A[n * (r / n) + r % n + 1][r] = 1;
		}
	}
}

template<size_t N> 
void gauss(vector<bitset<N> >& A, bitset<N>& a, bitset<N>& b)
{
	clock_t start, end;
	start = clock();

	bitset<N> cp = a ^ b;

	int r = 0;
	int c = 0;

	for (; c < N; c++)
	{
		if (A[r][c] != 1)
		{
			int mark = r;

			for (int i = r; i < N; i++)
				if (A[i][c])
				{
					mark = i;
					break;
				}

			if (mark != r)
			{
				swap(A[mark], A[r]);
				if (cp[mark] != cp[r])
				{
					cp.flip(mark);
					cp.flip(r);
				}
			}
			else
				continue;
		}

		for (int i = r + 1; i < N; i++)
		{
			if (A[i][c])
			{
				A[i] ^= A[r];
				cp[i] = cp[i] ^ cp[r];
			}
		}

		r++;
	}

	// 判断是否有解
	int cnt = 0;
	for (int i = N - 1; i >= 0; i--)
		if (cp[i] != 1)
			cnt++;
		else
			break;

	if (cnt < N - r) // 无解
	{
		cout << "no solution..." << endl;
		return;
	}

	vector<bitset<N> > ans;

	if (r == N)
	{
		bitset<N> sp(cp);
		for (int i = r - 1; i >= 0; i--)
			for (int j = i + 1; j < N; j++)
				sp[i] = sp[i] ^ (A[i][j] & sp[j]);

		ans.push_back(sp); // 唯一解
	}
	else if (N - r <= 10)
		helper(A, ans, cp, r, N - r); // 多解
	
	// 计算时间
	end = clock(); // 结束时间
	cout << "计算时间: " << (end - start) / 1000 << "s" << endl;

	// 打印解
	cout << "自由变量数目: " << N - r << endl;
	cout << "解数目: " << ans.size() << endl;
	for (auto i : ans)
	{
		string s = i.to_string();
		reverse(s.begin(), s.end());
		cout << s << endl;
	}
}

// 遍历自由变量取值求方程组多解
template<size_t N> 
void helper(vector<bitset<N> >& A, vector<bitset<N> >& ans, bitset<N> cp, int r, int f)
{
	if (f == 0)
	{
		for (int i = r - 1; i >= 0; i--)
			for (int j = i + 1; j < N; j++)
				cp[i] = cp[i] ^ (A[i][j] & cp[j]);

		ans.push_back(cp);
		return;
	}

	helper(A, ans, cp, r, f - 1);
	cp.set(N - f);
	helper(A, ans, cp, r, f - 1);
}


// 通解加特解的版本求方程组多解
//template<size_t N> 
//void helper(vector<bitset<N> >& A, vector<bitset<N> >& ans, bitset<N> sp, int r, int f)
//{
//	if (f == 0)
//	{
//		ans.push_back(sp);
//		return;
//	}
//
//	bitset<N> gp;
//	gp.set(N - f);
//	for (int i = r - 1; i >= 0; i--)
//		for (int j = i + 1; j < N; j++)
//			gp[i] = gp[i] ^ (A[i][j] & gp[j]);
//
//	helper(A, ans, sp, r, f - 1);
//	helper(A, ans, sp ^ gp, r, f - 1);
//}


int main()
{
	const int m = 104;
	const int n = m;
	vector<bitset<m * n> > A(m * n, bitset<m * n>());
	bitset<m * n> c; // 初始状态
	bitset<m * n> r; // 目标状态
	c.set();

	//string s = c.to_string();
	//reverse(s.begin(), s.end());
	//cout << "初始状态: " << s << endl;

	//s = r.to_string();
	//reverse(s.begin(), s.end());
	//cout << "目标状态: " << s << endl;

	initA(A, m, n);
	//for (auto i : A)
	//{
	//	string s = i.to_string();
	//	reverse(s.begin(), s.end());
	//	cout << s << endl;
	//}


	gauss(A, c, r);

	return 0;
}