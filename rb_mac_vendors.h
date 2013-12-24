// rb_mac_vendors.h

#pragma once

#include <stdint.h>

struct mac_vendor_database;


struct mac_vendor_database *rb_new_mac_vendor_avl(const char * filename);
const char * rb_find_mac_vendor(uint64_t mac,struct mac_vendor_database *db);
void rb_destroy_mac_vendor_avl(struct mac_vendor_database *db);