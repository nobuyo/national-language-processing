%: %.c
	cc -I/usr/local/include -L/usr/local/lib  `mecab-config --cflags` $< -o $@ `mecab-config --libs` -lmecab -liconv