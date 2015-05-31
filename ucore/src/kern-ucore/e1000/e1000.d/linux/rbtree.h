#ifndef __LINUX_RBTREE_H__
#define __LINUX_RBTREE_H__

#include <linux/kernel.h>
#include <linux/stddef.h>

struct rb_node {
	unsigned long  __rb_parent_color;
	struct rb_node *rb_right;
	struct rb_node *rb_left;
} __attribute__((aligned(sizeof(long))));

struct rb_root {
	struct rb_node *rb_node;
};

#define RB_ROOT	(struct rb_root) { NULL, }

#define RB_EMPTY_ROOT(root)  ((root)->rb_node == NULL)

#define RB_CLEAR_NODE(node)  \
	((node)->__rb_parent_color = (unsigned long)(node))

#endif /* ! __LINUX_RBTREE_H__ */
