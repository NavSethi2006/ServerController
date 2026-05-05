rm main
gcc src/*.c src/utils/*.c -o main -lm -pthread -lssl -lcrypto
./main