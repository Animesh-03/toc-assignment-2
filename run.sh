set -e
gcc main.c -o main.out
./main.out basicC.txt
python3 tree.py