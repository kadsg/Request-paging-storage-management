#include "paging.h"

int main(void) {
	Paging p;
	int switch_on;

	while (true) {
		cout << endl << endl << endl << endl;
		cout << "--------请求调页存储管理方式的模拟---------" << endl;
		p.showAllocation();
		cout << "|\t\t1.FIFO\t\t\t|" << endl;
		cout << "|\t\t2.LRU\t\t\t|" << endl;
		cout << "|	    3.查看页访问顺序\t\t|" << endl;
		cout << "|	    4.更改分配信息\t\t|" << endl;
		cout << "--------------------------------------------" << endl;
		cout << endl;

		cin >> switch_on;
		cout << endl;

		switch (switch_on)
		{
		case 1: p.FIFO(); break;
		case 2: p.LRU(); break;
		case 3: p.showPage(); break;
		case 4: {
			int allocation, capacity;

			cout << endl << "输入内存块分配数：";
			cin >> allocation;
			cout << "输入页可容纳的指令数：";
			cin >> capacity;

			p.setAllocation(allocation, capacity);
		}
		default:
			break;
		}
	}

	return 0;
}