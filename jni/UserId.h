int getUUID() {
	char buff[BUFFSIZE];
	int fd;

	if (0 < (fd = open("/proc/sys/kernel/random/uuid", O_RDONLY))) {
		read(fd, userid, BUFFSIZE);
		int bufSize = strlen(userid);
		userid[bufSize-1] = 0;
		close(fd);
	} else {
		strncpy(userid, "Fail uuid", BUFFSIZE);
	}

	sha256 (userid);

	return 0;
}
