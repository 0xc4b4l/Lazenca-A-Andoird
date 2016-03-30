#include "openssl/sha256.h"

#define BUFFSIZE 1024
#define STDIN 0
#define STDOUT 1
#define BUFSIZE 30
#define MEMREADSIZE 4096

/*
 * Cheat Engine Command
 */
#define ATTACH "ATTACH"
#define SEARCH "SEARCH"
#define FUZZING "FUZZING"
#define WRITE "WRITE"
#define DATALOCK "DATALOCK"

/*
 * Thread All Control Command
 */
#define CONTROL "CONTROL"
#define ENABLE "ENABLE"
#define DISABLE "DISABLE"

/*
 * Thread Control Option
 */
#define ALL "ALL"
#define SELECT "SELECT"
char option[BUFSIZE] = "ALL", enable[BUFSIZE] = "ENABLE", disable[BUFSIZE] =
		"DISABLE";

/*
 * Print List
 */
#define LOCKLIST "LOCKLIST"
#define DATALIST "DATALIST"
#define THREADLIST "THREADLIST"
#define PROCESSLIST "PROCESSLIST"


#define CLEAR "CLEAR"
#define EXIT "EXIT"

/*
 * Fuzzing Option
 */
#define SMALLER "SMALLER"
#define LAGER "LAGER"
#define EQUAL "EQUAL"
#define NOTEQUAL "NOTEQUAL"

/*
 * SHA256
 */
#define SHA256_DIGEST_LENGTH	32

/*
 * Thread ID, 설정 상태,등을 저장하는 구조체 및 list입니다.
 */
typedef struct THREADDATAPAIR {
	int threadPid;
	int ThreadControlState;
	char cmdLine[256];
} THREADDATA;

std::vector<THREADDATA> threadList;
std::vector<THREADDATA> backupThrList;
THREADDATA threadData;
THREADDATA backthreadData;

/*
 * Memory에서 검색한 값을 저장하는 구조체 및 list 입니다.
 */
typedef struct ADDRDATAPAIR {
	void* address;
	void* value;
	bool lock;
} INFODATA;

std::vector<INFODATA> memDataList;
std::vector<INFODATA> findDataList;
std::vector<INFODATA> tempDataList;
std::vector<INFODATA> lockDatalist;

INFODATA memInfoStruct;
INFODATA findInfoStruct;
INFODATA tempInfoStruct;

/*
 * /proc/self/maps 파일의 내용을 읽어서 저장하는 구조체 및 list 입니다.
 */
typedef struct MAPSINFOPAIR {
	long staMemAddr;
	long endMemAddr;
	char strPerms[4];
	long offset;
	char dev[5];
	int inode;
	char strPath[100];
} MAPSDATA;

std::vector<MAPSDATA> mapsList;			//"/proc/self/maps" File 내용을 저
std::vector<MAPSDATA> memReadAreaList; 	//메모리를 읽어들일 영역
MAPSDATA maps;

/*
 * Test용
 */
typedef struct CMPINFOPAIR {
	long staMemAddr;
} CMPDATA;

std::vector<CMPDATA> memCMPAreaList;
CMPDATA cmpdata;

/*
 * Process List 저장용
 */
typedef struct PROCINFO {
	int pid;
	char cmdLine[256];
} PROCDATA;

std::vector<PROCDATA> procDataList;
PROCDATA procData;

/*
 * http에서 사용되는 변수
 */
#define	MAX_PATH		260
#define	BUFF_MAX		0x2000

char userid[BUFFSIZE] = "";
char uuidtosha[65] = "";
char appname[MAX_PATH] = "";
char cmd[MAX_PATH] = "";
char subCmd[MAX_PATH] = "";
int count = 0;
long num = 0;
char addr[MAX_PATH] = "";
int tpid = 0;

char url[MAX_PATH] = "/collect";
char host[MAX_PATH] = "greycheatinglab.com";
int port = 80;

pid_t privatePid = 0;

char *ptr;
char *ptrTop;
int chkClone = 0;

char errorMsgAttach[MAX_PATH] = "숫자만 입력해주세요.";
char errorMsgPid[MAX_PATH] = "프로세스가 존재하지 않습니다.";
char errorMsgSetPid[MAX_PATH] = "프로세스가 설정되지 않습니다.";
char errorMsgThread[MAX_PATH] = "쓰레드가 존재하지 않습니다.";
char errorMsgCmdErr[MAX_PATH] = "Error : 잘못된 입력!";
char command[MAX_PATH] = "lazenca$ ";

int flagCmd = 1;

char *strupr(char *s) {
	char *p = s;

	while (*p) {
		*p = (char) toupper(*p);
		p++;
	}

	return s;
}

void getCmdLine(char *file, char *buf) {
	FILE *srcFp;
	srcFp = fopen(file, "r");

	int bufSize = sizeof(buf);
	memset(buf, 0, bufSize);
	fgets(buf, 256, srcFp);
	fclose(srcFp);
}

void sha256(char *string) {
	char hash[SHA256_DIGEST_LENGTH];
	SHA256_CTX sha256;

	sha256_init(&sha256);
	sha256_update(&sha256, string, strlen((char*) string));
	sha256_final(&sha256, hash);

	for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
		sprintf(uuidtosha + (i * 2), "%02x", hash[i]);
	}
	uuidtosha[64] = 0;
}

int checkDir(char *fileName) {
	if (0 == access(fileName, F_OK)) {
		return 1;
	}else{
		return 0;
	}
}

int isStringDouble(char *s) {
  size_t size = strlen(s);
  if (size == 0) return 0; // 0바이트 문자열은 숫자가 아님

  for (int i = 0; i < (int) size; i++) {
    if (s[i] == '.' || s[i] == '-' || s[i] == '+') continue;
    if (s[i] < '0' || s[i] > '9') return 0; // 알파벳 등이 있으면 숫자 아님
  }

  return 1; // 그밖의 경우는 숫자임
}

int chkFuzzingCommand(char *s){
	if(!strcmp( SMALLER, strupr(s))){
		return 1;
	}
	if(!strcmp( LAGER, strupr(s))){
		return 1;
	}
	if(!strcmp( EQUAL, strupr(s))){
		return 1;
	}
	if(!strcmp( NOTEQUAL, strupr(s))){
		return 1;
	}
	printf("%s\n",errorMsgCmdErr);
	return 0;
}

int cmdSizeCheck(char *cmd){
	if(strlen(cmd) == 0){
		printf("%s\n",errorMsgCmdErr);
		return 0;
	}
	return 1;
}
