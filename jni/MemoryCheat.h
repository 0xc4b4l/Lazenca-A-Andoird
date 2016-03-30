//#include "MemRead.h"
//#include <unistd.h>
//#include <sys/ptrace.h>
//#include <stdio.h>

#include "MemRead.h"
#include "Comparator.h"
#include "ProcessControl.h"
//#include "Http.h"

void ListClear() {
	//printf("ListClear Function! - Start\n");
	memDataList.clear();
	findDataList.clear();
	lockDatalist.clear();
	mapsList.clear();
	memReadAreaList.clear();
	count = 0;

	//Processlist 진행여부를 서버로 전송하기 위해 cmd값을 저장합니다.
	char cmd[MAX_PATH] = "CLEAR";
	setParametaPrintlist(cmd);
	HttpGetFile(host, port, url);

	//printf("ListClear Function! - End\n");
}

void FuzzingInit() {
	//printf("FuzzingInit Function! - Start\n");
	ListClear();
	//printf("FuzzingInit Function! - End\n");
}

void MemorySearch(char *command) {
	//printf("MemorySearch Function! - Start\n");
	//입력된 값이 있는지 확인
	if(cmdSizeCheck(command)){
		long value;

		sscanf(command, "%ld", &value);

		setParametaSearch(value);
		HttpGetFile(host, port, url);

		//printf("findDataList.size() : %u\n", findDataList.size());
		if (!findDataList.size()) {
			MapsFileRead();
			MapsListFiltering();
			ThreadControl(option, enable);
			MemRead();

		} else {
			ThreadControl(option, enable);
			reMemRead();
		}
		ThreadControl(option, disable);
		NumberComparator(value);
	}
	//printf("oriMemArray size : %d\n",oriMemArray.size());
	printf("Find data size : %d\n", findDataList.size());
	//printf("memDataList size : %d\n", memDataList.size());
	//printf("MemorySearch Function! - End\n");
}

void MemoryFuzzing(char *command2) {
	//printf("MemoryFuzzing Function! - Start\n");

	//검색하려는 값을 서버로 전송하기 위해 검색 값을 저장합니다.
	setParametaFuzzing(command2);
	HttpGetFile(host, port, url);

	if (!findDataList.size()) {
		MapsFileRead();
		MapsListFiltering();
		ThreadControl(option, enable);
		MemRead();
		findDataList = memDataList;
		memDataList.clear();
		ThreadControl(option, disable);
		//printf("findDataList size : %d\n", findDataList.size());
	} else {
		if (chkFuzzingCommand(command2)) {
			ThreadControl(option, enable);
			reMemRead();
			ThreadControl(option, disable);

			if (!strcmp(SMALLER, strupr(command2))) {
				SmallerComparator();
			} else if (!strcmp(LAGER, strupr(command2))) {
				LagerComparator();
			} else if (!strcmp(EQUAL, strupr(command2))) {
				EqualComparator();
			} else if (!strcmp(NOTEQUAL, strupr(command2))) {
				NotEqualComparator();
			}
		}
	}
	printf("Find data size : %d\n", findDataList.size());
	//printf("MemoryFuzzing Function! - End\n");
}

void MemoryWrite(char *cAddress, char *cValue) {
	//printf("MemoryWrite Function! - Start\n");

	//입력된 값이 있는지 확인
	if(cmdSizeCheck(cAddress)){
		if(cmdSizeCheck(cValue)){
			int status;
			//long *value;
			long tmpValue;
			long address;
			char memFileName[30];
			int mem_fd;

			//sscanf(cAddress, "%ld", &address);
			address = strtoul(cAddress, NULL, 16);
			//printf("Address : %ld\n",address);
			sscanf(cValue, "%ld", &tmpValue);
			sprintf(memFileName, "/proc/%d/mem", privatePid);

			//변경하려는 값을 서버로 전송하기 위해 변경 값,주소 값을 저장합니다.
			setParametaWrite(cAddress, tmpValue);
			HttpGetFile(host, port, url);

			ptrace(PTRACE_ATTACH, privatePid, NULL, NULL);
			waitpid(privatePid, &status, WUNTRACED);

			if (0 < (mem_fd = open(memFileName, O_WRONLY | O_LARGEFILE))) {
				lseek(mem_fd, address, SEEK_SET);
				write(mem_fd, &tmpValue, sizeof(tmpValue));
				close(mem_fd);
			} else {
				printf("%s\n", errorMsgPid);
			}
			ptrace(PTRACE_DETACH, privatePid, NULL, NULL);
			renewal();
		}
	}
	//printf("MemoryWrite Function! - End\n");
}

/*
 * Memory Lock을 위한 함수
 */
