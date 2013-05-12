
#pragma once


char * ReadFile (const char * fileName) {

	int size = 0;

	char * content = NULL;

	FILE * pFile = fopen(fileName, "rb");

	if (pFile == NULL) {
		perror(fileName);
	}

	fseek(pFile, 0, SEEK_END);
	size = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	if (size > 0) {
		content = new char [size + 1];
		fread(content, 1, size, pFile);
		content[size] = 0;
	}

	fclose(pFile);

	return content;
}
