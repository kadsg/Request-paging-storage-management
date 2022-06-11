#include <cstdlib>
#include <ctime>
#include <vector>
#include <queue>
#include <iostream>
#include "block.h"

using namespace std;

/* 页面调度类 */
class Paging {
	/* 类成员变量代表置换算法 */
	class Algorithm {
	public:
		const static int FIFO = 1; // 表示先进先出的页面调度算法
		const static int LRU = 2; // 表示最近最长时间未被使用的页面调度算法
	};
private:
	int blockSize = 4; // 分配的内存块数
	const int instructionNum = 320; // 生成指令的数量
	int capacity = 10; // 每页可存放的指令数量
	Block* block; // 内存块
	int* instruction; // 指令集

public:
	Paging() {
		init();
		
	}

	~Paging() {
		free();
	}
private:
	
	/* 初始化 */
	void init() {

		block = new Block[blockSize]; // 为内存块分配空间
		instruction = new int[instructionNum]; // 为指令集分配空间

		for (int i = 0; i < blockSize; ++i) {
			block[i].initBlock(); // 初始化内存块
		}

		generateInstruction();
	}

	/* 释放资源 */
	void free() {
		if (block) {
			delete[] block;
		}
		if (instruction) {
			delete[] instruction;
		}
	}

	/* 生成指令集 */
	void generateInstruction() {
		int temp; // 用于记录指令地址

		srand((unsigned int)time(NULL));

		for (int i = 0; i < instructionNum; i += 4) {
			
			temp = rand() % instructionNum; // 生成地址范围为[0, instructionNum - 1]的指令
			instruction[i] = temp + 1; // 执行地址为temp + 1的指令
			temp = rand() % (temp + 2); // 在前地址[0，temp + 1]中随机选取一条指令
			instruction[i + 1] = temp; // 执行该指令
			instruction[i + 2] = temp + 1; // 顺序执行地址为temp + 1的指令
			temp = rand() % (instructionNum - (temp + 2)) + temp + 2; // 在后地址[temp + 2，instructionNum - 1]中随机选取一条指令
			instruction[i + 3] = temp; // 执行
		}
	}

	/* 将指令变换成对应的页号 */
	int instructionToPage(int instruction) {
		return instruction / capacity;
	}

	/*  返回页面在内存中的位置
		返回-1说明该页未调入内存
	*/
	int pageLocation(int pageID) {
		for (int i = 0; i < blockSize; ++i) {
			if (block[i].pageID == pageID)
				return i;
		}
		return -1;
	}

	/*  返回空内存块的块号
		返回-1说明内存已满
	*/
	int freeMemory() {
		for (int i = 0; i < blockSize; ++i) {
			if (block[i].pageID == -1)
				return i;
		}
		return -1;
	}

	/*  输出调入内存的页面 
	 *	pageNum是内存中页的数量
	 *  algorithm代表所采用的调度算法
	*/
	void printPage(int pageNum, int algorithm) {

		cout << endl;
		cout << "页面地址：";
		for (int i = 0; i < pageNum; ++i) {
			cout << i << '\t';
		}
		cout << endl;


		cout << "页面走向：";
		for (int i = 0; i < pageNum; ++i) {
			cout << block[i].pageID << '\t';
		}

		if(algorithm == Algorithm::LRU) {
			cout << endl << "时间间隔：";
			for (int i = 0; i < pageNum; ++i) {
				cout << block[i].lastVisit << '\t';
			}
		}
		cout << endl << endl;
	}

	/* 寻找最近未被使用的页并返回其在内存中的地址 */
	int least(Block page[], int pageNum) {
		int max;
		int index = -1;

		for (int i = 0; i < pageNum; ++i) {
			if (index == -1) {
				max = page[i].lastVisit;
				index = i;
			} // 初始化
			
			if (max < page[i].lastVisit) {
				max = page[i].lastVisit;
				index = i;
			}
		}

		return index;
	}

	/* 
		对LRU算法中页面的计时器进行更新 
		index是刚刚访问过的页面
	*/
	void setLastVisitTime(Block page[], int pageNum, int index) {
		for (int i = 0; i < pageNum; ++i) {
			if (i == index) {
				block[i].lastVisit = 0; // 刚刚访问过，置0
			}
			else {
				++block[i].lastVisit; // 为访问过的计时器 + 1
			}
		}
	}

