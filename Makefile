TARGETS=gamelife
LIBS=`pkg-config --cflags --libs gtk+-2.0`
all: $(TARGETS)

%:%.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

install: all
	/usr/bin/install -m 755 $(TARGETS) /usr/bin

uninstall:
	cd /usr/bin/; /bin/rm $(TARGETS)

clean:
	/bin/rm $(TARGETS)
