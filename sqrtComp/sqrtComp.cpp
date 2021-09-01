/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// SQRTの演算速度の検証
/// 標準偏差を求める演算の高速化ができるかを検証
///  
/// 2021/09/01
///								Retar.jp
///
/// 開発環境
/// Windows 10 Pro
/// Visual Studio 2019
/// C++
/// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <Windows.h>
#include <iostream>
#include <random>
#include <iomanip>
#include <timeapi.h>
#include <time.h>    
#pragma comment(lib, "winmm.lib")
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#define BENCH_DEBUG_VECTORMAKE										//vector生成の途中経過
#define BENCH_DEBUG_STANDARD											//C++標準関数・標準偏差の途中経過
#define BENCH_DEBUG_EXPAND												//近似計算・標準偏差の途中経過
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//sqrt関数で近似値を求める
#define SQRT_ACCURACY_TEST 2.000000000f									//テストモード
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//３つの変数をいじって確認を！
#define BENCH_VECTORS 5													//テスト用のvectorの個数・1以上
#define BENCH_COUNT 2000000												//１つのベクターに含まれるデータ個数・1以上
#define SQRT_NEWTON_TIMES 5												//標準偏差を求めるときnTimesの個数
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define BENCH_LINE_FIGURE_LENGTH 12										//出力文字列長
#define BENCH_OUT_FIGURE_LENGTH 70										//出力文字列長
#define BENCH_MIN 1														//乱数の最小値
#define BENCH_MAX 99													//乱数の最大値
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int randnumber(int min, int max);										//乱数生成
double standardDivision_Standard(std::vector<double>& data);			//C++標準関数で演算
double standardDivision_Expand(std::vector<double>& data, int nTimes);	//近似値計算の関数、nTimesを2ぐらいが適切、テストを！
void randomVector(std::vector<double>& randvect);						//乱数を生成
float rsqrt32(float number, int times);									//sqrtのfloatバージョン
double rsqrt64(double number, int times);								//sqrtのdoubleバージョン
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//時間計測
	DWORD start_clock;
	DWORD end_clock;
	std::cout << "----------------------------------------------------------------------------------------\n";
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// SQRTの精度・Float
	float acTextFloat = SQRT_ACCURACY_TEST;
	std::cout << " FLOAT ORG DATA                        : "
		<< std::setfill(' ') << std::left << std::setw(BENCH_OUT_FIGURE_LENGTH) << std::setprecision(BENCH_OUT_FIGURE_LENGTH)
		<< acTextFloat << "\n";
	std::cout << " FLOAT SQRT  : sqrt(x)                 : "
		<< std::setfill(' ') << std::left << std::setw(BENCH_OUT_FIGURE_LENGTH) << std::setprecision(BENCH_OUT_FIGURE_LENGTH)
		<< sqrt(acTextFloat) << "\n";
	std::cout << " FLOAT SQRT  : std::sqrt(x)            : "
		<< std::setfill(' ') << std::left << std::setw(BENCH_OUT_FIGURE_LENGTH) << std::setprecision(BENCH_OUT_FIGURE_LENGTH)
		<< sqrt(acTextFloat) << "\n";
	std::cout << ">FLOAT SQRT  : rsqrt64(x,0)            : "
		<< std::setfill(' ') << std::left << std::setw(BENCH_OUT_FIGURE_LENGTH) << std::setprecision(BENCH_OUT_FIGURE_LENGTH)
		<< rsqrt32(acTextFloat, 0) << "\n";
	std::cout << ">FLOAT SQRT  : rsqrt64(x,1)            : "
		<< std::setfill(' ') << std::left << std::setw(BENCH_OUT_FIGURE_LENGTH) << std::setprecision(BENCH_OUT_FIGURE_LENGTH)
		<< rsqrt32(acTextFloat, 1) << "\n";
	std::cout << ">FLOAT SQRT  : rsqrt64(x,2)            : "
		<< std::setfill(' ') << std::left << std::setw(BENCH_OUT_FIGURE_LENGTH) << std::setprecision(BENCH_OUT_FIGURE_LENGTH)
		<< rsqrt32(acTextFloat, 2) << "\n";
	std::cout << ">FLOAT SQRT  : rsqrt64(x,3)            : "
		<< std::setfill(' ') << std::left << std::setw(BENCH_OUT_FIGURE_LENGTH) << std::setprecision(BENCH_OUT_FIGURE_LENGTH)
		<< rsqrt32(acTextFloat, 3) << "\n";
	std::cout << ">FLOAT SQRT  : rsqrt64(x,4)            : "
		<< std::setfill(' ') << std::left << std::setw(BENCH_OUT_FIGURE_LENGTH) << std::setprecision(BENCH_OUT_FIGURE_LENGTH)
		<< rsqrt32(acTextFloat, 4) << "\n";
	std::cout << ">FLOAT SQRT  : rsqrt64(x,5)            : "
		<< std::setfill(' ') << std::left << std::setw(BENCH_OUT_FIGURE_LENGTH) << std::setprecision(BENCH_OUT_FIGURE_LENGTH)
		<< rsqrt32(acTextFloat, 5) << "\n";
	std::cout << "----------------------------------------------------------------------------------------\n";
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// SQRTの精度・Double
	double acTextDouble = SQRT_ACCURACY_TEST;
	std::cout << " DOUBLE ORG DATA                       : "
		<< std::setfill(' ') << std::left << std::setw(BENCH_OUT_FIGURE_LENGTH) << std::setprecision(BENCH_OUT_FIGURE_LENGTH)
		<< acTextDouble << "\n";
	std::cout << " DOUBLE SQRT : sqrt(x)                 : "
		<< std::setfill(' ') << std::left << std::setw(BENCH_OUT_FIGURE_LENGTH) << std::setprecision(BENCH_OUT_FIGURE_LENGTH)
		<< sqrt(acTextDouble) << "\n";
	std::cout << " DOUBLE SQRT : std::sqrt(x)            : "
		<< std::setfill(' ') << std::left << std::setw(BENCH_OUT_FIGURE_LENGTH) << std::setprecision(BENCH_OUT_FIGURE_LENGTH)
		<< sqrt(acTextDouble) << "\n";
	std::cout << ">DOUBLE SQRT : rsqrt64(x,0)            : "
		<< std::setfill(' ') << std::left << std::setw(BENCH_OUT_FIGURE_LENGTH) << std::setprecision(BENCH_OUT_FIGURE_LENGTH)
		<< rsqrt64(acTextDouble, 0) << "\n";
	std::cout << ">DOUBLE SQRT : rsqrt64(x,1)            : "
		<< std::setfill(' ') << std::left << std::setw(BENCH_OUT_FIGURE_LENGTH) << std::setprecision(BENCH_OUT_FIGURE_LENGTH)
		<< rsqrt64(acTextDouble, 1) << "\n";
	std::cout << ">DOUBLE SQRT : rsqrt64(x,2)            : "
		<< std::setfill(' ') << std::left << std::setw(BENCH_OUT_FIGURE_LENGTH) << std::setprecision(BENCH_OUT_FIGURE_LENGTH)
		<< rsqrt64(acTextDouble, 2) << "\n";
	std::cout << ">DOUBLE SQRT : rsqrt64(x,3)            : "
		<< std::setfill(' ') << std::left << std::setw(BENCH_OUT_FIGURE_LENGTH) << std::setprecision(BENCH_OUT_FIGURE_LENGTH)
		<< rsqrt64(acTextDouble, 3) << "\n";
	std::cout << ">DOUBLE SQRT : rsqrt64(x,4)            : "
		<< std::setfill(' ') << std::left << std::setw(BENCH_OUT_FIGURE_LENGTH) << std::setprecision(BENCH_OUT_FIGURE_LENGTH)
		<< rsqrt64(acTextDouble, 4) << "\n";
	std::cout << ">DOUBLE SQRT : rsqrt64(x,5)            : "
		<< std::setfill(' ') << std::left << std::setw(BENCH_OUT_FIGURE_LENGTH) << std::setprecision(BENCH_OUT_FIGURE_LENGTH)
		<< rsqrt64(acTextDouble, 5) << "\n";
	std::cout << "----------------------------------------------------------------------------------------\n";
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// スタート時間
	start_clock = timeGetTime();
	//乱数のベクトル
	//メモリ上に乱数のベクトルを用意しておく
	std::cout << "Make Random Numbers Vector " << BENCH_VECTORS << " >>> START\n";
	std::vector<std::vector<double>> randvectS;
	for (auto j = 0; j < BENCH_VECTORS; j++)
	{
		std::vector<double> randvect;
		randomVector(randvect);
		randvectS.push_back(randvect);
		if (j % 100 == 0 && j != 0)
		{
			std::cout << "(" << j << ") ";
		}
	}
	std::cout << "\nMake Random Numbers Vector " << BENCH_VECTORS << " >>> END\n";
	// 終了時間
	end_clock = timeGetTime();
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//結果出力
#ifdef BENCH_DEBUG_VECTORMAKE
	//表示
	for (auto itrrand = randvectS.begin(); itrrand != randvectS.end(); itrrand++)
	{
		std::cout << "NUMBER : " << std::right << std::setw(BENCH_LINE_FIGURE_LENGTH) << std::setfill('_') << std::distance(randvectS.begin(), itrrand) << "\n";
		for (auto itr = (*itrrand).begin(); itr != (*itrrand).end(); itr++)
		{
			std::cout << "RANDOM : " << std::right << std::setw(BENCH_LINE_FIGURE_LENGTH) << std::setfill('0') << std::distance((*itrrand).begin(), itr);
			std::cout << "    " << std::setfill(' ') << std::left << std::setw(BENCH_OUT_FIGURE_LENGTH) << std::setprecision(BENCH_OUT_FIGURE_LENGTH) << (*itr) << std::endl;
		}
		std::cout << "----------------------------------------------------------------------------------------\n";
	}
