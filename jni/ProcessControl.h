//입력받은 PID 값을 저장합니다.
void setPid(int pid) {
	privatePid = pid;

	//Appname 을 서버로 전송하기 위해 Attach된 프로세스의 이름을 저장합니다.
	setParametaAppName();
	HttpGetFile(host,port,url);
}

//저장된 PID 값을 리턴합니다.
pid_t getPid() {
	return privatePid;
}

//문자로 전달받은 PID를 형병환하여 저장합니다.
void ProcAttach(char *cPid) {
	//printf("ProcAttach Function! - Start\n");
	int pid;
	//입력된 값이 있는지 확인
	if(cmdSizeCheck(cPid)){
		//입력된 값이 숫자인지 확인
		if(isStringDouble(cPid)){
			sscanf(cPid, "%d", &pid);
			char tempPath[256];
			sprintf(tempPath, "/proc/%d", pid);
			if (checkDir(tempPath)) {
				privatePid = pid;

				//Appname 을 서버로 전송하기 위해 Attach된 프로세스의 이름을 저장합니다.
				setParametaAppName();
				HttpGetFile(host, port, url);
			}else{
				printf("%s\n", errorMsgPid);
			}
		}else{
			printf("%s\n",errorMsgAttach);
		}
	}
	//printf("ProcAttach Function! - End\n");
}

//해당 OS에도 동작하는 Process를 출력합니다.
void ProcessList() {
	//printf("ProcessList Function!\n");

	procDataList.clear();

	DIR *dir;                       //  /proc/pid/ 를 가리킬 DIR* 변수
	struct dirent *entry;        	// 각 파일의 inode를 통해 파일을 선택할 dirent 구조체
	struct stat fileStat;          	// 파일의 정보를 담는 구조체

	int pid;
	char cmdLine[256];
	char tempPath[256];

	dir = opendir("/proc");

	while ((entry = readdir(dir)) != NULL) {	//   /proc에 존재하는 파일들을 차례대로 읽습니다.
		lstat(entry->d_name, &fileStat);	// DIR*가 가리키는 파일의 state 정보를 가져온다.

		if (!S_ISDIR(fileStat.st_mode))				//디렉토리인지 확인한다.
			continue;

		pid = atoi(entry->d_name);				// 프로세스(디렉토리)인것을 확인하면, 숫자로 반환한다.
		if (pid <= 0)
			continue;								// 숫자가 아니라면 다시 continue;

		sprintf(tempPath, "/proc/%d/cmdline", pid);
		getCmdLine(tempPath, cmdLine);

		procData.pid = pid;
		strncpy(procData.cmdLine, cmdLine, 256);
		procDataList.push_back(procData);
	}
	closedir(dir);

	//Processlist 진행여부를 서버로 전송하기 위해 cmd값을 저장합니다.
	char cmd[MAX_PATH] = "PROCESSLIST";
	setParametaPrintlist(cmd);
	HttpGetFile(host,port,url);
	//printf("ProcessList Function - End!\n");
}

//"ProcessList()"함수를 이용하여 추출한 Process List를 출력합니다.
void showProcessList() {
	//printf("showProcessList Function! - Start\n");
	//매번 Process 목록을 조회하고 데이터를 갱신합니다.
	ProcessList();

	for (int i = 0; i < procDataList.size(); i++) {
		procData = procDataList.at(i);
		printf("Process Count : [%d], Pid : (%d) , Path : %s\n", i, procData.pid,
				procData.cmdLine);
	}

	//printf("showProcessList Function! - End\n");
}
