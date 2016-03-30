#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>

#include <stdio.h>
#include <sched.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include <vector>
#include <ctype.h>
#include <dirent.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "MemoryCheat.h"


/*
 *	SEARCH
 *	FUZZING
 *	- small
 *	- lager
 *	- equal
 *	- notequal
 *	WRITE
 *	PROCESS(Process List)
 *	THREADLIST
 *	ATTACH
 *	CONTROL
 *	- STOP
 *	- START
 *	SHOWLIST(Data List)
 *	CLEAR
 *	EXIT
 */
int main(int argc, const char* argv[]) {

	getUUID();

	char message[BUFSIZE];
	char command1[BUFSIZE] = { '\0', }, command2[BUFSIZE] = { '\0', },command3[BUFSIZE] = { '\0', };

	while (1) {
		printf("Lazenca$ ");
		fgets(message, BUFSIZE, stdin);
		fflush (stdin);

		//입력값이 있는지 없는지 확인
		if (strlen(message) > 1) {
			sscanf(message, "%s%s%s", command1, command2, command3);
			/*printf("command1 : %s, command2 : %s, command3 : %s\n",
			 strupr(command1), command2, command3);*/

			//Process Attach
			if (!strcmp(ATTACH, strupr(command1))) {
				ListClear();
				ProcAttach(command2);
				continue;
				//Process List
			} else if (!strcmp(PROCESSLIST, strupr(command1))) {
				showProcessList();
				continue;
				//Process Exit
			} else if (!strcmp(EXIT, strupr(command1))) {
				exit(0);
			}

			char tempPath[256];
			sprintf(tempPath, "/proc/%d", privatePid);
			if (checkDir(tempPath)) {
				//검색
				if (!strcmp(SEARCH, strupr(command1))) {
					MemorySearch(command2);
					//퍼징
				} else if (!strcmp(FUZZING, strupr(command1))) {
					MemoryFuzzing(command2);
					//값 변경
				} else if (!strcmp(WRITE, strupr(command1))) {
					MemoryWrite(command2, command3);
					//Thread List
				} else if (!strcmp(THREADLIST, strupr(command1))) {
					showThreadList();
					//Process Control
				} else if (!strcmp(CONTROL, strupr(command1))) {
					ThreadControl(command2, command3);
					//Data List
				} else if (!strcmp(DATALIST, strupr(command1))) {
					ShowDataList();
					//Data List Clear
				} else if (!strcmp(CLEAR, strupr(command1))) {
					ListClear();
					//Data Lock
				} else if (!strcmp(DATALOCK, strupr(command1))) {
					DataLock(command2);
				} else if (!strcmp(LOCKLIST, strupr(command1))) {
					ShowLockDataList();
				} else {
					printf("Main Command Input Error!\n");
				}
			} else {
				ListClear();
				printf("%s\n", errorMsgPid);
			}
		}
	}
	return 0;
}
