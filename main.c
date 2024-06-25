#include <stdio.h>
#include <unistd.h>

int main(){
	char c, prev =0;
	while(read(STDIN_FILENO, &c, 1) == 1){
		if(prev == 'j' && c == 'q') break;
		prev = c;
	}
	return 0;
}
