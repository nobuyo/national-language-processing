main:
	cc -I/usr/local/include -L/usr/local/lib  `mecab-config --cflags` sample.c -o sample  `mecab-config --libs` -lmecab -liconv
