#include "Util.h"
#include "UserId.h"


/*
 * List에 저장된 Maps파일 값들을 출력합니다.
 */
void MapsFileView(std::vector<MAPSDATA> &datalist){
	for(int i = 0;i < datalist.size();i++){
		maps = datalist.at(i);
		/*printf(
				"staMemAddr : %lx,endMemAddr : %lx, strPerms : %s, offset : %lx, dev : %s, inode : %d, strPath : %s\n",
				maps.staMemAddr, maps.endMemAddr, maps.strPerms, maps.offset,
				maps.dev, maps.inode, maps.strPath);
				*/
	}
}

void MapsListFiltering(){
	//printf("MapsListFiltering Function! - Start\n");
	for(int i = 0;i < mapsList.size();i++){
		maps = mapsList.at(i);
		//"rw-p" 권한이 있는 영역만 검색
		if(!strstr(maps.strPerms,"rw-p")){
			continue;
		//"/system/bin" 경로의 파일은 메모리 검색에 제외
		}else if(strstr(maps.strPath,"/system/bin")){
			continue;
		//"/system/lib" 경로의 파일은 메모리 검색에 제외
		}else if(strstr(maps.strPath,"/system/lib")){
			continue;
		//"/system/bin" 경로의 파일은 메모리 검색에 제외
		}else if(strstr(maps.strPath,"/system/vendor")){
			continue;
		//"/dev/ashmem" 경로의 파일은 메모리 검색에 제외
		}else if(strstr(maps.strPath,"/dev/ashmem")){
			//"/dev/ashmem/dalvik-heap"경로의 파일은 메모리 검색에 포함
			if(!strstr(maps.strPath,"/dev/ashmem/dalvik-heap")){
				continue;
			}
		}else if(strstr(maps.strPath,"[anon:libc_malloc]")){
			continue;
		}
		memReadAreaList.push_back(maps);
	}
	//printf("MapsListFiltering Function! - End\n");
}

void MapsFileRead(){
	//char filename[40] = "/sdcard/Download/maps.txt";
	char filename[20];
	char buff[BUFFSIZE];
	FILE *fp;

	// "/proc/<pid>/maps" 파일 열
	sprintf(filename, "/proc/%d/maps", privatePid);
	if (0 < (fp = fopen(filename, "r"))) {
		while (fgets(buff, BUFFSIZE, fp) != NULL) {
			MAPSDATA tmpMaps = { };
			sscanf(buff, "%lx-%lx %s %lx %s %d %s", &tmpMaps.staMemAddr,
					&tmpMaps.endMemAddr, tmpMaps.strPerms, &tmpMaps.offset,
					tmpMaps.dev, &tmpMaps.inode, tmpMaps.strPath);
			mapsList.push_back(tmpMaps);
		}
	} else {
		printf("%s\n", errorMsgPid);
	}
	fclose(fp);
	return;
}
