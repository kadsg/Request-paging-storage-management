#include "paging.h"

int main(void) {
	Paging p;
	int switch_on;

	while (true) {
		cout << endl << endl << endl << endl;
		cout << "--------�����ҳ�洢����ʽ��ģ��---------" << endl;
		p.showAllocation();
		cout << "|\t\t1.FIFO\t\t\t|" << endl;
		cout << "|\t\t2.LRU\t\t\t|" << endl;
		cout << "|	    3.�鿴ҳ����˳��\t\t|" << endl;
		cout << "|	    4.���ķ�����Ϣ\t\t|" << endl;
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

			cout << endl << "�����ڴ���������";
			cin >> allocation;
			cout << "����ҳ�����ɵ�ָ������";
			cin >> capacity;

			p.setAllocation(allocation, capacity);
		}
		default:
			break;
		}
	}

	return 0;
}