#endif
	// 時間計測
	std::cout << "\n>>>  duration = " << std::setfill(' ') << std::left << std::setw(6) << std::setprecision(6) << (double)((double)end_clock - (double)start_clock) / 1000 << "sec.\n";
	std::cout << "----------------------------------------------------------------------------------------\n";
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 標準偏差・関数を分解
	for (auto ntimes = 0; ntimes < SQRT_NEWTON_TIMES; ntimes++)
	{
		std::cout << "Standard Division Standard Functions: Expand Function : SQRT_NEWTON_TIMES(" << ntimes + 1 << ") >>>\n";
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// スタート時間
		start_clock = timeGetTime();
		//標準偏差を求めるベンチマーク
		start_clock = timeGetTime();
		//標準偏差を求めるベンチマーク
		std::vector<double> resexp;
		for (auto itrrand = randvectS.begin(); itrrand != randvectS.end(); itrrand++)
		{
			resexp.push_back(standardDivision_Expand(*itrrand, ntimes));
		}
		// 終了時間
		end_clock = timeGetTime();
		//	//結果出力
#ifdef BENCH_DEBUG_EXPAND
		for (auto itr = resexp.begin(); itr != resexp.end(); itr++)
		{
			std::cout << "NUMBER : " << std::right
				<< std::setw(BENCH_LINE_FIGURE_LENGTH) << std::setfill('_') << std::distance(resexp.begin(), itr);
			std::cout << "  STDEVP " << (*itr) << "\n";
		}
#endif
		resexp.clear();
		// 時間計測
		std::cout << ">>>  TIMES : " << BENCH_VECTORS << "   CONTAIN : " << BENCH_COUNT << "\n";
		std::cout << ">>>  duration = " << std::setfill(' ') << std::left << std::setw(6) << std::setprecision(6) << (double)((double)end_clock - (double)start_clock) / 1000 << "sec.\n";
		std::cout << "----------------------------------------------------------------------------------------\n";
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 標準偏差・C++標準
	std::cout << "Standard Division Standard Functions: pow AND sqrt >>>\n";
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// スタート時間
	start_clock = timeGetTime();
	//標準偏差を求めるベンチマーク
	std::vector<double> res;
	for (auto itrrand = randvectS.begin(); itrrand != randvectS.end(); itrrand++)
	{
		res.push_back(standardDivision_Standard(*itrrand));
	}
	// 終了時間
	end_clock = timeGetTime();
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//結果出力
#ifdef BENCH_DEBUG_STANDARD
	for (auto itr = res.begin(); itr != res.end(); itr++)
	{
		std::cout << "NUMBER : " << std::right
			<< std::setw(BENCH_LINE_FIGURE_LENGTH) << std::setfill('_') << std::distance(res.begin(), itr);
		std::cout << "  STDEVP " << (*itr) << "\n";
	}
#endif
	// 時間計測
	std::cout << ">>>  TIMES : " << BENCH_VECTORS << "   CONTAIN : " << BENCH_COUNT << "\n";
	std::cout << ">>>  duration = " << std::setfill(' ') << std::left << std::setw(6) << std::setprecision(6) << (double)((double)end_clock - (double)start_clock) / 1000 << "sec.\n";
	std::cout << "----------------------------------------------------------------------------------------\n";
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//終了
	return 0;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// 乱数のベクトル生成
/// メモリ上に乱数のベクトルを用意しておく
/// </summary>
/// <param name="randvect">乱数のベクトル</param>
void randomVector(std::vector<double>& randvect)
{
	for (auto i = 0; i < BENCH_COUNT; i++)
	{
		double a = (double)randnumber(BENCH_MIN, BENCH_MAX) / (double)randnumber(BENCH_MIN, BENCH_MAX);
		randvect.push_back(a);
		//if (i % 10000 == 0 && i != 0)
		//{
		//	std::cout << i << " ";
		//}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// 標準偏差・関数を分解して掛算に直す
/// </summary>
/// <param name="data">データベクタ</param>
/// <returns>標準偏差</returns>
double standardDivision_Expand(std::vector<double>& data, int nTimes)
{
	double sum = 0.0f;
	double mean = 0.0f;
	double standardDeviation = 0.0f;

	//平均
	for (auto i = 0; i < data.size(); ++i)
	{
		sum += data[i];
	}
	mean = sum / data.size();											

	//標準偏差
	for (auto i = 0; i < data.size(); ++i)
	{
		//standardDeviation += pow(data[i] - mean, 2);					//<-もともと
		standardDeviation += (data[i] - mean) * (data[i] - mean);
	}
	//return std::sqrt(standardDeviation / data.size());				//<-もともと
	return rsqrt64(standardDeviation / data.size(), nTimes);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// sqrt ニュートン法で近似値求める・float
/// https://cs.uwaterloo.ca/~m32rober/rsqrt.pdf
/// http://takashiijiri.com/study/miscs/fastsqrt.html
/// </summary>
/// <param name="number">数字</param>
/// <param name="times">精度向上回数</param>
/// <returns>sqrtを返す</returns>
float rsqrt32(float number, int times)
{
	float x2 = 0.5f * number;
	int   tmp = 0x5F3759DF - (*(int*)&number >> 1); //initial guess
	float result = *(float*)&tmp;
	for (auto z = 0; z < times; z++)
	{
		result *= (1.5 - (x2 * result * result));
	}
	return result * number;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// sqrt ニュートン法で近似値求める・double
/// https://cs.uwaterloo.ca/~m32rober/rsqrt.pdf
/// http://takashiijiri.com/study/miscs/fastsqrt.html
/// </summary>
/// <param name="number">数字</param>
/// <param name="times">精度向上回数</param>
/// <returns>sqrtを返す</returns>
double rsqrt64(double number, int times)
{
	double         x2 = 0.5 * number;
	long long int  tmp = 0x5FE6EB50C7B537AAl - (*(long long int*) & number >> 1);
	double         result = *(double*)&tmp;
	for (auto z = 0; z < times; z++)
	{
		result *= (1.5 - (x2 * result * result));
	}
	return result * number;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// 標準偏差・C++標準
/// pow、sqrt
/// </summary>
/// <param name="data">データベクタ</param>
/// <returns>標準偏差</returns>
double standardDivision_Standard(std::vector<double>& data)
{
	double sum = 0.0f;
	double mean = 0.0f;
	double standardDeviation = 0.0f;

	//平均
	for (auto i = 0; i < data.size(); ++i)
	{
		sum += data[i];
	}
	mean = sum / data.size();

	//標準偏差
	for (auto i = 0; i < data.size(); ++i)
	{
		standardDeviation += pow(data[i] - mean, 2);
	}
	return std::sqrt(standardDeviation / data.size());
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// 整数・乱数生成
/// 
/// </summary>
/// <param name="min">最小</param>
/// <param name="max">最大</param>
int randnumber(int min, int max)
{
	std::random_device rd;									//乱数生成
	std::mt19937_64 mt(rd());								//メルセンヌ・ツイスター法によって生成される乱数列
	std::uniform_int_distribution<int> uid(min, max);		//最大最小の制限
	return uid(mt);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
