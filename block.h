#ifndef BLOCK_H

/* 内存信息类 */
class Block {
public:
	int pageID; // 页号
	int lastVisit; // 距离最后一次访问的间隔时间

	Block() {
		initBlock();
	}

	void initBlock() {
		this->pageID = -1;
		this->lastVisit = -1;
	}
};
#endif
