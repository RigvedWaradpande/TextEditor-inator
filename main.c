#include "includefile.h"

struct termios orig_termios;

void disableRawMode(){
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode(){
	tcgetattr(STDIN_FILENO, &orig_termios);
	atexit(disableRawMode);

	struct termios raw = orig_termios;
	raw.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag |= (CS8);
	raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main(){
	enableRawMode();

	char c, prev = 0;
	while(read(STDIN_FILENO, &c, 1) == 1 && prev != 'j' && c != 'q'){
		if(iscntrl(c)) printf("%d\r\n", c);
		else printf("%d('%c')\r\n", c, c);		
		prev = c;
	}
	return 0;
}
