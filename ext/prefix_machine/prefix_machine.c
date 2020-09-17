#include "prefix_machine.h"

void Init_prefix_machine_ext() {
  kPrefixMachine = rb_define_class("PrefixMachine", rb_cObject);
  kPrefixMachineTrie = rb_define_class("PrefixMachineTrie", rb_cObject);

  rb_define_method(kPrefixMachine, "initialize", method_prefix_machine_initialize, 0);
  rb_define_private_method(kPrefixMachine, "trie_insert", method_private_prefix_machine_insert_trie, 1);
  rb_define_private_method(kPrefixMachine, "trie_match", method_private_prefix_machine_match, 1);
}

VALUE method_prefix_machine_initialize(VALUE self) {
  struct trie* trie = xmalloc(sizeof(struct trie));
  char* empty_str = xmalloc(sizeof(char));
  empty_str[0] = '\0';
  trie->trace = empty_str;
  trie->matched = FALSE;

  for (size_t i = 0; i < TOKENS; i++) {
    trie->children[i] = NULL;
  }

  VALUE rules = TypedData_Wrap_Struct(kPrefixMachineTrie, &type_trie, trie);
  rb_iv_set(self, "@rules", rules);
  rb_iv_set(self, "@count", SIZET2NUM(0));

  return self;
}

VALUE method_private_prefix_machine_insert_trie(VALUE self, VALUE prefix) {
  struct trie* root = internal_trie_get(rb_iv_get(self, "@rules"));
  char* prefix_str = StringValueCStr(prefix);
  size_t len = strlen(prefix_str);
  internal_prefix_machine_insert_trie(root, prefix_str, len, 0);
  rb_iv_set(self, "@count", SIZET2NUM(NUM2SIZET(rb_iv_get(self, "@count")) + 1));
  return Qnil;
}

VALUE method_private_prefix_machine_match(VALUE self, VALUE str) {
  struct trie* root = internal_trie_get(rb_iv_get(self, "@rules"));
  char* str_ptr = StringValueCStr(str);
  size_t len = strlen(str_ptr);
  char* result = internal_prefix_machine_match(root, str_ptr, len, 0);

  if (result == NULL) {
    return Qnil;
  }

  return rb_str_new2(result);
}

size_t internal_compact_tokens(char c) {
  if (c >= '0' && c <= '9') {
    return c - '0';
  }
  
  if (c >= 'a' && c <= 'z') {
    return c - 'a' + 11;
  }
  
  if (c == '.') {
    return 36;
  }
  
  rb_raise(rb_eNoMatchingPatternError, "Pattern Not Supported.");
}

void internal_prefix_machine_insert_trie(struct trie* node, char* rule, size_t len, size_t offset) {
  // Check boundary first
  if (len == offset) {
    node->matched = TRUE;
    return;
  }

  char c = rule[offset];
  size_t index = internal_compact_tokens(c);
  if (node->children[index] != NULL) {
    return internal_prefix_machine_insert_trie(node->children[index], rule, len, offset + 1);
  }

  // Create the node
  struct trie* new_node = xmalloc(sizeof(struct trie));
  char* trace = xmalloc(sizeof(char) * (offset + 2));
  memcpy(trace, rule, offset);
  trace[offset] = c;
  trace[offset + 1] = '\0'; // NUL-terminated
  new_node->trace = trace;
  new_node->matched = FALSE;

  for (size_t i = 0; i < TOKENS; i++) {
    new_node->children[i] = NULL;
  }

  node->children[index] = new_node;

  return internal_prefix_machine_insert_trie(new_node, rule, len, offset + 1);
}

char* internal_prefix_machine_match(struct trie* node, char* str, size_t len, size_t offset) {
  if (node->matched == TRUE) {
    return node->trace;
  }

  if (len == offset) {
    return NULL;
  }

  size_t index = internal_compact_tokens(str[offset]);
  
  if (node->children[index] == NULL) {
    return NULL;
  }

  return internal_prefix_machine_match(node->children[index], str, len, offset + 1);
}

struct trie* internal_trie_get(VALUE wrapped) {
  struct trie* ptr;
  TypedData_Get_Struct(wrapped, struct trie, &type_trie, ptr);
  return ptr;
}

void internal_trie_free(void* ptr) {
  struct trie* root = (struct trie*) ptr;
  for (size_t i = 0; i < TOKENS; i++) {
    if (root->children[i] != NULL) {
      internal_trie_free(root->children[i]);
    }
  }
  xfree(root->trace);
  xfree(root);
}

size_t internal_trie_size(const void* ptr) {
  return 0; // Fetch size with O(n) algorithm is too costly.
}