static int ValueLockProc(void *arg) {
	//printf("ValueLockProc Function! - Start\n");
	//입력된 값이 있는지 확인
	struct timespec reqtime;
	int status;
	reqtime.tv_sec = 0;
	reqtime.tv_nsec = 100000000;

	sigset_t sigset;

	/* SIGINT에 대해 블록 설정하고 이전 블록화 된 시그널 집합을 oldset에 저장 */
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGSEGV);

	int listSize = lockDatalist.size();
	threadData = threadList.at(3);

	while (!lockDatalist.empty()) {
		//printf(">>>lockDataSize : %d\n",lockDatalist.empty());
		for (int i = 0; i < listSize; i++) {
			tempInfoStruct = lockDatalist.at(i);
			//printf("Address : %#lx, Data : %ld\n",tempInfoStruct.address,tempInfoStruct.value);
			ptrace(PTRACE_ATTACH, threadData.threadPid, NULL, NULL);
			waitpid(threadData.threadPid, &status, WUNTRACED);
			ptrace(PTRACE_PEEKDATA, threadData.threadPid,
					(void *) tempInfoStruct.address,
					(void *) tempInfoStruct.value);
			ptrace(PTRACE_POKEDATA, threadData.threadPid,
					(void *) tempInfoStruct.address,
					(void *) tempInfoStruct.value);
			ptrace(PTRACE_DETACH, threadData.threadPid, NULL, NULL);
			nanosleep(&reqtime, &reqtime);
		}
	}

	if (lockDatalist.empty()) {
		chkClone = 0;
		/* oldset의 시그널들이 블록화 된 시그널 집합으로 교체 */
		sigprocmask(SIG_SETMASK, &sigset, NULL);
		munmap(ptr, sizeof(ptr));
	}
	//printf("ValueLockProc Function! - End\n");
	return 0;
}

void ShowLockDataList(){
	//printf("ShowLockDataList Function! - Start\n");
	if(findDataList.size()){
		renewal();
	}

	//검색된 값을 출력합니다.
	for (int i = 0; i < lockDatalist.size(); i++) {
		tempInfoStruct = lockDatalist.at(i);
		printf("address : %#lx, Value : %ld\n",(long)tempInfoStruct.address,(long)tempInfoStruct.value);
	}
	//printf("ShowLockDataList Function! - End\n");
}

/*
 * Lock 설정이 되어 있는 Address가 있는지 확인하고, Address가 있다면 List에서 제거
 */
int lockDataListValueCheck(long address){
	for (int i = 0; i < lockDatalist.size(); i++) {
		tempInfoStruct = lockDatalist.at(i);
		if ((long) tempInfoStruct.address == address) {
			std::vector<INFODATA>::iterator iter = lockDatalist.begin();
			lockDatalist.erase(iter + i);
			return 0;
		}
	}
	return 1;
}

void DataLock(char *cAddress) {
	//printf("DataLock Function! - Start\n");
	if(cmdSizeCheck(cAddress)){
		long address;
		address = strtoul(cAddress, NULL, 16);

		char inCmd[MAX_PATH];
		if (lockDataListValueCheck(address)) {
			//Lock설정 주소 값을 서버로 전송하기 위해 주소 값을 저장합니다.
			strncpy(inCmd, "LOCK", 4);
			setParametaLock(inCmd, cAddress);
			HttpGetFile(host, port, url);

			//새로운 Address가 입력되었다면 검색 list에서 값을 찾아서 Locklist에 추가 합니다.
			for (int i = 0; i < findDataList.size(); i++) {
				findInfoStruct = findDataList.at(i);
				if ((long) findInfoStruct.address == address) {
					lockDatalist.push_back(findInfoStruct);
				}
			}
		} else {
			//Lock설정 주소 값을 서버로 전송하기 위해 주소 값을 저장합니다.
			strncpy(inCmd, "UNLOCK", 6);
			setParametaLock(inCmd, cAddress);
			HttpGetFile(host, port, url);
		}

		//메모리에 값을 고정 시키기 위해 "ValueLockProc" 함수를 자식 프로세스로 생성합니다.
		if (chkClone == 0) {
			ptr = (char *) mmap(NULL, 1048576, PROT_READ | PROT_WRITE,
					MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);
			if (ptr != (void *) -1) {
				mprotect(ptr, MEMREADSIZE, PROT_NONE);

				ptrTop = ptr + 1048576;
				chkClone = clone(ValueLockProc, ptrTop,CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND| CLONE_THREAD | CLONE_SYSVSEM, NULL);
				if (chkClone == -1) {
					perror(">>>clone failed\n");
					return;
				}
			} else {
				perror(">>>mmap failed\n");
				return;
			}
		}
	}
	//printf("DataLock Function! - End\n");
}

