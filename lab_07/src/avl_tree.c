#include "avl_tree.h"

#include "mem_manager.h"

struct avl_node {
    avl_tree_type_t data;

    avl_node_t *right;
    avl_node_t *left;

    unsigned int height;
};

avl_node_t *__node_new();
void __node_deep_delete(avl_node_t *node);

avl_node_t *__l_rotation(avl_node_t *node);
avl_node_t *__r_rotation(avl_node_t *node);

avl_node_t *__node_min(avl_node_t *node);
avl_node_t *__node_max(avl_node_t *node);

unsigned int __get_height(avl_node_t *node) {
    if (node == NULL)
        return 0;
    return node->height;
}

int __get_balance(avl_node_t *node) {
    if (node == NULL)
        return 0;
    
    return __get_height(node->left) - __get_height(node->right);
}

unsigned int __uint_max(unsigned int l, unsigned int r) {return l >= r ? l : r;}

avl_tree_t *avl_tree_new() {
    return (avl_tree_t *)calloc(1, sizeof(avl_tree_t *));
}

void avl_tree_delete(avl_tree_t *tree) {
    if (tree == NULL)
        return;

    avl_tree_clear(tree);
    my_free(tree);
}

void avl_tree_clear(avl_tree_t *tree) {
    if (tree == NULL || *tree == NULL)
        return;

    __node_deep_delete(*tree);

    *tree = NULL;
}

int avl_tree_is_empty(avl_tree_t *tree) {
    return *tree == NULL;
}

size_t avl_node_size() {return sizeof(avl_node_t);}

size_t __height(avl_node_t *node) {
    if (node == NULL)
        return 0;

    size_t h_l = __height(node->left);
    size_t h_r = __height(node->right);

    return h_l > h_r ? h_l + 1 : h_r + 1;
}

size_t avl_height(avl_tree_t *tree) {
    return __height(*tree);
}

avl_node_t *__insert(avl_node_t *node, avl_node_t *to_insert, avl_comparator_f comparator) {

    if (node == NULL)
        return to_insert;

    int cmp = comparator(to_insert->data, node->data);
    if (cmp < 0) {
        node->left = __insert(node->left, to_insert, comparator);
    }
    if (cmp > 0)
        node->right = __insert(node->right, to_insert, comparator);

    node->height = 1 + __uint_max(__get_height(node->left), __get_height(node->right));

    int balance_factor = __get_balance(node);

    if (balance_factor > 1) {
        cmp = comparator(to_insert->data, node->left->data);
        if (cmp < 0)
            return __r_rotation(node);
        if (cmp > 0) {
            node->left = __l_rotation(node->left);
            return __r_rotation(node);
        }
    }
    else if (balance_factor < -1) {
        cmp = comparator(to_insert->data, node->right->data);
        if (cmp > 0)
            return __l_rotation(node);
        if (cmp < 0) {
            node->right = __r_rotation(node->right);
            return __l_rotation(node);
        }
    }

    return node;
}

int avl_tree_insert(avl_tree_t *tree, avl_tree_type_t value, avl_comparator_f comparator) {
    avl_node_t *to_insert = __node_new();
    if (to_insert == NULL)
        return 1;
    to_insert->data = value;
    
    *tree = __insert(*tree, to_insert, comparator);
    return 0;
}

avl_node_t *__remove(avl_node_t *node, avl_tree_type_t key, avl_comparator_f comparator) {
    if (node == NULL)
        return NULL;
    
    int cmp = comparator(key, node->data);

    if (cmp < 0)
        node->left = __remove(node->left, key, comparator);
    else if (cmp > 0)
        node->right = __remove(node->right, key, comparator);
    else {
        if (node->left == NULL) {
            avl_node_t* tmp = node->right;
            free(node);
            return tmp;
        }
        else if (node->right == NULL) {
            avl_node_t* tmp = node->left;
            free(node);
            return tmp;
        }

        avl_node_t *min_right = __node_min(node->right);

        node->data = min_right->data;
        node->right = __remove(node->right, min_right->data, comparator);
    }

    node->height = 1 + __uint_max(__get_height(node->left), __get_height(node->right));

    int balance_factor = __get_balance(node);

    if (balance_factor > 1) {
        if (__get_balance(node->left) >= 0)
            return __r_rotation(node);
        else {
            node->left = __l_rotation(node->left);
            return __r_rotation(node);
        }
    }
    if (balance_factor < -1) {
        if (__get_balance(node->right) <= 0)
            return __l_rotation(node);
        else {
            node->right = __r_rotation(node->right);
            return __l_rotation(node);
        }
    }

    return node;
}

