#include "includefile.h"

struct termios orig_termios;

void die(const char *s){
	perror(s);
	exit(1);
}

void disableRawMode(){
        if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios)==-1) die("tcsetattr");
}

void enableRawMode(){
	if(tcgetattr(STDIN_FILENO, &orig_termios)==-1) die("tcgetattr");
	atexit(disableRawMode);

	struct termios raw = orig_termios;
	raw.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
	//ICRNL for carraige return new line && IXON for ^S and ^Q. All others are useless
	raw.c_oflag &= ~(OPOST);
	//OPOST seperates \r and \n
	raw.c_cflag |= (CS8);
	//CS8 is useless
	raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
	//ECHO for printing input && ICANON for canonical view && ISIG for ^C & ^Z && IEXTEN for ^V & ^O
	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 1;

	if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw)==-1) die("tcsetattr");
}

int main(){
	enableRawMode();

	while(1){
		char c = '\0', prev = '\0';
	        if(read(STDIN_FILENO, &c, 1)==-1 && errno != EAGAIN) die("read");
		if(iscntrl(c)) printf("%d\r\n", c);
		else printf("%d('%c')\r\n", c, c);
		if(prev == 'j' && c == 'q') break;		
		prev = c;
	}return 0;
}
