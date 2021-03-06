#ifndef STREE_H
#define STREE_H
#ifdef __cplusplus
extern "C" {
#endif

/*
 * stree.h
 *
 * Self-balancing sorted binary tree.
 *
 * Copyright (c) 2015-2016, F. Aragon. All rights reserved. Released under
 * the BSD 3-Clause License (see the doc/LICENSE file included).
 *
 * Features:
 * - See documentation for API reference.
 */

#include "sdata.h"

/*
 * Structures and types
 */

#define ST_NODE_BITS 31
#define ST_NIL	((((uint32_t)1)<<ST_NODE_BITS) - 1)

typedef uint32_t stndx_t;

typedef int (*st_cmp_t)(const void *tree_node, const void *new_node);
typedef void(*stn_callback_t)(void *tree_node);

struct S_Node
{
	struct {
		stndx_t is_red : 1;
		stndx_t l : ST_NODE_BITS;
	} x;
	stndx_t r;
};

struct S_Tree
{
	struct SDataFull d;
	stndx_t root;
	st_cmp_t cmp_f;
};

typedef struct S_Node stn_t;
typedef struct S_Tree st_t;

struct STraverseParams
{
	void *context;
	const st_t *t;
	stndx_t c;
	const stn_t *cn;
	ssize_t level;
	ssize_t max_level;
};

typedef int (*st_traverse)(struct STraverseParams *p);
typedef void (*st_rewrite_t)(const st_t *t, stn_t *node, const stn_t *new_data);

/*
* Constants
*/
/*
#define EMPTY_STC { 0, 0, 0, 0, 0, 0, 0 }
#define EMPTY_ST { EMPTY_SData_Full(sizeof(st_t)), 0, EMPTY_STC }
*/
#define EMPTY_STN { { 0, ST_NIL }, ST_NIL }

/*
 * Functions
 */

/*
#API: |Allocate tree (stack)|node compare function; node size; space preallocated to store n elements|allocated tree|O(1)|0;1|
sv_t *st_alloca(st_cmp_t cmp_f, const size_t elem_size, const size_t max_size)
*/
#define st_alloca(cmp_f, elem_size, max_size)				       \
	st_alloc_raw(cmp_f, S_TRUE,					       \
		     alloca(sd_alloc_size(sizeof(st_t), elem_size, max_size)), \
		     elem_size, max_size)

st_t *st_alloc_raw(st_cmp_t cmp_f, const sbool_t ext_buf,
		   void *buffer, const size_t elem_size, const size_t max_size);

/* #API: |Allocate tree (heap)|compare function;element size;space preallocated to store n elements|allocated tree|O(1)|1;2| */
st_t *st_alloc(st_cmp_t cmp_f, const size_t elem_size, const size_t init_size);

SD_BUILDFUNCS_FULL(st)

/*
#API: |Free one or more trees (heap)|tree;more trees (optional)|-|O(1)|1;2|
void st_free(st_t **c, ...)

#API: |Ensure space for extra elements|tree;number of extra eelements|extra size allocated|O(1)|0;1|
size_t st_grow(st_t **c, const size_t extra_elems)

#API: |Ensure space for elements|tree;absolute element reserve|reserved elements|O(1)|0;1|
size_t st_reserve(st_t **c, const size_t max_elems)

#API: |Free unused space|tree|same tree (optional usage)|O(1)|0;1|
st_t *st_shrink(st_t **c)

#API: |Get tree size|tree|number of tree nodes|O(1)|0;1|
size_t st_size(const st_t *c)

#API: |Set tree size (for integer-only trees) |tree;set tree number of elements|-|O(1)|0;1|
void st_set_size(st_t *c, const size_t s)

#API: |Equivalent to st_size|tree|number of tree nodes|O(1)|1;2|
size_t st_len(const st_t *c)
*/

#define st_free(...) st_free_aux(S_NARGS_STPW(__VA_ARGS__), __VA_ARGS__)

/*
 * Operations
 */

/* #API: |Duplicate tree|tree|output tree|O(n)|0;1| */
st_t *st_dup(const st_t *t);

/* #API: |Insert element into tree|tree; element to insert|S_TRUE: OK, S_FALSE: error (not enough memory)|O(log n)|1;2| */
sbool_t st_insert(st_t **t, const stn_t *n);

/* #API: |Insert element into tree, with rewrite function (in case of key already written)|tree; element to insert; rewrite function (if NULL it will behave like st_insert()|S_TRUE: OK, S_FALSE: error (not enough memory)|O(log n)|1;2| */
sbool_t st_insert_rw(st_t **t, const stn_t *n, const st_rewrite_t rw_f);

/* #API: |Delete tree element|tree; element to delete; node delete handling callback (optional if e.g. nodes use no extra dynamic memory references)|S_TRUE: found and deleted; S_FALSE: not found|O(log n)|1;2| */
sbool_t st_delete(st_t *t, const stn_t *n, stn_callback_t callback);

/* #API: |Locate node|tree; node|Reference to the located node; NULL if not found|O(log n)|1;2| */
const stn_t *st_locate(const st_t *t, const stn_t *n);

/* #API: |Fast unsorted enumeration|tree; element, 0 to n - 1, being n the number of elements|Reference to the located node; NULL if not found|O(1)|0;1| */
stn_t *st_enum(st_t *t, const stndx_t index);

/* #API: |Fast unsorted enumeration (read-only)|tree; element, 0 to n - 1, being n the number of elements|Reference to the located node; NULL if not found|O(1)|0;1| */
const stn_t *st_enum_r(const st_t *t, const stndx_t index);

/* #API: |Full tree traversal: pre-order|tree; traverse callback; callback context|Number of levels stepped down|O(n)|1;2| */
ssize_t st_traverse_preorder(const st_t *t, st_traverse f, void *context);

/* #API: |Full tree traversal: in-order|tree; traverse callback; callback context|Number of levels stepped down|O(n)|1;2| */
ssize_t st_traverse_inorder(const st_t *t, st_traverse f, void *context);

/* #API: |Full tree traversal: post-order|tree; traverse callback; callback context|Number of levels stepped down|O(n)|1;2| */
ssize_t st_traverse_postorder(const st_t *t, st_traverse f, void *context);

/* #API: |Bread-first tree traversal|tree; traverse callback; callback contest|Number of levels stepped down|O(n); Aux space: n/2 * sizeof(stndx_t)|1;2| */
ssize_t st_traverse_levelorder(const st_t *t, st_traverse f, void *context);

/*
 * Other
 */

/* #API: |Tree check (debug purposes)|tree|S_TREE: OK, S_FALSE: breaks RB tree rules|O(n)|1;2| */
sbool_t st_assert(const st_t *t);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* #ifndef STREE_H */

