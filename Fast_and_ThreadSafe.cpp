// Test_Cpp1.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <thread>
#include <vector>
#include <time.h>
#include <windows.h>

using namespace std;
#define LOOP_CNT 100000000
#define LOOP_LIMIT LOOP_CNT * 4

void worker(int* result)
{
	for (int n = 0; n < LOOP_CNT; n++)
	{
		(*result) += 1;
	}
}

void volatile_worker(volatile int* result)
{
	for (int n = 0; n < LOOP_CNT; n++)
	{
		(*result) += 1;
	}
}

void atomic_worker(atomic<int>* result)
{
	for (int n = 0; n < LOOP_CNT; n++)
	{
		(*result) += 1;
	}
}
void atomic_worker_fetch_add(atomic<int>* result)
{
	for (int n = 0; n < LOOP_CNT; n++)
	{
		(*result).fetch_add(1);
	}
}

void atomic_sse_worker_fetch_add(atomic<__int64>* result)
{
	for (int n = 0; n < LOOP_CNT; n++)
	{
		(*result).fetch_add(1);
	}
}

void sse_worker(__int64* result)
{
	for (int n = 0; n < LOOP_CNT; n++)
	{
		(*result) += 1;
	}
}

CRITICAL_SECTION gCS;

void worker_cs(int* result)
{
	::EnterCriticalSection(&gCS);
	for (int n = 0; n < LOOP_CNT; n++)
	{
		if ((*result) >= LOOP_LIMIT)
			break;
		(*result) += 1;
	}
	::LeaveCriticalSection(&gCS);
}

void volatile_worker_cs(volatile int* result)
{
	::EnterCriticalSection(&gCS);
	for (int n = 0; n < LOOP_CNT; n++)
	{
		if ((*result) >= LOOP_LIMIT)
			break;
		(*result) += 1;
	}
	::LeaveCriticalSection(&gCS);
}

void sse_worker_cs(__int64* result)
{
	::EnterCriticalSection(&gCS);
	for (int n = 0; n < LOOP_CNT; n++)
	{
		if ((*result) >= LOOP_LIMIT)
			break;
		(*result) += 1;
	}
	::LeaveCriticalSection(&gCS);
}

