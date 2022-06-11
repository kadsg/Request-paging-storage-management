#include <cstdlib>
#include <ctime>
#include <vector>
#include <queue>
#include <iostream>
#include "block.h"

using namespace std;

/* ҳ������� */
class Paging {
	/* ���Ա���������û��㷨 */
	class Algorithm {
	public:
		const static int FIFO = 1; // ��ʾ�Ƚ��ȳ���ҳ������㷨
		const static int LRU = 2; // ��ʾ����ʱ��δ��ʹ�õ�ҳ������㷨
	};
private:
	int blockSize = 4; // ������ڴ����
	const int instructionNum = 320; // ����ָ�������
	int capacity = 10; // ÿҳ�ɴ�ŵ�ָ������
	Block* block; // �ڴ��
	int* instruction; // ָ�

public:
	Paging() {
		init();
		
	}

	~Paging() {
		free();
	}
private:
	
	/* ��ʼ�� */
	void init() {

		block = new Block[blockSize]; // Ϊ�ڴ�����ռ�
		instruction = new int[instructionNum]; // Ϊָ�����ռ�

		for (int i = 0; i < blockSize; ++i) {
			block[i].initBlock(); // ��ʼ���ڴ��
		}

		generateInstruction();
	}

	/* �ͷ���Դ */
	void free() {
		if (block) {
			delete[] block;
		}
		if (instruction) {
			delete[] instruction;
		}
	}

	/* ����ָ� */
	void generateInstruction() {
		int temp; // ���ڼ�¼ָ���ַ

		srand((unsigned int)time(NULL));

		for (int i = 0; i < instructionNum; i += 4) {
			
			temp = rand() % instructionNum; // ���ɵ�ַ��ΧΪ[0, instructionNum - 1]��ָ��
			instruction[i] = temp + 1; // ִ�е�ַΪtemp + 1��ָ��
			temp = rand() % (temp + 2); // ��ǰ��ַ[0��temp + 1]�����ѡȡһ��ָ��
			instruction[i + 1] = temp; // ִ�и�ָ��
			instruction[i + 2] = temp + 1; // ˳��ִ�е�ַΪtemp + 1��ָ��
			temp = rand() % (instructionNum - (temp + 2)) + temp + 2; // �ں��ַ[temp + 2��instructionNum - 1]�����ѡȡһ��ָ��
			instruction[i + 3] = temp; // ִ��
		}
	}

	/* ��ָ��任�ɶ�Ӧ��ҳ�� */
	int instructionToPage(int instruction) {
		return instruction / capacity;
	}

	/*  ����ҳ�����ڴ��е�λ��
		����-1˵����ҳδ�����ڴ�
	*/
	int pageLocation(int pageID) {
		for (int i = 0; i < blockSize; ++i) {
			if (block[i].pageID == pageID)
				return i;
		}
		return -1;
	}

	/*  ���ؿ��ڴ��Ŀ��
		����-1˵���ڴ�����
	*/
	int freeMemory() {
		for (int i = 0; i < blockSize; ++i) {
			if (block[i].pageID == -1)
				return i;
		}
		return -1;
	}

	/*  ��������ڴ��ҳ�� 
	 *	pageNum���ڴ���ҳ������
	 *  algorithm���������õĵ����㷨
	*/
	void printPage(int pageNum, int algorithm) {

		cout << endl;
		cout << "ҳ���ַ��";
		for (int i = 0; i < pageNum; ++i) {
			cout << i << '\t';
		}
		cout << endl;


		cout << "ҳ������";
		for (int i = 0; i < pageNum; ++i) {
			cout << block[i].pageID << '\t';
		}

		if(algorithm == Algorithm::LRU) {
			cout << endl << "ʱ������";
			for (int i = 0; i < pageNum; ++i) {
				cout << block[i].lastVisit << '\t';
			}
		}
		cout << endl << endl;
	}

	/* Ѱ�����δ��ʹ�õ�ҳ�����������ڴ��еĵ�ַ */
	int least(Block page[], int pageNum) {
		int max;
		int index = -1;

		for (int i = 0; i < pageNum; ++i) {
			if (index == -1) {
				max = page[i].lastVisit;
				index = i;
			} // ��ʼ��
			
			if (max < page[i].lastVisit) {
				max = page[i].lastVisit;
				index = i;
			}
		}

		return index;
	}

	/* 
		��LRU�㷨��ҳ��ļ�ʱ�����и��� 
		index�Ǹոշ��ʹ���ҳ��
	*/
	void setLastVisitTime(Block page[], int pageNum, int index) {
		for (int i = 0; i < pageNum; ++i) {
			if (i == index) {
				block[i].lastVisit = 0; // �ոշ��ʹ�����0
			}
			else {
				++block[i].lastVisit; // Ϊ���ʹ��ļ�ʱ�� + 1
			}
		}
	}

