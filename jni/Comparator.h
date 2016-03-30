//FUZZING - 이전에 검색된 값과 비교하여 값이 감소한 것들만 저장
void SmallerComparator() {
	//printf("SmallerComparator Function! - Start\n");
	if(memDataList.size() == findDataList.size()){
		for (int i = 0; i < findDataList.size(); i++) {
			findInfoStruct = findDataList.at(i);
			memInfoStruct = memDataList.at(i);
			if (memInfoStruct.address == findInfoStruct.address) {
				//이전에 검색한 값이 새로 검색된 값보다 크면 리스트에 추가
				if (findInfoStruct.value > memInfoStruct.value) {
					tempDataList.push_back(memInfoStruct);
				}
			}
		}
	} else {
		int temp = 0;
		for(int i = 0; i < findDataList.size();i++){
			findInfoStruct = findDataList.at(i);
			memInfoStruct = memDataList.at(i + temp);
			if (memInfoStruct.address == findInfoStruct.address) {
				//이전에 검색한 값이 새로 검색된 값보다 작으면 리스트에 추가
				if (findInfoStruct.value > memInfoStruct.value) {
					tempDataList.push_back(memInfoStruct);
				}
			}else{
				for(;memInfoStruct.address <= findInfoStruct.address;temp++){
					memInfoStruct = memDataList.at(i + temp);
					if (memInfoStruct.address == findInfoStruct.address) {
						//이전에 검색한 값이 새로 검색된 값보다 작으면 리스트에 추가
						if (findInfoStruct.value > memInfoStruct.value) {
							tempDataList.push_back(memInfoStruct);
						}
					}
				}
			}
		}
	}

	findDataList.clear();
	findDataList = tempDataList;
	tempDataList.clear();
	memDataList.clear();

	//printf("find size : %u \n",findDataList.size());
	//printf("SmallerComparator Function! - End\n");
}

//FUZZING - 이전에 검색된 값과 비교하여 값이 증가한 것들만 저장
void LagerComparator() {
	//printf("LagerComparator Function! - Start\n");
	if(memDataList.size() == findDataList.size()){
		for(int i = 0; i < findDataList.size();i++){
			findInfoStruct = findDataList.at(i);
			memInfoStruct = memDataList.at(i);
			if (memInfoStruct.address == findInfoStruct.address) {
				//이전에 검색한 값이 새로 검색된 값보다 작으면 리스트에 추가
				if (findInfoStruct.value < memInfoStruct.value) {
					tempDataList.push_back(memInfoStruct);
				}
			}
		}
	}else{
		int temp = 0;
		for(int i = 0; i < findDataList.size();i++){
			findInfoStruct = findDataList.at(i);
			memInfoStruct = memDataList.at(i + temp);
			if (memInfoStruct.address == findInfoStruct.address) {
				//이전에 검색한 값이 새로 검색된 값보다 작으면 리스트에 추가
				if (findInfoStruct.value < memInfoStruct.value) {
					tempDataList.push_back(memInfoStruct);
				}
			}else{
				for(;memInfoStruct.address <= findInfoStruct.address;temp++){
					memInfoStruct = memDataList.at(i + temp);
					if (memInfoStruct.address == findInfoStruct.address) {
						//이전에 검색한 값이 새로 검색된 값보다 작으면 리스트에 추가
						if (findInfoStruct.value < memInfoStruct.value) {
							tempDataList.push_back(memInfoStruct);
						}
					}
				}
			}
		}
	}

	findDataList.clear();
	findDataList = tempDataList;
	tempDataList.clear();
	memDataList.clear();

	//printf("find size : %u \n",findDataList.size());
	//printf("LagerComparator Function! - End\n");
}

