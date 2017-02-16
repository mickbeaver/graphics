#include <cassert>
#include <climits>
#include <cstdio>

namespace FileUtil {
    char* loadFileAsString(const char* filename) {
        int ret;

        FILE* const inputObj = fopen(filename, "rb");
        assert(inputObj != NULL);
        ret = fseek(inputObj, 0, SEEK_END);
        assert(ret == 0);
        long const fileLength = ftell(inputObj);
        assert(fileLength > 0 && fileLength != LONG_MAX);
        ret = fseek(inputObj, 0, SEEK_SET);
        assert(ret == 0);
        char* const buffer = new char[fileLength + 1];
    
        size_t totalBytesRead = 0;
        do {
            size_t const currentBytesRead = fread(buffer, 1, (unsigned long)fileLength, inputObj);
            if (currentBytesRead == 0 && ferror(inputObj) != 0) {
                fprintf(stderr, "Error reading from %s. errno is %d\n", filename, errno);
                assert(0);
            }
            totalBytesRead += currentBytesRead;
        } while (totalBytesRead < (size_t)fileLength);

        fclose(inputObj);
        buffer[fileLength] = '\0';

        return buffer;
    }
}
