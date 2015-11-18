
DESTDIR=/opt/rb

LIBNAME=librb_mac_vendors
LIBVER=0

SRCS= rb_mac_vendors.c
HDRS= rb_mac_vendors.h
OBJS= $(SRCS:.c=.o)

CPPFLAGS+=-O3 -Wall -Werror -fPIC -DNDEBUG
#CFLAGS=-O0 -Wall -Werror -fPIC -g

all: libs
libs: $(LIBNAME).so.$(LIBVER) $(LIBNAME).a

%.o: %.c
	$(CC) -MD -MP $(CPPFLAGS) -c $<

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
	$(CC) $(CPPFLAGS) -o $@ $< $(LIBNAME).a $(LDFLAGS) -lrd -lrt -lz

clean:
	rm -rf $(OBJS) example $(LIBNAME).a $(LIBNAME).so.$(LIBVER)
