#include "MapsParse.h"
#include "ThreadControl.h"

/*
 * 입력받은 메모리 영역에 값을 추출 합니다.
 */
void getValue(int mem_fd,long staMemAddr,long endMemAddr,void *buf){
	//printf("getValue Function! - Start\n");
	//printf("staMemAddr : %#lx, endMemAddr : %#lx\n",staMemAddr,endMemAddr);

	for (; staMemAddr < endMemAddr; staMemAddr += 4096) {
		//printf("fd : %d, staMemAddr : %#lx, endMemAddr : %#lx\n",mem_fd,staMemAddr,endMemAddr);
		lseek(mem_fd, staMemAddr, SEEK_SET);
		read(mem_fd, buf, MEMREADSIZE);
		for (int j = 0; j < MEMREADSIZE / 4; j++) {
			memInfoStruct.address = (void*) (staMemAddr + (j * 4));
			memInfoStruct.value = ((long**) buf)[j];
			memDataList.push_back(memInfoStruct);
		}
	}
	//printf("getValue Function! - End\n");
}

void reMemRead(){
	//printf("reMemRead Function! - Start\n");
	char memFileName[30];
	int mem_fd;
	void *buf = malloc(MEMREADSIZE);
	sprintf(memFileName, "/proc/%d/mem", privatePid);
	long staMemAddr = 0,endMemAddr = 0;
	if(0 < (mem_fd = open(memFileName, O_RDONLY | O_LARGEFILE))){
		for (int i = 0; i < findDataList.size(); i++) {
			findInfoStruct = findDataList.at(i);
			long cmp = (long)findInfoStruct.address;
			if((staMemAddr >= cmp) || (cmp >= endMemAddr)){
				staMemAddr = (long)findInfoStruct.address;
				endMemAddr = staMemAddr+MEMREADSIZE;
				getValue(mem_fd, staMemAddr, endMemAddr,buf);
			}
			//printf("staMemAddr : %ld, (long)findInfoStruct.address : %ld, endMemAddr : %ld\n",staMemAddr,(long)findInfoStruct.address,endMemAddr);
		}
	}else{
		printf("%s\n", errorMsgPid);
	}
	free(buf);
	close(mem_fd);
	//printf("reMemRead memDataList size : %u \n",memDataList.size());
	//printf("reMemRead Function! - End\n");
}

void renewal(){
	//printf("renewal Function! - Start\n");
	if(findDataList.size()){
		reMemRead();
		long listSize = memDataList.size();
		for(int i = 0;i < findDataList.size();i++) {
			findInfoStruct = findDataList.at(i);
			for (int j = 0; j < listSize; j++) {
				memInfoStruct = memDataList.at(j);

				if ((long) findInfoStruct.address == (long) memInfoStruct.address) {
					//printf("Find Value : %ld\n",(long) memInfoStruct.value);
					tempDataList.push_back(memInfoStruct);
				}
			}
		}
		findDataList.clear();
		findDataList = tempDataList;
		tempDataList.clear();
		memDataList.clear();
	}
	//printf("renewal Function! - End\n");
}

void ShowDataList(){
	//printf("ShowDataList Function! - Start\n");
	//printf("List Size : %d\n",findDataList.size());

	if(findDataList.size()){
		renewal();
	}

	//검색된 값을 출력합니다.
	for (int i = 0; i < findDataList.size(); i++) {
		findInfoStruct = findDataList.at(i);
		printf("address : %#lx, Value : %ld\n",(long)findInfoStruct.address,(long)findInfoStruct.value);
	}
	//printf("ShowDataList Function! - End\n");
}

void MemRead() {
	//printf("MemRead Function! - Start\n");
	char memFileName[30];
	int mem_fd;
	void *buf = malloc(MEMREADSIZE);

	//printf("pid : %d \n",privatePid);
	sprintf(memFileName, "/proc/%d/mem", privatePid);
	if(0 < (mem_fd = open(memFileName, O_RDONLY | O_LARGEFILE))){
		for (int i = 0; i < memReadAreaList.size(); i++) {
			maps = memReadAreaList.at(i);
			long staMemAddr = maps.staMemAddr;
			long endMemAddr = maps.endMemAddr;

			getValue(mem_fd,staMemAddr,endMemAddr,buf);
		}
	}else{
		printf("%s\n", errorMsgPid);
	}
	free(buf);
	close(mem_fd);
	//printf("MemRead Function! - End\n");
}
