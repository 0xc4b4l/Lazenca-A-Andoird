#include "Http.h"

int chkThreadCommand(char *s){
	if(!strcmp( ENABLE, strupr(s))){
		return 1;
	}
	if(!strcmp( DISABLE, strupr(s))){
		return 1;
	}
	printf("%s\n",errorMsgCmdErr);
	return 0;
}

//입력받은 PID가 사용하는 Thread를 모두 추출합니다.
void ThreadList(){
	//printf("ThreadList Function! - Start\n");
	char task[32];
	char tempPath[256];
	char cmdLine[256];

	struct dirent *entry;
	struct stat fstat;
	int count = 1;

	if(threadList.size() != 0){
		backupThrList = threadList;
	}
	threadList.clear();

	snprintf(task, sizeof(task), "/proc/%d/task", privatePid);
	DIR *dir = opendir(task);

	while((entry = readdir(dir)) != NULL)
	{
		lstat(entry->d_name, &fstat);
		if(!S_ISDIR(fstat.st_mode))
		{
			continue;
		}

		threadData.threadPid = atoi(entry->d_name);

		if(threadData.threadPid != 0){
			for(int i=0;i<backupThrList.size();i++){
					backthreadData = backupThrList.at(i);
					if(backthreadData.threadPid == threadData.threadPid){
						threadData.ThreadControlState = backthreadData.ThreadControlState;
						std::vector<THREADDATA>::iterator iter = backupThrList.begin();
						backupThrList.erase(iter + i);
					}
			}
			sprintf(tempPath, "/proc/%d/task/%d/cmdline", privatePid, threadData.threadPid);

			if(checkDir(tempPath)){
				getCmdLine(tempPath, cmdLine);
			}else{
				printf("%s : %d\n",errorMsgThread,threadData.threadPid);
			}

			strncpy(threadData.cmdLine, cmdLine, 256);
			threadList.push_back(threadData);
		}
	}
	backupThrList.clear();
	closedir(dir);
	//printf("ThreadList Function! - End\n");
}

//"ThreadList()"함수를 이용하여 추출한 Thread List를 출력합니다.
void showThreadList(){
	//printf("showThreadList Function! - Start\n");
	ThreadList();

	for(int i=0;i<threadList.size();i++){
		threadData = threadList.at(i);
		printf("Thread Count (%4d) - Pid : %7d , ThreadName : %s, Status : %d\n", i, threadData.threadPid , threadData.cmdLine ,threadData.ThreadControlState);
	}

	//Processlist 진행여부를 서버로 전송하기 위해 cmd값을 저장합니다.
	char cmd[MAX_PATH] = "THREADLIST";
	setParametaPrintlist(cmd);
	HttpGetFile(host,port,url);
	//printf("showThreadList Function! - End\n");
}

/*
 * 입력받은 PID의 Thread들을 제어합니다.
 * Option 설명
 *  - all : 전체 Thread Stop/Start
 *  - select : 입력받은 Thread id만 Stop/Start
 */
void ThreadControl(char *option, char *option2){
	//printf("ThreadControl Function! - Start\n");
	if (threadList.size() == 0) {
		ThreadList();
	}

	//Attach 된 프로세스의 모든 Thread를 Control(정지,시작) 합니다.
	if (!strcmp(ALL, strupr(option))) {
		if(chkThreadCommand(option2)){
			for (int i = 0; i < threadList.size(); i++) {
				threadData = threadList.at(i);
				std::vector<THREADDATA>::iterator iter = threadList.begin();
				//Thread를 정지
				//if(threadData.ThreadControlState == 0){
				if (!strcmp(ENABLE, strupr(option2))) {
					//Thread Control 값을 서버로 전송하기 위해 Thread id 값을 저장합니다.
					char tmp[MAX_PATH] = "ENABLEALL";
					setParametaThreadCtr(tmp, 0);

					threadData.ThreadControlState = 1;
					ptrace(PTRACE_ATTACH, threadData.threadPid, NULL, NULL);
					waitpid(threadData.threadPid, NULL, 0);
					//Thread를 실행
				} else if (!strcmp(DISABLE, strupr(option2))) {
					//Thread Control 값을 서버로 전송하기 위해 Thread id 값을 저장합니다.
					char tmp[MAX_PATH] = "DISABLEALL";
					setParametaThreadCtr(tmp, 0);

					threadData.ThreadControlState = 0;
					ptrace(PTRACE_DETACH, threadData.threadPid, NULL, NULL);
				} else {
					printf("%s\n", errorMsgCmdErr);
				}
				threadList.erase(iter + i);
				threadList.insert(iter + i, threadData);
			}
			HttpGetFile(host, port, url);
		}
		//Attach 된 프로세스의 개별적으로 Thread를 Control(정지,시작) 합니다.
	} else if (!strcmp(SELECT, strupr(option))) {
		int selectPid;
		sscanf(option2, "%d", &selectPid);

		char tempPath[256];
		sprintf(tempPath, "/proc/%d/task/%d/cmdline", privatePid, selectPid);
		if(checkDir(tempPath)){
			for (int i = 0; i < threadList.size(); i++) {
				threadData = threadList.at(i);

				if (threadData.threadPid == selectPid) {
					std::vector<THREADDATA>::iterator iter = threadList.begin();
					//정지되지 않음
					if (threadData.ThreadControlState == 0) {
						//Thread Control 값을 서버로 전송하기 위해 Thread id 값을 저장합니다.
						char tmp[MAX_PATH] = "ENABLE";
						setParametaThreadCtr(tmp, threadData.threadPid);

						threadData.ThreadControlState = 1;
						ptrace(PTRACE_ATTACH, threadData.threadPid, NULL, NULL);
					} else {
						//Thread Control 값을 서버로 전송하기 위해 Thread id 값을 저장합니다.
						char tmp[MAX_PATH] = "DISABLE";
						setParametaThreadCtr(tmp, threadData.threadPid);

						threadData.ThreadControlState = 0;
						ptrace(PTRACE_DETACH, threadData.threadPid, NULL, NULL);
					}
					threadList.erase(iter + i);
					threadList.insert(iter + i, threadData);
				}
			}
			HttpGetFile(host, port, url);
		}else{
			printf("%s",errorMsgThread);
		}

	} else {
		//ptrace(PTRACE_DETACH, privatePid, NULL, NULL);
		printf("%s\n",errorMsgCmdErr);
	}
	//printf("ThreadControl Function! - End\n");
}
