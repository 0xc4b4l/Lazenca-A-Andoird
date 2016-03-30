void cmpListView(){
	for(int i = 0;i < memCMPAreaList.size();i++){
		cmpdata = memCMPAreaList.at(i);
		printf("Address area : %#lx : \n",cmpdata.staMemAddr);
	}
}

int addressCMP(){
	char filename[30] = "/sdcard/Download/mapsarea.txt";
	char buff[BUFFSIZE];
	FILE *fp;
	char	*ptr;

	fp = fopen(filename, "r");

	long tmp;
	if (0 < fp) {
		while (fgets(buff, BUFFSIZE, fp) != NULL) {

			ptr = strtok(buff, "= ");
			for(int i = 0;ptr != NULL;i++ ){
				if(6 == i){
					//sscanf(prt,"%ld",&tmp)
					cmpdata.staMemAddr = atol(ptr);
					memCMPAreaList.push_back(cmpdata);
				}
				ptr = strtok(NULL, " ");
			}
		}
		fclose(fp);
	} else {
		printf("Cannot open \"%s\"\n", filename);
		fclose(fp);
		return 0;
	}
	return 1;
}

void addressSearch(){
	int cnt = 0;
	for(int i = 0;i < memCMPAreaList.size();i++){
		cmpdata = memCMPAreaList.at(i);
		for (int j = 0; j < mapsList.size(); j++) {
			maps = mapsList.at(j);
			if (cmpdata.staMemAddr == maps.staMemAddr) {
				cnt++;
				printf("%d. Address : %#lx => ", cnt, cmpdata.staMemAddr);
				printf(
						"staMemAddr : %lx,endMemAddr : %lx, strPerms : %s, offset : %lx, dev : %s, inode : %d, strPath : %s\n",
						maps.staMemAddr, maps.endMemAddr, maps.strPerms,
						maps.offset, maps.dev, maps.inode, maps.strPath);
			}
		}
	}
}
