#include <cstdio>

using namespace std;

int main() {
	for(int i = 0; i < 256; i++) {
		if (i != 8 && i != 17 && !(10 <= i && i <= 14))
			printf("handler%d:\n\tpushq $0;\n\tpushq $%d;\n\tjmp start_handler;\n\n", i, i);
		else
			printf("handler%d:\n\tpushq $%d;\n\tjmp start_handler;\n\n", i, i);
	}
}
