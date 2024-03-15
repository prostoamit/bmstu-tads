#include "bs_tree.h"

struct bs_node {
    bs_tree_type_t data;

    bs_node_t *left;
    bs_node_t *right;
};

bs_node_t *bs_node_new();
void bs_node_deep_delete(bs_node_t *node);

bs_tree_t *bs_tree_new() {
    bs_tree_t *tree = NULL;
    if (my_calloc((void *)&(tree), 1, sizeof(bs_tree_t *)) != 0)
        return NULL;
    return tree;
}

void bs_tree_delete(bs_tree_t *tree) {
    bs_tree_clear(tree);

    my_free(tree);    
}

void bs_tree_clear(bs_tree_t *tree) {
    if (tree == NULL || *tree == NULL)
        return;

    bs_node_deep_delete(*tree);

    *tree = NULL;
}

void bs_tree_pre_order(bs_tree_t *tree, void (*function)(void *data, void *param), void *param) {
    if (tree == NULL || *tree == NULL)
        return;
    
    function((*tree)->data, param);
    bs_tree_pre_order(&((*tree)->left), function, param);
    bs_tree_pre_order(&((*tree)->right), function, param);
}

void bs_tree_in_order(bs_tree_t *tree, void (*function)(void *data, void *param), void *param) {
    if (tree == NULL || *tree == NULL)
        return;
    
    bs_tree_in_order(&((*tree)->left), function, param);
    function((*tree)->data, param);
    bs_tree_in_order(&((*tree)->right), function, param);
}

void bs_tree_post_order(bs_tree_t *tree, void (*function)(void *data, void *param), void *param) {
    if (tree == NULL || *tree == NULL)
        return;
    
    bs_tree_post_order(&((*tree)->left), function, param);
    bs_tree_post_order(&((*tree)->right), function, param);
    function((*tree)->data, param);
}

int bs_tree_is_empty(bs_tree_t *tree) {
    return *tree == NULL;
}

int bs_tree_insert(bs_tree_t *tree, bs_tree_type_t value, bs_comparator_f comparator) {
    if (tree == NULL)
        return 1;

    bs_node_t **current = tree;
    while (*current != NULL) {
        int rc = comparator(value, (*current)->data);
        if (rc < 0)
            current = &(*current)->left;
        else if (rc > 0)
            current = &(*current)->right;
        else
            return 0 ;
    }

    *current = bs_node_new();
    (*current)->data = value;
    if (current == NULL)
        return 2;

    return 0;
}

bs_node_t *bs_node_remove(bs_node_t *root, bs_tree_type_t key, bs_comparator_f comparator) {
    if (root == NULL)
        return root;

    int cmp = comparator(key, root->data);

    if (cmp < 0)
        root->left = bs_node_remove(root->left, key, comparator);
    else if (cmp > 0)
        root->right = bs_node_remove(root->right, key, comparator);
    else {
        if (root->left == NULL) {
            bs_node_t* tmp = root->right;
            free(root);
            return tmp;
        }
        else if (root->right == NULL) {
            bs_node_t* tmp = root->left;
            free(root);
            return tmp;
        }

        bs_node_t *tmp = root->right;

        while (tmp->left != NULL)
            tmp = tmp->left;

        root->data = tmp->data;
        root->right = bs_node_remove(root->right, tmp->data, comparator);
    }
    return root;
}


int bs_tree_remove(bs_tree_t *tree, bs_tree_type_t value, bs_comparator_f comparator) {
    if (tree == NULL)
        return 1;
    
    *tree = bs_node_remove(*tree, value, comparator);
    return 0;
}

bs_tree_type_t bs_node_search(bs_node_t *node, bs_tree_type_t value, bs_comparator_f comparator) {
    if (node == NULL)
        return NULL;
    
    int rc = comparator(value, node->data);
    if (rc == 0)
        return node->data;
    
    if (rc > 0)
        return bs_node_search(node->right, value, comparator);
    
    return bs_node_search(node->left, value, comparator);
}

bs_tree_type_t bs_tree_search(bs_tree_t *tree, bs_tree_type_t value, bs_comparator_f comparator) {
    return bs_node_search(*tree, value, comparator);
}

void bs_node_to_dot(FILE *stream, bs_node_t *node) {
    if (node == NULL)
        return;
    if (node->left != NULL) {
        fprintf(stream, "\t\"%s\" -> \"%s\" [color=\"blue\"]\n", node->data, node->left->data);
        bs_node_to_dot(stream, node->left);
    }
    if (node->right != NULL) {
        fprintf(stream, "\t\"%s\" -> \"%s\" [color=\"red\"]\n", node->data, node->right->data);
        bs_node_to_dot(stream, node->right);
    }
}


bs_tree_type_t bs_node_search_comparisons(bs_node_t *node, bs_tree_type_t value, bs_comparator_f comparator, int *comparisons) {
    if (node == NULL)
        return NULL;
    
    int rc = comparator(value, node->data);
    (*comparisons)++;
    if (rc == 0)
        return node->data;
    
    if (rc > 0)
        return bs_node_search_comparisons(node->right, value, comparator, comparisons);
    
    return bs_node_search_comparisons(node->left, value, comparator, comparisons);
}

bs_tree_type_t bs_tree_search_comparisons(bs_tree_t *tree, bs_tree_type_t value, bs_comparator_f comparator, int *comparisons) {
    *comparisons = 0;
    return bs_node_search_comparisons(*tree, value, comparator, comparisons);
}


void bs_tree_to_dot(FILE *stream, bs_tree_t *tree) {
    fprintf(stream, "digraph tree {\n");

    bs_node_to_dot(stream, *tree);

    fputs("}\n", stream);
}


size_t bs_node_size() {return sizeof(bs_node_t);}

size_t __bs_height(bs_node_t *node) {
    if (node == NULL)
        return 0;

    size_t h_l = __bs_height(node->left);
    size_t h_r = __bs_height(node->right);

    return h_l > h_r ? h_l + 1 : h_r + 1;
}

size_t bs_height(bs_tree_t *tree) {
    return __bs_height(*tree);
}

bs_node_t *bs_node_new() {
    bs_node_t *node = NULL;
    if (my_calloc((void *)&node, 1, sizeof(sizeof(bs_node_t))) != 0)
        return NULL;
    
    return node;
}

void bs_node_deep_delete(bs_node_t *node) {
    if (node == NULL)
        return;
    
    bs_node_deep_delete(node->left);
    bs_node_deep_delete(node->right);

    my_free(node);
}
