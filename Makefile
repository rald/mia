mia: main.c common.h ini.h strutil.h irc.h
	gcc main.c -o mia -lm -g -Wall -Wextra -pedantic

clean:
	rm mia