int main()
{
	clock_t start, end;
	int nK = 0;
	register int rK = 0;
	volatile int vK = 0;
	__int64 sseK = 0;
	atomic<int> aK = 0;

	::InitializeCriticalSection(&gCS);

	//각 변수 타입의 계산 시간을 확인해보자.
	start = clock();
	for (nK = 0; nK < LOOP_CNT; nK++)
	{
	}
	end = clock();
	cout << end - start<< " nK done!" << endl;

	start = clock();
	for (rK = 0; rK < LOOP_CNT; rK++)
	{
	}
	end = clock();
	cout << end - start << " rK done!" << endl;

	start = clock();
	for (vK = 0; vK < LOOP_CNT; vK++)
	{
	}
	end = clock();
	cout << end - start << " vK done!" << endl;

	start = clock();
	for (sseK = 0; sseK < LOOP_CNT; sseK++)
	{
	}
	end = clock();
	cout << end - start << " sseK done!" << endl;

	start = clock();
	for (aK = 0; aK < LOOP_CNT; aK++)
	{
	}
	end = clock();
	cout << end - start << " aK done!" << endl;

	nK = 0;
	rK = 0;
	vK = 0;
	aK = 0;
	sseK;

	//동기화 없이 계산하면 어떻게 될까?
	vector<thread> vThread;
	vThread.push_back(thread(worker, &nK));
	vThread.push_back(thread(worker, &nK));
	vThread.push_back(thread(worker, &nK));
	vThread.push_back(thread(worker, &nK));

	start = clock();
	for (int i = 0; i < 4; i++)
	{
		vThread[i].join();
	}
	end = clock();
	cout << nK << " " << end - start << " nK done!" << endl;
	vThread.clear();

	vThread.push_back(thread(worker, &rK));
	vThread.push_back(thread(worker, &rK));
	vThread.push_back(thread(worker, &rK));
	vThread.push_back(thread(worker, &rK));

	start = clock();
	for (int i = 0; i < 4; i++)
	{
		vThread[i].join();
	}
	end = clock();
	cout << rK << " " << end - start << " rK done!" << endl;
	vThread.clear();

	vThread.push_back(thread(volatile_worker, &vK));
	vThread.push_back(thread(volatile_worker, &vK));
	vThread.push_back(thread(volatile_worker, &vK));
	vThread.push_back(thread(volatile_worker, &vK));

	start = clock();
	for (int i = 0; i < 4; i++)
	{
		vThread[i].join();
	}
	end = clock();
	cout << vK << " " << end - start << " vK done!" << endl;
	vThread.clear();

	vThread.push_back(thread(sse_worker, &sseK));
	vThread.push_back(thread(sse_worker, &sseK));
	vThread.push_back(thread(sse_worker, &sseK));
	vThread.push_back(thread(sse_worker, &sseK));

	start = clock();
	for (int i = 0; i < 4; i++)
	{
		vThread[i].join();
	}
	end = clock();
	cout << sseK << " " << end - start << " sseK done!" << endl;
	vThread.clear();

	vThread.push_back(thread(atomic_worker, &aK));
	vThread.push_back(thread(atomic_worker, &aK));
	vThread.push_back(thread(atomic_worker, &aK));
	vThread.push_back(thread(atomic_worker, &aK));

	start = clock();
	for (int i = 0; i < 4; i++)
	{
		vThread[i].join();
	}
	end = clock();
	cout << aK << " " << end - start << " aK done!" << endl;
	vThread.clear();

	aK = 0;
	vThread.push_back(thread(atomic_worker_fetch_add, &aK));
	vThread.push_back(thread(atomic_worker_fetch_add, &aK));
	vThread.push_back(thread(atomic_worker_fetch_add, &aK));
	vThread.push_back(thread(atomic_worker_fetch_add, &aK));

	start = clock();
	for (int i = 0; i < 4; i++)
	{
		vThread[i].join();
	}
	end = clock();
	cout << aK << " " << end - start << " fetch_add aK done!" << endl;
	vThread.clear();

	nK = 0;
	rK = 0;
	vK = 0;
	sseK = 0;

	//크리티컬 섹션을 걸어 동기화 시키면 어떻게 될까?
	vThread.push_back(thread(worker_cs, &nK));
	vThread.push_back(thread(worker_cs, &nK));
	vThread.push_back(thread(worker_cs, &nK));
	vThread.push_back(thread(worker_cs, &nK));

	start = clock();
	for (int i = 0; i < 4; i++)
	{
		vThread[i].join();
	}
	end = clock();
	cout << nK << " " << end - start << " CS nK done!" << endl;
	vThread.clear();

	vThread.push_back(thread(worker_cs, &rK));
	vThread.push_back(thread(worker_cs, &rK));
	vThread.push_back(thread(worker_cs, &rK));
	vThread.push_back(thread(worker_cs, &rK));

	start = clock();
	for (int i = 0; i < 4; i++)
	{
		vThread[i].join();
	}
	end = clock();
	cout << rK << " " << end - start << " CS rK done!" << endl;
	vThread.clear();

	vThread.push_back(thread(volatile_worker_cs, &vK));
	vThread.push_back(thread(volatile_worker_cs, &vK));
	vThread.push_back(thread(volatile_worker_cs, &vK));
	vThread.push_back(thread(volatile_worker_cs, &vK));

	start = clock();
	for (int i = 0; i < 4; i++)
	{
		vThread[i].join();
	}
	end = clock();
	cout << vK << " " << end - start << " CS vK done!" << endl;
	vThread.clear();

	vThread.push_back(thread(sse_worker_cs, &sseK));
	vThread.push_back(thread(sse_worker_cs, &sseK));
	vThread.push_back(thread(sse_worker_cs, &sseK));
	vThread.push_back(thread(sse_worker_cs, &sseK));

	start = clock();
	for (int i = 0; i < 4; i++)
	{
		vThread[i].join();
	}
	end = clock();
	cout << sseK << " " << end - start << " CS sseK done!" << endl;
	vThread.clear();

	rK = 0;

	//SSE 연산이 확실히 빨라보인다. 혹시 스레드 갯수의 차이가 아닐까?
	vThread.push_back(thread(worker_cs, &rK));
	vThread.push_back(thread(worker_cs, &rK));
	vThread.push_back(thread(worker_cs, &rK));
	vThread.push_back(thread(worker_cs, &rK));
	vThread.push_back(thread(worker_cs, &rK));
	vThread.push_back(thread(worker_cs, &rK));
	vThread.push_back(thread(worker_cs, &rK));
	vThread.push_back(thread(worker_cs, &rK));

	start = clock();
	for (int i = 0; i < 8; i++)
	{
		vThread[i].join();
	}
	end = clock();
	cout << rK << " " << end - start << " 8 Thread CS rK done!" << endl;
	vThread.clear();
}