#include <ruby.h>

#define TOKENS 37 // 26 alphabets + 10 numbers + dot symbol
#define TRUE 1
#define FALSE 0

struct trie {
  int matched;
  char* trace;
  struct trie* children[TOKENS];
};

VALUE kPrefixMachine = Qnil;
VALUE kPrefixMachineTrie = Qnil;

void Init_prefix_machine_ext();
VALUE method_prefix_machine_initialize(VALUE self);

// private methods
VALUE method_private_prefix_machine_insert_trie(VALUE self, VALUE prefix);
VALUE method_private_prefix_machine_match(VALUE self, VALUE str);

// internal methods
size_t internal_compact_tokens(char c);
void internal_prefix_machine_insert_trie(struct trie* node, char* rule, size_t len, size_t offset);
char* internal_prefix_machine_match(struct trie* node, char* str, size_t len, size_t offset);

struct trie* internal_trie_get(VALUE wrapped);
void internal_trie_free(void* ptr);
size_t internal_trie_size(const void* ptr);

static const rb_data_type_t type_trie = {
  .wrap_struct_name = "trie",
  .function = {
    .dmark = NULL,
    .dfree = internal_trie_free,
    .dsize = internal_trie_size,
  },
  .data = NULL,
  .flags = RUBY_TYPED_FREE_IMMEDIATELY,
};