	/* 内存清理 */
	void clear() {
		for (int i = 0; i < blockSize; ++i) {
			block[i].initBlock(); // 初始化内存块
		}
	}

public:
	/* 先进先出的置换算法 */
	void FIFO() {
		int pageID; // 页号
		int freeBlockLocation; // 空闲内存块号
		int pageNum = 0; // 已调入内存的页面数
		int failureNum = 0; // 故障次数
		queue<int>pageQueue; // FIFO的数据结构

		for (int i = 0; i < instructionNum; ++i) {
			pageID = instructionToPage(instruction[i]);

			cout << "执行第 " << i + 1 << " 条指令：" << instruction[i]
				<< "，对应页号：" << pageID << endl;

			printPage(pageNum, Algorithm::FIFO);

			int location = pageLocation(pageID);
			// 如果该页在内存中
			if (location != -1) {
				cout << "该页在内存中的物理地址为：" << location;
			}
			// 如果该页不在内存中
			else {
				cout << "发生缺页！";

				++failureNum;
				pageQueue.push(pageID);

				freeBlockLocation = freeMemory();

				if (freeBlockLocation != -1) { // 尚有空闲内存块
					++pageNum;
					block[freeBlockLocation].pageID = pageID; 
					block[freeBlockLocation].lastVisit = -1; // 将该页直接调入空闲内存块中
					cout << "调入页 " << pageID << endl;
				}
				else { // 需要选择一页换出
					int index = pageQueue.front();

					cout << "将页 " << index << " 换出" << endl;

					pageQueue.pop();

					// 换出页
					for (int i = 0; i < blockSize; ++i) {
						if (block[i].pageID == index) {
							index = i; // 记录下换出页的内存块号（物理地址）
							break;
						}
					}
					// 将该页调入到内存中
					block[index].pageID = pageID;
					block[index].lastVisit = -1;

					cout << "该页在内存中的物理地址为：" << pageLocation(pageID) << endl;
				}
				printPage(pageNum, Algorithm::FIFO);
			}
			cout << endl << endl;
		}

		cout << "访问页的总次数为 " << instructionNum << " 次" << endl;
		cout << "使用FIFO算法，缺页次数为 " << failureNum << " 次，缺页率为 "
			<< 1.0 * failureNum / instructionNum * 100 << " %" << endl << endl;

		clear();
	}

	/* 最近最久未被访问的置换算法 */
	void LRU() {
		int pageID; // 页号
		int freeBlockLocation; // 空闲内存块号
		int pageNum = 0; // 已调入内存的页面数
		int failureNum = 0; // 故障次数

		for (int i = 0; i < instructionNum; ++i) {
			pageID = instructionToPage(instruction[i]);

			cout << endl;
			cout << "执行第 " << i + 1 << " 条指令：" << instruction[i]
				<< "，对应页号：" << pageID << endl;

			// 如果该页在内存中
			int location = pageLocation(pageID);
			if (location != -1) {
				// 更新页的访问时间
				setLastVisitTime(block, pageNum, location);
				printPage(pageNum, Algorithm::LRU);

				cout << "该页在内存中的物理地址为：" << location;
			}
			// 如果该页不在内存中
			else {
				printPage(pageNum, Algorithm::LRU);
				cout << "发生缺页！";

				++failureNum;

				freeBlockLocation = freeMemory();
				if (freeBlockLocation != -1) { // 尚有空闲内存块
					++pageNum;
					block[freeBlockLocation].pageID = pageID; // 将该页直接调入空闲内存块中
					cout << "调入页 " << pageID << endl;

					// 更新页的访问时间
					setLastVisitTime(block, pageNum, freeBlockLocation);
					printPage(pageNum, Algorithm::LRU);
				}
				else { // 需要选择一页换出
					int index = least(block, pageNum);
					int tempID = block[index].pageID;
					cout << "将页 " << tempID << " 换出" << endl;

					// 换出页
					for (int i = 0; i < blockSize; ++i) {
						if (block[i].pageID == tempID) {
							index = i; // 记录下换出页的内存块号（物理地址）
							break;
						}
					}
					// 将该页调入到内存中
					block[index].pageID = pageID;
					// 更新页的访问时间
					setLastVisitTime(block, pageNum, index);

					printPage(pageNum, Algorithm::LRU);
					cout << "该页在内存中的物理地址为：" << pageLocation(pageID) << endl;
				}	
			}
			cout << endl << endl;
		}

		cout << "访问页的总次数为 " << instructionNum << " 次" << endl;
		cout << "使用LRU算法，缺页次数为 " << failureNum << " 次，缺页率为 "
			<< 1.0 * failureNum / instructionNum * 100 << " %" << endl << endl;

		clear();
	}

	/* 查看页访问顺序 */
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

	/* 查看分配信息 */
	void showAllocation() {
		cout << endl;
		cout << "|\t系统分配的内存块数：" << blockSize << "\t\t|" << endl
			<< "|\t系统生成的指令数量：" << instructionNum << "\t\t|" << endl
			<< "|\t页可容纳的指令数量：" << capacity << "\t\t|" << endl << endl;
	}

	/* 更改分配信息 */
	void setAllocation(int blockSize, int capacity) {
		this->blockSize = blockSize;
		this->capacity = capacity;
		
		delete[] block;
		block = new Block[blockSize]; // 为内存块分配空间
		for (int i = 0; i < blockSize; ++i) {
			block[i].initBlock(); // 初始化内存块
		}
	}
};

