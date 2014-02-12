
DESTDIR=/opt/rb

LIBNAME=librb_mac_vendors
LIBVER=0

SRCS= rb_mac_vendors.c
HDRS= rb_mac_vendors.h
OBJS= $(SRCS:.c=.o)

CFLAGS=-O3 -Wall -Werror -fPIC -DNDEBUG 
#CFLAGS=-O0 -Wall -Werror -fPIC -g

# If you have librd in another location
CFLAGS+=-I/opt/rb/include
LDFLAGS+=-L/opt/rb/lib/

all: libs
libs: $(LIBNAME).so.$(LIBVER) $(LIBNAME).a

%.o: %.c
	$(CC) -MD -MP $(CFLAGS) -c $<

$(LIBNAME).so.$(LIBVER): $(OBJS)
	$(CC) -shared -Wl,-soname,$@ \
	$(OBJS) -o $@ $(LDFLAGS) -lrd -lrt -lz

$(LIBNAME).a: $(OBJS)
	$(AR) rcs $@ $(OBJS)

install: $(LIBNAME).so.$(LIBVER) $(LIBNAME).a
	install -t $(DESTDIR)/include $(HDRS)
	install -t $(DESTDIR)/lib $(LIBNAME).a
	install -t $(DESTDIR)/lib $(LIBNAME).so.$(LIBVER)
	cd $(DESTDIR)/lib && ln -sf $(LIBNAME).so.$(LIBVER) $(LIBNAME).so

example: example.c $(LIBNAME).a $(HDRS)
	$(CC) $(CFLAGS) -o $@ $< $(LIBNAME).a $(LDFLAGS) -lrd -lrt -lz

clean:
	rm -rf $(OBJS) example $(LIBNAME).a $(LIBNAME).so.$(LIBVER)