	/* �ڴ����� */
	void clear() {
		for (int i = 0; i < blockSize; ++i) {
			block[i].initBlock(); // ��ʼ���ڴ��
		}
	}

public:
	/* �Ƚ��ȳ����û��㷨 */
	void FIFO() {
		int pageID; // ҳ��
		int freeBlockLocation; // �����ڴ���
		int pageNum = 0; // �ѵ����ڴ��ҳ����
		int failureNum = 0; // ���ϴ���
		queue<int>pageQueue; // FIFO�����ݽṹ

		for (int i = 0; i < instructionNum; ++i) {
			pageID = instructionToPage(instruction[i]);

			cout << "ִ�е� " << i + 1 << " ��ָ�" << instruction[i]
				<< "����Ӧҳ�ţ�" << pageID << endl;

			printPage(pageNum, Algorithm::FIFO);

			int location = pageLocation(pageID);
			// �����ҳ���ڴ���
			if (location != -1) {
				cout << "��ҳ���ڴ��е������ַΪ��" << location;
			}
			// �����ҳ�����ڴ���
			else {
				cout << "����ȱҳ��";

				++failureNum;
				pageQueue.push(pageID);

				freeBlockLocation = freeMemory();

				if (freeBlockLocation != -1) { // ���п����ڴ��
					++pageNum;
					block[freeBlockLocation].pageID = pageID; 
					block[freeBlockLocation].lastVisit = -1; // ����ҳֱ�ӵ�������ڴ����
					cout << "����ҳ " << pageID << endl;
				}
				else { // ��Ҫѡ��һҳ����
					int index = pageQueue.front();

					cout << "��ҳ " << index << " ����" << endl;

					pageQueue.pop();

					// ����ҳ
					for (int i = 0; i < blockSize; ++i) {
						if (block[i].pageID == index) {
							index = i; // ��¼�»���ҳ���ڴ��ţ������ַ��
							break;
						}
					}
					// ����ҳ���뵽�ڴ���
					block[index].pageID = pageID;
					block[index].lastVisit = -1;

					cout << "��ҳ���ڴ��е������ַΪ��" << pageLocation(pageID) << endl;
				}
				printPage(pageNum, Algorithm::FIFO);
			}
			cout << endl << endl;
		}

		cout << "����ҳ���ܴ���Ϊ " << instructionNum << " ��" << endl;
		cout << "ʹ��FIFO�㷨��ȱҳ����Ϊ " << failureNum << " �Σ�ȱҳ��Ϊ "
			<< 1.0 * failureNum / instructionNum * 100 << " %" << endl << endl;

		clear();
	}

	/* ������δ�����ʵ��û��㷨 */
	void LRU() {
		int pageID; // ҳ��
		int freeBlockLocation; // �����ڴ���
		int pageNum = 0; // �ѵ����ڴ��ҳ����
		int failureNum = 0; // ���ϴ���

		for (int i = 0; i < instructionNum; ++i) {
			pageID = instructionToPage(instruction[i]);

			cout << endl;
			cout << "ִ�е� " << i + 1 << " ��ָ�" << instruction[i]
				<< "����Ӧҳ�ţ�" << pageID << endl;

			// �����ҳ���ڴ���
			int location = pageLocation(pageID);
			if (location != -1) {
				// ����ҳ�ķ���ʱ��
				setLastVisitTime(block, pageNum, location);
				printPage(pageNum, Algorithm::LRU);

				cout << "��ҳ���ڴ��е������ַΪ��" << location;
			}
			// �����ҳ�����ڴ���
			else {
				printPage(pageNum, Algorithm::LRU);
				cout << "����ȱҳ��";

				++failureNum;

				freeBlockLocation = freeMemory();
				if (freeBlockLocation != -1) { // ���п����ڴ��
					++pageNum;
					block[freeBlockLocation].pageID = pageID; // ����ҳֱ�ӵ�������ڴ����
					cout << "����ҳ " << pageID << endl;

					// ����ҳ�ķ���ʱ��
					setLastVisitTime(block, pageNum, freeBlockLocation);
					printPage(pageNum, Algorithm::LRU);
				}
				else { // ��Ҫѡ��һҳ����
					int index = least(block, pageNum);
					int tempID = block[index].pageID;
					cout << "��ҳ " << tempID << " ����" << endl;

					// ����ҳ
					for (int i = 0; i < blockSize; ++i) {
						if (block[i].pageID == tempID) {
							index = i; // ��¼�»���ҳ���ڴ��ţ������ַ��
							break;
						}
					}
					// ����ҳ���뵽�ڴ���
					block[index].pageID = pageID;
					// ����ҳ�ķ���ʱ��
					setLastVisitTime(block, pageNum, index);

					printPage(pageNum, Algorithm::LRU);
					cout << "��ҳ���ڴ��е������ַΪ��" << pageLocation(pageID) << endl;
				}	
			}
			cout << endl << endl;
		}

		cout << "����ҳ���ܴ���Ϊ " << instructionNum << " ��" << endl;
		cout << "ʹ��LRU�㷨��ȱҳ����Ϊ " << failureNum << " �Σ�ȱҳ��Ϊ "
			<< 1.0 * failureNum / instructionNum * 100 << " %" << endl << endl;

		clear();
	}

	/* �鿴ҳ����˳�� */
	void showPage() {
		int cnt = 0;

		for (int i = 0; i < instructionNum; ++i) {
			++cnt;
			cout << instructionToPage(instruction[i]) << '\t';
			if (cnt % 5 == 0) {
				cout << endl;
			}
		}
		cout << endl << endl;
	}

	/* �鿴������Ϣ */
	void showAllocation() {
		cout << endl;
		cout << "|\tϵͳ������ڴ������" << blockSize << "\t\t|" << endl
			<< "|\tϵͳ���ɵ�ָ��������" << instructionNum << "\t\t|" << endl
			<< "|\tҳ�����ɵ�ָ��������" << capacity << "\t\t|" << endl << endl;
	}

	/* ���ķ�����Ϣ */
	void setAllocation(int blockSize, int capacity) {
		this->blockSize = blockSize;
		this->capacity = capacity;
		
		delete[] block;
		block = new Block[blockSize]; // Ϊ�ڴ�����ռ�
		for (int i = 0; i < blockSize; ++i) {
			block[i].initBlock(); // ��ʼ���ڴ��
		}
	}
};