//FUZZING - 이전에 검색된 값과 비교하여 값이 같은 것들만 저장
void EqualComparator() {
	//printf("EqualComparator Function! - Start\n");
	if(memDataList.size() == findDataList.size()){
		for (int i = 0; i < findDataList.size(); i++) {
			findInfoStruct = findDataList.at(i);
			memInfoStruct = memDataList.at(i);
			if (memInfoStruct.address == findInfoStruct.address) {
				//이전에 검색한 값과 새로 검색된 값이 같으면 리스트에 추가
				if (findInfoStruct.value == memInfoStruct.value) {
					tempDataList.push_back(memInfoStruct);
				}
			}
		}
	}else{
		int temp = 0;
		for(int i = 0; i < findDataList.size();i++){
			findInfoStruct = findDataList.at(i);
			memInfoStruct = memDataList.at(i + temp);
			if (memInfoStruct.address == findInfoStruct.address) {
				//이전에 검색한 값이 새로 검색된 값보다 작으면 리스트에 추가
				if (findInfoStruct.value == memInfoStruct.value) {
					tempDataList.push_back(memInfoStruct);
				}
			}else{
				for(;memInfoStruct.address <= findInfoStruct.address;temp++){
					memInfoStruct = memDataList.at(i + temp);
					if (memInfoStruct.address == findInfoStruct.address) {
						//이전에 검색한 값이 새로 검색된 값보다 작으면 리스트에 추가
						if (findInfoStruct.value == memInfoStruct.value) {
							tempDataList.push_back(memInfoStruct);
						}
					}
				}
			}
		}
	}

	findDataList.clear();
	findDataList = tempDataList;
	tempDataList.clear();
	memDataList.clear();

	//printf("find size : %u \n",findDataList.size());
	//printf("EqualComparator Function! - End\n");
}

//FUZZING - 이전에 검색된 값과 비교하여 값이 다른 것들만 저장
void NotEqualComparator() {
	//printf("NotEqualComparator Function! - Start\n");
	if(memDataList.size() == findDataList.size()){
		for (int i = 0; i < findDataList.size(); i++) {
			findInfoStruct = findDataList.at(i);
			memInfoStruct = memDataList.at(i);
			if (memInfoStruct.address == findInfoStruct.address) {
				//이전에 검색한 값과 새로 검색된 값이 다르면 리스트에 추가
				if (findInfoStruct.value != memInfoStruct.value) {
					tempDataList.push_back(memInfoStruct);
				}
			}
		}
	} else{
		int temp = 0;
		for(int i = 0; i < findDataList.size();i++){
			findInfoStruct = findDataList.at(i);
			memInfoStruct = memDataList.at(i + temp);
			if (memInfoStruct.address == findInfoStruct.address) {
				//이전에 검색한 값이 새로 검색된 값보다 작으면 리스트에 추가
				if (findInfoStruct.value != memInfoStruct.value) {
					tempDataList.push_back(memInfoStruct);
				}
			}else{
				for(;memInfoStruct.address <= findInfoStruct.address;temp++){
					memInfoStruct = memDataList.at(i + temp);
					if (memInfoStruct.address == findInfoStruct.address) {
						//이전에 검색한 값이 새로 검색된 값보다 작으면 리스트에 추가
						if (findInfoStruct.value != memInfoStruct.value) {
							tempDataList.push_back(memInfoStruct);
						}
					}
				}
			}
		}
	}

	findDataList.clear();
	findDataList = tempDataList;
	tempDataList.clear();
	memDataList.clear();

	//printf("find size : %u \n",findDataList.size());
	//printf("NotEqualComparator Function! - End\n");
}

//SEARCH - 이전에 검색된 값에서 입력된 값과 같은 것들만 저장
void NumberComparator(long value) {
	//printf("NumberComparator Function! - Start\n");
	//std::vector<INFODATA>::iterator iter = memDataList.begin();

	if (findDataList.size()) {
		int temp = 0;
		for (int i = 0; i < findDataList.size(); i++) {
			findInfoStruct = findDataList.at(i);
			memInfoStruct = memDataList.at(i + temp);
			if (memInfoStruct.address == findInfoStruct.address) {
				//이전에 검색한 값이 새로 검색된 값보다 작으면 리스트에 추가
				if (value == (long)memInfoStruct.value) {
					tempDataList.push_back(memInfoStruct);
				}
			} else {
				for (; memInfoStruct.address <= findInfoStruct.address; temp++) {
					memInfoStruct = memDataList.at(i + temp);
					if (memInfoStruct.address == findInfoStruct.address) {
						//이전에 검색한 값이 새로 검색된 값보다 작으면 리스트에 추가
						if (value == (long)memInfoStruct.value) {
							tempDataList.push_back(memInfoStruct);
						}
					}
				}
			}
		}
	} else {
		//printf("findDataList Size : %d\n", findDataList.size());
		for (int i = 0; i < memDataList.size(); i++) {
			memInfoStruct = memDataList.at(i);
			if (value == (long) memInfoStruct.value) {
				//printf("Find Value : %ld\n", (long) memInfoStruct.value);
				tempDataList.push_back(memInfoStruct);
			}
		}

	}

	findDataList.clear();
	findDataList = tempDataList;
	tempDataList.clear();
	memDataList.clear();
	//printf("NumberComparator Function! - End\n");
}
