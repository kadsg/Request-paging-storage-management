#ifndef BLOCK_H

/* �ڴ���Ϣ�� */
class Block {
public:
	int pageID; // ҳ��
	int lastVisit; // �������һ�η��ʵļ��ʱ��

	Block() {
		initBlock();
	}

	void initBlock() {
		this->pageID = -1;
		this->lastVisit = -1;
	}
};
#endif
