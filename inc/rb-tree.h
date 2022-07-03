#ifndef __RB_TREE_H__
#define __RB_TREE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

typedef enum { rbt_red = 0, rbt_black = 1 } rbt_color;

typedef enum {
    rbt_status_success = 0,
    rbt_status_memory_out = -1,
    rbt_status_key_duplicate = -2,
    rbt_status_key_not_exist = -3
} rbt_status;

struct rbt_tree;
struct rbt_node;

int rbt_node_is_valid(const struct rbt_node* node);
rbt_color rbt_node_get_color(const struct rbt_node* node);
struct rbt_node* rbt_node_get_left(const struct rbt_node* node);
struct rbt_node* rbt_node_get_right(const struct rbt_node* node);
struct rbt_node* rbt_node_get_parent(const struct rbt_node* node);
const void* rbt_node_get_key(const struct rbt_node* node);

typedef void* (*rbt_mem_allocate)(size_t size);
typedef void (*rbt_mem_release)(void* ptr);

typedef void (*rbt_node_destruct)(void*);
typedef int (*rbt_node_compare)(const void*, const void*);

struct rbt_tree* rbt_tree_create(rbt_mem_allocate allocator,
                                 rbt_mem_release releaser, int allow_dup,
                                 rbt_node_compare cmp, rbt_node_destruct dest);
struct rbt_node* rbt_tree_get_root(struct rbt_tree* tree);
rbt_status rbt_tree_insert(struct rbt_tree* tree, void* key, size_t size);
struct rbt_node* rbt_tree_find(struct rbt_tree* tree, const void* key);
rbt_status rbt_tree_remove_node(struct rbt_tree* tree, const void* key);
rbt_status rbt_tree_destroy(struct rbt_tree* tree);
int rbt_tree_is_empty(struct rbt_tree* tree);
struct rbt_node* rbt_tree_minimum(struct rbt_tree* tree, struct rbt_node* x);
struct rbt_node* rbt_tree_maximum(struct rbt_tree* tree, struct rbt_node* x);
struct rbt_node* rbt_tree_successor(struct rbt_tree* tree, struct rbt_node* x);

typedef void (*rbt_node_walk_cb)(struct rbt_node* x, void* p);
void rbt_inorder_walk(struct rbt_tree* tree, rbt_node_walk_cb cb, void* p);

#ifdef __cplusplus
}
#endif

#endif /* __RB_TREE_H__ */
