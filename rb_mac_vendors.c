// rb_mac_vendors.c

#include "rb_mac_vendors.h"
#include "librd/rdavl.h"
#include "librd/rdmem.h"



#define MAC_VENDOR_LIST_MAGIC 0x123456

struct mac_vendor_database
{
	rd_avl_t * avl;
};

struct mac_vendor_node{
#ifdef MAC_VENDOR_LIST_MAGIC
  uint64_t magic;
#endif
  uint64_t mac_prefix;
  char * name;
  rd_avl_node_t rd_avl_node;
};

static rd_memctx_t mac_vendor_mem;

static inline int mac_vendor_cmp(const void *_node1,const void *_node2)
{
  const struct mac_vendor_node * node1 = (const struct mac_vendor_node *)_node1;
  const struct mac_vendor_node * node2 = (const struct mac_vendor_node *)_node2;
#ifdef MAC_VENDOR_LIST_MAGIC
  assert(_node1!=NULL);
  assert(_node2!=NULL);
  assert(MAC_VENDOR_LIST_MAGIC == node1->magic);
  assert(MAC_VENDOR_LIST_MAGIC == node2->magic);
#endif
  return node2->mac_prefix > node1->mac_prefix ? 1 : node2->mac_prefix < node1->mac_prefix ? -1 : 0;
}

struct mac_vendor_database *rb_new_mac_vendor_db(const char * filename)
{
  rd_memctx_init(&mac_vendor_mem,"",RD_MEMCTX_F_TRACK);

  struct mac_vendor_database * db = malloc(sizeof(struct mac_vendor_database));

  db->avl = rd_avl_init(NULL,mac_vendor_cmp,0);
  if(!db->avl)
  	return NULL;

  char line[1024];

  FILE * file = fopen(filename,"r");
  if(file)
  {
  	while(NULL!=fgets(line,sizeof(line)-1,file))
  	{
  		if(line[0]!='\n' && line[0]!='#')
  		{
  			struct mac_vendor_node * node = rd_memctx_calloc(&mac_vendor_mem,1,sizeof(struct mac_vendor_node));
  			if(node)
  			{
  				#ifdef MAC_VENDOR_LIST_MAGIC
  				node->magic = MAC_VENDOR_LIST_MAGIC;
  				#endif
          char * strtok_aux;
  				char * tok1 = strtok_r(line,"|\n",&strtok_aux);
  				if(tok1)
  				{
            char * strtol_aux;
  					node-> mac_prefix = ((uint64_t) strtol(tok1, &strtol_aux, 16)) << 24;
          }
          char * tok2 = strtok_r(NULL,"|\n",&strtok_aux);
          if(tok2)
          {
            const size_t lentok = strlen(tok2);
            node->name = rd_memctx_malloc(&mac_vendor_mem,lentok+1);
            snprintf(node->name,lentok+1,"%s",tok2);
            #ifdef WARNINGS
            else
            {
              printf("no mac vendor in line: %s\n",line);
            }
            #endif
          }
  				RD_AVL_INSERT(db->avl,node,rd_avl_node);
  			}
  		}
  	}
  }

  if(file)
  	fclose(file);

  return db;
}

void rb_destroy_mac_vendor_db(struct mac_vendor_database *db)
{
  if(RD_MEMCTX_INITED(&mac_vendor_mem))
  {
  	rd_memctx_freeall(&mac_vendor_mem);
  	rd_memctx_destroy(&mac_vendor_mem);
  	rd_avl_destroy(db->avl);
  	free(db);
  }
}

const char * rb_find_mac_vendor(uint64_t mac,struct mac_vendor_database *list)
{
  const struct mac_vendor_node mac_to_find = {
    #ifdef MAC_VENDOR_LIST_MAGIC
    .magic = MAC_VENDOR_LIST_MAGIC,
    #endif
    .mac_prefix = mac&~0xFFFFFF,
    NULL};
  struct mac_vendor_node * node = rd_avl_find(list->avl,&mac_to_find,0 /* Don't lock */);
  return node?node->name:NULL;
}

