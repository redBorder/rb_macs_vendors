// example.c

#include "rb_mac_vendors.h"
#include <stdio.h>
#include <stdlib.h>

static void printUsage(char * progname)
{
	printf("Usage: %s database <mac1> <mac2> ...\n",progname);
}

int main(int argc,char *argv[])
{
	if(argc<2)
	{
		printUsage(argv[0]);
	}
	else
	{
		struct mac_vendor_database *db = rb_new_mac_vendor_avl(argv[1]);
		if(argc==2)
		{
			// known MACs
			printf("You should see 0010CE -> VOLAMP, LTD:\t%s\n",rb_find_mac_vendor(0x0010CE000000,db));
			printf("You should see 0010CE -> VOLAMP, LTD:\t%s\n",rb_find_mac_vendor(0x0010CE00FF00,db));
			printf("You should see F40F9B -> WAVELINK:\t%s\n",rb_find_mac_vendor(0xF40F9B00FF00,db));
		}
		else
		{
			int i;
			for(i=2;i<argc;++i)
			{
				const uint64_t mac_prefix = ((uint64_t) strtol(argv[i], NULL, 16));
				printf("%s vendor: %s\n",argv[i],rb_find_mac_vendor(mac_prefix,db));
			}
		}
		rb_destroy_mac_vendor_avl(db);
	}

	return 0;
}