int avl_tree_remove(avl_tree_t *tree, avl_tree_type_t value, avl_comparator_f comparator) {
    if (tree == NULL || *tree == NULL)
        return 1;
    
    *tree = __remove(*tree, value, comparator);
    return 0;
}

avl_tree_type_t __avl_node_search(avl_node_t *node, avl_tree_type_t value, avl_comparator_f comparator) {
    if (node == NULL)
        return NULL;
    
    int rc = comparator(value, node->data);
    if (rc == 0)
        return node->data;
    
    if (rc > 0)
        return __avl_node_search(node->right, value, comparator);
    
    return __avl_node_search(node->left, value, comparator);
}

avl_tree_type_t avl_tree_search(avl_tree_t *tree, avl_tree_type_t value, avl_comparator_f comparator) {
    return __avl_node_search(*tree, value, comparator);
}
avl_tree_type_t __avl_node_search_comparisons(avl_node_t *node, avl_tree_type_t value, avl_comparator_f comparator, int *comparisons) {
    if (node == NULL)
        return NULL;
    
    int rc = comparator(value, node->data);
    (*comparisons)++;
    if (rc == 0)
        return node->data;
    
    if (rc > 0)
        return __avl_node_search_comparisons(node->right, value, comparator, comparisons);
    
    return __avl_node_search_comparisons(node->left, value, comparator, comparisons);
}

avl_tree_type_t avl_tree_search_comparisons(avl_tree_t *tree, avl_tree_type_t value, avl_comparator_f comparator, int *comparisons) {
    *comparisons = 0;
    return __avl_node_search_comparisons(*tree, value, comparator, comparisons);
}


void avl_tree_pre_order(avl_tree_t *tree, void (*function)(void *data)) {
    if (tree == NULL || *tree == NULL)
        return;
    
    function((*tree)->data);
    avl_tree_pre_order(&((*tree)->left), function);
    avl_tree_pre_order(&((*tree)->right), function);
}

void avl_tree_in_order(avl_tree_t *tree, void (*function)(void *data)) {
    if (tree == NULL || *tree == NULL)
        return;
    
    avl_tree_in_order(&((*tree)->left), function);
    function((*tree)->data);
    avl_tree_in_order(&((*tree)->right), function);
}

void avl_tree_post_order(avl_tree_t *tree, void (*function)(void *data)) {
    if (tree == NULL || *tree == NULL)
        return;
    
    avl_tree_post_order(&((*tree)->left), function);
    avl_tree_post_order(&((*tree)->right), function);
    function((*tree)->data);
}

void __to_dot(FILE *stream, avl_node_t *node) {
    if (node == NULL)
        return;
    fprintf(stream, "\t\"%s\"\n", node->data);
    if (node->left != NULL) {
        fprintf(stream, "\t\"%s\" -> \"%s\" [color=\"blue\"]\n", node->data, node->left->data);
        __to_dot(stream, node->left);
    }
    if (node->right != NULL) {
        fprintf(stream, "\t\"%s\" -> \"%s\" [color=\"red\"]\n", node->data, node->right->data);
        __to_dot(stream, node->right);
    }
}

void avl_tree_to_dot(FILE *stream, avl_tree_t *tree) {
    fprintf(stream, "digraph avl_tree {\n");

    __to_dot(stream, *tree);

    fputs("}\n", stream);
}

avl_node_t *__l_rotation(avl_node_t *node) {
    avl_node_t *new_root = node->right;
    node->right = new_root->left;
    new_root->left = node;

    node->height = 1 + __uint_max(__get_height(node->left), __get_height(node->right));
    new_root->height = 1 + __uint_max(__get_height(new_root->left), __get_height(new_root->right));

    return new_root;
}

avl_node_t *__r_rotation(avl_node_t *node) {
    avl_node_t *new_root = node->left;
    node->left = new_root->right;
    new_root->right = node;
    
    node->height = 1 + __uint_max(__get_height(node->left), __get_height(node->right));
    new_root->height = 1 + __uint_max(__get_height(new_root->left), __get_height(new_root->right));

    return new_root;
}

avl_node_t *__node_min(avl_node_t *node) {
    if (node == NULL)
        return NULL;
    
    return node->left == NULL ? node : __node_min(node->left);
}

avl_node_t *__node_max(avl_node_t *node) {
    if (node == NULL)
        return NULL;
    
    return node->right == NULL ? node : __node_max(node->right);
}

avl_node_t *__node_new() {
    avl_node_t *node = NULL;
    if (my_calloc((void *)&node, 1, sizeof(avl_node_t)) != 0)
        return NULL;
    
    node->height = 1;
    
    return node;
}

void __node_deep_delete(avl_node_t *node) {
    if (node == NULL)
        return;
    
    __node_deep_delete(node->left);
    __node_deep_delete(node->right);

    my_free(node);
}
